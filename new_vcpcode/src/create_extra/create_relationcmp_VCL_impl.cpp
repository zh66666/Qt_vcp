/*
 * create_relationcmp_VCL_impl.cpp
 *
 *  Created on: 2014年12月30日
 *      Author: LordVoldemort
 *  关系比较运算编码算法的实现
 */
#include "common.h"
#include "gen_utils.h"
#include <fstream>
#include "BuildOutputFunc_Utils.h"
#include "gen_secondary.h"

//定义在文件：main_utils.cpp
extern string int2HexStr_data(const int num);

//定义在文件：./create_extra_utils.cpp
extern ofstream extra_out;

//保存各关系比较符号表
static string RemCmpSymbol[6]={">=",">=",">=",">=","==","=="};
                              //>=    <    <=  >    ==   !=

struct TAG_IfSig
{
	int S1;
	int S2;
};

void constant2Str(ofstream &out,const int constant)
{
	string value=int2HexStr_data(constant);
	string bind_str="AC_BIND_CONST_NUM_"+value;
	string const_str="CONST_NUM_"+value;
//	if(constant<0)
//		value="NEG_"+int2str(-constant);
//	else
//		value=int2str(constant);
	out_indent(1,out);
	out<<bind_str+".Data.F"<<" = "<<const_str+".Data.F"<<";"<<endl;

	out_indent(1,out);
	out<<bind_str+".Data.C.C1"<<" = "<<const_str+".Data.C.C1 + g_GS_TL1;"<<endl;

	out_indent(1,out);
	out<<bind_str+".Data.C.C2"<<" = "<<const_str+".Data.C.C2 + g_GS_TL2;"<<endl;
}

class BeforeIf
{
protected:
	vector<string> init_stmt;
	string out_sig_func_name;
	string diff1_name;

	int calsig_K;
	int diff_sig;

	string left_var;
	string right_var;

	void Output_OutSigCalFunc(ofstream &out,string caloutsig_name)
	{
		string left_var="T_CHECK OutSig";
		string FuncCall=BuildOutputFunc_str(left_var,out_sig_func_name,
				string(caloutsig_name+VAR_TDATA).c_str(),int2HexStr(calsig_K).c_str(),NULL);
		out<<"  "<<FuncCall<<endl;
	}

	void Output_OutSigCalFunc(ofstream &out,string caloutsig_name_1,string caloutsig_name_2)
	{
		string left_var="T_CHECK OutSig";
		string FuncCall=BuildOutputFunc_str(left_var,out_sig_func_name,
				string(caloutsig_name_1+VAR_TDATA).c_str(),string(caloutsig_name_2+VAR_TDATA).c_str(),
				int2HexStr(calsig_K).c_str(),NULL);
		out<<"  "<<FuncCall<<endl;
	}

	void OutInitStmt(ofstream &out)
	{
		for(vector<string>::iterator iter=init_stmt.begin();iter!=init_stmt.end();iter++)
		{
			out<<"  "<<*iter<<endl;
		}
	}

	vector<TAG_IfSig> CalculateOutSig(int CalOutSig1,int CalOutSig2=0)
	{
		vector<TAG_IfSig> VecData;
		TAG_IfSig Data;
		int Prime[2]={P1,P2};
		int TwoPower2KMod[2]={0};
		TwoPower2KMod[0]=(K_NUM_P1%P1*K_NUM%P1)%P1;
		TwoPower2KMod[1]=(K_NUM_P2%P2*K_NUM%P2)%P2;
		for(int i=0;i<2;i++)
		{
			long long R=((long long)CalOutSig1+(long long)CalOutSig2)%Prime[i];
			Data.S1=((long long)calsig_K*R)%Prime[i];
			Data.S2=(((long long)TwoPower2KMod[i]+R)%Prime[i] * (long long)calsig_K%Prime[i])%Prime[i];

			VecData.push_back(Data);
		}
		return VecData;
	}

public:
	BeforeIf(int __diffsig,int __k,	string __leftvar,string __rightvar,
			string __funcname="F_VCL_CalIfSig_GreatThan",
			string __diff1name="Diff"
			):out_sig_func_name(__funcname),diff1_name(__diff1name),diff_sig(__diffsig),calsig_K(__k),
			left_var(__leftvar),right_var(__rightvar)
	{
		init_stmt.push_back(string("T_CODE Diff,IfOutVar;"));
		init_stmt.push_back(string("INT32 AdjTmpVar=0;"));
		init_stmt.push_back(string("Diff.Data=F_VCL_Sub("+left_var+","+right_var+",inSubComp1,inSubComp2);//Diff's sig"));
	}

