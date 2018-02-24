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
//ǩ����Ϣ����
extern int S_last,S_this;
extern vector<int> sig_vector;
extern vector<int>::iterator iter;
extern int idx;
extern vector<int> sig_vector_select;
extern vector<int>::iterator iter_select;
extern int idx_select;
// �������ļ� : ./gen_utils.cpp
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


// �������ļ� : src/main_utils.cpp
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
extern map<int,int> constantMap;//���ڴ�����������ǩ��  ���
extern void constant2Str(const int);
extern void bool2Str(ofstream &out,bool inbool);
extern string constantBindedStr(const int);

//�������ļ�gen_var_dec.cpp �ж��Ƿ���IO����
extern bool IsIOArrayName(string ArrayName);

string GetArrayNameInCSC(string ArrayName);

/*****************ʱ���ǩ����*********************************/
extern int DeltaTimeStamp;
/*************************************************************/

/*****************2014-12��Ӵ�������¼����*********************/
const int P1=12970357;
const int P2=12239417;
/**************************************************************/
/*************************����ֵ����****************************/

const int TRUE_VALUE=0x00A2E527;
const int FALSE_VALUE=0xFFBED234;
/**************************************************************/

/****************����ֵǩ����������*****************************/
extern int Bool_True_Sig;
extern int Bool_False_Sig;
/**************************************************************/

/***************��ϵ�������ǩ����������************************/
extern int Diff1Sig[];//��¼��ֵǩ�� Diff1
extern int Diff2Sig[];//��¼��ֵǩ��Diff2
extern int CalOutSigK[];//�����֧����ǩ����Kֵ
extern int VarSig1[];//True��֧���������ǩ��
extern int VarSig2[];//False��֧���������ǩ��

/**************************************************************/
/*****************2014-12������鸽�ӱ�����*********************/
const string ARRAY_EXTR_NAME = "Array_EXTR"; //���鸽�ӱ�������  ����
#endif
