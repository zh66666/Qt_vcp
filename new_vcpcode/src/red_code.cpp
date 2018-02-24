#include <string>
#include <iostream>
#include "program.h"
#include "var_info.h"
#include <fstream>
#include <cstdlib>
#include "app_utils.h"
#include "new_Siginfo.h"
using namespace std;

// 定义在文件: ./front_end/parser_utils.cpp
extern void front_end(string,Program*&);

// 定义在文件 : ./back_end/back_end_utils.cpp
extern void back_end(Program*&);

// 定义在文件: ./create_extra/create_extra_files.cpp
extern void create_extra_files(void);

// 定义在文件：./get_config_info.cpp
extern void get_config_info(void);

//定义在./main_utils.cpp
extern void Tranfile(string strFileCopeIn, string strFileCopeOut);

//定义在./resolve_file_name.cpp
extern string resolve_file_name(string& path_name);

//io
extern void gen_io(string InputFile, string SrcFile, string HeaderFile);


// 定义在文件 : ./main_utils.cpp
extern int line_num;
extern int macro_label;
extern bool debug_flag;
extern list<Var_info>* var_infos;
extern ifstream fin;
extern string red_path;
extern string sig_list_file;
extern string enter_fun_name;
extern string current_file_name;
extern string bool_config_file;
extern string sig_config_file;
extern string prime_config_file;	
extern string asm_macro_file;


extern void InitSigInfo();

//2010年3月10日追加
extern string file_info;
extern int fake_line_num;

extern void out_var_sigs(list<Var_info>* s, string file_name);

// 功能：冗余编码程序的main函数，根据输入的文件名，以及命令的参数信息，对文件进行冗余编码，
//			包括读取配置信息，前端处理，后端处理以及附加文件生成
// 输入参数：argc-参数个数
//            argv-参数内容
// 返回值：成功则返回0，出错则报错，并退出
// 作者：李刚
int main(int argc ,char *argv[])
{

	//判断命令的第一个参数是否为"-D"
	if("-D"==(string)(argv[1]))
	{
		//如果是，则说明用户选择调试模式，则将调试标记置为真值
		debug_flag=true;
	}
	else
	{
		//如果不为"-D"，则说明为正常模式，将调试标记置为假值
		debug_flag=false;
	}
	
	//第二个参数即为入口函数名
	enter_fun_name=argv[2];
		
	int index=0;//记录最后一个待编码文件在参数中的位置
	//遍历第三个参数至最后一个参数
	for(int i=3;i<argc;++i)
	{
		//如果发现某个参数为"-o"，则取出其后的参数，设置冗余代码存放的路径
		if("-o"==(string)(argv[i]))
		{
			index=i;//设置最后一个待编码文件在参数中的位置
			red_path=argv[++i];//设置冗余代码文件存放的目录
			break;
		}
	}
	
	//根据冗余代码目录名，设置配置文件所在路径
	sig_list_file=red_path+sig_list_file;
	
	try
	{
		//读取配置信息
		//get_config_info();
		
		//关联签名文件输入流
		fin.open((char*)(sig_list_file.c_str()));
		
		//初始化签名信息
		//InitSigInfo();
		Init_Save_SigInfo();

		//外部IO数组处理
		gen_io(red_path+str_IO_FILE_PATH+str_IO_FILE_NAME,str_IO_TMP_SRC,str_IO_TMP_H);

		//对输入的文件依次进行处理
		for(int i=3;i<index;++i)
		{
			string strPathName = string(argv[i]);
			string strFileName = resolve_file_name(strPathName);
			if(strFileName != str_IO_FILE_NAME)
			{
				Program *pro=NULL;											 //中间结构指针，初始化为NULL
				current_file_name=string(argv[i]);					//设置当前正在处理的文件名
				line_num=1;														//每编码一个新文件时，行号置1
				fake_line_num=1;												//2010年3月10日追加
				macro_label=0;												    //每编码一个新文件时，汇编宏标号置0

				front_end(argv[i],pro);										//前端处理

				back_end(pro);	  												//后端处理

				cout<<"file : "<<argv[i]<<" redundent coding sucessful!"<<endl;
			}
		}

		fin.close();																//关闭签名文件输入流
		macro_label=0;														//所有文件编码完毕，汇编宏标号置0
		create_extra_files();												//生成附加文件
		
		//如果是调试模式，则编码结束后，输出所有全局变量的历史签名到中间文件
		if(true==debug_flag)
		{
			out_var_sigs(var_infos,"./temp_result/sig_info/sig_history.txt");
			cout<<"print signature history sucessful!"<<endl;
		}


	}
	catch(string s)
	{
		//如果捕获到错误信息，则报错，并退出程序
		//2010年3月10日修改
		cerr<<"error : when coding file \""<<current_file_name<<"\", on line "<<line_num<<
		" of file "<<file_info<<" : "<<s<<endl;
		exit(-1);
	}
	return 0;
}

