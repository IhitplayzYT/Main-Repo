
pub mod LangCompose{

use regex::Regex;

    pub fn extract_strs(fpath: &str) -> Vec<String> {
        let text = std::fs::read_to_string(fpath).unwrap();
        let re = Regex::new(r#""[^"]*"|'[^']*'|`[^`]*`"#).unwrap();
        re.find_iter(&text).map(|m| m.as_str().to_string()).collect()
    }



    pub fn get_strings(root_dir:&str) -> Vec<(String,Vec<String>)>{
        let mut ret = Vec::new();
        std::fs::read_dir(root_dir).unwrap().for_each(|x| {let k  = x.unwrap();if k.path().is_dir(){ret.append(&mut get_strings(k.path().to_str().unwrap()))}else{ret.push((k.path().to_str().unwrap().to_string(),extract_strs(k.path().to_str().unwrap())));}});
        ret
    }
    pub fn get_langs(root: &str) -> Vec<String>{
        for x in std::fs::read_dir(root).unwrap(){
            let k = x.unwrap();
            if &k.file_name().into_string().unwrap()[..] == "LangCompose"{
                return std::fs::read_to_string(k.path().to_str().unwrap().to_string()).unwrap().split("\n").map(|x| x.to_string()).collect::<Vec<String>>()
            }};
    vec![]
    }
}