pub mod Tokeniser {
    use std::collections::HashMap;

    use crate::Lexer_Tok::Lex_Tok::LTOK;
    use once_cell::sync::Lazy;

    pub static ALLOWED_KEYWORDS: Lazy<HashMap<&'static str, LTOK>> = Lazy::new(|| {
        HashMap::from([
            ("let", LTOK::LET),
            ("mut", LTOK::MUT),
            ("const", LTOK::CONST),
            ("if", LTOK::IF),
            ("else", LTOK::ELSE),
            ("while", LTOK::WHILE),
            ("for", LTOK::FOR),
            ("fn", LTOK::FN),
        ])
    });
    pub fn resolve_ltok(v: &str) -> Option<LTOK> {
        if v.is_empty() {
            return None;
        }
        if let Some(v) = ALLOWED_KEYWORDS.get(v) {
            return Some(v.clone());
        } else {
            Some(match v {
                "+" => LTOK::PLUS,
                "-" => LTOK::MINUS,
                "/" => LTOK::DIV,
                "%" => LTOK::MODULO,
                "*" => LTOK::STAR,
                "=" => LTOK::ASSGN,
                "==" => LTOK::EQ,
                "<=" => LTOK::LT_EQ,
                ">=" => LTOK::GT_EQ,
                "!=" => LTOK::N_EQ,
                "+=" => LTOK::S_PLUS,
                "-=" => LTOK::S_MINUS,
                "*=" => LTOK::S_MULT,
                "/=" => LTOK::S_DIV,
                "%=" => LTOK::S_MOD,
                "<<" => LTOK::LSHIFT,
                ">>" => LTOK::RSHIFT,
                "&" => LTOK::AMP,
                "^" => LTOK::CARET,
                "|" => LTOK::PIPE,
                "~" => LTOK::TILDA,
                "&&" => LTOK::ANDAND,
                "||" => LTOK::OROR,
                "!" => LTOK::BANG,
                "{" => LTOK::LBRACE,
                "}" => LTOK::RBRACE,
                "[" => LTOK::LBRACK,
                "]" => LTOK::RBRACK,
                "(" => LTOK::LPAREN,
                ")" => LTOK::RPAREN,
                ";" => LTOK::SEMICOLON,
                ":" => LTOK::COLON,
                "," => LTOK::COMMA,
                "None" => LTOK::NULL,
                "Null" => LTOK::NULL,
                "NULL" => LTOK::NULL,
                "\"" => LTOK::DQUOTE,
                "\'" => LTOK::QUOTE,
                _ => LTOK::LITERAL(v.to_string()),
            })
        }
    }

    pub fn Tokenise(x: String) -> Vec<LTOK> {
        let mut ret: Vec<LTOK> = Vec::new();
        let mut iter = x.chars().peekable();
        let mut temp = String::new();
        while let Some(i) = iter.next() {
            if i == ' ' || i == '\n' {
                if let Some(v) = resolve_ltok(&temp[..]) {
                    ret.push(v);
                } else {
                }
                temp.clear();
                continue;
            } else if i == '('
                || i == '['
                || i == '{'
                || i == ')'
                || i == '}'
                || i == ']'
                || i == ':'
                || i == ';'
                || i == ','
                || i == '\''
                || i == '\"'
                || i == '~'
            {
                if let Some(k) = resolve_ltok(&temp[..]) {
                    ret.push(k);
                }
                if let Some(k) = resolve_ltok(&i.to_string()[..]) {
                    ret.push(k);
                }
                temp.clear();
            } else if i == '+' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::S_PLUS);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::PLUS);
                    temp.clear();
                }
            } else if i == '-' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::S_MINUS);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::MINUS);
                    temp.clear();
                }
            } else if i == '*' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::S_MULT);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::STAR);
                    temp.clear();
                }
            } else if i == '/' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::S_DIV);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::DIV);
                    temp.clear();
                }
            } else if i == '%' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::S_MOD);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::MODULO);
                    temp.clear();
                }
            } else if i == '=' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::EQ);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::ASSGN);
                    temp.clear();
                }
            } else if i == '!' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::N_EQ);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::BANG);
                    temp.clear();
                }
            } else if i == '>' {
                if let Some(v) = iter.next() {
                    match v {
                        '=' => ret.push(LTOK::GT_EQ),
                        '>' => ret.push(LTOK::RSHIFT),
                        _ => continue,
                    };
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::GT);
                    temp.clear();
                }
            } else if i == '<' {
                if let Some(v) = iter.next() {
                    match v {
                        '=' => ret.push(LTOK::LT_EQ),
                        '<' => ret.push(LTOK::LSHIFT),
                        _ => continue,
                    };
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::LT);
                    temp.clear();
                }
            } else if i == '&' {
                if let Some(v) = iter.next() {
                    match v {
                        '=' => ret.push(LTOK::S_AMP),
                        '&' => ret.push(LTOK::ANDAND),
                        _ => continue,
                    };
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::AMP);
                    temp.clear();
                }
            } else if i == '|' {
                if let Some(v) = iter.next() {
                    match v {
                        '=' => ret.push(LTOK::S_PIPE),
                        '|' => ret.push(LTOK::OROR),
                        _ => continue,
                    };
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::PIPE);
                    temp.clear();
                }
            } else if i == '^' {
                if let Some('=') = iter.next() {
                    ret.push(LTOK::S_CARET);
                    temp.clear();
                    iter.next();
                } else {
                    ret.push(LTOK::CARET);
                    temp.clear();
                }
            } else {
                temp.push(i);
            }
        }

        if !temp.is_empty() {
            if let Some(d) = resolve_ltok(&temp[..]) {
                ret.push(d);
                temp.clear();
            } else {
            }
        }
        ret.push(LTOK::EOF);
        ret
    }
}
