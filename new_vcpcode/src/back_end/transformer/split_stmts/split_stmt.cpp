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

//�������ļ�: ./split_utils.cpp
extern int if_while_deep;
extern int if_deep;
extern int while_deep_num;
extern vector<int>* tmpconds;

// �������ļ� : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern int tempvar_num_max;
extern int tempcondvar_num_max;
extern string int2str(int num);
extern void check_pt_arg(void *pt,string info);

//�������ļ� ��./split_exp.cpp
extern Expression *split_exp(Expression* s,list<Statement>* l);
extern void split_exp_start(Expression *s,list<Statement> *l);

//�������ļ� ��./split_cond_exp.cpp
extern void split_condition_exp_start(Expression* s,list<Statement>* l);

// �������ļ�: src/back_end/back_end_utils.cpp
extern int goto_label;

//�������ļ���./spit_utils.cpp
extern int temp_var_num;
extern int tempcond_var_num;
extern int  judge_exp_type(Expression* s);
extern void split_var(Var* s,list<Statement>* l);
extern void split_args(list<Expression>* s,list<Statement>* l);
extern Statement* generate_newstmt(Expression* exp,string str,int* num);
extern void split_args_check(list<Expression>* s,list<Statement>* l);
//�������ļ���src/back_end/generator/gen_stmt/gen_iterat_stmt.cpp
extern map<CFCondVar,int> g_WhileMap_CondVar_SumVar;



void split_stmt_list(list<Statement>* s);
static void split_stmt(Statement* s,list<Statement>*l);

 
// ���ܣ��Ը�ֵ�����в�֣�Ҫ������ֵ�����͸�ֵ���ʽ�����ñ�����ֺ����ͱ��ʽ��ֺ�����
//			����Ҫ��ֵط����в��
// ���������var-��ֵ��������ָ��
// 			      exp-��ֵ���ʽ����ָ��
// ���������var-��ֵ��������ָ��
//            	  exp-��ֵ���ʽ����ָ��
//                  l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void split_assign_stmt(Var* var, Expression* exp,list<Statement>* l)
{
	try
	{
		//����������ֵ��������ָ�����Ч��
		check_pt_arg(var,"left variable object's pointer");
		//����������ֵ���ʽ����ָ�����Ч��
		check_pt_arg(exp,"assign expression object's pointer");
		//���������������ָ�����Ч��
		check_pt_arg(l,"statement list's pointer");
		
		list<Statement>* stmts_var=new list<Statement>;//�洢�����ֵ����ʱ���ɵ����
		list<Statement>* stmts_exp=new list<Statement>;//�洢��ָ�ֵ���ʽʱ���ɵ����
		

		split_var(var,stmts_var);//�����ֵ����
		//�ж���ֵ�����Ƿ�Ϊ�������
		if(1==var->get_stamp())
		{
			//�ж��Ƿ�Ϊ������ֵ����
			if(exp->get_stamp()==5)
			{
				//���Ϊ������ֵ�����������ʵ�α��ʽ����
				split_args(exp->get_args(),stmts_exp);
			}
			else
			{
				split_exp_start(exp,stmts_exp);//��ָ�ֵ���ʽ
			}
			
			//�жϸ�ֵ���ʽ������ֺ��Ƿ�Ϊ�򵥱������߳���
			if(1==judge_exp_type(exp))
			{
				//������ǣ���������һ����ʱ�������
				Var *new_left_var=new Var(BuildTmpVarName(SplitArithmTmpVarPrefix,CurFuncNameHash,++*SplitArithmTmpVarCntPtr),::tmp_var_style);
				Statement *new_stmt=new Statement(new_left_var,new Expression(*exp));
				new_stmt->set_line_begin(line_num);
				new_stmt->set_fake_line(fake_line_num);
				stmts_exp->push_back(*new_stmt);//����ʱ������ֵ�������ֱ��ʽʽ���ɵ����֮��

				Expression *new_exp=new Expression(new_left_var);
				*exp=*new_exp;

				delete new_stmt;
			}
		}
		else
		{
			//�������ͨ���������ָ�ֵ���ʽ
			//�ж��Ƿ�Ϊ������ֵ����
			if(exp->get_stamp()==5)
			{
				//���Ϊ������ֵ�����������ʵ�α��ʽ����
				split_args(exp->get_args(),stmts_exp);
			}
			else
			{
				split_exp_start(exp,stmts_exp);//��ָ�ֵ���ʽ
			}
		}
	   
		//����������ɵ����������뵽ͨ���������ݵ����������
		l->insert(l->end(),stmts_var->begin(),stmts_var->end());
		l->insert(l->end(),stmts_exp->begin(),stmts_exp->end());
		delete stmts_var;
		delete stmts_exp;
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan assign statement : "+str;
		throw error_info;
	}
}

