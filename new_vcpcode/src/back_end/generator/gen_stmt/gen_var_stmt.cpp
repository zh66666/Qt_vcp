#include "expression.h"
#include "var.h"
#include "gen_utils.h"
#include "CodedVarBuild_Util.h"

class Var_info;

// 定义在文件 : src/main_utils.cpp
extern int str2int(string);
extern void check_str_arg(string arg_name, string info);
extern void check_pt_arg(void *pt, string info);

// 定义在文件 : ./gen_array_stmt.cpp
extern void gen_array_refer(string, Var*, list<Var_info>*);

// 定义在文件 : ./gen_fun_stmt.cpp
extern void gen_fun_assign(string, Expression*, list<Var_info>*);

// 以下函数定义在文件夹: ../../gen_op
extern void red_add(string, Expression*, list<Var_info>*);
extern void red_sub(string, Expression*, list<Var_info>*);
extern void red_mul(string, Expression*, list<Var_info>*);
extern void red_div(string, Expression*, list<Var_info>*);
extern void red_shift_l(string, Expression*, list<Var_info>*);
extern void red_shift_r(string, Expression*, list<Var_info>*);
extern void red_and(string, Expression*, list<Var_info>*);
extern void red_or(string, Expression*, list<Var_info>*);
extern void red_not(string, Expression*, list<Var_info>*);
extern void red_assign_const(string, int, list<Var_info>*);
extern void red_assign_var(string, string, list<Var_info>*);
extern void Red_RelationCmp(string var_name, Expression *exp,
		list<Var_info>* v);
extern void red_bit_inverse(string, Expression*, list<Var_info>*); //李晨欢

#define IF_SIG_TEST 0 //if编码语句签名正确性测试
static int check_relationCmp_op(Expression *s);

