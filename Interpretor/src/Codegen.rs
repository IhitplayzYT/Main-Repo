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
    use crate::{Ast::AST::{BIN_OP, Code, Declare, Expr, Statmnt, Type}, Errors::Err::{self, InterpretorReturn}};
    use std::{collections::HashMap, fmt::Display};
    use crate::Errors::Err::*;

    type Codegen_result<T> = Result<T,CodegenErr>;


    #[derive(Debug,Clone)]
    struct Codegen {
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
            Declare::Enum { name, variations } => {
                // TODO:
                // FIXME:
            },

        }
        Ok(())
    }

    pub fn Exec(&mut self,code: &Code) -> InterpretorReturn<bool>{
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

// TODO:
// FIXME: 

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
                }else{
                    return Err(InterpretorError::DivideByZero);
                } 
            },
            (Val::Float(a),Val::Float(b)) => {
                if b != 0.0 { 
                    return Ok(Val::Float(a/b));
                }else{
                    return Err(InterpretorError::DivideByZero);
                } 
            },
            (l,r) => Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}")))
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
        //TODO:
        //FIXME: 

        






        _ => {}

       } 
        


        Ok(Val::Null)
    }

    pub fn cmp_op(l:Val,r: Val,f: impl Fn(f64,f64) -> bool) -> InterpretorReturn<Val>{
        let (a,b) = match (l,r) {
            (Val::Int(c),Val::Int(d)) => ( c as f64, d as f64),
            (Val::Float(c),Val::Float(d)) => (c, d),
            _ => {return Err(InterpretorError::IncompatibleTypes(format!("{l:?}"), format!("{r:?}")))},
        };
        Ok(Val::Bool(f(a,b)))

    }

    pub fn eval_expr(&mut self,expr:&Expr) -> InterpretorReturn<Val>{

        Ok(Val::Null)
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
