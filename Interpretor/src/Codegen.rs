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
    use crate::{Ast::AST::{Code, Declare, Type}, Errors::Err::InterpretorReturn};
    use std::{collections::HashMap, fmt::Display};
    use crate::Errors::Err::*;

    type Codegen_result<T> = Result<T,CodegenErr>;


    #[derive(Debug,Clone)]
    struct Codegen {
        code: Code,
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
            code:ast,
            errs: false,
            env: Env::new()
        }
    }

    pub fn register(&self,decl: &Declare) -> InterpretorReturn<()> {
        Ok(())
    }

    pub fn Exec(&mut self) -> InterpretorReturn<bool>{
        for decl in &self.code.Program{
            self.register(decl)?;
        }        

        Ok(true)    
    }
    







    }






}
