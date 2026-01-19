// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

pub mod Compiler{
use crate::Frontend::Frontend::Frontend;
pub struct Compiler{
    frontend:Frontend,
}

impl Compiler{
pub fn new(file_path : &str) -> Self{
    Self{frontend:Frontend::new(file_path.to_string())}
}

}
}