#ifndef SELECTION_STMT_H_
#define SELECTION_STMT_H_

#include "common.h"

/* a class used to store a selection statement */
class Selection_stmt
{	
	private:
		Expression* ce_;//store the branch expression
		Statement* cs_if_;//store the statements in the if branch
		Statement* cs_else_;//store the statements in the else branch
		int stamp_;//a stamp used to mark what kinds of branch
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//该分支语句的伪行号，2010年3月10日
		
	public:
		Selection_stmt();//default ,stamp_=-1
		explicit Selection_stmt(Expression* ,Statement *);//only one branch，stamp_=0
		explicit Selection_stmt(Expression* ,Statement *,Statement *);//two branches ,if-else，stamp_=1

		Expression* get_ce();//get the branch expression
		Statement* get_cs_if();//get the if statements
		Statement* get_cs_else();//get the else statements
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//获取该分支语句的伪行号，2010年3月10日
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_exp(Expression*);//set the branch expression
		void set_cs_if(Statement*);//set the statements in the "if" branch
		void set_cs_else(Statement*);//set the statements in the "else" branch
		void set_fake_line(int);//设置该分支语句的伪行号，2010年3月10日
};

#endif

