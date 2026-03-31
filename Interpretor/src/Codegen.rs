// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Tokeniser.rs    //
// Contains the code for the Codegenerator

#![allow(
    non_camel_case_types,
    non_snake_case,
    non_upper_case_globals,
    dead_code
)]
pub mod Codegen {
    use crate::{Ast::AST::{BIN_OP, Code, Declare, Expr, Statmnt, Type, UN_OP}, Errors::Err::{InterpretorReturn}};
    use core::panic;
    use std::{collections::HashMap, fmt::Display, io};
    use crate::Errors::Err::*;

    type Codegen_result<T> = Result<T,CodegenErr>;


    #[derive(Debug,Clone)]
    pub struct Codegen {
        env: Env
        // TODO: Add maybe return value that the Exec fn may return to signal a fn main() -> int{}
        // FIXME:
    }

    #[derive(Debug,Clone)]
    struct Env{
       scopes: Scopes,
       func : HashMap<String,Declare> ,
       dtypes: HashMap<String,Vec<(String,Type)>>
    }

    pub enum CodegenErr{
        Fatal_err,
        Custom_err(String)
    }

    #[derive(Debug,Clone,PartialEq)]
    pub enum Val {
    Int(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Null,
    Custom(String,HashMap<String,Val>)
    }


    #[derive(Debug,Clone)]
    struct Scopes{
        scope: Vec<HashMap<String,(Val,bool)>>    
    }

    impl Display for Val{
     fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self{
        Val::Int(x) => {write!(f,"{x}")},
        Val::Float(x) => {write!(f,"{x}")},
        Val::String(x) =>  {write!(f,"{x}")},
        Val::Bool(x) =>  {write!(f,"{x}")},
        Val::Custom(x,_) =>  {write!(f,"<Object {x}>")},
        Val::Null =>  {write!(f,"Null")},
        }
     }

    }

    impl Scopes{ 
        fn new() -> Self{
           Self{
               scope:vec![HashMap::new()]
           }
        }

        fn push(&mut self) {
            self.scope.push(HashMap::new());
        }

        fn pop(&mut self) {
            self.scope.pop();
        }

        fn declare(&mut self,name: String,val: Val,mutable:bool) {
            self.scope.last_mut().unwrap().insert(name, (val,mutable));
        }

        fn is_declared(&self,name:String) -> bool{
           self.scope.iter().rev().any(|x| { x.contains_key(&name)}) 
        }

        fn get(&self,name: String) -> InterpretorReturn<Val>{
          for i in self.scope.iter().rev(){
            if let Some((ret,_)) = i.get(&name){
                return Ok(ret.clone());
            }
          } 
          Err(InterpretorError::UndefinedVariable(name))
        }

        fn set(&mut self,name: String, new:Val) -> InterpretorReturn<()>{
            for i in self.scope.iter_mut().rev(){
                if let Some((old,mutable)) = i.get_mut(&name) {
                    if !*mutable {
                        return Err(InterpretorError::ImmutableVariable(name));
                    }
                    *old = new;
                    return Ok(());
                }

            }
            Err(InterpretorError::UndefinedVariable(name))
        }



    } 



    #[derive(Debug,Clone)]
        pub enum ControlFlow{
        Return(Val),
        Break,
        Continue    
        }


        impl Env{
            fn new() -> Self{
                Self{
                    scopes: Scopes::new(),
                    func: HashMap::new(),
                    dtypes: HashMap::new(),
                }
            }
        }


    impl Codegen {

    pub fn new() -> Self{
        Self{
            env: Env::new()
        }
    }

    pub fn register(&mut self,decl: &Declare) -> InterpretorReturn<()> {
        match decl{
            Declare::Function {name,..} => {
                self.env.func.insert(name.clone(),decl.clone());
            },
            Declare::Struct { name,fields} => {
                self.env.dtypes.insert(name.clone(), fields.clone());
            },
            Declare::Enum { name:_name, variations:_variations } => {
                // TODO:
                // FIXME:
            },

        }
        Ok(())
    }

