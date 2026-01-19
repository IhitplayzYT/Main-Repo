// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

// Helper.rs //
// Provides some required basic helper functions.

#![allow(non_camel_case_types,non_snake_case,non_upper_case_globals)]

pub mod collections {
    use std::{
        collections::{HashMap, HashSet},
        hash::Hash,
    };

    pub fn Counter<T: Hash + Eq + Clone>(vect: &Vec<T>) -> HashMap<T, usize> {
        let mut ret: HashMap<T, usize> = HashMap::new();
        for i in vect {
            *ret.entry(i.clone()).or_insert(0) += 1;
        }
        ret
    }

    pub fn Set<T: Hash + Eq + Clone>(vect: &Vec<T>) -> HashSet<T> {
        let mut ret: HashSet<T> = HashSet::new();
        for i in vect {
            ret.insert(i.clone());
        }
        ret
    }
}

pub mod Checkers {
    pub fn is_numeric(c: char) -> bool {
        let c = c as u8;
        c >= 48 && c <= 57
    }
}

pub mod utilities {
    use std::fs;

    pub fn read_file(x: &str) -> std::io::Result<String> {
        fs::read_to_string(x)
    }
}
