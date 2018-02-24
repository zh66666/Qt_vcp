#include "CodedVarBuild_Util.h"
#include "compound_stmt.h"
#include "declaration.h"
#include "fun_declaration.h"
#include "program.h"
#include "statement.h"
#include "TmpVarIndexRange.h"
#include "declaration_specifiers.h"
#include "type_specifier.h"
#include "fun_info.h"
#include "statement.h"
#include "return_stmt.h"
#include "expression.h"
#include "var.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "ControlFlow_CodeUtil.h"
#include "gen_utils.h"
#include <algorithm>

// 定义在文件 :  src/main_utils.cpp
extern int project_tempvar_num;
extern int project_tempcondvar_num;
//2010年3月10日追加
extern string file_info;
extern void check_pt_arg(void *pt,string info);
//2014-12-15
extern map<string,TFuncTmpRange> g_MapFuncTmpVar;

// 定义在文件 :./split_utils.c
extern vector<int>* tmpconds;
extern int tempvar_num_max;
extern int tempcondvar_num_max;
extern const int VOID_RET_VALUE;

// 定义在文件 : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
// 定义在文件 : ./split_stmt.c
extern void split_stmt_list(list<Statement>* s);
//定义在文件：./spit_utils.cpp
extern Statement* generate_newstmt(Expression* exp, string str, int* num);
extern int temp_var_num;

// 定义在文件: src/back_end/back_end_utils.cpp
extern int goto_label;

void UpdateFuncTmpRange(TFuncTmpRange *__FuncRange); //更新函数中临时变量的数据范围
void SaveTmpVarIndexNo(TFuncTmpRange *__FuncRange); //保存拆分前临时变量的索引号

// 功能：遍历中间结构，对无返回值函数进行处理，强制增加返回值
// 输入参数：s-中间结构对象指针
// 输出参数：s-中间结构对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
bool trans_void_func(Statement* s, list<Statement> *stmts, bool bFlagRet)
{
	try
	{
		//判断语句的类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//如果为复合语句，则遍历复合语句中的语句链表
				for(list<Statement>::iterator iter=s->get_ccstmt()->get_csl()->begin();
					   iter!=s->get_ccstmt()->get_csl()->end();iter++)
				{
					trans_void_func(&(*iter), stmts, bFlagRet);//遍历每条语句
				}
			}
			break;
			case 1:
			{
				//遍历if分支中的语句
				trans_void_func(s->get_csstmt()->get_cs_if(), stmts, bFlagRet);

				//判断是否含有else分支
				if(1==s->get_csstmt()->get_stamp())
				{
					//如果含有else分支，则遍历else分支中的语句
					trans_void_func(s->get_csstmt()->get_cs_else(), stmts, bFlagRet);
				}
				else
				{
					//如果无else分支，则忽略
				}
			}
			break;
			case 2:
			{
				//遍历循环体中的语句
				trans_void_func(s->get_cistmt()->get_cs(), stmts, bFlagRet);
			}
			break;
			case 5:
			{
				//函数调用语句
				if(5==s->get_ce()->get_stamp())
				{
					string functionName = s->get_ce()->get_c();
					if(functionName == "F_VCE_CheckVar"
							|| functionName == "F_VCE_CheckCode"
							|| "F_USR_DataFetch" == functionName
							|| "F_USR_DataDeliver" == functionName
							|| "F_USR_DataRecv" == functionName
							|| "F_USR_DataSend" == functionName)
					{
						//特殊接口，不进行处理
					}
					else
					{
						//一定是函数调用语句，非赋值语句，将计数器增加1
						void_func_counts ++;
					}

					//添加用于接受无返回值函数返回值的变量的声明语句
					//Var* void_left_var = new Var("AC_void_func_RetValue" + int2str(void_func_counts));
					Var* void_left_var = new Var(VOID_FUNC_RETVAL);//邹宇修改，无返回值强制返回的变量名，合并时请注意！
					Expression *left_exp = new Expression(void_left_var);
					Expression* zero_value = new Expression("0");
					Expression* void_left_exp = new Expression(left_exp, "=", zero_value);
					//void_left_exp->set_stamp(4);
					Statement* new_stmt = new Statement(void_left_exp);
					new_stmt->set_line_begin(line_num);
					new_stmt->set_fake_line(fake_line_num);
					temp_var_num++;	//普通临时变量个数加1
					//stmts->push_front(*new_stmt);


					//添加校验语句
					if(void_func_counts == 1)
					{
						list<Expression>* args = new list<Expression>();
						Expression *zero_constant = new Expression("0");
						args->push_back(*left_exp);
						args->push_back(*zero_constant);
						Expression *check_func = new Expression("F_VCE_CheckVar", args);
						//check_func->set_stamp(5);
						Statement* new_stmt2 = new Statement(check_func);
						new_stmt2->set_line_begin(line_num);
						new_stmt2->set_fake_line(fake_line_num);
						temp_var_num++;	//普通临时变量个数加1

						if(bFlagRet)
						{
							stmts->insert(--stmts->end(), *new_stmt2);
						}
						else
						{
							stmts->push_back(*new_stmt2);
						}
					}


					//创建函数调用赋值语句

					Expression* function_value = new Expression(s->get_ce()->get_c(), s->get_ce()->get_args());
					Statement* function_stmt = new Statement(void_left_var, function_value);
					if(function_value->get_c() == "F_VCE_CheckVar" || function_value->get_c() == "F_VCE_CheckArr")
					{

					}
					else
					{
						*s = *function_stmt;
					}

					//list<Statement>::iterator it = std::find(stmts->begin(), stmts->end(), *s);

					delete new_stmt;
				}
			}
			break;
			default:
			{
				//其他语句不遍历，忽略
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "trans void func error the program : " + str;
		throw error_info;
	}
}

