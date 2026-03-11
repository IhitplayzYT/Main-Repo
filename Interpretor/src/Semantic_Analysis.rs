// SPDX-License-Identifier: GPL-3.0-only
//
// Copyright (C) 2025 Ihit Acharya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.

//    Semantic_Analysis.rs    //
// Contains the code for the Semantic Analyser

#![allow(
    non_camel_case_types,
    non_snake_case,
    non_upper_case_globals,
    unused_imports,
    dead_code
)]
pub mod Analyser {
    use crate::{
        Ast::{self, AST::*},
        Errors::Err::{self, Parser_ret, Semantic_Ret, Semantic_err},
    };
    use std::collections::HashMap;

    /// Semanilizer structure[MAIN API]
    ///
    /// # Traits
    /// - Derived<br/>
    ///     - Debug
    ///     - Clone
    /// # Example
    /// ```
    /// Semantilizer{
    ///     symbol_table: HashMap<String,Type>,
    ///     function_map: HashMap<String,(Vec<Type>,Option<Type>)>,
    ///     in_loop : usize,                                       
    /// }
    /// ```    

    #[derive(Debug, Clone)]
    pub struct Semantilizer {
        variables: HashMap<String, (Type, bool)>, // Variable name -> (type, is_mutable)
        functions: HashMap<String, (Vec<Type>, Option<Type>)>,
        structs: HashMap<String, Vec<(String, Type)>>,
        enums: HashMap<String, Vec<EnumVars>>,
        current_function_return: Option<Type>,
        loop_depth: usize,
        scope_stack: Vec<HashMap<String, (Type, bool)>>,
    }

    impl Semantilizer {
        /// Semanilizer Constructor
        ///
        /// # Example
        /// ```
        /// Semantilizer::new();
        /// ```    

        pub fn new() -> Self {
            Self {
                variables: HashMap::new(),
                functions: HashMap::new(),
                structs: HashMap::new(),
                enums: HashMap::new(),
                current_function_return: None,
                loop_depth: 0,
                scope_stack: vec![HashMap::new()],
            }
        }

        /// Semanilizer main call
        ///
        /// # Return
        ///  Returns a Semnatic ret with useless return
        /// # Example
        /// ```
        /// semantic_analyse.analyse()?;
        /// ```    

        pub fn analyse(&mut self, code: &Code) -> Semantic_Ret<()> {
            for decl in &code.Program {
                self.save_declares(decl)?;
            }

            for decl in &code.Program {
                self.analyze_decl(decl)?;
            }

            if !self.functions.contains_key("main") {
                return Err(Semantic_err::UndefinedFunction("main".to_string()));
            }

            Ok(())
        }

        /// Semantilizer helper fxn
        ///
        /// Enters a scope
        ///
        /// # Example
        /// ```
        /// self.enter_scope();
        /// ```
        ///

        fn enter_scope(&mut self) {
            self.scope_stack.push(HashMap::new());
        }

        /// Semantilizer helper fxn
        ///
        /// Exits a scope
        ///
        /// # Example
        /// ```
        /// self.exit_scope();
        /// ```
        ///

        fn exit_scope(&mut self) {
            self.scope_stack.pop();
        }

        /// Semantilizer helper fxn
        ///
        /// Declares a variable in the current scope
        /// # Arguments
        /// - name : String -> Name of the variable being declared
        /// - type_ : Type -> The datatype of the variable we are declaring
        /// - mutable : bool -> Whether the variable is mutable or not  
        ///
        /// # Returns
        /// On success emoty tuple else throws semantic_err
        ///
        ///
        /// # Example
        /// ```
        /// self.declare_varib("a".to_string(),Type::Int,false)?;
        /// ```
        ///

        fn declare_varib(&mut self, name: String, type_: Type, mutable: bool) -> Semantic_Ret<()> {
            let cur_scope = self.scope_stack.last_mut().unwrap();
            if cur_scope.contains_key(&name) {
                return Err(Semantic_err::RedefinedVariable(name));
            }
            cur_scope.insert(name, (type_, mutable));
            Ok(())
        }

