// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]
mod Ast;
mod Frontend;
mod Helper;
mod Lexer_Tok;
mod Parser;
mod Semantic_Analysis;
mod Ident_table;
mod Tokeniser;
mod Errors;
mod Compiler;
mod printer;
use std::env;

use crate::{Helper::utilities::read_file, printer::printer::Components};

fn main() {
    let arguments: Vec<String> = env::args().collect();
    if arguments.len() != 2 {
        std::process::exit(0);
    }

    let mut LEXER = Tokeniser::Tokeniser::Lexer::new(arguments[1].clone());
    LEXER.Tokenise();
    LEXER.print_tok();
    println!();
    println!();
    let mut PARSER = Parser::PARSER::Parser::new(LEXER.Lexer_Output);
    let t = PARSER.Parse();
    println!("{:?}",t);
    println!("{:?}: {:?}",PARSER.idx,PARSER.input[PARSER.idx]);
}
