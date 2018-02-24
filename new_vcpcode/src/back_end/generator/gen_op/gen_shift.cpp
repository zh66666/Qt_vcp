
#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include <cmath>
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// �������ļ�: src/main_utils.cpp
extern bool debug_flag;

//�������ļ� : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);
//�������ļ� : ./inverse.c
extern long long inverse(long long b,const long long prime);

// ���ܣ�������������б��룬�����������ĸ�λ����͵�λ���룬ע�⣬ֻ�ܶ�λ��Ϊ������
//			��λ������б��룬ͬʱ����ѭ�����⣬���ò�ͬ�ı��뷽����
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_shift_l(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
		if((4==left)&&(3==right))
		{
			//����ǣ�����б���
			string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ�������������������
			string r_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//ȡ�����������ǩ��
			int high=str2int(r_name);//���Ҳ������ַ���ת��Ϊ����

			int const_sig=getConstantSig(high);//Ϊ����������������ǩ��


			//int const_sig=get_sig();//Ϊ��������ǩ��
			int m=(int)pow(2.0,(double)high);//��������ת��������	
			int new_sig=0;//�洢��ֵ��������ǩ��
			int constant1=0;//�洢��λ�����еĲ�������
			int constant2=0;//�洢��λ�����еĲ�������
			string str_const="";//�洢��λ�����в��������ַ���
			int SigID=0;
			new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*(long long)m)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*(long long)m)%P2)%P2+P2)%P2;
			//str_const=" + "+int2str(constant);//���ò��������ַ���������
			
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����

		/*	//�����������ĸ�λ����
			
			string str_divided="H_"+l_name;//���Ʊ������ֵĸ�λ�ַ���
			//�жϱ��Ʊ������Ƿ�ͬ��ֵ��������ͬ
			if(var_name==l_name)
			{
				//�����ͬ�������һ����ʱ�����滻���Ʊ����Ĵ���
				out_indent(indent_num,gen_out);
				gen_out<<red_Tmp1_INT32<<" = H_"<<l_name<<";"<<endl;
				//�����Ʊ����ĸ�λ�����޸�Ϊ��ʱ������
				str_divided=red_Tmp1_INT32;
			}
			else
			{
				//�������ͬ������
			}*/
			//����������ʼ��
//			constant2Str(high);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitShiftL("<<l_name_head
								 << ".Data, " << int2HexStr(high) << ", "
								<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<l_name
				<<"'s sig :"<<left_sig<<endl<<endl;
			}
			//gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" << "<<r_name<<";"<<endl;
			//�����������ĵ�λ����
		/*	out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")(("<<str_int64<<")("
			<<str_divided<<" >> "<<32-high<<") + (("<<str_divided<<" >> "<<31-high<<
			") & 1)) * "<<((long long)RK*RK)%A<<";"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD(red_Tmp1_INT32, red_Tmp_INT64);
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")((("<<str_int64<<")L_"
			<<l_name<<" - DT) << "<<r_name<<") + "<<red_Tmp1_INT32<<str_const
			<<" + DT;"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD("L_"+var_name, red_Tmp_INT64);*/
		}
		//�ж����Ҳ������Ƿ��Ϊ����	
		else if((3==left)&&(3==right))
		{
			//����������ͳ�����Ϊ��������ֱ�ӽ���������
			string left_name=exp->get_cel()->get_c();//ȡ���������������
			string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			//int high=str2int(left_name)<<str2int(right_name);//�����λ�����Ľ��
			//red_assign_const(var_name,high,v);//���ó�����ֵ���뺯�����б���
			int left_sig=getConstantSig(str2int(left_name));//ȡ�����������ǩ��
			int high=str2int(right_name);//���Ҳ������ַ���ת��Ϊ����

			int const_sig=getConstantSig(high);//Ϊ����������������ǩ��


			//int const_sig=get_sig();//Ϊ��������ǩ��
			int m=(int)pow(2.0,(double)high);//��������ת��������
			int new_sig=0;//�洢��ֵ��������ǩ��
			int constant1=0;//�洢��λ�����еĲ�������
			int constant2=0;//�洢��λ�����еĲ�������
			string str_const="";//�洢��λ�����в��������ַ���
			int SigID=0;
			new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*(long long)m)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*(long long)m)%P2)%P2+P2)%P2;

			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
			//����������ʼ��
			constant2Str(str2int(left_name));
			//����������ʼ��
//			constant2Str(high);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitShiftL(AC_BIND_CONST_NUM_"
								<< constantBindedStr(str2int(left_name)) << ".Data, " << int2HexStr(high) << ", "
								<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_name
				<<"'s sig :"<<left_sig<<endl<<endl;
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
		string error_info="coding shift left operation : "+str;
		throw error_info;
	}
}

