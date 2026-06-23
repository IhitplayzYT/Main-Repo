mod helper;
mod Langcompose;
mod ai;
use helper::Helper;
use Langcompose::LangCompose;
use ai::LLM;
use crate::{Langcompose::LangCompose::get_strings, ai::LLM::{translate, translate_all}};
fn main() {
    let mut clargs = Helper::CLI::new();
    clargs.Parse_Args();
    if clargs.dbg_flag {
        println!("{clargs:?}");
    }
    let langs = LangCompose::get_langs(&clargs.proj_dir[..]);
    let words = LangCompose::get_strings(&clargs.proj_dir[..]);
    if langs.is_empty(){
        // Do mkdir at root proj then create an empty value kv dict json
    }
    for (fname,strings) in &words{
       let ret:Vec<Vec<String>> = langs.iter().map(|x| translate_all(strings,x).unwrap()).collect();
       if ret.len() != langs.len(){
        println!("Some Languages weren't recognised");
        std::process::exit(1);
       }

    }

    
}
