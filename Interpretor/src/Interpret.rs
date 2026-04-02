// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.
#![allow(non_snake_case,non_camel_case_types,unused_imports,dead_code)]

//   Interpretor.rs   //
// Contains the main Interpretor API

pub mod interpretor{
use crate::Tokeniser::Tokeniser::Lexer;
use crate::{Codegen::Codegen::Codegen, Frontend::Frontend::Frontend, Helper::Main::CLI};
use crate::Errors::Err::{self, InterpretorError, InterpretorReturn};
    /// Interpretor structure[MAIN API]
    /// 
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// Interpretor{
    ///     frontend: Frontend,
    /// }
    /// ```    
    #[derive(Debug,Clone)]
    pub struct Interpretor{
        rules_engine: CLI,
        frontend:Frontend,
        codegen:Codegen
    }


    impl Interpretor{
        /// Create and Initialise a new Interpretor struct
        /// 
        /// # Arguments
        /// file_path : &str  -> Path to the file being compiled
        /// 
        /// # Example
        /// ```
        /// Interpretor::new("myfile.c");
        /// ```    
    
        pub fn new() -> Self{
            Self{rules_engine:CLI::new(),frontend:Frontend::new(),codegen:Codegen::new()}
        }

        pub fn Run(&mut self) -> InterpretorReturn<bool>{
            /*  ***************************************** Rules Engine *****************************************  */
           let clargs = self.rules_engine.parse_clargs().map_err(|e| {
            InterpretorError::RULES_ERROR(format!("{e:?}"))
           })?; 
            /*  ***************************************** Rules Engine *****************************************  */

            if !clargs{
           return Err(InterpretorError::RULES_ERROR(format!("Rules engine failed! : {clargs:?}")));
            }

           if self.rules_engine.files.is_empty(){
            return Err(InterpretorError::RULES_ERROR("No target file provided!!".to_string()));
           }

            /*  ***************************************** Frontend *****************************************  */
           self.frontend.lexer = Some(Lexer::new(self.rules_engine.files[0].clone()));
            let code = self.frontend.Exec(self.rules_engine.clone()).map_err(|e| {
            InterpretorError::FRONTEND_ERROR(format!("{e:?}"))
           })?;
            /*  ***************************************** Frontend *****************************************  */


           
            /*  ***************************************** Codegen *****************************************  */
           let status = self.codegen.Exec(&code,self.rules_engine.clone()).map_err(|e| {
            InterpretorError::CODEGEN_ERROR(format!("{e:?}"))
           })?;
            /*  ***************************************** Codegen *****************************************  */

            Ok(status)
        }         

    }

}