// ���ܣ�������������������е�ÿ������������ֺ������в��
// ���������s-�������ָ��
// ���������s-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void split_stmt_list(list<Statement>* s)
{
	try
	{
		//���������������ָ���Ƿ�Ϊ��
		check_pt_arg(s,"statement list's pointer");
		//����һ����ʱ��������洢��ÿ������ʱ���ɵ����
		list<Statement>* new_stmts=new list<Statement>;
		//�����������
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//���õ�ǰ����λ�ڵ��к�
			fake_line_num=iter->get_fake_line();
			split_stmt(&(*iter),new_stmts);//��ֵ�ǰ���
			//��������ɵ������뵽��ǰ�����������ǰ��
			s->insert(iter,new_stmts->begin(),new_stmts->end());
			new_stmts->clear();//�����ʱ�������
		}
		delete new_stmts;
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan the statement list : "+str;
		throw error_info;
	}
}

// ���ܣ�����������䣬�Է��صı��ʽ���в�֣���ֽ������ٴ��жϱ��ʽ�Ƿ���Ҫ��һ���Ĳ��
//			�����Ҫ����������һ����ʱ�������
// ���������rtstmt-����������ָ��
// ���������rtstmt-����������ָ�룬s-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void split_return_stmt(Return_stmt* rtstmt,list<Statement>* s)
{
	try
	{
		//������������������ָ���Ƿ�Ϊ��
		check_pt_arg(rtstmt,"return statement object's pointer");
		//���������������ָ���Ƿ�Ϊ��
		check_pt_arg(s,"statement list's pointer");
		
		//�жϷ�������Ƿ���з���ֵ
		if(1==rtstmt->get_stamp())
		{
			//����з���ֵ����������ر��ʽ
			//��ַ������ı��ʽ
			//������ʱ������ֵĹ��߲���
			set_tmpvar_split_utils_exp(rtstmt->get_ce());
			//�Է���ֵ���ʽ���в��
			Expression *RetExp=split_exp(rtstmt->get_ce(),s);
			//�޸ķ���ֵ���ʽ
			*rtstmt->get_ce()=*RetExp;

			delete RetExp;
		}
		else
		{
			//����޷���ֵ�������
		}	
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan return statement : "+str;
		throw error_info;
	}
}

// ���ܣ����ڱ���ѭ������ѭ������߷�֧����if��֧��else��֧����������Ҫ��ֵ������в�֣�
// 			������ʱ��������ɣ������ɵ����ͬԭ�е����ϲ������������һ���µĸ�����䣬
// 			���˸�������װ��һ����䷵�أ�������ʱ��������ɣ��򷵻�ΪNULL�����ָ��
// ���������s-��������ѭ����������if��֧������else��֧���
// ���������s-��������ѭ����������if��֧������else��֧���
// ����ֵ���ɹ��򷵻�������ָ�룬ʧ�����׳�������Ϣ
// ���ߣ����
// ע�⣺��������ʹ��newΪ���ص����������ڴ棬��Ҫ�ϲ㺯���ͷ�
static Statement* split_iter_select_body(Statement* s)
{
	try
	{
		//��������������ָ���Ƿ�Ϊ��
		check_pt_arg(s,"statement object's pointer");
		
		//�洢������Ҫ���ص�������ָ��
		Statement* new_stmt=NULL;
		
		//�洢���ʱ���ɵ����
		list<Statement>* stmt_list= new list<Statement>;
		//������
		split_stmt(s,stmt_list);
		//�ж������������
		if(1!=stmt_list->empty())
		{
			//����У��򽫲��ʱ���ɵ���䣬ͬԭ�������ϲ��������һ���µĸ������
			stmt_list->push_back(*s);//�ϲ��¾����
			list<Declaration>* dclr=new list<Declaration>;//����յ���������
			//���츴�����
			Compound_stmt* compd_stmt=new Compound_stmt(dclr,stmt_list);
			compd_stmt->set_line_begin(s->get_line_begin());
			compd_stmt->set_line_end(line_num);
			compd_stmt->set_fake_line(s->get_fake_line());
			new_stmt=new Statement(compd_stmt);
	
			delete dclr;
		}
		else
		{
			//�����������ɣ������
		}
		delete stmt_list;
		return new_stmt;
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan while or if or else body : "+str;
		throw error_info;
	}
}

