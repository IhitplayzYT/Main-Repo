use std::{env, process::exit,fs};
fn main() {
    let arg: Vec<String> = env::args().collect();
    if arg.len() != 2{
        exit(-1);
    }
    let mut file = fs::read(&arg[1]).unwrap_or_else(|e| {println!("{e}"); exit(-1);});
    let mut ret:Vec<u8> = Vec::new();
    for i in 0..file.len(){
        if file[i] == file[i+1]  && file[i] == b'?'{
            
        }
        ret.push(file[i]);
    }


}
