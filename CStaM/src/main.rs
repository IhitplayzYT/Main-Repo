use crate::{cstam::Cstam, helper::Helper};

mod cstam;
mod helper;

fn main() {
    let args = Helper::Parse_Args();
    if args.dbg {
        println!("{:?}",args);
    }
    for i in args.targets{
        Cstam::process_file(i);
    }

}
