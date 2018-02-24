#include "CodedVarBuild_Util.h"
#include "expression.h"
#include "var.h"
#include "statement.h"


/*************2014-12����޷���ֵ����ǿ�Ʒ��ص�ֵ**************************/
const int VOID_RET_VALUE = 0;
/**************************************************************/

int if_while_deep=0;//��¼��ǰ�м�ṹ�з�֧��ѭ��Ƕ�׵ĵĲ���
int while_deep_num=0;//��¼��ǰ�м�ṹ��ѭ��Ƕ�׵Ĳ���
int if_deep=0;//��¼��ǰ�м�ṹ�з�֧Ƕ�׵Ĳ���
int temp_var_num=0;//��¼�Ե�ǰ�м�ṹ���в��ʱ�õ�����ͨ��ʱ�����ĸ���
int tempcond_var_num=0;//��¼�Ե�ǰ�м�ṹ���в��ʱ�õ���������ʱ�����ĸ���

vector<int>* tmpconds=new vector<int>;//��¼��ÿ���������ʽ��ֺ�������ʱ�����ĸ���


//�������ļ� ��./split_exp.cpp
extern void split_exp_start(Expression* s,list<Statement>* l);
extern Expression *split_exp(Expression* s,list<Statement>* l);
extern Expression *split_sub_exp(Expression* s,list<Statement>* l);
extern void modify_sub_exp_II(Expression* s);
extern void modify_sub_exp_I(Expression* s);
//�������ļ� ��src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern void check_pt_arg(void *pt,string info);
extern string int2str(int num);
extern int str2int(string name);

/*****2014-12����****************/
list<string> tmp_var_style;//��ʱ��������

string CurFuncNameHash;//�ɺ�����ϣ�õ�����ʱ����ǰ׺

string CurSplitFuncName;//��ǰ������ʱ�����ں�����

string SplitArithmTmpVarPrefix;//��ֱ��ʽʱ����ʱ����ǰ׺���Ȱ�����������(��������)��ʱ������Ҳ�����߼�������ʱ���������������������ʽ����ʱ����
string SplitCondTmpVarPrefix;//��ֱ��ʽʱ���������ʽ���ʱ��ʱ����ǰ׺
string CondSubVarPrefix;//�߼�ֵ��ȥTRUE��ǰ׺

int IfTmpBoolCnt;//��ǰ�����·�֧��������������ʽ���߼�����Ľ��
int IfBoolSubCnt;//��֧����ȥTRUEֵ�Ĳ�ֵ�ļ�����

int WhileTmpBoolCnt;//��ǰ������ѭ����������������ʽ���߼�����Ľ��
int WhileBoolSubCnt;//ѭ������ȥTRUEֵ�Ĳ�ֵ�ļ�����

int TmpRelationBoolCnt;//��ǰ�����·�֧��������жϱ��ʽ�У���ϵ����Ĳ���ֵ���������

int *SplitCondTmpVarCntPtr;//�������ʽ��ʱ��������������ָ��
int *CondSubVarCntPtr;//�߼�ֵ��ȥTRUE�ļ�����ָ��

int WhileAC_AB_Cnt;//While����ۼ�������������

int *SplitArithmTmpVarCntPtr;//��������(��������)���߼�������ʽ��ʱ����������ָ��
int ACIntTmpCnt;//����������ʽ������ �з�����
int ACUIntTmpCnt;//����������ʽ������ �޷�����

int void_func_counts;//�޷���ֵ����ǿ�Ʒ��ص�����ֵ


// ���ܣ��жϱ��ʽ�����ͣ�����ñ��ʽΪ��ͨ�������ʽ���߳������ʽ���򷵻�0��������������
// 			������1
// ���������s-���ʽ����ָ��
// ����ֵ���ɹ��򷵻�int���͵�ֵ��ʧ�����׳�������Ϣ
// ���ߣ����
int  judge_exp_type(Expression* s)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		
		int stamp = s->get_stamp();//ȡ���ñ��ʽ�ı�ǩֵ
		// �жϱ��ʽ�Ƿ�Ϊ��ͨ�������ʽ�������ǳ������ʽ
		if((4==stamp && 0==s->get_cvar()->get_stamp() )||  3==stamp)
		{
			//����ǣ��򷵻�0
			return 0;
		}
		else
		{
			//������ǣ��򷵻�1
			return 1;
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="judge expression's type : "+str;
		throw error_info;
	}
}

