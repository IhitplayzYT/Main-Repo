//VARIB(String),  // -> (Variable)
//        INT(i64),       // -> (Integer Literal)
//        FLOAT(f64),     // -> (Floating point Literal)
//        STRING(String), // -> (String Literal)
//        BOOL(bool),     // -> (Boolean Literal)
//        CHAR(char),     // -> (Character Literal)
//
//

pub mod Tokens {

    pub enum Keyword {
        LET,
        IF,
        ELSE,
        CONST,
        MUT,
        USE,
        FN,
        LOOP,
        WHILE,
        FOR,
    }

    pub enum Arithemetic {
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        MODULO,
        LSHIFT,
        RSHIFT,
        AND,
        OR,
        NOT,
        XOR,
        ASSGN,
    }

    pub enum Logical {
        AND,
        OR,
        NOT,
    }

    pub enum Comparison {
        LT,
        GT,
        EQ,
        N_EQ,
        LT_EQ,
        GT_EQ,
    }

    pub enum Token {
        Keyword,
        Variable(String),
        Arithemetic,
        Logical,
        Comparison,
        Literal,
        SEMICOLON,
        EOF,
    }
}
