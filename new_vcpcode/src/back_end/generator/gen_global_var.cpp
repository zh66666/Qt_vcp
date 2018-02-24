/*
 * gen_global_var.cpp
 *
 *  Created on: 2015年5月13日
 *      Author: LordVoldemort
 */

#include "gen_global_var.h"
#include "global_var.h"
#include<string>
extern list<string>* global_var_names;
extern list<Global_var>* global_vars;
vector<string> global_var_vec;

//判断变量是否是全局变量，是则加入global_var_vec,反之不作处理
void search_global_var(Var* var) {
	try {
		check_pt_arg(var, "statement object 's pointer");
		int t = 0;
		int stamp = -2;
		for (list<Global_var>::iterator giter = global_vars->begin();
				giter != global_vars->end(); giter++) {
			string s1 = giter->get_var_name();
			string s2 = var->get_name();
			if (s1.compare(s2) == 0) {
				//变量在全局链表中
				t = 1;
				stamp = giter->get_stamp();
			}

		}
		if (t == 0) {
			//如果没有找到,则不是全局变量
		} else {
			//t==1 如果找到了，则是全局变量，并加入global_var_vec中且不重复
			/*if (global_var_vec.end()
			 == find(global_var_vec.begin(), global_var_vec.end(),
			 var->get_name())) { //首次遇见，加入global_var_vec
			 global_var_vec.push_back(var->get_name());

			 } else { //非首次遇见

			 }*/
			int gt = 0;
			string name = var->get_name();
			for (vector<string>::iterator gl = global_var_vec.begin();
					gl != global_var_vec.end(); gl++) {
				if ((*gl) == name)
					gt = 1;
			}
			if (gt == 0
					&& ((stamp == 0 || stamp == 1) && (var->get_stamp() == 0)))
				//首次出现并且是普通全局变量，非数组
				global_var_vec.push_back(name);

		} //if
	} catch (string str) {
		string error_info = "search global_var error in expression : " + str;
		throw error_info;
	}
}

//遍历并获得表达式exp中的全局变量
void search_global_var(Expression* exp) {
	try {
		check_pt_arg(exp, "statement object 's pointer");
		switch (exp->get_stamp()) {
		case 0: {
			//expl op expr
			Expression* expl = exp->get_cel();
			Expression* exprr = exp->get_cer();
			search_global_var(exprr);
			search_global_var(expl);

		}
			break;
		case 1: {
			//op expr
			Expression* expr = exp->get_cer();
			search_global_var(expr);
		}
			break;
		case 2: {
			//(exp)
			Expression* expre = exp->get_exp();
			search_global_var(expre);
		}
			break;
		case 3: {
			//constant
		}
			break;
		case 4: {
			// var
			Var* varc = exp->get_cvar();
			search_global_var(varc);
		}
			break;
		case 5: {
			//the expression is a function calling
			list<Expression>* fun_args = exp->get_args();
			string fun_name = exp->get_c();
			if (fun_name.compare("F_VCE_CheckArr") == 0) {
				//F_VCE_CheckArr()判断第一个参数
				list<Expression>::iterator iterexp = fun_args->begin();
				Expression first_arg = *iterexp;
				int typeArg = first_arg.get_stamp();
				if (typeArg == 4) {
					//参数为变量
					Var* vararg = first_arg.get_cvar();
					switch (vararg->get_stamp()) {
					case 0: {
						//识别为变量，不处理
					}
						break;
					case 1: {
						//一维数组
						Expression* oneDim = vararg->get_cexp();
						if (oneDim->get_stamp() == 4) {
							//变量
							Var* subVar = oneDim->get_cvar();
							search_global_var(subVar);
						} else {
							//常量或者是结构体等
						}
					}
						break;
					case 2: {
						//多维数组 TODO

						list<Expression>* oneMore = vararg->get_exp_list();
					}
						break;
					case 3: { //TODO
					}
						break;
					case 4: { //TODO
					}
						break;
					default:
						break;
					}

				}
				//遍历后续参数
				for (iterexp++; iterexp != fun_args->end(); iterexp++) {
					search_global_var(&(*iterexp));
				}

			} else { //非F_VCE_CheckArr()
				for (list<Expression>::iterator iterexp = fun_args->begin();
						iterexp != fun_args->end(); iterexp++) {
					search_global_var(&(*iterexp));
				}
			}
		}
			break;
			defult: {
			}
		} //switch
	} catch (string str) {
		string error_info = "search global_var error in expression : " + str;
		throw error_info;
	}

}
//遍历并获得语句s中的全局变量
void search_global_var(Statement* s) {

	try {
		//检查参数：语句对象指针的有效性
		check_pt_arg(s, "statement object 's pointer");
		//如果不为空，则匹配语句的类型
		switch (s->get_stamp()) {
		case 0: {
			//compound statement 如果语句为复合语句，则遍历该复合语句中的语句链表
			for (list<Statement>::iterator iter =
					s->get_ccstmt()->get_csl()->begin();
					iter != s->get_ccstmt()->get_csl()->end(); iter++) {
				search_global_var(&(*iter)); //遍历每条语句
			}
		}
			break;
		case 1: {
			//selection statement 如果为分支语句
			Selection_stmt* ifstate = s->get_csstmt();
			if (ifstate->get_stamp() == 0) {
				//只有if分支
				search_global_var(ifstate->get_cs_if());
				Expression* ifexp = ifstate->get_ce(); //判断语句
				search_global_var(ifexp);

			} else if (ifstate->get_stamp() == 1) { // if-else分支
				//if分支
				search_global_var(ifstate->get_cs_if());
				Expression* ifexp = ifstate->get_ce();
				search_global_var(ifexp);
				//else分支
				search_global_var(ifstate->get_cs_else());

			} else if (ifstate->get_stamp() == -1) {
				//default
			}
		}
			break;
		case 2: {
			//iteration statement 如果为循环语句\
			//while (expression) statement
			Iteration_stmt* iterstate = s->get_cistmt();
			search_global_var(iterstate->get_ce()); //expression
			search_global_var(iterstate->get_cs()); //statement  遍历循环内的语句

		}
			break;
		case 3: {
			//return statement ，return 语句
			Return_stmt* restate = s->get_crs();
			if (restate->get_stamp() == 0) {
				//return ;
			} else if (restate->get_stamp() == 1) {
				//return expression ;
				Expression* returnexp = restate->get_ce();
				search_global_var(returnexp);
			}
		}
			break;
		case 4: {
			//assign statement ,赋值语句
			Var* svar = s->get_cvar();
			search_global_var(svar);
			Expression* expr = s->get_ce();
			search_global_var(expr);

		}
			break;
		case 5: {
			//expression statement
			Expression* expr = s->get_ce();
			search_global_var(expr);
		}
			break;
		default: {
			//","则不遍历
		}
		} //switch
	} catch (string str) {
		string error_info = "search global_var error in statement : " + str;
		throw error_info;
	}
}

