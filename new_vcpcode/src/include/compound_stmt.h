#ifndef COMPOUND_STMT_H_
#define COMPOUND_STMT_H_

#include "common.h"

/* a class used to store a compound statement*/
class Compound_stmt
{	
	private:
		list<Declaration>* cdl_;//use a list to store the declarations
		list<Statement>* csl_;//use a list to store the statements
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//�ø��������������ڵ�α�кţ�2010��3��10��׷��
	public:
		Compound_stmt();//default
		explicit Compound_stmt(list<Declaration>*, list<Statement>*);
	
		list<Declaration>* get_cdl();//get the declaration list
		list<Statement>* get_csl();//get the statement list
		
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//��ȡ�ø��������������ڵ�α�кţ�2010��3��10��׷��
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_fake_line(int);//���øø��������������ڵ�α�кţ�2010��3��10��׷��
};

#endif

