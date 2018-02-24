/*
 * gen_secondary.h
 *
 *  Created on: 2014��12��26��
 *      Author: LordVoldemort
 *   ������ʱ���ɵ�������룬��������
 *  ����Ķ������ɣ��õ������������
 */

#ifndef SRC_INCLUDE_GEN_SECONDARY_H_
#define SRC_INCLUDE_GEN_SECONDARY_H_
#include "CodedVarBuild_Util.h"
#include "common.h"
#include "TmpVarIndexRange.h"

const int SPEC_STR_ID_STYLE=5;
const int COL_LEN_VAR_DECL=4;

struct SpecStrId
{
	int TmpId[SPEC_STR_ID_STYLE];
	/*
	 * ��������еı�����ʾ
	 * 0-INT32��ʱ������Χ
	 * 1-��֧����ǩ��������Χ
	 * 2-ѭ����ǩ��������Χ
	 * 3-ѭ������ǩ��������Χ
	 * 4-While_A/B_1/2_ ��Χ
	 */
};

typedef vector<string> StmtSet;
extern int SecondAddIndex;//�������������

const string SECOND_ADD_SPEC_STR=":|Second Add Stmt:";
const int MAXCHAR=1024;

string OutIndentStr(int indentNum);
void AddNewStmtSet(StmtSet &New_Stmt,int __Index);
void Second_Output_File(string __OutputPath,string __InputPath);
void InitCodedTmpVarCnt();
void InitCodedTmpIndex(TFuncTmpRange *__TmpRange);
void GetCodedTmpIndex(TFuncTmpRange *__TmpRange,TValueRange &WhileABRange);
void OutCodedTmpSpecStr(SpecStrId *__TmpVarId);
void BuildCodedTmpVarDeclrStmtSet(SpecStrId *__SpecId,TFuncTmpRange *__TmpIdRange);

#endif /* SRC_INCLUDE_GEN_SECONDARY_H_ */
