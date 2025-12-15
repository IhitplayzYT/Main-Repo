pub mod Lex_Tok {
    // For compound assignment operators
    #[derive(Debug, Clone)]
    pub enum LTOK {
        LET,             // -> let (Varib Declaring)
        MUT,             // -> mut (mutability declaring)
        CONST,           // -> const (For constants)
        IF,              // -> (If conditional for Statement && Expr)
        ELSE,            // -> (Else conditional for Statement && Expr)
        WHILE,           // -> (Conditional Loop over Iterable)
        FOR,             // ->  (Iterative Loop over Iterable)
        FN,              // -> (Function Declaration KeyWord)
        LITERAL(String), // -> Can be any number string or variable
        PLUS,            // -> (+)
        MINUS,           // -> (-)
        DIV,             // -> (/)
        MODULO,          // -> (%)
        STAR,            // -> (*)
        ASSGN,           // -> (=)
        EQ,              // -> (==)
        LT_EQ,           // -> (<=)
        GT_EQ,           // -> (>=)
        N_EQ,            // -> (!=)
        LT,              // -> (<)
        GT,              // -> (>)
        S_PLUS,          // -> (+=)
        S_MINUS,         // -> (+=)
        S_MULT,          // -> (+=)
        S_DIV,           // -> (+=)
        S_MOD,           // -> (+=)
        S_AMP,           // -> (+=)
        S_PIPE,          // -> (+=)
        S_CARET,         // -> (+=)
        RSHIFT,          // -> (>>)
        LSHIFT,          // -> (<<)
        AMP,             // -> (&)
        CARET,           // -> (^)
        PIPE,            // -> (|)
        TILDA,           // -> (~)
        ANDAND,          // -> (&&)
        OROR,            // -> (||)
        BANG,            // -> (!)
        LBRACE,          // -> {
        RBRACE,          // -> }
        LBRACK,          // -> [
        RBRACK,          // -> ]
        LPAREN,          // -> (
        RPAREN,          // -> )
        SEMICOLON,       // -> (;)
        COLON,           // -> (:)
        COMMA,           // -> (,)
        NULL,            // -> (None/Null)
        EOF,             // -> ('')
        QUOTE,           // -> (')
        DQUOTE,          // -> (")
    }
}