        /// Semantilizer helper fxn
        ///
        /// Declares a variable in the current scope
        /// # Arguments
        /// - name : String -> Name of the variable being serached for
        ///
        /// # Returns
        /// On success returns (Type,bool) where they are (type of varib,mutablility) of the
        /// variable onm error thorws semantic_err
        ///
        ///
        /// # Example
        /// ```
        /// self.get_varib("a".to_string())?;
        /// ```
        ///

        fn get_varib(&self, name: &str) -> Semantic_Ret<(Type, bool)> {
            for sc in self.scope_stack.iter().rev() {
                if let Some(var) = sc.get(name) {
                    return Ok(var.clone());
                }
            }
            Err(Semantic_err::UndefinedVariable(name.to_string()))
        }

        fn update_varib(&mut self, name: &str) -> Semantic_Ret<()> {
            let (_, is_mutable) = self.get_varib(name)?;
            if !is_mutable {
                return Err(Semantic_err::Immutable_Variable(name.to_string()));
            }

            Ok(())
        }

        /// Semanilizer function to save any function declarations
        ///
        /// # Return
        ///  Returns a Semnatic ret with useless return
        /// # Example
        /// ```
        /// semantic_analyse.save_declares()?;
        /// ```    

        pub fn save_declares(&mut self, decl: &Declare) -> Semantic_Ret<()> {
            match decl {
                Declare::Function {
                    name, rtype, args, ..
                } => {
                    let param_types: Vec<Type> = args.iter().map(|(_, t)| t.clone()).collect();
                    if self.functions.contains_key(name) {
                        return Err(Semantic_err::RedefinedFunction(name.to_string()));
                    }
                    self.functions
                        .insert(name.clone(), (param_types, rtype.clone()));
                }
                Declare::Enum { name, variations } => {
                    if self.enums.contains_key(name) {
                        return Err(Semantic_err::Custom(format!(
                            "Enum {} already defined",
                            name
                        )));
                    }
                    self.enums.insert(name.clone(), variations.clone());
                }
                Declare::Struct { name, fields } => {
                    if self.structs.contains_key(name) {
                        return Err(Semantic_err::Custom(format!(
                            "Struct {} already defined",
                            name
                        )));
                    }
                    self.structs.insert(name.clone(), fields.clone());
                }
            }
            Ok(())
        }

        /// Semanilizer function to check validity of scope declarations
        ///
        /// # Return
        ///  Returns a Semnatic ret with useless return
        /// # Example
        /// ```
        /// semantic_analyse.analyze_decl()?;
        /// ```    

        pub fn analyze_decl(&mut self, decl: &Declare) -> Semantic_Ret<()> {
            match decl {
                Declare::Function {
                    rtype, args, body, ..
                } => {
                    self.scope_stack = vec![HashMap::new()];
                    self.current_function_return = rtype.clone();

                    for (p_name, var_type) in args {
                        self.declare_varib(p_name.clone(), var_type.clone(), false)?;
                    }

                    for stmt in body {
                        self.analyze_stmt(stmt)?;
                    }
                    self.current_function_return = None;
                }
                _ => {}
            }

            Ok(())
        }

        /// Semanilizer function to validate proper statments
        ///
        /// # Return
        ///  Returns a Semnatic ret with useless return
        /// # Example
        /// ```
        /// semantic_analyse.analyze_stmt()?;
        /// ```    

        pub fn analyze_stmt(&mut self, stmt: &Statmnt) -> Semantic_Ret<()> {
            match stmt {
                Statmnt::Let {
                    name,
                    mutable,
                    type_annot,
                    value,
                } => {
                    let rhs_type = self.eval_type(value)?;
                    if let Some(t) = type_annot {
                        if !self.is_compatible(t, &rhs_type) {
                            return Err(Semantic_err::TypeMismatch {
                                expected: t.clone(),
                                got: rhs_type,
                            });
                        }
                    }
                    let final_type = type_annot.clone().unwrap_or(rhs_type);
                    self.declare_varib(name.clone(), final_type, *mutable)?;
                }
                Statmnt::Assignment { name, op, val } => {
                    self.update_varib(name)?;
                    let (var_type, _) = self.get_varib(name)?;
                    let val_type = self.eval_type(val)?;
                    if let Some(operat) = op {
                        self.check_bin_op_types(operat, &var_type, &val_type)?;
                    } else {
                        if !self.is_compatible(&var_type, &val_type) {
                            return Err(Semantic_err::TypeMismatch {
                                expected: var_type,
                                got: val_type,
                            });
                        }
                    }
                }
                Statmnt::If {
                    cond,
                    then_branch,
                    else_branch,
                } => {
                    let cond_type = self.eval_type(cond)?;
                    if !self.is_compatible(&Type::BOOL, &cond_type) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::BOOL,
                            got: cond_type,
                        });
                    }
                    self.enter_scope();

