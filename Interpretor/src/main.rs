// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//   main.rs   //
// The main entry point

#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
mod Ast;
mod Errors;
mod Frontend;
mod Helper;
mod Ident_table;
mod Interpret;
mod Lexer_Tok;
mod Parser;
mod Semantic_Analysis;
mod Tokeniser;
mod Codegen;
mod printer;

use crate::{Interpret::interpretor::Interpretor}; 

fn main() {

    let mut interpretor:Interpretor = Interpretor::new();
    if let Ok(result) = interpretor.Run(){
        println!("Result={result}");
    }

}