    pub fn Exec(&mut self,code: &Code) -> InterpretorReturn<bool>{
            self.register(&Declare::Function { name: "println".to_string(), rtype: None, args: vec![("fmt".to_string(),Type::STRING,false),("a".to_string(),Type::STRING,false)], body: vec![] })?;  
        for decl in &code.Program{
            self.register(decl)?;
        }        

        let _start = self.env.func.get("main").ok_or_else(|| InterpretorError::FunctionNotFound("main".to_string()))?.clone();
        if let Declare::Function {body ,..} = _start{
        let fn_body = body.clone();
        self.env.scopes.push();
        match self.exec_block(&fn_body) {
            Ok(_) |  Err(InterpretorError::ControlFault(ControlFlow::Return(_))) => {},
            Err(e) => return Err(e),
        }
        self.env.scopes.pop();
        }
        Ok(true)    
    }
    


    pub fn exec_block(&mut self,block: &Vec<Statmnt>) -> InterpretorReturn<Val>{
        block.iter().for_each(|statmnt| self.exec_statmnt(statmnt).unwrap());
        Ok(Val::Null)
    }

    pub fn exec_statmnt(&mut self,statmnt: &Statmnt) -> InterpretorReturn<()>{
        match statmnt{
            Statmnt::Let { name, mutable, value,..} => {
                let val = self.eval_expr(value)?;
                self.env.scopes.declare(name.clone(), val, *mutable);
            },
            Statmnt::Assignment { target, op, val } => {
                let value = self.eval_expr(val)?;
                self.try_assign(target,op,value)?;
            },
            Statmnt::If { cond, then_branch, else_branch } => {
                let cbool = self.eval_expr(cond)?;
                self.env.scopes.push();
                if Self::is_truthy(&cbool){
                    then_branch.iter().for_each(|f| self.exec_statmnt(f).unwrap());
                }else{
                    if let Some(branch) = else_branch{
                        branch.iter().for_each(|f| self.exec_statmnt(f).unwrap());
                    }
                }
                self.env.scopes.pop();



            },
            Statmnt::For { var_name, lb, rb, body } => {
                let (l,r) = (self.eval_expr(lb)?,self.eval_expr(rb)?);
                let (l,r) = match (l,r) {
                    (Val::Int(a),Val::Int(b)) => (a,b),
                    _ => return Err(InterpretorError::TypeError("For loops only allow <INT> TYPE".to_string()))
                };
                for idx in l..r {
                    self.env.scopes.push();
                    self.env.scopes.declare(var_name.clone(), Val::Int(idx), false);
                    match self.exec_block(body){
                        Ok(_) => {},
                        Err(InterpretorError::ControlFault(ControlFlow::Continue)) => {self.env.scopes.pop();continue;},
                        Err(InterpretorError::ControlFault(ControlFlow::Break)) => {self.env.scopes.pop();break;},
                        Err(e) => {self.env.scopes.pop();return Err(e);}
                    }
                    self.env.scopes.pop();
                }


            },
            Statmnt::Loop { body } => {
            loop {
                self.env.scopes.push();
                match self.exec_block(body){
                    Ok(_) => {},
                    Err(InterpretorError::ControlFault(ControlFlow::Continue)) => {self.env.scopes.pop();continue;},
                    Err(InterpretorError::ControlFault(ControlFlow::Break)) => {self.env.scopes.pop();break;},
                    Err(e) => {self.env.scopes.pop();return Err(e);}

                }
                self.env.scopes.pop();
            }
            },
            Statmnt::While { cond, body } => {
                loop {
                let cbool = self.eval_expr(cond)?;
                if !Self::is_truthy(&cbool) {
                    break;
                }
                self.env.scopes.push();
                match self.exec_block(body){
                    Ok(_) => {},
                    Err(InterpretorError::ControlFault(ControlFlow::Continue)) => {self.env.scopes.pop();continue;},
                    Err(InterpretorError::ControlFault(ControlFlow::Break)) => {self.env.scopes.pop();break;},
                    Err(e) => {self.env.scopes.pop();return Err(e);}

                }
                self.env.scopes.pop();

                }

            },
            Statmnt::Return(v) => {
                let val = match v {
                    Some(e) => self.eval_expr(e)?,
                    None => Val::Null
                };
                return Err(InterpretorError::ControlFault(ControlFlow::Return(val)));
            },
            Statmnt::Break => return Err(InterpretorError::ControlFault(ControlFlow::Break)),
            Statmnt::Continue => return Err(InterpretorError::ControlFault(ControlFlow::Continue)),
            Statmnt::Block(blk ) => {
            self.env.scopes.push();
            let r = self.exec_block(blk);
            self.env.scopes.pop();
            r?;
            },
            Statmnt::Expr(e) => {self.eval_expr(e)?;},
        }


        Ok(())
    }




