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

// �������ļ� :  src/main_utils.cpp
extern int project_tempvar_num;
extern int project_tempcondvar_num;
//2010��3��10��׷��
extern string file_info;
extern void check_pt_arg(void *pt,string info);
//2014-12-15
extern map<string,TFuncTmpRange> g_MapFuncTmpVar;

// �������ļ� :./split_utils.c
extern vector<int>* tmpconds;
extern int tempvar_num_max;
extern int tempcondvar_num_max;
extern const int VOID_RET_VALUE;

// �������ļ� : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
// �������ļ� : ./split_stmt.c
extern void split_stmt_list(list<Statement>* s);
//�������ļ���./spit_utils.cpp
extern Statement* generate_newstmt(Expression* exp, string str, int* num);
extern int temp_var_num;

// �������ļ�: src/back_end/back_end_utils.cpp
extern int goto_label;

void UpdateFuncTmpRange(TFuncTmpRange *__FuncRange); //���º�������ʱ���������ݷ�Χ
void SaveTmpVarIndexNo(TFuncTmpRange *__FuncRange); //������ǰ��ʱ������������

// ���ܣ������м�ṹ�����޷���ֵ�������д���ǿ�����ӷ���ֵ
// ���������s-�м�ṹ����ָ��
// ���������s-�м�ṹ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
bool trans_void_func(Statement* s, list<Statement> *stmts, bool bFlagRet)
{
	try
	{
		//�ж���������
		switch(s->get_stamp())
		{
			case 0:
			{
				//���Ϊ������䣬�������������е��������
				for(list<Statement>::iterator iter=s->get_ccstmt()->get_csl()->begin();
					   iter!=s->get_ccstmt()->get_csl()->end();iter++)
				{
					trans_void_func(&(*iter), stmts, bFlagRet);//����ÿ�����
				}
			}
			break;
			case 1:
			{
				//����if��֧�е����
				trans_void_func(s->get_csstmt()->get_cs_if(), stmts, bFlagRet);

				//�ж��Ƿ���else��֧
				if(1==s->get_csstmt()->get_stamp())
				{
					//�������else��֧�������else��֧�е����
					trans_void_func(s->get_csstmt()->get_cs_else(), stmts, bFlagRet);
				}
				else
				{
					//�����else��֧�������
				}
			}
			break;
			case 2:
			{
				//����ѭ�����е����
				trans_void_func(s->get_cistmt()->get_cs(), stmts, bFlagRet);
			}
			break;
			case 5:
			{
				//�����������
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
						//����ӿڣ������д���
					}
					else
					{
						//һ���Ǻ���������䣬�Ǹ�ֵ��䣬������������1
						void_func_counts ++;
					}

					//������ڽ����޷���ֵ��������ֵ�ı������������
					//Var* void_left_var = new Var("AC_void_func_RetValue" + int2str(void_func_counts));
					Var* void_left_var = new Var(VOID_FUNC_RETVAL);//�����޸ģ��޷���ֵǿ�Ʒ��صı��������ϲ�ʱ��ע�⣡
					Expression *left_exp = new Expression(void_left_var);
					Expression* zero_value = new Expression("0");
					Expression* void_left_exp = new Expression(left_exp, "=", zero_value);
					//void_left_exp->set_stamp(4);
					Statement* new_stmt = new Statement(void_left_exp);
					new_stmt->set_line_begin(line_num);
					new_stmt->set_fake_line(fake_line_num);
					temp_var_num++;	//��ͨ��ʱ����������1
					//stmts->push_front(*new_stmt);


					//���У�����
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
						temp_var_num++;	//��ͨ��ʱ����������1

						if(bFlagRet)
						{
							stmts->insert(--stmts->end(), *new_stmt2);
						}
						else
						{
							stmts->push_back(*new_stmt2);
						}
					}


					//�����������ø�ֵ���

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
				//������䲻����������
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "trans void func error the program : " + str;
		throw error_info;
	}
}

