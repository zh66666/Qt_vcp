/*
 * ControlFlow_CodeUtil.h
 *
 *  Created on: 2014��12��18��
 *      Author: LordVoldemort
 *  �����������������ж�����е��жϱ���
 *  ��
 *  �����жϱ�����TRUEֵ��ֵ�ı���֮��Ĺ�ϵ
 *  ���ڷ�֧��ѭ������
 */

#ifndef SRC_INCLUDE_CONTROLFLOW_CODEUTIL_H_
#define SRC_INCLUDE_CONTROLFLOW_CODEUTIL_H_

#include <string>
#include <map>
#include <list>
#include "var.h"
#include "statement.h"

struct CFVarFunc
{
	string VarName;
	string FuncName;
};

struct CFCondVar //�ж���������������
{
	CFVarFunc CondVar;
	friend bool operator<(const CFCondVar &__T1,const CFCondVar &__T2);
};

inline bool operator<(const CFCondVar &__T1,const CFCondVar &__T2)
{
	if(__T1.CondVar.VarName < __T2.CondVar.VarName)
		return true;
	else if(__T1.CondVar.VarName==__T2.CondVar.VarName)
	{
		if(__T1.CondVar.FuncName < __T2.CondVar.FuncName)
			return true;
		else
			return false;
	}
	else
		return false;
}



struct CFSubVar //��ȥTRUE�Ĳ�ֵ����
{
	CFVarFunc SubVar[2];
};
//�ж�������������������Ӧ�Ĳ���ֵ�����Ĳ�ֵ������ ����AC_IfTmpBool_1<--->AC_IfSub_1,AC_IfSub_2
extern map<CFCondVar,CFSubVar> g_CFMap_CondVar_SubVar;
//ѭ���У�ѭ���ж�������������Ӧ��WHILE_A_/WHILE_B��������
extern map<CFCondVar,int> g_WhileMap_CondVar_SumVar;

void Build_CF_CondSub_Var(Var *__CondJudgeVar,list<Statement> *l);


#endif /* SRC_INCLUDE_CONTROLFLOW_CODEUTIL_H_ */
