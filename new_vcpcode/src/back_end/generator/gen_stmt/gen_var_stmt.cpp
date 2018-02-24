#include "expression.h"
#include "var.h"
#include "gen_utils.h"
#include "CodedVarBuild_Util.h"

class Var_info;

// �������ļ� : src/main_utils.cpp
extern int str2int(string);
extern void check_str_arg(string arg_name, string info);
extern void check_pt_arg(void *pt, string info);

// �������ļ� : ./gen_array_stmt.cpp
extern void gen_array_refer(string, Var*, list<Var_info>*);

// �������ļ� : ./gen_fun_stmt.cpp
extern void gen_fun_assign(string, Expression*, list<Var_info>*);

// ���º����������ļ���: ../../gen_op
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
extern void red_bit_inverse(string, Expression*, list<Var_info>*); //���

#define IF_SIG_TEST 0 //if�������ǩ����ȷ�Բ���
static int check_relationCmp_op(Expression *s);

// ���ܣ��Ա�����ֵ�����б��롣���ݸ�ֵ���ʽ�Ĳ�ͬ���ͣ����ò�ͬ���²���뺯���������������
// ���������var_name-��ֵ������ֵ������
// 			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_var_assign(string var_name, Expression* exp, list<Var_info>* v)
{
	try
	{
		check_str_arg(var_name, "left variable's name"); //����������ֵ����������Ч��
		check_pt_arg(exp, "expression object's pointer"); //�����������ʽָ�����Ч��
		check_pt_arg(v, "var_info list's pointer"); //����������̬ǩ����ָ�����Ч��

		//�жϸ�ֵ���ʽ������
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

				add_new_sig(v,var_name,new_sig,var_scope); //����ֵ������ǩ�����붯̬ǩ����
			}
#else
			//����Ǵ��������������ı��ʽ�����ݲ��������͵��ò�ͬ�ı��뺯��
			//�ж��Ƿ�Ϊ�ӷ�����
			if (exp->get_op() == "+")
			{
				//����ǣ�����öԼӷ�����ı��뺯��
				red_add(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ��������
			else if (exp->get_op() == "-")
			{
				//����ǣ�����öԼ�������ı��뺯��
				red_sub(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ�˷�����
			else if (exp->get_op() == "*")
			{
				//����ǣ�����öԳ˷�����ı��뺯��
				red_mul(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ��������
			else if (exp->get_op() == "/")
			{
				//����ǣ�����öԳ�������ı��뺯��
				red_div(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ�߼�������
			else if (exp->get_op() == "&&")
			{
				//����ǣ�����ö��߼�������ı��뺯��
				red_and(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ�߼�������
			else if (exp->get_op() == "||")
			{
				//����ǣ�����ö��߼�������ı��뺯��
				red_or(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ��������
			else if (exp->get_op() == "<<")
			{
				//����ǣ�����ö���������ı��뺯��
				red_shift_l(var_name, exp, v);
			}
			//�ж��Ƿ�Ϊ��������
			else if (exp->get_op() == ">>")
			{
				//����ǣ�����ö���������ı��뺯��
				red_shift_r(var_name, exp, v);
			}
			//�ж��Ƿ��ǹ�ϵ�Ƚ�����(ֻ�����ڷ�֧��ѭ��ת����Ĵ�����)
			else if (check_relationCmp_op(exp) == 1)
			{
				Red_RelationCmp(var_name, exp, v);
			}
			else
			{
				//���Ϊ����δ֪����������׳�������Ϣ
				string error_info = "invalid operator!";
				throw error_info;
			}
#endif
		}
			break;

		case 1:
		{
			//���Ϊ�����������ʽ�����жϲ������Ƿ�Ϊ�߼�������
			if (exp->get_op() == "!")
			{
				//����ǣ�����ö��߼�������ı��뺯��
				red_not(var_name, exp, v);
			}
			else if (exp->get_op() == "~")
			{
				red_bit_inverse(var_name, exp, v);
			}
			else
			{
				//��������������������׳�������Ϣ
				string error_info = "invalid operator!";
				throw error_info;
			}
		}
			break;

		case 2:
		{
			//��������ű��ʽ�����׳�������Ϣ
			string error_info = "the ( ) is unexpected! ";
			throw error_info;
		}
			break;

		case 3:
		{
#if IF_SIG_TEST
			//�����һ���������ʽ
			string new_var_name;
			string constant_name=exp->get_c();//ȡ���ó����ַ���

			new_var_name=TmpVarJudge(var_name)==true? var_name:"TC_"+var_name;

			int new_sig=get_sig_index(NULL);
			out_indent(indent_num,gen_out);
			string Func_FieldStr=constant_name;
			//gen_out<<new_var_name+VAR_TDATA+"="+"GetCodedData("+Func_FieldStr+","+int2str(new_sig)+");"<<endl;
			gen_out<<new_var_name<<"_1="<<constant_name<<"*A1;"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<new_var_name<<"_2="<<constant_name<<"*A2;"<<endl;

			add_new_sig(v,var_name,new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
#else
			//�����һ���������ʽ
			string constant_name = exp->get_c();				//ȡ���ó����ַ���
			int high = str2int(constant_name);				//����ת���ɶ�Ӧ�ĳ���
			red_assign_const(var_name, high, v);				//���ó�����ֵ���뺯��
#endif
		}
			break;

		case 4:
		{
			//����Ǳ������ʽ�����жϱ������Ƿ�Ϊ��ͨ����
			if (0 == exp->get_cvar()->get_stamp())
			{
#if IF_SIG_TEST
				//�������ͨ����������ñ�����ֵ���뺯��
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

				add_new_sig(v,var_name,new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
#else
				//�������ͨ����������ñ�����ֵ���뺯��
				string name_tmp = exp->get_cvar()->get_name();
				red_assign_var(var_name, name_tmp, v);
#endif
			}
			else
			{
				//��������������������������ñ��뺯��
				gen_array_refer(var_name, exp->get_cvar(), v);
			}
		}
			break;

		case 5:
		{
			//����Ǻ������ñ��ʽ������ú�����ֵ���뺯��
			gen_fun_assign(var_name, exp, v);
		}
			break;

		default:
		{
			//������������ͣ����׳�������Ϣ
			string error_info = "expression object's stamp is invalid!";
			throw error_info;
		}
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding variable assignment : " + str;
		throw error_info;
	}
}

static int check_relationCmp_op(Expression *s)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s, "expression object's pointer");

		//�жϲ������Ƿ�Ϊ�Ƚ����������
		if ((s->get_op() == ">") || (s->get_op() == ">=")
				|| (s->get_op() == "<") || (s->get_op() == "<=")
				|| (s->get_op() == "==") || (s->get_op() == "!="))
		{
			//����ǣ�����1
			return 1;
		}
		else
		{
			//�����������򷵻�0
			return 0;
		}
	} catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "check expression's operation : " + str;
		throw error_info;
	}
}