// ���ܣ������м�ṹ������Ҫ��ֵĵط����в�֣������������ʽ�������жϱ��ʽ�ȣ���¼
// 			���ʹ�õ���ʱ�����ĸ���
// ���������s-�м�ṹ����ָ��
// ���������s-�м�ṹ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void split_pro(Program *s)
{	
	try
	{
		//���������м�ṹ����ָ���Ƿ�Ϊ��
		check_pt_arg(s,"program struct object's pointer");
		tmpconds->push_back(0);
		goto_label=0;	//goto�����0
		TFuncTmpRange *CurFuncTmpRange=NULL;

		bool ifVoidFunc = false;
		//������������
		for(list<Declaration>::iterator iter=s->get_declist()->begin();iter!=s->get_declist()->end();iter++)
		{
			//2010��3��10��׷��
			file_info=iter->get_file_info();//���õ�ǰ������λ�ڵ��ļ���
			
			//�жϵ�ǰ��������ָ�������Ƿ�Ϊ�������������Ҹú��������Ƿ���к�����
			if(1==iter->get_stamp() && 0==iter->get_cfd()->get_stamp())
			{	
				//��ʼ����ʱ����������
				InitSplitTempVarCnt();
				//����Ǵ��к�����ĺ���������������������е��������

				CurSplitFuncName = iter->get_cfd()->get_fun_name();
				CurFuncNameHash = iter->get_cfd()->get_fun_name(); //2014-12-01���ӣ�ָ����ǰ����

				CurFuncTmpRange = new TFuncTmpRange();  //������Ըú�������ʱ���������ŷ�Χ

				SaveTmpVarIndexNo(CurFuncTmpRange);
				split_stmt_list(iter->get_cfd()->get_ccstmt()->get_csl());

				list<Statement> *stmts = iter->get_cfd()->get_ccstmt()->get_csl();
				//�жϵ�ǰ�����Ƿ���ڷ���ֵ���
				bool bFlagRet = false;
				for(list<Statement>::iterator iter = stmts->begin();iter != stmts->end();iter++)
				{
					if(iter->get_stamp()==3)
					{
						bFlagRet = true;
						break;
					}
				}

				//�����һ���Ժ����������е�ѭ����֧�����в���

				for (list<Statement>::iterator iter2 = stmts->begin();iter2 != stmts->end(); iter2++)
				{
					//����ǰ�����if���
					if(1 == iter2->get_stamp())
					{
						Selection_stmt* s = iter2->get_csstmt();
						Statement *cur_stmts = s->get_cs_if();
						//����if��֧�е���䣬��¼���еķǺ�����ֵ���õĸ���
						trans_void_func(cur_stmts,stmts,bFlagRet);

						//�ж��Ƿ���else��֧
						if(1==s->get_stamp())
						{
							//���Ϊif-else�ṹ������else�����
							trans_void_func(s->get_cs_else(),stmts,bFlagRet);
						}
						else
						{
							//���û��else��֧�������
						}

						//���ҵ����޸�ֵ�������ã���֮��Ϊ�и�ֵ��������

					}
					//����ǰ�����while���
					if(2 == iter2->get_stamp())
					{
						//iter2->get_cistmt();
						Iteration_stmt* s = iter2->get_cistmt();
						Statement *cur_stmts = s->get_cs();
						//����if��֧�е���䣬��¼���еķǺ�����ֵ���õĸ���
						trans_void_func(cur_stmts,stmts,bFlagRet);

					}
				}

				//������޷���ֵ��������ǿ�����ӷ������
				//�˴�ֻ����ͨ�û����庯�����в�����У����ⲿ��������ӿڲ����в���
				if (iter->get_cfd()->get_decl_spec()->get_type_spec_list()->front().get_type_name()
						== "void" && "F_VCE_CheckVar" != CurSplitFuncName && "F_VCE_CheckArr" != CurSplitFuncName
						&& "F_USR_DataFetch" != CurSplitFuncName && "F_USR_DataRecv" != CurSplitFuncName
						&& "F_USR_DataSend" != CurSplitFuncName && "F_USR_DataDeliver" != CurSplitFuncName)
				{
					//�½�һ������ֵ������
					Return_stmt* return_stmt = new Return_stmt();
					return_stmt->set_stamp(1);
					return_stmt->set_line_begin(line_num);
					return_stmt->set_fake_line(fake_line_num);

					//�½�Var&Expression
					//Var* return_zero = new Var(iter->get_cfd()->get_fun_name());
					Expression* return_zero = new Expression("0");
					Expression* zero_value = new Expression(*return_zero);

					return_stmt->set_ce(zero_value);
					Statement* stmt = new Statement(return_stmt);

					Statement* new_stmt = new Statement(return_stmt);
					new_stmt->set_line_begin(line_num);
					new_stmt->set_fake_line(fake_line_num);
					temp_var_num++;	//��ͨ��ʱ����������1

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

				//������޷���ֵ����������ǿ��ת��Ϊ����ֵ����Ϊint�ĺ���
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
				//��ǰ��������ָ�������Ǻ������������Ҹú������������к�����
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
		//���ñ��빤�����õ�����ͨ��ʱ�������������ֵ
		project_tempvar_num = tempvar_num_max >= project_tempvar_num ? 
												 tempvar_num_max : project_tempvar_num;
	
		//���ñ��빤�������õ���������ʱ�������������ֵ			  	
		project_tempcondvar_num = tempcondvar_num_max >= project_tempcondvar_num ? 
														tempcondvar_num_max : project_tempcondvar_num;
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="spilt the program : "+str;
		throw error_info;
	}
}
//������ǰ��ʱ������������
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



//���º�������ʱ���������ݷ�Χ
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
