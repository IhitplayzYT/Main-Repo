pub mod printer{
use crate::{Ast::AST::*,Tokeniser::Tokeniser::Lexer,Lexer_Tok::Lex_Tok::LTOK,Parser::PARSER::Parser};

impl Lexer{

fn print(&self){
for &i in &self.Lexer_Output{
print!("{}",match i{
LTOK::LET =>  {"let "}  
LTOK::MUT => {"mut "}, 
LTOK::CONST => {"const "},        
LTOK::IF => {"if "},            
LTOK::ELSE => {"else "},          
LTOK::WHILE => {"while "},         
LTOK::LOOP => {"loop "},          
LTOK::FOR => {"for "},           
LTOK::FN => {"fn "},            
LTOK::BREAK => {"break"},         
LTOK::CONTINUE => {"continue"},      
LTOK::RETURN => {"return "},        
LTOK::IDENT(x) | LTOK::STRING(x) => {x.as_str()},
LTOK::INT(x) => {let z = format!("{}",x);z.as_str()},
LTOK::FLOAT(x) => {let z = format!("{}",x);z.as_str()},
LTOK::PLUS => {" + "},          
LTOK::MINUS => {" - "},         
LTOK::DIV => {" / "},           
LTOK::MODULO => {" % "},        
LTOK::STAR => {" * "},          
LTOK::ASSGN => {" = "},         
LTOK::EQ => {" == "},            
LTOK::LT_EQ => {" <= "},         
LTOK::GT_EQ => {" >= "},         
LTOK::N_EQ => {" != "},          
LTOK::LT => {" < "},            
LTOK::GT => {" > "},            
LTOK::S_PLUS => {" += "},        
LTOK::S_MINUS => {" -= "},       
LTOK::S_MULT => {" *= "},        
LTOK::S_DIV => {" /= "},         
LTOK::S_MOD => {" %= "},         
LTOK::S_AMP => {" &= "},         
LTOK::S_PIPE => {" |= "},        
LTOK::S_CARET => {" ^= "},       
LTOK::RSHIFT => {" >>= "},        
LTOK::LSHIFT => {" <<= "},        
LTOK::AMP => {" & "},           
LTOK::CARET => {" ^ "},         
LTOK::PIPE => {" | "},          
LTOK::TILDA => {"~"},         
LTOK::ANDAND => {" && "},        
LTOK::OROR => {" || "},          
LTOK::BANG => {"!"},          
LTOK::LBRACE => {"{{ "},        
LTOK::RBRACE => {" }}"},        
LTOK::LBRACK => {"[ "},        
LTOK::RBRACK => {" ]"},        
LTOK::LPAREN => {"( "},        
LTOK::RPAREN => {" )"},        
LTOK::SEMICOLON => {""},     
LTOK::COLON => {": "},         
LTOK::COMMA => {", "},         
LTOK::NULL => {" None"},          
LTOK::EOF => {"\n"},           
LTOK::QUOTE => {"'"},         
LTOK::DQUOTE => {"\""},        
LTOK::ARROW => {" -> "},         
LTOK::INT_TYPE => {"int "},      
LTOK::FLOAT_TYPE => {"float "},    
LTOK::STRING_TYPE => {"string "},   
_ => std::process::exit(-1),
}
);
}
}






}




}