// ���ܣ����ݴ���ı��ʽ������һ���µ���ʱ������ֵ��䣬�����أ�ͬʱ������ı��ʽ
//			�޸�Ϊ��ʱ�������ʽ
// ���������exp-���ʽ����ָ��
// 			      str-��ʱ��������
// 			      num-��ǰ��ʱ����ʹ�õĸ���
// ����ֵ���ɹ��򷵻�������ָ�룬ʧ�����׳�������Ϣ
// ���ߣ����
// ע�⣺��������ʹ��newΪ���������ڴ棬��Ҫ�ϲ㺯�����ͷ�
Statement* generate_newstmt(Expression* exp,string str,int* num)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(exp,"expression object's pointer");
		
		Var* new_var=new Var(str+"_"+int2str(*num),::tmp_var_style);//������ʱ��������
		Expression *temp_exp=new Expression(*exp);//��������ı��ʽ����
		Statement* new_stmt=new Statement(new_var,temp_exp);//������ʱ�������
		Expression *new_exp=new Expression(new_var);//ʹ����ʱ���������±��ʽ
		*exp=*new_exp;//������ı��ʽ���ó���ʱ�������ʽ
		
		delete new_exp;
		return new_stmt;
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="generate a new statement : "+str;
		throw error_info;
	}
}

// ���ܣ���ֱ�������������������������������������±���ʽ�������±���в��
// ���������s-��������ָ��
// ���������s-��������ָ�룬l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void split_var(Var* s,list<Statement>* l)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s,"variable object's pointer");
		//���������������ָ�����Ч��
		check_pt_arg(l,"statement list's pointer");
		//�жϸñ����Ƿ�Ϊ�������
		if(1==s->get_stamp())//array
		{
			//modify_sub_exp_I(s->get_cexp());
			//modify_sub_exp_II(s->get_cexp());
			//��������������������±���ʽ
			Expression *array_sub=split_sub_exp(s->get_cexp(),l);//����±���ʽ
			
			*s->get_cexp()=*array_sub;
			delete array_sub;
		}
		else
		{
			//���������������������
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable : "+str;
		throw error_info;
	}
}

// ���ܣ��Ժ������ñ��ʽ�е�ʵ�α��ʽ������б�������ÿ��ʵ�α��ʽ���в��
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ�룬l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void split_args(list<Expression>* s,list<Statement>* l)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression list's pointer");
		//���������������ָ�����Ч��
		check_pt_arg(l,"statement list's pointer");
		
		//�洢ÿ�����ʽ����������ɵ����
		list<Statement>* new_stmts= new list<Statement>;
		//�������ʽ����
		for(list<Expression>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			//������ʱ������ֵĹ��߲���
			set_tmpvar_split_utils_exp(&(*iter));
			//�Ե�ǰ��������ָ�ı��ʽ���в��
			Expression *ArgsExp=split_exp(&(*iter),new_stmts);
			//�޸ĺ���ʵ�α��ʽ
			*iter=*ArgsExp;

			//��������ɵ������뵽ͨ��������������������
			l->insert(l->end(),new_stmts->begin(),new_stmts->end());
			new_stmts->clear();

			delete ArgsExp;
		}
		delete new_stmts;
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan function's arguments : "+str;
		throw error_info;
	}
}

