// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Helper.rs    //
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
