// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]
pub mod PARSER {

    use crate::{Ast::{AST::{Code,Type,Declare,Statmnt,Expr,BIN_OP,UN_OP}},Lexer_Tok::Lex_Tok::LTOK};
    use crate::Errors::Err::{ParserError,Parser_ret};
    pub struct Parser {
        pub input: Vec<LTOK>,
        pub idx:usize,
    }
    impl Parser{
        /* ******************************** CONSTRUCTOR ********************************  */
        pub fn new(v:Vec<LTOK>) -> Self{
            Self{input:v,idx:0}
        }
        /* ******************************** CONSTRUCTOR ********************************  */


        /* ******************************** MAIN ********************************  */
        pub fn Parse(&mut self) -> Parser_ret<Code> {         
            let mut ret =  Vec::new();
            while !self.check(&LTOK::EOF){
                ret.push(self.eval_declare()?);
            }
            Ok(Code { Program: ret })
        }
        /* ******************************** MAIN ********************************  */


        /* ******************************** HELPER ********************************  */
        fn next(&mut self) -> LTOK{
            if self.input.is_empty() || self.idx == self.input.len() {
                return LTOK::EOF;
            }
            self.idx += 1;
            self.peek().clone()}


        fn peek(&self) -> &LTOK{
            if self.input.is_empty() || self.idx == self.input.len() {
                return &LTOK::EOF;
            }
            self.input.get(self.idx).unwrap_or(&LTOK::EOF)}

        fn check(&mut self,e: &LTOK) -> bool{std::mem::discriminant(self.peek()) == std::mem::discriminant(e)}


        fn match_token(&mut self,token:&[LTOK]) -> bool{
            for i in token{
                if self.check(i){
                    self.next();
                    return true;
                }
            }
            false
        }


        fn consume(&mut self,e: &LTOK) -> Parser_ret<LTOK>{
            if self.check(e) {
                return Ok(self.next());
            }
            Err(ParserError::UnexpectedToken { expected:format!("{:?}",e.clone()), got:format!("{:?}",self.peek().clone()) })
        }

        /* ******************************** HELPER ********************************  */

        
        /* ******************************** FUNCTIONS ********************************  */
        fn eval_declare(&mut self) -> Parser_ret<Declare>{
            match self.peek(){
            LTOK::FN => self.eval_fxn(),
                _ => {return Err(ParserError::Invalid_Code);}
            }

        }         

        fn eval_fxn(&mut self) -> Parser_ret<Declare>{
        self.consume(&LTOK::FN)?;

        let name = match self.next(){
            LTOK::IDENT(x) => x,
            tok => {return Err(ParserError::UnexpectedToken{expected:"Function name".to_string(),got:format!("{:?}",tok)});}};

        self.consume(&LTOK::LPAREN)?;

        let param = self.eval_params()?;

        self.consume(&LTOK::RPAREN)?;

        let rtype = 
            if self.match_token(&[LTOK::ARROW]) {Some(self.eval_type().unwrap())}
            else{None};

        let body = self.eval_block()?;
        self.consume(&LTOK::RBRACE)?;
        return Ok(Declare::Function { name:name, rtype: rtype, args: param, body: body });
        }

        fn eval_params(&mut self) -> Parser_ret<Vec<(String,Type)>>{
        let mut ret:Vec<(String,Type)> = Vec::new();
        if !self.check(&LTOK::RPAREN) {
            loop{
                let name = match self.next() {
                    LTOK::IDENT(x) => x,
                    t => {return Err(ParserError::UnexpectedToken { expected: "Identifier".to_string(), got: format!("{:?}", t) })}
                };
                self.consume(&LTOK::COLON)?;
                let type_varib = self.eval_type()?;
                ret.push((name,type_varib));
                if !self.match_token(&[LTOK::COMMA]){break;}
            }
        }
        Ok(ret)
        }
        /* ******************************** FUNCTIONS ********************************  */


        /* ******************************** LET & CONST ********************************  */

        fn eval_let(&mut self) -> Parser_ret<Statmnt> {
        self.consume(&LTOK::LET)?;
        let mutable = self.match_token(&[LTOK::MUT]);
        let name: String = match self.next() {
            LTOK::IDENT(x) => x,
            t => return Err(ParserError::UnexpectedToken { expected: "Identifier".to_string(), got:  format!("{:?}",t)})
        };
        let annot = if self.match_token(&[LTOK::COLON]) {
            Some(self.eval_type()?)
        }
        else{
            None
        };

        self.consume(&LTOK::ASSGN)?;
        let val = self.eval_expr()?;
        self.consume(&LTOK::SEMICOLON)?;
        Ok(Statmnt::Let { name, mutable, type_annot:annot, value: val })
        }

