// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Frontend.rs    //
// Contains the Frontend Sub-API for Compiler


#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals,dead_code,unused_imports)]

pub mod Frontend {
    use crate::Parser::PARSER::{Parser};
    use crate::Semantic_Analysis::Analyser::Semantilizer;
    use crate::Tokeniser::Tokeniser::Lexer;
    use crate::Errors::Err::*;

    /// Frontend struct
    /// 
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// Frontend{
    /// lexer:  Lexer::new("Path.rs"),
    /// parser: None
    /// }
    /// ```
    ///    
    #[derive(Debug,Clone)]
    pub struct Frontend {
        pub lexer: Lexer,
        pub parser: Option<Parser>,
        pub semantic_analyser: Semantilizer,
    }

    impl Frontend{

        /// Frontend constructor function
        ///
        /// # Arguments
        /// v : String -> Path to file.rs
        /// 
        /// # Example
        /// ```
        /// Frontend::new("hello.rs");
        /// ```
        ///    
        pub fn new(v : String) -> Self{
            Self{lexer:Lexer::new(v),parser:None,semantic_analyser:Semantilizer::new()}
        }

        /// Frontend executor function
        /// 
        /// # Example
        /// ```
        /// frontend.exec();
        /// ```
        ///    
        pub fn exec(&mut self) -> Result<(),ERROR>{
            if !self.lexer.Tokenise(){
                panic!("FATAL INTERNAL ERROR IN COMPILER");
            }
            self.parser = Some(Parser::new(self.lexer.Lexer_Output.clone()));
            let ast = self.parser.as_mut().unwrap().Parse().map_err(ERROR::Parseerr)?;
            self.semantic_analyser.analyse(&ast).map_err(ERROR::Semerr)?;
            Ok(())
        }


    }


}
