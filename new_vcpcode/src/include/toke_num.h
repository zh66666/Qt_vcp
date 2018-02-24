#ifndef TOKE_NUM_
#define TOKE_NUM_

#include "common.h"

/* a class used to store the token and its line number */
class Toke_num
{
	private:
		string toke_;//store the token's content
		int num_;//store the token's line number
		int fake_line_;//该标识符的伪行号，2010年3月10日追加
	public:
		Toke_num();
		explicit Toke_num(string,int);
		
		int get_fake_line();//获取伪行号，2010年3月10日追加
		void set_fake_line(int);//设置伪行号，2010年3月10日追加
		string get_toke();//get the token's content
		int get_num();//get the token' line number
};

#endif

