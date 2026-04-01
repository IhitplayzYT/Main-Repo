// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//   Errors.rs   //
// Contains the error structures for the compiler

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals,dead_code)]
pub mod Err{
    use crate::{Ast::AST::Type, Codegen::Codegen::{ControlFlow}};
    /// Type defination of the Parser Return Type
    /// This is will be the majority return type used in the parser
    /// 
    /// # Example
    /// ```
    /// fn test() -> Parser_Ret<()> 
    /// ```
    ///     
    pub type Parser_ret<T> = Result<T,ParserError>;

    /// Type defination of the Sematic Analyser Return Type
    /// 
    /// # Example
    /// ```
    /// fn test() -> Semantic_Ret<()> 
    /// ```
    ///     
    pub type Semantic_Ret<T> = Result<T,Semantic_err>;

    /// Type defination of the Codegen Return Type
    /// 
    /// # Example
    /// ```
    /// fn test() -> CodegenReturn<()> 
    /// ```
    ///     
    pub type CodegenReturn<T> = Result<T,CodegenError>;

    /// Enum that stores the Codegen Errors Possible
    /// 
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// InterpretorError::UndefinedVariable("Undefined varib".to_string());
    /// InterpretorError::DivideByZero;
    /// ```
    ///    
    #[derive(Debug,Clone)] 
    pub enum CodegenError{
        UndefinedVariable(String),
        RedefinedVariable(String),
        ImmutableVariable(String),
        TypeError(String),
        DivideByZero,
        InvalidAssignment,
        NoFieldOnType(String, String),
        ControlFault(ControlFlow), 
        FunctionNotFound(String),
        IncompatibleTypes(String,String),
        Custom(String),
    }

    
    /// Enum that stores the Semantic Errors Possible
    /// 
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// Semantic_err::UndefinedVariable("Undefined varib".to_string());
    /// Semantic_err::Break_Continue_location;
    /// ```
    ///    
    #[derive(Debug,Clone)] 
    pub enum Semantic_err{
        UndefinedVariable(String),
        RedefinedVariable(String),
        TypeMismatch{expected: Type,got: Type},
        Break_Continue_location,
        UndefinedFunction(String),
        RedefinedFunction(String),
        Immutable_Variable(String),
        Argument_err{expected:String,got:String},
        Reassignment(String),
        Custom(String),
    }

    #[derive(Debug,Clone)] 
    pub enum InterpretorError{
        RULES_ERROR(String),
        FRONTEND_ERROR(String),
        CODEGEN_ERROR(String),
    }

    pub type InterpretorReturn<T> = Result<T,InterpretorError>;


    /// Enum that stores the Parser Errors Possible
    /// 
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// ParserError::UnexpectedToken("expected".to_string(),"got".to_string());
    /// ParserError::Invalid_Code;
    /// ```
    ///    
    #[derive(Debug,Clone)]
        pub enum ParserError{
            UnexpectedToken{expected:String,got:String},
            Invalid_Code,
            Custom(String),
        }
    
    #[derive(Debug,Clone)]
    pub enum ERROR{
        Semerr(Semantic_err),
        Parseerr(ParserError),
        Codegenerr(CodegenError),
        Intererr(InterpretorError),
    }

    impl ParserError{

    /// Function to show formatted Parser Errir 
    ///  
    /// # Example
    /// ```
    /// parser_err.show();
    /// ```
    ///        
        fn show(&mut self) {
            println!("{:?}",match self {
                ParserError::UnexpectedToken{expected,got} => format!("Expected: {} but Got: {}",expected,got),
                ParserError::Custom(x) => x.to_owned(),
                ParserError::Invalid_Code => "Invalid syntax".to_string(),
            });
        }


    }



}