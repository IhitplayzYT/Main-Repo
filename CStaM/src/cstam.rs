#![allow(non_camel_case_types,non_snake_case,dead_code)]

pub mod Cstam {
const HASH_SIZE: usize = 5;
    use std::{collections::{HashMap, HashSet}, fmt::format, fs::TryLockError::WouldBlock};
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

                let mut ret_type = "".to_string();
                let k = temp;
                while temp > 0 && &doc[start+pos..temp+start+pos] != "async "{
                    temp -= 1;
                }
                ret_type += &doc[temp+start+pos..k+start+pos];

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
                    let (poll_enum,state_enum,future_struct) = (Gen_Poll_Type(fn_name.clone(),ret_type),Gen_Enum(fn_name.clone(),fn_body.clone(),names.clone()),Gen_Future(fn_name.clone(), fn_body.clone(), &is_async, names.clone(),&mut Future_Map,&Fn_Body_Map));
                    Future_Map.insert(fn_name.clone(), future_struct.clone());
                    println!("Enum  => {}",state_enum);
                    println!("Future  => {}",future_struct);
                    println!("Poll Enum  => {}",poll_enum);
                    let poll_fn = Gen_Poll(fn_name, fn_body, &is_async, state_enum, future_struct);
                    println!("{poll_fn}");
                start = fn_end;
            }
        }
    }

    pub fn Gen_Poll_Type(fn_name: String,ret_type: String) -> String{ 
        format!("typedef enum e_PollStatus{{\nPOLL_PENDING,\nPOLL_READY\n}} PollStatus;\n\ntypedef struct s_PollResult{{\nPollStatus status;\n{ret_type} result;\n}} {fn_name}_PollResult;\n")
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
        let ret = format!("typedef struct s_Future_{} {{\ne_State_{}_{} state;\n{{}}\n}} Future_{};\n",fn_name.clone(),fn_name.clone(),&hashed_digest(fn_body.clone())[..HASH_SIZE],fn_name.clone()); 
        let mut s_future = "".to_string();
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
        let vars = Extract_Persistent_Vars(fn_body, &is_async);
        ret.replace("{}", &(s_future + &vars))
    }

    pub fn Gen_Enum(fn_name:String,fn_body:String,names: Vec<String>) -> String {
        let ret = format!("typedef enum e_State_{} {{\n{{}}\n}} e_State_{}_{};\n",fn_name,fn_name,&hashed_digest(fn_body.clone())[..HASH_SIZE]);
        let mut counts = HashMap::<String, usize>::new();
        let states = names.iter().map(|name| {
                let n = counts.entry(name.clone()).or_insert(0);
                *n += 1;
                if *n == 1 {
                    format!("Waiting{}", name)
                } else {
                    format!("Waiting{}{}", name, *n)
                }
            })
            .collect::<Vec<_>>();

        let enum_contents = std::iter::once("Start".to_string()).chain(states).chain(std::iter::once("Done".to_string())).collect::<Vec<_>>().join(",\n");
        ret.replace("{}", &enum_contents)
    }

    pub fn Gen_Poll(fn_name:String,fn_body:String,is_async: & HashSet<&String>,s_enum:String,s_Future: String) -> String {
        let ret  = format!("
{fn_name}_PollResult {fn_name}_poll(Future_{fn_name} *self) {{
    {fn_name}_PollResult ret = ({fn_name}_PollResult){{.status = POLL_PENDING}};
    while (1) {{
        switch (self->state) {{  
    {{}}
    }}

}}");

        let mut ladder = "".to_string();

        /*
        Gen code for Start
        */

        /*
         *      {
         *          printf("");
         *          let y = 9;
         *          let x = a1(y);
         *          if x > 2 {
         *              return x;
         *          }
         *          return a2(x)
         *      }
         * 
         * 
         */

        let mut buff = "".to_string();
        let mut iop = 0;
        let mut ASSIGN = "".to_string();
        let fn_map = parse_calls(&fn_body);
        for i in  fn_body.split("\n").skip(1){
        let mut has_async = "".to_string();
           for j in is_async{
            if i.contains(*j){
                has_async = j.to_string();
                break;
            }
           }

                if has_async.is_empty(){
                    if let Some(idx) = i.find("return"){
                        let mut fn_ret = i[idx+7..].trim();
                        fn_ret = &fn_ret[..fn_ret.len() - 1];
                        buff += &format!("self->state = Done;\nreturn ({fn_name}_PollResult){{.result = {fn_ret},.status = POLL_READY}};\n");
                    } else{
                        buff += i;
                        buff += "\n";
                    }
                }else{
                let (mut lp,mut rp) = (0,0);
                    if let Some(idx) = i.find(&has_async){
                     lp = idx + has_async.len()+1;
                     rp = lp + i[lp..].find(")").unwrap();
                    }
                    
                    let params = split_args(&i[lp..rp]);
                    let p_buff = params.iter().map(|x| if s_Future.contains(&(" ".to_string() + x + ";")) {format!("self->{x},")} else {x.clone()+","} ).collect::<Vec<String>>().join(" ");
                    let p_buff = &p_buff[..p_buff.len() - 1];
                    
                    
                    let mut next_state = "".to_string();

                    let sp = s_enum.find(&format!("Waiting{has_async}")).unwrap() + 7 + has_async.len()+1;
                    if let Some(z) = s_enum[sp..].find(","){
                        println!("{} {}",sp+1,sp+z);
                        next_state += &s_enum[sp +1..sp+z];
                    }else{
                    next_state = format!("Done");
                    }

                    next_state = next_state.trim().to_string();
                    let mut next_fn = "".to_string();
                    let mut next_p_buff = "".to_string();
                    let mut params = vec![];


                    if &next_state[..] != "Done" {
                    next_fn = next_state[7..].to_string();
                    let k = fn_body[fn_body.find(&i).unwrap()+i.len()..].to_string();

                    if let Some(idx) = k.find(&next_fn){
                     lp = idx + next_fn.len()+1;
                     rp = lp + k[lp..].find(")").unwrap();
                    }
                     params = split_args(&k[lp..rp]);
                     next_p_buff = params.iter().map(|x| if s_Future.contains(&(" ".to_string() + x + ";")) {format!("self->{x},")} else {x.clone()+","} ).collect::<Vec<String>>().join(" ");
                     next_p_buff = next_p_buff[..next_p_buff.len() - 1].to_string();

                     println!("{next_fn} {params:?} {next_p_buff:?}");
                }


                // TODO: FIXME: New issue is that we need to assignt he results of the async fn to the varib in the self...eg self->r1 = ret.result
                for i in &fn_map {
                    if i.function_name == has_async{
                        if let Some(var) = i.variable_name.clone(){
                            ASSIGN += &format!("\n\t\tself->{var}={has_async}_ret.result;");
                        }
                        break;
                    }

                }


                    if iop == 0{
                    ladder += &format!("
            case Start :{{
                {}self->fn_{has_async} = {has_async}({p_buff});{ASSIGN}
                self->state = Waiting{has_async}; 
                continue;
            }}

            case Waiting{has_async} :{{
                {has_async}_PollResult {has_async}_ret = {has_async}_poll(&self->fn_{has_async});
                if ({has_async}_ret.status == POLL_PENDING) {{return ret;}}{}{ASSIGN}
                self->fn_{next_fn} = {next_fn}({next_p_buff});
                self->state = {next_state};
                continue;
            }}
                    ",if buff.is_empty(){buff.clone()} else {"\n".to_string()+&buff},if buff.is_empty(){buff.clone()} else {"\n".to_string()+&buff});
                    
                        
                }
                else if &next_state[..] == "Done"{
            ladder += &format!("
            case Waiting{has_async} :{{
                {has_async}_PollResult {has_async}_ret = {has_async}_poll(&self->fn_{has_async});
                if ({has_async}_ret.status == POLL_PENDING) {{return ret;}}{}{ASSIGN}
                self->state = {next_state};
                continue;
            }}
                   ",if buff.is_empty(){buff.clone()} else {"\n".to_string()+&buff}) 
                }
                else{


                    ladder += &format!("
            case Waiting{has_async} :{{
                {has_async}_PollResult {has_async}_ret = {has_async}_poll(&self->fn_{has_async});
                if ({has_async}_ret.status == POLL_PENDING) return ret;{}{ASSIGN}
                self->fn_{next_fn} = {next_fn}({next_p_buff});
                self->state = {next_state};
                continue;
            }}
                    ",if buff.is_empty(){buff.clone()} else {"\n".to_string()+&buff});}

                    iop +=1;
                    buff.clear();
                }
                ASSIGN.clear();
        }

        /*

        for i in s_enum.split("\n").skip(1){
            if i.ends_with(",") {
                let e_var = &i[..i.len() - 1];
                let e_fn_name = &e_var[7..];
                ladder += &format!("
                    case {e_var}:{{
                        ret = {e_fn_name}_poll(&self->fn_{e_fn_name});
                        if (ret.result = POLL_PENDING) return ret;
                        self->

                    }}
                ")

            }
        }

        */

        /*
        Gen code for Done 
        */

        ladder += &format!("
            case DONE :{{
                return ({fn_name}_PollResult){{
                    .status = POLL_READY,
                    .result = self->result
                }};
            }}
        }}
        ");

        ret.replace("{}", &ladder)
    }

pub fn split_args(args: &str) -> Vec<String> {
    let mut out = Vec::new();
    let mut start = 0;
    let mut depth = 0;
    for (i, ch) in args.char_indices() {
        match ch {
            '(' => depth += 1,
            ')' => depth -= 1,
            ',' if depth == 0 => {
                out.push(args[start..i].trim().to_string());
                start = i + 1;
            }
            _ => {}
        }
    }
    out.push(args[start..].trim().to_string());
    out
}

#[derive(Debug, Clone)]
pub struct FunctionCall {
    pub variable_type: Option<String>,
    pub variable_name: Option<String>,
    pub function_name: String,
    pub parameter_list: Vec<String>,
}

pub fn parse_calls(text: &str) -> Vec<FunctionCall> {
    let mut result = Vec::new();
    for line in text.lines() {
        let line = line.trim();
        if line.is_empty() {
            continue;
        }
        let (variable_type, variable_name, rhs) =
            if let Some(eq_pos) = line.find('=') {
                let lhs = line[..eq_pos].trim();
                let rhs = line[eq_pos + 1..].trim();
                let parts: Vec<&str> =
                    lhs.split_whitespace().collect();
                if parts.len() >= 2 {
                    let variable_name =
                        parts.last().unwrap().to_string();

                    let variable_type =
                        parts[..parts.len() - 1].join(" ");
                    (
                        Some(variable_type),
                        Some(variable_name),
                        rhs,
                    )
                } else {
                    (None, None, rhs)
                }
            } else {
                (None, None, line)
            };
        let Some(open_pos) = rhs.find('(') else {
            continue;
        };
        let function_name = rhs[..open_pos].trim().to_string();
        let bytes = rhs.as_bytes();
        let mut depth = 1;
        let mut i = open_pos + 1;
        while i < bytes.len() && depth > 0 {
            match bytes[i] {
                b'(' => depth += 1,
                b')' => depth -= 1,
                _ => {}
            }
            i += 1;
        }
        if depth != 0 {
            continue;
        }
        let p_list =rhs[open_pos + 1..i - 1].trim().to_string();
        result.push(FunctionCall {
            variable_type,
            variable_name,
            function_name,
            parameter_list:split_args(&p_list)
        });
    }
    result
}



    pub fn Extract_Persistent_Vars(fn_body:String,is_async: & HashSet<&String>) -> String {
        let mut ret = "".to_string();
        let fxn_call_table = parse_calls(&fn_body);
        let l = fxn_call_table.len();
        for i in 0..l {
            if is_async.contains(&fxn_call_table[i].function_name) {
                for k in &fxn_call_table[i].parameter_list{
                for j in 0..i{
                    if let Some(name) = fxn_call_table[j].variable_name.clone(){
                        if name == k.clone() {
                            if let Some(n_type) = fxn_call_table[j].variable_type.clone(){
                            ret += &format!("{n_type} {name};\n");
                            }
                        }
                    }
                }
            }
            }
        } 
        ret
    }


}

