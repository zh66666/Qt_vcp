#include "gen_utils.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// �������ļ�: src/main_utils.cpp
extern bool debug_flag;

// ���ܣ��Գ�����ֵ������б��룬�������ĸ�λ����͵�λ���룬ѭ������ѭ�������ͬ���ı��뷽��
// ���������var_name-��ֵ������
//			      high-������ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����޸�by�ܺ�
void red_assign_const(string var_name,int high,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(v,"var_info list's pointer");
		int R;
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_1="<<high<<"*A1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<high<<"*A2+("<<R<<");"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
		gen_out<<endl;

		//int const_sig=get_sig();//Ϊ��������һ��ǩ��
/*		int const_sig=getConstantSig(high);
		constantMap[high]=const_sig;
		int sig_num_0=getConstantSig(0);//Ϊ0����һ��ǩ����Z=1==>Z=1+0;
		constantMap[0]=sig_num_0;
		long long low1=-((long long)high*K_NUM_P1)%P1+(long long)const_sig;//���㳣���ĵ�λ
		long long low2=-((long long)high*K_NUM_P2)%P2+(long long)const_sig;//���㳣���ĵ�λ
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		//int new_sig=get_sig();//Ϊ��ֵ��������һ����ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ǩ����ӵ���̬ǩ������
		//�����λ�����еĲ�������
		int constant1=(((long long)new_sig-(long long)const_sig-(long long)sig_num_0)%(long long)P1+(long long)P1)%(long long)P1;
		int constant2=(((long long)new_sig-(long long)const_sig-(long long)sig_num_0)%(long long)P2+(long long)P2)%(long long)P2;

			
		//���������ֵ����ĸ�λ����
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = "<<high<<";"<<endl;

		//���������ֵ����ĵ�λ����
		out_indent(indent_num,gen_out);
		gen_out<<"L_"<<var_name<<" = "<<constant<<" + DT;"<<endl;
		constant2Str(high);
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		gen_out << var_head  << ".Data = " << "F_VCL_Assign(AC_BIND_CONST_NUM_"
				<< constantBindedStr(high) << ".Data, " << int2HexStr(constant1) << ", "
				<< int2HexStr(constant2) << ");" << endl;
		//���ǩ��Id�������
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;*/
	/*	//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ����������ǩ��
		if(true==debug_flag)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<high<<"'s sig :"
			<<const_sig<<endl<<endl;
		}*/
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding const assign operation : "+str;
		throw error_info;
	}
	
}

// ���ܣ��Ա�����ֵ������б��룬�������ĸ�λ����͵�λ���룬ѭ������ѭ�������ͬ���ı��뷽��
// ���������var_name-��ֵ������
//			      right_name-���ʽ������
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_assign_var(string var_name, string right_name,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_str_arg(right_name,"right variable's name");
		check_pt_arg(v,"var_info list's pointer");
		int R;
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_1="<<right_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<right_name<<"_2+("<<R<<");"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
		gen_out<<endl;
//		int var_sig=get_current_sig(v,get_global_string_sig(right_name,var_scope),var_scope);//ȡ����ֵ������ǩ��
//		int SigID=0;
//		int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
//		//int new_sig=get_sig();//Ϊ��ֵ����������ǩ��
//		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
//		int sig_num_0=getConstantSig(0);//Ϊ0����һ��ǩ����z=a==>z=a+0;
//		constantMap[0]=sig_num_0;
//		int constant1=(((long long)new_sig-(long long)var_sig-(long long)sig_num_0)%(long long)P1+(long long)P1)%(long long)P1;//�����λ�õ��Ĳ�������
//		int constant2=(((long long)new_sig-(long long)var_sig-(long long)sig_num_0)%(long long)P2+(long long)P2)%(long long)P2;//�����λ�õ��Ĳ�������
//
//		/*//���������ֵ�ĸ�λ����
//		out_indent(indent_num,gen_out);
//		gen_out<<"H_"<<var_name<<" = H_"<<right_name<<";"<<endl;
//
//		//���������ֵ�ĵ�λ����
//		out_indent(indent_num,gen_out);
//		red_Add_VVI("L_"+var_name, "L_"+right_name, int2str(constant));*/
//		out_indent(indent_num,gen_out);
//		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
//		string r_name_head=TmpVarJudge(right_name)?right_name:get_global_string(right_name,var_scope);
//
//		gen_out << var_head << ".Data = " << "F_VCL_Assign("
//				<< r_name_head << ".Data, " << int2HexStr(constant1) << ", "
//				<< int2HexStr(constant2) << ");" << endl;
//		//���ǩ��Id�������
//		string LeftVar=var_head;
//		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
//				int2HexStr(SigID).c_str(),NULL);
//		out_indent(indent_num,gen_out);
//		gen_out<<CodedFuncCallStr<<endl;
//
//		//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ����������ǩ��
//		if(true==debug_flag)
//		{
//			out_indent(indent_num,gen_out);
//			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<right_name<<"'s sig :"
//			<<var_sig<<endl<<endl;
//		}

	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding variable assign operation : "+str;
		throw error_info;
	}
}


