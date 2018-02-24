#ifndef ITERATION_STMT_H_
#define ITERATION_STMT_H_

#include "common.h"

/* a class used to store an iteration statement */
class Iteration_stmt
{	
	private:
		Expression* ce_;//store the entrance expression
		Statement* cs_;//store the statements
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//��ѭ������α�кţ�2010��3��10��׷��
	public:
		Iteration_stmt();//default 
		explicit Iteration_stmt(Expression*, Statement*);
		Iteration_stmt(const Iteration_stmt&);//copy constructor function
		Expression* get_ce();//get the entrance expression
		Statement* get_cs();//get the statements in the loop body
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//��ȡ��ѭ������α�кţ�2010��3��10��׷��
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_cs(Statement*);//set the statements in the loop body
		void set_fake_line(int);//���ø�ѭ������α�кţ�2010��3��10��׷��
};

#endif

