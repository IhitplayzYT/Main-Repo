pub mod Helper{

#[derive(Debug)]
pub struct CLI{ 
pub dbg_flag: bool,
pub proj_dir: String
}

const OK:i32 = 0;
const ERR:i32 = 1;
const DBG_STR:&str = "";

impl CLI{
    pub fn new() -> CLI{
        Self{dbg_flag:false,proj_dir:String::new()}
    }

    pub fn Parse_Args(&mut self){
        let args = std::env::args().collect::<Vec<String>>().iter_mut().map(|x| x.to_lowercase()).collect::<Vec<String>>();
        for i in &args {
            if &i[..] == "-d" || &i[..] == "--debug"{
                self.dbg_flag = true;
            }
            else{
                self.proj_dir = i.to_string();
            } 
        }
    }


}

pub fn Help() {
    println!("{DBG_STR:?}");
    std::process::exit(OK);
}



}