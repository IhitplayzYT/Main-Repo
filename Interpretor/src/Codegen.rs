// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Tokeniser.rs    //
// Contains the code for the Codegenerator

#![allow(
    non_camel_case_types,
    non_snake_case,
    non_upper_case_globals,
    dead_code
)]
pub mod Codegen {
    use crate::Ast::AST::Code;

type Codegen_result<T> = Result<T,CodegenErr>;


#[derive(Debug,Clone,PartialEq)]
struct Codegen {
    ast: Code,
    errs: bool,
}

pub enum CodegenErr{
    Fatal_err,
    Custom_err(String)
}


impl Codegen {

pub fn new(ast:Code) -> Self{
    Self{
        ast,
        errs: false,
    }
}


pub fn Exec(&mut self) -> Codegen_result<bool>{


Ok(true)    
}





}






}
