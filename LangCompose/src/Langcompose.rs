
pub mod LangCompose{

use std::{collections::{HashMap, HashSet}, fs, path::Path};

use regex::Regex;

const i18n_js:&str ="
let currentLang = \"en\";
let translations = {};

export async function setLanguage(lang) {
    currentLang = lang;

    const response = await fetch(
        `./locales/${lang}.json`
    );

    translations = await response.json();
}

function t(str) {
    return translations[str] ?? str;
}
";


    pub fn extract_strs(fpath: &str) -> Vec<String> {
        let text = std::fs::read_to_string(fpath).unwrap();
        let re = Regex::new(r#""[^"]*"|'[^']*'|`[^`]*`"#).unwrap();
        re.find_iter(&text).map(|m| m.as_str().to_string()).collect()
    }


    pub fn get_strings(root_dir:&str,ignores:&HashSet<String>) -> Vec<(String,Vec<String>)>{
        let mut ret = Vec::new();
        std::fs::read_dir(root_dir).unwrap().for_each(|x| {
            let k  = x.unwrap();
            if !ignores.contains(k.path().to_str().unwrap()){
            if k.path().is_dir(){
                ret.append(&mut get_strings(k.path().to_str().unwrap(),ignores))
            }else{
                ret.push((k.path().to_str().unwrap().to_string(),extract_strs(k.path().to_str().unwrap())));
            }
        }
        });
        ret
    }

    pub fn get_langs(root: &str) -> Vec<String>{
        let fpath = Path::new(root).join("LangCompose");
        if fpath.is_file(){
            std::fs::read_to_string(fpath).unwrap().split("\n").map(|x| x.to_string()).collect::<Vec<String>>()
        }else{
            return vec![]
        }
    }

    pub fn get_langignore(root: &str) -> Vec<String>{
        let fpath = Path::new(root).join(".LangIgnore");
        if fpath.is_file(){
            std::fs::read_to_string(fpath).unwrap().split("\n").map(|x| x.to_string()).collect::<Vec<String>>()
        }else{
            return vec![]
        }
    }

    pub fn build_dir(root: &str,og:&Vec<String>,lang_maps: Option<& HashMap<String,Vec<String>>>,langs:Option<& Vec<String>>) -> std::io::Result<()>{
        let bpath = Path::new(root).join("locales/");
        if let Ok(_) = std::fs::create_dir(bpath.clone()){
           if let Some(x)  = lang_maps{
                x.iter().for_each(|(a,b)| {
                let fpath = bpath.join(a.to_string()+".json");
                let mut buff = "{".to_string();
                   for (old,new) in og.iter().zip(b.iter()){
                    buff += &format!("\"{old}\":\"{new}\",\n");
                   }
                   if buff.len() > 2{
                        buff.pop();
                        buff.pop();
                   }
                   buff += "\n}";
                    fs::write(fpath,buff).unwrap();
                }); 
           }
            if let Some(x)  = langs{
                let fpath = bpath.join("temp.json");
                let mut buff = "{".to_string();
                for word in x{
                buff += &format!("\"{word}\":\"\",\n");
                }
                if buff.len() > 2{
                    buff.pop();
                    buff.pop();
                }
                buff += "\n}";
                fs::write(fpath,buff).unwrap();               
            }
           }

            Ok(())
        }
    }
