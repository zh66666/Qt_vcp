/*
 * gen_secondary.cpp

 *
 *  Created on: 2014年12月26日
 *      Author: LordVoldemort
 *  根据临时生成的冗余代码，进行冗余
 *  代码的二次生成，得到最终冗余代码
 */
#include <stdio.h>
#include "gen_secondary.h"
char *str_find(char *__str,const char *pattern);
static int GetAddIndex(const char *NumStrPtr);
static void Output_AddStmt(ofstream &out,int __AddIndex);

extern ofstream gen_out;
extern int indent_num;
extern string int2str(int num);
extern void out_indent(int num, ofstream& out);

ofstream FileOut;
ifstream FileIn;

static vector<StmtSet> SecondAddStmt;//二次添加的语句

int SecondAddIndex;//二次添加索引号

//添加语句集合
//新语句集合,索引号
void AddNewStmtSet(StmtSet &New_Stmt,int __Index)
{
	if(__Index>=SecondAddStmt.size())
	{
		int new_size=__Index==0? __Index+1:2*__Index;
		SecondAddStmt.resize(new_size);
	}
	SecondAddStmt[__Index]=New_Stmt;
}

string OutIndentStr(int indentNum)
{
	string indexStr="";
	for(int i=0;i<indentNum;i++)
		indexStr+="  ";
	return indexStr;
}

//二次输出冗余代码
void Second_Output_File(string __OutputPath,string __InputPath)
{
	FileOut.open(__OutputPath.c_str());
	FileIn.open(__InputPath.c_str());

	char LineBuffer[MAXCHAR];
	int AddIndex=0;
	char *SpecStrBeg=NULL;

	while(FileIn.getline(LineBuffer,MAXCHAR))
	{

		if((SpecStrBeg=str_find(LineBuffer,SECOND_ADD_SPEC_STR.c_str()))!=NULL)
		{
			SpecStrBeg+=SECOND_ADD_SPEC_STR.size();
			AddIndex=GetAddIndex(SpecStrBeg);
			Output_AddStmt(FileOut,AddIndex);
		}
		else
				{
					FileOut<<LineBuffer<<endl;
				}
	}

	FileIn.clear();
	FileIn.close();
	FileOut.clear();
	FileOut.close();
}

//根据特殊字符，输出之前编码时不能顺序输出的语句
static void Output_AddStmt(ofstream &out,int __AddIndex)
{
	StmtSet CurAddStmt=SecondAddStmt[__AddIndex];
	for(StmtSet::iterator iter=CurAddStmt.begin();iter!=CurAddStmt.end();iter++)
	{
		out<<*iter<<endl;
	}
}

char *str_find(char *__str,const char *pattern)
{
	const char *strptr=NULL;
	const char *patptr=NULL;
	for(;*__str!='\0';__str++)
	{
		strptr=__str;
		for(patptr=pattern;*patptr!='\0' && *patptr++==*strptr++;);
		if(*patptr=='\0')
		{
			break;
		}
	}

	return *__str=='\0'? NULL:__str;
}

static int GetAddIndex(const char *NumStrPtr)
{
	int index=0;
	sscanf(NumStrPtr,"%d",&index);
	return index;
}

//初始化编码时产生的临时变量的计数器
void InitCodedTmpVarCnt()
{
	Int32TmpVarCnt=0;
	SelectOutSigCnt=0;
	WhileSigCnt=0;
}

void InitCodedTmpIndex(TFuncTmpRange *__TmpRange)
{
	__TmpRange->VarRange[0].V_Start= Int32TmpVarCnt;

	__TmpRange->VarRange[1].V_Start=SelectOutSigCnt;

	__TmpRange->VarRange[2].V_Start=WhileSigCnt;

	__TmpRange->VarRange[3].V_Start=WhileSigCnt;
}

