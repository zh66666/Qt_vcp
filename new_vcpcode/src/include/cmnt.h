#ifndef CMNT_H_
#define CMNT_H_

#include "common.h"

/*a class used to store information of the comment*/
class Cmnt
{
	private:
		string content_;//store the content of the comment
		int line_begin_;//store the beginning line number of the comment 
		int line_end_;//store the ending line number of the comment 
		bool output_;//when the comment has been output,output_=true,or out_put_=false
		
		//2010��3��10��׷��
		string file_info_;//��¼����ע���������ļ���
		bool is_next_line_start(int index,string &str);
		
	public:
		Cmnt();
		explicit Cmnt(string,int,int);
		
		string get_content();//get the content of the comment
		string output_cmnt(int);//output cmnt following the CSC rule
		int get_line_begin();//get the beginning line number of the comment
		int get_line_end();//get the ending line number of the comment 
		//2010��3��10��׷��
		string get_file_info();//��ȡ����ע���������ļ���
		bool get_output();//get the boolean mark
		void set_output(bool);//set the boolean mark
		//2010��3��10��׷��
		void set_file_info(string);//���ø���ע���������ļ���
};

#endif

