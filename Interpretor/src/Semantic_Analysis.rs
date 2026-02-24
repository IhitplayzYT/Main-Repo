// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Semantic_Analysis.rs    //
// Contains the code for the Semantic Analyser


#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals,unused_imports,dead_code)]
pub mod Analyser {
    use std::collections::HashMap;
    use crate::{Ast::{self, AST::*},Errors::Err::{Parser_ret, Semantic_Ret, Semantic_err}};

    #[derive(Debug,Clone)]
    pub struct Semantilizer {
        pub symbol_table: HashMap<String,Type>,
        pub function_map: HashMap<String,(Vec<Type>,Option<Type>)>,
        pub in_loop : usize,
    }


    impl Semantilizer{
        pub fn new() -> Self{
            Self{symbol_table:HashMap::new(),
                function_map: HashMap::new(),
                in_loop: 0,
            }}

        pub fn analyse(&mut self,code:&Code) -> Semantic_Ret<()>{
            for decl in &code.Program{
                self.save_declares(decl)?;
            }

            for decl in &code.Program{
                self.analyze_decl(decl)?;
            }
            Ok(())
        }


       pub fn save_declares(&mut self,decl: &Declare) -> Semantic_Ret<()>{
            match decl {
                Declare::Function { name, rtype, args, ..} => {
                    let param_types: Vec<Type> = args.iter().map(|(_,t)| {t.clone()}).collect();
                    self.function_map.insert(name.clone(),(param_types,rtype.clone()));
                }
            }
        Ok(())
       }


       pub fn analyze_decl(&mut self,decl: &Declare) -> Semantic_Ret<()>{
            match decl {
                Declare::Function {args, body, ..} => {
                    self.symbol_table.clear();
                    for  (name,var_type) in args {
                        self.symbol_table.insert(name.clone(),var_type.clone());
                    }
                    for stmt in body {
                        self.analyze_stmt(stmt)?;

                    }

                }
            }

        Ok(())
       }


