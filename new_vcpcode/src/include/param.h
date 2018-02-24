#ifndef PARAM_H_
#define PARAM_H_

#include "common.h"

/* a class used to store a parameter */
class Param
{
	private:
		list<Type_specifier>* type_spec_list_;//store the type specifiers
		Declarator* dec_;//store the declarator
		int stamp_;//a stamp used to mark the kind of the parameter
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number

	public:
		Param();//default
		explicit Param(list<Type_specifier>*,Declarator*);//stamp_=0
		explicit Param(list<Type_specifier>*);//stamp_=1
		
		list<Type_specifier>* get_type_spec_list();//get the type specifiers
		Declarator* get_dec();//get the declarator
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number

		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		
};

#endif



