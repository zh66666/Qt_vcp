#ifndef TOKE_NUM_
#define TOKE_NUM_

#include "common.h"

/* a class used to store the token and its line number */
class Toke_num
{
	private:
		string toke_;//store the token's content
		int num_;//store the token's line number
		int fake_line_;//�ñ�ʶ����α�кţ�2010��3��10��׷��
	public:
		Toke_num();
		explicit Toke_num(string,int);
		
		int get_fake_line();//��ȡα�кţ�2010��3��10��׷��
		void set_fake_line(int);//����α�кţ�2010��3��10��׷��
		string get_toke();//get the token's content
		int get_num();//get the token' line number
};

#endif

