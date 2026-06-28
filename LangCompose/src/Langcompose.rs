pub mod LangCompose {

    use std::{
        collections::{HashMap, HashSet}, fmt::format, fs, path::{Path, PathBuf}, process::exit, sync::{LazyLock, RwLock},
    };

    use regex::Regex;

static UNIQUE_MAP: LazyLock<RwLock<HashSet<String>>> = LazyLock::new(|| RwLock::new(HashSet::new()));

pub fn is_added(word:&str) -> bool{
    UNIQUE_MAP.try_read().unwrap().contains(word)
}

pub fn add(word:&str){
    UNIQUE_MAP.try_write().unwrap().insert(word.to_string());
}

pub fn remove(word:&str){
    UNIQUE_MAP.try_write().unwrap().remove(word);
}





    pub fn get_i18n(langs: Vec<String>) -> String {
        format!(
            "
const locales = {{
{},
}};

let translations = {{}};
let currentLang = \"en\";

export async function setLanguage(lang) {{
    if (!locales[lang]) {{
        lang = \"en\";
    }}

    const module = await locales[lang]();
    translations = module.default;
    currentLang = lang;

    localStorage.setItem(\"language\", lang);
}}

export function t(str) {{
    return translations[str] ?? str;
}}

export function getLanguage() {{
    return currentLang;
}}
",
            langs
                .iter()
                .map(|x| format!("{x}: () => import(\"./locales/{x}.json\")"))
                .collect::<Vec<String>>()
                .join(",\n")
        )
    }

    pub fn build_i18n(root: &str, langs: Vec<String>) {
        fs::write(Path::new(root).join("i18n.js"), get_i18n(langs)).unwrap();
    }

    pub fn extract_strs(fpath: &str) -> Vec<String> {
    let text = std::fs::read_to_string(fpath).unwrap();
    let re = Regex::new(r#"(?:^|[^\[])("[^"]*[a-zA-Z][^"]*"|'[^']*[a-zA-Z][^']*'|`[^`]*[a-zA-Z][^`]*`)"#).unwrap();
    re.captures_iter(&text)
        .map(|cap| cap[1].to_string())
        .collect()
}

    pub fn get_strings(root_dir: &str, ignores: &HashSet<String>) -> Vec<(String, Vec<String>)> {
        let mut ret = Vec::new();
        std::fs::read_dir(root_dir).unwrap().for_each(|x| {
            let k = x.unwrap();
            if !ignores.contains(k.path().to_str().unwrap()) {
                if k.path().is_dir() {
                    ret.append(&mut get_strings(k.path().to_str().unwrap(), ignores))
                } else {
                    ret.push((
                        k.path().to_str().unwrap().to_string(),
                        extract_strs(k.path().to_str().unwrap()),
                    ));
                }
            }
        });
        ret
    }

    pub fn get_langs(root: &str) -> Vec<String> {
        let fpath = Path::new(root).join("LangCompose");
        if fpath.is_file() {
            std::fs::read_to_string(fpath)
                .unwrap().trim()

                .split("\n")
                .map(|x| x.to_string())
                .collect::<Vec<String>>()
        } else {
            return vec![];
        }
    }

    pub fn get_langignore(root: &str) -> Vec<String> {
        let fpath = Path::new(root).join(".LangIgnore");
        let root_path = Path::new(root);
        let k;
        if fpath.is_file() {
            k = Some(
                std::fs::read_to_string(fpath)
                    .unwrap().trim()
                    .split("\n")
                    .map(|x| x.to_string())
                    .collect::<Vec<String>>(),
            );
        } else {
            return vec![];
        }
        if let Some(pnames) = k {
            return pnames
                .iter()
                .map(|x| {
                    let mo = Path::new(x);
                    if mo.is_absolute() {
                        mo.to_str().unwrap().to_string()
                    } else {
                        root_path.join(mo).to_str().unwrap().to_string()
                    }
                })
                .collect();
        }
        vec![]
    }

    pub fn build_dir(
        root: &str,
        og: &Vec<String>,
        lang_maps: Option<&HashMap<String, Vec<String>>>,
        langs: Option<&Vec<String>>,
    ) -> std::io::Result<()> {
        let bpath = Path::new(root).join("locales/");
        if !bpath.is_dir() {
         std::fs::create_dir(bpath.clone()).unwrap()
        }
            if let Some(x) = lang_maps {
                x.iter().for_each(|(a, b)| {
                    let fpath = bpath.join(a.to_string() + ".json");
                    let mut buff = "{".to_string();
                    for (old, new) in og.iter().zip(b.iter()) {
                        if is_added(old) {
                            continue;
                        }
                        let mut old = old[1..].to_string();
                        old.pop();
                        buff += &format!("\"{old}\":\"{new}\",\n");
                        add(&old[..]);
                    }
                    if buff.len() > 2 {
                        buff.pop();
                        buff.pop();
                    }
                    buff += "\n}";
                    if Path::new(&fpath).is_file(){
                        let mut old = fs::read_to_string(fpath.clone()).unwrap(); 
                        old.pop();
                        old.pop();
                        old += ",\n";
                        buff = old + &buff[1..];
                    }
                    fs::write(fpath, buff).unwrap();
                });
            }
            if let Some(x) = langs {
                let fpath = bpath.join("temp.json");
                let mut buff = "{".to_string();
                for word in x {
                    buff += &format!("\"{word}\":\"\",\n");
                }
                if buff.len() > 2 {
                    buff.pop();
                    buff.pop();
                }
                buff += "\n}";
                fs::write(fpath, buff).unwrap();
            }

        Ok(())
    }

    pub fn i18n_import_path(project_root: &Path, file_path: &Path) -> String {
        let file_dir = file_path.parent().unwrap();

        let relative = file_dir.strip_prefix(project_root).unwrap();

        let depth = relative.components().count();

        let mut path = PathBuf::new();

        if depth == 0 {
            path.push("./i18n.js");
        } else {
            for _ in 0..depth {
                path.push("..");
            }
            path.push("i18n.js");
        }

        path.to_string_lossy().replace('\\', "/")
    }

    pub fn convert_template_literal(s: &str) -> String {
        let re = Regex::new(r"\$\{([A-Za-z_][A-Za-z0-9_]*)\}").unwrap();

        let mut out = format!("t(\"{}\")", s);

        for cap in re.captures_iter(s) {
            let name = &cap[1];
            out.push_str(&format!("\n    .replace(\"${{{}}}\", {})", name, name));
        }

        out
    }

    pub fn update_files(root: &str, fmap: Vec<(String, Vec<String>)>) {
        for (fpath, list) in &fmap {
            let mut buff = std::fs::read_to_string(fpath.clone()).unwrap();
            for i in list {
                if i.starts_with("`") {
                    let mut i = (&i[1..]).to_string();
                    i.pop();
                    buff = buff.replace(&i[..], &convert_template_literal(&i[..])[..]);
                } else {
                    let mut m = (&i[1..]).to_string();
                    m.pop().unwrap();
                    buff = buff.replace(i, &format!("t(\"{m}\")",)[..]);
                }
            }
            let updated = format!(
                "import {{ setLanguage, t, getLanguage}} from \"{}\";\n",
                i18n_import_path(Path::new(root), Path::new(fpath))
            ) + &buff;
            fs::write(fpath, updated).unwrap();
        }
    }


fn find_fn_t(text: &str) -> Vec<usize> {
    let re = Regex::new(r#"t\("([^"\\]|\\.)*"\)"#).unwrap();

    re.find_iter(text)
        .map(|m| m.start())
        .collect()
}

fn remove_replaces(s: &str) -> String {
    let re = Regex::new(
        r#"(?m)^[ \t]*\.replace\("\$\{[^"]+\}",\s*[^)]+\)\r?\n?"#
    ).unwrap();

    re.replace_all(s, "").into_owned()
}


static PLACEHOLDER_RE: LazyLock<Regex> =
    LazyLock::new(|| Regex::new(r"\$\{[^}]+\}").unwrap());

fn contains_placeholder(s: &str) -> bool {
    PLACEHOLDER_RE.is_match(s)
}

    pub fn restore(root:&str){
        fs::remove_dir(Path::new(root).join("locales")).unwrap();
        fs::remove_file(Path::new(root).join("i18n.js")).unwrap();
        fs::read_dir(Path::new(root)).unwrap().for_each(|x| {
            let dirent = x.unwrap();
            if dirent.path().is_dir(){
                restore(dirent.path().to_str().unwrap());
            }else if dirent.path().is_file(){
               let mut buff = remove_replaces(&fs::read_to_string(dirent.path()).unwrap()[..]);
            if let Some(i_idx) = buff.find("import { setLanguage, t, getLanguage} from "){
                let i_end = buff[i_idx..].find(";").unwrap();
                buff = buff.replace(&buff[i_idx..i_idx+i_end+1 + if buff[i_idx+i_end+2..].starts_with("\n") {1}else{0}],"");
                


            find_fn_t(&buff[..]).into_iter().for_each(|idx| {
                let start = idx;
                let fn_t_end = &buff[start..].find(")").unwrap();
                let old = &buff[start..fn_t_end+1];
                let mut new = (&old[2..]).to_string();
                new.pop();
                if contains_placeholder(&new[..]){
                new = format!("`{}`",&new[1..new.len()-1]);
                }
                buff = buff.replace(old,&new);
            });
            fs::write(dirent.path(), buff).unwrap();
        }
            }else{
                std::process::exit(2);
            }
        });

    }
}
 