// 功能：遍历中间结构，对需要拆分的地方进行拆分，如多操作符表达式，条件判断表达式等，记录
// 			拆分使用的临时变量的个数
// 输入参数：s-中间结构对象指针
// 输出参数：s-中间结构对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void split_pro(Program *s)
{	
	try
	{
		//检查参数：中间结构对象指针是否为空
		check_pt_arg(s,"program struct object's pointer");
		tmpconds->push_back(0);
		goto_label=0;	//goto标号置0
		TFuncTmpRange *CurFuncTmpRange=NULL;

		bool ifVoidFunc = false;
		//遍历声明链表
		for(list<Declaration>::iterator iter=s->get_declist()->begin();iter!=s->get_declist()->end();iter++)
		{
			//2010年3月10日追加
			file_info=iter->get_file_info();//设置当前处理所位于得文件名
			
			//判断当前迭代器所指的声明是否为函数声明，并且该函数声明是否带有函数体
			if(1==iter->get_stamp() && 0==iter->get_cfd()->get_stamp())
			{	
				//初始化临时变量索引号
				InitSplitTempVarCnt();
				//如果是带有函数体的函数声明，则遍历函数体中的语句链表

				CurSplitFuncName = iter->get_cfd()->get_fun_name();
				CurFuncNameHash = iter->get_cfd()->get_fun_name(); //2014-12-01增加，指明当前函数

				CurFuncTmpRange = new TFuncTmpRange();  //构造针对该函数的临时变量索引号范围

				SaveTmpVarIndexNo(CurFuncTmpRange);
				split_stmt_list(iter->get_cfd()->get_ccstmt()->get_csl());

				list<Statement> *stmts = iter->get_cfd()->get_ccstmt()->get_csl();
				//判断当前函数是否存在返回值语句
				bool bFlagRet = false;
				for(list<Statement>::iterator iter = stmts->begin();iter != stmts->end();iter++)
				{
					if(iter->get_stamp()==3)
					{
						bFlagRet = true;
						break;
					}
				}

				//这里进一步对函数声明体中的循环分支语句进行操作

				for (list<Statement>::iterator iter2 = stmts->begin();iter2 != stmts->end(); iter2++)
				{
					//若当前语句是if语句
					if(1 == iter2->get_stamp())
					{
						Selection_stmt* s = iter2->get_csstmt();
						Statement *cur_stmts = s->get_cs_if();
						//遍历if分支中的语句，记录所有的非函数赋值调用的个数
						trans_void_func(cur_stmts,stmts,bFlagRet);

						//判断是否有else分支
						if(1==s->get_stamp())
						{
							//如果为if-else结构，则处理else中语句
							trans_void_func(s->get_cs_else(),stmts,bFlagRet);
						}
						else
						{
							//如果没有else分支，则忽略
						}

						//若找到了无赋值函数调用，则将之变为有赋值函数调用

					}
					//若当前语句是while语句
					if(2 == iter2->get_stamp())
					{
						//iter2->get_cistmt();
						Iteration_stmt* s = iter2->get_cistmt();
						Statement *cur_stmts = s->get_cs();
						//遍历if分支中的语句，记录所有的非函数赋值调用的个数
						trans_void_func(cur_stmts,stmts,bFlagRet);

					}
				}

				//如果是无返回值函数，则强制增加返回语句
				//此处只对普通用户定义函数进行操作，校验和外部输入输出接口不进行操作
				if (iter->get_cfd()->get_decl_spec()->get_type_spec_list()->front().get_type_name()
						== "void" && "F_VCE_CheckVar" != CurSplitFuncName && "F_VCE_CheckArr" != CurSplitFuncName
						&& "F_USR_DataFetch" != CurSplitFuncName && "F_USR_DataRecv" != CurSplitFuncName
						&& "F_USR_DataSend" != CurSplitFuncName && "F_USR_DataDeliver" != CurSplitFuncName)
				{
					//新建一个返回值语句对象
					Return_stmt* return_stmt = new Return_stmt();
					return_stmt->set_stamp(1);
					return_stmt->set_line_begin(line_num);
					return_stmt->set_fake_line(fake_line_num);

					//新建Var&Expression
					//Var* return_zero = new Var(iter->get_cfd()->get_fun_name());
					Expression* return_zero = new Expression("0");
					Expression* zero_value = new Expression(*return_zero);

					return_stmt->set_ce(zero_value);
					Statement* stmt = new Statement(return_stmt);

					Statement* new_stmt = new Statement(return_stmt);
					new_stmt->set_line_begin(line_num);
					new_stmt->set_fake_line(fake_line_num);
					temp_var_num++;	//普通临时变量个数加1

					list<Statement>* s =
							iter->get_cfd()->get_ccstmt()->get_csl();
//					for (list<Statement>::iterator iter = s->begin();
//							iter != s->end(); iter++)
//					{
//						s->insert(iter, new_stmts->begin(), new_stmts->end());
//					}
					s->push_back(*new_stmt);
					delete new_stmt;
				}

				//如果是无返回值函数，将其强制转化为返回值类型为int的函数
				if (iter->get_cfd()->get_decl_spec()->get_type_spec_list()->front().get_type_name()
						== "void")
				{
					iter->get_cfd()->get_decl_spec()->get_type_spec_list()->front().set_type_name(
							"int");
				}

				//UPDATE index
				UpdateFuncTmpRange(CurFuncTmpRange);
				g_MapFuncTmpVar.insert(make_pair(iter->get_cfd()->get_fun_name(),*CurFuncTmpRange));
			}
			else
			{
				//当前迭代器所指的声明是函数声明，并且该函数声明不带有函数体
				if(1==iter->get_stamp() && 1==iter->get_cfd()->get_stamp())
				{
					if (iter->get_cfd()->get_decl_spec()->get_type_spec_list()->front().get_type_name()
											== "void")
					{
						iter->get_cfd()->get_decl_spec()->get_type_spec_list()->front().set_type_name(
								"int");
					}
				}
			}
		}
		//设置编码工程所用到的普通临时变量个数的最大值
		project_tempvar_num = tempvar_num_max >= project_tempvar_num ? 
												 tempvar_num_max : project_tempvar_num;
	
		//设置编码工程中所用到的条件临时变量个数的最大值			  	
		project_tempcondvar_num = tempcondvar_num_max >= project_tempcondvar_num ? 
														tempcondvar_num_max : project_tempcondvar_num;
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="spilt the program : "+str;
		throw error_info;
	}
}
//保存拆分前临时变量的索引号
void SaveTmpVarIndexNo(TFuncTmpRange *__FuncRange)
{
	__FuncRange->VarRange[0].V_Start=ACIntTmpCnt;

	__FuncRange->VarRange[1].V_Start=ACUIntTmpCnt;

	__FuncRange->VarRange[2].V_Start=IfTmpBoolCnt;

	__FuncRange->VarRange[3].V_Start=IfBoolSubCnt;

	__FuncRange->VarRange[4].V_Start=WhileTmpBoolCnt;

	__FuncRange->VarRange[5].V_Start=WhileBoolSubCnt;

	__FuncRange->VarRange[6].V_Start=TmpRelationBoolCnt;

	__FuncRange->VarRange[7].V_Start=void_func_counts;

	__FuncRange->VarRange[8].V_Start=WhileAC_AB_Cnt;

	__FuncRange->VarRange[9].V_Start=WhileAC_AB_Cnt;
}



