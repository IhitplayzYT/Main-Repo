pub mod Helper {
    use std::sync::{LazyLock, RwLock};

    #[derive(Debug)]
    pub struct CLI {
        pub dbg_flag: bool,
        pub proj_dir: String,
        pub restore: bool,
        pub clean_run: bool,
    }

    const OK: i32 = 0;
    const ERR: i32 = 1;
    pub static MODEL: LazyLock<RwLock<String>> =
        LazyLock::new(|| RwLock::new("llama3.2".to_string()));
    const DBG_STR: &str = "
LangCompose - Fast JavaScript i18n Migration Tool

USAGE:
    LangCompose [OPTIONS] <PROJECT_DIR>

DESCRIPTION:
    LangCompose scans a JavaScript project, extracts hardcoded strings,
    wraps them with the translation function defined in i18n.js, generates
    translation locale files, and produces a ready-to-use multilingual
    project with minimal manual effort.

    During a normal run LangCompose will:
      • Scan the project recursively.
      • Detect translatable string literals.
      • Generate an i18n.js helper in the project root.
      • Wrap discovered strings using t(\"...\") macro.
      • Create a locales/ directory containing generated translations.

OPTIONS:
    -d, --debug
        Enable verbose debugging output.

    -r, --restore
        Restore the project to its original state by:
          • Removing generated locales.
          • Removing generated i18n.js.
          • Reverting modified source files.
          • Removing all LangCompose-generated changes.

    -m, --model
        Change the LLM model to be used

    -c, --clean-run
        Performs a clean migration by first executing --restore and then
        running LangCompose again on the project.

    -h, --help
        Display this help message and exit.

PROJECT DIRECTORY:
    The project directory may optionally contain:

      LangCompose
          Configuration file describing which language locales should
          be generated.

      .LangIgnore
          Lists directories and files that should be ignored while
          scanning the project.

EXAMPLES:
    langcompose ./MyProject

    langcompose --debug ./MyProject

    langcompose --restore ./MyProject

    langcompose --clean-run ./MyProject

OUTPUT:
    PROJECT/
    ├── i18n.js          Generated translation helper
    ├── locales/         Generated locale JSON files
    └── ...              Source files updated to use t(\"...\")

EXIT STATUS:
    0    Success
    else    Error during processing

-> LangCompose works with template strings as well
-> A background local instance of Ollama llama3.2 is required for generating the translations
For more information, visit:
https://github.com/IhitplayzYT/LangCompose
Author: Ihit Acharya
";

    impl CLI {
        pub fn new() -> CLI {
            Self {
                dbg_flag: false,
                proj_dir: String::new(),
                restore: false,
                clean_run: false,
            }
        }

        pub fn Parse_Args(&mut self) {
            let args = std::env::args().collect::<Vec<String>>();
            for i in &args {
                let i = i.trim();
                if &i[..] == "-d" || &i[..] == "--debug" || &i[..] == "-D" || &i[..] == "--DEBUG" {
                    self.dbg_flag = true;
                } else if &i[..] == "-r"
                    || &i[..] == "--restore"
                    || &i[..] == "-R"
                    || &i[..] == "--RESTORE"
                {
                    self.restore = true;
                } else if &i[..] == "-c"
                    || &i[..] == "--clean-run"
                    || &i[..] == "-C"
                    || &i[..] == "--CLEAN-RUN"
                {
                    self.clean_run = true;
                } else if &i[..] == "-h"
                    || &i[..] == "--help"
                    || &i[..] == "-H"
                    || &i[..] == "--HELP"
                {
                    self.clean_run = true;
                } else if i[..].starts_with("-m=")
                    || i[..].starts_with("--model=")
                    || i[..].starts_with("-M=")
                    || i[..].starts_with("--MODEL=")
                {
                    *MODEL.try_write().unwrap() = i[i.find("=").unwrap() + 1..].to_string();
                } else {
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