    pub fn try_assign(&mut self,lhs:&Expr,bin_op:&Option<BIN_OP>,rhs:Val) -> InterpretorReturn<()>{
        match lhs {
            Expr::Ident(x) => {
                let fin = if let Some(bop) = bin_op {
                    let cur = self.env.scopes.get(x.clone())?;
                    Self::try_bin_op(bop,cur,rhs)?
                }else{ 
                    rhs
                };
                self.env.scopes.set(x.clone(), fin)?;
            },
            Expr::Field_access { obj, field } => {
                let member = Self::traverse_root(obj).ok_or(InterpretorError::InvalidAssignment)?;
                let mut cstm_val = self.env.scopes.get(member.clone())?;
                let fin_rhs = if let Some(bop) = bin_op{
                    let cur = Self::get_field_val(&cstm_val,field)?;
                    Self::try_bin_op(bop,cur,rhs)?
                } else{
                    rhs
                };

                Self::set_field_val(&mut cstm_val,obj,field,fin_rhs)?; // Updates the struct
                self.env.scopes.set(member.clone(), cstm_val)?;   // Updates our scopes[This scopes has members of structs also[..Flattened memory for faster serach in a scope]]

            }
            _ => {}
        };


        Ok(())
    }

    pub fn get_field_val(val:&Val,name: &String) -> InterpretorReturn<Val>{
        match val{
            Val::Custom(_,f) => f.get(name).cloned().ok_or_else(|| InterpretorError::NoFieldOnType(name.to_string(),"<Object>".to_string())),
            _ => Err(InterpretorError::Custom("Can't access non-object's fields".to_string()))
        }
    }

    pub fn set_field_val(val:&mut Val,expr:&Expr,name: &String,new: Val) -> InterpretorReturn<()>{
        match expr{
            Expr::Ident(_) => {
                if let Val::Custom(_, s_fields) = val{
                    s_fields.insert(name.to_string(), new);
                    Ok(())      
                }else{
                    Err(InterpretorError::Custom(name.to_string()))
                }
            },
            Expr::Field_access { obj, field } => {
                if let Val::Custom(_,s_fields ) = val {
                    let inner = s_fields.get_mut(field).ok_or_else(|| {InterpretorError::NoFieldOnType(field.to_string(), name.to_string())})?;
                    Self::set_field_val(inner, obj, name, new)
                }else{
                    Err(InterpretorError::TypeError("Can't access non-object fields".to_string()))
                } 
            },
            _ => Err(InterpretorError::TypeError("Can't assign to non-object fields".to_string()))


        }

    }

    pub fn traverse_root(root: &Expr) -> Option<String>{
        match root {
            Expr::Ident(x) => Some(x.clone()), 
            Expr::Field_access { obj, .. } => {Self::traverse_root(obj)},
            _ => None
        }
    }

    pub fn is_truthy(v: &Val) -> bool {
        match v {
           Val::Bool(true) => true,
           Val::Custom(_,f) => !f.is_empty(),
           Val::Int(x) => *x != 0,
           Val::Float(y) => *y != 0.0,
           Val::String(z) => z.len() > 0,
           _ => false
        }
    }


