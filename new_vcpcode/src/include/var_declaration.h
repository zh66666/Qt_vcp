#ifndef VAR_DECLARATION_H_
#define VAR_DECLARATION_H_

#include "common.h"

/* a class used to store the variable declaration*/
class Var_declaration
{	
	private:
 		Declaration_specifiers* decl_spec_;//store the declaration specifiers
 		list<Init_declarator>* inits_;//store the inition declarators
		int stamp_;//a stamp used to mark the kind of declaration
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int noadj_stamp_;	//����Ƿ��ڱ���ʱ��Ҫ�Ըñ�������ǩ������
										//Ϊ1��˵������Ҫ��Ϊ0˵����Ҫ����ʼ��Ϊ-1
		int bool_stamp_;	//��Ǹñ����Ƿ��ǲ�������
										//Ϊ0��˵�����ǲ�������Ϊ1˵���ǲ���������ʼ��Ϊ-1
		int nodt_stamp_;	//��Ǳ���ʱ�Ƿ���Ҫ��DT
										//Ϊ1��˵������Ҫ��Ϊ0˵����Ҫ����ʼ��Ϊ-1
		string file_info_;//�������������ļ�����,2010��3��10��׷��
		int fake_line_;//�ñ���������չ�����ļ��е�α�кţ�2010��3��10��׷��
		
	public:
		Var_declaration();//default,stamp_=-1
		
		//when only has the declaration specifiers��stamp_=0
		explicit Var_declaration(Declaration_specifiers*);
		
		//when has the declaration specifiers and inition declarators both,stamp_=1
		explicit Var_declaration(Declaration_specifiers*,list<Init_declarator>*);
		
		
		Declaration_specifiers* get_decl_spec();//get the declaration specifiers
		list<Init_declarator>* get_inits();//get the inition declarators
		int get_stamp();//get the stamp
		int get_noadj_stamp();
		int get_bool_stamp();
		int get_nodt_stamp();
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		string get_file_info();//��ȡ�ñ����������ڵ��ļ����ƣ�2010��3��10��׷��
		int get_fake_line();//��ȡ�ñ���������α�кţ�2010��3��10��׷��
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_noadj_stamp(int);
		void set_nodt_stamp(int);
		void set_bool_stamp(int);
		void set_stamp(int);//set the stamp
		void set_file_info(string);//���ñ����������ڵ��ļ����ƣ�2010��3��10��׷��
		void set_fake_line(int);//���øñ���������α�кţ�2010��3��10��׷��
		
};

#endif
