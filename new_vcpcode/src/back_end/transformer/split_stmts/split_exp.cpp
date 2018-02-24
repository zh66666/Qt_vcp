/*
 * split_exp.cpp
 *
 *  Created on: 2015年7月26日
 *      Author: LordVoldemort
 */

#include "CodedVarBuild_Util.h"
#include "ControlFlow_CodeUtil.h"
#include "expression.h"
#include "var.h"
#include "statement.h"s
#include "app_utils.h"

/***************定义在main_utils.cpp中***************************/
extern int fake_line_num;
extern int line_num;

/**************定义在split_utils.cpp中***************************/
extern void split_var(Var* s,list<Statement>* l);


/***************本文件定义的函数**********************************/
extern void split_exp_start(Expression *s,list<Statement> *l);
extern Expression* split_exp(Expression *s,list<Statement> *l);
extern void split_condition_exp_start(Expression* s,list<Statement>* l);


// 功能：拆分分支循环入口处的条件表达式，根据条件表达式为单条件表达式还是多条件判断表达式，采用
// 			不同的拆分方法，其中多条件判断表达式需要拆分成分支语句
// 输入参数：s-表达式对象指针
// 输出参数：s-表达式对象指针
// 			       l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void split_condition_exp_start(Expression* s,list<Statement>* l)
{
	Expression *ExpResultVar=NULL;
	ExpResultVar=split_exp(s, l);

	//添加逻辑值减去TRUE的语句
//	Build_CF_CondSub_Var(ExpResultVar->get_cvar(),l);
	s->set_stamp(0);
	s->set_op(">=");
	s->set_cel(ExpResultVar);
	s->set_cer(new Expression("0"));
//	s->set_cer(new Expression("1"));
}

//功能：表达式拆分的入口函数
void split_exp_start(Expression *s,list<Statement>* l)
{
	if(!s)
	{
		log_error("expression pointer is NULL.\n")
		abort();
	}
	Expression *ExpResult=NULL;
	ExpResult=split_exp(s,l);

	if(!ExpResult)
	{
		log_error("expression result pointer is NULL.\n")
		abort();
	}
	if(ExpResult->get_stamp()==4 &&
			ExpResult->get_cvar()->get_name().substr(0,::SplitArithmTmpVarPrefix.size())==::SplitArithmTmpVarPrefix)
	{
		//if(s->get_stamp()==0 || s->get_stamp()==1)
		(*SplitArithmTmpVarCntPtr)--;

		//修改表达式s的形式
		Statement &LastStmt=l->back();
		*s=*LastStmt.get_ce();
		l->pop_back();
	}
}

