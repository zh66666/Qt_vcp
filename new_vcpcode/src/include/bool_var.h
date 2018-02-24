#ifndef BOOL_VAR_H_
#define BOOL_VAR_H_

#include <list>
#include <string>
using namespace std;

class Bool_var
{
	private:
		string name_;
		string scope_;
		int t_;
		int f_;
	public:
		Bool_var();
		explicit Bool_var(string,string,int,int);
		
		string get_name();
		string get_scope();
		int get_t();
		int get_f();
};
#endif



