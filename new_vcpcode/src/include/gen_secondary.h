/*
 * gen_secondary.h
 *
 *  Created on: 2014年12月26日
 *      Author: LordVoldemort
 *   根据临时生成的冗余代码，进行冗余
 *  代码的二次生成，得到最终冗余代码
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
	 * 编码过程中的变量表示
	 * 0-INT32临时变量范围
	 * 1-分支出口签名索引范围
	 * 2-循环体签名索引范围
	 * 3-循环出口签名索引范围
	 * 4-While_A/B_1/2_ 范围
	 */
};

typedef vector<string> StmtSet;
extern int SecondAddIndex;//二次添加索引号

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
