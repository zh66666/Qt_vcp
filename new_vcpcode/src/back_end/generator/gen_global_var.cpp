/*
 * gen_global_var.cpp
 *
 *  Created on: 2015��5��13��
 *      Author: LordVoldemort
 */

#include "gen_global_var.h"
#include "global_var.h"
#include<string>
extern list<string>* global_var_names;
extern list<Global_var>* global_vars;
vector<string> global_var_vec;

//�жϱ����Ƿ���ȫ�ֱ������������global_var_vec,��֮��������
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
				//������ȫ��������
				t = 1;
				stamp = giter->get_stamp();
			}

		}
		if (t == 0) {
			//���û���ҵ�,����ȫ�ֱ���
		} else {
			//t==1 ����ҵ��ˣ�����ȫ�ֱ�����������global_var_vec���Ҳ��ظ�
			/*if (global_var_vec.end()
			 == find(global_var_vec.begin(), global_var_vec.end(),
			 var->get_name())) { //�״�����������global_var_vec
			 global_var_vec.push_back(var->get_name());

			 } else { //���״�����

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
				//�״γ��ֲ�������ͨȫ�ֱ�����������
				global_var_vec.push_back(name);

		} //if
	} catch (string str) {
		string error_info = "search global_var error in expression : " + str;
		throw error_info;
	}
}

//��������ñ��ʽexp�е�ȫ�ֱ���
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
				//F_VCE_CheckArr()�жϵ�һ������
				list<Expression>::iterator iterexp = fun_args->begin();
				Expression first_arg = *iterexp;
				int typeArg = first_arg.get_stamp();
				if (typeArg == 4) {
					//����Ϊ����
					Var* vararg = first_arg.get_cvar();
					switch (vararg->get_stamp()) {
					case 0: {
						//ʶ��Ϊ������������
					}
						break;
					case 1: {
						//һά����
						Expression* oneDim = vararg->get_cexp();
						if (oneDim->get_stamp() == 4) {
							//����
							Var* subVar = oneDim->get_cvar();
							search_global_var(subVar);
						} else {
							//���������ǽṹ���
						}
					}
						break;
					case 2: {
						//��ά���� TODO

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
				//������������
				for (iterexp++; iterexp != fun_args->end(); iterexp++) {
					search_global_var(&(*iterexp));
				}

			} else { //��F_VCE_CheckArr()
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
//������������s�е�ȫ�ֱ���
void search_global_var(Statement* s) {

	try {
		//��������������ָ�����Ч��
		check_pt_arg(s, "statement object 's pointer");
		//�����Ϊ�գ���ƥ����������
		switch (s->get_stamp()) {
		case 0: {
			//compound statement ������Ϊ������䣬������ø�������е��������
			for (list<Statement>::iterator iter =
					s->get_ccstmt()->get_csl()->begin();
					iter != s->get_ccstmt()->get_csl()->end(); iter++) {
				search_global_var(&(*iter)); //����ÿ�����
			}
		}
			break;
		case 1: {
			//selection statement ���Ϊ��֧���
			Selection_stmt* ifstate = s->get_csstmt();
			if (ifstate->get_stamp() == 0) {
				//ֻ��if��֧
				search_global_var(ifstate->get_cs_if());
				Expression* ifexp = ifstate->get_ce(); //�ж����
				search_global_var(ifexp);

			} else if (ifstate->get_stamp() == 1) { // if-else��֧
				//if��֧
				search_global_var(ifstate->get_cs_if());
				Expression* ifexp = ifstate->get_ce();
				search_global_var(ifexp);
				//else��֧
				search_global_var(ifstate->get_cs_else());

			} else if (ifstate->get_stamp() == -1) {
				//default
			}
		}
			break;
		case 2: {
			//iteration statement ���Ϊѭ�����\
			//while (expression) statement
			Iteration_stmt* iterstate = s->get_cistmt();
			search_global_var(iterstate->get_ce()); //expression
			search_global_var(iterstate->get_cs()); //statement  ����ѭ���ڵ����

		}
			break;
		case 3: {
			//return statement ��return ���
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
			//assign statement ,��ֵ���
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
			//","�򲻱���
		}
		} //switch
	} catch (string str) {
		string error_info = "search global_var error in statement : " + str;
		throw error_info;
	}
}

//��������f_dָ�������ĺ����г��ֵ�ȫ�ֱ���
//ɨ��˳��ͬ��ǩ������˳�����Һ���
//ɨ���ȫ�ֱ����洢��vector�У����˳����ɨ��˳��һ��
void search_and_get_global_var(Fun_declaration* f_d) {

	check_pt_arg(f_d, "statement object 's pointer");
	Compound_stmt* com = f_d->get_ccstmt();
	list<Statement>* state = com->get_csl(); //��ȡstatement-list
	//�������������global_var_vec
	global_var_vec.clear();
	for (list<Statement>::iterator iter = state->begin(); iter != state->end();
			iter++) {
		search_global_var(&(*iter)); //����ÿ�����
	}
}

//���غ���func_name�г��ֵ�����ȫ�ֱ�����
vector<string> get_global_var_infunc(string func_name) {
	return global_var_vec;
}

//��ȡȫ�ֱ����󶨵ľֲ�����������
string get_bind_local_var(string global_var, string func_name) {
	return "AC_" + global_var + "_" + func_name;
}

//�ɰ󶨱�����ȡ��Ӧ��ȫ�ֱ���������
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

//�ж��Ƿ���ȫ�ֱ���
bool is_global_var(string var_name) {

	if (global_var_vec.end()
			== find(global_var_vec.begin(), global_var_vec.end(), var_name))
		return false;
	else
		return true;

}

//�жϿɷ�ͨ���ֲ��������õ���Ӧ��ȫ�ֱ�����
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
 * ���ظñ����������ʽ����������������
 */
string get_global_string(string var_name, string func_name) {
	if (is_global_var(var_name)) {
		return get_bind_local_var(var_name, func_name);
	} else {
		return "TC_" + var_name;
	}

}

/**
 * ����ñ����������ʽ�����ڼ��붯̬ǩ����
 */
string get_global_string_sig(string var_name, string func_name) {
	if (is_global_var(var_name)) {
		return get_bind_local_var(var_name, func_name);
	} else {
		return var_name;
	}

}
