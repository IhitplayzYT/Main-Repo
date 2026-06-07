#![allow(non_camel_case_types,non_snake_case,dead_code)]

pub mod Cstam {

    use std::{collections::HashSet, collections::HashMap};

    use regex::Regex;
use sha2::{Digest, Sha256};
    pub fn process_dir(_fpath: String) {}

    pub fn process_file(fname: String) {
        if let Ok(doc) = std::fs::read_to_string(fname) {
            let l = doc.len();
            let bytes = doc.as_bytes();
            let re = Regex::new(
                r"(?m)^\s*(?:[A-Za-z_][A-Za-z0-9_\s\*]+)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(",
            )
            .unwrap();
            let mut async_fns: Vec<String> = vec![];
            for i in re.captures_iter(&doc) {
                async_fns.push(i[1].to_string());
            }

            let is_async: HashSet<_> = async_fns.iter().collect();
            println!("Async set = {:?}",is_async);





            let mut start = 0;
            while start < l {
                let pos = doc[start..].find("async").unwrap_or(l);
                if pos >= l {
                    break;
                }

                let mut temp = doc[start+pos..].find("(").unwrap_or(l);
                if temp >= l {
                    break;
                }

                let temp_end = temp;
                while temp > 0 && bytes[temp+start+pos] != b' ' {
                    temp -= 1;
                }

                let cur_fn_name:Vec<u8> = bytes[temp+start+pos+1..start+pos+temp_end].iter().map(|x| x.clone()).into_iter().collect();
                let fn_name:String = String::from_utf8(cur_fn_name).unwrap();

                println!("Cur fn = {fn_name}");

                let rel_off = doc[start+pos..].find("{").unwrap_or(l);
                if rel_off >= l {
                    break;
                }


                let mut stack = 1;
                let mut k = start + pos + rel_off+1;
                while stack != 0 && k <= l {
                    match bytes[k] {
                        b'{' => stack += 1,
                        b'}' => stack -= 1,
                        _ => {}
                    }
                    k += 1;
                }
                let (fn_start, fn_end) = (start + pos + rel_off, k);
                let fn_body: String = String::from_utf8(bytes[fn_start..fn_end].iter().map(|x| x.clone()).into_iter().collect::<Vec<u8>>()).unwrap();
                println!("{fn_name} => {}",fn_body);

                    let state_enum = Gen_Enum(fn_name,fn_body,&is_async);
                    println!("Enum  => {}",state_enum);
//                    let future_struct = Gen_Future();

                start = fn_end;
            }
        }
    }

    pub fn hashed_digest(digest:String) -> String {
        let mut hasher = Sha256::new();
        hasher.update(digest.as_bytes());
        format!("{:x}", hasher.finalize())
    } 

    pub fn Gen_Enum(fn_name:String,fn_body:String,is_async: & HashSet<&String>) -> String {
        let ret = format!("typedef enum e_State_{} {{\n{{}}\n}} e_State_{}_{}",fn_name,fn_name,hashed_digest(fn_body.clone()));
        let mut fn_names = String::new();
        fn_body.split_ascii_whitespace().for_each(|s| {
            for start in 0..s.len() {
                for end in start + 1..=s.len() {
                    if let Some(sub) = s.get(start..end) {
                        if is_async.contains(&sub.to_string()) {
                            fn_names += " ";
                            fn_names += sub;
                        }
                    }
                }
            }
        });

        let fn_names = fn_names.trim();
        let names = fn_names.split_ascii_whitespace().map(|x| x.to_string()).collect::<Vec<String>>();
        let mut counts = HashMap::<String, usize>::new();
        let states = names.iter().map(|name| {
                let n = counts.entry(name.clone()).or_insert(0);
                *n += 1;
                if *n == 1 {
                    format!("Waiting{}", name.to_uppercase())
                } else {
                    format!("Waiting{}{}", name.to_uppercase(), *n)
                }
            })
            .collect::<Vec<_>>();

        let enum_contents = std::iter::once("Start".to_string()).chain(states).chain(std::iter::once("Done".to_string())).collect::<Vec<_>>().join(",\n");
        ret.replace("{}", &enum_contents)
    }

}

