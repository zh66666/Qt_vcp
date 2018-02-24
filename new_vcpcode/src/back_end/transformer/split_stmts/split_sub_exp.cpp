/**
 * �����±���ģ��
 */

#include "CodedVarBuild_Util.h"
#include "expression.h"
#include "var.h"
#include "statement.h"

//�������ļ�: src/back_end/back_end_utils.c
extern string compute_exp(Expression* s,int* result);
//�������ļ���back_end/back_end_utils.cpp
extern int compute_exp(int left,int right,string op);

// �������ļ� : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern string int2str(int num);
extern int str2int(string name);
extern void check_pt_arg(void *pt,string info);

//�������ļ���./spit_utils.cpp
extern int temp_var_num;
extern int judge_exp_type(Expression* s);
extern void split_var(Var* s,list<Statement>* l);

/***************���ļ�����ĺ���**********************************/
extern vector<SPLIT_VAR> build_Var_list(Expression *s);
void order_sub_priority_op_exp(Expression *s,list<Expression*> &__PriorityExp,int __Priority);
int check_op_priority(Expression *s,int __Priority);

extern Expression *split_sub_exp(Expression* s,list<Statement>* l);
static bool is_all_constant_insub(Expression *s) throw(string);
static Expression *split_same_priority_op_subexp(Expression *s,list<Statement> *l);
Expression *merge_priority_op_sub_exp(Expression *s,list<Statement> *l,int __Priority);
SPLIT_VAR *add_subexp_operation(Expression *s,vector<SPLIT_VAR> &VarVect);

void order_sub_priority_op_exp(Expression *s,list<Expression*> &__PriorityExp,int __Priority)
{
	if(check_op_priority(s,__Priority))//����������ȼ�
	{
		__PriorityExp.push_back(s);
	}
	else
	{
		if(s->get_stamp()==0)
		{
			order_sub_priority_op_exp(s->get_cel(),__PriorityExp,__Priority);
			order_sub_priority_op_exp(s->get_cer(),__PriorityExp,__Priority);
		}
		else if(s->get_stamp()==1)
		{
			order_sub_priority_op_exp(s->get_cer(),__PriorityExp,__Priority);
		}
	}
}

int check_op_priority(Expression *s,int __Priority)
{
	try
	{
		string error_str;
		int flag=0;
		if(s->get_stamp()==4)//�жϱ��ʽ�Ƿ��Ǳ���
		{
			if(s->get_cvar()->get_stamp()==1 && __Priority==1)//�������
			{
				flag=1;
			}
		}
		else if(s->get_stamp()==0)//�жϱ��ʽ�Ƿ��Ǻ��������������ı��ʽ
		{
			string ExpOp=s->get_op();
			if((ExpOp=="*" || ExpOp=="/") && __Priority==3)
			{
				flag=1;
			}
			else if((ExpOp=="+" || ExpOp=="-") && __Priority==4)
			{
				flag=1;
			}
			else if((ExpOp=="<<" || ExpOp==">>") && __Priority==5)
			{
				flag=1;
			}
			else if(ExpOp=="&&" && __Priority==6)
			{
				flag=1;
			}
			else if(ExpOp=="||" && __Priority==7)
			{
				flag=1;
			}
			else
			{
				;
			}
		}
		else if(s->get_stamp()==1 && __Priority==2)//���ʽ�ǵ�Ŀ�����
		{
			flag=1;
		}
		else if(s->get_stamp()==3)//���ʽ�ǳ���
		{
			;
		}
		else
		{
			;
		}
		return flag;
	}
	catch(string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="split expression : "+str;
		throw error_info;
	}
}

/****************************�����±�����ģ��*********************************/

