#include <app_utils.h>
#include "global_var.h"
#include "var_info.h"
#include "gen_utils.h"
#include <list>
#include <fstream>
#include <map>
#include <cstdlib>

using namespace std;

// 定义在文件 : src/main_utils.cpp
extern map<string ,int> g_ExFun_Hash_map;
extern map<string ,string> AC_RetVal_map;
extern map<string ,string> AC_ParmVar_map;
//定义在文件：gen_utils.cpp
extern long long  K_NUM;
extern const int P1;
extern const int P2;

//定义在文件：./create_extra_utils.cpp
extern ofstream extra_out;

// 定义在文件 : src/main_utils.cpp
extern string red_path;
extern string int2HexStr(const int num);
// 功能：生成编码工程中的附加头文件-CCP_Generic_Service_Param.h，此头文件中包含有常数的定义。
// 参数：无
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
void create_CCP_Generic_Service_Param_h(void)
{
	extra_out.open((char*)(red_path+"/CFG_Param.h").c_str());//打开文件
	//文件是否创建成功
	if(!extra_out.is_open())
	{
		//为true，说明文件创建失败，抛出错误信息
		string error_info="create_CCP_Generic_Service_Param_h.h creating failed!";
		throw error_info;
	}
	else
	{
		//如果文件创建成功，则输出代码
		extra_out<<"#ifndef CFG_PARAM_H"<<endl;
		extra_out<<"#define CFG_PARAM_H"<<endl<<endl;
		extra_out<<"#include \"VCL_Generic_Service_Root.h\""<<endl<<endl;

		//附加变量声明输出
		map<string,int>::iterator iter;
		for (iter = g_ExFun_Hash_map.begin(); iter != g_ExFun_Hash_map.end(); iter++ )
		{
			string key = "g_ExFun_" + iter->first;
			extra_out<<"extern INT32 " + key<<";"<<endl;
		}

		map<string,string>::iterator iter2;
		for (iter2 = AC_RetVal_map.begin(); iter2 != AC_RetVal_map.end(); iter2++ )
		{
			string key = "AC_RetVal_" + iter2->first;
			extra_out<<"extern T_CODE " + key<<";"<<endl;
		}

		for (iter2 = AC_ParmVar_map.begin(); iter2 != AC_ParmVar_map.end(); iter2++ )
		{
			string key = iter2->first;
			extra_out<<"extern T_CODE " + key<<";"<<endl;
		}

		//IO数组声明输出
		extra_out << endl;
		ifstream streamIOArray;
		ofstream streamIOArray1;
		streamIOArray.open(str_IO_TMP_H.c_str());
		string strInput;
		if(streamIOArray.is_open())
		{
			while(getline(streamIOArray, strInput))
			{
				extra_out << strInput << endl;
			}
			streamIOArray.close();
			remove(str_IO_TMP_H.c_str());
		}
		else
		{
			//log_error("IO Array declaration Temp Header File open failed.\n")
			//exit(1);
		}

		extra_out<<endl;
		extra_out<<"#endif"<<endl;
		extra_out.close();
	}

}





