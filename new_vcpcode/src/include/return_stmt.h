#ifndef RETURN_STMT_H_
#define RETURN_STMT_H_

#include "common.h"

/* a class used to store the return statement */
class Return_stmt
{	
	private:
		int stamp_;//the stamp used to mark which kinds of return statement
		Expression* ce_;//when it has a return value,store the return expression
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//该返回语句的伪行号，2010年3月10日追加
	public:
		Return_stmt();//when no return value，stamp_=0
		explicit Return_stmt( Expression *);//when it has a return value，stamp_=1

		Expression* get_ce();//get the return expression
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//获取该返回语句的伪行号，2010年3月10日追加
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_ce(Expression*);//set the return expression
		void set_stamp(int);//set the stamp
		void set_fake_line(int);//设置该返回语句的伪行号，2010年3月10日追加
};

#endif