	string GetCmpVarName()
	{
		return diff1_name;
	}

	virtual vector<TAG_IfSig> GetOutSig()
	{
		return CalculateOutSig(diff_sig);
	}

	virtual void Output_BeforeIf(ofstream &out)
	{
		OutInitStmt(out);
		Output_OutSigCalFunc(out,diff1_name);
	}

	virtual ~BeforeIf()
	{}
};

class ExtraVarBeforeIf:public BeforeIf
{
protected:
	string diff2_name;
	int diff2_sig;

	void GetSigAdj(int *Adj)
	{
		int Prime[2]={P1,P2};
		int Sig_const_0=getConstantSig(0);
		for(int i=0;i<2;i++)
		{
			int pre_sig=((long long)Sig_const_0-diff_sig+Prime[i])%Prime[i];
			Adj[i]=((long long)diff2_sig-pre_sig+Prime[i])%Prime[i];
		}
	}

	void Output_CalDiff_2(ofstream &out)
	{
		constant2Str(out,0);
		out<<"  "<<"T_CODE "<<diff2_name+";"<<endl;

		int AdjSig[2]={0,0};
		GetSigAdj(AdjSig);
		string str_0=CONST_NUM_BIND_PREFIX+int2HexStr_data(0);
		string left_var=diff2_name+VAR_TDATA;
		string FuncCall=BuildOutputFunc_str(left_var,string("F_VCL_Sub"),
				string(str_0+VAR_TDATA).c_str(),string(diff1_name+VAR_TDATA).c_str(),
				int2HexStr(AdjSig[0]).c_str(),int2HexStr(AdjSig[1]).c_str(),NULL);
		out<<"  "<<FuncCall<<endl;
	}
public:
	ExtraVarBeforeIf(int __diffsig,int __diff2sig,int __k,string __leftvar,	string __rightvar,
				string __funcname="F_VCL_CalIfSig_GreatThan",
				string __diff1name="Diff",
				string __diff2name="Diff2"
				):BeforeIf(__diffsig,__k,__leftvar,__rightvar,__funcname),
				diff2_name(__diff2name),diff2_sig(__diff2sig){}

	vector<TAG_IfSig> GetOutSig()
	{
		return CalculateOutSig(diff2_sig);
	}
	void Output_BeforeIf(ofstream &out)
	{
		OutInitStmt(out);
		Output_CalDiff_2(out);
		Output_OutSigCalFunc(out,diff2_name);
	}
};

class EqualCmpBeforIf:public ExtraVarBeforeIf
{
public:
	EqualCmpBeforIf(int __diffsig,int __diff2sig,int __k,string __leftvar,string __rightvar,
				string __funcname="F_VCL_CalExitSig",
				string __diff1name="Diff",
				string __diff2name="Diff2"
				):ExtraVarBeforeIf(__diffsig,__diff2sig,__k,__leftvar,__rightvar,__funcname){}

	virtual vector<TAG_IfSig> GetOutSig()
	{
		return CalculateOutSig(diff_sig,diff2_sig);
	}

	void Output_BeforeIf(ofstream &out)
	{
		OutInitStmt(out);
		Output_CalDiff_2(out);
		Output_OutSigCalFunc(out,diff1_name,diff2_name);
	}
};

class BranchStmt
{
private:
	string bool_str;//"TRUE" or "FALSE"

