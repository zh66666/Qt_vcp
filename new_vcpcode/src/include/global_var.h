#ifndef GLOBAL_VAR_H_
#define GLOBAL_VAR_H_

#include <string>
#include <vector>
using namespace std;

class Global_var
{
	private:
		int stamp_;
		bool ex_;
		string var_name_;
		int var_value_;
		int sub_;
		int sig_;
		vector<int>* sub_value_;
		string adj_stamp_;
	
	public:
		Global_var();
		explicit Global_var(string,int);//int a;stamp_=0
		explicit Global_var(string,int,int,int);//int a=1;stamp_=1;int a[2];stamp_=2
		explicit Global_var(string,int,int,vector<int>*);//int a[3]={1,2,3};stamp_=3
		
		int get_stamp();
		string get_var_name();
		string get_adj_stamp();
		int get_var_value();
		int get_sub();
		int get_sig();
		bool get_ex();
		vector<int>* get_sub_value();
		void set_ex(bool);
		void set_adj_stamp(string);
};


#endif