// 功能：对表达式进行分析，如果需要拆分，则进行拆分，生成临时变量赋值语句，并通过参数将生成的
//			拆分语句返回
// 输入参数：s-表达式对象指针
// 输出参数：s-表达式对象指针
//      			  l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
Expression* split_exp(Expression *s, list<Statement>* l)
{
	if(!s)
	{
		log_error("expression pointer is NULL.\n")
		abort();
	}
	switch(s->get_stamp())
	{
		case -1:
		{
			//空表达式
		}
		break;
		case 0:
		{
			Expression* pLeftExp=split_exp(s->get_cel(), l);
			Expression* pRightExp=split_exp(s->get_cer(), l);
			//运算结果变量
			Expression* pResultExp=new Expression(new Var(BuildTmpVarName(::SplitArithmTmpVarPrefix, CurFuncNameHash, ++*SplitArithmTmpVarCntPtr)));
			//构建运算表达式
			string op;
			if(s->get_op()==">=")
				 op="-";
				else
					 op=s->get_op();
			Expression *pNewExp=new Expression(pLeftExp, op, pRightExp);
			Statement *pNewStmt=new Statement(pResultExp->get_cvar(), pNewExp);
			pNewStmt->set_fake_line(fake_line_num);
			pNewStmt->set_line_begin(line_num);
			l->push_back(*pNewStmt);
			delete pNewStmt;

			return pResultExp;
		}
		break;
		case 1:
		{
			//如果为单操作数表达式，则拆分其右表达式
			Expression *pRightVar=split_exp(s->get_cer(), l);//拆分右表达式
			//运算结果变量
			Expression* pResult=new Expression(new Var(BuildTmpVarName(::SplitArithmTmpVarPrefix, CurFuncNameHash, ++*SplitArithmTmpVarCntPtr)));
			//构建运算表达式
			Expression *pNewExp=new Expression(s->get_op(), pRightVar);
			Statement *pNewStmt=new Statement(pResult->get_cvar(), pNewExp);
			pNewStmt->set_fake_line(fake_line_num);
			pNewStmt->set_line_begin(line_num);

			l->push_back(*pNewStmt);

			delete pNewStmt;
			return pResult;
		}
		break;
		case 2:
		{
			//如果为括号表达式，则拆分其括号内表达式
			Expression *pResult=split_exp(s->get_exp(), l);//拆分括号表达式

			return pResult;
		}
		break;
		case 3:
		{
			//如果为常数表达式

			Expression *pResult=new Expression(s->get_c());


			return pResult;
		}
		break;
		case 4:
		{
			//变量表达式的处理
			Expression *pReslut=NULL;

			if(s->get_cvar()->get_stamp()==1)//如果是数组，则输出赋值语句
			{
				split_var(s->get_cvar(), l);
				pReslut=new Expression(new Var(BuildTmpVarName(::SplitArithmTmpVarPrefix, CurFuncNameHash, ++*SplitArithmTmpVarCntPtr)));
				Statement *pNewStmt=new Statement(pReslut->get_cvar(), new Expression(*s));
				pNewStmt->set_fake_line(fake_line_num);
				pNewStmt->set_line_begin(line_num);

				l->push_back(*pNewStmt);
				delete pNewStmt;
			}
			else

				pReslut=new Expression(s->get_cvar());

			return pReslut;
		}
		break;
		case 5:
		{
			//如果为函数调用表达式，则报出错误信息
			log_error("split exp, and the exp is function.\n")
			abort();
		}
		break;
		default:
		{
			//其他类型的表达式，非法，报出错误信息
			log_error("split invalid expression.\n")
			abort();
		}
	}
}

/*
 * 构建控制流条件判断语句中
 * 条件判断变量与减法差值变量的关系
 * 例如：if_bool==True 对于的变量为 sub_1 sub_2,其中
 * sub_1=if_bool-True sub_2=0-sub_1
 *
 * __CondJudgeVar--->if_bool
 */
void Build_CF_CondSub_Var(Var *__CondJudgeVar,list<Statement> *l)
{
	Var *LeftVar_1=NULL;
	Var *LeftVar_2=NULL;
	Expression *RightExp=NULL;
	Statement *new_stmt=NULL;

	CFCondVar *new_condVar=new CFCondVar();
	CFSubVar *new_subVar=new CFSubVar();

	new_condVar->CondVar.FuncName=CurSplitFuncName;
	new_condVar->CondVar.VarName=__CondJudgeVar->get_name();

	//AC_ifSub_1=AC_IFBool_1-TRUE
	LeftVar_1=new Var(BuildTmpVarName(CondSubVarPrefix,CurFuncNameHash,++*CondSubVarCntPtr),::tmp_var_style);
	new_subVar->SubVar[0].FuncName=CurSplitFuncName;
	new_subVar->SubVar[0].VarName=LeftVar_1->get_name();

	RightExp=new Expression(new Expression(__CondJudgeVar),"-",new Expression("e_TRUE"));
	new_stmt=new Statement(LeftVar_1,RightExp);
	new_stmt->set_fake_line(fake_line_num);
	new_stmt->set_line_begin(line_num);
	l->push_back(*new_stmt);
	delete new_stmt;

	//AC_ifSub_2=0-AC_ifSub_1
	LeftVar_2=new Var(BuildTmpVarName(CondSubVarPrefix,CurFuncNameHash,++*CondSubVarCntPtr),::tmp_var_style);
	new_subVar->SubVar[1].FuncName=CurSplitFuncName;
	new_subVar->SubVar[1].VarName=LeftVar_2->get_name();

	RightExp=new Expression(new Expression("0"),"-",new Expression(LeftVar_1));
	new_stmt=new Statement(LeftVar_2,RightExp);
	new_stmt->set_fake_line(fake_line_num);
	new_stmt->set_line_begin(line_num);
	l->push_back(*new_stmt);
	delete new_stmt;

	g_CFMap_CondVar_SubVar.insert(make_pair(*new_condVar,*new_subVar));
	delete new_condVar;
	delete new_subVar;
}
