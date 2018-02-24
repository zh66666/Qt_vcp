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
		
		//2010年3月10日追加
		string file_info_;//记录本条注释所属的文件名
		bool is_next_line_start(int index,string &str);
		
	public:
		Cmnt();
		explicit Cmnt(string,int,int);
		
		string get_content();//get the content of the comment
		string output_cmnt(int);//output cmnt following the CSC rule
		int get_line_begin();//get the beginning line number of the comment
		int get_line_end();//get the ending line number of the comment 
		//2010年3月10日追加
		string get_file_info();//获取该条注释所属的文件名
		bool get_output();//get the boolean mark
		void set_output(bool);//set the boolean mark
		//2010年3月10日追加
		void set_file_info(string);//设置该条注释所属的文件名
};

#endif

