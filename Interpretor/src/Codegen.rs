// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Tokeniser.rs    //
// Contains the code for the Codegenerator

#![allow(
    non_camel_case_types,
    non_snake_case,
    non_upper_case_globals,
    dead_code
)]
pub mod Codegen {
    use crate::{Ast::AST::{Code, Declare, Statmnt, Type}, Errors::Err::{self, InterpretorReturn}};
    use std::{collections::HashMap, fmt::Display};
    use crate::Errors::Err::*;

    type Codegen_result<T> = Result<T,CodegenErr>;


    #[derive(Debug,Clone)]
    struct Codegen {
        errs: bool,
        env: Env
    }

    #[derive(Debug,Clone)]
    struct Env{
       scopes: Scopes,
       func : HashMap<String,Declare> ,
       dtypes: HashMap<String,Vec<(String,Type)>>
    }

    pub enum CodegenErr{
        Fatal_err,
        Custom_err(String)
    }

    #[derive(Debug,Clone)]
    pub enum Val {
    Int(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Null,
    Custom(String,HashMap<String,Val>)
    }


    #[derive(Debug,Clone)]
    struct Scopes{
        scope: Vec<HashMap<String,(Val,bool)>>    
    }

    impl Display for Val{
     fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self{
        Val::Int(x) => {write!(f,"{x}")},
        Val::Float(x) => {write!(f,"{x}")},
        Val::String(x) =>  {write!(f,"{x}")},
        Val::Bool(x) =>  {write!(f,"{x}")},
        Val::Custom(x,_) =>  {write!(f,"<Object {x}>")},
        Val::Null =>  {write!(f,"Null")},
        }
     }

    }

    impl Scopes{ 
        fn new() -> Self{
           Self{
               scope:vec![HashMap::new()]
           }
        }

        fn push(&mut self) {
            self.scope.push(HashMap::new());
        }

        fn pop(&mut self) {
            self.scope.pop();
        }

        fn declare(&mut self,name: String,val: Val,mutable:bool) {
            self.scope.last_mut().unwrap().insert(name, (val,mutable));
        }

        fn is_declared(&self,name:String) -> bool{
           self.scope.iter().rev().any(|x| { x.contains_key(&name)}) 
        }

        fn get(&self,name: String) -> InterpretorReturn<Val>{
          for i in self.scope.iter().rev(){
            if let Some((ret,_)) = i.get(&name){
                return Ok(ret.clone());
            }
          } 
          Err(InterpretorError::UndefinedVariable(name))
        }

        fn set(&mut self,name: String, new:Val) -> InterpretorReturn<()>{
            for i in self.scope.iter_mut().rev(){
                if let Some((old,mutable)) = i.get_mut(&name) {
                    if !*mutable {
                        return Err(InterpretorError::ImmutableVariable(name));
                    }
                    *old = new;
                    return Ok(());
                }

            }
            Err(InterpretorError::UndefinedVariable(name))
        }



    } 



    #[derive(Debug,Clone)]
        pub enum ControlFlow{
        Return(Val),
        Break,
        Continue    
        }


        impl Env{
            fn new() -> Self{
                Self{
                    scopes: Scopes::new(),
                    func: HashMap::new(),
                    dtypes: HashMap::new(),
                }
            }
        }


    impl Codegen {

    pub fn new(ast:Code) -> Self{
        Self{
            errs: false,
            env: Env::new()
        }
    }

    pub fn register(&mut self,decl: &Declare) -> InterpretorReturn<()> {
        match decl{
            Declare::Function {name,..} => {
                self.env.func.insert(name.clone(),decl.clone());
            },
            Declare::Struct { name,fields} => {
                self.env.dtypes.insert(name.clone(), fields.clone());
            },
            Declare::Enum { name, variations } => {
                // TODO:
                // FIXME:
            },

        }
        Ok(())
    }

    pub fn Exec(&mut self,code:Code) -> InterpretorReturn<bool>{
        for decl in &code.Program{
            self.register(decl)?;
        }        

        let _start = self.env.func.get("main").ok_or_else(|| InterpretorError::FunctionNotFound("main".to_string()))?;
        if let Declare::Function {body ,..} = _start{
        let fn_body = body.clone();
        self.env.scopes.push();
        match self.exec_block(&body) {
            Ok(_) |  Err(InterpretorError::ControlFault(ControlFlow::Return(_))) => {},
            Err(e) => return Err(e),
        }
        self.env.scopes.pop();
        }
        Ok(true)    
    }
    