//更新函数中临时变量的数据范围
void UpdateFuncTmpRange(TFuncTmpRange *__FuncRange)
{
	__FuncRange->VarRange[0].V_Start+=1;
	__FuncRange->VarRange[0].V_End=ACIntTmpCnt;

	__FuncRange->VarRange[1].V_Start+=1;
	__FuncRange->VarRange[1].V_End=ACUIntTmpCnt;

	__FuncRange->VarRange[2].V_Start+=1;
	__FuncRange->VarRange[2].V_End=IfTmpBoolCnt;

	__FuncRange->VarRange[3].V_Start+=1;
	__FuncRange->VarRange[3].V_End=IfBoolSubCnt;

	__FuncRange->VarRange[4].V_Start+=1;
	__FuncRange->VarRange[4].V_End=WhileTmpBoolCnt;

	__FuncRange->VarRange[5].V_Start+=1;
	__FuncRange->VarRange[5].V_End=WhileBoolSubCnt;

	__FuncRange->VarRange[6].V_Start+=1;
	__FuncRange->VarRange[6].V_End=TmpRelationBoolCnt;

	__FuncRange->VarRange[7].V_Start=1;
	__FuncRange->VarRange[7].V_End=void_func_counts==0? 0:1;

	__FuncRange->VarRange[8].V_Start+=1;
	__FuncRange->VarRange[8].V_End=WhileAC_AB_Cnt;

	__FuncRange->VarRange[9].V_Start+=1;
	__FuncRange->VarRange[9].V_End=WhileAC_AB_Cnt;

}
