#![allow(non_camel_case_types,non_snake_case,dead_code)]

pub mod Helper {
    pub const OK: i32 = 0;
    pub const ERR: i32 = 1;
    pub const DBG_STR: &str = "";

    #[derive(Debug,Clone,PartialEq)]
    pub struct CLI {
        pub targets: Vec<String>,
        pub dbg: bool,
    }

    impl CLI {
        pub fn new() -> CLI {
            Self {
                targets: vec![],
                dbg: false,
            }
        }
    }

    pub fn Help() {
        println!("{DBG_STR}");
        std::process::exit(OK);
    }

    pub fn Parse_Args() -> CLI {
        let mut ret = CLI::new();
        let clargs: Vec<String> = std::env::args().collect::<Vec<String>>();
        for i in &clargs {
            if &i[..] == "-h" || &i[..] == "--help" || &i[..] == "-H" || &i[..] == "--HELP" {
                Help();
            } else if &i[..] == "-d" || &i[..] == "--debug" || &i[..] == "-D" || &i[..] == "--DEBUG"
            {
                ret.dbg = true;
            } else {
                ret.targets.push(i.to_string());
            }
        }
        ret
    }
}
