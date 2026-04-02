// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//   Helper.rs   //
// Provides some required basic helper functions.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals,dead_code)]

pub mod collections {
    use std::{
        collections::{HashMap, HashSet},
        hash::Hash,
    };
    
    ///  Created a Counter from a Vector
    ///  # Arguments
    /// - vect : &Vec<T> -> A refernce to a vector of any type </br>
    /// # Returns
    /// - HashMap -> The counter map
    /// # Example 
    /// ```
    /// Counter(&[1,2,34,2,4,8]);
    /// ```

    pub fn Counter<T: Hash + Eq + Clone>(vect: &Vec<T>) -> HashMap<T, usize> {
        let mut ret: HashMap<T, usize> = HashMap::new();
        for i in vect {
            *ret.entry(i.clone()).or_insert(0) += 1;
        }
        ret
    }
    ///  # **Deprecated**
    ///  Created a Set from a Vector
    ///  # Arguments
    /// - vect : &Vec<T> -> A refernce to a vector of any type </br>
    /// # Returns
    /// - HashMap -> The counter map
    /// # Example 
    /// ```
    /// Counter(&[1,2,34,2,4,8]);
    /// ```
    #[deprecated(note="Use HashSet::from()")]
    pub fn Set<T: Hash + Eq + Clone>(vect: &Vec<T>) -> HashSet<T> {
        let mut ret: HashSet<T> = HashSet::new();
        for i in vect {
            ret.insert(i.clone());
        }
        ret
    }
}

pub mod Main{
use std::env;

#[derive(Debug,Clone)]
pub struct CLI{
    pub files: Vec<String>,
    pub debug: bool,
    pub env_var: Vec<(String,String)>,
    pub pretty: bool
}

impl CLI{
    pub fn new() -> Self{
        Self{
            files: Vec::new(),
            debug: false,
            env_var: Vec::new(),
            pretty: false,
        }       
    }

    pub fn parse_envs(&mut self,args: &str){
    let mut args = args.to_string();
        if !args.ends_with(")") {
            return;
        }else{
        args.pop();
        }
        //   a=b,c=d ,d=k , ...
        let args =String::from_utf8( args.bytes().filter(|c| *c != b' ' || *c != b'\n' ).collect()).unwrap();
        args.split(",").for_each(|x| {
           let x = x.split("=").collect::<Vec<&str>>(); 
           self.env_var.push((x[0].to_string(),x[1].to_string()));
        });
    }
    
    pub fn DEBUG_STR(){
        println!("./Interpretor [-d|-E|-f] <FILES> ...\n
   -d: To enable the Debug mode
   -f: To enable pretty Debug mode[Debug mode has to be enabled to work]
   -E=(ENV_VAR=VAL,...): A tuple of comma seperated env vars
   <FILES>: A file taken as argument with extentions .rs and .ihit");
    }

    pub fn parse_clargs(&mut self) -> CLI_ret<bool>{
    let arguments: Vec<String> = env::args().collect();
    if arguments.len() < 2  {
        Self::DEBUG_STR();    
    }
    let l = arguments.len();
    for i in 1..l {
        if arguments[i].starts_with("-E=("){
           self.parse_envs(&arguments[i][4..]); 
        }
        else{
        match &arguments[i][..]{
            "-d" => {
                self.debug = true;
            },
            "-f" => {
                self.pretty = true;
            },
            "-df" | "-fd" => {
                self.pretty = true;
                self.debug = true;
            }
            t => {
            if !t.ends_with(".rs") && !t.ends_with(".ihit"){
                let x:Vec<&str> = t.split("/").collect();
                if x.last().iter().any(|c| {**c == "."}){
                    if let Some(y) = x.last(){
                        Self::DEBUG_STR();    
                        return Err(CLI_ERR::UnsupportedFileformat(y.to_string()));
                    } else{
                    Self::DEBUG_STR();    
                    return Err(CLI_ERR::UnsupportedFileformat(x.last().unwrap().to_string().split_off(x.last().unwrap().to_string().rfind(".").unwrap())))
                    }
                }
                else{
                    Self::DEBUG_STR();    
                    return Err(CLI_ERR::Unsupported(t.to_string()));
                }
            }else{
                self.files.push(t.to_string());
            }

            }
        }
    }
    }


    Ok(true)
    }
}

#[derive(Debug,Clone)]
pub enum CLI_ERR {
    UnknownParam(String),
    UnsupportedFileformat(String),
    Unsupported(String),
    Custom(String),
}

static SUPPORTED_ARGS:&'static [&str;6] = &["<FILES>","-d","-E=(<ENV_VAR_NAME>=<VAL>,...)","-f","-df","-fd"];

pub type CLI_ret<T> = Result<T,CLI_ERR>;






}

pub mod Checkers {
    ///  Checks if a char is numeric 
    ///  # Arguments
    /// - c : char -> the char to be checked
    /// # Returns
    /// - bool -> True/False result of whether it is char or not
    /// # Example 
    /// ```
    /// is_numeric('a');
    /// ```
    pub fn is_numeric(c: char) -> bool {
        let c = c as u8;
        c >= 48 && c <= 57
    }
}

pub mod utilities {
    use std::fs;
    ///  Reads a file and returns an Result<String>
    ///  # Arguments
    /// - x: &str -> A refernce to a string of filename
    /// # Returns
    /// - Result<String> -> A result with string containing file contents on success
    /// # Example 
    /// ```
    /// let file_content: String = read_file("hello.txt")?;
    /// ```
    pub fn read_file(x: &str) -> std::io::Result<String> {
        fs::read_to_string(x)
    }
}
