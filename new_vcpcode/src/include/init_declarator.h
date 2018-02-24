#ifndef INIT_DECLARATOR_H_
#define INIT_DECLARATOR_H_

#include "common.h"

/* a class used to store the init declarator */
class Init_declarator
{
	private:
		Declarator* dec_;//store the declarator
		Initializer* init_;//store the initializer
		int stamp_;//a stamp used to mark the kind of the init declarator
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		
	public:
		Init_declarator();//default,stamp_=-1
		explicit Init_declarator(Declarator*);//only has a declarator,stamp_=0
		explicit Init_declarator(Declarator*,Initializer*);//has both a declarator and an initializer,stamp_=1
		
		Declarator* get_dec();//get the declarator
		Initializer* get_init();//get the initializer
		int get_stamp();//get the stamp 
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
};

#endif

