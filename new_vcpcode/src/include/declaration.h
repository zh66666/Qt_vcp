#ifndef DECLARATION_H_
#define DECLARATION_H_

#include "common.h"

/* a class used to store a declaration
   functional declaration or variable declaration */
class Declaration
{	
	private:
		Var_declaration* cvd_;//store a variable declaration
		Fun_declaration* cfd_;//store a functional declaration
		int stamp_;// a stamp used to mark which kinds of declaration
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		string file_info_;//�������������ļ����ƣ�2010��3��10��׷��
		int fake_line_;//��������չ���ļ��е��кţ�2010��3��10��׷��
		
	public:
		Declaration();//default��stamp_=-1
		explicit Declaration(Var_declaration*);//when it is a variable declaration��stamp_=0
		explicit Declaration(Fun_declaration*);//when it is a functional declaration��stamp_=1

		Var_declaration* get_cvd();//get the variable declaration
		Fun_declaration* get_cfd();//get the functional declaration
		int get_stamp();//get the stamp		
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		string get_file_info();//��ȡ���������ڵ��ļ����ƣ�2010��3��10��׷��
		int get_fake_line();//��ȡ��������α�кţ�2010��3��10��׷��
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_file_info(string);//�����������ڵ��ļ����ƣ�2010��3��10��׷��
		void set_fake_line(int);//����������α�кţ�2010��3��10��׷��
};

#endif

