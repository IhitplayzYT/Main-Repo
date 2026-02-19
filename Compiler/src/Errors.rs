// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.
// FIXME:TODO:     Docs left from here onwards
#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals,dead_code)]
pub mod Err{
    use crate::Ast::AST::Type;
    /// Type defination of the Parser Return Type
    /// 
    /// # Example
    /// ```
    /// fn test() -> Parser_Ret<()> 
    /// ```
    ///     
    pub type Parser_ret<T> = Result<T,ParserError>;



    pub enum Semantic_err{
        UndefinedVariable(String),
        TypeMismatch{expected: Type,got: Type},
        Break_Continue_location,
        UndefinedFunction(String),
        Argument_err{expected:String,got:String},
        Reassignment(String)
    }
    
    pub type Semantic_Ret<T> = Result<T,Semantic_err>;




    #[derive(Debug,Clone)]
        pub enum ParserError{
            UnexpectedToken{expected:String,got:String},
            Invalid_Code,
            Custom(String),
        }

    impl ParserError{
        fn show(&mut self) {
            println!("{:?}",match self {
                ParserError::UnexpectedToken{expected,got} => format!("Expected: {} but Got: {}",expected,got),
                ParserError::Custom(x) => x.to_owned(),
                ParserError::Invalid_Code => "Invalid syntax".to_string(),
            });
        }


    }

}