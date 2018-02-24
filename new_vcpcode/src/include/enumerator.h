#ifndef ENUMERATOR_H_
#define ENUMERATOR_H_

#include "common.h"

/* a class used to store the elements in the enum */
class Enumerator
{
	private:
		string element_;//store the name of the element
		Expression* exp_;//store the expression
		int stamp_;//a stamp used to mark the kind of the enumerator
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		
	public:
		Enumerator();//default,stamp_=-1
		explicit Enumerator(string);//simple element,stamp_=0
		explicit Enumerator(string,Expression*);//element has an expression,stamp_=1
		
		string get_element();//get the name of the element
		Expression* get_exp();//get the expression
		int get_stamp();//get the stamp
		
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number

		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number

};

#endif

