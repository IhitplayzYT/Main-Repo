// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]
pub mod Err{

#[derive(Debug,Clone)]
pub enum ParserError{
UnexpectedToken{expected:String,got:String},
Invalid_Code,
Custom(String),
}

impl ParserError{
    fn show(&mut self) {
    println!("{:?}",match self {
        ParserError::Custom(x) => x.to_owned(),
        ParserError::Invalid_Code => "Invalid syntax".to_string(),
        _ => "BROKEN ERROR[FATAL]".to_string(),
    });
}


}


pub type Parser_ret<T> = Result<T,ParserError>;
}