//搜索函数f_d指针描述的函数中出现的全局变量
//扫描顺序：同《签名分配顺序》先右后左
//扫描的全局变量存储在vector中，存放顺序与扫描顺序一致
void search_and_get_global_var(Fun_declaration* f_d) {

	check_pt_arg(f_d, "statement object 's pointer");
	Compound_stmt* com = f_d->get_ccstmt();
	list<Statement>* state = com->get_csl(); //获取statement-list
	//清空其他函数的global_var_vec
	global_var_vec.clear();
	for (list<Statement>::iterator iter = state->begin(); iter != state->end();
			iter++) {
		search_global_var(&(*iter)); //遍历每条语句
	}
}

//返回函数func_name中出现的所有全局变量名
vector<string> get_global_var_infunc(string func_name) {
	return global_var_vec;
}

//获取全局变量绑定的局部变量的名称
string get_bind_local_var(string global_var, string func_name) {
	return "AC_" + global_var + "_" + func_name;
}

//由绑定变量获取对应的全局变量的名称
string get_global_var_from_local(string local_var, string func_name)
		throw (string) {
	if (is_canbe_convert2global(local_var, func_name)) {
		int global_var_len = local_var.size() - string("AC_").size()
				- func_name.size() - 1;
		int global_var_start = 3;
		return local_var.substr(global_var_start, global_var_len);
	} else {
		throw string("local var convert to global var error!\n");
	}
}

//判断是否是全局变量
bool is_global_var(string var_name) {

	if (global_var_vec.end()
			== find(global_var_vec.begin(), global_var_vec.end(), var_name))
		return false;
	else
		return true;

}

//判断可否通过局部变量名得到对应的全局变量名
bool is_canbe_convert2global(string local_var, string func_name) {
	int global_var_len = local_var.size() - string("AC_").size()
			- func_name.size() - 1;
	int global_var_start = 3;
	bool result = false;
	if (global_var_len > 0) {
		string maybe_var = local_var.substr(global_var_start, global_var_len);
		result = is_global_var(maybe_var);
	}

	return result;
}

/**
 * 返回该变量的输出形式，用于冗余代码输出
 */
string get_global_string(string var_name, string func_name) {
	if (is_global_var(var_name)) {
		return get_bind_local_var(var_name, func_name);
	} else {
		return "TC_" + var_name;
	}

}

/**
 * 输出该变量的输出形式，用于加入动态签名表
 */
string get_global_string_sig(string var_name, string func_name) {
	if (is_global_var(var_name)) {
		return get_bind_local_var(var_name, func_name);
	} else {
		return var_name;
	}

}
