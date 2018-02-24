/*
 * TmpVarIndexRange.h
 *
 *  Created on: 2014年12月15日
 *      Author: LordVoldemort
 *  临时变量索引下标范围描述类型定义
 */

#ifndef SRC_INCLUDE_TMPVARINDEXRANGE_H_
#define SRC_INCLUDE_TMPVARINDEXRANGE_H_

#include <map>
#include <string>


const int TMP_VAR_STYLE=10;
struct TValueRange
{
	int V_Start;
	int V_End;
};

struct TFuncTmpRange
{
	TValueRange VarRange[TMP_VAR_STYLE];
	/* 语句拆分中的表示						   |     编码过程中的变量表示
	 * 0-有符号数普通变量范围                                       |
	 * 1-无符号数普通变量范围                                      |    0-INT32临时变量范围
	 * 2-If语句逻辑运算范围                                          |    1-分支出口签名索引范围
	 * 3-If语句逻辑值减去TRUE的差值变量范围            |    2-循环体签名索引范围
	 * 4-while语句逻辑运算范围                                    |    3-循环出口签名索引范围
	 * 5-while语句逻辑值减去TRUE的差值变量范围      |    4-While_A/B_1/2_  范围
	 * 6-关系比较运算的结果范围                                    |
	 * 7-无返回值函数临时变量范围                                |
	 * 8-While_A的范围                 				   |
	 * 9-While_B的范围						   |
	 */
};





#endif /* SRC_INCLUDE_TMPVARINDEXRANGE_H_ */