// ���ܣ�����ѭ����䣬��ѭ�������з���������ѭ����ڵı��ʽ��ѭ���ڲ�����䣬�����Ҫ��֣�
//			�����ɲ�ֵ���䣬��ģ�鴫�ݸ��ϲ�ģ������Ϊ����ѭ����ڱ��ʽ���ʱ�����ɵ����
// ���������s-ѭ��������ָ��
// ���������s-ѭ��������ָ��
// 			      l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void split_iterate_stmt(Iteration_stmt* s,list<Statement>* l)
{
	try
	{
		//��������ѭ��������ָ���Ƿ�Ϊ��
		check_pt_arg(s,"iteration statement object's pointer");
		//���������������ָ���Ƿ�Ϊ��
		check_pt_arg(l,"statement list's pointer");
		
		while_deep_num++;//ѭ��Ƕ�ײ�����1
		if_while_deep++;//��֧ѭ��Ƕ�ײ�����1
		
		//tempcond_var_num=tmpconds->back();//ȡ���ϲ��������ʽ��ֺ�������ʱ�����Ѿ�ʹ�õĸ���
		
		list<Statement>* stmt1=new list<Statement>;//�洢����������ʽʱ���ɵ����
		
		list<Statement> *While_Sum_StmtLst=new list<Statement>; //�洢�㷨��ǩ���ۼӱ����ĳ�ʼ�����

		//AC_WhileA/B_index.C1/C2 ��������index �� 1
		WhileAC_AB_Cnt++;



		split_condition_exp_start(s->get_ce(),stmt1);	//����������ʽ
		
		//����ѭ������������Ӧ��AC_WHILE_A/B��������
		CFCondVar Iterator_CondVar;
		Iterator_CondVar.CondVar.FuncName=CurSplitFuncName;
		Iterator_CondVar.CondVar.VarName=s->get_ce()->get_cel()->get_cvar()->get_name();
		g_WhileMap_CondVar_SumVar.insert(make_pair(Iterator_CondVar,WhileAC_AB_Cnt));
		
		Statement* stmt_while=split_iter_select_body(s->get_cs());//����ѭ���壬������
		
		//�жϲ��ѭ���巵�ص����ָ���Ƿ�Ϊ��
		if(NULL!=stmt_while)
		{
			//�����Ϊ�գ���ѭ�����е�����޸�Ϊ��ַ��ص����
			s->set_cs(stmt_while);
		}
		else
		{
			//���Ϊ�գ������
		}
		
		//��ѭ���������������뵽ѭ���������
		list<Statement> *Iterator_StmtLst=NULL;
		if(s->get_cs()->get_stamp()==0)//�������
		{
			Iterator_StmtLst=s->get_cs()->get_ccstmt()->get_csl();
			Iterator_StmtLst->insert(Iterator_StmtLst->end(),stmt1->begin(),stmt1->end());
		}
		else
		{
			//�����������
			Iterator_StmtLst=new list<Statement>;
			Iterator_StmtLst->push_back(*(s->get_cs()));
			Iterator_StmtLst->insert(Iterator_StmtLst->end(),stmt1->begin(),stmt1->end());

			//����յ���������
			list<Declaration> *Iterator_declr=new list<Declaration>;

			//���츴�����
			Compound_stmt *IterComp=new Compound_stmt(Iterator_declr,Iterator_StmtLst);
			s->set_cs(new Statement(IterComp));
		}

		while_deep_num--;//ѭ��Ƕ�ײ�����1
		if_while_deep--;//��֧��ѭ��Ƕ�ײ�����1

		//���������������뵽����������ʽ���ɵ����������ǰ��
		stmt1->insert(stmt1->begin(),While_Sum_StmtLst->begin(),While_Sum_StmtLst->end());

		//�����ɵ����ͨ���������
		l->insert(l->end(),stmt1->begin(),stmt1->end());

		While_Sum_StmtLst->clear();
		stmt1->clear();
		delete While_Sum_StmtLst;
		delete stmt1;
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan iteration statement : "+str;
		throw error_info;
	}
}

