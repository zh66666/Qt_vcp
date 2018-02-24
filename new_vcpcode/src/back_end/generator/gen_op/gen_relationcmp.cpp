#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"

// 定义在文件: src/main_utils.cpp
extern bool debug_flag;
extern int indent_num;
extern string int2HexStr_data(const int num);

//定义在文件 : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);

int GetOpCode(string OpStr);

//计算关系运算接口中补偿数(SubComp1/2  Comp1/2)所需要用到的签名
static int PreCalRelCmpSig[2][6];//关系运算预计算签名
static int RelCmpSubVarSig[6];//关系比较运算中，两个输入的操作数相减所得变量的签名

// 功能：对关系比较运算进行编码
// 输入参数：var_name-左值变量名
//           exp-赋值表达式对象指针
//           op_str-操作符号
// 			 v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：邹
void Red_RelationCmp(string var_name,Expression *exp,list<Var_info>* v)
{
	long long Prime[2]={P1,P2};
	int SubConst[2]={0};//减法运算的补偿常数，即文档中inSubComp1/2
	int OpConst[2]={0};//最终补偿常数，即文档中inComp1/2
	try
	{
		//检查输入参数的有效性
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


		//计算SubComp1/2
/*		for(int i=0;i<2;i++)
		{
			long long SubPreSig=0;
			if(OpCode==2 || OpCode==3)
				SubPreSig=((long long)right_sig-(long long)left_sig+Prime[i])%Prime[i];
			else
				SubPreSig=((long long)left_sig-(long long)right_sig+Prime[i])%Prime[i];
			SubConst[i]=((RelCmpSubVarSig[OpCode]-SubPreSig)%Prime[i]+Prime[i])%Prime[i];
		}
		//计算Comp1/2
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//存储左值变量的新签名

		for(int i=0;i<2;i++)
		{
			OpConst[i]=(((long long)new_sig-(long long)PreCalRelCmpSig[i][OpCode])%Prime[i]+Prime[i])%Prime[i];
		}*/

		//输出编码语句
//		string LeftVar=TmpVarJudge(var_name)? var_name:var_name;
/*		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar,FUNC_REL_CMP_CODED[OpCode],
				l_name.c_str(),r_name.c_str(),int2HexStr(SubConst[0]).c_str(),int2HexStr(SubConst[1]).c_str(),
				int2HexStr(OpConst[0]).c_str(),int2HexStr(OpConst[1]).c_str(),NULL);*/
		out_indent(indent_num,gen_out);
		gen_out<<var_name+"_1"<<"="<<l_name_1.c_str()<<RELATIONSHIP[OpCode]<<r_name_1.c_str()<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name+"_2"<<"="<<l_name_2.c_str()<<RELATIONSHIP[OpCode]<<r_name_2.c_str()<<";"<<endl;
/*		//输出签名Id调整语句
		LeftVar=TmpVarJudge(var_name)? var_name:CODED_VAR_PREFIX+var_name;
		CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;*/

		//如果是调试模式，则输出调试信息：左值的签名，左操作数和右操作数的签名
		if(true==debug_flag)
		{
	/*		out_indent(indent_num,gen_out);
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<l_name
			<<"'s sig :"<<left_sig<<" "<<r_name<<"'s sig :"<<right_sig<<endl<<endl;*/
		}
//		add_new_sig(v,var_name,new_sig,var_scope);//将左值变量新签名加入动态签名表
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding relation cmp operation (operators are both variable) : "+str;
		throw error_info;
	}
}

//获取操作符对应的操作代号
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

//计算关系比较运算的各类运算的预计算签名
//预计算签名=S_Var+SIF_U1  输出变量签名+True分支签名
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

