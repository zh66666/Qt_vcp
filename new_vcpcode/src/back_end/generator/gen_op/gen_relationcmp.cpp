#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"

// �������ļ�: src/main_utils.cpp
extern bool debug_flag;
extern int indent_num;
extern string int2HexStr_data(const int num);

//�������ļ� : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);

int GetOpCode(string OpStr);

//�����ϵ����ӿ��в�����(SubComp1/2  Comp1/2)����Ҫ�õ���ǩ��
static int PreCalRelCmpSig[2][6];//��ϵ����Ԥ����ǩ��
static int RelCmpSubVarSig[6];//��ϵ�Ƚ������У���������Ĳ�����������ñ�����ǩ��

// ���ܣ��Թ�ϵ�Ƚ�������б���
// ���������var_name-��ֵ������
//           exp-��ֵ���ʽ����ָ��
//           op_str-��������
// 			 v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���
void Red_RelationCmp(string var_name,Expression *exp,list<Var_info>* v)
{
	long long Prime[2]={P1,P2};
	int SubConst[2]={0};//��������Ĳ������������ĵ���inSubComp1/2
	int OpConst[2]={0};//���ղ������������ĵ���inComp1/2
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");

		int OpCode=GetOpCode(exp->get_op());

		int left_sig=0,right_sig=0;
		int OpValSig[2]={0};
		string OpValName[2];
		Expression *OpVal[2]={exp->get_cel(),exp->get_cer()};
		string const_name;
		string l_name_1,l_name_2,r_name_1,r_name_2;


			if(OpVal[0]->get_stamp()==3)
			{
				const_name=OpVal[0]->get_c();
				l_name_1="8193*"+int2HexStr_data(str2int(const_name));
				l_name_2="8191*"+int2HexStr_data(str2int(const_name));

			}
			else if(OpVal[0]->get_stamp()==4)
			{
				string origin_name=OpVal[0]->get_cvar()->get_name();
				if(is_global_var(origin_name))
					origin_name=get_bind_local_var(origin_name,var_scope);
				l_name_1=origin_name+"_1";
				l_name_2=origin_name+"_2";
			}
			else
			{
				string error_str="Invaild opVal not constant or variable!\n";
				throw error_str;
			}

			if(OpVal[1]->get_stamp()==3)
			{
				const_name=OpVal[1]->get_c();
/*				r_name_1="8193*"+int2HexStr_data(str2int(const_name));
				r_name_2="8191*"+int2HexStr_data(str2int(const_name));*/
				r_name_1="8193*"+(const_name);
				r_name_2="8191*"+(const_name);
			}
			else if(OpVal[1]->get_stamp()==4)
			{
				string origin_name=OpVal[1]->get_cvar()->get_name();
				if(is_global_var(origin_name))
					origin_name=get_bind_local_var(origin_name,var_scope);
				r_name_1=origin_name+"_1";
				r_name_2=origin_name+"_2";
			}
			else
			{
				string error_str="Invaild opVal not constant or variable!\n";
				throw error_str;
			}


		//����SubComp1/2
/*		for(int i=0;i<2;i++)
		{
			long long SubPreSig=0;
			if(OpCode==2 || OpCode==3)
				SubPreSig=((long long)right_sig-(long long)left_sig+Prime[i])%Prime[i];
			else
				SubPreSig=((long long)left_sig-(long long)right_sig+Prime[i])%Prime[i];
			SubConst[i]=((RelCmpSubVarSig[OpCode]-SubPreSig)%Prime[i]+Prime[i])%Prime[i];
		}
		//����Comp1/2
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��

		for(int i=0;i<2;i++)
		{
			OpConst[i]=(((long long)new_sig-(long long)PreCalRelCmpSig[i][OpCode])%Prime[i]+Prime[i])%Prime[i];
		}*/

		//����������
//		string LeftVar=TmpVarJudge(var_name)? var_name:var_name;
/*		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar,FUNC_REL_CMP_CODED[OpCode],
				l_name.c_str(),r_name.c_str(),int2HexStr(SubConst[0]).c_str(),int2HexStr(SubConst[1]).c_str(),
				int2HexStr(OpConst[0]).c_str(),int2HexStr(OpConst[1]).c_str(),NULL);*/
		out_indent(indent_num,gen_out);
		gen_out<<var_name+"_1"<<"="<<l_name_1.c_str()<<RELATIONSHIP[OpCode]<<r_name_1.c_str()<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name+"_2"<<"="<<l_name_2.c_str()<<RELATIONSHIP[OpCode]<<r_name_2.c_str()<<";"<<endl;
/*		//���ǩ��Id�������
		LeftVar=TmpVarJudge(var_name)? var_name:CODED_VAR_PREFIX+var_name;
		CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;*/

		//����ǵ���ģʽ�������������Ϣ����ֵ��ǩ��������������Ҳ�������ǩ��
		if(true==debug_flag)
		{
	/*		out_indent(indent_num,gen_out);
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<l_name
			<<"'s sig :"<<left_sig<<" "<<r_name<<"'s sig :"<<right_sig<<endl<<endl;*/
		}
//		add_new_sig(v,var_name,new_sig,var_scope);//����ֵ������ǩ�����붯̬ǩ����
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding relation cmp operation (operators are both variable) : "+str;
		throw error_info;
	}
}

//��ȡ��������Ӧ�Ĳ�������
//0->=
//1-<
//2-<=
//3->
//4-==
//5-!=
int GetOpCode(string OpStr)
{
	int OpCode=0;
	if(OpStr==">=")
	{
		OpCode=0;
	}
	else if(OpStr=="<")
	{
		OpCode=1;
	}
	else if(OpStr== "<=")
	{
		OpCode=2;
	}
	else if(OpStr== ">")
	{
		OpCode=3;
	}
	else if(OpStr=="==")
	{
		OpCode=4;
	}
	else if(OpStr=="!=")
	{
		OpCode=5;
	}
	else
	{
		string error_str="Invaild relation compare operator!\n";
		throw error_str;
	}

	return OpCode;
}

//�����ϵ�Ƚ�����ĸ��������Ԥ����ǩ��
//Ԥ����ǩ��=S_Var+SIF_U1  �������ǩ��+True��֧ǩ��
void PreCalRelationCmpSig()
{
	for(int i=0;i<6;i++)
		RelCmpSubVarSig[i]=Diff1Sig[i];

	int Prime[2]={P1,P2};
	for(int Prime_i=0;Prime_i<2;Prime_i++)
	{
		for(int j=0;j<4;j++)
		{
			long long IfSig1=((long long)Diff1Sig[j]*CalOutSigK[j])%Prime[Prime_i];
			PreCalRelCmpSig[Prime_i][j]=(IfSig1+VarSig1[j])%Prime[Prime_i];
		}
		for(int j=4;j<6;j++)
		{
			long long R=(Diff1Sig[j]+Diff2Sig[j])%Prime[Prime_i];
			long long IfSig1=(R*CalOutSigK[j])%Prime[Prime_i];
			PreCalRelCmpSig[Prime_i][j]=(IfSig1+VarSig1[j])%Prime[Prime_i];
		}
	}
}