                    for i in then_branch {
                        self.analyze_stmt(i)?;
                    }
                    if let Some(other) = else_branch {
                        self.enter_scope();
                        for i in other {
                            self.analyze_stmt(i)?;
                        }
                        self.exit_scope();
                    }
                    self.exit_scope();
                }

                Statmnt::While { cond, body } => {
                    let cond_type = self.eval_type(cond)?;
                    if !self.is_compatible(&Type::BOOL, &cond_type) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::BOOL,
                            got: cond_type,
                        });
                    }
                    self.loop_depth += 1;
                    self.enter_scope();
                    for i in body {
                        self.analyze_stmt(i)?;
                    }
                    self.exit_scope();
                    self.loop_depth -= 1;
                }

                Statmnt::For {
                    var_name,
                    lb,
                    rb,
                    body,
                } => {
                    let lb_t = self.eval_type(lb)?;
                    let rb_t = self.eval_type(rb)?;
                    if !self.is_compatible(&Type::INT, &lb_t) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::INT,
                            got: lb_t,
                        });
                    }
                    if !self.is_compatible(&Type::INT, &rb_t) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::INT,
                            got: rb_t,
                        });
                    }
                    self.loop_depth += 1;
                    self.enter_scope();
                    self.declare_varib(var_name.clone(), Type::INT, false)?;
                    for i in body {
                        self.analyze_stmt(i)?;
                    }
                    self.exit_scope();
                    self.loop_depth -= 1;
                }
                Statmnt::Loop { body } => {
                    self.loop_depth += 1;
                    self.enter_scope();
                    for i in body {
                        self.analyze_stmt(i)?;
                    }
                    self.exit_scope();
                    self.loop_depth -= 1;
                }
                Statmnt::Break => {
                    if self.loop_depth == 0 {
                        return Err(Semantic_err::Break_Continue_location);
                    }
                }
                Statmnt::Continue => {
                    if self.loop_depth == 0 {
                        return Err(Semantic_err::Break_Continue_location);
                    }
                }
                Statmnt::Return(val) => {
                    let rtype = if let Some(x) = val {
                        Some(self.eval_type(x)?)
                    } else {
                        None
                    };

                    match (&self.current_function_return, &rtype) {
                        (Some(expected), Some(got)) => {
                            if !self.is_compatible(expected, got) {
                                return Err(Semantic_err::TypeMismatch {
                                    expected: expected.clone(),
                                    got: got.clone(),
                                });
                            }
                        }
                        (Some(expected), None) => {
                            return Err(Semantic_err::TypeMismatch {
                                expected: expected.clone(),
                                got: Type::NULL,
                            });
                        }
                        (None, Some(got)) => {
                            return Err(Semantic_err::TypeMismatch {
                                expected: Type::NULL,
                                got: got.clone(),
                            });
                        }
                        (None, None) => {}
                    }
                }
                Statmnt::Expr(x) => {
                    self.eval_type(x)?;
                }
                Statmnt::Block(blk) => {
                    self.enter_scope();
                    for i in blk {
                        self.analyze_stmt(i)?;
                    }
                    self.exit_scope();
                }
            }

            Ok(())
        }

        /// Semanilizer function to get the expression type
        ///
        /// # Arguments
        /// expr : &Expr -> Takes the refernce to the expression whose type to be evaluated
        ///
        /// # Return
        ///  Returns a Semnatic ret containing the Type on success
        ///
        /// # Example
        /// ```
        /// semantic_analyse.eval_type(&Expr::Bool(true))?;
        /// ```    

        pub fn eval_type(&self, expr: &Expr) -> Semantic_Ret<Type> {
            match expr {
                Expr::Bool(_) => Ok(Type::BOOL),
                Expr::Float(_) => Ok(Type::FLOAT),
                Expr::Int(_) => Ok(Type::INT),
                Expr::String(_) => Ok(Type::STRING),
                Expr::Ident(name) => {
                    let (typ, _) = self.get_varib(name)?;
                    Ok(typ)
                }
                Expr::Binary_op { op, left, right } => {
                    let (l_type, r_type) = (self.eval_type(left)?, self.eval_type(right)?);
                    self.check_bin_op_types(op, &l_type, &r_type)
                }
                Expr::Unary_op { op, operand } => {
                    let operant_type = self.eval_type(operand)?;
                    match op {
                        UN_OP::Bang => {
                            if matches!(operant_type, Type::BOOL) {
                                Ok(Type::BOOL)
                            } else {
                                Err(Semantic_err::TypeMismatch {
                                    expected: Type::BOOL,
                                    got: operant_type,
                                })
                            }
                        }
                        UN_OP::Neg => {
                            if matches!(operant_type, Type::INT | Type::FLOAT) {
                                Ok(operant_type)
                            } else {
                                Err(Semantic_err::TypeMismatch {
                                    expected: Type::INT,
                                    got: operant_type,
                                })
                            }
                        }
                        UN_OP::Tilda => {
                            if matches!(operant_type, Type::INT) {
                                Ok(Type::INT)
                            } else {
                                Err(Semantic_err::TypeMismatch {
                                    expected: Type::INT,
                                    got: operant_type,
                                })
                            }
                        }
                    }
                }

                Expr::Fxn_call { name, args } => {
                    let (param, ret_type) = self
                        .functions
                        .get(name)
                        .ok_or_else(|| Semantic_err::UndefinedFunction(name.clone()))?;

                    if args.len() != param.len() {
                        return Err(Semantic_err::Custom(format!(
                            "Invalid number of arguments provided for Function {:?} expected:{:?} got {:?}",
                            name,
                            param.len(),
                            args.len()
                        )));
                    }
                    for (arg, param_t) in args.iter().zip(param.iter()) {
                        let arg_t = self.eval_type(arg)?;
                        if !self.is_compatible(param_t, &arg_t) {
                            return Err(Semantic_err::TypeMismatch {
                                expected: param_t.clone(),
                                got: arg_t,
                            });
                        }
                    }

                    Ok(ret_type.clone().unwrap_or(Type::INT))
                }
                Expr::Struct_enum_init { name, fields } => {
                    if let Some(s_fields) = self.structs.get(name) {
                        if s_fields.len() != fields.len() {
                            return Err(Semantic_err::Custom(format!(
                                "Invalid number of arguments provided for Struct {:?} expected:{:?} got {:?}",
                                name,
                                s_fields.len(),
                                fields.len()
                            )));
                        }
                        for (fname, fexpr) in fields {
                            let ftype = self.eval_type(fexpr)?;
                            let exp_t = s_fields
                                .iter()
                                .find(|(n, _)| n == fname)
                                .map(|(_, t)| t)
                                .ok_or_else(|| {
                                Semantic_err::Custom(format!(
                                    "Struct '{}' has no field '{}'",
                                    name, fname
                                ))
                            })?;
                            if !self.is_compatible(exp_t, &ftype) {
                                return Err(Semantic_err::TypeMismatch {
                                    expected: exp_t.clone(),
                                    got: ftype,
                                });
                            }
                        }
                        return Ok(Type::CUSTOM(name.clone()));
                    }

                    if self.enums.contains_key(name) {
                        return Ok(Type::CUSTOM(name.clone()));
                    }

                    Err(Semantic_err::Custom(format!("Undefined type: {}", name)))
                }
                Expr::Field_access { obj, field } => {
                    let typ = self.eval_type(obj)?;
                    match typ {
                        Type::CUSTOM(sname) => {
                            if let Some(fields) = self.structs.get(&sname) {
                                for (fname, ftype) in fields {
                                    if fname == field {
                                        return Ok(ftype.clone());
                                    }
                                }
                                Err(Semantic_err::Custom(format!(
                                    "Struct {} has no field named {}",
                                    sname, field
                                )))
                            } else {
                                Err(Semantic_err::Custom(format!(
                                    "Type {} has no fields",
                                    sname
                                )))
                            }
                        }
                        _ => Err(Semantic_err::Custom(format!(
                            "Cannot access field '{}' on type {:?}",
                            field, typ
                        ))),
                    }
                }
                Expr::Postdecr(name)
                | Expr::Predecr(name)
                | Expr::Preincr(name)
                | Expr::Postincr(name) => {
                    let (typ, is_mut) = self.get_varib(name)?;
                    if !is_mut {
                        return Err(Semantic_err::Immutable_Variable(name.clone()));
                    }

                    if !matches!(typ, Type::INT | Type::FLOAT) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::INT,
                            got: typ,
                        });
                    }
                    Ok(typ)
                }
                Expr::Null => Ok(Type::NULL),
            }
        }

        /// Semanilizer function to check equality of the two Types
        ///
        /// # Arguments
        /// t1 : &Expr -> Takes the refernce to the expression 1
        /// t2 : &Expr -> Takes the refernce to the expression 2
        ///
        /// # Return
        ///  Returns a True/False based on equality of the two types
        ///
        /// # Example
        /// ```
        /// semantic_analyse.is_compatible(&Expr::Bool(true),&Expr::Bool(false))?;S
        /// ```    

        pub fn is_compatible(&self, t1: &Type, t2: &Type) -> bool {
            match (t1, t2) {
                (Type::INT, Type::INT) => true,
                (Type::FLOAT, Type::FLOAT) => true,
                (Type::STRING, Type::STRING) => true,
                (Type::BOOL, Type::BOOL) => true,
                (Type::CUSTOM(a), Type::CUSTOM(b)) => a == b,
                _ => false,
            }
        }

        pub fn check_bin_op_types(
            &self,
            op: &BIN_OP,
            l_type: &Type,
            r_type: &Type,
        ) -> Semantic_Ret<Type> {
            match op {
                BIN_OP::Add
                | BIN_OP::Sub
                | BIN_OP::Mul
                | BIN_OP::Div
                | BIN_OP::Mod
                | BIN_OP::Pow => {
                    if self.is_compatible(&l_type, &r_type) {
                        if matches!(l_type, Type::INT | Type::FLOAT) {
                            Ok(l_type.clone())
                        } else {
                            Err(Semantic_err::Custom(format!(
                                "[ARITHMETIC] Required numeric types, found {:?}",
                                l_type
                            )))
                        }
                    } else {
                        Err(Semantic_err::TypeMismatch {
                            expected: l_type.clone(),
                            got: r_type.clone(),
                        })
                    }
                }
                BIN_OP::Eq
                | BIN_OP::N_eq
                | BIN_OP::Lt
                | BIN_OP::Lt_eq
                | BIN_OP::Gt
                | BIN_OP::Gt_eq => {
                    if self.is_compatible(l_type, r_type) {
                        Ok(Type::BOOL)
                    } else {
                        Err(Semantic_err::TypeMismatch {
                            expected: l_type.clone(),
                            got: r_type.clone(),
                        })
                    }
                }
                BIN_OP::And | BIN_OP::Or => {
                    if !self.is_compatible(&Type::BOOL, &l_type) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::BOOL,
                            got: l_type.clone(),
                        });
                    }
                    if !self.is_compatible(&Type::BOOL, &r_type) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::BOOL,
                            got: r_type.clone(),
                        });
                    }

                    Ok(Type::BOOL)
                }
                BIN_OP::Amp | BIN_OP::Pipe | BIN_OP::Xor | BIN_OP::Lshift | BIN_OP::Rshift => {
                    if !matches!(l_type, Type::INT) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::INT,
                            got: l_type.clone(),
                        });
                    }
                    if !matches!(r_type, Type::INT) {
                        return Err(Semantic_err::TypeMismatch {
                            expected: Type::INT,
                            got: r_type.clone(),
                        });
                    }
                    Ok(Type::INT)
                }
            }
        }
    }
}
