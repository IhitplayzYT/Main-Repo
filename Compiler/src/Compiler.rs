// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.
#![allow(non_snake_case,non_camel_case_types,unused_imports,dead_code)]


pub mod Compiler{
use crate::Frontend::Frontend::Frontend;
    /// Compiler structure[MAIN API]
    /// 
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// Compiler{
    ///     frontend: Frontend,
    /// }
    /// ```    
    #[derive(Debug,Clone)]
    pub struct Compiler{
        frontend:Frontend,
    }


    impl Compiler{
        /// Create and Initialise a new Compiler struct
        /// 
        /// # Arguments
        /// file_path : &str  -> Path to the file being compiled
        /// 
        /// # Example
        /// ```
        /// Compiler::new("myfile.c");
        /// ```    
    
        pub fn new(file_path : &str) -> Self{
            Self{frontend:Frontend::new(file_path.to_string())}
        }

    }

}