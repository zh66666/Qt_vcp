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
void create_CCP_Generic_Service_Constants_c(void)
{
	extra_out.open((char*)(red_path+"/CFG_Constants.c").c_str());//打开文件
	//文件是否创建成功

	if(!extra_out.is_open())
	{
		//为true，说明文件创建失败，抛出错误信息
		string error_info="CCP_Generic_Service_Constants.c creating failed!";
		throw error_info;
	}
	else
	{
		extra_out<<"#include \"CFG_Constants.h\""<<endl;
		extra_out<<endl;
		//如果文件创建成功，则输出代码
		/******************modidy by zou**************************/
		//输出布尔量的编码 By Zou
		int bool_sig[2]={Bool_True_Sig,Bool_False_Sig};
		int bool_value[2]={TRUE_VALUE,FALSE_VALUE};
		string Name[2]={"TRUE","FALSE"};

		for(int i=0;i<2;i++)
		{
			int sig=bool_sig[i];
			int value=bool_value[i];
			//根据变量的初始化值和签名计算其低位值
			int C1 = ((-((long long) value * K_NUM_P1) % P1 + sig) % P1 - P1) % P1;
			int C2 = ((-((long long) value * K_NUM_P2) % P2 + sig) % P2 - P2) % P2;
			extra_out<<"const T_CODE CONST_BOOL_"<<Name[i]<<" = {{"<<
					int2HexStr(value)<<",{"<<int2HexStr(C1)<<","<<int2HexStr(C2)<<"}},"<<int2HexStr(i+2)<<"};"<<endl;
			extra_out<<"T_CODE "<<CONST_BOOL_BIND_PREFIX<<Name[i]<<";"<<endl;
		}
		/*****************zou modify end***************************/
		//输出立即数的编码
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
			extra_out<<"const T_CODE CONST_NUM_"<<constantBindedStr(value)<<" = {{"<<
					int2HexStr(iter->first)<<",{"<<int2HexStr(C1)<<","<<int2HexStr(C2)<<"}},"<<int2HexStr(sigIndex)<<"};"<<endl;
			extra_out<<"T_CODE AC_BIND_CONST_NUM_"<<constantBindedStr(value)<<";"<<endl;
		}
		extra_out<<endl;
		extra_out.close();
	}

}





