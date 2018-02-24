/*
 * BuildOutputFunc_Utils.h
 *
 *  Created on: 2014年12月19日
 *      Author: LordVoldemort
 *	构建输出函数形式
 *	例如：AC_64bit_Add(x,y,c)
 *	F_VCL_CalIfSig_EQU(AC_IfSub_1.Data,AC_IfSub_2.Data,CONK1)等需要预编译器在线计算的函数
 */

#ifndef SRC_INCLUDE_BUILDOUTPUTFUNC_UTILS_H_
#define SRC_INCLUDE_BUILDOUTPUTFUNC_UTILS_H_

#include "common.h"

const string LongInt_FunAdd="AC_64bit_Add";//64位加法函数
const string LongInt_FunSub="AC_64bit_Sub";//64位减法函数

const string FUNC_OUTSIG="F_VCL_CalExitSig";//在线计算分支、循环出口签名的函数名称
const string FUNC_WHILE_BODYSIG="F_VCL_CalBodySig";//在线计算循环体签名的函数名称

const string FUNC_REL_CMP_CODED[6]={"F_VCL_GreaterThanOrEqual","F_VCL_LessThan",
									"F_VCL_LessThanOrEqual","F_VCL_GreaterThan",
									"F_VCL_Equal","F_VCL_NotEqual"};
const string RELATIONSHIP[6]={">=","<","<=",">","==","!="};

const string SIG_REG_ITF="F_VCL_RegisterCodeId";

const string FUNC_ARRAYEXT_CHK_UPDATE="F_VCL_ArrayCheckUpdate"; //数组附加变量校验域更新
const string FUNC_ARRAYEXT_SIG_UPDATE="F_VCL_ArrayExtUpdate";//数组附加变量签名Id更新

//定义位置./main_utils.cpp
//可变参数，构建输出函数调用字符串
//参数1：函数名
//中间参数：实参名称
//以空字符串string() 结束
string BuildOutputFunc_str(string LeftVar,string FuncName,...);

//定义位置./main_utils.cpp
//可变参数，构建输出算术运算字符串
//参数1：函数名
//参数2：运算符
//参数3：模数名称
//中间参数：算术运算名称
string BuildOperationExp_str(string& LeftVar,const string& Op,string& Mod,...);

//定义位置main_utils.cpp
//构建签名Id更新语句
string BuildSigIdAdjust_str(string LeftVar,int sig_id);




#endif /* SRC_INCLUDE_BUILDOUTPUTFUNC_UTILS_H_ */