        fn eval_const(&mut self) -> Parser_ret<Statmnt> {
            self.consume(&LTOK::CONST)?;
            let name: String = match self.next() {
            LTOK::IDENT(x) => x,
            t => return Err(ParserError::UnexpectedToken { expected: "Identifier".to_string(), got:  format!("{:?}",t)})
            };
            let annot = if self.match_token(&[LTOK::COLON]) {
                Some(self.eval_type()?)
            }
            else{
                None
            };
            self.consume(&LTOK::ASSGN)?;
            let val = self.eval_expr()?;
            self.consume(&LTOK::SEMICOLON)?;
            Ok(Statmnt::Let { name, mutable:true, type_annot:annot, value: val })
        }
   
        /* ******************************** LET & CONST ********************************  */

        /* ******************************** IF-ELSE ********************************  */

        fn eval_if_else(&mut self) -> Parser_ret<Statmnt>{
            self.consume(&LTOK::IF)?;
           
            let cond = self.eval_expr()?;
            self.consume(&LTOK::LBRACE)?;
            let then_branch = self.eval_block()?;
            self.consume(&LTOK::RBRACE)?;


            let else_branch = if self.match_token(&[LTOK::ELSE]) {
                if self.check(&LTOK::IF){
                    Some(vec![self.eval_if_else()?])
                }else{
                    self.consume(&LTOK::LBRACE)?;
                   let bl = self.eval_block()?;
                   self.consume(&LTOK::RBRACE)?;
                   Some(bl)
                }
                }else{
                    None
                };
                       
            Ok(Statmnt::If { cond , then_branch, else_branch })
        }

        /* ******************************** IF-ELSE ********************************  */



        /* ******************************** FOR-WHILE-LOOP ********************************  */

        fn eval_for(&mut self) -> Parser_ret<Statmnt>{
            self.consume(&LTOK::FOR)?;
            let var_name = match self.next() {
                LTOK::IDENT(x) => x,
                _ => return Err(ParserError::Custom("Provide an identifier to store iterator results".to_string())),
            };
            self.consume(&LTOK::IN)?;
            let lb = self.eval_expr()?;
            let rb = self.eval_expr()?;
            self.consume(&LTOK::LBRACE)?;
            let body = self.eval_block()?;
            self.consume(&LTOK::RBRACE)?;   
            Ok(Statmnt::For { var_name, lb, rb, body })
        }

        fn eval_while(&mut self) -> Parser_ret<Statmnt> {
            self.consume(&LTOK::WHILE)?;
            let cond = self.eval_expr()?;
            self.consume(&LTOK::LBRACE)?;
            let body = self.eval_block()?;
            self.consume(&LTOK::RBRACE)?;
            Ok(Statmnt::While { cond, body })
        }

        fn eval_loop(&mut self) -> Parser_ret<Statmnt>{
        self.consume(&LTOK::LOOP)?;
        self.consume(&LTOK::LBRACE)?;
        let body = self.eval_block()?;
        self.consume(&LTOK::RBRACE)?;    
        Ok(Statmnt::Loop { body })
        }

        
        /* ******************************** FOR-WHILE-LOOP ********************************  */



        /* ******************************** HELPER ********************************  */
        
        fn eval_type(&mut self) -> Parser_ret<Type>{
            match self.next(){
                LTOK::INT_TYPE => Ok(Type::INT),
                LTOK::FLOAT_TYPE => Ok(Type::FLOAT),
                LTOK::STRING_TYPE => Ok(Type::STRING),
                z => Err(ParserError::UnexpectedToken { expected: "INT|FLOAT|STRING".to_string(), got:  format!("{:?}",z)})
            }
        }

        fn eval_return(&mut self) -> Parser_ret<Statmnt> {
            self.consume(&LTOK::RETURN)?;
            let val = match self.next() {
                LTOK::SEMICOLON => None,
                _ => Some(self.eval_expr()?),
            };

            Ok(Statmnt::Return(val))
        }

        fn eval_tuple_types(&mut self) -> Parser_ret<Vec<Type>>{
            let mut ret = Vec::new();
            while !self.check(&LTOK::LPAREN){
                ret.push(self.eval_type()?);
                if !self.match_token(&[LTOK::COMMA]){
                    break;
                }
            }
            Ok(ret)
        }