void GetCodedTmpIndex(TFuncTmpRange *__TmpRange,TValueRange &WhileABRange)
{
	__TmpRange->VarRange[0].V_Start+=1;
	__TmpRange->VarRange[0].V_End= Int32TmpVarCnt;

	__TmpRange->VarRange[1].V_Start+=1;
	__TmpRange->VarRange[1].V_End=SelectOutSigCnt;

	__TmpRange->VarRange[2].V_Start+=1;
	__TmpRange->VarRange[2].V_End=WhileSigCnt;

	__TmpRange->VarRange[3].V_Start+=1;
	__TmpRange->VarRange[3].V_End=WhileSigCnt;

	__TmpRange->VarRange[4].V_Start=WhileABRange.V_Start;
	__TmpRange->VarRange[4].V_End=WhileABRange.V_End;
}

//输出二次输出冗余代码时用于识别临时变量声明位置的特殊字符
void OutCodedTmpSpecStr(SpecStrId *__TmpVarId)
{
	for(int i=0;i<SPEC_STR_ID_STYLE;i++)
	{
		out_indent(indent_num,gen_out);
		gen_out<<SECOND_ADD_SPEC_STR+int2str(SecondAddIndex)<<endl;
		__TmpVarId->TmpId[i]=SecondAddIndex++;
	}
}

//构建编码中用到的临时变量的声明语句
void BuildCodedTmpVarDeclrStmtSet(SpecStrId *__SpecId,TFuncTmpRange *__TmpIdRange)
{
	StmtSet DeclStmtSet;
	string TmpVarStyle[SPEC_STR_ID_STYLE]={"INT32","T_CHECK","T_CHECK","T_CHECK","T_CHECK"};
	string VarNamePrefix[SPEC_STR_ID_STYLE-1]={INT32_TMPVAR_PREIFX,IF_SELECT_OUTSIG_PREFIX,
											WHILE_BODYSIG_PREFIX,WHILE_OUTSIG_PREFIX};
	string VarInitData[SPEC_STR_ID_STYLE-1]={" = 0"," = {0, 0}"," = {0, 0}"," = {0, 0}"};
	//AC_whileA/B临时变量的情况特殊处理
	string While_AB_Prefix[2]={::WHILE_AC_A_PRE,::WHILE_AC_B_PRE};

	string LineStr; //声明语句
	string TypeDeclStr; //类型声明字符串
	int TmpVar_i=0;
	int	Var_Cnt=0,Var_GenNum=0;//临时变量计数器，临时变量总数
	for(int style_i=0;style_i<SPEC_STR_ID_STYLE;style_i++)
	{
		TypeDeclStr=OutIndentStr(indent_num+1)+TmpVarStyle[style_i]+" ";
		int TmpNum=__TmpIdRange->VarRange[style_i].V_End-__TmpIdRange->VarRange[style_i].V_Start+1;
		Var_GenNum=style_i!=4? TmpNum:2*TmpNum;
		Var_Cnt=0;
		for(TmpVar_i=__TmpIdRange->VarRange[style_i].V_Start;TmpVar_i<=__TmpIdRange->VarRange[style_i].V_End;TmpVar_i++)
		{
			if(style_i!=4)//Int32 If_Sig While_Sig While_OutSig
			{
				LineStr=TypeDeclStr+VarNamePrefix[style_i]+int2str(TmpVar_i)+VarInitData[style_i]+";";
				DeclStmtSet.push_back(LineStr);
				Var_Cnt++;
			}
			else//While_A/B_
			{
				LineStr=TypeDeclStr+::WHILE_AC_A_PRE+int2str(TmpVar_i)+" = {0, 0};";
				DeclStmtSet.push_back(LineStr);
				LineStr=TypeDeclStr+::WHILE_AC_B_PRE+int2str(TmpVar_i)+" = {0, 0};";
				DeclStmtSet.push_back(LineStr);
			}
		}
		AddNewStmtSet(DeclStmtSet,__SpecId->TmpId[style_i]);
		DeclStmtSet.clear();
	}
}

