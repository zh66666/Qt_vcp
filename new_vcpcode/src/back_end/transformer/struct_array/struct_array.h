#ifndef STRUCT_ARRAY_H_
#define STRUCT_ARRAY_H_

#include "program_struct.h"

//�洢�ṹ���Ա��������Ϣ
struct mem_loc
{
	string name;//��¼�ṹ���Ա����������
	int loc;//��¼��Ա�����ڽṹ���е����λ��
};

//�洢�ṹ�����͵���Ϣ
class Struct_info
{
	private:
		string struct_name_;//�洢�ṹ���������
		list<mem_loc>* members_;//�洢�ṹ��ĳ�Ա��Ϣ
		int num_;//��¼�ṹ���Ա�ĸ���
		string scope_;//��¼�ṹ���������
		
	public:
		//Struct_info();
		Struct_info(string,list<mem_loc>*,int,string);
		
		string get_struct_name();//��ȡ�ṹ��������
		list<mem_loc>* get_members();//��ȡ��Ա��Ϣ����
		int get_num();//��ȡ��Ա����
		string get_scope();//��ȡ������
		
		void set_struct_name(string);//���ýṹ����
		void set_members(list<mem_loc>*);//���ýṹ���Ա��Ϣ����
		void set_num(int);//���ýṹ���Ա����
		void set_scope(string);//���ýṹ���������
};

//�ýṹ�����ڴ洢�ṹ�������Ϣ
struct struct_var
{
	string var_name_;//�洢�ṹ�����������
	Struct_info* struct_info_;//�洢�ṹ��������Ϣ
	string scope_;//��¼�ýṹ�������������
	string stamp_;//�ṹ�������ǩ��Ϣ����¼�ñ��������Ľṹ��������������ñ�����ĳ���ṹ���
							//��Ա����˱�ǩ��ϢΪ�ṹ������ƣ�����Ϊ"common"
};

//���ڴ洢�����������Ϣ
class Array_info
{
	private:
		string array_name_;//�洢�������������
		vector<int>* subs_;//��¼����������±�ֵ
		string scope_;//��¼���������������
		string stamp_;//��¼��������������Ľṹ���������������Ϊ�ṹ���Ա��������Ϊ"common"
	
	public:
		Array_info();
		explicit Array_info(string,vector<int>*);
		
		string get_name();//��ȡ����������
		vector<int>* get_subs();//��ȡ�����±�
		string get_scope();//��ȡ�����������������Ϣ
		string get_stamp();//��ȡ��������ı�ǩ��Ϣ���������Ľṹ��������
		void set_scope(string);//���������������������Ϣ
		void set_stamp(string);//������������ı�ǩ��Ϣ�������Ľṹ��������
};

#endif