        /* ******************************** HELPER ********************************  */


    
    /* ******************************** BLOCKS & STATEMENTS ********************************  */

        fn eval_statmnt(&mut self) -> Parser_ret<Statmnt>{
            match self.peek() {
                LTOK::LET => {self.eval_let()},
                LTOK::CONST => {self.eval_const()},
                LTOK::IF  => {self.eval_if_else()},
                LTOK::WHILE => {self.eval_while()},
                LTOK::FOR => {self.eval_for()},
                
                LTOK::BREAK => {
                self.next();
                self.consume(&LTOK::SEMICOLON);
                Ok(Statmnt::Break)
                },

                LTOK::CONTINUE =>{
                self.next();
                self.consume(&LTOK::SEMICOLON);
                Ok(Statmnt::Continue)
                },

                LTOK::RETURN => self.eval_return(),
                LTOK::LBRACE => {
                    self.next();
                    let blk = self.eval_block()?;
                    self.consume(&LTOK::RBRACE)?;
                    Ok(Statmnt::Block(blk))
                },
                _ => self.eval_assign(),

            }  
        }

        fn eval_block(&mut self) -> Parser_ret<Vec<Statmnt>>{
            let mut statmnts:Vec<Statmnt> = Vec::new();
            while !self.check(&LTOK::RBRACE) && !self.check(&LTOK::EOF){
                statmnts.push(self.eval_statmnt()?);
            }
            Ok(statmnts)
        }

    /* ******************************** BLOCKS & STATEMENTS ********************************  */

    /* ******************************** EXPRESSIONS ********************************  */

    fn eval_assign(&mut self) -> Parser_ret<Statmnt> {
        let expr = self.eval_expr()?;
        if let Some(op) = self.match_assignment(){
            if let Expr::Ident(name) = expr{
                let val = self.eval_expr()?;
                self.consume(&LTOK::SEMICOLON)?;
                return Ok(Statmnt::Assignment { name, op, val });
            } else{
            return Err(ParserError::Custom("Invalid assignment\n".to_string()))
            }
        }
        self.consume(&LTOK::SEMICOLON)?;
        Ok(Statmnt::Expr(expr))
   }


    fn eval_expr(&mut self) -> Parser_ret<Expr>{
        return self.eval_logical_or();
    }

    fn eval_logical_or(&mut self) -> Parser_ret<Expr>{
        let mut left = self.eval_logical_and()?;

        while self.match_token(&[LTOK::OROR]){
            let right = self.eval_logical_and()?;
            left = Expr::Binary_op{op:BIN_OP::Or, left:Box::new(left), right:Box::new(right) };
        }

        Ok(left)
    }
        
    fn eval_logical_and(&mut self) -> Parser_ret<Expr>{
        let mut left = self.eval_equality()?;

        while self.match_token(&[LTOK::ANDAND]){
            let right = self.eval_equality()?;
            left = Expr::Binary_op{op:BIN_OP::And, left:Box::new(left), right:Box::new(right) };
        }
        Ok(left)
    }


    fn eval_equality(&mut self) -> Parser_ret<Expr>{
        let mut left = self.eval_comparator()?;
        while let Some(op) = self.match_eq_neq(){
            let right = self.eval_comparator()?;
            left = Expr::Binary_op { op, left: Box::new(left), right:Box::new(right)};
        }
        Ok(left)
    }

    fn match_eq_neq(&mut self) -> Option<BIN_OP>{
        match self.peek() {
            LTOK::EQ => {
                self.next();
                return Some(BIN_OP::Eq);},
            LTOK::N_EQ => {self.next();
                return Some(BIN_OP::N_eq);},
            _ => {return None},
        };
    }

    fn eval_comparator(&mut self) -> Parser_ret<Expr>{
        let mut left = self.parse_term()?;
        while let Some(x) =self.match_comparison(){
            let right = self.parse_term()?;
            left = Expr::Binary_op { op:x, left: Box::new(left), right:Box::new(right)}
        };
        Ok(left)
    }

    fn match_comparison(&mut self) -> Option<BIN_OP>{
        match self.peek() {
            LTOK::LT => {self.next();Some(BIN_OP::Lt)},
            LTOK::LT_EQ => {self.next();Some(BIN_OP::Lt_eq)},
            LTOK::GT => {self.next();Some(BIN_OP::Gt)},
            LTOK::GT_EQ => {self.next();Some(BIN_OP::Gt_eq)},
            _ => None,
        }
    }

