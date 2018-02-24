
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
/*���ܣ����컯����ļ���
���������l_1 ��������ĵ�һ�����ֵ
	   l_2��������ĵڶ������ֵ
	   r_1 �Ҳ������ĵ�һ�����ֵ
	   r_2 �Ҳ������ĵڶ������ֵ
	   var_name ������ֵ
���������
����ֵ��
���ߣ��ܺ�
*/
static void new_sub(string l_1,string l_2,string r_1,string r_2,string var_name)
{	int R;
	S_this=*++iter;
	R=S_this-S_last;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_1="<<l_1<<"-"<<r_1<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<l_2<<"-"<<r_2<<"+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
	gen_out<<endl;

}
// ���ܣ������Ҳ�������Ϊ�����ļ���������б��룬���ڼ�������������ģ�顣�����������ĸ�λ����
// 			�͵�λ���룬���е�λ����ѭ��������ѭ�����⣬����ʱ����͸���ǩ��
// ���������var_name-��ֵ������
//			      l_name-�������������
//			      r_name-�Ҳ�����������
// 			     v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void red_sub_I(string var_name,string l_name,string r_name,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_str_arg(l_name,"left operator's name");
		check_str_arg(r_name,"right operator's name");
		check_pt_arg(v,"var_info list's pointer");
		
		//int left_sig=get_current_sig(v,l_name,var_scope);//ȡ���������������ǩ��
		//int right_sig=get_current_sig(v,r_name,var_scope);//ȡ���������������ǩ��
		int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);
		int right_sig=get_current_sig(v,get_global_string_sig(r_name,var_scope),var_scope);//ȡ���Ҳ�����������ǩ��
		
		//�����������ĸ�λ����
		/*out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" - H_"<<r_name<<";"<<endl;*/
		
		int new_sig=0;//�洢��ֵ��������ǩ��
		int constant1=0;//�洢��λ�����еĲ�������
		int constant2=0;//�洢��λ�����еĲ�������
			//��λ��ѭ�����λ��ѭ���壬��Ϊ��ֵ����������ǩ���������㲹������
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
		//new_sig=get_sig();
			constant1=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
			                 %(long long)P1+(long long)P1)%(long long)P1;
			constant2=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
						                 %(long long)P2+(long long)P2)%(long long)P2;
			//�������������ѭ������ĵ�λ����
			out_indent(indent_num,gen_out);
		/*	red_Sub_VVVVV("L_"+var_name, "L_"+l_name,
                                			"L_"+r_name, int2str(constant), "DT");*/
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			string r_name_head=TmpVarJudge(r_name)?r_name:get_global_string(r_name,var_scope);
			gen_out<<var_head<<".Data = "<<"F_VCL_Sub("<<l_name_head<<".Data, "<<r_name_head
								<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			<<"'s sig :"<<left_sig<<" "<<r_name<<"'s sig :"<<right_sig<<endl<<endl;
		}
		
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding sub operation (operators are both variable) : "+str;
		throw error_info;
	}
}

// ���ܣ����������Ϊ�������Ҳ�����Ϊ�����ļ���������б��룬���ڼ�������������ģ�顣
// 			�����������ĸ�λ����͵�λ���룬���е�λ����ѭ��������ѭ�����⣬����ʱ����͸���ǩ��
// ���������var_name-��ֵ������
//			      left_name-�����������������
//			      right_const-�Ҳ�����������ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void red_sub_II(string var_name,string left_name,int right_const ,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_str_arg(left_name,"left operand's name");
		check_pt_arg(v,"var_info list's pointer");
		
		int left_sig=get_current_sig(v,get_global_string_sig(left_name,var_scope),var_scope);//ȡ�����������ǩ��
		//int const_sig=get_sig();//Ϊ�Ҳ�����������ǩ��
		int const_sig=getConstantSig(right_const);//Ϊ����������������ǩ��
		//�����Ҳ������ĵ�λ
		//long long const_low=-((long long)(right_const)*(long long)K_NUM)%A+(long long)const_sig;
		
		int new_sig=0;//�洢��ֵ��������ǩ��
		int constant1=0;//�洢��λ�����еĲ�������
		int constant2=0;//�洢��λ�����еĲ�������
