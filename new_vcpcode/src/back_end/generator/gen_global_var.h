/*
 * gen_global_var.h
 *
 *  Created on: 2015Äê5ÔÂ13ÈÕ
 *      Author: LordVoldemort
 */

#ifndef SRC_GEN_GLOBAL_VAR_H_
#define SRC_GEN_GLOBAL_VAR_H_
#include "gen_utils.h"
#include "fun_declaration.h"
#include "compound_stmt.h"
#include"expression.h"
#include"iteration_stmt.h"
#include"return_stmt.h"
#include"selection_stmt.h"
#include"var.h"
#include"statement.h"
void search_global_var(Var* var);
void search_global_var(Expression* exp);
void search_global_var(Statement* s);
void search_and_get_global_var(Fun_declaration* f_d);
vector<string> get_global_var_infunc(string func_name);
string get_bind_local_var(string global_var,string func_name);
string get_global_var_from_local(string local_var,string func_name) throw(string);
bool is_global_var(string var_name);
bool is_canbe_convert2global(string local_var,string func_name);
string get_global_string(string var_name,string func_name);
string get_global_string_sig(string var_name,string func_name);

#endif /* SRC_BACK_END_GENERATOR_GEN_GLOBAL_VAR_H_ */
