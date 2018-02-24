/*
 * CodedVarBuild_Util.h
 *
 *  Created on: 2014年12月15日
 *      Author: LordVoldemort
 *  各类附加变量命名前缀
 *  临时变量命名函数声明
 */

#ifndef SRC_INCLUDE_CODEDVARBUILD_UTIL_H_
#define SRC_INCLUDE_CODEDVARBUILD_UTIL_H_

#include "common.h"

/******************************语句拆分引入的数据结构********************/
//作用:在拆分、归并语句时，分析语句中的操作符和变量/常量，这些操作符/变量/常量
//按照下列数据结构进行存储
struct SPLIT_VAR
{
	string op_var_name;
	int stamp;//常量-3/变量-4/运算符号-10
};

/**********************************************************************/

/********************语句拆分相关的声明********************************/
const string ARITHM_INTTMP_PRE="AC_Tmp_";//算术运算表达式临时变量名前缀 有符号数
const string ARITHM_UINTTMP_PRE="ACU_Tmp_";//算术运算表达式临时变量名前缀 无符号数
const string IF_TMP_BOOL_PRE="AC_IfTmp_Bool_";//If语句条件表达式拆分中，逻辑运算结果的临时变量前缀
const string WHILE_TMP_BOOL_PRE="AC_WhileTmp_Bool_";//While语句的逻辑运算结果临时变量前缀
const string RELATION_CMP_PRE="AC_RelationTmp_Bool_";//关系比较运算的结果临时变量前缀
const string IF_LOGICSUB_PRE="AC_IfSub_";//If语句逻辑值减去TRUE值的结果
const string WHILE_LOGICSUB_PRE="AC_WhileSub_";//while语句逻辑值减去TRUE值的结果
const string VOID_FUNC_RETVAL="AC_Void_Func_RetVal";//无返回值函数强制添加的返回值变量

const string WHILE_AC_A_PRE="AC_WhileA_";//AC_WHILE_A_1_1变量名前缀
const string WHILE_AC_B_PRE="AC_WhileB_";//AC_WHILE_B_1_1变量名前缀

extern list<string> tmp_var_style;//临时变量类型
extern string CurFuncNameHash;//由函数哈希得到的临时变量前缀

extern string SplitArithmTmpVarPrefix;//拆分表达式时，临时变量前缀，既包括算术运算(包括数组)临时变量，也包括逻辑运算临时变量，但不包括条件表达式的临时变量
extern string SplitCondTmpVarPrefix;//拆分表达式时，条件表达式拆分时临时变量前缀
extern string CondSubVarPrefix;//逻辑值减去TRUE的前缀

extern int IfTmpBoolCnt;//当前函数下分支语句的组合条件表达式的逻辑运算的结果
extern int IfBoolSubCnt;//分支语句减去TRUE值的差值的计数器

extern int WhileTmpBoolCnt;//当前函数下循环语句的组合条件表达式的逻辑运算的结果
extern int WhileBoolSubCnt;//循环语句减去TRUE值的差值的计数器

extern int TmpRelationBoolCnt;//当前函数下分支语句条件判断表达式中，关系运算的布尔值结果计数器

extern int *SplitCondTmpVarCntPtr;//条件表达式临时变量索引计数器指针
extern int *CondSubVarCntPtr;//逻辑值减去TRUE的计数器指针

extern int WhileAC_AB_Cnt;//While语句累加器变量计数器

extern int *SplitArithmTmpVarCntPtr;//算术运算(包括数组)、逻辑运算表达式临时变量计数器指针
extern int ACIntTmpCnt;//算术运算表达式计数器 有符号数
extern int ACUIntTmpCnt;//算术运算表达式计数器 无符号数

extern int void_func_counts;//无返回值函数强制返回的变量名索引值

extern string CurSplitFuncName;//当前拆分语句时的所在函数名


/*************************************************************************/

/************************函数编码临时变量声明*****************************/

const string FUNC_EXTA_PREFIX="AC_df_";

extern int void_func_counts;//无返回值函数强制返回的索引值


/************************************************************************/

/************************分支编码运用的变量声明***************************/
const string IF_SELECT_OUTSIG_PREFIX="AC_IfOutSig_";

extern int SelectOutSigCnt;//分支出口签名计数器


/************************************************************************/

/************************循环编码运用的变量声明***************************/
const string WHILE_OUTSIG_PREFIX="AC_WhileOutSig_";
const string WHILE_BODYSIG_PREFIX="AC_WhileSig_";
const string DELTA_T_NAME="dT";//时间标签增量名称

extern int WhileSigCnt;//循环签名计数器


/************************************************************************/

/***********************INT32临时变量命名*********************************/
const string INT32_TMPVAR_PREIFX="AC_Tmp_";//int 32类型的临时变量前缀
extern int Int32TmpVarCnt;//int 32类型的临时变量计数器



/************************************************************************/

/**********************数据域，校验域名称*********************************/

const string VAR_TDATA=".Data";
const string VAR_CHK_NAME_1=".C.C1";
const string VAR_CHK_NAME_2=".C.C2";
const string VAR_DATA_NAME=".F";
const string VAR_CHK_ID_NAME=".Id";

const string IF_WHILE_SIG_S1=".C1";
const string IF_WHILE_SIG_S2=".C2";

const string TL1_NAME="g_GS_TL1";
const string TL2_NAME="g_GS_TL2";

const string CODED_VAR_PREFIX="TC_";

/************************************************************************/

/**************************编码变量名称***********************************/
const string CONST_NUM_BIND_PREFIX="AC_BIND_CONST_NUM_";
const string CONST_NUM_PREFIX="CONST_NUM_";

const string CONST_BOOL_BIND_PREFIX="AC_BIND_BOOL_";
const string CONST_BOOL_PREFIX="CONST_BOOL_";





/***********************************************************************/

string BuildTmpVarName(string __Prefix,string _FuncHash,int VarStyleCnt);

/*
 * 初始化分支结构中的判断条件变量计数器
 * 初始化临时变量的下标索引
 *
 */
void InitSplitTempVarCnt();


void InitIfWhileCodeTmpCnt();

//获取表达式变量是否为有符号 true-有符号 false-无符号
bool get_tmpvar_style(Var *var);

//获取函数实参表达式是否为有符号 true-有符号 false-无符号
bool get_args_style(Expression *s);

//表达式拆分(不包括赋值表达式)时临时变量构建工具
//包括类型：函数实参、函数返回值
void set_tmpvar_split_utils_exp(Expression *s);

//算术运算拆分时临时变量构建工具
void set_tmpvar_split_utils_arithm(Var *var);

//分支条件表达式拆分时临时变量构建工具
void set_tmpvar_split_utils_select();

//循环条件表达式拆分时临时变量构建
void set_tmpvar_split_utils_while();


#endif /* SRC_INCLUDE_CODEDVARBUILD_UTIL_H_ */
