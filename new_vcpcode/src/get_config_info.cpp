#include "bool_var.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <cstdlib>

// �������ļ� : ./main_utils.cpp
extern string bool_config_file;	
extern string sig_config_file;
extern  string prime_config_file;	
extern int A;
extern string str_A;
extern list<Bool_var>* bool_vars;
extern map<string, int>* pre_sigs;
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);
extern string int2str(int);
extern int str2int(string);

// ���ܣ���ָ���ļ������ļ��ж�ȡԤ����ǩ����������д�뵽ָ����map�С�
// ���������m - ���Ԥ����ǩ���ģ���������ǩ����ֵ�ԣ�������Ϊ�գ�0����
// ���������file_name - ָ�����ļ������Ӵ��ļ��ж�ȡԤ����ǩ����������Ϊ�մ���
// ����ֵ���ɹ�����void��ʧ���׳�������Ϣ
// ���ߣ�������
static void read_config_sig(map<string, int> *m, const string &file_name)
{   
	try
	{
		//�����������ڴ��Ԥ����ǩ����Ϣ��mapָ���Ƿ�Ϊ��
		check_pt_arg(m,"pre-config signature map's pointer ");
		//�����������Ԥ����ǩ�����ļ����ַ����Ƿ�Ϊ��
		check_str_arg(file_name,"pre-config signature file's name ");
		
		 ifstream input(file_name.c_str());  // ����file_nameָ�����ļ����ļ���
		// �ж��ļ����Ƿ�򿪳ɹ�
		if(input == 0)
		{
		  // ����ļ�����ʧ�� 
		  string err_info = "cannot open file: " + file_name;
		  throw err_info;
		}
		else
		{
		  // ����ļ����򿪳ɹ�
		  string line = "";   // ����ļ�������һ��
		  string var_name = "";   // ��������ļ��еı�����
		  int sig = 0;        // ��Ŷ�Ӧ��������Ӧ��ǩ��
		  
		  // �ж��ļ��Ƿ��ȡ���ļ�ĩβ
		  while(std::getline(input, line) != 0)
		  {
			// ����ļ�û�ж���
			std::istringstream words(line);   // ����ļ���һ�У�line����string��
			words >> var_name;	//��������
			words >> sig;				//��ǩ��
			map<string,int>::iterator p = m->find(var_name);  // ָ��map����var_name
																									// Ϊ���ֵ�Ԫ�صĵ�����
			// �ж�map���Ƿ��ҵ���var_nameΪ���ֵ�Ԫ��
			if(p != m->end())
			{
			  // �����map���Ѵ��ڱ�����Ϊvar_name��˵�������ظ�������
			  string err_info = "name duplication on variable: "+ var_name;
			  throw err_info;
			}
			else
			{
			  // map�в�������var_nameΪ���ֵı���������֮��
			  (*m)[var_name] = sig;
			}
		  }
		}
		input.close();    // �ر��ļ���	
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="read config signatures : "+s;
		throw error_info;
	}
}

