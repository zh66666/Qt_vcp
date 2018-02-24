#ifndef VAR_INFO_H_
#define VAR_INFO_H_

#include <string>
#include <vector>
using namespace std;


//it is a class used to store the variable's name and signatures
class Var_info
{
	private:
		string name;//store the variable's name
		vector<int>* sigs;//store the variable's signatures
		string scope;//store the variable's scope
		string stamp;//common variable or array
		int sigIndex;//store the index of the signature
		
	public:
		Var_info();
		explicit Var_info(string,int,string);
		Var_info(const Var_info&);
		
		string get_name();//get the name of the variable
		vector<int>* get_sigs();//get signatures of the variable
		string get_scope();//get the scope of the variable
		string get_stamp();
		int getSigIndex();//get the value of the signature's index
		void set_sigIndex(int);
		void set_stamp(string);
		void add_sigs(int);//add new signature to the vector
};


#endif

