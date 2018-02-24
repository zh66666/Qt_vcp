#ifndef DECLARATOR_H_
#define DECLARATOR_H_

#include "common.h"

/* a class used to store the declarator */
class Declarator
{
	private:
		string name_;//store the declarator'name
		list<Expression>* array_subs_;//store the array declarator's subscripts
		int stamp_;//a stamp used to mark the kind of the declarator
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		
	public:
		Declarator();//default,stamp_=-1
		explicit Declarator(string);//when it is a common declarator,stamp_=0
		explicit Declarator(string,list<Expression>*);//when it is a array declarator,stamp_=1
		
		string get_name();//get the declarator'name
		list<Expression>* get_array_subs();//get the array declarator's subscripts
		int get_stamp();//get the stamp 
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		
		void set_stamp(int);//set the stamp
		void set_array_subs(list<Expression>*);//set the array's sub
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
};

#endif

