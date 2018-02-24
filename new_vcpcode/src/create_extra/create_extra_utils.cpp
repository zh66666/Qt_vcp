
#include "fun_info.h"
#include <fstream>

//定义在文件：src/main_utils.cpp
extern int project_tempvar_num;
extern int project_tempcondvar_num;
extern list<Fun_info>* fun_infos;
extern int indent_num;
extern string int2str(int);
extern int A;
extern void out_indent(int,ofstream& out);

ofstream extra_out;	//定义生成附加文件过程中用到的输出文件流

// 功能：在redGlobal.h和redGlobal.c文件中，输出编码工程中所有函数的附加变量的声明代码或者定义
//			代码或者初始化代码，不包括无参无返回值函数，根据传入的参数来确定输出定义、输出声明还
//			是输出初始化
// 输入参数：stamp-标记是要输出定义还是输出声明，还是输出初始化，如果为"dec"，说明要输出声明;
//			如果为"def"，说明要输出定义；如果为"init"，说明要输出初始化代码
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void out_fun_addvar(string stamp)
{
	string type_temp="";//存储类型字符串
	string assign_temp="";//存储赋值字符串
	
	//判断是否要输出声明
	if("dec"==stamp)
	{
		//如果是要输出声明代码
		type_temp="extern INT_32 ";
		assign_temp="";
	}
	//判断是否要输出定义
	else if("def"==stamp)
	{
		//如果是要输出定义代码
		type_temp="INT_32 ";
		assign_temp=" = 0";
	}
	else
	{
		//如果是要输出初始化代码
		type_temp="";
		assign_temp=" = 0";
	}
		
	//遍历函数信息链表
	for(list<Fun_info>::iterator iter=fun_infos->begin();iter!=fun_infos->end();iter++)
	{
		int ret_stamp=iter->get_return_stamp();//取出当前对象的返回值标签
		int para_stamp=iter->get_para_stamp();//取出但当前对象的参数标签
		string name=iter->get_name();//取出函数名
	
		//判断函数是否无参无返回值
		if(0==ret_stamp && 0==para_stamp)
		{
			//如果是，则不输出附加变量
			;
		}
		else
		{
			//如果是有参或者有返回值，则输出函数附加变量
			//判断是否有返回值
			if(1==ret_stamp)
			{
				//如果有返回值，则输出返回值附加变量
				out_indent(indent_num,extra_out);
				extra_out<<type_temp<<"H_retVal_"<<name<<assign_temp<<";"<<endl;
				out_indent(indent_num,extra_out);
				extra_out<<type_temp<<"L_retVal_"<<name<<assign_temp<<";"<<endl;
			}
			else
			{
				//如果无返回值，则不输出返回值附加变量
				;
			}
			//判断是否有参数
			if(1==para_stamp)
			{
				//如果有参数，则输出参数附加变量
				int para_num=iter->get_para_num();//取出参数个数
				string para_str="paraVar_"+name+"_";//参数附加变量名字
				//输出所有参数的附加变量
				for(int i=0;i<para_num;i++)
				{
					out_indent(indent_num,extra_out);
					extra_out<<type_temp<<"H_"<<para_str<<i<<assign_temp<<";"<<endl;
					out_indent(indent_num,extra_out);
					extra_out<<type_temp<<"L_"<<para_str<<i<<assign_temp<<";"<<endl;
				}
			}
			else
			{
				//如果无参数，则不输出参数的附加变量
				;
			}
			//输出有参或有返回值函数的red_dF附加变量
			//判断上层模块是否是要输出red_dF附加变量的声明
			if("dec"==stamp)
			{
				//如果是，则输出高低位的声明
				out_indent(indent_num,extra_out);
				extra_out<<"extern INT_32 H_red_dF_"<<name<<";"<<endl;
				out_indent(indent_num,extra_out);
				extra_out<<"extern INT_32 L_red_dF_"<<name<<";"<<endl;
			}
			//判断上层模块是否要输出red_dF附加变量的定义
			else if("def"==stamp)
			{
				//如果是，则输出高低位的定义
				out_indent(indent_num,extra_out);
				extra_out<<"INT_32 H_red_dF_"<<name<<" = 0;"<<endl;
				out_indent(indent_num,extra_out);
				extra_out<<"INT_32 L_red_dF_"<<name<<" = "<<iter->get_sig()-A<<";"<<endl;
			}
			else
			{
				//如果是初始化，则不输出冗余代码
			}
		}
	}
} 

// 功能：在redGlobal.h和redGlobal.c文件中输出编码工程中所用到的全局临时变量的声明或者定义，
//			包括tmpArray_n和 tmpCondArray_n的声明或者定义，根据传入的参数来确定输出定义
//			还是输出声明
// 输入参数：stamp-标记是要输出定义还是输出声明，如果为true，说明要输出声明，否则输出定义
// 返回值：成功则返回void
// 作者：李刚
void out_temp_var(bool stamp)
{
	string dec_temp=(stamp==true)?"extern ":"";//根据标签设置声明字符串
	string def_temp=(stamp==true)?"":" = 0";//根据标签设置定义字符串
	
	//输出tmpArray_n临时变量
	for(int i=0;i<project_tempvar_num;i++)
	{
		string str_tmp_h="H_tmpArray_"+int2str(i);//普通临时变量的高位
		string str_tmp_l="L_tmpArray_"+int2str(i);//普通临时变量的低位
		//输出普通临时变量的高位和低位代码
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_h<<def_temp<<";"<<endl;
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_l<<def_temp<<";"<<endl;
	}
	extra_out<<endl;
	
	//输出tmpCondArray_n临时变量
	for(int i=0;i<project_tempcondvar_num;i++)
	{
		string str_tmp_h="H_tmpCondArray_"+int2str(i);//条件临时变量的高位
		string str_tmp_l="L_tmpCondArray_"+int2str(i);//条件临时变量的低位
		//输出条件临时变量的高位和低位代码
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_h<<def_temp<<";"<<endl;
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_l<<def_temp<<";"<<endl;
	}
}
