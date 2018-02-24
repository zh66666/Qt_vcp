
#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// �������ļ�: src/main_utils.cpp
extern bool debug_flag;

extern long long i_temp_P1;

extern long long i_temp_P2;

//static long long i_temp_P1=0;//�洢�Գ˷�����ʱ�õ�����ֵ
//static long long i_temp_P2=0;//�洢�Գ˷�����ʱ�õ�����ֵ
//�������ļ� : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);
//�������ļ� : ./inverse.c
extern long long inverse(long long b,long long prime);
/*
 ���ܣ����컯����ĳ˷�
 ���������l_1 ��������ĵ�һ�����ֵ
	   l_2��������ĵڶ������ֵ
	   r_1 �Ҳ������ĵ�һ�����ֵ
 	   r_2 �Ҳ������ĵڶ������ֵ
	   var_name ������ֵ
 ���������
 ����ֵ��
 ���ߣ��ܺ�
*/
static void new_mul(string l_1,string l_2,string r_1,string r_2,string var_name)
{	int R;
	S_this=*++iter;
	R=S_this-S_last;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_A_1="<<l_1<<"*"<<r_1<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_A_2="<<l_1<<"*"<<r_2<<"+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_B_1="<<l_2<<"*"<<r_1<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_B_2="<<l_2<<"*"<<r_2<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_1=(mul_temp_A_1+mul_temp_A_2-("<<R<<"))>>14;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2=((mul_temp_B_1-mul_temp_B_2)>>1)+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
	gen_out<<endl;
}
// ���ܣ������Ҳ�������Ϊ�����ĳ˷�������б��룬���ڳ˷�����������ģ�顣����˷�����ĸ�λ����
// 			�͵�λ���룬���е�λ����ѭ��������ѭ�����⣬����ʱ����͸���ǩ��
// ���������var_name-��ֵ������
//			      l_name-�������������
//			      r_name-�Ҳ�����������
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void red_mul_I(string var_name,string l_name,string r_name,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_str_arg(l_name,"left operator's name");
		check_str_arg(r_name,"right operator's name");
		check_pt_arg(v,"var_info list's pointer");
		
		int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//ȡ���������������ǩ��
		int right_sig=get_current_sig(v,get_global_string_sig(r_name,var_scope),var_scope);//ȡ���Ҳ�����������ǩ��
		
		int const_1_1=((-(long long)2*(long long)left_sig)%P1+P1)%P1;//��������1����ֵǩ����2��
		int const_2_1=((-(long long)2*(long long)right_sig)%P1+P1)%P1;//��������2����ֵǩ����2��
		
		int const_1_2=((-(long long)2*(long long)left_sig)%P2+P2)%P2;//��������1����ֵǩ����2��
		int const_2_2=((-(long long)2*(long long)right_sig)%P2+P2)%P2;//��������2����ֵǩ����2��

		int new_sig=0;//�洢��ֵ��������ǩ��
		int const_3_1=0;//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������,������ΪP1
		int const_3_2=0;//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������,������ΪP2
		//string str_const_3="";//�洢ǩ�����������ַ���
		//λ��ѭ����ͷ�ѭ���壬��Ϊ��ֵ����������ǩ����������ǩ����������
		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		const_3_1=(((long long)new_sig-(((long long)left_sig*(long long)right_sig)%P1*i_temp_P1)%P1*2)%P1+P1)%P1;
		const_3_2=(((long long)new_sig-(((long long)left_sig*(long long)right_sig)%P2*i_temp_P2)%P2*2)%P2+P2)%P2;
		//str_const_3=" + "+int2str(const_3);//���ò��������ַ���
		
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
		
		//����˷�����ĸ�λ����
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
		string r_name_head=TmpVarJudge(r_name)?r_name:get_global_string(r_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Mul("<<l_name_head<<".Data, "<<r_name_head
				<<".Data, "<<""<<int2HexStr(left_sig)<<", "<<int2HexStr(right_sig)<<", "
				<<int2HexStr(const_3_1)<<", "<<int2HexStr(const_3_2)<<");"<<endl;
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
		/*red_Sub_VVVV(red_Tmp3_INT32, "L_"+l_name, "DT", int2str(const_1));
		out_indent(indent_num,gen_out);
		red_Sub_VVV(red_Tmp4_INT32,"DT", "L_"+r_name);
		out_indent(indent_num,gen_out);
		gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<red_Tmp3_INT32
		<<" * "<<red_Tmp4_INT32<<";"<<endl;
		out_indent(indent_num,gen_out);
		red_INT64MOD(red_Tmp1_INT32, red_Tmp_INT64);
		out_indent(indent_num,gen_out);
		red_Sub_VVVV(red_Tmp4_INT32, "L_"+r_name, "DT",int2str(const_2));
		out_indent(indent_num,gen_out);
		red_Sub_VVV(red_Tmp3_INT32, "DT", "L_"+l_name);
		out_indent(indent_num,gen_out);
		gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<red_Tmp3_INT32
		<<" * "<<red_Tmp4_INT32<<";"<<endl;
		out_indent(indent_num,gen_out);
		red_INT64MOD(red_Tmp2_INT32, red_Tmp_INT64);
		out_indent(indent_num,gen_out);
		red_Add_VVV("L_"+var_name, red_Tmp1_INT32, red_Tmp2_INT32);
		out_indent(indent_num,gen_out);
		gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")L_"<<var_name<<" * "
		<<i_temp<<str_const_3<<" + DT;"<<endl;
		out_indent(indent_num,gen_out);
		red_INT64MOD("L_"+var_name, red_Tmp_INT64);*/
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding sub operation (operators are both variable) : "+str;
		throw error_info;
	}
}

// ���ܣ������Ҳ���������һ��Ϊ������һ��Ϊ�����ĳ˷�������б��룬���ڳ˷�����������ģ�顣
// 			����˷�����ĵ�λ���룬������ѭ��������ѭ�����⣬����ʱ����͸���ǩ��
// ���������var_name-��ֵ������
//			      right_var-����������������
//			      right_const-������������ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void red_mul_II(string var_name,string left_var,int right_const ,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name, "left variable's name");
		check_str_arg(left_var, "varibale operator's name");
		check_pt_arg(v, "var_info list's pointer");
		int const_sig = getConstantSig(right_const);		//Ϊ����������������ǩ��
		int var_sig = get_current_sig(v, get_global_string_sig(left_var,var_scope), var_scope);		//ȡ��������������ǩ��
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		//int new_sig = get_sig();		//Ϊ��ֵ����������ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����

		int constant1 = 0;		//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������
		int constant2 = 0;		//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������
		constant1 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P1 * i_temp_P1)
						% P1 * 2) % P1 + P1) % P1;
		constant2 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P2 * i_temp_P2)
						% P2 * 2) % P2 + P2) % P2;
		//�����λ�����еĲ�������
		//int constant=((((long long)right_const*(long long)var_sig)%A-(long long)new_sig)%A+A)%A;
		//����������ʼ��
		constant2Str(right_const);
		//����˷�����ĸ�λ����
		out_indent(indent_num, gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(left_var)?left_var:get_global_string(left_var,var_scope);
		gen_out << var_head  << ".Data = " << "F_VCL_Mul("<<l_name_head
			 << ".Data, AC_BIND_CONST_NUM_" << constantBindedStr(right_const)
				<< ".Data, " << "" << int2HexStr(var_sig) << ", " << int2HexStr(const_sig) << ", "
				<< int2HexStr(constant1) << ", " << int2HexStr(constant2) << ");" << endl;
		//���ǩ��Id�������
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;
		/*	//����˷�����ĵ�λ����
		 out_indent(indent_num,gen_out);
		 red_Sub_VVV(red_Tmp3_INT32, "L_"+right_var, "DT");
		 out_indent(indent_num,gen_out);
		 gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<right_const
		 <<" * "<<red_Tmp3_INT32<<";"<<endl;
		 out_indent(indent_num,gen_out);
		 red_INT64MOD(red_Tmp3_INT32, red_Tmp_INT64);
		 out_indent(indent_num,gen_out);
		 red_Sub_VVVV("L_"+var_name, red_Tmp3_INT32, int2str(constant), "DT");*/

		//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//" << var_name << "'s new sig :" << new_sig << " "
					<< left_var << "'s sig :" << var_sig << " "<<right_const
					<<"'s sig :"<<const_sig<<endl << endl;
		}

	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding mul operation (one operator is variable,one is constant) : "+str;
		throw error_info;
	}
}

