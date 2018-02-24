#include "CodedVarBuild_Util.h"
#include "expression.h"
#include "var.h"
#include "statement.h"
#include "compound_stmt.h"
#include "return_stmt.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "declaration.h"
#include "ControlFlow_CodeUtil.h"

//定义在文件: ./split_utils.cpp
extern int if_while_deep;
extern int if_deep;
extern int while_deep_num;
extern vector<int>* tmpconds;

// 定义在文件 : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern int tempvar_num_max;
extern int tempcondvar_num_max;
extern string int2str(int num);
extern void check_pt_arg(void *pt,string info);

//定义在文件 ：./split_exp.cpp
extern Expression *split_exp(Expression* s,list<Statement>* l);
extern void split_exp_start(Expression *s,list<Statement> *l);

//定义在文件 ：./split_cond_exp.cpp
extern void split_condition_exp_start(Expression* s,list<Statement>* l);

// 定义在文件: src/back_end/back_end_utils.cpp
extern int goto_label;

//定义在文件：./spit_utils.cpp
extern int temp_var_num;
extern int tempcond_var_num;
extern int  judge_exp_type(Expression* s);
extern void split_var(Var* s,list<Statement>* l);
extern void split_args(list<Expression>* s,list<Statement>* l);
extern Statement* generate_newstmt(Expression* exp,string str,int* num);
extern void split_args_check(list<Expression>* s,list<Statement>* l);
//定义在文件：src/back_end/generator/gen_stmt/gen_iterat_stmt.cpp
extern map<CFCondVar,int> g_WhileMap_CondVar_SumVar;



void split_stmt_list(list<Statement>* s);
static void split_stmt(Statement* s,list<Statement>*l);

 
// 功能：对赋值语句进行拆分，要遍历左值变量和赋值表达式，调用变量拆分函数和表达式拆分函数，
//			对需要拆分地方进行拆分
// 输入参数：var-左值变量对象指针
// 			      exp-赋值表达式对象指针
// 输出参数：var-左值变量对象指针
//            	  exp-赋值表达式对象指针
//                  l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void split_assign_stmt(Var* var, Expression* exp,list<Statement>* l)
{
	try
	{
		//检查参数：左值变量对象指针的有效性
		check_pt_arg(var,"left variable object's pointer");
		//检查参数：赋值表达式对象指针的有效性
		check_pt_arg(exp,"assign expression object's pointer");
		//检查参数：语句链表指针的有效性
		check_pt_arg(l,"statement list's pointer");
		
		list<Statement>* stmts_var=new list<Statement>;//存储拆分左值变量时生成的语句
		list<Statement>* stmts_exp=new list<Statement>;//存储拆分赋值表达式时生成的语句
		

		split_var(var,stmts_var);//拆分左值变量
		//判断左值变量是否为数组变量
		if(1==var->get_stamp())
		{
			//判断是否为函数赋值运算
			if(exp->get_stamp()==5)
			{
				//如果为函数赋值，则遍历函数实参表达式链表
				split_args(exp->get_args(),stmts_exp);
			}
			else
			{
				split_exp_start(exp,stmts_exp);//拆分赋值表达式
			}
			
			//判断赋值表达式经过拆分后是否为简单变量或者常数
			if(1==judge_exp_type(exp))
			{
				//如果不是，则需生成一条临时变量语句
				Var *new_left_var=new Var(BuildTmpVarName(SplitArithmTmpVarPrefix,CurFuncNameHash,++*SplitArithmTmpVarCntPtr),::tmp_var_style);
				Statement *new_stmt=new Statement(new_left_var,new Expression(*exp));
				new_stmt->set_line_begin(line_num);
				new_stmt->set_fake_line(fake_line_num);
				stmts_exp->push_back(*new_stmt);//将临时变量赋值语句插入拆分表达式式生成的语句之后

				Expression *new_exp=new Expression(new_left_var);
				*exp=*new_exp;

				delete new_stmt;
			}
		}
		else
		{
			//如果是普通变量，则拆分赋值表达式
			//判断是否为函数赋值运算
			if(exp->get_stamp()==5)
			{
				//如果为函数赋值，则遍历函数实参表达式链表
				split_args(exp->get_args(),stmts_exp);
			}
			else
			{
				split_exp_start(exp,stmts_exp);//拆分赋值表达式
			}
		}
	   
		//将拆分所生成的语句链表插入到通过参数传递的语句链表中
		l->insert(l->end(),stmts_var->begin(),stmts_var->end());
		l->insert(l->end(),stmts_exp->begin(),stmts_exp->end());
		delete stmts_var;
		delete stmts_exp;
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan assign statement : "+str;
		throw error_info;
	}
}