/*		string str_const=(right_const>=0)?(" - "+int2str(right_const))://������������ת��Ϊ�ַ���
                                       (" - ("+int2str(right_const)+")");
		
		//�����������ĸ�λ����
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = H_"<<left_name<<str_const<<";"<<endl;*/

		//�����λ��ѭ���壬��Ϊ��ֵ����������ǩ���������㲹������
		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����

		//constant=(((long long)new_sig-(long long)left_sig+(long long)const_sig-(long long)const_low)
		//				  %(long long)A+(long long)A)%(long long)A;
		constant1=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
					                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
							                 %(long long)P2+(long long)P2)%(long long)P2;
	/*	//�������������ѭ������ĵ�λ����
		out_indent(indent_num,gen_out);
		red_Add_VVI("L_"+var_name, "L_"+left_name,int2str(constant));
		*/
		//����������ʼ��
		constant2Str(right_const);
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(left_name)?left_name:get_global_string(left_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub("<<l_name_head<<".Data, AC_BIND_CONST_NUM_"<<constantBindedStr(right_const)
					<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			<<"'s sig :"<<left_sig<<" "<<right_const<<"'s sig :"<<const_sig<<endl<<endl;
		}
		

	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding sub operation (left operator is variable,right is constant) : "+str;
		throw error_info;
	}
}

static void red_sub_II(string var_name,string left_name,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_str_arg(left_name,"left operand's name");
		check_pt_arg(v,"var_info list's pointer");

		int left_sig=get_current_sig(v,get_global_string_sig(left_name,var_scope),var_scope);//ȡ�����������ǩ��
		//int const_sig=get_sig();//Ϊ�Ҳ�����������ǩ��
		int const_sig=Bool_True_Sig;//Ϊ����������������ǩ��
		//�����Ҳ������ĵ�λ
		//long long const_low=-((long long)(right_const)*(long long)K_NUM)%A+(long long)const_sig;

		int new_sig=0;//�洢��ֵ��������ǩ��
		int constant1=0;//�洢��λ�����еĲ�������
		int constant2=0;//�洢��λ�����еĲ�������
/*		string str_const=(right_const>=0)?(" - "+int2str(right_const))://������������ת��Ϊ�ַ���
                                       (" - ("+int2str(right_const)+")");

		//�����������ĸ�λ����
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = H_"<<left_name<<str_const<<";"<<endl;*/

		//�����λ��ѭ���壬��Ϊ��ֵ����������ǩ���������㲹������
		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����

		//constant=(((long long)new_sig-(long long)left_sig+(long long)const_sig-(long long)const_low)
		//				  %(long long)A+(long long)A)%(long long)A;
		constant1=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
					                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
							                 %(long long)P2+(long long)P2)%(long long)P2;
	/*	//�������������ѭ������ĵ�λ����
		out_indent(indent_num,gen_out);
		red_Add_VVI("L_"+var_name, "L_"+left_name,int2str(constant));
		*/
		//����������ʼ��
		out_indent(indent_num,gen_out);
		gen_out<<"AC_BIND_CONST_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;"<<endl;

		out_indent(indent_num,gen_out);
		gen_out<<"AC_BIND_CONST_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;"<<endl;

		out_indent(indent_num,gen_out);
		gen_out<<"AC_BIND_CONST_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;"<<endl;
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(left_name)?left_name:get_global_string(left_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub("<<l_name_head<<".Data, AC_BIND_CONST_BOOL_TRUE"
					<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;

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
			<<"'s sig :"<<left_sig<<" "<<"TRUE"<<"'s sig :"<<const_sig<<endl<<endl;
		}


	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding sub operation (left operator is variable,right is constant) : "+str;
		throw error_info;
	}
}