// ���ܣ�������������б��룬�����������ĸ�λ����͵�λ���룬ע�⣬ֻ�ܶ�λ��Ϊ������
//			��λ������б��룬ͬʱ����ѭ�����⣬���ò�ͬ�ı��뷽����
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_shift_r(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		
		//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
		if((4==left)&&(3==right))
		{
			//����ǣ�����б���
			string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ�������������������
			string r_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//ȡ�����������ǩ��
			int high=str2int(r_name);//���Ҳ������ַ���ת��Ϊ����

			int const_sig=getConstantSig(high);//Ϊ����������������ǩ

			//int const_sig=get_sig();//Ϊ��������������ǩ��
			int m=(int)pow(2.0,(double)high);
			long long tmp1=inverse(m,(long long)P1);//�������е���
			long long tmp2=inverse(m,(long long)P2);//�������е���
			
			int new_sig=0;//�洢��ֵ��������ǩ��
			int constant1=0;//�洢��λ�����еĲ�������
			int constant2=0;//�洢��λ�����еĲ�������
			string str_const="";//�洢��λ�����в��������ַ���
			int SigID=0;
			new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*tmp1)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*tmp2)%P2)%P2+P2)%P2;
			//str_const=" + "+int2str(constant);//���ò��������ַ���������
			
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����

			//�����������ĸ�λ����

			/*string str_divided="H_"+l_name;//���Ʊ������ֵĸ�λ�ַ���
			//�жϱ��Ʊ������Ƿ�ͬ��ֵ��������ͬ
			if(var_name==l_name)
			{
				//�����ͬ�������һ����ʱ�����滻���Ʊ����Ĵ���
				out_indent(indent_num,gen_out);
				gen_out<<red_Tmp1_INT32<<" = H_"<<l_name<<";"<<endl;
				//�����Ʊ����ĸ�λ�����޸�Ϊ��ʱ������
				str_divided=red_Tmp1_INT32;
			}
			else
			{
				//�������ͬ������
			}*/
			//����������ʼ��
//			constant2Str(high);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			out_indent(indent_num,gen_out);
			gen_out << var_head << ".Data = " << "F_VCL_BitShiftR("<<l_name_head
					 << ".Data, " << int2HexStr(high) << ", "
					<< int2HexStr(tmp1) << ", "<< int2HexStr(tmp2) << ", "
					<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<l_name
				<<"'s sig :"<<left_sig<<endl<<endl;
			}

			/*gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" >> "<<r_name<<";"<<endl;

			//�����������ĵ�λ����
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<RK<<" * ("<<str_divided
			<<" & "<<m-1<<");"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD(red_Tmp1_INT32, red_Tmp_INT64);
			out_indent(indent_num,gen_out);
			red_Add_VVVV(red_Tmp1_INT32, "L_"+l_name, red_Tmp1_INT32, "DT");
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<red_Tmp1_INT32<<
			" * "<<tmp<<str_const<<" + DT;"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD("L_"+var_name, red_Tmp_INT64);*/
		}
		//�ж����Ҳ������Ƿ��Ϊ����
		else if((3==left)&&(3==right))//if the operands are both constants
		{
			//����������ͳ�����Ϊ��������ֱ�ӽ���������
			string left_name=exp->get_cel()->get_c();//ȡ���������������
			string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			int left_sig=getConstantSig(str2int(left_name));//ȡ�����������ǩ��
			int high=str2int(right_name);//���Ҳ������ַ���ת��Ϊ����

			int const_sig=getConstantSig(high);//Ϊ����������������ǩ

			//int const_sig=get_sig();//Ϊ��������������ǩ��
			int m=(int)pow(2.0,(double)high);
			long long tmp1=inverse(m,(long long)P1);//�������е���
			long long tmp2=inverse(m,(long long)P2);//�������е���

			int new_sig=0;//�洢��ֵ��������ǩ��
			int constant1=0;//�洢��λ�����еĲ�������
			int constant2=0;//�洢��λ�����еĲ�������
			string str_const="";//�洢��λ�����в��������ַ���
			//�����λ��ѭ���壬��Ϊ��ֵ����������ǩ���������㲹������
			int SigID=0;
			new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*tmp1)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*tmp2)%P2)%P2+P2)%P2;
			//str_const=" + "+int2str(constant);//���ò��������ַ���������

			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
			//����������ʼ��
			constant2Str(str2int(left_name));
			//����������ʼ��
//			constant2Str(high);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitShiftR(AC_BIND_CONST_NUM_"
					<< constantBindedStr(str2int(left_name)) << ".Data, " << int2HexStr(high) << ", "
					<< int2HexStr(tmp1) << ", "<< int2HexStr(tmp2) << ", "
					<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_name
				<<"'s sig :"<<left_sig<<endl<<endl;
			}

			/*int high=str2int(left_name)>>str2int(right_name);//�����λ�����Ľ��
			red_assign_const(var_name,high,v);//���ó�����ֵ���뺯�����б���*/

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
		string error_info="coding shift right operation : "+str;
		throw error_info;
	}
}

