/*
 * gen_secondary.cpp

 *
 *  Created on: 2014��12��26��
 *      Author: LordVoldemort
 *  ������ʱ���ɵ�������룬��������
 *  ����Ķ������ɣ��õ������������
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

static vector<StmtSet> SecondAddStmt;//������ӵ����

int SecondAddIndex;//�������������

//�����伯��
//����伯��,������
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

//��������������
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

//���������ַ������֮ǰ����ʱ����˳����������
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

//��ʼ������ʱ��������ʱ�����ļ�����
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

//�����������������ʱ����ʶ����ʱ��������λ�õ������ַ�
void OutCodedTmpSpecStr(SpecStrId *__TmpVarId)
{
	for(int i=0;i<SPEC_STR_ID_STYLE;i++)
	{
		out_indent(indent_num,gen_out);
		gen_out<<SECOND_ADD_SPEC_STR+int2str(SecondAddIndex)<<endl;
		__TmpVarId->TmpId[i]=SecondAddIndex++;
	}
}

//�����������õ�����ʱ�������������
void BuildCodedTmpVarDeclrStmtSet(SpecStrId *__SpecId,TFuncTmpRange *__TmpIdRange)
{
	StmtSet DeclStmtSet;
	string TmpVarStyle[SPEC_STR_ID_STYLE]={"INT32","T_CHECK","T_CHECK","T_CHECK","T_CHECK"};
	string VarNamePrefix[SPEC_STR_ID_STYLE-1]={INT32_TMPVAR_PREIFX,IF_SELECT_OUTSIG_PREFIX,
											WHILE_BODYSIG_PREFIX,WHILE_OUTSIG_PREFIX};
	string VarInitData[SPEC_STR_ID_STYLE-1]={" = 0"," = {0, 0}"," = {0, 0}"," = {0, 0}"};
	//AC_whileA/B��ʱ������������⴦��
	string While_AB_Prefix[2]={::WHILE_AC_A_PRE,::WHILE_AC_B_PRE};

	string LineStr; //�������
	string TypeDeclStr; //���������ַ���
	int TmpVar_i=0;
	int	Var_Cnt=0,Var_GenNum=0;//��ʱ��������������ʱ��������
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