// ���ܣ�������֧��䣬�Է�֧�����з�����������֧��ڵı��ʽ����֧�ṹ�ڲ�����䡣�����Ҫ
//			��֣������ɲ�ֵ���䣬��ģ�鴫�ݸ��ϲ�ģ������Ϊ���Է�֧��ڱ��ʽ���ʱ�����ɵ����
// ���������s-��֧������ָ��
// ���������s-��֧������ָ��
// 			      l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void split_select_stmt(Selection_stmt* s,list<Statement>* l)
{
	try
	{
		//����������֧������ָ���Ƿ�Ϊ��
		check_pt_arg(s,"select statement object's pointer");
		//���������������ָ���Ƿ�Ϊ��
		check_pt_arg(l,"statement list's pointer");
		
		if_while_deep++;//��֧ѭ��Ƕ�ײ�����1
		if_deep++;//��֧Ƕ�ײ�����1
			
		list<Statement>* stmt_exp = new list<Statement>;//�洢����������ʽʱ�����ɵ����
		
		split_condition_exp_start(s->get_ce(),stmt_exp);//����������ʽ
		
		//������������ʽ���ɵ����ͨ���������
		l->insert(l->end(),stmt_exp->begin(),stmt_exp->end());
		delete stmt_exp;
	
		Statement *stmt_if=split_iter_select_body(s->get_cs_if());//��ַ�֧�ṹ��if��֧���
		
		//�жϲ��if��֧���ص����ָ���Ƿ�Ϊ��
		if(NULL!=stmt_if)
		{
			//�����Ϊ�գ���if��֧�е�����޸�Ϊ��ַ��ص����
			s->set_cs_if(stmt_if);
			//delete stmt_if;
		}
		else
		{
			//���Ϊ�գ������
		}

		//�жϸ÷�֧�ṹ�Ƿ���else��֧
		if(1==s->get_stamp())
		{
			//�����else��֧�������else��֧
			Statement *stmt_else=split_iter_select_body(s->get_cs_else());//��else��֧���б��������
			
			//�жϲ��else��֧���ص����ָ���Ƿ�Ϊ��
			if(NULL!=stmt_else)
			{
				//�����Ϊ�գ���else��֧�е�����޸�Ϊ��ַ��ص����
				s->set_cs_else(stmt_else);
			}
			else
			{
				//���Ϊ�գ������
			}
		}
		else
		{
			//�������else��֧�������
		}
		
		if_while_deep--;//��֧ѭ��Ƕ�ײ�����1
		if_deep--;//��֧Ƕ�ײ�����1
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan select statement : "+str;
		throw error_info;
	}
}

// ���ܣ�������䣬�������Ĳ�ͬ���ͣ����ò�ͬ�ĺ����������в��
// ���������s-������ָ��
// ���������s-������ָ��
//			       l-�������ָ�룬���ڴ洢���ʱ���ɵ����
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void split_stmt(Statement* s,list<Statement>*l)
{
	try
	{
		//���������������ָ���Ƿ�Ϊ��
		check_pt_arg(l,"statement list's pointer");
		//��������������ָ���Ƿ�Ϊ��
		check_pt_arg(s,"statement object's pointer");
		//����ͨ��ʱ����������0
		temp_var_num=0;
		//�ж���������
		switch(s->get_stamp())
		{
			case -1:
			{
				//����䣬����
			}
			break;
			case 0:
			{
				//����Ǹ�����䣬�������������е��������
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
				//����Ƿ�����䣬�����������
				split_return_stmt(s->get_crs(),l);
				//���õ�ǰ���ʱ�õ�����ͨ��ʱ�������������ֵ
//				tempvar_num_max = temp_var_num>=tempvar_num_max ?
//				temp_var_num : tempvar_num_max;
			}
			break;
			case 4:
			{
				//����Ǹ�ֵ��䣬�������ֵ���
				set_tmpvar_split_utils_arithm(s->get_cvar());
				split_assign_stmt(s->get_cvar(),s->get_ce(),l);

//				���õ�ǰ���ʱ�õ�����ͨ��ʱ�������������ֵ
//				tempvar_num_max = temp_var_num>=tempvar_num_max ?
//				temp_var_num : tempvar_num_max;
			}
			break;
			case 5:
			{
				//����Ǳ��ʽ��䣬��������ʽ
				//�ж��Ƿ�Ϊ�����������
				if(s->get_ce()->get_stamp()==5)
				{
					string func_name = s->get_ce()->get_c();
					if("F_VCE_CheckArr" == func_name || "F_VCE_CheckVar" == func_name)
					{
						//У��ӿں�������Ҫ���в�������
						split_args_check(s->get_ce()->get_args(),l);
					}
					else
					{
						//���Ϊ�������ã����������ʵ�α��ʽ����
						split_args(s->get_ce()->get_args(),l);
					}
				}
				else
				{
					//������Ǻ������ã���˱��ʽ����������
					string error_info="expression statement is invalid!";
					throw error_info;
				}

				//���õ�ǰ���ʱ�õ�����ͨ��ʱ�������������ֵ
				tempvar_num_max = temp_var_num>=tempvar_num_max ? 
				temp_var_num : tempvar_num_max;
			}
			break;
			case 6:
			{
				//����Ƿֺ���䣬����
			}
			break;
			default:
			{
				//�����ǩΪ����ֵ����Ƿ����׳�������Ϣ
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan statement : "+str;
		throw error_info;
	}
}
