// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]

pub mod Frontend {
    use crate::Parser::PARSER::{Parser};
    use crate::Semantic_Analysis::Analyser::Semantilizer;
    use crate::Tokeniser::Tokeniser::Lexer;
    pub struct Frontend {
        pub lexer: Lexer,
        pub parser: Option<Parser>,
    }

impl Frontend{
pub fn new(v : String) -> Self{
    Self{lexer:Lexer::new(v),parser:None}
}

pub fn exec(&mut self){
if !self.lexer.Tokenise(){
panic!("FATAL INTERNAL ERROR IN COMPILER");
}
self.parser = Some(Parser::new(self.lexer.Lexer_Output.clone()));
let ast = self.parser.as_mut().unwrap().Parse();
//  ADD SEMANTIC ANALYSER TO THE CHAIN
}


}


}
