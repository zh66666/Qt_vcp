/*
 * ControlFlow_CodeUtil.h
 *
 *  Created on: 2014年12月18日
 *      Author: LordVoldemort
 *  控制流编码中条件判断语句中的判断变量
 *  和
 *  条件判断变量与TRUE值差值的变量之间的关系
 *  用于分支、循环编码
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

struct CFCondVar //判断条件中条件变量
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



struct CFSubVar //减去TRUE的差值变量
{
	CFVarFunc SubVar[2];
};
//判断条件的条件变量名对应的布尔值减法的差值变量名 例如AC_IfTmpBool_1<--->AC_IfSub_1,AC_IfSub_2
extern map<CFCondVar,CFSubVar> g_CFMap_CondVar_SubVar;
//循环中，循环判断条件变量名对应的WHILE_A_/WHILE_B的索引号
extern map<CFCondVar,int> g_WhileMap_CondVar_SumVar;

void Build_CF_CondSub_Var(Var *__CondJudgeVar,list<Statement> *l);


#endif /* SRC_INCLUDE_CONTROLFLOW_CODEUTIL_H_ */
