#include <fstream>
#include <iostream>
#include "program.h"

// 定义在文件：src/main_utils.cpp
extern string current_file_name;
extern bool debug_flag;
extern void check_pt_arg(void *pt,string info);
extern string resolve_file_name(string& path_name);

// 定义在文件 : ./printer/print_program.cpp
extern void print_program(Program *s, string path_name);

// 定义在文件 : ./typedef/replace_typedef.cpp
extern void replace_typedef(Program *s);

// 定义在文件 : ./enum/trans_enum.cpp
extern void trans_enum(Program *s);

// 定义在文件 : ./struct_array/struct_array_dec.cpp
extern void scan_struct_array_pro(Program *s);

// 定义在文件 : ./split_stmts/split_pro.cpp
extern void split_pro(Program *s); 
extern void trans_void_func(Program *s);


// 功能：依次对前端处理所得到的中间结构进行修改，拆分,完成转换功能
// 输入参数：s-中间结构指针
// 输出参数：s-中间结构指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void trans_struct(Program *s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s,"program struct 's pointer");
		
		string full_name=current_file_name;					 //存储当前处理的文件名（包括路径）
		string file_name=resolve_file_name(full_name);//存储解析出来的文件名
		
		//如果是调试模式，则将规约的原始程序输出到中间文件
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/ori_pro/"+file_name);
			cout<<"print original program successful!"<<endl;
		}
		
		replace_typedef(s);//替换typedef
		cout<<"repalce typedef successful!"<<endl;//输出提示信息
		
		//如果是调试模式，则将经过typedef替换后的结果输出到中间文件
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/replace_typedef/"+file_name);
			cout<<"print program (typedef  replaced) successful!"<<endl;

		}
		
		trans_enum(s);//转换中间结构中的枚举类型
		cout<<"transform enum successful!"<<endl;
		
		//如果是调试模式，则将enum经过处理后的结果输出到中间文件
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/trans_enum/"+file_name);
			cout<<"print program (enum  transformed) successful!"<<endl;
		}
		
		scan_struct_array_pro(s);//处理结构体和数组
		cout<<"scan struct&&array successful!"<<endl;

		
		//如果是调试模式，则将结构体和数组经过处理后的结果输出到中间文件
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/trans_array/"+file_name);
			cout<<"print program (array && struct  transformed) successful!"<<endl;
		}
		
		split_pro(s);//拆分结构
		cout<<"split the program successful!"<<endl;
		
		//如果是调试模式，则将经过拆分后的结果输出到中间文件
		if(true==debug_flag)
		{
			print_program(s,"./temp_result/split/"+file_name);
			cout<<"print program splited  successful!"<<endl;
		}

		//对无返回值函数进行特殊操作，强制其返回0，以在分支循环语句中使用无返回值函数
		//trans_void_func(s);
		cout<<"transtuct void function successful!"<<endl;

	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="transforming program struct : "+str;
		throw error_info;
	}
}




