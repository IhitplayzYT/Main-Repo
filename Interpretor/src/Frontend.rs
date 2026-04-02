// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//   Frontend.rs   //
// Contains the Frontend Sub-API for Compiler


#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals,dead_code,unused_imports)]

pub mod Frontend {
    use crate::Ast::AST::Code;
    use crate::Helper::Main::CLI;
    use crate::Parser::PARSER::{Parser};
    use crate::Semantic_Analysis::Analyser::Semantilizer;
    use crate::Tokeniser::Tokeniser::Lexer;
    use crate::Errors::Err::*;
    use crate::printer::printer::Components;

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
        pub lexer: Option<Lexer>,
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
        pub fn new() -> Self{
            Self{lexer:None,parser:None,semantic_analyser:Semantilizer::new()}
        }

        /// Frontend executor function
        /// 
        /// # Example
        /// ```
        /// frontend.exec();
        /// ```
        ///    
        pub fn Exec(&mut self,clargs: CLI) -> Result<Code,ERROR>{
            if let Some(mut lexer) = self.lexer.clone(){
                if !lexer.Tokenise(){
                panic!("FATAL INTERNAL ERROR IN COMPILER");
                }
            println!("\nLexer Sucess!!");
            if clargs.debug{
                println!("\nTOKENSTREAM: {{");
                if clargs.pretty {lexer.print();} else {lexer.print_tok();}
                println!("}}");
            }                
            self.parser = Some(Parser::new(lexer.Lexer_Output.clone()));
            let ast = self.parser.as_mut().unwrap().Parse().map_err(|e| {println!("Parser Error {e:?}");ERROR::Parseerr(e)})?;
            println!("{}Parser Sucess!!",if clargs.debug {"\n"} else {""});
            if clargs.debug{
                if let Some(x) = self.parser.clone(){
                    println!("\nAST: ");
                    if clargs.pretty {x.print();} else {println!("{:?}\n",ast);}
                }
            }
            self.semantic_analyser.analyse(&ast).map_err(|e| {println!("Semantic Error {e:?}");ERROR::Semerr(e)})?;
            println!("{}Semantalizer Sucess!!",if clargs.debug {"\n"} else {""});
            Ok(ast)

            }else{
                panic!("No file provided");
            } 

        }


    }


}
