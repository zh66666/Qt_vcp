#include "app_utils.h"
#include "gen_utils.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>

using namespace std;

// 定义在文件 : ./create_redGlobal_c.cpp
extern void create_redglobal_c(void);

// 定义在文件 : ./create_redGlobal_h.cpp
extern void create_redglobal_h(void);

// 定义在文件 : ./create_redDefine_h.cpp
extern void create_reddefine_h(void);

// 定义在文件 : ./create_CCP_Generic_Service_Constants_h.cpp
extern void create_CCP_Generic_Service_Constants_h(void);

// 定义在文件 : ./create_CCP_Generic_Service_Constants_c.cpp
extern void create_CCP_Generic_Service_Constants_c(void);

// 定义在文件：./create_relationcmp_VCL_impl.cpp
extern void output_relacmp_impl(const string &StrOutFile);

// 定义在文件 : ./create_CCP_Generic_Service_Param_h.cpp
extern void create_CCP_Generic_Service_Param_h(void);

// 定义在文件 : ./create_CCP_Generic_Service_Param_c.cpp
extern void create_CCP_Generic_Service_Param_c(void);

//定义在文件 main_utils.cpp中
extern string red_path;
extern void Tranfile(string strFileCopeIn, string strFileCopeOut);

//数组附加变量初始化签名
static int ArrayExtrInitSig = 0;
static int ArrayExtrInitSigIndex = 0;

void SetArrayExtrInitSig(int Sig, int SigIndex)
{
	ArrayExtrInitSig = Sig;
	ArrayExtrInitSigIndex = SigIndex;
}

static void create_VCLimplFile(const string &strVCLimplFile)
{
	//VCL_Impl VCL库实现文件处理
	//1、增加关系运算编码实现代码
	output_relacmp_impl(strVCLimplFile);

	//2、增加数组附加变量的定义语句
	int ArrExtrChk1 = (-((long long) 0 * K_NUM_P1) % P1 + ArrayExtrInitSig)	% P1;
	int ArrExtrChk2 = (-((long long) 0 * K_NUM_P1) % P1 + ArrayExtrInitSig)	% P1;
	ofstream outVCLFile(strVCLimplFile.c_str(), ios::app);

	char ArrayDec[1024];
	snprintf(ArrayDec, sizeof(ArrayDec),
			"static T_CODE Array_EXTA = { 0, { %d, %d}, %d};", ArrExtrChk1, ArrExtrChk2, ArrayExtrInitSigIndex);
	outVCLFile << string(ArrayDec) << endl;

	outVCLFile.close();
}

// 功能：生成编码工程中的附加文件，redGlobal.c，redGlobal.h，redDefine.h三个文件。该模块是
//			在冗余代码生成模块执行之后，才运行的
// 参数：无
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void create_extra_files(void)
{
	try
	{
		//创建编码工程的CCP_Generic_Service_Constants.h文件
		/*create_CCP_Generic_Service_Constants_h();
		cout<<"create CCP_Generic_Service_Constants.h succeeded!"<<endl;

		//创建编码工程的CCP_Generic_Service_Constants.c文件
		create_CCP_Generic_Service_Constants_c();
		cout<<"create CCP_Generic_Service_Constants.c succeeded!"<<endl;

		//创建编码工程的CCP_Generic_Service_Param.h
		create_CCP_Generic_Service_Param_h();
		cout<<"create CFG_Param.h!"<<endl;

		//创建编码工程的CCP_Generic_Service_Param.c
		create_CCP_Generic_Service_Param_c();
		cout<<"create CFG_Param.c!"<<endl;*/

		//将相关VCL库文件复制到指定项目目录
		string ConfigPath = red_path+str_IO_FILE_PATH;
		//string ConfigFileName[] = {"CFG_Service.h", "Interface.h", "Interface.c", "VCL_Generic_Service_Root.h"};
		string ConfigFileName[] = { "Interface.h", "Interface.c","var.c"};
		for(int i = 0; i < __countof(ConfigFileName); ++i)
		{
			Tranfile(ConfigPath+ConfigFileName[i],red_path+"/"+ConfigFileName[i]) ;
		}

		//create_VCLimplFile(red_path+"/"+ConfigFileName[2]);
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，打印出来，并退出程序
		//2010年3月10日修改
		cerr<<"error : create extra files : "<<str<<endl;
		exit(-1);
	}
}