    pub fn exec_block(&self,block: &Vec<Statmnt>) -> InterpretorReturn<()>{
        block.iter().for_each(|statmnt| self.exec_statmnt(statmnt).unwrap());
        Ok(())
    }

    pub fn exec_statmnt(&self,statmnt: &Statmnt) -> InterpretorReturn<()>{
        match statmnt{
            Statmnt::Let { name, mutable, value,..} => {
                let val = self.eval_expr(value)?;
                self.env.scopes.declare(name.clone(), val, *mutable);
            },
            Statmnt::Assignment { target, op, val } => {
                let value = self.eval_expr(val)?;
                self.try_assign(target,op,value)?;
            },
            Statmnt::If { cond, then_branch, else_branch } => {
                let cbool = self.eval_expr(cond)?;
                self.env.scopes.push();
                if Self::is_truthy(&cbool){
                    then_branch.iter().for_each(|f| self.exec_statmnt(f).unwrap());
                }else{
                    if let Some(branch) = else_branch{
                        branch.iter().for_each(|f| self.exec_statmnt(f).unwrap());
                    }
                }
                self.env.scopes.pop();



            },
            Statmnt::For { var_name, lb, rb, body } => {
                let (l,r) = (self.eval_expr(lb)?,self.eval_expr(rb)?);
                let (l,r) = match (l,r) {
                    (Val::Int(a),Val::Int(b)) => (a,b),
                    _ => return Err(InterpretorError::TypeError("For loops only allow <INT> TYPE".to_string()))
                };
                for idx in l..r {
                    self.env.scopes.push();
                    self.env.scopes.declare(var_name.clone(), Val::Int(idx), false);
                    match self.exec_block(body){
                        Ok(_) => {},
                        Err(InterpretorError::ControlFault(ControlFlow::Continue)) => {self.env.scopes.pop();continue;},
                        Err(InterpretorError::ControlFault(ControlFlow::Break)) => {self.env.scopes.pop();break;},
                        Err(e) => {self.env.scopes.pop();return Err(e);}
                    }
                    self.env.scopes.pop();
                }


            },
            Statmnt::Loop { body } => {
            loop {
                self.env.scopes.push();
                match self.exec_block(body){
                    Ok(_) => {},
                    Err(InterpretorError::ControlFault(ControlFlow::Continue)) => {self.env.scopes.pop();continue;},
                    Err(InterpretorError::ControlFault(ControlFlow::Break)) => {self.env.scopes.pop();break;},
                    Err(e) => {self.env.scopes.pop();return Err(e);}

                }
                self.env.scopes.pop();
            }
            },
            Statmnt::While { cond, body } => {
                loop {
                let cbool = self.eval_expr(cond)?;
                if !Self::is_truthy(&cbool) {
                    break;
                }
                self.env.scopes.push();
                match self.exec_block(body){
                    Ok(_) => {},
                    Err(InterpretorError::ControlFault(ControlFlow::Continue)) => {self.env.scopes.pop();continue;},
                    Err(InterpretorError::ControlFault(ControlFlow::Break)) => {self.env.scopes.pop();break;},
                    Err(e) => {self.env.scopes.pop();return Err(e);}

                }
                self.env.scopes.pop();

                }

            },
            Statmnt::Return(v) => {
                let val = match v {
                    Some(e) => self.eval_expr(e)?,
                    None => Val::Null
                };
                return Err(InterpretorError::ControlFault(ControlFlow::Return(val)));
            },
            Statmnt::Break => return Err(InterpretorError::ControlFault(ControlFlow::Break)),
            Statmnt::Continue => return Err(InterpretorError::ControlFault(ControlFlow::Continue)),
            Statmnt::Block(blk ) => {
            self.env.scopes.push();
            let r = self.exec_block(blk)?;
            self.env.scopes.pop();
            r
            },
            Statmnt::Expr(e) => self.eval_expr(e)?,
        }


        Ok(())
    }

    pub fn is_truthy(v: Val) -> bool {
        match v {
           Val::Bool(true) => true,
           Val::Custom(_,f) => !f.is_empty(),
           Val::Int(x) => x != 0,
           Val::Float(y) => y != 0.0,
           Val::String(z) => z.len() > 0,
           _ => false
        }
    }


    }






}