// ���ܣ����������Ϊ�������Ҳ�����Ϊ�����ļ���������б��룬���ڼ�������������ģ�顣
// 			�����������ĸ�λ����͵�λ���룬���е�λ����ѭ��������ѭ�����⣬����ʱ����͸���ǩ��
// ���������var_name-��ֵ������
//			      left_const-�������������ֵ
//			      right_name-�Ҳ���������������
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void red_sub_III(string var_name,int left_const,string right_name ,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_str_arg(right_name,"right operand's name");
		check_pt_arg(v,"var_info list's pointer");
		
		int right_sig=get_current_sig(v,get_global_string_sig(right_name,var_scope),var_scope);//ȡ���Ҳ�������ǩ��
		//int const_sig=get_sig();//Ϊ�������������ǩ��
		int const_sig=getConstantSig(left_const);//Ϊ����������������ǩ��
		//������������ĵ�λ
		//long long const_low=-((long long)(left_const)*(long long)K_NUM)%(long long)A+(long long)const_sig;
		
		int new_sig=0;//�洢��ֵ��������ǩ��
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
		int constant1=0;//�洢��λ�����еĲ�������
		int constant2=0;//�洢��λ�����еĲ�������
		constant1=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
							                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
									                 %(long long)P2+(long long)P2)%(long long)P2;

		//new_sig=get_sig();
		//constant=(((long long)new_sig+(long long)right_sig-(long long)const_sig
		//				  +(long long)const_low)%(long long)A+(long long)A)%(long long)A;
		/*//�����������ĸ�λ����
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = "<<left_const<<" - H_"<<right_name<<";"<<endl;
		
		//�����������ĵ�λ����
		out_indent(indent_num,gen_out);
		red_Sub_VVVVV("L_"+var_name, int2str(constant), "L_"+right_name, "DT", "DT");
		*/

		/*//����������ʼ��
		constant2Str(left_const);
		out_indent(indent_num,gen_out);
		gen_out<<"TC_"<<(var_name)<<".Data= "<<"F_VCL_Sub(AC_BIND_CONST_NUM_"<<(left_const)<<".Data,TC_"<<right_name
					<<".Data,("<<(constant1)<<"),("<<(constant2)<<"));"<<endl;*/

		//����������ʼ��
		constant2Str(left_const);
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string r_name_head=TmpVarJudge(right_name)?right_name:get_global_string(right_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub(AC_BIND_CONST_NUM_"<<constantBindedStr(left_const)<<".Data, "<<r_name_head
					<<".Data, " <<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_const
			<<"'s sig :"<<const_sig<<" "<<right_name<<"'s sig :"<<right_sig<<endl<<endl;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding sub operation (left operator is constant,right is variable) : "+str;
		throw error_info;
	}
}
// ���ܣ������Ҳ�������Ϊ�����ļ���������б��룬���ڼ�������������ģ�顣
// ���������var_name-��ֵ������
//			      left_const-�������������ֵ
//			      right_const-�Ҳ�����������ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void red_sub_IIII(string var_name,int left_const,int right_const ,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(v,"var_info list's pointer");
		int left_sig=getConstantSig(left_const);
		int right_sig=getConstantSig(right_const);
		int new_sig=0;//�洢��ֵ��������ǩ��
		int constant1=0;//�洢��λ�����еĲ�������
		int constant2=0;//�洢��λ�����еĲ�������

		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		//constant=(((long long)new_sig+(long long)right_sig-(long long)const_sig
		//				  +(long long)const_low)%(long long)A+(long long)A)%(long long)A;
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
		constant1=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
									                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
													 %(long long)P2+(long long)P2)%(long long)P2;
		/*//�����������ĸ�λ����
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = "<<left_const<<" - H_"<<right_name<<";"<<endl;

		//�����������ĵ�λ����
		out_indent(indent_num,gen_out);
		red_Sub_VVVVV("L_"+var_name, int2str(constant), "L_"+right_name, "DT", "DT");
		*/
		//����������ʼ��
		constant2Str(left_const);
		constant2Str(right_const);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		out_indent(indent_num,gen_out);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub(AC_BIND_CONST_NUM_"<<constantBindedStr(left_const)<<".Data, AC_BIND_CONST_NUM_"<<constantBindedStr(right_const)
					<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_const
			<<"'s sig :"<<left_sig<<" "<<right_const<<"'s sig :"<<right_sig<<endl<<endl;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding sub operation (left operator is constant,right is variable) : "+str;
		throw error_info;
	}
}
// ���ܣ��Լ���������б��룬���ڼ�������������ģ�顣�����������ĸ�λ����͵�λ���룬
//			�������Ҳ�����Ϊ�������߳����Ĳ�ͬ��������������б���
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_sub(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//����������ֵ���������ַ�������Ч��
		check_str_arg(var_name,"left variable's name");
		//����������ֵ���ʽ����ָ�����Ч��
		check_pt_arg(exp,"expression object's pointer");
		//����������̬ǩ����ָ�����Ч��
		check_pt_arg(v,"var_info list's pointer");
		int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
				int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
				//�ж����Ҳ������Ƿ��Ϊ����
				string l_1;
				string l_2;
				string r_1;
				string r_2;
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
				new_sub(l_1,l_2,r_1,r_2,var_name);
		/*int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		//�ж����Ҳ������Ƿ��Ϊ����
		if((4==left)&&(4==right))
		{	
			//����ǣ�����б���
			string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ���������������
			string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������
			//�����Ҳ�������Ϊ�����ļ���������б���
			gen_out<<var_name<<"_1="<<"sub_enc("<<l_name<<"_1,"<<r_name<<"_1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<l_name<<"_2,"<<r_name<<"_2"<<");"<<endl;
			//red_sub_I(var_name,l_name,r_name,v);
		}
		//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
		else if((4==left)&&(3==right))
		{
			//����ǣ�����б���
			string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ�������������
			string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			//���������Ϊ�������Ҳ�����Ϊ�����ļ���������б���
			gen_out<<var_name<<"_1="<<"sub_enc("<<l_name<<"_1,"<<str2int(right_name)<<"*A1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<l_name<<"_2,"<<str2int(right_name)<<"*A2"<<");"<<endl;
			if(right_name=="e_TRUE")
				red_sub_II(var_name,l_name,v);
			else
				red_sub_II(var_name,l_name,str2int(right_name),v);

		}
		//�ж���������Ƿ�Ϊ�������Ҳ������Ƿ�Ϊ����
		else if((3==left)&&(4==right))
		{
			//����ǣ�����б���
			string left_name=exp->get_cel()->get_c();//ȡ�������������������
			string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������

			//���������Ϊ�������Ҳ�����Ϊ�����ļ���������б���
			gen_out<<var_name<<"_1="<<"sub_enc("<<str2int(left_name)<<"*A1,"<<r_name<<"_1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<str2int(left_name)<<"*A2,"<<r_name<<"_2"<<");"<<endl;
			cout<<1;
			//red_sub_III(var_name,str2int(left_name),r_name,v);
		}
		else if((3==left)&&(3==right))
		{
			//������Ҳ�������Ϊ�����������������
			string left_name=exp->get_cel()->get_c();//ȡ���������������
			string right_name=exp->get_cer()->get_c();//ȡ���Ҳ�����������
			gen_out<<var_name<<"_1="<<"sub_enc("<<str2int(left_name)<<"*A1,"<<str2int(right_name)<<"*A1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<str2int(left_name)<<"*A2,"<<str2int(right_name)<<"*A2"<<");"<<endl;
			//red_sub_IIII(var_name,str2int(left_name),str2int(right_name),v);
			//int high=str2int(left_name)-str2int(right_name);//��������Ҳ�����֮��
			//red_assign_const(var_name,high,v);//���ó�����ֵ���뺯�����б���
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
		string error_info="coding sub operation : "+str;
		throw error_info;
	}
}

