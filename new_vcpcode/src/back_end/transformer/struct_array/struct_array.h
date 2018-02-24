#ifndef STRUCT_ARRAY_H_
#define STRUCT_ARRAY_H_

#include "program_struct.h"

//存储结构体成员变量的信息
struct mem_loc
{
	string name;//记录结构体成员变量的名称
	int loc;//记录成员变量在结构体中的相对位置
};

//存储结构体类型的信息
class Struct_info
{
	private:
		string struct_name_;//存储结构体的类型名
		list<mem_loc>* members_;//存储结构体的成员信息
		int num_;//记录结构体成员的个数
		string scope_;//记录结构体的作用域
		
	public:
		//Struct_info();
		Struct_info(string,list<mem_loc>*,int,string);
		
		string get_struct_name();//获取结构体类型名
		list<mem_loc>* get_members();//获取成员信息链表
		int get_num();//获取成员个数
		string get_scope();//获取作用域
		
		void set_struct_name(string);//设置结构体名
		void set_members(list<mem_loc>*);//设置结构体成员信息链表
		void set_num(int);//设置结构体成员个数
		void set_scope(string);//设置结构体的作用域
};

//该结构体用于存储结构体变量信息
struct struct_var
{
	string var_name_;//存储结构体变量的名字
	Struct_info* struct_info_;//存储结构体类型信息
	string scope_;//记录该结构体变量的作用域
	string stamp_;//结构体变量标签信息，记录该变量隶属的结构体类型名，如果该变量是某个结构体的
							//成员，则此标签信息为结构体的名称，否则为"common"
};

//用于存储数组变量的信息
class Array_info
{
	private:
		string array_name_;//存储数组变量的名字
		vector<int>* subs_;//记录数组变量的下标值
		string scope_;//记录数组变量的作用域
		string stamp_;//记录数组变量所隶属的结构体类型名，如果不为结构体成员变量，则为"common"
	
	public:
		Array_info();
		explicit Array_info(string,vector<int>*);
		
		string get_name();//获取变量的名字
		vector<int>* get_subs();//获取数组下标
		string get_scope();//获取数组变量的作用域信息
		string get_stamp();//获取数组变量的标签信息，即隶属的结构体类型名
		void set_scope(string);//设置数组变量的作用域信息
		void set_stamp(string);//设置数组变量的标签信息即隶属的结构体类型名
};

#endif


