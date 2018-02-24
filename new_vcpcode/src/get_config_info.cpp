#include "bool_var.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <cstdlib>

// 定义在文件 : ./main_utils.cpp
extern string bool_config_file;	
extern string sig_config_file;
extern  string prime_config_file;	
extern int A;
extern string str_A;
extern list<Bool_var>* bool_vars;
extern map<string, int>* pre_sigs;
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);
extern string int2str(int);
extern int str2int(string);

// 功能：从指定文件名的文件中读取预配置签名，并将其写入到指定的map中。
// 输出参数：m - 存放预配置签名的（变量名，签名）值对，不允许为空（0）。
// 输入参数：file_name - 指定的文件名，从此文件中读取预配置签名，不允许为空串。
// 返回值：成功返回void，失败抛出错误信息
// 作者：梁盟磊
static void read_config_sig(map<string, int> *m, const string &file_name)
{   
	try
	{
		//检查参数：用于存放预配置签名信息的map指针是否为空
		check_pt_arg(m,"pre-config signature map's pointer ");
		//检查参数：存放预配置签名的文件名字符串是否为空
		check_str_arg(file_name,"pre-config signature file's name ");
		
		 ifstream input(file_name.c_str());  // 关联file_name指定的文件的文件流
		// 判断文件流是否打开成功
		if(input == 0)
		{
		  // 如果文件流打开失败 
		  string err_info = "cannot open file: " + file_name;
		  throw err_info;
		}
		else
		{
		  // 如果文件流打开成功
		  string line = "";   // 存放文件读出的一行
		  string var_name = "";   // 存放配置文件中的变量名
		  int sig = 0;        // 存放对应变量名对应的签名
		  
		  // 判断文件是否读取到文件末尾
		  while(std::getline(input, line) != 0)
		  {
			// 如果文件没有读完
			std::istringstream words(line);   // 存放文件的一行（line）的string流
			words >> var_name;	//读变量名
			words >> sig;				//读签名
			map<string,int>::iterator p = m->find(var_name);  // 指向map中以var_name
																									// 为名字的元素的迭代器
			// 判断map中是否找到以var_name为名字的元素
			if(p != m->end())
			{
			  // 如果在map中已存在变量名为var_name，说明变量重复，报错。
			  string err_info = "name duplication on variable: "+ var_name;
			  throw err_info;
			}
			else
			{
			  // map中不存在以var_name为名字的变量，加入之。
			  (*m)[var_name] = sig;
			}
		  }
		}
		input.close();    // 关闭文件流	
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="read config signatures : "+s;
		throw error_info;
	}
}

// 功能：从指定文件名的文件中读取bool类型变量的变量名，及其真值与假值。
// 输出参数：l - 存放bool信息的对象Bool_var的列表，不允许为空（0）。
// 输入参数：file_name - 指定的文件名，从此文件中读取bool类型变量信息，
//           不允许为空串。
// 返回值：成功返回void，失败抛出错误信息
// 作者：梁盟磊
static void read_bool_values(list<Bool_var> *l, const string &file_name)
{
	try
	{
		//检查参数：用于存放布尔真假值的list指针是否为空
		check_pt_arg(l,"pre-config signature map's pointer ");
		//检查参数：存放布尔真假值的文件名字符串是否为空
		check_str_arg(file_name,"bool values file's name ");
		
		 ifstream input(file_name.c_str());  // 关联file_name指定的文件的文件流
		// 判断文件是否打开成功
		if(input == 0)
		{
		  // 如果失败
		  string err_info = "cannot open file: "+ file_name;
		  throw err_info;
		}
		else
		{
		  // 如果打开文件成功
		  string line = "";   				// 存放文件读出的一行
		  string bool_name = "";  // 存放bool变量的名字
		  string t_value = "";      	 // 存放上述bool变量的真值字符串
		  string f_value = "";       // 存放上述bool变量的假值字符串
		  
		  // 判断是否读到文件末尾
		  while(std::getline(input, line) != 0)
		  {
			// 如果未到文件末尾
			std::istringstream words(line);   // 存放文件的一行（line）的string流
			words >> bool_name;	// 读取bool变量的名字
			words >> t_value;		// 读取上述bool变量的真值字符串
			words >> f_value;		// 读取上述bool变量的假值字符串
			
			// 遍历整个list，检查是否已存在以bool_name为名字的bool变量
			for(list<Bool_var>::iterator p = l->begin(); p != l->end(); ++p)
			{
			  // 判断是否已存在以bool_name为名字的bool变量
			  if(bool_name == p->get_name())
			  { 
				// 如果存在，则报错
				string err_info = "name duplication on variable: "+ bool_name;
				throw err_info;
			  }
			}  
			// 如果不存在，则加入到list中。
			Bool_var bv(bool_name, "global",str2int(t_value), str2int(f_value));// 存放bool信息的Bool_var对象
			l->push_back(bv);
		  }      
		}
		input.close();
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="read bool values : "+s;
		throw error_info;
	}
}

// 功能：从指定文件名的文件中读取大素数A的值。
// 输入参数：file_name - 指定的文件名，从此文件中读取大素数的值，不允许为空串。
// 返回值：成功返回大素数的值，失败则抛出错误信息
// 作者：梁盟磊
static int read_prime_A(const string &file_name)
{ 
	try
	{
		//检查参数：存放大素数A的文件名字符串是否为空
		check_str_arg(file_name,"prime file's name ");
		
		ifstream input(file_name.c_str());	//关联配置文件
		int prime_value = 0;								//用于存储读取的A的值
		
		 // 判断文件流是否关联失败
		if(input == 0)
		{  
			 // 文件打开失败，则报错并退出
			string err_info = "cannot open file: "+ file_name;
			throw err_info;
		}
		else
		{
			  // 文件成功打开，则读取内容
		  string line = ""; //用于存放读取到的行的内容
			
			  // 读取行内容是否成功同时该行是否有内容
		  if(std::getline(input, line) != 0 && !line.empty())
		  {
			// 行读取成功并且有内容，则保存文件中的值到prime_value
			std::istringstream words(line);	//关联存有行内容的字符串
			
			// 判断字符串流读取的是否是整型值
			if(!(words >> prime_value)) //从字符串流中读素数A的值
			{
			  // 如果出错，表示不是整形值
			  string err_info = "format of prime A value in file is invalid!";
			  throw err_info;
			}        
		  }
		  else
		  {
			// 行读取失败或者无内容，报错并退出
			string err_info =  "no value in file: " + file_name;
			throw err_info;
		  }
		}
		
		input.close();	//关闭文件流
		return prime_value;
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="read prime A : "+s;
		throw error_info;
	}
}

// 功能：读取工程中的配置信息，设置相关的全局变量
// 参数：无
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
void get_config_info(void)
{
	try
	{
		read_config_sig(pre_sigs,sig_config_file);			//读预配置签名信息
		read_bool_values(bool_vars,bool_config_file);	//读布尔配置信息
		A=read_prime_A(prime_config_file);					//读素数A
		str_A=int2str(A);														//设置大素数字符串的内容
	}
	catch(string s)
	{
		//2010年3月10日修改
		cerr<<"error : read config infomation : "<<s<<endl;
		exit(-1);
	}
}


