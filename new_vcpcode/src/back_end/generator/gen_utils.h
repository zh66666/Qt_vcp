#ifndef GEN_UTILS_H_
#define GEN_UTILS_H_

#include <app_utils.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include "var_info.h"
#include <map>


bool TmpVarJudge(string VarName);
bool ExtraVarJudge(string VarName);
//签名信息变量
extern int S_last,S_this;
extern vector<int> sig_vector;
extern vector<int>::iterator iter;
extern int idx;
extern vector<int> sig_vector_select;
extern vector<int>::iterator iter_select;
extern int idx_select;
// 定义在文件 : ./gen_utils.cpp
extern ofstream gen_out;
extern int iteration_level;
extern int select_level;
extern int dD_num; 
extern long long K_NUM;
extern long long K_NUM_P1;
extern long long K_NUM_P2;
extern long long i_temp_P1;
extern long long i_temp_P2;
extern string var_scope;	

extern int get_sig();
extern int get_sig_index(int *index);
extern void add_new_sig(list<Var_info>*,string,int,string);
extern void add_new_vars(list<Var_info>*,string,int,string,string,int);
extern void AssignCodeSig();
extern void InitSigInfo();


// 定义在文件 : src/main_utils.cpp
extern int A;
extern int macro_label;
extern int indent_num;
extern string str_A;
extern list<Var_info>* var_infos;
extern int  str2int(string);
extern string  int2str(int);
extern string int2HexStr(const int num);
extern void out_indent(int,ofstream&);
extern string OutIndentStr(int indentNum);
extern int get_current_sig(list<Var_info>*,string,string);
extern int get_current_sigIndex(list<Var_info>* s,string str,string scope);
extern void check_str_arg(string arg_name,string info);
extern void check_pt_arg(void *pt,string info);
extern int getConstantSig(int);
extern map<int,int> constantMap;//用于存立即数和其签名  李晨欢
extern void constant2Str(const int);
extern void bool2Str(ofstream &out,bool inbool);
extern string constantBindedStr(const int);

//定义在文件gen_var_dec.cpp 判断是否是IO数组
extern bool IsIOArrayName(string ArrayName);

string GetArrayNameInCSC(string ArrayName);

/*****************时间标签增量*********************************/
extern int DeltaTimeStamp;
/*************************************************************/

/*****************2014-12添加大素数记录区域*********************/
const int P1=12970357;
const int P2=12239417;
/**************************************************************/
/*************************布尔值数据****************************/

const int TRUE_VALUE=0x00A2E527;
const int FALSE_VALUE=0xFFBED234;
/**************************************************************/

/****************布尔值签名声明区域*****************************/
extern int Bool_True_Sig;
extern int Bool_False_Sig;
/**************************************************************/

/***************关系运算编码签名声明区域************************/
extern int Diff1Sig[];//记录差值签名 Diff1
extern int Diff2Sig[];//记录差值签名Diff2
extern int CalOutSigK[];//计算分支出口签名的K值
extern int VarSig1[];//True分支中输出变量签名
extern int VarSig2[];//False分支中输出变量签名

/**************************************************************/
/*****************2014-12添加数组附加变量名*********************/
const string ARRAY_EXTR_NAME = "Array_EXTR"; //数组附加变量名称  闪闪
#endif
