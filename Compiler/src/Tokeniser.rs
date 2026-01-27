// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]
pub mod Tokeniser {
    use crate::Lexer_Tok::Lex_Tok::LTOK;
    use once_cell::sync::Lazy;
    use std::{collections::HashMap, fs, ops::Index, process::exit};

    pub static ALLOWED_KEYWORDS: Lazy<HashMap<&'static str, LTOK>> = Lazy::new(|| {
        HashMap::from([
            ("let", LTOK::LET),
            ("mut", LTOK::MUT),
            ("const", LTOK::CONST),
            ("if", LTOK::IF),
            ("else", LTOK::ELSE),
            ("while", LTOK::WHILE),
            ("Loop", LTOK::LOOP),
            ("for", LTOK::FOR),
            ("fn", LTOK::FN),
            ("break", LTOK::BREAK),
            ("continue", LTOK::CONTINUE),
            ("return", LTOK::RETURN),
            ("s8", LTOK::INT_TYPE),
            ("s16", LTOK::INT_TYPE),
            ("s32", LTOK::INT_TYPE),
            ("s64", LTOK::INT_TYPE),
            ("i8", LTOK::INT_TYPE),
            ("i16", LTOK::INT_TYPE),
            ("i32", LTOK::INT_TYPE),
            ("i64", LTOK::INT_TYPE),
            ("int", LTOK::INT_TYPE),
            ("short", LTOK::INT_TYPE),
            ("char", LTOK::INT_TYPE),
            ("long", LTOK::INT_TYPE),
            ("f32", LTOK::FLOAT_TYPE),
            ("f64", LTOK::FLOAT_TYPE),
            ("float", LTOK::FLOAT_TYPE),
            ("double", LTOK::FLOAT_TYPE),
            ("string",LTOK::STRING_TYPE),
            ("str",LTOK::STRING_TYPE),
            ("String",LTOK::STRING_TYPE),
            ("in",LTOK::IN)
        ])
    });

    pub struct Lexer {
        pub text: String,
        pub Lexer_Output: Vec<LTOK>,
    }

    impl Lexer {
        pub fn new(v: String) -> Self {
            let file = fs::read_to_string(v).unwrap();
            let t = file.replace("..", ";");
            Self {
                text:t,
                Lexer_Output: Vec::new(),
            }
        }

        pub fn print_tok(&self){
            if (!self.is_lexed()){
             println!("Use Tokeniser before printing!");      
            }
            println!("{:?}",self.Lexer_Output);
        }

        pub fn is_lexed(&self) -> bool {
            self.Lexer_Output.len() > 0
        }

        pub fn resolve_ltok(v: &str) -> Option<LTOK> {
            if v.is_empty() {
                return None;
            }
            if let Some(y) = ALLOWED_KEYWORDS.get(v) {
                return Some(y.clone());
            } else {
                if v.chars().all(|c| c.is_ascii_digit() || c == '.') {
                    let count = v.chars().filter(|&c| c == '.').count();
                    if count < 1 {
                        return Some(LTOK::INT((v.to_string()).parse().unwrap_or(0)));
                    } else if count == 1 {
                        return Some(LTOK::FLOAT(v.to_string().trim().parse().unwrap_or_else(|_e| {exit(-1);})));
                    } else {
                        return Some(LTOK::STRING(v.to_string()));
                    }
                }
               Some(match v {
                    "+" => LTOK::PLUS,
                    "-" => LTOK::MINUS,
                    "/" => LTOK::DIV,
                    "%" => LTOK::MODULO,
                    "*" => LTOK::STAR,
                    "=" => LTOK::ASSGN,
                    "==" => LTOK::EQ,
                    "<=" => LTOK::LT_EQ,
                    ">=" => LTOK::GT_EQ,
                    "!=" => LTOK::N_EQ,
                    "+=" => LTOK::S_PLUS,
                    "-=" => LTOK::S_MINUS,
                    "*=" => LTOK::S_MULT,
                    "/=" => LTOK::S_DIV,
                    "%=" => LTOK::S_MOD,
                    "<<" => LTOK::LSHIFT,
                    ">>" => LTOK::RSHIFT,
                    "&" => LTOK::AMP,
                    "^" => LTOK::CARET,
                    "|" => LTOK::PIPE,
                    "~" => LTOK::TILDA,
                    "&&" => LTOK::ANDAND,
                    "||" => LTOK::OROR,
                    "!" => LTOK::BANG,
                    "{" => LTOK::LBRACE,
                    "}" => LTOK::RBRACE,
                    "[" => LTOK::LBRACK,
                    "]" => LTOK::RBRACK,
                    "(" => LTOK::LPAREN,
                    ")" => LTOK::RPAREN,
                    ";" => LTOK::SEMICOLON,
                    ":" => LTOK::COLON,
                    "," => LTOK::COMMA,
                    "None" => LTOK::NULL,
                    "Null" => LTOK::NULL,
                    "NULL" => LTOK::NULL,
                    "\"" => LTOK::DQUOTE,
                    "\'" => LTOK::QUOTE,
                    _ => LTOK::IDENT(v.to_string()),
                })
            }
        }

        pub fn Tokenise(&mut self) -> bool {
            let x = self.text.clone();
            let mut ret: Vec<LTOK> = Vec::new();
            let mut iter = x.chars().peekable();
            let mut temp = String::new();
            let mut c = 0;
            while let Some(i) = iter.next() {
                if c >= 1 {
                    if i == '\'' && c == 1 {
                        c = 0;
                        ret.push(LTOK::STRING((&temp[..]).to_string()));
                        temp.clear();
                    } else if i == '\"' && c == 2 {
                        c = 0;
                        ret.push(LTOK::STRING((&temp[..]).to_string()));
                        temp.clear();
                    } else {
                        temp.push(i);
                    }
                } else if (i == '\'') && (c == 0) {
                    c = 1;
                    temp.clear();
                } else if (i == '\"') && (c == 0) {
                    c = 2;
                    temp.clear();
                } else if i == ' ' || i == '\n' {
                    if let Some(v) = Self::resolve_ltok(&temp[..]) {
                        ret.push(v);
                    } else {
                    }
                    temp.clear();
                    continue;
                } else if i == '('
                    || i == '['
                    || i == '{'
                    || i == ')'
                    || i == '}'
                    || i == ']'
                    || i == ':'
                    || i == ';'
                    || i == ','
                    || i == '~'
                {
                    if let Some(k) = Self::resolve_ltok(&temp[..]) {
                        ret.push(k);
                    }
                    if let Some(k) = Self::resolve_ltok(&i.to_string()[..]) {
                        ret.push(k);
                    }
                    temp.clear();
                } else if i == '+' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::S_PLUS);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::PLUS);
                        temp.clear();
                    }
                } else if i == '-' {
                    if let Some(v) = iter.next() {
                        let r = match v{
                            '=' => LTOK::S_MINUS,
                            '>' => LTOK::ARROW,
                            _ => {exit(-1);}
                        };
                        ret.push(r);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::MINUS);
                        temp.clear();
                    }
                } else if i == '*' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::S_MULT);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::STAR);
                        temp.clear();
                    }
                } else if i == '/' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::S_DIV);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::DIV);
                        temp.clear();
                    }
                } else if i == '%' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::S_MOD);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::MODULO);
                        temp.clear();
                    }
                } else if i == '=' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::EQ);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::ASSGN);
                        temp.clear();
                    }
                } else if i == '!' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::N_EQ);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::BANG);
                        temp.clear();
                    }
                } else if i == '>' {
                    if let Some(v) = iter.next() {
                        match v {
                            '=' => ret.push(LTOK::GT_EQ),
                            '>' => ret.push(LTOK::RSHIFT),
                            _ => continue,
                        };
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::GT);
                        temp.clear();
                    }
                } else if i == '<' {
                    if let Some(v) = iter.next() {
                        match v {
                            '=' => ret.push(LTOK::LT_EQ),
                            '<' => ret.push(LTOK::LSHIFT),
                            _ => continue,
                        };
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::LT);
                        temp.clear();
                    }
                } else if i == '&' {
                    if let Some(v) = iter.next() {
                        match v {
                            '=' => ret.push(LTOK::S_AMP),
                            '&' => ret.push(LTOK::ANDAND),
                            _ => continue,
                        };
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::AMP);
                        temp.clear();
                    }
                } else if i == '|' {
                    if let Some(v) = iter.next() {
                        match v {
                            '=' => ret.push(LTOK::S_PIPE),
                            '|' => ret.push(LTOK::OROR),
                            _ => continue,
                        };
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::PIPE);
                        temp.clear();
                    }
                } else if i == '^' {
                    if let Some('=') = iter.next() {
                        ret.push(LTOK::S_CARET);
                        temp.clear();
                        iter.next();
                    } else {
                        ret.push(LTOK::CARET);
                        temp.clear();
                    }
                } else {
                    temp.push(i);
                }
            }

            if !temp.is_empty() {
                if let Some(d) = Self::resolve_ltok(&temp[..]) {
                    ret.push(d);
                    temp.clear();
                } else {
                }
            }
            ret.push(LTOK::EOF);
            self.Lexer_Output = ret;
            true
        }
    }
}