	int left_var_sig;
	string left_var_name;

	int bool_sig;
	string bool_var_name;//"AC_Bind_..."

	vector<string> bool_var_assign;

	void Output_BoolVarAssign(ofstream &out)
	{
		for(int i=0;i<bool_var_assign.size();i++)
		{
			out<<"    "<<bool_var_assign[i]<<endl;
		}
	}

	void Output_varAssign(ofstream &out)
	{
		int sig_0=getConstantSig(0);
		int sig_adj_1=((long long)left_var_sig-bool_sig-sig_0+P1)%P1;
		int sig_adj_2=((long long)left_var_sig-bool_sig-sig_0+P2)%P2;

		string FuncCall=BuildOutputFunc_str(string(left_var_name+VAR_TDATA),"F_VCL_Assign",
				string(bool_var_name+VAR_TDATA).c_str(),int2HexStr(sig_adj_1).c_str(),int2HexStr(sig_adj_2).c_str(),NULL);
		out<<"    "<<FuncCall<<endl;
	}
public:
	BranchStmt(string __bool_name,int __boolsig,int __leftvar_sig,string __left_var="IfOutVar"):
		bool_str(__bool_name),bool_sig(__boolsig),left_var_sig(__leftvar_sig),left_var_name(__left_var),
		bool_var_name(CONST_BOOL_BIND_PREFIX+bool_str)
		{
			string bind_str=CONST_BOOL_BIND_PREFIX+bool_str;
			string const_str=CONST_BOOL_PREFIX+bool_str;
			bool_var_assign.push_back(bind_str+".Data.F"+" = "+const_str+".Data.F"+";");
			bool_var_assign.push_back(bind_str+".Data.C.C1"+" = "+const_str+".Data.C.C1 + g_GS_TL1;");
			bool_var_assign.push_back(bind_str+".Data.C.C2"+" = "+const_str+".Data.C.C2 + g_GS_TL2;");
		}

	void OutputBranch(ofstream &out)
	{
		Output_BoolVarAssign(out);
		Output_varAssign(out);
	}

};

class ElseAdj
{
private:
	string adj_var_name;

	vector<TAG_IfSig> If_Sig;

	int left_var_sig1;
	int left_var_sig2;

	void CalSigAdj(int *adj)
	{
		int Prime[2]={P1,P2};
		for(int i=0;i<2;i++)
		{
			int left_var_adj=((long long)left_var_sig1-left_var_sig2+Prime[i])%Prime[i];
			int if_s1s2_diff=((long long)If_Sig[i].S1-If_Sig[i].S2+Prime[i])%Prime[i];
			adj[i]=((long long)left_var_adj+if_s1s2_diff)%Prime[i];
		}
	}
public:
	ElseAdj(vector<TAG_IfSig> &__vec_ifsig,int __sig1,int __sig2,string __name="IfOutVar"):
		If_Sig(__vec_ifsig),left_var_sig1(__sig1),left_var_sig2(__sig2),adj_var_name(__name){}

	void Output_ElseAdj(ofstream &out)
	{
		int SigAdj[2];
		CalSigAdj(SigAdj);

		string left_var_with_check[2]={adj_var_name+VAR_TDATA+VAR_CHK_NAME_1,adj_var_name+VAR_TDATA+VAR_CHK_NAME_2};
		string PrimeStr[2]={"P1","P2"};

		for(int i=0;i<2;i++)
		{
			string FuncCall=BuildOutputFunc_str(left_var_with_check[i],LongInt_FunAdd,
					left_var_with_check[i].c_str(),int2HexStr(SigAdj[i]).c_str(),PrimeStr[i].c_str(),NULL);
			out<<"    "<<FuncCall<<endl;
		}
	}
};
//0->=
//1-<
//2-<=
//3->
//4-==
//5-!=
class RelaCmpBuild
{
private:
	int RelaCmpCode;

	string FuncName;
	string ITNName;
	string CmpSymbol;

