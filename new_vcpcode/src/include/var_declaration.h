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
		int noadj_stamp_;	//标记是否在编码时需要对该变量进行签名调整
										//为1，说明不需要，为0说明需要，初始化为-1
		int bool_stamp_;	//标记该变量是否是布尔变量
										//为0，说明不是布尔量，为1说明是布尔量，初始化为-1
		int nodt_stamp_;	//标记编码时是否需要加DT
										//为1，说明不需要，为0说明需要，初始化为-1
		string file_info_;//该声明所属的文件名称,2010年3月10日追加
		int fake_line_;//该变量声明在展开后文件中的伪行号，2010年3月10日追加
		
	public:
		Var_declaration();//default,stamp_=-1
		
		//when only has the declaration specifiers，stamp_=0
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
		string get_file_info();//获取该变量声明所在的文件名称，2010年3月10日追加
		int get_fake_line();//获取该变量声明的伪行号，2010年3月10日追加
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_noadj_stamp(int);
		void set_nodt_stamp(int);
		void set_bool_stamp(int);
		void set_stamp(int);//set the stamp
		void set_file_info(string);//设置变量声明所在的文件名称，2010年3月10日追加
		void set_fake_line(int);//设置该变量声明的伪行号，2010年3月10日追加
		
};

#endif
