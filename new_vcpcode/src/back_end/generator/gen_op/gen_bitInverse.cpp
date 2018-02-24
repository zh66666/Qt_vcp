
#include "gen_utils.h"
#include "expression.h"
#include "bool_var.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// �������ļ�: src/main_utils.cpp
extern bool debug_flag;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;

//�������ļ�: ../gen_utils.c
extern int get_bool_value(string name,list<Bool_var>* bool_vars,int stamp);
extern int get_bool_sig(string name,list<Bool_var>* bool_sigs,int stamp);

//�������ļ�: ./inverse.c
extern long long inverse(long long, const long long);

// ���ܣ����߼���������б��룬�������ĸ�λ����͵�λ���룬ѭ������ѭ�������ͬ���ı��뷽��
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_bit_inverse(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");//����������ֵ���������ַ�������Ч��
		check_pt_arg(exp,"expression object's pointer");//����������ֵ���ʽ����ָ�����Ч��
		check_pt_arg(v,"var_info list's pointer");//����������̬ǩ����ָ�����Ч��

		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		//�ж��Ҳ������Ƿ�Ϊ����
		if(4==right)
		{
			//����ǣ�����б���
			int SigID=0;
			int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//int new_sig=get_sig();//Ϊ��ֵ������ǩ��
			int const_sig=getConstantSig(-1);//Ϊ����-1������������ǩ��
			string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������
			int right_sig=get_current_sig(v,get_global_string_sig(r_name,var_scope),var_scope);//ȡ���Ҳ�����������ǩ��
			int constant1=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
								                 %(long long)P1+(long long)P1)%(long long)P1;
			int constant2=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
								                 %(long long)P2+(long long)P2)%(long long)P2;

			add_new_sig(v,var_name,new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����

			out_indent(indent_num, gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string r_name_head=TmpVarJudge(r_name)?r_name:get_global_string(r_name,var_scope);
			gen_out << var_head << ".Data = " << "F_VCL_BitInverse("<<r_name_head
				<<".Data, " << int2HexStr(constant1)
					<< ", " << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<"-1"
							<<"'s sig :"<<const_sig<<" "<<r_name<<"'s sig :"<<const_sig<<endl<<endl;
			}
		}

		else if(3==right)
		{
			int SigID=0;
			int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//int new_sig=get_sig();//Ϊ��ֵ������ǩ��
			string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������
			int right_sig=getConstantSig(str2int(r_name));////ȡ���Ҳ�����������ǩ��
			int const_sig=getConstantSig(-1);//Ϊ����-1������������ǩ��

			int constant1=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
	                 %(long long)P1+(long long)P1)%(long long)P1;
			int constant2=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
	                 %(long long)P2+(long long)P2)%(long long)P2;
			add_new_sig(v,var_name,new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
			//����������ʼ��
			constant2Str(str2int(r_name));
			out_indent(indent_num, gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitInverse(AC_BIND_CONST_NUM_"
					<< constantBindedStr(str2int(r_name))<<".Data, " << int2HexStr(constant1)
					<< "," << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<"-1"
							<<"'s sig :"<<const_sig<<" "<<r_name<<"'s sig :"<<const_sig<<endl<<endl;
			}
		}
		else
		{
			//������Ϊ�������ʱ���׳�������Ϣ
			string error_info="operand is invalid!";
			throw error_info;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding bitinverse operation : "+str;
		throw error_info;
	}
}