static void red_mul_III(string var_name,int left_const,string right_var ,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name, "left variable's name");
		check_str_arg(right_var, "varibale operator's name");
		check_pt_arg(v, "var_info list's pointer");
		int const_sig = getConstantSig(left_const);		//Ϊ����������������ǩ��
		int var_sig = get_current_sig(v, get_global_string_sig(right_var,var_scope), var_scope);		//ȡ��������������ǩ��
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		//int new_sig = get_sig();		//Ϊ��ֵ����������ǩ��
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
		int constant1 = 0;		//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������
		int constant2 = 0;		//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������
		constant1 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P1 * i_temp_P1)
						% P1 * 2) % P1 + P1) % P1;
		constant2 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P2 * i_temp_P2)
						% P2 * 2) % P2 + P2) % P2;
		//�����λ�����еĲ�������
		//int constant=((((long long)right_const*(long long)var_sig)%A-(long long)new_sig)%A+A)%A;
		//����������ʼ��
		constant2Str(left_const);
		//����˷�����ĸ�λ����
		out_indent(indent_num, gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string r_name_head=TmpVarJudge(right_var)?right_var:get_global_string(right_var,var_scope);
		/*gen_out << var_head << (var_name) << ".Data= " << "F_VCL_Mul("<<r_name_head
				<< (right_var) << ".Data,AC_BIND_CONST_NUM_" << (left_const)
				<< ".Data," << "" << int2HexStr(var_sig) << "," << int2HexStr(const_sig) << ","
				<< int2HexStr(constant1) << "," << int2HexStr(constant2) << ");" << endl;*/
		gen_out << var_head  << ".Data = "
				<< "F_VCL_Mul(AC_BIND_CONST_NUM_" << constantBindedStr(left_const) << ".Data, "
				<< r_name_head  << ".Data, " << ""
				<<int2HexStr(const_sig)  << ", " <<  int2HexStr(var_sig)<< ", "
				<< int2HexStr(constant1) << ", " << int2HexStr(constant2) << ");"
				<< endl;
		//���ǩ��Id�������
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;
		/*	//����˷�����ĵ�λ����
		 out_indent(indent_num,gen_out);
		 red_Sub_VVV(red_Tmp3_INT32, "L_"+right_var, "DT");
		 out_indent(indent_num,gen_out);
		 gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<right_const
		 <<" * "<<red_Tmp3_INT32<<";"<<endl;
		 out_indent(indent_num,gen_out);
		 red_INT64MOD(red_Tmp3_INT32, red_Tmp_INT64);
		 out_indent(indent_num,gen_out);
		 red_Sub_VVVV("L_"+var_name, red_Tmp3_INT32, int2str(constant), "DT");*/

		//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//" << var_name << "'s new sig :" << new_sig << " "
					<< right_var << "'s sig :" << var_sig << " "<<left_const
					<<"'s sig :"<<const_sig<<endl << endl;
		}

	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding mul operation (one operator is variable,one is constant) : "+str;
		throw error_info;
	}
}

