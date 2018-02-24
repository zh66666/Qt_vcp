#include <string>
#include <iostream>
#include "program.h"
#include "var_info.h"
#include <fstream>
#include <cstdlib>
#include "app_utils.h"
#include "new_Siginfo.h"
using namespace std;

// �������ļ�: ./front_end/parser_utils.cpp
extern void front_end(string,Program*&);

// �������ļ� : ./back_end/back_end_utils.cpp
extern void back_end(Program*&);

// �������ļ�: ./create_extra/create_extra_files.cpp
extern void create_extra_files(void);

// �������ļ���./get_config_info.cpp
extern void get_config_info(void);

//������./main_utils.cpp
extern void Tranfile(string strFileCopeIn, string strFileCopeOut);

//������./resolve_file_name.cpp
extern string resolve_file_name(string& path_name);

//io
extern void gen_io(string InputFile, string SrcFile, string HeaderFile);


// �������ļ� : ./main_utils.cpp
extern int line_num;
extern int macro_label;
extern bool debug_flag;
extern list<Var_info>* var_infos;
extern ifstream fin;
extern string red_path;
extern string sig_list_file;
extern string enter_fun_name;
extern string current_file_name;
extern string bool_config_file;
extern string sig_config_file;
extern string prime_config_file;	
extern string asm_macro_file;


extern void InitSigInfo();

//2010��3��10��׷��
extern string file_info;
extern int fake_line_num;

extern void out_var_sigs(list<Var_info>* s, string file_name);

// ���ܣ������������main����������������ļ������Լ�����Ĳ�����Ϣ�����ļ�����������룬
//			������ȡ������Ϣ��ǰ�˴�����˴����Լ������ļ�����
// ���������argc-��������
//            argv-��������
// ����ֵ���ɹ��򷵻�0�������򱨴����˳�
// ���ߣ����
int main(int argc ,char *argv[])
{

	//�ж�����ĵ�һ�������Ƿ�Ϊ"-D"
	if("-D"==(string)(argv[1]))
	{
		//����ǣ���˵���û�ѡ�����ģʽ���򽫵��Ա����Ϊ��ֵ
		debug_flag=true;
	}
	else
	{
		//�����Ϊ"-D"����˵��Ϊ����ģʽ�������Ա����Ϊ��ֵ
		debug_flag=false;
	}
	
	//�ڶ���������Ϊ��ں�����
	enter_fun_name=argv[2];
		
	int index=0;//��¼���һ���������ļ��ڲ����е�λ��
	//�������������������һ������
	for(int i=3;i<argc;++i)
	{
		//�������ĳ������Ϊ"-o"����ȡ�����Ĳ�����������������ŵ�·��
		if("-o"==(string)(argv[i]))
		{
			index=i;//�������һ���������ļ��ڲ����е�λ��
			red_path=argv[++i];//������������ļ���ŵ�Ŀ¼
			break;
		}
	}
	
	//�����������Ŀ¼�������������ļ�����·��
	sig_list_file=red_path+sig_list_file;
	
	try
	{
		//��ȡ������Ϣ
		//get_config_info();
		
		//����ǩ���ļ�������
		fin.open((char*)(sig_list_file.c_str()));
		
		//��ʼ��ǩ����Ϣ
		//InitSigInfo();
		Init_Save_SigInfo();

		//�ⲿIO���鴦��
		gen_io(red_path+str_IO_FILE_PATH+str_IO_FILE_NAME,str_IO_TMP_SRC,str_IO_TMP_H);

		//��������ļ����ν��д���
		for(int i=3;i<index;++i)
		{
			string strPathName = string(argv[i]);
			string strFileName = resolve_file_name(strPathName);
			if(strFileName != str_IO_FILE_NAME)
			{
				Program *pro=NULL;											 //�м�ṹָ�룬��ʼ��ΪNULL
				current_file_name=string(argv[i]);					//���õ�ǰ���ڴ�����ļ���
				line_num=1;														//ÿ����һ�����ļ�ʱ���к���1
				fake_line_num=1;												//2010��3��10��׷��
				macro_label=0;												    //ÿ����һ�����ļ�ʱ����������0

				front_end(argv[i],pro);										//ǰ�˴���

				back_end(pro);	  												//��˴���

				cout<<"file : "<<argv[i]<<" redundent coding sucessful!"<<endl;
			}
		}

		fin.close();																//�ر�ǩ���ļ�������
		macro_label=0;														//�����ļ�������ϣ���������0
		create_extra_files();												//���ɸ����ļ�
		
		//����ǵ���ģʽ�������������������ȫ�ֱ�������ʷǩ�����м��ļ�
		if(true==debug_flag)
		{
			out_var_sigs(var_infos,"./temp_result/sig_info/sig_history.txt");
			cout<<"print signature history sucessful!"<<endl;
		}


	}
	catch(string s)
	{
		//������񵽴�����Ϣ���򱨴����˳�����
		//2010��3��10���޸�
		cerr<<"error : when coding file \""<<current_file_name<<"\", on line "<<line_num<<
		" of file "<<file_info<<" : "<<s<<endl;
		exit(-1);
	}
	return 0;
}

