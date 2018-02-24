#ifndef STRUCT_SPECIFIER_H_
#define STRUCT_SPECIFIER_H_

#include "common.h"

/* a class used to store struct specifier */
class Struct_specifier
{
	private:
		string name_;//store the name of the struct type
		list<Var_declaration>* var_dec_list_;//store the declarations in the struct body
		int stamp_;//a stamp used to mark the kind of the struct specifier
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//该结构体类型标记的伪行号，2010年3月10日追加
	public:
		Struct_specifier();//default,stamp_=-1
		explicit Struct_specifier(string ,list<Var_declaration>*);//has both the name and the struct body,stamp_=0
		explicit Struct_specifier(list<Var_declaration>* );//has only the struct body,stamp_=1
		explicit Struct_specifier(string);//has only the name,stamp_=2
		

		string get_name();//get the name of the struct type
		list<Var_declaration>* get_var_dec_list();//get the declarations in the struct body
		int get_stamp();//get the  stamp 
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//获取该结构体类型标记的伪行号，2010年3月10日追加
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_fake_line(int);//设置该结构体类型标记的伪行号，2010年3月10日追加
};

#endif

