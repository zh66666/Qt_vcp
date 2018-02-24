#ifndef FUN_DECLARATION_H_
#define FUN_DECLARATION_H_

#include "common.h"

/* a class used to store a functional declaration
   including the statements if it has */
class Fun_declaration
{	
	private:
		Declaration_specifiers* decl_spec_;//store the declaration specifiers
		string fun_name_;//store the function's name
		list<Param>* params_;//store the params
		Compound_stmt* ccstmt_;//store the compound statements if the function has
		int stamp_;//a stamp used to mark the kind of function's declaration
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		string file_info_;//�ú��������������ļ���,2010��3��10��׷��
		int fake_line_;//�ú���������α�кţ�2010��3��10��׷��
		
	public:
		Fun_declaration();//default��stamp_=-1
		
		/*when the function has compound statements��stamp_=0*/
		explicit Fun_declaration(Declaration_specifiers*,string,list<Param>*,Compound_stmt *);
		
		/*when the function has no compound statements��stamp_=1*/
		explicit Fun_declaration(Declaration_specifiers*,string,list<Param>*);
		
		Declaration_specifiers* get_decl_spec();//get the declaration specifiers
		string get_fun_name();//get the function's name
		list<Param>* get_params();//get the params
		Compound_stmt* get_ccstmt();//get the compound statements
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		string  get_file_info();//��øú��������������ļ�����2010��3��10��׷��
		int get_fake_line();//��ȡ�ú���������α�кţ�2010��3��10��׷��
		void set_file_info(string);//���øú��������������ļ���
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_fake_line(int);//���øú���������α�кţ�2010��3��10��׷��
};

#endif