	BeforeIf *before_if_build;
	vector<BranchStmt> branch_stmt;
	ElseAdj *else_adj;

	static vector<string> out_if_stmt;
public:
	RelaCmpBuild(int __code):RelaCmpCode(__code)
	{
		FuncName=FUNC_REL_CMP_CODED[RelaCmpCode];
		CmpSymbol=RemCmpSymbol[RelaCmpCode];

		ITNName="(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)";

		if(RelaCmpCode<2)
			before_if_build=new BeforeIf(Diff1Sig[RelaCmpCode],CalOutSigK[RelaCmpCode],"inX","inY");
		else if(RelaCmpCode>=4)
			before_if_build=new EqualCmpBeforIf(Diff1Sig[RelaCmpCode],Diff2Sig[RelaCmpCode],CalOutSigK[RelaCmpCode],"inX","inY");
		else
			before_if_build=new BeforeIf(Diff1Sig[RelaCmpCode],CalOutSigK[RelaCmpCode],"inY","inX");

		if(RelaCmpCode%2==0)
		{
			branch_stmt.push_back(BranchStmt("TRUE",Bool_True_Sig,VarSig1[RelaCmpCode]));
			branch_stmt.push_back(BranchStmt("FALSE",Bool_False_Sig,VarSig2[RelaCmpCode]));
		}
		else
		{
			branch_stmt.push_back(BranchStmt("FALSE",Bool_False_Sig,VarSig1[RelaCmpCode]));
			branch_stmt.push_back(BranchStmt("TRUE",Bool_True_Sig,VarSig2[RelaCmpCode]));
		}

		vector<TAG_IfSig> vec_sig=before_if_build->GetOutSig();
		else_adj=new ElseAdj(vec_sig,VarSig1[RelaCmpCode],VarSig2[RelaCmpCode]);
	}

	void OutputRelaCmp_Impl(ofstream &out)
	{
		out<<"T_DATA "<<FuncName<<ITNName<<endl;
		out<<"{"<<endl;

		before_if_build->Output_BeforeIf(out);

		out<<"  "<<"if("<<before_if_build->GetCmpVarName()+VAR_TDATA+VAR_DATA_NAME<<" "+CmpSymbol+" "<<"0)"<<endl;
		out<<"  "<<"{"<<endl;
		branch_stmt[0].OutputBranch(out);
		out<<"  "<<"}"<<endl;
		out<<"  "<<"else"<<endl;
		out<<"  "<<"{"<<endl;
		branch_stmt[1].OutputBranch(out);
		out<<endl;
		else_adj->Output_ElseAdj(out);

		out<<"  "<<"}"<<endl;

		for(vector<string>::iterator iter=out_if_stmt.begin();iter!=out_if_stmt.end();iter++)
		{
			out<<"  "<<*iter<<endl;
		}
		out<<"}"<<endl;
	}

	static void InitOutIfStmt();

	~RelaCmpBuild()
	{
		delete before_if_build;
		delete else_adj;
	}
};
vector<string> RelaCmpBuild::out_if_stmt;

void RelaCmpBuild::InitOutIfStmt()
{
	out_if_stmt.push_back(string("AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);"));
	out_if_stmt.push_back(string("IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);"));
	out_if_stmt.push_back(string("AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);"));
	out_if_stmt.push_back(string("IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);"));
	out_if_stmt.push_back(string("return IfOutVar.Data;"));
}

ofstream& operator<<(ofstream &out,RelaCmpBuild &rmp_build)
{
	rmp_build.OutputRelaCmp_Impl(out);
	return out;
}


void output_relacmp_impl(const string &StrOutFile)
{
	RelaCmpBuild::InitOutIfStmt();
	ofstream outfile(StrOutFile.c_str(),ios::app);
	RelaCmpBuild *cur_build=NULL;
	for(int i=0;i<6;i++)
	{
		cur_build=new RelaCmpBuild(i);
		outfile<<*cur_build;
		delete cur_build;
	}

	outfile.close();
}



