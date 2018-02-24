#include <iostream>
#include "program.h"

// �������ļ���src/main_utils.cpp
extern void check_pt_arg(void *pt,string info);

// �������ļ���./scan_enum.cpp
extern void scan_enum(Program *s);  

// �������ļ���./replace_enum.cpp
extern void replace_enumerator(Program *s);

// ���ܣ�ת���м�ṹ�е�ö�����ͣ���ɨ��ö�����ͣ���¼ö�ٳ�Ա�����滻ö�ٳ�Ա
// ���������s-�м�ṹָ��
// ���������s-�м�ṹָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void trans_enum(Program *s)
{
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(s,"program struct 's pointer");
		
		scan_enum(s);									//ɨ�貢��¼ö�ٳ�Ա
		replace_enumerator(s);				   //�滻ö�ٳ�Ա
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="transform enum : "+str;
		throw error_info;
	}
}

