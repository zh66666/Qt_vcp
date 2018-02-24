#ifndef DECLARATION_SPECIFIERS_H_
#define DECLARATION_SPECIFIERS_H_

#include "common.h"

/* a class used to store the declaration specifiers*/
class Declaration_specifiers
{
	private:
		string stor_spec_;//store the storage specifier
		list<Type_specifier>* type_spec_list_;//store the type specifiers
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//���������η���α�кţ�2010��3��10��׷��
	public:
		Declaration_specifiers();//default
		explicit Declaration_specifiers(string,list<Type_specifier>*);
		
		string get_stor_spec();//get the storage specifier
		list<Type_specifier>* get_type_spec_list();//get the type specifiers
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//��ȡ���������η���α�кţ�2010��3��10��׷��
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_fake_line(int);//���ø��������η���α�кţ�2010��3��10��׷��
};

#endif