// 功能：遍历语句链表，对链表中的每条语句调用语句拆分函数进行拆分
// 输入参数：s-语句链表指针
// 输出参数：s-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void split_stmt_list(list<Statement>* s)
{
	try
	{
		//检查参数：语句链表指针是否为空
		check_pt_arg(s,"statement list's pointer");
		//构造一个临时语句链表，存储拆每条语句分时生成的语句
		list<Statement>* new_stmts=new list<Statement>;
		//遍历语句链表
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//设置当前处理位于的行号
			fake_line_num=iter->get_fake_line();
			split_stmt(&(*iter),new_stmts);//拆分当前语句
			//将拆分生成的语句插入到当前被分析的语句前面
			s->insert(iter,new_stmts->begin(),new_stmts->end());
			new_stmts->clear();//清空临时语句链表
		}
		delete new_stmts;
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan the statement list : "+str;
		throw error_info;
	}
}

// 功能：遍历返回语句，对返回的表达式进行拆分，拆分结束后，再次判断表达式是否需要进一步的拆分
//			如果需要，则再生成一个临时变量语句
// 输入参数：rtstmt-返回语句对象指针
// 输出参数：rtstmt-返回语句对象指针，s-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void split_return_stmt(Return_stmt* rtstmt,list<Statement>* s)
{
	try
	{
		//检查参数：返回语句对象指针是否为空
		check_pt_arg(rtstmt,"return statement object's pointer");
		//检查参数：语句链表指针是否为空
		check_pt_arg(s,"statement list's pointer");
		
		//判断返回语句是否带有返回值
		if(1==rtstmt->get_stamp())
		{
			//如果有返回值，则遍历返回表达式
			//拆分返回语句的表达式
			//设置临时变量拆分的工具参数
			set_tmpvar_split_utils_exp(rtstmt->get_ce());
			//对返回值表达式进行拆分
			Expression *RetExp=split_exp(rtstmt->get_ce(),s);
			//修改返回值表达式
			*rtstmt->get_ce()=*RetExp;

			delete RetExp;
		}
		else
		{
			//如果无返回值，则忽略
		}	
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan return statement : "+str;
		throw error_info;
	}
}

// 功能：用于遍历循环语句的循环体或者分支语句的if分支、else分支，将其中需要拆分的语句进行拆分，
// 			如果拆分时有语句生成，则将生成的语句同原有的语句合并起来，构造成一个新的复合语句，
// 			将此复合语句封装成一条语句返回；如果拆分时无语句生成，则返回为NULL的语句指针
// 输入参数：s-待遍历的循环体语句或者if分支语句或者else分支语句
// 输出参数：s-待遍历的循环体语句或者if分支语句或者else分支语句
// 返回值：成功则返回语句对象指针，失败则抛出错误信息
// 作者：李刚
// 注意：本函数中使用new为返回的语句分配了内存，需要上层函数释放
static Statement* split_iter_select_body(Statement* s)
{
	try
	{
		//检查参数：语句对象指针是否为空
		check_pt_arg(s,"statement object's pointer");
		
		//存储本函数要返回的语句对象指针
		Statement* new_stmt=NULL;
		
		//存储拆分时生成的语句
		list<Statement>* stmt_list= new list<Statement>;
		//拆分语句
		split_stmt(s,stmt_list);
		//判断有无语句生成
		if(1!=stmt_list->empty())
		{
			//如果有，则将拆分时生成的语句，同原来的语句合并，构造成一个新的复合语句
			stmt_list->push_back(*s);//合并新旧语句
			list<Declaration>* dclr=new list<Declaration>;//构造空的声明链表
			//构造复合语句
			Compound_stmt* compd_stmt=new Compound_stmt(dclr,stmt_list);
			compd_stmt->set_line_begin(s->get_line_begin());
			compd_stmt->set_line_end(line_num);
			compd_stmt->set_fake_line(s->get_fake_line());
			new_stmt=new Statement(compd_stmt);
	
			delete dclr;
		}
		else
		{
			//如果无语句生成，则忽略
		}
		delete stmt_list;
		return new_stmt;
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan while or if or else body : "+str;
		throw error_info;
	}
}

