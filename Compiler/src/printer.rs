pub mod printer{
use crate::{Ast::AST::*,Tokeniser::Tokeniser::Lexer,Lexer_Tok::Lex_Tok::LTOK,Parser::PARSER::Parser};

trait Components{
fn print(&self);
}


impl Components for Lexer{
fn print(&self){
for i in &self.Lexer_Output{
print!("{}",match i{
LTOK::LET =>  {"let ".to_string()}  
LTOK::MUT => {"mut ".to_string()}, 
LTOK::CONST => {"const ".to_string()},        
LTOK::IF => {"if ".to_string()},            
LTOK::ELSE => {"else ".to_string()},          
LTOK::WHILE => {"while ".to_string()},         
LTOK::LOOP => {"loop ".to_string()},          
LTOK::FOR => {"for ".to_string()},           
LTOK::FN => {"fn ".to_string()},      
LTOK::IN => {" in ".to_string()},
LTOK::BREAK => {"break".to_string()},         
LTOK::CONTINUE => {"continue".to_string()},      
LTOK::RETURN => {"return ".to_string()},        
LTOK::IDENT(x) | LTOK::STRING(x) => {x.clone()},
LTOK::INT(y) => format!("{:?}",y),
LTOK::FLOAT(z) => format!("{:?}",z),
LTOK::PLUS => {" + ".to_string()},          
LTOK::MINUS => {" - ".to_string()},         
LTOK::DIV => {" / ".to_string()},           
LTOK::MODULO => {" % ".to_string()},        
LTOK::STAR => {" * ".to_string()},          
LTOK::ASSGN => {" = ".to_string()},         
LTOK::EQ => {" == ".to_string()},            
LTOK::LT_EQ => {" <= ".to_string()},         
LTOK::GT_EQ => {" >= ".to_string()},         
LTOK::N_EQ => {" != ".to_string()},          
LTOK::LT => {" < ".to_string()},            
LTOK::GT => {" > ".to_string()},            
LTOK::S_PLUS => {" += ".to_string()},        
LTOK::S_MINUS => {" -= ".to_string()},       
LTOK::S_MULT => {" *= ".to_string()},        
LTOK::S_DIV => {" /= ".to_string()},         
LTOK::S_MOD => {" %= ".to_string()},         
LTOK::S_AMP => {" &= ".to_string()},         
LTOK::S_PIPE => {" |= ".to_string()},        
LTOK::S_CARET => {" ^= ".to_string()},       
LTOK::RSHIFT => {" >>= ".to_string()},        
LTOK::LSHIFT => {" <<= ".to_string()},        
LTOK::AMP => {" & ".to_string()},           
LTOK::CARET => {" ^ ".to_string()},         
LTOK::PIPE => {" | ".to_string()},          
LTOK::TILDA => {"~".to_string()},         
LTOK::ANDAND => {" && ".to_string()},        
LTOK::OROR => {" || ".to_string()},          
LTOK::BANG => {"!".to_string()},          
LTOK::LBRACE => {"{ ".to_string()},        
LTOK::RBRACE => {" }".to_string()},        
LTOK::LBRACK => {"[ ".to_string()},        
LTOK::RBRACK => {" ]".to_string()},        
LTOK::LPAREN => {"( ".to_string()},        
LTOK::RPAREN => {" )".to_string()},        
LTOK::SEMICOLON => {"".to_string()},     
LTOK::COLON => {": ".to_string()},         
LTOK::COMMA => {", ".to_string()},         
LTOK::NULL => {"None".to_string()},          
LTOK::EOF => {"\n".to_string()},           
LTOK::QUOTE => {"'".to_string()},         
LTOK::DQUOTE => {"\"".to_string()},        
LTOK::ARROW => {" -> ".to_string()},         
LTOK::INT_TYPE => {"int ".to_string()},      
LTOK::FLOAT_TYPE => {"float ".to_string()},    
LTOK::STRING_TYPE => {"string ".to_string()},   
_ => std::process::exit(-1),
}
);
}
}
}

impl Components for Parser{
fn print(&self) {

}
}




}
