#ifndef INITIALIZER_H_
#define INITIALIZER_H_

#include "common.h"

/* a class used to store the initializer */
class Initializer
{
	private:
		Expression* exp_;//store the expression which is used  to inition
		list<Initializer>* init_list_;//store the initializers
		int stamp_;//a stamp used to mark the kind of the initializer
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		
	public:
		Initializer();//default,stamp_=-1
		explicit Initializer(Expression*);//when it is an expression,stamp_=0
		explicit Initializer(list<Initializer>*);//when it is an initializer list,stamp_=1
		
		Expression* get_exp();//get the expression
		list<Initializer>* get_init_list();//get the initializers
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number

		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
};

#endif
