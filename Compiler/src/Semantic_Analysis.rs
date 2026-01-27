// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]
pub mod Analyser {
    use std::collections::HashMap;
    use crate::{Ast::AST::*,Parser::PARSER::Parser};

    pub enum Semantic_err{
        UndefinedVariable(String),
        TypeMismatch{expected: Type,got: Type},
        Break_Continue_location,
        UndefinedFunction(String),
        Argument_err{expected:String,got:String},
    }
    
    type Semantic_Ret<T> = Result<T,Semantic_err>;


    pub struct Semantilizer {
        symbol_table: HashMap<String,Type>,
        function_map: HashMap<String,(Vec<Type>,Option<Type>)>,
        in_loop : bool,
    }


    impl Semantilizer{
        pub fn new() -> Self{
            Self{symbol_table:HashMap::new(),
                function_map: HashMap::new(),
                in_loop: false,
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

       fn save_declares(&mut self,decl: &Declare) -> Semantic_Ret<()>{
        Ok(())
       }
       fn analyze_decl(&mut self,decl: &Declare) -> Semantic_Ret<()>{
        Ok(())
       }


    }
}
