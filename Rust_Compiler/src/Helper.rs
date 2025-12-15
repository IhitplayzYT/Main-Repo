// Helper.rs //
// Provides some required basic helper functions.

use std::fs;

pub mod collections {
    use std::{
        collections::{HashMap, HashSet},
        hash::Hash,
    };

    pub fn Counter<T: Hash + Eq + Clone>(vect: &Vec<T>) -> HashMap<T, usize> {
        let mut ret: HashMap<T, usize> = HashMap::new();
        for i in vect {
            *ret.entry(i.clone()).or_insert(0) += 1;
        }
        ret
    }

    pub fn Set<T: Hash + Eq + Clone>(vect: &Vec<T>) -> HashSet<T> {
        let mut ret: HashSet<T> = HashSet::new();
        for i in vect {
            ret.insert(i.clone());
        }
        ret
    }
}

pub mod utilities {
    use std::fs;

    pub fn read_file(x: &str) -> std::io::Result<String> {
        fs::read_to_string(x)
    }
}