    fn match_assignment(&mut self) -> Option<Option<BIN_OP>>{
    match self.peek(){
        LTOK::ASSGN => {
            self.next();
            Some(None)
        },
        LTOK::S_PLUS => {
            self.next();
            Some(Some(BIN_OP::Add))
        },
        LTOK::S_MINUS => {
            self.next();
            Some(Some(BIN_OP::Sub))
        },    
        LTOK::S_MULT => {
            self.next();
            Some(Some(BIN_OP::Mul))
        },    
        LTOK::S_DIV => {
            self.next();
            Some(Some(BIN_OP::Div))
        },
        LTOK::S_MOD => {
            self.next();
            Some(Some(BIN_OP::Mod))
        },
        LTOK::S_AMP => {
            self.next();
            Some(Some(BIN_OP::Amp))
        },    
        LTOK::S_PIPE => {
            self.next();
            Some(Some(BIN_OP::Pipe))
        },    
        LTOK::S_CARET => {
            self.next();
            Some(Some(BIN_OP::Xor))
        },
        _ => None,
    }
    }



    fn parse_term(&mut self) ->Parser_ret<Expr>{
        let mut left = self.parse_factor()?;
        while let Some(op) = self.match_term_op(){
            let right = self.parse_factor()?;
            left = Expr::Binary_op { op, left: Box::new(left), right: Box::new(right) };
        }
        Ok(left)
    }

    fn match_term_op(&mut self) -> Option<BIN_OP>{
        match self.peek()  {
            LTOK::PLUS => {self.next();Some(BIN_OP::Add)},
            LTOK::MINUS => {self.next();Some(BIN_OP::Sub)},
            _ => None,
        }
    }

    fn parse_factor(&mut self) -> Parser_ret<Expr>{
        let mut left = self.eval_unary()?;
        while let Some(op) = self.match_factor_op(){
            let right = self.eval_unary()?;
            left = Expr::Binary_op { op, left: Box::new(left), right: Box::new(right)};
        }
        Ok(left)
    }

    fn match_factor_op(&mut self) -> Option<BIN_OP>{
        match self.peek(){
        LTOK::STAR => {self.next();Some(BIN_OP::Mul)},
        LTOK::DIV => {self.next();Some(BIN_OP::Div)},
        LTOK::MODULO => {self.next();Some(BIN_OP::Mod)},
        _ => None,
        }
    }


    fn eval_unary(&mut self) -> Parser_ret<Expr>{
        match self.peek(){
        LTOK::MINUS => {
            self.next();
            let operand = self.eval_unary()?;
            Ok(Expr::Unary_op { op:UN_OP::Neg, operand: Box::new(operand) })
        },
        LTOK::BANG => {
            self.next();
            let operand= self.eval_unary()?;
            Ok(Expr::Unary_op { op:UN_OP::Bang, operand: Box::new(operand) })
        },LTOK::TILDA => {
            self.next();
            let operand= self.eval_unary()?;
            Ok(Expr::Unary_op { op:UN_OP::Tilda, operand: Box::new(operand) })
        },
        _ => self.eval_fxn_call(),
        }    
    }

    fn eval_fxn_call(&mut self) -> Parser_ret<Expr>{
        let mut expr = self.eval_primary()?;
        loop {
            match self.peek() {
                LTOK::LPAREN => {
                    self.next();
                    let args = self.eval_args()?;
                    self.consume(&LTOK::RPAREN)?;
                    if let Expr::Ident(x) = expr{
                        expr = Expr::Fxn_call { name:x, args:args };
                    }else{
                        return Err(ParserError::Invalid_Code);
                    }

                }
                _ => break,
            }

        }
        Ok(expr)
    }

    fn eval_args(&mut self) -> Parser_ret<Vec<Expr>> {
        let mut ret = Vec::new();
        if !self.check(&LTOK::RPAREN){
            loop{
                ret.push(self.eval_expr()?);
                if !self.match_token(&[LTOK::COMMA]){
                    break;
                }
            }
        };
        Ok(ret)
    }
    fn eval_primary(&mut self) -> Parser_ret<Expr> {
        match self.next() {
            LTOK::INT(x) => Ok(Expr::Int(x)),
            LTOK::FLOAT(x) => Ok(Expr::Float(x)),
            LTOK::STRING(x) => Ok(Expr::String(x)),
            LTOK::LPAREN => {
                let expr = self.eval_expr()?;
                self.consume(&LTOK::RPAREN);
                Ok(expr)
            }
            _ => Err(ParserError::Invalid_Code),

        }
    }

   
    /* ******************************** EXPRESSIONS ********************************  */

    }

}