    pub fn try_bin_op(op: &BIN_OP,old: Val, new:Val) -> InterpretorReturn<Val> {
       match op {
        BIN_OP::Add => match (old,new) {
            (Val::Int(a),Val::Int(b)) => Ok(Val::Int(a+b)),
            (Val::Float(a),Val::Float(b)) => Ok(Val::Float(a+b)),
            (Val::String(a),Val::String(b)) => Ok(Val::String(a+&b)),
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}"))),
        },
        BIN_OP::Sub => match (old,new) {
            (Val::Int(a),Val::Int(b)) => Ok(Val::Int(a-b)),
            (Val::Float(a),Val::Float(b)) => Ok(Val::Float(a-b)),
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}"))),

        },BIN_OP::Mul => match (old,new) {
            (Val::Int(a),Val::Int(b)) => Ok(Val::Int(a*b)),
            (Val::Float(a),Val::Float(b)) => Ok(Val::Float(a*b)),
            (Val::String(z),Val::Int(cnt)) => Ok(Val::String(z.repeat(cnt as usize))),
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}"))),
        },BIN_OP::Div => match (old,new) {
            (Val::Int(a),Val::Int(b)) => {
                if b != 0 { 
                   return Ok(Val::Int(a/b));
                }
                Err(InterpretorError::DivideByZero)
            },
            (Val::Float(a),Val::Float(b)) => {
                if b != 0.0 { 
                    return Ok(Val::Float(a/b));
                }
                Err(InterpretorError::DivideByZero)
                 
            },
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}"))),
        },BIN_OP::Mod => match (old,new) {
            (Val::Int(a),Val::Int(b)) => {
                if b == 0 {
                    return Err(InterpretorError::DivideByZero);
                }
                return Ok(Val::Int(a%b));
            },
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}"))),
        },
        BIN_OP::Pow =>  match (old,new) {
            (Val::Int(a),Val::Int(b)) => Ok(Val::Int(a.pow(b as u32))),
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}"))),
        },
        BIN_OP::Eq => Ok(Val::Bool(old == new)),
        BIN_OP::N_eq => Ok(Val::Bool(old != new)),
        BIN_OP::Gt => Self::cmp_op(old,new,|a,b| a > b),
        BIN_OP::Lt => Self::cmp_op(old,new,|a,b| a < b),
        BIN_OP::Gt_eq => Self::cmp_op(old,new,|a,b| a >= b),
        BIN_OP::Lt_eq => Self::cmp_op(old,new,|a,b| a <= b),
        BIN_OP::Pipe => Self::bitwise_op(old,new,|a,b| {a | b}),
        BIN_OP::Xor => Self::bitwise_op(old,new,|a,b| {a ^ b}),
        BIN_OP::Amp => Self::bitwise_op(old,new,|a,b| {a & b}),
        BIN_OP::Lshift => Self::bitwise_op(old,new,|a,b| {a << b}),
        BIN_OP::Rshift => Self::bitwise_op(old,new,|a,b| {a >> b}),
        BIN_OP::Or => match (old,new) {
            (Val::Bool(x),Val::Bool(y)) => Ok(Val::Bool(x || y)),
            (x,y) => Err(InterpretorError::IncompatibleTypes(x.to_string(), y.to_string()))
        },
        BIN_OP::And => match (old,new) {
            (Val::Bool(x),Val::Bool(y)) => Ok(Val::Bool(x && y)),
            (x,y) => Err(InterpretorError::IncompatibleTypes(x.to_string(), y.to_string()))
        },

       } 
        


    }

    pub fn bitwise_op(l:Val,r:Val,f: impl Fn(i64,i64) -> i64) -> InterpretorReturn<Val>{
        match (l,r) {
            (Val::Int(x),Val::Int(y)) => {Ok(Val::Int(f(x,y)))},
            (Val::Bool(x),Val::Bool(y)) => {Ok(Val::Bool((f(x as i64,y as i64) & 1) == 1))}
            (x,y) => Err(InterpretorError::IncompatibleTypes(x.to_string(), y.to_string()))
        }

    }

    pub fn cmp_op(l:Val,r: Val,f: impl Fn(f64,f64) -> bool) -> InterpretorReturn<Val>{
        let (a,b) = match (l,r) {
            (Val::Int(c),Val::Int(d)) => ( c as f64, d as f64),
            (Val::Float(c),Val::Float(d)) => (c, d),
            (x,y) => {return Err(InterpretorError::IncompatibleTypes(format!("{x:?}"), format!("{y:?}")))},
        };
        Ok(Val::Bool(f(a,b)))

    }

    pub fn eval_expr(&mut self,expr:&Expr) -> InterpretorReturn<Val>{
        match expr {
            Expr::Bool(x) => Ok(Val::Bool(*x)),
            Expr::Int(x) => Ok(Val::Int(*x)),
            Expr::Float(x) => Ok(Val::Float(*x)),
            Expr::String(x) => Ok(Val::String(x.to_string())),
            Expr::Null => Ok(Val::Null),
            Expr::Ident(name) => self.env.scopes.get(name.to_string()),
            Expr::Binary_op { op, left, right } => {
                let (l,r) = (self.eval_expr(left)?,self.eval_expr(right)?);
                Self::try_bin_op(op, l, r)
            },Expr::Postdecr(x) => {
                let old= self.env.scopes.get(x.to_string() )?;
                let new = Self::add(old.clone(), -1.0)?;
                self.env.scopes.set(x.to_string(), new)?;
                Ok(old)
            },Expr::Predecr(x) => {
                let old= self.env.scopes.get(x.to_string() )?;
                let new = Self::add(old.clone(), -1.0)?;
                self.env.scopes.set(x.to_string(), new.clone())?;
                Ok(new)
            },Expr::Postincr(x) => {
                let old= self.env.scopes.get(x.to_string() )?;
                let new = Self::add(old.clone(), 1.0)?;
                self.env.scopes.set(x.to_string(), new)?;
                Ok(old)
            },Expr::Preincr(x) => {
                let old= self.env.scopes.get(x.to_string() )?;
                let new = Self::add(old.clone(), 1.0)?;
                self.env.scopes.set(x.to_string(), new.clone())?;
                Ok(new)
            },
            Expr::Unary_op { op, operand } => {
                let x = self.eval_expr(operand)?;
                match op {
                    UN_OP::Bang => {
                        match x {
                            Val::Bool(y) => Ok(Val::Bool(!y)),
                            t => Err(InterpretorError::IncompatibleTypes(t.to_string(), "Bool".to_string()))
                        }
                    },
                    UN_OP::Tilda => {
                        match x {
                            Val::Int(y) => Ok(Val::Int(!y)),
                            t => Err(InterpretorError::IncompatibleTypes(t.to_string(), "Bool".to_string()))
                        }
                    
                    },
                    UN_OP::Neg => {
                        match x {
                            Val::Float(y) => Ok(Val::Float(-y)),
                            Val::Int(y) => Ok(Val::Int(-y)),
                            Val::Bool(y) => Ok(Val::Bool(!y)),
                            t => Err(InterpretorError::IncompatibleTypes(t.to_string(), "Bool".to_string()))
                        }
                    },
                    

                }
            },
            Expr::Struct_enum_init { name, fields } => {
                let mut fmap = HashMap::new();
                for (f,expression) in fields{
                    fmap.insert(f.to_string(),self.eval_expr(expression)?);
                }

                Ok(Val::Custom(name.clone(),fmap))

            },

            Expr::Fxn_call { name, args } => {
                let mut args = args.clone();
                match name.as_str(){
                    "Println" | "Println!" | "println" | "println!" => {
                        if let Ok(Val::String(fmt)) = self.eval_expr(&args[0]){
                           let _ = args.iter_mut().map(|f| self.eval_expr(f).unwrap_or(Val::Null));
                           let mut segments = fmt.split("{}");
                           let mut ret = "".to_string();
                           for (i,part) in segments.by_ref().enumerate(){
                                ret.push_str(part);
                                if let Some(arg) = args.get(i){
                                    let val = self.eval_expr(arg)?;
                                    match val {
                                        Val::Bool(x) => ret.push_str(&x.to_string()),
                                        Val::Int(x) => ret.push_str(&x.to_string()),
                                        Val::String(x) => ret.push_str(&x),
                                        Val::Float(x) => ret.push_str(&x.to_string()),
                                        Val::Null => ret.push_str("None"),
                                        Val::Custom(x,y) => {
                                            ret.push_str(&x);
                                            ret.push_str("{");
                                               for (member,Val) in y {
                                                ret.push_str(&format!("{}:{}",member,match Val{
                                                    Val::Bool(x) => x.to_string(),
                                                    Val::Int(x) => x.to_string(),
                                                    Val::String(x) => x,
                                                    Val::Float(x) => x.to_string(),
                                                    Val::Null => "None".to_string(),
                                                    Val::Custom(m,_) => m,
                                                }));
                                               } 

                                            ret.push_str("}");
                                        }

                                    }
                                }
                           }
                            println!("{ret}\n");
                        }
                    },
                    "Print" | "Print!" | "print" | "print!" => {
                        if let Ok(Val::String(fmt)) = self.eval_expr(&args[0]){
                            let _ = args.iter_mut().map(|f| self.eval_expr(f).unwrap_or(Val::Null));
                           let mut segments = fmt.split("{}");
                           let mut ret = "".to_string();
                           for (i,part) in segments.by_ref().enumerate(){
                                ret.push_str(part);
                                if let Some(arg) = args.get(i){
                                    let val = self.eval_expr(arg)?;
                                    match val {
                                        Val::Bool(x) => ret.push_str(&x.to_string()),
                                        Val::Int(x) => ret.push_str(&x.to_string()),
                                        Val::String(x) => ret.push_str(&x),
                                        Val::Float(x) => ret.push_str(&x.to_string()),
                                        Val::Null => ret.push_str("None"),
                                        Val::Custom(x,y) => {
                                            ret.push_str(&x);
                                            ret.push_str("{");
                                               for (member,Val) in y {
                                                ret.push_str(&format!("{}:{}",member,match Val{
                                                    Val::Bool(x) => x.to_string(),
                                                    Val::Int(x) => x.to_string(),
                                                    Val::String(x) => x,
                                                    Val::Float(x) => x.to_string(),
                                                    Val::Null => "None".to_string(),
                                                    Val::Custom(m,_) => m,
                                                }));
                                               } 

                                            ret.push_str("}");
                                        }

                                    }
                                }
                           }
                            println!("{ret}");
                        }
                    },
                    "Scan" | "Scan!" | "scan" | "scan!"  => {
                        let la = args.len();
                        let mut cur = 0;
                        while cur < la {
                        let mut line = "".to_string();
                        io::stdin().read_line(&mut line).expect("STDIO failed FATAL!!");
                        let cli_args:Vec<String> = line.split_whitespace().map(|f| f.to_string()).collect();
                            for i in cli_args{
                                cur += self.try_read(args[cur].clone(),i)? as usize;
                            }                            
                        }

                    },
                    _ => {}

                };

                let decl = self.env.func.get(name).ok_or_else(|| InterpretorError::UndefinedVariable(name.clone()))?.clone();
                if let Declare::Function { args:params, body,.. } = &decl{
                    if args.len() != params.len(){
                        return Err(InterpretorError::Custom("Fxn Arg count mismatched".to_string()));
                    }
                    let mut eval: Vec<(String,Val,bool)> = Vec::new();
                    for (arg_expr,(param_name,_,mutable)) in args.iter().zip(params.iter()){
                        let val = self.eval_expr(arg_expr)?;
                        eval.push((param_name.clone(),val,*mutable));
                    }
                    let body = body.clone();
                    self.env.scopes.push();
                    for (pname,val,mutable) in eval{
                        self.env.scopes.declare(pname, val, mutable);
                    }
                    
                    let ret = match self.exec_block(&body){
                        Ok(_) => Val::Null,
                        Err(InterpretorError::ControlFault(ControlFlow::Return(v))) => v,
                        Err(e) => {self.env.scopes.pop();return Err(e);}
                    };
                    self.env.scopes.pop();
                    Ok(ret)
                }else{
                    Err(InterpretorError::UndefinedVariable(name.clone()))
                }
            },
            Expr::Field_access { obj, field } => {
                let val = self.eval_expr(obj)?;
                match val{
                    Val::Custom(_,fields) => {
                        fields.get(field).cloned().ok_or_else(|| InterpretorError::Custom(format!("Can't find field: {:?} on the Object",field.to_string())))
                    },
                    _ => Err(InterpretorError::Custom(format!("Can't find field: {:?} of type {:?} on Object",field.to_string(), val.to_string())))
                }
            },

        }
    }


                                
    pub fn try_read(&mut self,arg: Expr,i:String) -> InterpretorReturn<bool> {
        if let Expr::Ident(name) = arg{
            if self.env.scopes.is_declared(name.clone()){
                let data = Self::get_Val(&i);
                self.env.scopes.set(name,data)?;
            }else{
                panic!("Variable {name} is not declared and hence can't be read into!")
            }
        }
        Ok(true)
    }


    pub fn parse_json_struct(bs: &Vec<u8>) -> InterpretorReturn<(String,HashMap<String,Val>)>{
        let mut map = HashMap::new();
        let name = String::from_utf8(bs.iter().take_while(|b| **b != b'{').map(|x| {x.clone()}).collect::<Vec<u8>>()).or_else(|_f| {return Err(InterpretorError::Custom("Failed to parse invalid object".to_string()));})?;
        let mut i = 0;

        for z in bs{
            if *z != b'{'{
                i += 1;
            }else{
                break;
            }
        }

        let bs:Vec<u8> = bs[(i+1)..].to_vec();
        // We have skipped to after the first bracket... also we have to convert bytes to words and values
        let mut fie = "".to_string();
        let mut field_val = "".to_string();
        let mut off = true;
        for i in bs {
            if off{
                fie.push(i as char);
            }else{
                field_val.push(i as char);
            }
            if i == b' ' || i == b':'{
                off = false;
                fie.pop();
            }
            if i == b','{
                off = true;
                field_val.pop();
                map.insert(fie.clone(), Self::get_Val(&field_val));
                fie.clear();
                field_val.clear();
            }

        }

        Ok((name,map))
    }

    pub fn get_Val(x:&str) -> Val{
        if x == "None" || x == "Null" || x == "none" || x == "NULL" {
            return Val::Null;   // Deals with None
        }
        if x == "true" || x == "True" || x == "TRUE" { return Val::Bool(true);}      // Deals with bool
        if x == "false" || x == "False" || x == "FALSE" { return Val::Bool(false);}  // Deals with bool
        let y:Vec<u8> = x.to_string().bytes().collect();    
        if y.iter().all(|f| (*f == b'.') || (*f <= b'9' && *f >= b'0') || (*f == b'+') || (*f == b'-') || (*f == b'e') || (*f == b'E')) {
            let sgn_cnt = y.iter().filter(|x| **x == b'+' || **x == b'-').count();
            let e_cnt = y.iter().filter(|x| **x == b'e' || **x == b'E').count();
            if (e_cnt > 1) || (e_cnt == 1 && sgn_cnt >=2 ){
                panic!("Invalid numeric");
            }            
            if e_cnt == 1{
                if x.contains("."){
                    let z = x.to_string();
                    let po = if x.contains('e'){ x.find('e')} else {x.find('E')};
                    let po = po.unwrap();
                    return Val::Float(z[..po].parse::<f64>().expect("Exponent can't be decimal") * (10_i64.pow(z[(po+1)..].parse::<u32>().expect("Only positive integers allowed for powers")) as f64));
                }else{
                    let z = x.to_string();
                    let po = if x.contains('e'){ x.find('e')} else {x.find('E')};
                    let po = po.unwrap();
                    return Val::Int(z[..po].parse::<i64>().expect("Exponent can't be decimal") * 10_i64.pow(z[(po+1)..].parse::<u32>().expect("Only positive integers allowed for powers")));
                }
            }

            if y.contains(&b'.'){
                if (x.contains('+') && x.starts_with('+')) || (x.contains('-') && x.starts_with('-')) 
                    { 
                        return Val::Float(x.parse::<f64>().expect("Invalid float[Float can consist of . ,[1-9]]"));
                    }
            }else{
                return Val::Int(x.parse::<i64>().expect("Invalid int[Int can consist of [1-9]]"));
            }
        }
        if (x.starts_with("\"") && x.ends_with("\"")) || (x.starts_with("`") && x.ends_with("`")) || (x.starts_with("\'") && x.ends_with("\'")){
            return Val::String(x.to_string());
        }  
        if x.is_empty() {return Val::Null;}
        let (a,b) = Self::parse_json_struct(&y).expect("Invalid input type can't be asserted");
        Val::Custom(a, b)   
    }


    pub fn add(a: Val,b: f64) -> InterpretorReturn<Val>{
        match a {
            Val::Float(x) => {Ok(Val::Float(x+b))},
            Val::Int(x) => {Ok(Val::Int(x + (b as i64)))},
            Val::Bool(_) => {Ok(Val::Bool(if b < 0.0 {false} else {true}))}, 
            t => Err(InterpretorError::IncompatibleTypes(format!("{t:?}"), format!("{b}")))
        }

    }



    }






}