       pub fn analyze_stmt(&mut self,stmt: &Statmnt) -> Semantic_Ret<()> {
            match stmt {
                Statmnt::Let { name, type_annot, value,.. } => {
                    let rhs_type = self.eval_type(value)?;
                    if let Some(t) = type_annot {
                        if !self.is_compatible(t,&rhs_type) {
                            return Err(Semantic_err::TypeMismatch { expected: t.clone(), got: rhs_type });
                        }

                    }
                    if self.symbol_table.contains_key(name) {
                        return Err(Semantic_err::Reassignment(name.clone()))
                    }
                    self.symbol_table.insert(name.clone(), rhs_type);
                },
                Statmnt::Assignment { name, val,.. } => {
                    if !self.symbol_table.contains_key(name){
                        return Err(Semantic_err::UndefinedVariable(name.clone()));
                    }
                    let var_type = self.symbol_table.get(name).unwrap();
                    let val_type = self.eval_type(val)?;

                    if !self.is_compatible(var_type,&val_type){
                        return Err(Semantic_err::TypeMismatch { expected: var_type.clone(), got: val_type });                        
                    }
                }
                Statmnt::If { cond, then_branch, else_branch } => {
                    let cond_type = self.eval_type(cond)?;
                    if !self.is_compatible(&Type::BOOL, &cond_type){
                        return Err(Semantic_err::TypeMismatch { expected: Type::BOOL, got: cond_type });
                    }

                    for i in then_branch{
                        self.analyze_stmt(i)?;
                    }
                    if let Some(other) = else_branch {
                        for i in other{
                            self.analyze_stmt(i)?;
                        }

                    }

                },
                Statmnt::While { cond, body } => {
                    let cond_type = self.eval_type(cond)?;
                    if !self.is_compatible(&Type::BOOL, &cond_type){
                        return Err(Semantic_err::TypeMismatch { expected: Type::BOOL, got: cond_type });
                    }
                    self.in_loop += 1;
                    for i in body{
                        self.analyze_stmt(i)?;
                    }
                    self.in_loop -= 1;
                },

                Statmnt::For { var_name, lb, rb, body } => {
                    self.eval_type(lb)?;
                    self.eval_type(rb)?;
                    self.symbol_table.insert(var_name.clone(), Type::INT);
                    self.in_loop += 1;
                    for i in body{
                        self.analyze_stmt(i)?;
                    }
                    self.in_loop -= 1;
                },
                Statmnt::Loop { body } => {
                    for i in body{
                        self.analyze_stmt(i)?;
                    }
                },
                Statmnt::Break => {
                    if self.in_loop == 0 {
                        return Err(Semantic_err::Break_Continue_location);
                    }
                },
                Statmnt::Continue => {
                    if self.in_loop == 0 {
                        return Err(Semantic_err::Break_Continue_location);
                    }
                }
                Statmnt::Return(x) => {
                    if let Some(val)  = x {
                        self.eval_type(val)?;
                    }
                },
                Statmnt::Expr(x) => {
                    self.eval_type(x)?;
                },
                Statmnt::Block(blk) => {
                    for i in blk {
                        self.analyze_stmt(i)?;
                    }
                }

            }

            Ok(())
       }
      pub fn eval_type(&self, expr: &Expr) -> Semantic_Ret<Type>{
        match expr {
            Expr::Bool(_)  => Ok(Type::BOOL),
            Expr::Float(_)  => Ok(Type::FLOAT),
            Expr::Int(_)  => Ok(Type::INT),
            Expr::String(_)  => Ok(Type::STRING),
            Expr::Ident(name) => self.symbol_table.get(name).cloned().ok_or_else(|| Semantic_err::UndefinedVariable(name.clone())),
            Expr::Binary_op { op, left, right } => {
                let (l_type,r_type) = (self.eval_type(left)?,self.eval_type(right)?);
                match op {
                    BIN_OP::Add | BIN_OP::Sub | BIN_OP::Mul | BIN_OP::Div | BIN_OP::Mod | BIN_OP::Amp | BIN_OP::Lshift | BIN_OP::Pipe | BIN_OP::Rshift | BIN_OP::Xor => {
                        if self.is_compatible(&l_type, &r_type) {
                            Ok(l_type)
                        } else{
                            Err(Semantic_err::TypeMismatch { expected: l_type, got:r_type })
                        } 
                    },
                    BIN_OP::Eq | BIN_OP::N_eq | BIN_OP::Lt | BIN_OP::Lt_eq | BIN_OP::Gt | BIN_OP::Gt_eq => Ok(Type::BOOL),
                    BIN_OP::And | BIN_OP::Or => {
                        if !self.is_compatible(&Type::BOOL, &l_type) {
                            return Err(Semantic_err::TypeMismatch { expected: Type::BOOL, got: l_type });
                        }
                        if !self.is_compatible(&Type::BOOL, &r_type){
                            return Err(Semantic_err::TypeMismatch { expected: Type::BOOL, got: r_type });
                        }

                        Ok(Type::BOOL)
                    }
                }
            }
            Expr::Unary_op { op, operand } => {
                let operant_type = self.eval_type(operand)?;
                match op {
                    UN_OP::Bang => Ok(Type::BOOL),
                    UN_OP::Neg => Ok(operant_type),
                    UN_OP::Tilda => Ok(Type::INT),
                }
            },
            Expr::Fxn_call { name, args } => {
                let (param,ret_type) = self.function_map.get(name).ok_or_else(|| Semantic_err::UndefinedFunction(name.clone()))?;
                if args.len() != param.len() {
                    return Err(Semantic_err::Custom(format!("Invalid number of arguments provided for Function {:?} expected:{:?} got {:?}",name,param.len(),args.len())));
                }
                for (arg,param_t) in args.iter().zip(param.iter()){
                    let arg_t = self.eval_type(arg)?;
                    if !self.is_compatible(param_t,&arg_t){
                        return Err(Semantic_err::TypeMismatch { expected: param_t.clone(), got: arg_t });
                    }
                }

                Ok(ret_type.clone().unwrap_or(Type::INT))
            },

            _ => Ok(Type::INT),
        }

        }


       pub fn is_compatible(&self,t1: &Type,t2: &Type) -> bool{
            t1 == t2
       }

       



    }
}
