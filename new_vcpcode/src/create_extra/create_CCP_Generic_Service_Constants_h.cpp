#include <app_utils.h>
#include "global_var.h"
#include "var_info.h"
#include "gen_utils.h"
#include "CodedVarBuild_Util.h"
#include <list>
#include <fstream>
#include <map>

using namespace std;

// 定义在文件 : src/main_utils.cpp
extern map<int,int> constantMap;
extern int getConstantSig(int constant,int *sigIndex);
//定义在文件：gen_utils.cpp
extern long long  K_NUM;
extern const int P1;
extern const int P2;

//定义在文件：./create_extra_utils.cpp
extern ofstream extra_out;

// 定义在文件 : src/main_utils.cpp
extern int A;
extern string red_path;
extern string int2HexStr(const int num);
// 功能：生成编码工程中的附加头文件-CCP_Generic_Service_Constants.h，此头文件中包含有常数的定义。
// 参数：无
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李晨欢
void create_CCP_Generic_Service_Constants_h(void)
{
	extra_out.open((char*)(red_path+"/CFG_Constants.h").c_str());//打开文件
	//文件是否创建成功
	if(!extra_out.is_open())
	{
		//为true，说明文件创建失败，抛出错误信息
		string error_info="CCP_Generic_Service_Constants.h creating failed!";
		throw error_info;
	}
	else
	{
		extra_out<<"#ifndef CFG_CONSTANTS_H"<<endl;
		extra_out<<"#define CFG_CONSTANTS_H"<<endl<<endl;
		extra_out<<"#include \"VCL_Generic_Service_Root.h\""<<endl<<endl;

		/****布尔值常量声明******bu Zou********************/
		extra_out<<"extern const T_CODE CONST_BOOL_TRUE;"<<endl;
		extra_out<<"extern T_CODE "<<CONST_BOOL_BIND_PREFIX<<"TRUE;"<<endl;

		extra_out<<"extern const T_CODE CONST_BOOL_FALSE;"<<endl;
		extra_out<<"extern T_CODE "<<CONST_BOOL_BIND_PREFIX<<"FALSE;"<<endl;
		/****布尔值常量声明******end***********************/

		//如果文件创建成功，则输出代码
		map<int,int>::iterator iter;
		for (iter = constantMap.begin(); iter != constantMap.end(); iter++ )
		{
			int value=iter->first;
			int sig=iter->second;
			//根据变量的初始化值和签名计算其低位值
			int C1 = ((-((long long) value * K_NUM_P1) % P1 + sig) % P1 - P1) % P1;
			int C2 = ((-((long long) value * K_NUM_P2) % P2 + sig) % P2 - P2) % P2;
			int sigIndex=0;
			int signature=getConstantSig(value,&sigIndex);
			extra_out<<"extern const T_CODE CONST_NUM_"<<constantBindedStr(value)<<";"<<endl;
			extra_out<<"extern T_CODE AC_BIND_CONST_NUM_"<<constantBindedStr(value)<<";"<<endl;
		}
		extra_out<<endl;
		extra_out<<"#endif"<<endl;
		extra_out.close();
	}

}





