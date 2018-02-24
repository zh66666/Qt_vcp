#ifndef VAR_H_
#define VAR_H_

#include "common.h"

/* a class used to store the variable */
class Var
{
	private:
		Var* var_;//store the variable
		string name_;//store the variable's name
		Expression* cexp_;//store the array's subscript (one dimension)
		list<Expression>* exp_list;//when it is an array,store the array subscripts
		int stamp_;//a stamp used to mark what kinds of variable
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//该变量的伪行号，2010年3月10日追加
		list<string> store_var_type;//闪闪 存储变量类型
	public:
		Var();//default,stamp_=-1
		explicit Var(string name,list<string> vartype=list<string>(1,string("INT32")));//when it's a common variable,stamp_=0
		explicit Var(string,list<string>, Expression*);//one dimension array,stamp_=1
		explicit Var(string,list<string>, list<Expression>*);//may be more than one dimension array,stamp_=2
		explicit Var(Var*, string,list<string>);//struct variable,stamp_=3
		explicit Var(Var*,string,list<string>,list<Expression>*);//struct array,stamp_=4

		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//获取该变量的伪行号，2010年3月10日追加
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
    	int get_stamp();//get the stamp
	   	string get_name();//get variable's name
	    Expression* get_cexp();//get the array's subscript when one dimension
	    list<Expression>* get_exp_list();//get the array subscripts
	    list<string> get_var_type();//闪闪
		Var* get_var();//get the variable inside
	    void set_cexp(Expression*);	//set the array subscript
	    void set_stamp(int);//set stamp
		void set_fake_line(int);//设置该变量的伪行号，2010年3月10日追加
		void set_var_type(list<string>);//闪闪

};

#endif