// ���ܣ��Ժ������ñ��ʽ�е�ʵ�α��ʽ������б�������ÿ��ʵ�α��ʽ���в��
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ�룬l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
void split_args_check(list<Expression>* s,list<Statement>* l)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression list's pointer");
		//���������������ָ�����Ч��
		check_pt_arg(l,"statement list's pointer");

		//�洢ÿ�����ʽ����������ɵ����
		list<Statement>* new_stmts= new list<Statement>;
		//�������ʽ����
		for(list<Expression>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			//������ʱ������ֵĹ��߲���
			set_tmpvar_split_utils_exp(&(*iter));
			//�Ե�ǰ��������ָ�ı��ʽ���в��
			split_exp(&(*iter),new_stmts);

			//��������ɵ������뵽ͨ��������������������
			l->insert(l->end(),new_stmts->begin(),new_stmts->end());
			new_stmts->clear();
		}
		delete new_stmts;
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan function's arguments : "+str;
		throw error_info;
	}
}
/*
 * ��ʼ����֧�ṹ�е��ж���������������
 * ��ʼ����ʱ�������±�����
 *
 */
void InitSplitTempVarCnt()
{
	ACIntTmpCnt=0;
	ACUIntTmpCnt=0;

	IfTmpBoolCnt=0;
	IfBoolSubCnt=0;

	WhileTmpBoolCnt=0;
	WhileBoolSubCnt=0;

	TmpRelationBoolCnt=0;

	WhileAC_AB_Cnt=0;

	void_func_counts = 0;//���޸�ֵ�����������ļ���������
}

/*
 * ��ʱ���ӱ�����������
 * ���������ǰ׺+������ϣ�ַ���+������
 */
string BuildTmpVarName(string __Prefix,string _FuncHash,int VarStyleCnt)
{
	string VarName=__Prefix+int2str(VarStyleCnt);
	return VarName;
}

//��ȡ���ʽ�����Ƿ�Ϊ�з��� true-�з��� false-�޷���
bool get_tmpvar_style(Var *var)
{
	::tmp_var_style=var->get_var_type();

	list<string>::iterator iter=::tmp_var_style.begin();
	for(;iter!=::tmp_var_style.end();iter++)
	{
		if(*iter=="unsigned" || *iter=="UINT32")
		{
			break;
		}
	}
	if(iter!=::tmp_var_style.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

//��ȡ����ʵ�α��ʽ�Ƿ�Ϊ�з��� true-�з��� false-�޷���
bool get_args_style(Expression *s)
{
	int final_bool=false;
	if(s->get_stamp()==4)
		final_bool=get_tmpvar_style(s->get_cvar());
	else if(s->get_stamp()==2)
		final_bool=get_args_style(s->get_exp());
	else if(s->get_stamp()==1)
		final_bool=get_args_style(s->get_cer());
	else if(s->get_stamp()==0)
	{
		int left_bool=get_args_style(s->get_cel());
		int right_bool=get_args_style(s->get_cer());

		final_bool=left_bool && right_bool;
	}
	else
	{
		final_bool=true;
	}

	return final_bool;
}

//���ʽ���(��������ֵ���ʽ)ʱ��ʱ������������
//�������ͣ�����ʵ�Ρ���������ֵ
void set_tmpvar_split_utils_exp(Expression *s)
{
	if(get_args_style(s))
	{
		SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
	}
	else
	{
		SplitArithmTmpVarCntPtr=&::ACUIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_UINTTMP_PRE;
	}
}

//����������ʱ��ʱ������������
void set_tmpvar_split_utils_arithm(Var *var)
{
	if(get_tmpvar_style(var))
	{
		SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
	}
	else
	{
		SplitArithmTmpVarCntPtr=&::ACUIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_UINTTMP_PRE;
	}
}

//��֧�������ʽ���ʱ��ʱ������������
void set_tmpvar_split_utils_select()
{
	CondSubVarPrefix=IF_LOGICSUB_PRE;
	CondSubVarCntPtr=&IfBoolSubCnt;

	SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
	SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
}

//ѭ���������ʽ���ʱ��ʱ��������
void set_tmpvar_split_utils_while()
{
	CondSubVarPrefix=WHILE_LOGICSUB_PRE;
	CondSubVarCntPtr=&WhileBoolSubCnt;

	SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
	SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
}





