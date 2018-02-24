#include <fstream>
#include <iostream>
#include "program.h"

// �������ļ���src/main_utils.cpp
extern string current_file_name;
extern bool debug_flag;
extern void check_pt_arg(void *pt,string info);
extern string resolve_file_name(string& path_name);

// �������ļ� : ./printer/print_program.cpp
extern void print_program(Program *s, string path_name);

// �������ļ� : ./typedef/replace_typedef.cpp
extern void replace_typedef(Program *s);

// �������ļ� : ./enum/trans_enum.cpp
extern void trans_enum(Program *s);

// �������ļ� : ./struct_array/struct_array_dec.cpp
extern void scan_struct_array_pro(Program *s);

// �������ļ� : ./split_stmts/split_pro.cpp
extern void split_pro(Program *s); 
extern void trans_void_func(Program *s);


// ���ܣ����ζ�ǰ�˴������õ����м�ṹ�����޸ģ����,���ת������
// ���������s-�м�ṹָ��
// ���������s-�м�ṹָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void trans_struct(Program *s)
{
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(s,"program struct 's pointer");
		
		string full_name=current_file_name;					 //�洢��ǰ������ļ���������·����
		string file_name=resolve_file_name(full_name);//�洢�����������ļ���
		
		//����ǵ���ģʽ���򽫹�Լ��ԭʼ����������м��ļ�
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/ori_pro/"+file_name);
			cout<<"print original program successful!"<<endl;
		}
		
		replace_typedef(s);//�滻typedef
		cout<<"repalce typedef successful!"<<endl;//�����ʾ��Ϣ
		
		//����ǵ���ģʽ���򽫾���typedef�滻��Ľ��������м��ļ�
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/replace_typedef/"+file_name);
			cout<<"print program (typedef  replaced) successful!"<<endl;

		}
		
		trans_enum(s);//ת���м�ṹ�е�ö������
		cout<<"transform enum successful!"<<endl;
		
		//����ǵ���ģʽ����enum���������Ľ��������м��ļ�
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/trans_enum/"+file_name);
			cout<<"print program (enum  transformed) successful!"<<endl;
		}
		
		scan_struct_array_pro(s);//����ṹ�������
		cout<<"scan struct&&array successful!"<<endl;

		
		//����ǵ���ģʽ���򽫽ṹ������龭�������Ľ��������м��ļ�
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/trans_array/"+file_name);
			cout<<"print program (array && struct  transformed) successful!"<<endl;
		}
		
		split_pro(s);//��ֽṹ
		cout<<"split the program successful!"<<endl;
		
		//����ǵ���ģʽ���򽫾�����ֺ�Ľ��������м��ļ�
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/split/"+file_name);
			cout<<"print program splited  successful!"<<endl;
		}

		//���޷���ֵ�����������������ǿ���䷵��0�����ڷ�֧ѭ�������ʹ���޷���ֵ����
		//trans_void_func(s);
		cout<<"transtuct void function successful!"<<endl;

	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="transforming program struct : "+str;
		throw error_info;
	}
}




