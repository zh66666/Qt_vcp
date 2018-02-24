/*
 * TmpVarIndexRange.h
 *
 *  Created on: 2014��12��15��
 *      Author: LordVoldemort
 *  ��ʱ���������±귶Χ�������Ͷ���
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
	/* ������еı�ʾ						   |     ��������еı�����ʾ
	 * 0-�з�������ͨ������Χ                                       |
	 * 1-�޷�������ͨ������Χ                                      |    0-INT32��ʱ������Χ
	 * 2-If����߼����㷶Χ                                          |    1-��֧����ǩ��������Χ
	 * 3-If����߼�ֵ��ȥTRUE�Ĳ�ֵ������Χ            |    2-ѭ����ǩ��������Χ
	 * 4-while����߼����㷶Χ                                    |    3-ѭ������ǩ��������Χ
	 * 5-while����߼�ֵ��ȥTRUE�Ĳ�ֵ������Χ      |    4-While_A/B_1/2_  ��Χ
	 * 6-��ϵ�Ƚ�����Ľ����Χ                                    |
	 * 7-�޷���ֵ������ʱ������Χ                                |
	 * 8-While_A�ķ�Χ                 				   |
	 * 9-While_B�ķ�Χ						   |
	 */
};





#endif /* SRC_INCLUDE_TMPVARINDEXRANGE_H_ */