// ���ܣ���ָ���ļ������ļ��ж�ȡbool���ͱ����ı�������������ֵ���ֵ��
// ���������l - ���bool��Ϣ�Ķ���Bool_var���б�������Ϊ�գ�0����
// ���������file_name - ָ�����ļ������Ӵ��ļ��ж�ȡbool���ͱ�����Ϣ��
//           ������Ϊ�մ���
// ����ֵ���ɹ�����void��ʧ���׳�������Ϣ
// ���ߣ�������
static void read_bool_values(list<Bool_var> *l, const string &file_name)
{
	try
	{
		//�����������ڴ�Ų������ֵ��listָ���Ƿ�Ϊ��
		check_pt_arg(l,"pre-config signature map's pointer ");
		//����������Ų������ֵ���ļ����ַ����Ƿ�Ϊ��
		check_str_arg(file_name,"bool values file's name ");
		
		 ifstream input(file_name.c_str());  // ����file_nameָ�����ļ����ļ���
		// �ж��ļ��Ƿ�򿪳ɹ�
		if(input == 0)
		{
		  // ���ʧ��
		  string err_info = "cannot open file: "+ file_name;
		  throw err_info;
		}
		else
		{
		  // ������ļ��ɹ�
		  string line = "";   				// ����ļ�������һ��
		  string bool_name = "";  // ���bool����������
		  string t_value = "";      	 // �������bool��������ֵ�ַ���
		  string f_value = "";       // �������bool�����ļ�ֵ�ַ���
		  
		  // �ж��Ƿ�����ļ�ĩβ
		  while(std::getline(input, line) != 0)
		  {
			// ���δ���ļ�ĩβ
			std::istringstream words(line);   // ����ļ���һ�У�line����string��
			words >> bool_name;	// ��ȡbool����������
			words >> t_value;		// ��ȡ����bool��������ֵ�ַ���
			words >> f_value;		// ��ȡ����bool�����ļ�ֵ�ַ���
			
			// ��������list������Ƿ��Ѵ�����bool_nameΪ���ֵ�bool����
			for(list<Bool_var>::iterator p = l->begin(); p != l->end(); ++p)
			{
			  // �ж��Ƿ��Ѵ�����bool_nameΪ���ֵ�bool����
			  if(bool_name == p->get_name())
			  { 
				// ������ڣ��򱨴�
				string err_info = "name duplication on variable: "+ bool_name;
				throw err_info;
			  }
			}  
			// ��������ڣ�����뵽list�С�
			Bool_var bv(bool_name, "global",str2int(t_value), str2int(f_value));// ���bool��Ϣ��Bool_var����
			l->push_back(bv);
		  }      
		}
		input.close();
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="read bool values : "+s;
		throw error_info;
	}
}

// ���ܣ���ָ���ļ������ļ��ж�ȡ������A��ֵ��
// ���������file_name - ָ�����ļ������Ӵ��ļ��ж�ȡ��������ֵ��������Ϊ�մ���
// ����ֵ���ɹ����ش�������ֵ��ʧ�����׳�������Ϣ
// ���ߣ�������
static int read_prime_A(const string &file_name)
{ 
	try
	{
		//����������Ŵ�����A���ļ����ַ����Ƿ�Ϊ��
		check_str_arg(file_name,"prime file's name ");
		
		ifstream input(file_name.c_str());	//���������ļ�
		int prime_value = 0;								//���ڴ洢��ȡ��A��ֵ
		
		 // �ж��ļ����Ƿ����ʧ��
		if(input == 0)
		{  
			 // �ļ���ʧ�ܣ��򱨴��˳�
			string err_info = "cannot open file: "+ file_name;
			throw err_info;
		}
		else
		{
			  // �ļ��ɹ��򿪣����ȡ����
		  string line = ""; //���ڴ�Ŷ�ȡ�����е�����
			
			  // ��ȡ�������Ƿ�ɹ�ͬʱ�����Ƿ�������
		  if(std::getline(input, line) != 0 && !line.empty())
		  {
			// �ж�ȡ�ɹ����������ݣ��򱣴��ļ��е�ֵ��prime_value
			std::istringstream words(line);	//�������������ݵ��ַ���
			
			// �ж��ַ�������ȡ���Ƿ�������ֵ
			if(!(words >> prime_value)) //���ַ������ж�����A��ֵ
			{
			  // ���������ʾ��������ֵ
			  string err_info = "format of prime A value in file is invalid!";
			  throw err_info;
			}        
		  }
		  else
		  {
			// �ж�ȡʧ�ܻ��������ݣ������˳�
			string err_info =  "no value in file: " + file_name;
			throw err_info;
		  }
		}
		
		input.close();	//�ر��ļ���
		return prime_value;
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="read prime A : "+s;
		throw error_info;
	}
}

// ���ܣ���ȡ�����е�������Ϣ��������ص�ȫ�ֱ���
// ��������
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
void get_config_info(void)
{
	try
	{
		read_config_sig(pre_sigs,sig_config_file);			//��Ԥ����ǩ����Ϣ
		read_bool_values(bool_vars,bool_config_file);	//������������Ϣ
		A=read_prime_A(prime_config_file);					//������A
		str_A=int2str(A);														//���ô������ַ���������
	}
	catch(string s)
	{
		//2010��3��10���޸�
		cerr<<"error : read config infomation : "<<s<<endl;
		exit(-1);
	}
}


