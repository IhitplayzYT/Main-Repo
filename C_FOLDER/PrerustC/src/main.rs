use std::{env, process::exit,fs,io};
fn main() {
    let arg: Vec<String> = env::args().collect();
    if arg.len() != 2{
        exit(-1);
    }
    let mut file = fs::read_to_string(&arg[1]).unwrap_or_else(| e | {println!("{e}\n");exit(-1)});
    let mut ret:Vec<&str> = file.split_whitespace().collect();
    println!("{ret:?}\n");
    for i in ret{
        if i == "<"{
               
        }
    }

}