// 功能：遍历循环语句，对循环语句进行分析，包括循环入口的表达式及循环内部的语句，如果需要拆分，
//			则生成拆分的语句，本模块传递给上层模块的语句为，对循环入口表达式拆分时所生成的语句
// 输入参数：s-循环语句对象指针
// 输出参数：s-循环语句对象指针
// 			      l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void split_iterate_stmt(Iteration_stmt* s,list<Statement>* l)
{
	try
	{
		//检查参数：循环语句对象指针是否为空
		check_pt_arg(s,"iteration statement object's pointer");
		//检查参数：语句链表指针是否为空
		check_pt_arg(l,"statement list's pointer");
		
		while_deep_num++;//循环嵌套层数加1
		if_while_deep++;//分支循环嵌套层数加1
		
		//tempcond_var_num=tmpconds->back();//取出上层条件表达式拆分后条件临时变量已经使用的个数
		
		list<Statement>* stmt1=new list<Statement>;//存储拆分条件表达式时生成的语句
		
		list<Statement> *While_Sum_StmtLst=new list<Statement>; //存储算法中签名累加变量的初始化语句

		//AC_WhileA/B_index.C1/C2 的索引号index 加 1
		WhileAC_AB_Cnt++;



		split_condition_exp_start(s->get_ce(),stmt1);	//拆分条件表达式
		
		//构造循环条件变量对应的AC_WHILE_A/B的索引号
		CFCondVar Iterator_CondVar;
		Iterator_CondVar.CondVar.FuncName=CurSplitFuncName;
		Iterator_CondVar.CondVar.VarName=s->get_ce()->get_cel()->get_cvar()->get_name();
		g_WhileMap_CondVar_SumVar.insert(make_pair(Iterator_CondVar,WhileAC_AB_Cnt));
		
		Statement* stmt_while=split_iter_select_body(s->get_cs());//遍历循环体，拆分语句
		
		//判断拆分循环体返回的语句指针是否为空
		if(NULL!=stmt_while)
		{
			//如果不为空，则将循环体中的语句修改为拆分返回的语句
			s->set_cs(stmt_while);
		}
		else
		{
			//如果为空，则忽略
		}
		
		//将循环条件计算语句插入到循环体语句中
		list<Statement> *Iterator_StmtLst=NULL;
		if(s->get_cs()->get_stamp()==0)//复合语句
		{
			Iterator_StmtLst=s->get_cs()->get_ccstmt()->get_csl();
			Iterator_StmtLst->insert(Iterator_StmtLst->end(),stmt1->begin(),stmt1->end());
		}
		else
		{
			//构造语句链表
			Iterator_StmtLst=new list<Statement>;
			Iterator_StmtLst->push_back(*(s->get_cs()));
			Iterator_StmtLst->insert(Iterator_StmtLst->end(),stmt1->begin(),stmt1->end());

			//构造空的声明链表
			list<Declaration> *Iterator_declr=new list<Declaration>;

			//构造复合语句
			Compound_stmt *IterComp=new Compound_stmt(Iterator_declr,Iterator_StmtLst);
			s->set_cs(new Statement(IterComp));
		}

		while_deep_num--;//循环嵌套层数减1
		if_while_deep--;//分支和循环嵌套层数减1

		//将上述四条语句插入到拆分条件表达式生成的语句链表最前面
		stmt1->insert(stmt1->begin(),While_Sum_StmtLst->begin(),While_Sum_StmtLst->end());

		//将生成的语句通过参数输出
		l->insert(l->end(),stmt1->begin(),stmt1->end());

		While_Sum_StmtLst->clear();
		stmt1->clear();
		delete While_Sum_StmtLst;
		delete stmt1;
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan iteration statement : "+str;
		throw error_info;
	}
}