// ���ܣ�ר�����ڶ������±���ʽ���з�����������ʱ������ֵ��䣬��ͨ�����������ɵ�
//			�����䷵��
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ��
// 			      l-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
Expression *split_sub_exp(Expression* s,list<Statement>* l)
{
	Expression *RightVar=NULL;
	Expression *ExpResult=NULL;
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		//���������������ָ�����Ч��
		check_pt_arg(l,"statement list's pointer");
		//�жϱ��ʽ������
		int OpPriority=0;
		switch(s->get_stamp())
		{
			case 0:
			{
				//����������ʽ
				Expression *tmp_var=NULL;
				Expression *merge_var=NULL;

				//�ϲ�������ʽ
				for(OpPriority=2;OpPriority<=7;OpPriority++)
				{
					if((tmp_var=merge_priority_op_sub_exp(s,l,OpPriority))!=NULL)
						merge_var=tmp_var;
				}
				if(merge_var==NULL)
				{
					throw string("split error! merge priority op error!\n");
				}

				ExpResult=merge_var;

				return ExpResult;
			}
			break;
			case 3:
			{
				//���Ϊ�������ʽ
				ExpResult=new Expression(s->get_c());
				return ExpResult;
			}
			break;
			case 4:
			{
				Statement *new_stmt=NULL;
				Var *var_result=NULL;
				//���Ϊ�������ʽ��������ñ���
				ExpResult=new Expression(s->get_cvar());
				return ExpResult;
			}
			break;
			case 5:
			{
				//���Ϊ�������ñ��ʽ�����׳�������Ϣ
				string error_info="function call expression used invalid!";
				throw error_info;
			}
			break;
			default:
			{
				//�������͵ı��ʽ���Ƿ����׳�������Ϣ
				string error_info="invalid expression!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan expression : "+str;
		throw error_info;
	}
}

//���ȼ���1-����
//       2-��λȡ��  �߼���
//	   3-�˳�
//	   4-�Ӽ�
//	   5-��λ
//     6-�߼���
//     7-�߼���
//����ͳһ���ȼ������б��ʽ��������Щ���ʽ���в��
Expression *merge_priority_op_sub_exp(Expression *s,list<Statement> *l,int __Priority)
{
	list<Expression*> l_priority_exp;
	if(check_op_priority(s,__Priority))//����������ȼ�
	{
		l_priority_exp.push_back(s);
	}
	else
	{
		if(s->get_stamp()==0)
		{
			order_sub_priority_op_exp(s->get_cel(),l_priority_exp,__Priority);
			order_sub_priority_op_exp(s->get_cer(),l_priority_exp,__Priority);
		}
	}

	Expression *opResult=NULL;
	Expression *new_exp=NULL;
	Statement *new_statement=NULL;

	for(list<Expression*>::iterator iter=l_priority_exp.begin();iter!=l_priority_exp.end();iter++)
	{
		//ͬһ�������ȼ����㣬���й���
		opResult=split_same_priority_op_subexp(*iter,l);
		**iter=*opResult;
		delete opResult;
	}
	//���һ�����ȼ�����Ĺ鲢�����ع鲢��ı���
	Expression *RetExp=NULL;
	if(l_priority_exp.size()==1)
		RetExp=new Expression(**l_priority_exp.rbegin());

	l_priority_exp.clear();

	return RetExp;
}

//����ֵ�ɷ񱻺���
inline bool isCanBeIngoreInExp(int inVal, string inStrOp)
{
	return (inStrOp=="+" && inVal==0) || (inStrOp=="*" && inVal==1);
}

//���ͬһ��������ʽ
//flag_const:���ʽ�Ƿ�ȫ��������
static Expression *split_same_priority_op_subexp(Expression *s,list<Statement> *l)
{
	vector<SPLIT_VAR> VarVect;
	VarVect=build_Var_list(s);

	string strOp=VarVect[1].op_var_name;
	vector<string> vecConst;
	vector<string> vecVar;

	for(int i=0;i < VarVect.size();i++)
	{
		if(VarVect[i].stamp==4)
		{
			vecVar.push_back(VarVect[i].op_var_name);
		}
		else if(VarVect[i].stamp==3)
		{
			vecConst.push_back(VarVect[i].op_var_name);
		}
	}

	int iConstReslut=-1;
	if(strOp=="*")
		iConstReslut=1;
	else if(strOp=="+")
		iConstReslut=0;

	for(int i=0;i < vecConst.size();i++)
	{
		iConstReslut=compute_exp(iConstReslut,str2int(vecConst[i]),strOp);
	}

	Expression *LastExp=NULL;
	Expression *CurRightVar=NULL;
	Var *LeftVar=NULL;
	Expression *NewArithmExp=NULL;
	Statement *NewArithmState=NULL;

	if(vecVar.empty())
	{
		LastExp=new Expression(int2str(iConstReslut));
	}
	else
	{
		LastExp=new Expression(new Var(vecVar[0],::tmp_var_style));
		if(!isCanBeIngoreInExp(iConstReslut,strOp))
			vecVar.push_back(int2str(iConstReslut));
	}

	for(int i=1;i < vecVar.size();i++)
	{
		if(i < vecVar.size()-1)
			CurRightVar=new Expression(new Var(vecVar[i],::tmp_var_style));
		else
			CurRightVar=new Expression(vecVar[i]);

		NewArithmExp=new Expression(LastExp,strOp,CurRightVar);
		LeftVar=new Var(BuildTmpVarName(SplitArithmTmpVarPrefix,CurFuncNameHash,++*SplitArithmTmpVarCntPtr),::tmp_var_style);
		NewArithmState=new Statement(LeftVar,NewArithmExp);
		NewArithmState->set_fake_line(fake_line_num);
		NewArithmState->set_line_begin(line_num);
		LastExp=new Expression(LeftVar);

		l->push_back(*NewArithmState);
	}

	return LastExp;
}



//�ж��±���ʽ�Ƿ�ȫ��Ϊ������
static bool is_all_constant_insub(Expression *s) throw(string)
{
	if(s->get_stamp()==3)
		return true;
	else if(s->get_stamp()==4)
		return false;
	else if(s->get_stamp()==0)
	{
		return is_all_constant_insub(s->get_cel()) && is_all_constant_insub(s->get_cer());
	}
	else
	{
		throw string("invalid expression in array sub expression!\n");
	}
}

vector<SPLIT_VAR> build_Var_list(Expression *s)
{
	vector<SPLIT_VAR> VarVect;
	SPLIT_VAR OpSym;

	SPLIT_VAR *leftVarName=add_subexp_operation(s->get_cel(),VarVect);
	if(leftVarName!=NULL)
	{
		VarVect.push_back(*leftVarName);
		delete leftVarName;
	}

	OpSym.op_var_name=s->get_op();
	OpSym.stamp=10;
	VarVect.push_back(OpSym);

	SPLIT_VAR *rightVarName=add_subexp_operation(s->get_cer(),VarVect);
	if(rightVarName!=NULL)
	{
		VarVect.push_back(*rightVarName);
		delete rightVarName;
	}

	return VarVect;
}

//��ӱ��ʽ�����еĲ������Ͳ������������������
//����:a+b+c�������ֳ�'a' '+' 'b' '+' 'c'���������ַ��������������ַ���
SPLIT_VAR *add_subexp_operation(Expression *s,vector<SPLIT_VAR> &VarVect)
{
	SPLIT_VAR *retn_split_var=new SPLIT_VAR;
	SPLIT_VAR OpSym;
	if(s->get_stamp()==4)
	{
		retn_split_var->op_var_name=s->get_cvar()->get_name();
		retn_split_var->stamp=4;
		return retn_split_var;
	}
	else if(s->get_stamp()==3)
	{
		retn_split_var->op_var_name=s->get_c();
		retn_split_var->stamp=3;
		return retn_split_var;
	}
	else if(s->get_stamp()==0)
	{
		SPLIT_VAR *leftVarName=add_subexp_operation(s->get_cel(),VarVect);
		if(leftVarName!=NULL)
		{
			VarVect.push_back(*leftVarName);
			delete leftVarName;
		}

		OpSym.op_var_name=s->get_op();
		OpSym.stamp=10;
		VarVect.push_back(OpSym);

		SPLIT_VAR *rightVarName=add_subexp_operation(s->get_cer(),VarVect);
		if(rightVarName!=NULL)
		{
			VarVect.push_back(*rightVarName);
			delete rightVarName;
		}

		return NULL;
	}
	else
	{
		perror("Invailed Expression when split exp!\nNot expect exp like op expr!");
	}
}
