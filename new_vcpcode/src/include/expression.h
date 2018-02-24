#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "common.h"

/* a class used to store the expression */
class Expression
{	
	private :
		string op_;//store the operator
		Expression* exp_;//when it is like "(exp)",store the exp in the "( )"
		Expression* cel_;//store the expression on the left of an operator
		Expression* cer_;//store the expression on the right of an operator
		string c_;//store the constant or the function name
		Var* cvar_;//store the variable
		list<Expression>* args_;//store the arguments when it is a function calling expression
		int stamp_;//the stamp used to mark the kind of expression
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//表达式的伪行号，2010年3月10日追加
		
	public:
		Expression();//default,stamp_=-1
		explicit Expression(Expression*,string,Expression*);//when the expression is like "expl op expr",stamp_=0 
		explicit Expression(string,Expression*);//when the expression is like "op expr",stamp_=1
		explicit Expression(Expression*);//when the expression is like "(exp)",stamp_=2 
		explicit Expression(string);//when the expression is a constant,stamp_=3
		explicit Expression(Var*);//*when the expression is a variable,stamp_=4
		explicit Expression(string,list<Expression>*);//when the expression is a function calling,stamp_=5
		
		//Expression(const Expression&);//copy constructor function
		
		string get_op();//get the	operator
		Expression* get_exp();//get the expression in the "()"
		Expression* get_cel();//get the expression on the left of an operator
		Expression* get_cer();//get the expression on the right of an operator
		string get_c();//get the constant  or the function name
		Var* get_cvar();//get the variable 
		list<Expression>* get_args();//get the arguments in the function
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//获取表达式的伪行号，2010年3月10日追加
		
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_cer(Expression*);//set the expression on the right of an operator
		void set_cel(Expression*);//set the expression on the left of an operator
		void set_cvar(Var*);//set the variable
		void set_c(string);//set the constant  or the function name
		void set_stamp(int );//set the stamp
		void set_op(string);//set the oprator
		void set_exp(Expression*);//set the expression in the "()"
		void set_args(list<Expression>*);//set the arguments of the function
		void set_fake_line(int );//设置该表达式的伪行号，2010年3月10日追加
};

#endif

