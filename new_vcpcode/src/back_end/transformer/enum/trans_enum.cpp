#include <iostream>
#include "program.h"

// 定义在文件：src/main_utils.cpp
extern void check_pt_arg(void *pt,string info);

// 定义在文件：./scan_enum.cpp
extern void scan_enum(Program *s);  

// 定义在文件：./replace_enum.cpp
extern void replace_enumerator(Program *s);

// 功能：转换中间结构中的枚举类型，先扫描枚举类型，记录枚举成员，再替换枚举成员
// 输入参数：s-中间结构指针
// 输出参数：s-中间结构指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void trans_enum(Program *s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s,"program struct 's pointer");
		
		scan_enum(s);									//扫描并记录枚举成员
		replace_enumerator(s);				   //替换枚举成员
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="transform enum : "+str;
		throw error_info;
	}
}

