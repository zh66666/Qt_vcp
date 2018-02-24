#ifndef FUN_INFO_H_
#define FUN_INFO_H_

#include <string>
#include <list>
using namespace std;

//it is a class used to store the function's type,name and parameter's number */
class Fun_info
{
	private:
		string name_;//store the name of the function
		int return_stamp_;//mark whether the function has a return value
		int para_stamp_;//mark whether the function has parameters
		int para_num_;//store the number of the parameters
		int sig_;//store the red_dF_fun's first signature
		list<string>* paras_;//store the parameters' names
		
	public:
		Fun_info();
		explicit Fun_info(string);
		
		string get_name();//get the function's name
		int get_return_stamp();
		int get_para_stamp();
		int get_para_num();//get the number of the parameters
		int get_sig();
		list<string>* get_paras();
		
		void set_return_stamp(int);
		void set_para_stamp(int);
		void set_para_num(int);//set the number of the parameters
		void set_paras(list<string>*);
		void set_sig(int);
};




#endif



