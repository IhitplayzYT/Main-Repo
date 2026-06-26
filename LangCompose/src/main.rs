mod helper;
mod Langcompose;
mod ai;
use std::{collections::{HashMap, HashSet}};

use helper::Helper;
use Langcompose::LangCompose;
use crate::ai::LLM::{translate_all};

// TODO: FIXME: The json has errors use mssg_id encoding

fn main() {
    let mut clargs = Helper::CLI::new();
    clargs.Parse_Args();
    if clargs.dbg_flag {
        println!("{clargs:?}");
    }
    let mut ignores= LangCompose::get_langignore(&clargs.proj_dir).into_iter().collect::<HashSet<String>>();
    ignores.insert("LangCompose".to_string());

    let langs = LangCompose::get_langs(&clargs.proj_dir[..]);
    let words = LangCompose::get_strings(&clargs.proj_dir[..],&ignores);
    let all_words:Vec<String> = words.iter().flat_map(|(_, inner)| inner.iter().cloned()).collect();
    if langs.is_empty(){
        // Do mkdir apatht root proj then create an empty value kv dict json
       LangCompose::build_dir(&clargs.proj_dir[..],&all_words,None,Some(&all_words)).unwrap();

    }else {
    for (fname,strings) in &words{
       let ret:Vec<Vec<String>> = langs.iter().map(|x| translate_all(strings,x).unwrap()).collect();
       if ret.len() != langs.len(){
        println!("Some Languages weren't recognised");
        std::process::exit(1);
       }

       let ret:HashMap<String,Vec<String>> = langs.clone().into_iter().zip(ret.clone().into_iter()).into_iter().collect();
       LangCompose::build_dir(&clargs.proj_dir[..],&all_words,Some(&ret),None).unwrap();

    }
    }
    
}
