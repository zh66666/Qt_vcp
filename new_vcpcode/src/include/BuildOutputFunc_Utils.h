/*
 * BuildOutputFunc_Utils.h
 *
 *  Created on: 2014��12��19��
 *      Author: LordVoldemort
 *	�������������ʽ
 *	���磺AC_64bit_Add(x,y,c)
 *	F_VCL_CalIfSig_EQU(AC_IfSub_1.Data,AC_IfSub_2.Data,CONK1)����ҪԤ���������߼���ĺ���
 */

#ifndef SRC_INCLUDE_BUILDOUTPUTFUNC_UTILS_H_
#define SRC_INCLUDE_BUILDOUTPUTFUNC_UTILS_H_

#include "common.h"

const string LongInt_FunAdd="AC_64bit_Add";//64λ�ӷ�����
const string LongInt_FunSub="AC_64bit_Sub";//64λ��������

const string FUNC_OUTSIG="F_VCL_CalExitSig";//���߼����֧��ѭ������ǩ���ĺ�������
const string FUNC_WHILE_BODYSIG="F_VCL_CalBodySig";//���߼���ѭ����ǩ���ĺ�������

const string FUNC_REL_CMP_CODED[6]={"F_VCL_GreaterThanOrEqual","F_VCL_LessThan",
									"F_VCL_LessThanOrEqual","F_VCL_GreaterThan",
									"F_VCL_Equal","F_VCL_NotEqual"};
const string RELATIONSHIP[6]={">=","<","<=",">","==","!="};

const string SIG_REG_ITF="F_VCL_RegisterCodeId";

const string FUNC_ARRAYEXT_CHK_UPDATE="F_VCL_ArrayCheckUpdate"; //���鸽�ӱ���У�������
const string FUNC_ARRAYEXT_SIG_UPDATE="F_VCL_ArrayExtUpdate";//���鸽�ӱ���ǩ��Id����

//����λ��./main_utils.cpp
//�ɱ����������������������ַ���
//����1��������
//�м������ʵ������
//�Կ��ַ���string() ����
string BuildOutputFunc_str(string LeftVar,string FuncName,...);

//����λ��./main_utils.cpp
//�ɱ����������������������ַ���
//����1��������
//����2�������
//����3��ģ������
//�м������������������
string BuildOperationExp_str(string& LeftVar,const string& Op,string& Mod,...);

//����λ��main_utils.cpp
//����ǩ��Id�������
string BuildSigIdAdjust_str(string LeftVar,int sig_id);




#endif /* SRC_INCLUDE_BUILDOUTPUTFUNC_UTILS_H_ */
