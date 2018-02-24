
#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// �������ļ�: src/main_utils.cpp
extern bool debug_flag;

//�������ļ� : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);

//�������ļ� : ./inverse.c
extern long long inverse(long long b,long long prime);
/*
 ���ܣ����컯����ĳ���
 ���������l_1 ��������ĵ�һ�����ֵ
	   l_2��������ĵڶ������ֵ
	   r_1 �Ҳ������ĵ�һ�����ֵ
 	   r_2 �Ҳ������ĵڶ������ֵ
	   var_name ������ֵ
 ���������
 ����ֵ��
 ���ߣ��ܺ�
*/
static void new_div(string l_1,string l_2,string r_1,string r_2,string var_name)
{	int R;
	S_this=*++iter;
	R=S_this-S_last;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_1="<<"("<<l_1<<")/("<<r_1<<")*A1;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<"("<<l_2<<")/("<<r_2<<")*A2+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_var_decode=check_decode("<<var_name<<"_1,"<<var_name<<"_2-("<<R<<"))+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_l=check_decode("<<l_1<<","<<l_2<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_r=check_decode("<<r_1<<","<<r_2<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_var="<<"div_temp_l/div_temp_r;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"RS=RS+div_temp_var"<<"-"<<"div_temp_var_decode;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<endl;
}
// ���ܣ��Գ���������б��룬�����������ĸ�λ����͵�λ���룬ע�⣬ֻ�ܶԳ���Ϊ�����ĳ�
//			�����б��룬ͬʱ����ѭ�����⣬���ò�ͬ�ı��뷽����
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_div(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		string l_1;
		string l_2;
		string r_1;
		string r_2;
		int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		if(left==4)
		{
			l_1=exp->get_cel()->get_cvar()->get_name()+"_1";
			l_2=exp->get_cel()->get_cvar()->get_name()+"_2";
		}
		else
		{
			l_1=exp->get_cel()->get_c()+"*A1";
			l_2=exp->get_cel()->get_c()+"*A2";
		}
		if(right==4)
		{
			r_1=exp->get_cer()->get_cvar()->get_name()+"_1";
			r_2=exp->get_cer()->get_cvar()->get_name()+"_2";
		}
		else
		{
			r_1=exp->get_cer()->get_c()+"*A1";
			r_2=exp->get_cer()->get_c()+"*A2";
		}

		new_div(l_1,l_2,r_1,r_2,var_name);
		/*int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
		if((4==left)&&(4==right))
				{
					//����ǣ�����б���
					string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ���������������
					string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������
					//�����Ҳ�������Ϊ�����ĳ˷�������б���
					gen_out<<var_name<<"_1="<<"div_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A2);"<<endl;
					//red_mul_I(var_name,l_name,r_name,v);
				}
				//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
				else if((4==left)&&(3==right))
				{
					//����ǣ�����б���
					string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ�������������
					string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
					int right_const=str2int(right_name);//���Ҳ�����ת��Ϊ����
					//������������ת��Ϊ�ַ���
					string str_const=(right_const>=0)?(right_name):(" ("+right_name+")");
					//����˷�����ĸ�λ����
					out_indent(indent_num,gen_out);
					gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" * "<<str_const<<";"<<endl;

					//�����λ����
					gen_out<<var_name<<"_1="<<"div_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A2);"<<endl;
					//red_mul_II(var_name,l_name,right_const,v);
				}
				//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
				else if((3==left)&&(4==right))
				{
					//����ǣ�����б���
					string left_const=exp->get_cel()->get_c();//ȡ�������������������
					string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������

					//����˷�����ĸ�λ����
					out_indent(indent_num,gen_out);
					gen_out<<"H_"<<var_name<<" = "<<left_name<<" * H_"<<r_name<<";"<<endl;

					//�����λ����
					gen_out<<var_name<<"_1="<<"div_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A2);"<<endl;
					//red_mul_III(var_name,str2int(left_const),r_name,v);
				}
				//�ж����Ҳ������Ƿ��Ϊ����
				else if((3==left)&&(3==right))
				{
					//������Ҳ�������Ϊ������������������
					string left_name=exp->get_cel()->get_c();//ȡ���������������
					string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
					//int high=str2int(left_name)+str2int(right_name);//��������Ҳ�����֮��
					//red_assign_const(var_name,high,v);//���ó�����ֵ���뺯�����б���
					gen_out<<var_name<<"_1="<<"div_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A2);"<<endl;
					//red_mul_IIII(var_name,str2int(left_name),str2int(right_name),v);
				}
	if((4==left)&&(3==right))
		{
			//����ǣ�����б���
			string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ�������������������
			string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//ȡ�����������ǩ��
			int right_value=str2int(right_name);//���Ҳ������ַ���ת��Ϊ����

			int const_sig=getConstantSig(right_value);//Ϊ����������������ǩ��

			//int const_sig=get_sig();//Ϊ��������ǩ����������ʹ�ã��Ա���չ
			long long h_temp_P1=inverse((long long)right_value,(long long)P1);//���������A�е���
			long long h_temp_P2=inverse((long long)right_value,(long long)P2);//���������A�е���
			int new_sig=0;//�洢��ֵ��������ǩ��
			int constant1=0;//�洢��λ�����еĲ�������
			int constant2=0;//�洢��λ�����еĲ�������
			string str_const="";//�洢��λ�����в��������ַ���
			//λ��ѭ����ͷ�ѭ���壬��Ϊ��ֵ����������ǩ���������㲹������
			int SigID=0;
			new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*h_temp_P1)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*h_temp_P2)%P2)%P2+P2)%P2;
			str_const=" + "+int2str(constant1);//���ò��������ַ���������
			
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
			//�����������ĸ�λ����
			
			/*string str_divided="H_"+l_name;//���������ֵĸ�λ�ַ���
			//�жϱ������������Ƿ�ͬ��ֵ��������ͬ
			if(var_name==l_name)
			{
				//�����ͬ�������һ����ʱ�����滻�������Ĵ���
				out_indent(indent_num,gen_out);
				gen_out<<red_Tmp1_INT32<<" = H_"<<l_name<<";"<<endl;
				//���������ĸ�λ�����޸�Ϊ��ʱ������
				str_divided=red_Tmp1_INT32;
			}
			else
			{
				//�������ͬ������
			}
			//����������ʼ��
//			constant2Str(right_value);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			gen_out<<var_head<<".Data = "<<"F_VCL_Div("<<l_name_head<<".Data, "<<int2HexStr(right_value)
								<<", "<<int2HexStr(h_temp_P1)<<", "<<int2HexStr(h_temp_P2)<<", "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
		}
		//�ж����Ҳ������Ƿ��Ϊ����
		else if((3==left)&&(3==right))
		{
			//����������ͳ�����Ϊ��������ֱ�ӽ��������
			string left_name = exp->get_cel()->get_c();			//ȡ��������������
			string right_name = exp->get_cer()->get_c();			//ȡ������������
			//int high=str2int(left_name)/str2int(right_name);//������
			//red_assign_const(var_name,high,v);//���ó�����ֵ���뺯�����б���
			int left_sig = getConstantSig(str2int(left_name));	//Ϊ����������������ǩ��
			int right_value = str2int(right_name);			//���Ҳ������ַ���ת��Ϊ����

			int const_sig=getConstantSig(right_value);//Ϊ����������������ǩ��

			long long h_temp_P1 = inverse((long long) right_value, (long long) P1);	//���������P1�е���
			long long h_temp_P2 = inverse((long long) right_value, (long long) P2);	//���������P2�е���
			int new_sig = 0;			//�洢��ֵ��������ǩ��
			int constant1 = 0;			//�洢��λ�����еĲ�������
			int constant2 = 0;			//�洢��λ�����еĲ�������
			//λ��ѭ����ͷ�ѭ���壬��Ϊ��ֵ����������ǩ���������㲹������
			//new_sig = get_sig();
			int SigID=0;
			new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			constant1 = (((long long) new_sig
					- ((long long) left_sig * h_temp_P1) % P1) % P1 + P1) % P1;
			constant2 = (((long long) new_sig
					- ((long long) left_sig * h_temp_P2) % P2) % P2 + P2) % P2;

			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);	//����ֵ������ǩ�����붯̬ǩ����
			//����������ʼ��
			constant2Str(str2int(left_name));
			//����������ʼ��
//			constant2Str(right_value);
			out_indent(indent_num, gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_Div(AC_BIND_CONST_NUM_"
					<< constantBindedStr(str2int(left_name)) << ".Data, " << int2HexStr(right_value) << ","
					<< int2HexStr(h_temp_P1) << ", " << int2HexStr(h_temp_P2) << ", " << int2HexStr(constant1)
					<< ", " << int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
			if (true == debug_flag)
			{
				out_indent(indent_num, gen_out);
				gen_out << "//" << var_name << "'s new sig :" << new_sig << " "
						<< left_name << "'s sig :" << left_sig  << endl << endl;
			}
		}
		else
		{
			//������Ϊ�������ʱ���׳�������Ϣ
			string error_info="operand is invalid!";
			throw error_info;
		}*/
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding div operation : "+str;
		throw error_info;
	}
}

