#![allow(non_camel_case_types,non_snake_case,dead_code)]

pub mod Cstam {
const HASH_SIZE: usize = 10;
    use std::{collections::{HashMap, HashSet}, fmt::format};

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
            let mut Future_Map:HashMap<String,String> = HashMap::new();
            let mut Fn_Body_Map:HashMap<String,String> = HashMap::new();
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
                Fn_Body_Map.insert(fn_name.clone(),fn_body.clone());

                    let names = Get_Fn_Name_Ordered(fn_body.clone(), &is_async);
                    let (state_enum,future_struct) = (Gen_Enum(fn_name.clone(),fn_body.clone(),names.clone()),Gen_Future(fn_name.clone(), fn_body.clone(), &is_async, names.clone(),&mut Future_Map,&Fn_Body_Map));
                    Future_Map.insert(fn_name.clone(), future_struct.clone());
                    println!("Enum  => {}",state_enum);
                    println!("Future  => {}",future_struct);

                start = fn_end;
            }
        }
    }

    pub fn hashed_digest(digest:String) -> String {
        let mut hasher = Sha256::new();
        hasher.update(digest.as_bytes());
        format!("{:x}", hasher.finalize())
    } 
    pub fn Get_Fn_Name_Ordered(fn_body:String,is_async: & HashSet<&String>) -> Vec<String> { 
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
       fn_names.split_ascii_whitespace().map(|x| x.to_string()).collect::<Vec<String>>()
    }

    /*
        a() {
        }

        b() {
        a()
        c()
        }

     */


// We assume Hoisting to not be there and it be sequentially defined to avoid maintaining of ast to parse Future creation orders
    pub fn Gen_Future(fn_name:String,fn_body:String,is_async: & HashSet<&String>,names: Vec<String>,future_map: &mut HashMap<String,String>,fn_body_map: &HashMap<String,String>) -> String {
        let ret = format!("typedef struct s_Future_{} {{\ne_State_{}_{} state;\n{{}}\n}} Future_{};",fn_name.clone(),fn_name.clone(),&hashed_digest(fn_body.clone())[..HASH_SIZE],fn_name.clone()); 
        let mut s_future = "".to_string();
        let mut is_async = is_async.clone();
        for i in &names {
                if future_map.contains_key(i){
                }else{
                    let k = s_future.clone();
                    let t = &Gen_Future(i.to_string(), fn_body_map[i].clone(), &is_async,Get_Fn_Name_Ordered(fn_body_map[i].clone(), &is_async) , future_map, fn_body_map);
                    future_map.insert(fn_name.clone(), t.to_string());
                    s_future = t.to_string() + "\n" + &k;

                }
                    s_future += &format!("Future_{i} fn_{i};\n");
        }
        ret.replace("{}", &s_future)
    }

    pub fn Gen_Enum(fn_name:String,fn_body:String,names: Vec<String>) -> String {
        let ret = format!("typedef enum e_State_{} {{\n{{}}\n}} e_State_{}_{};",fn_name,fn_name,&hashed_digest(fn_body.clone())[..HASH_SIZE]);
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