// 功能：遍历分支语句，对分支语句进行分析，包括分支入口的表达式及分支结构内部的语句。如果需要
//			拆分，则生成拆分的语句，本模块传递给上层模块的语句为，对分支入口表达式拆分时所生成的语句
// 输入参数：s-分支语句对象指针
// 输出参数：s-分支语句对象指针
// 			      l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void split_select_stmt(Selection_stmt* s,list<Statement>* l)
{
	try
	{
		//检查参数：分支语句对象指针是否为空
		check_pt_arg(s,"select statement object's pointer");
		//检查参数：语句链表指针是否为空
		check_pt_arg(l,"statement list's pointer");
		
		if_while_deep++;//分支循环嵌套层数加1
		if_deep++;//分支嵌套层数加1
			
		list<Statement>* stmt_exp = new list<Statement>;//存储拆分条件表达式时所生成的语句
		
		split_condition_exp_start(s->get_ce(),stmt_exp);//拆分条件表达式
		
		//将拆分条件表达式生成的语句通过参数输出
		l->insert(l->end(),stmt_exp->begin(),stmt_exp->end());
		delete stmt_exp;
	
		Statement *stmt_if=split_iter_select_body(s->get_cs_if());//拆分分支结构的if分支语句
		
		//判断拆分if分支返回的语句指针是否为空
		if(NULL!=stmt_if)
		{
			//如果不为空，则将if分支中的语句修改为拆分返回的语句
			s->set_cs_if(stmt_if);
			//delete stmt_if;
		}
		else
		{
			//如果为空，则忽略
		}

		//判断该分支结构是否含有else分支
		if(1==s->get_stamp())
		{
			//如果有else分支，则遍历else分支
			Statement *stmt_else=split_iter_select_body(s->get_cs_else());//对else分支进行遍历，拆分
			
			//判断拆分else分支返回的语句指针是否为空
			if(NULL!=stmt_else)
			{
				//如果不为空，则将else分支中的语句修改为拆分返回的语句
				s->set_cs_else(stmt_else);
			}
			else
			{
				//如果为空，则忽略
			}
		}
		else
		{
			//如果不含else分支，则忽略
		}
		
		if_while_deep--;//分支循环嵌套层数减1
		if_deep--;//分支嵌套层数减1
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan select statement : "+str;
		throw error_info;
	}
}

// 功能：分析语句，根据语句的不同类型，调用不同的函数对语句进行拆分
// 输入参数：s-语句对象指针
// 输出参数：s-语句对象指针
//			       l-语句链表指针，用于存储拆分时生成的语句
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void split_stmt(Statement* s,list<Statement>*l)
{
	try
	{
		//检查参数：语句链表指针是否为空
		check_pt_arg(l,"statement list's pointer");
		//检查参数：语句对象指针是否为空
		check_pt_arg(s,"statement object's pointer");
		//将普通临时变量个数置0
		temp_var_num=0;
		//判断语句的类型
		switch(s->get_stamp())
		{
			case -1:
			{
				//空语句，忽略
			}
			break;
			case 0:
			{
				//如果是复合语句，则遍历复合语句中的语句链表
				split_stmt_list(s->get_ccstmt()->get_csl());
			}
			break;
			case 1:
			{
				set_tmpvar_split_utils_select();
				split_select_stmt(s->get_csstmt(),l);
			}
			break;
			case 2:
			{
				set_tmpvar_split_utils_while();
				split_iterate_stmt(s->get_cistmt(),l);
			}
			break;
			case 3:
			{
				//如果是返回语句，遍历返回语句
				split_return_stmt(s->get_crs(),l);
				//设置当前拆分时用到的普通临时变量个数的最大值
//				tempvar_num_max = temp_var_num>=tempvar_num_max ?
//				temp_var_num : tempvar_num_max;
			}
			break;
			case 4:
			{
				//如果是赋值语句，则遍历赋值语句
				set_tmpvar_split_utils_arithm(s->get_cvar());
				split_assign_stmt(s->get_cvar(),s->get_ce(),l);

//				设置当前拆分时用到的普通临时变量个数的最大值
//				tempvar_num_max = temp_var_num>=tempvar_num_max ?
//				temp_var_num : tempvar_num_max;
			}
			break;
			case 5:
			{
				//如果是表达式语句，则遍历表达式
				//判断是否为函数调用语句
				if(s->get_ce()->get_stamp()==5)
				{
					string func_name = s->get_ce()->get_c();
					if("F_VCE_CheckArr" == func_name || "F_VCE_CheckVar" == func_name)
					{
						//校验接口函数不需要进行参数处理
						split_args_check(s->get_ce()->get_args(),l);
					}
					else
					{
						//如果为函数调用，则遍历函数实参表达式链表
						split_args(s->get_ce()->get_args(),l);
					}
				}
				else
				{
					//如果不是函数调用，则此表达式语句出错，报错
					string error_info="expression statement is invalid!";
					throw error_info;
				}

				//设置当前拆分时用到的普通临时变量个数的最大值
				tempvar_num_max = temp_var_num>=tempvar_num_max ? 
				temp_var_num : tempvar_num_max;
			}
			break;
			case 6:
			{
				//如果是分号语句，忽略
			}
			break;
			default:
			{
				//如果标签为其他值，则非法，抛出错误信息
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan statement : "+str;
		throw error_info;
	}
}