static void red_mul_IIII(string var_name,int left_const,int right_const,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name, "left variable's name");
		check_pt_arg(v, "var_info list's pointer");

		int left_sig = getConstantSig(left_const);		//Ϊ����������������ǩ��
		int right_sig = getConstantSig(right_const);		//Ϊ����������������ǩ��

		int new_sig = 0;		//�洢��ֵ��������ǩ��
		int constant1 = 0;		//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������
		int constant2 = 0;		//�洢��ѭ���ⲿ�Ե�λ����ʱ��ǩ����������
		//string str_const_3="";//�洢ǩ�����������ַ���
		//λ��ѭ����ͷ�ѭ���壬��Ϊ��ֵ����������ǩ����������ǩ����������
		int SigID=0;
		new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
		//new_sig = get_sig();
		constant1 = (((long long) new_sig
				- (((long long) left_sig * (long long) right_sig) % P1 * i_temp_P1)
						% P1 * 2) % P1 + P1) % P1;
		constant2 = (((long long) new_sig
				- (((long long) left_sig * (long long) right_sig) % P2 * i_temp_P2)
						% P2 * 2) % P2 + P2) % P2;

		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);		//����ֵ������ǩ�����붯̬ǩ����
		//����������ʼ��
		constant2Str(left_const);
		constant2Str(right_const);

		//����˷�����ĸ�λ����
		out_indent(indent_num, gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		gen_out << var_head << ".Data = "
				<< "F_VCL_Mul(AC_BIND_CONST_NUM_" << constantBindedStr(left_const)
				<< ".Data, AC_BIND_CONST_NUM_" << constantBindedStr(right_const) << ".Data, "
				<< "" << int2HexStr(left_sig) << ", " << int2HexStr(right_sig) << ", " << int2HexStr(constant1)
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
					<< left_const << "'s sig :" << left_sig << " "
					<< right_const << "'s sig :" << right_sig << endl << endl;
		}

	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info =
				"coding sub operation (operators are both variable) : " + str;
		throw error_info;
	}
}


// ���ܣ��Գ˷�������б��룬���ڳ˷�����������ģ�顣����˷�����ĸ�λ����͵�λ���룬
//			�������Ҳ�����Ϊ�������߳����Ĳ�ͬ��������������б���
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_mul(string var_name,Expression* exp,list<Var_info>* v)
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

		new_mul(l_1,l_2,r_1,r_2,var_name);
		

		//i_temp_P1=inverse(8589934592ll,(long long)P1);//��2^33��P1���� Kֵ�йز���
		//i_temp_P2=inverse(8589934592ll,(long long)P2);//��2^33��P2����
		//�ж����Ҳ������Ƿ��Ϊ����
		/*if((4==left)&&(4==right))
		{	
			//����ǣ�����б���
			string l_name=exp->get_cel()->get_cvar()->get_name();//ȡ���������������
			string r_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������
			//�����Ҳ�������Ϊ�����ĳ˷�������б���
			gen_out<<var_name<<"_1="<<"mul_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A2);"<<endl;
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
			gen_out<<var_name<<"_1="<<"mul_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A2);"<<endl;
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
			gen_out<<var_name<<"_1="<<"mul_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A2);"<<endl;
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
			gen_out<<var_name<<"_1="<<"mul_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A2);"<<endl;
			//red_mul_IIII(var_name,str2int(left_name),str2int(right_name),v);
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
		string error_info="coding mul operation : "+str;
		throw error_info;
	}
}



