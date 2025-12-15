mod Helper;
mod Lexer_Tok;
mod Parser_Tok;
mod Tokeniser;
use std::env;

use crate::Helper::utilities::read_file;
fn main() {
    let arguments: Vec<String> = env::args().collect();
    if arguments.len() != 2 {
        std::process::exit(0);
    }
    let file =
        Helper::utilities::read_file(&arguments[1]).unwrap_or_else(|e| std::process::exit(0));

    let ret = Tokeniser::Tokeniser::Tokenise(file);
    println!("{:?}", ret);
}
