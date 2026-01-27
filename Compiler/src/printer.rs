pub mod printer{
use crate::{Ast::AST::*,Tokeniser::Tokeniser::Lexer,Lexer_Tok::Lex_Tok::LTOK,Parser::PARSER::Parser};

pub trait Components{
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
LTOK::IDENT(x) => {x.clone()},
LTOK::STRING(x)  => {"\"".to_string() + x + "\""},
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
LTOK::LBRACE => {"{\n".to_string()},        
LTOK::RBRACE => {"}\n".to_string()},        
LTOK::LBRACK => {"[ ".to_string()},        
LTOK::RBRACK => {" ]".to_string()},        
LTOK::LPAREN => {"( ".to_string()},        
LTOK::RPAREN => {" )".to_string()},        
LTOK::SEMICOLON => {";\n".to_string()},     
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
if let Some(ast) = &self.ast {
for x in &ast.Program{
print_decl(x,0);
}

fn print_decl(decl: &Declare, indent: usize) {
    let prefix = "│   ".repeat(indent);
    let branch = if indent > 0 { "├── " } else { "" };
    
    match decl {
        Declare::Function { name, rtype, args, body } => {
            println!("{}{} Function: '{}'", prefix, branch, name);
            if !args.is_empty() {
                println!("{}│   ├── Parameters:", prefix);
                for (i, (pname, ptype)) in args.iter().enumerate() {
                    let last = i == args.len() - 1;
                    let connector = if last { "└──" } else { "├──" };
                    println!("{}│   │   {} {} : {:?}", prefix, connector, pname, ptype);
                }
            } else {
                println!("{}│   ├── Parameters: (none)", prefix);
            }
            
            if let Some(rt) = rtype {
                println!("{}│   ├── Return Type: {:?}", prefix, rt);
            } else {
                println!("{}│   ├── Return Type: void", prefix);
            }

            println!("{}│   └── Body: ({} statements)", prefix, body.len());
            for (i, stmt) in body.iter().enumerate() {
                let last = i == body.len() - 1;
                print_statement(stmt, indent + 2, last);
            }
        }
        
    }
}

fn print_statement(stmt: &Statmnt, indent: usize, is_last: bool) {
    let prefix = "│   ".repeat(indent);
    let connector = if is_last { "└──" } else { "├──" };
    
    match stmt {
        Statmnt::Let { name, mutable, type_annot, value } => {
            let mut_str = if *mutable { "mut " } else { "" };
            print!("{}{} Let {}'{}'", prefix, connector, mut_str, name);
            
            if let Some(t) = type_annot {
                print!(" : {:?}", t);
            }
            println!(" =");
            
            let expr_prefix = if is_last { "    " } else { "│   " };
            print_expression(value, indent, expr_prefix);
        }
        
        Statmnt::Assignment { name, op, val } => {
            if let Some(shorthand) = op {
                println!("{}{}️  Assignment: '{}' {:?}=", prefix, connector, name, shorthand);
            } else {
                println!("{}{}️  Assignment: '{}' =", prefix, connector, name);
            }
            
            let expr_prefix = if is_last { "    " } else { "│   " };
            print_expression(val, indent, expr_prefix);
        }
        
        Statmnt::If { cond, then_branch, else_branch } => {
            println!("{}{} If Statement", prefix, connector);
            
            println!("{}{}  ├── Condition:", prefix, if is_last { " " } else { "│" });
            let cond_prefix = if is_last { "    " } else { "│   " };
            print_expression(cond, indent, cond_prefix);
            
            println!("{}{}  ├── Then: ({} statements)", prefix, if is_last { " " } else { "│" }, then_branch.len());
            for (i, s) in then_branch.iter().enumerate() {
                let last = i == then_branch.len() - 1 && else_branch.is_none();
                print_statement(s, indent + 1, last);
            }
            
            if let Some(else_b) = else_branch {
                println!("{}{}   └── Else: ({} statements)", prefix, if is_last { " " } else { "│" }, else_b.len());
                for (i, s) in else_b.iter().enumerate() {
                    let last = i == else_b.len() - 1;
                    print_statement(s, indent + 1, last);
                }
            }
        }
        
        Statmnt::While { cond, body } => {
            println!("{}{} While Loop", prefix, connector);
            println!("{}{}  ├── Condition:", prefix, if is_last { " " } else { "│" });
            
            let cond_prefix = if is_last { "    " } else { "│   " };
            print_expression(cond, indent, cond_prefix);
            
            println!("{}{}  └── Body: ({} statements)", prefix, if is_last { " " } else { "│" }, body.len());
            for (i, s) in body.iter().enumerate() {
                let last = i == body.len() - 1;
                print_statement(s, indent + 1, last);
            }
        }
        
        Statmnt::For { var_name, lb, rb, body } => {
            println!("{}{} For Loop: '{}'", prefix, connector, var_name);
            
            println!("{}{}  ├── From:", prefix, if is_last { " " } else { "│" });
            let from_prefix = if is_last { "    " } else { "│   " };
            print_expression(lb, indent, from_prefix);
            
            println!("{}{}  ├── To:", prefix, if is_last { " " } else { "│" });
            let to_prefix = if is_last { "    " } else { "│   " };
            print_expression(rb, indent, to_prefix);
            
            println!("{}{}  └── Body: ({} statements)", prefix, if is_last { " " } else { "│" }, body.len());
            for (i, s) in body.iter().enumerate() {
                let last = i == body.len() - 1;
                print_statement(s, indent + 1, last);
            }
        }
        
        Statmnt::Loop { body } => {
            println!("{}{}️  Infinite Loop", prefix, connector);
            println!("{}{}  └── Body: ({} statements)", prefix, if is_last { " " } else { "│" }, body.len());
            for (i, s) in body.iter().enumerate() {
                let last = i == body.len() - 1;
                print_statement(s, indent + 1, last);
            }
        }
        
        Statmnt::Break => {
            println!("{}{} Break", prefix, connector);
        }
        
        Statmnt::Continue => {
            println!("{}{}️  Continue", prefix, connector);
        }
        
        Statmnt::Return(val) => {
            println!("{}{}️  Return", prefix, connector);
            if let Some(v) = val {
                let expr_prefix = if is_last { "    " } else { "│   " };
                print_expression(v, indent, expr_prefix);
            }
        }
        
        Statmnt::Expr(e) => {
            println!("{}{} Expression Statement", prefix, connector);
            let expr_prefix = if is_last { "    " } else { "│   " };
            print_expression(e, indent, expr_prefix);
        }
        
        Statmnt::Block(stmts) => {
            println!("{}{} Block: ({} statements)", prefix, connector, stmts.len());
            for (i, s) in stmts.iter().enumerate() {
                let last = i == stmts.len() - 1;
                print_statement(s, indent + 1, last);
            }
        }
    }
}

fn print_expression(expr: &Expr, indent: usize, extra_prefix: &str) {
    let base_prefix = "│   ".repeat(indent);
    let prefix = format!("{}{}", base_prefix, extra_prefix);
    
    match expr {
        Expr::Int(n) => {
            println!("{}└──  Int: {}", prefix, n);
        }
        
        Expr::Float(f) => {
            println!("{}└──  Float: {}", prefix, f);
        }
        
        Expr::String(s) => {
            println!("{}└──  String: \"{}\"", prefix, s);
        }
        
        Expr::Ident(name) => {
            println!("{}└──  Identifier: '{}'", prefix, name);
        }
        
        Expr::Binary_op { op, left, right } => {
            println!("{}└── BinaryOp: {:?}", prefix, op);
            println!("{}    ├── Left:", prefix);
            print_expression(left, indent + 1, "    ");
            println!("{}    └── Right:", prefix);
            print_expression(right, indent + 1, "    ");
        }
        
        Expr::Unary_op { op, operand } => {
            println!("{}└── UnaryOp: {:?}", prefix, op);
            print_expression(operand, indent + 1, "    ");
        }
        
        Expr::Fxn_call { name, args } => {
            println!("{}└── FunctionCall: '{}'", prefix, name);
            if args.is_empty() {
                println!("{}    └── Arguments: (none)", prefix);
            } else {
                println!("{}    └── Arguments: ({} args)", prefix, args.len());
                for (i, arg) in args.iter().enumerate() {
                    let last = i == args.len() - 1;
                    let arg_connector = if last { "└──" } else { "├──" };
                    println!("{}        {} Arg #{}:", prefix, arg_connector, i + 1);
                    print_expression(arg, indent + 2, "    ");
                }
            }
        },
        Expr::Null => {
            println!("Null ");
        }

    }

}
}

}
}
}
