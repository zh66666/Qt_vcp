/*
 * CodedVarBuild_Util.h
 *
 *  Created on: 2014��12��15��
 *      Author: LordVoldemort
 *  ���฽�ӱ�������ǰ׺
 *  ��ʱ����������������
 */

#ifndef SRC_INCLUDE_CODEDVARBUILD_UTIL_H_
#define SRC_INCLUDE_CODEDVARBUILD_UTIL_H_

#include "common.h"

/******************************�������������ݽṹ********************/
//����:�ڲ�֡��鲢���ʱ����������еĲ������ͱ���/��������Щ������/����/����
//�����������ݽṹ���д洢
struct SPLIT_VAR
{
	string op_var_name;
	int stamp;//����-3/����-4/�������-10
};

/**********************************************************************/

/********************�������ص�����********************************/
const string ARITHM_INTTMP_PRE="AC_Tmp_";//����������ʽ��ʱ������ǰ׺ �з�����
const string ARITHM_UINTTMP_PRE="ACU_Tmp_";//����������ʽ��ʱ������ǰ׺ �޷�����
const string IF_TMP_BOOL_PRE="AC_IfTmp_Bool_";//If����������ʽ����У��߼�����������ʱ����ǰ׺
const string WHILE_TMP_BOOL_PRE="AC_WhileTmp_Bool_";//While�����߼���������ʱ����ǰ׺
const string RELATION_CMP_PRE="AC_RelationTmp_Bool_";//��ϵ�Ƚ�����Ľ����ʱ����ǰ׺
const string IF_LOGICSUB_PRE="AC_IfSub_";//If����߼�ֵ��ȥTRUEֵ�Ľ��
const string WHILE_LOGICSUB_PRE="AC_WhileSub_";//while����߼�ֵ��ȥTRUEֵ�Ľ��
const string VOID_FUNC_RETVAL="AC_Void_Func_RetVal";//�޷���ֵ����ǿ����ӵķ���ֵ����

const string WHILE_AC_A_PRE="AC_WhileA_";//AC_WHILE_A_1_1������ǰ׺
const string WHILE_AC_B_PRE="AC_WhileB_";//AC_WHILE_B_1_1������ǰ׺

extern list<string> tmp_var_style;//��ʱ��������
extern string CurFuncNameHash;//�ɺ�����ϣ�õ�����ʱ����ǰ׺

extern string SplitArithmTmpVarPrefix;//��ֱ��ʽʱ����ʱ����ǰ׺���Ȱ�����������(��������)��ʱ������Ҳ�����߼�������ʱ���������������������ʽ����ʱ����
extern string SplitCondTmpVarPrefix;//��ֱ��ʽʱ���������ʽ���ʱ��ʱ����ǰ׺
extern string CondSubVarPrefix;//�߼�ֵ��ȥTRUE��ǰ׺

extern int IfTmpBoolCnt;//��ǰ�����·�֧��������������ʽ���߼�����Ľ��
extern int IfBoolSubCnt;//��֧����ȥTRUEֵ�Ĳ�ֵ�ļ�����

extern int WhileTmpBoolCnt;//��ǰ������ѭ����������������ʽ���߼�����Ľ��
extern int WhileBoolSubCnt;//ѭ������ȥTRUEֵ�Ĳ�ֵ�ļ�����

extern int TmpRelationBoolCnt;//��ǰ�����·�֧��������жϱ��ʽ�У���ϵ����Ĳ���ֵ���������

extern int *SplitCondTmpVarCntPtr;//�������ʽ��ʱ��������������ָ��
extern int *CondSubVarCntPtr;//�߼�ֵ��ȥTRUE�ļ�����ָ��

extern int WhileAC_AB_Cnt;//While����ۼ�������������

extern int *SplitArithmTmpVarCntPtr;//��������(��������)���߼�������ʽ��ʱ����������ָ��
extern int ACIntTmpCnt;//����������ʽ������ �з�����
extern int ACUIntTmpCnt;//����������ʽ������ �޷�����

extern int void_func_counts;//�޷���ֵ����ǿ�Ʒ��صı���������ֵ

extern string CurSplitFuncName;//��ǰ������ʱ�����ں�����


/*************************************************************************/

/************************����������ʱ��������*****************************/

const string FUNC_EXTA_PREFIX="AC_df_";

extern int void_func_counts;//�޷���ֵ����ǿ�Ʒ��ص�����ֵ


/************************************************************************/

/************************��֧�������õı�������***************************/
const string IF_SELECT_OUTSIG_PREFIX="AC_IfOutSig_";

extern int SelectOutSigCnt;//��֧����ǩ��������


/************************************************************************/

/************************ѭ���������õı�������***************************/
const string WHILE_OUTSIG_PREFIX="AC_WhileOutSig_";
const string WHILE_BODYSIG_PREFIX="AC_WhileSig_";
const string DELTA_T_NAME="dT";//ʱ���ǩ��������

extern int WhileSigCnt;//ѭ��ǩ��������


/************************************************************************/

/***********************INT32��ʱ��������*********************************/
const string INT32_TMPVAR_PREIFX="AC_Tmp_";//int 32���͵���ʱ����ǰ׺
extern int Int32TmpVarCnt;//int 32���͵���ʱ����������



/************************************************************************/

/**********************������У��������*********************************/

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

/**************************�����������***********************************/
const string CONST_NUM_BIND_PREFIX="AC_BIND_CONST_NUM_";
const string CONST_NUM_PREFIX="CONST_NUM_";

const string CONST_BOOL_BIND_PREFIX="AC_BIND_BOOL_";
const string CONST_BOOL_PREFIX="CONST_BOOL_";





/***********************************************************************/

string BuildTmpVarName(string __Prefix,string _FuncHash,int VarStyleCnt);

/*
 * ��ʼ����֧�ṹ�е��ж���������������
 * ��ʼ����ʱ�������±�����
 *
 */
void InitSplitTempVarCnt();


void InitIfWhileCodeTmpCnt();

//��ȡ���ʽ�����Ƿ�Ϊ�з��� true-�з��� false-�޷���
bool get_tmpvar_style(Var *var);

//��ȡ����ʵ�α��ʽ�Ƿ�Ϊ�з��� true-�з��� false-�޷���
bool get_args_style(Expression *s);

//���ʽ���(��������ֵ���ʽ)ʱ��ʱ������������
//�������ͣ�����ʵ�Ρ���������ֵ
void set_tmpvar_split_utils_exp(Expression *s);

//����������ʱ��ʱ������������
void set_tmpvar_split_utils_arithm(Var *var);

//��֧�������ʽ���ʱ��ʱ������������
void set_tmpvar_split_utils_select();

//ѭ���������ʽ���ʱ��ʱ��������
void set_tmpvar_split_utils_while();


#endif /* SRC_INCLUDE_CODEDVARBUILD_UTIL_H_ */