// 功能：对变量赋值语句进行编码。根据赋值表达式的不同类型，调用不同的下层编码函数，生成冗余代码
// 输入参数：var_name-赋值语句的左值变量名
// 			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_var_assign(string var_name, Expression* exp, list<Var_info>* v)
{
	try
	{
		check_str_arg(var_name, "left variable's name"); //检查参数：左值变量名的有效性
		check_pt_arg(exp, "expression object's pointer"); //检查参数：表达式指针的有效性
		check_pt_arg(v, "var_info list's pointer"); //检查参数：动态签名表指针的有效性

		//判断赋值表达式的类型
		switch (exp->get_stamp())
		{
		case 0:
		{
#if IF_SIG_TEST
			if (check_relationCmp_op(exp) == 1)
			{
				Red_RelationCmp(var_name, exp, v);
			}
			else
			{
				string left,right,new_var_name;
				string prefix;
				if(exp->get_cel()->get_stamp()==4)
				{
					prefix=exp->get_cel()->get_cvar()->get_name();
					prefix=TmpVarJudge(prefix)==true? prefix:"TC_"+prefix;
					left=prefix+VAR_TDATA+VAR_DATA_NAME;
				}
				else
				{
					left=exp->get_cel()->get_c();
				}

				if(exp->get_cer()->get_stamp()==4)
				{
					prefix=exp->get_cer()->get_cvar()->get_name();
					prefix=TmpVarJudge(prefix)==true? prefix:"TC_"+prefix;
					right=prefix+VAR_TDATA+VAR_DATA_NAME;
				}
				else
				right=exp->get_cer()->get_c();

				new_var_name=TmpVarJudge(var_name)==true? var_name:"TC_"+var_name;

				int new_sig=get_sig_index(NULL);
				out_indent(indent_num,gen_out);
				string Func_FieldStr=left+exp->get_op()+right;
				gen_out<<new_var_name+VAR_TDATA+"="+"GetCodedData("+Func_FieldStr+","+int2str(new_sig)+");"<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"//new_sig:"<<new_sig<<endl;

				add_new_sig(v,var_name,new_sig,var_scope); //将左值变量新签名加入动态签名表
			}
#else
			//如果是带有两个操作数的表达式，根据操作符类型调用不同的编码函数
			//判断是否为加法运算
			if (exp->get_op() == "+")
			{
				//如果是，则调用对加法运算的编码函数
				red_add(var_name, exp, v);
			}
			//判断是否为减法运算
			else if (exp->get_op() == "-")
			{
				//如果是，则调用对减法运算的编码函数
				red_sub(var_name, exp, v);
			}
			//判断是否为乘法运算
			else if (exp->get_op() == "*")
			{
				//如果是，则调用对乘法运算的编码函数
				red_mul(var_name, exp, v);
			}
			//判断是否为除法运算
			else if (exp->get_op() == "/")
			{
				//如果是，则调用对除法运算的编码函数
				red_div(var_name, exp, v);
			}
			//判断是否为逻辑与运算
			else if (exp->get_op() == "&&")
			{
				//如果是，则调用对逻辑与运算的编码函数
				red_and(var_name, exp, v);
			}
			//判断是否为逻辑或运算
			else if (exp->get_op() == "||")
			{
				//如果是，则调用对逻辑或运算的编码函数
				red_or(var_name, exp, v);
			}
			//判断是否为左移运算
			else if (exp->get_op() == "<<")
			{
				//如果是，则调用对左移运算的编码函数
				red_shift_l(var_name, exp, v);
			}
			//判断是否为右移运算
			else if (exp->get_op() == ">>")
			{
				//如果是，则调用对右移运算的编码函数
				red_shift_r(var_name, exp, v);
			}
			//判断是否是关系比较运算(只出现在分支、循环转换后的代码中)
			else if (check_relationCmp_op(exp) == 1)
			{
				Red_RelationCmp(var_name, exp, v);
			}
			else
			{
				//如果为其他未知运算符，则抛出错误信息
				string error_info = "invalid operator!";
				throw error_info;
			}
#endif
		}
			break;

		case 1:
		{
			//如果为单操作数表达式，则判断操作符是否为逻辑非运算
			if (exp->get_op() == "!")
			{
				//如果是，则调用对逻辑非运算的编码函数
				red_not(var_name, exp, v);
			}
			else if (exp->get_op() == "~")
			{
				red_bit_inverse(var_name, exp, v);
			}
			else
			{
				//如果是其他操作符，则抛出错误信息
				string error_info = "invalid operator!";
				throw error_info;
			}
		}
			break;

		case 2:
		{
			//如果是括号表达式，则抛出错误信息
			string error_info = "the ( ) is unexpected! ";
			throw error_info;
		}
			break;

		case 3:
		{
#if IF_SIG_TEST
			//如果是一个常数表达式
			string new_var_name;
			string constant_name=exp->get_c();//取出该常数字符串

			new_var_name=TmpVarJudge(var_name)==true? var_name:"TC_"+var_name;

			int new_sig=get_sig_index(NULL);
			out_indent(indent_num,gen_out);
			string Func_FieldStr=constant_name;
			//gen_out<<new_var_name+VAR_TDATA+"="+"GetCodedData("+Func_FieldStr+","+int2str(new_sig)+");"<<endl;
			gen_out<<new_var_name<<"_1="<<constant_name<<"*A1;"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<new_var_name<<"_2="<<constant_name<<"*A2;"<<endl;

			add_new_sig(v,var_name,new_sig,var_scope);//将左值变量新签名加入动态签名表
#else
			//如果是一个常数表达式
			string constant_name = exp->get_c();				//取出该常数字符串
			int high = str2int(constant_name);				//将其转换成对应的常数
			red_assign_const(var_name, high, v);				//调用常数赋值编码函数
#endif
		}
			break;

		case 4:
		{
			//如果是变量表达式，先判断变量的是否为普通变量
			if (0 == exp->get_cvar()->get_stamp())
			{
#if IF_SIG_TEST
				//如果是普通变量，则调用变量赋值编码函数
				string new_var_name;
				string prefix=exp->get_cvar()->get_name();
				prefix=TmpVarJudge(prefix)==true? prefix:"TC_"+prefix;
				string name_tmp=prefix+VAR_TDATA+VAR_DATA_NAME;

				new_var_name=TmpVarJudge(var_name)==true? var_name:"TC_"+var_name;

				int new_sig=get_sig_index(NULL);

				string Func_FieldStr=name_tmp;
				out_indent(indent_num,gen_out);
				gen_out<<new_var_name+VAR_TDATA+"="+"GetCodedData("+Func_FieldStr+","+int2str(new_sig)+");"<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"//new_sig:"<<new_sig<<endl;

				add_new_sig(v,var_name,new_sig,var_scope);//将左值变量新签名加入动态签名表
#else
				//如果是普通变量，则调用变量赋值编码函数
				string name_tmp = exp->get_cvar()->get_name();
				red_assign_var(var_name, name_tmp, v);
#endif
			}
			else
			{
				//如果是数组变量，则调用数组引用编码函数
				gen_array_refer(var_name, exp->get_cvar(), v);
			}
		}
			break;

		case 5:
		{
			//如果是函数调用表达式，则调用函数赋值编码函数
			gen_fun_assign(var_name, exp, v);
		}
			break;

		default:
		{
			//如果是其他类型，则抛出错误信息
			string error_info = "expression object's stamp is invalid!";
			throw error_info;
		}
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding variable assignment : " + str;
		throw error_info;
	}
}

static int check_relationCmp_op(Expression *s)
{
	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(s, "expression object's pointer");

		//判断操作符是否为比较运算操作符
		if ((s->get_op() == ">") || (s->get_op() == ">=")
				|| (s->get_op() == "<") || (s->get_op() == "<=")
				|| (s->get_op() == "==") || (s->get_op() == "!="))
		{
			//如果是，返回1
			return 1;
		}
		else
		{
			//其他操作符则返回0
			return 0;
		}
	} catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "check expression's operation : " + str;
		throw error_info;
	}
}

