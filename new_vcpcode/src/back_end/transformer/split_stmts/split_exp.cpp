/*
 * split_exp.cpp
 *
 *  Created on: 2015��7��26��
 *      Author: LordVoldemort
 */

#include "CodedVarBuild_Util.h"
#include "ControlFlow_CodeUtil.h"
#include "expression.h"
#include "var.h"
#include "statement.h"s
#include "app_utils.h"

/***************������main_utils.cpp��***************************/
extern int fake_line_num;
extern int line_num;

/**************������split_utils.cpp��***************************/
extern void split_var(Var* s,list<Statement>* l);


/***************���ļ�����ĺ���**********************************/
extern void split_exp_start(Expression *s,list<Statement> *l);
extern Expression* split_exp(Expression *s,list<Statement> *l);
extern void split_condition_exp_start(Expression* s,list<Statement>* l);


// ���ܣ���ַ�֧ѭ����ڴ����������ʽ�������������ʽΪ���������ʽ���Ƕ������жϱ��ʽ������
// 			��ͬ�Ĳ�ַ��������ж������жϱ��ʽ��Ҫ��ֳɷ�֧���
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ��
// 			       l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void split_condition_exp_start(Expression* s,list<Statement>* l)
{
	Expression *ExpResultVar=NULL;
	ExpResultVar=split_exp(s, l);

	//����߼�ֵ��ȥTRUE�����
//	Build_CF_CondSub_Var(ExpResultVar->get_cvar(),l);
	s->set_stamp(0);
	s->set_op(">=");
	s->set_cel(ExpResultVar);
	s->set_cer(new Expression("0"));
//	s->set_cer(new Expression("1"));
}

//���ܣ����ʽ��ֵ���ں���
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

		//�޸ı��ʽs����ʽ
		Statement &LastStmt=l->back();
		*s=*LastStmt.get_ce();
		l->pop_back();
	}
}

// ���ܣ��Ա��ʽ���з����������Ҫ��֣�����в�֣�������ʱ������ֵ��䣬��ͨ�����������ɵ�
//			�����䷵��
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ��
//      			  l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
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
			//�ձ��ʽ
		}
		break;
		case 0:
		{
			Expression* pLeftExp=split_exp(s->get_cel(), l);
			Expression* pRightExp=split_exp(s->get_cer(), l);
			//����������
			Expression* pResultExp=new Expression(new Var(BuildTmpVarName(::SplitArithmTmpVarPrefix, CurFuncNameHash, ++*SplitArithmTmpVarCntPtr)));
			//����������ʽ
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
			//���Ϊ�����������ʽ���������ұ��ʽ
			Expression *pRightVar=split_exp(s->get_cer(), l);//����ұ��ʽ
			//����������
			Expression* pResult=new Expression(new Var(BuildTmpVarName(::SplitArithmTmpVarPrefix, CurFuncNameHash, ++*SplitArithmTmpVarCntPtr)));
			//����������ʽ
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
			//���Ϊ���ű��ʽ�������������ڱ��ʽ
			Expression *pResult=split_exp(s->get_exp(), l);//������ű��ʽ

			return pResult;
		}
		break;
		case 3:
		{
			//���Ϊ�������ʽ

			Expression *pResult=new Expression(s->get_c());


			return pResult;
		}
		break;
		case 4:
		{
			//�������ʽ�Ĵ���
			Expression *pReslut=NULL;

			if(s->get_cvar()->get_stamp()==1)//��������飬�������ֵ���
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
			//���Ϊ�������ñ��ʽ���򱨳�������Ϣ
			log_error("split exp, and the exp is function.\n")
			abort();
		}
		break;
		default:
		{
			//�������͵ı��ʽ���Ƿ�������������Ϣ
			log_error("split invalid expression.\n")
			abort();
		}
	}
}

/*
 * ���������������ж������
 * �����жϱ����������ֵ�����Ĺ�ϵ
 * ���磺if_bool==True ���ڵı���Ϊ sub_1 sub_2,����
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
