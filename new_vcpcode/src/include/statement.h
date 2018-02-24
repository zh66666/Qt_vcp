#ifndef STATEMENT_H_
#define STATEMENT_H_

#include "common.h"

/* a class used to store a statement*/
class Statement
{    
	private:
		Compound_stmt* ccstmt_;//store the compound statement
		Selection_stmt* csstmt_;//store the selection statement
		Iteration_stmt* cistmt_;//store the iteration statement
		Expression* ce_;//store the expression
		Var* cvar_;//store the variable on the left of an assign statement
		Return_stmt* crs_;//store the return statement
		string  semi_;//*when the statement is just a ";",store the ";"
		int stamp_;//a stamp used to mark which kinds of statement
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		string file_info_;//该语句所属的文件名，2010年3月10日追加
		int fake_line_;//该条语句的伪行号，2010年3月10日追加

		
	public:
		Statement();//default stamp_=-1
		explicit Statement(Compound_stmt *);//when it is a compound statement,stamp_=0
		explicit Statement(Selection_stmt *);//when it is a selection statement,stamp_=1
		explicit Statement(Iteration_stmt *);//when it is an iteration statement,stamp_=2
		explicit Statement(Return_stmt *);//when it is a return statement,stamp_=3
		explicit Statement(Var*, Expression *);//when it is an assign statement,stamp_=4
		explicit Statement(Expression *);//when it is an expression statement,stamp_=5
		explicit Statement(string);//when the statement is just a ";",stamp_=6
		
		Compound_stmt* get_ccstmt();//get the compound statement
		Selection_stmt* get_csstmt();//get the selection statement
		Iteration_stmt* get_cistmt();//get the iteration statement
		Expression* get_ce();//get the expression
		Var* get_cvar();//get the  variable on the left of an assign statement
		Return_stmt* get_crs();//get the return statement
		string  get_semi();//get the ";"
		int get_stamp();//get the stamp

		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		string get_file_info();//获取该语句所属的文件名，2010年3月10日追加
		int get_fake_line();//获取该语句的伪行号，2010年3月10日追加
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_file_info(string);//设置该语句所属的文件名，2010年3月10日追加
		void set_fake_line(int);//设置该语句的伪行号，2010年3月10日追加
		
};

#endif

