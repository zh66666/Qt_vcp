#include "app_utils.h"
#include "gen_utils.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>

using namespace std;

// �������ļ� : ./create_redGlobal_c.cpp
extern void create_redglobal_c(void);

// �������ļ� : ./create_redGlobal_h.cpp
extern void create_redglobal_h(void);

// �������ļ� : ./create_redDefine_h.cpp
extern void create_reddefine_h(void);

// �������ļ� : ./create_CCP_Generic_Service_Constants_h.cpp
extern void create_CCP_Generic_Service_Constants_h(void);

// �������ļ� : ./create_CCP_Generic_Service_Constants_c.cpp
extern void create_CCP_Generic_Service_Constants_c(void);

// �������ļ���./create_relationcmp_VCL_impl.cpp
extern void output_relacmp_impl(const string &StrOutFile);

// �������ļ� : ./create_CCP_Generic_Service_Param_h.cpp
extern void create_CCP_Generic_Service_Param_h(void);

// �������ļ� : ./create_CCP_Generic_Service_Param_c.cpp
extern void create_CCP_Generic_Service_Param_c(void);

//�������ļ� main_utils.cpp��
extern string red_path;
extern void Tranfile(string strFileCopeIn, string strFileCopeOut);

//���鸽�ӱ�����ʼ��ǩ��
static int ArrayExtrInitSig = 0;
static int ArrayExtrInitSigIndex = 0;

void SetArrayExtrInitSig(int Sig, int SigIndex)
{
	ArrayExtrInitSig = Sig;
	ArrayExtrInitSigIndex = SigIndex;
}

static void create_VCLimplFile(const string &strVCLimplFile)
{
	//VCL_Impl VCL��ʵ���ļ�����
	//1�����ӹ�ϵ�������ʵ�ִ���
	output_relacmp_impl(strVCLimplFile);

	//2���������鸽�ӱ����Ķ������
	int ArrExtrChk1 = (-((long long) 0 * K_NUM_P1) % P1 + ArrayExtrInitSig)	% P1;
	int ArrExtrChk2 = (-((long long) 0 * K_NUM_P1) % P1 + ArrayExtrInitSig)	% P1;
	ofstream outVCLFile(strVCLimplFile.c_str(), ios::app);

	char ArrayDec[1024];
	snprintf(ArrayDec, sizeof(ArrayDec),
			"static T_CODE Array_EXTA = { 0, { %d, %d}, %d};", ArrExtrChk1, ArrExtrChk2, ArrayExtrInitSigIndex);
	outVCLFile << string(ArrayDec) << endl;

	outVCLFile.close();
}

// ���ܣ����ɱ��빤���еĸ����ļ���redGlobal.c��redGlobal.h��redDefine.h�����ļ�����ģ����
//			�������������ģ��ִ��֮�󣬲����е�
// ��������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void create_extra_files(void)
{
	try
	{
		//�������빤�̵�CCP_Generic_Service_Constants.h�ļ�
		/*create_CCP_Generic_Service_Constants_h();
		cout<<"create CCP_Generic_Service_Constants.h succeeded!"<<endl;

		//�������빤�̵�CCP_Generic_Service_Constants.c�ļ�
		create_CCP_Generic_Service_Constants_c();
		cout<<"create CCP_Generic_Service_Constants.c succeeded!"<<endl;

		//�������빤�̵�CCP_Generic_Service_Param.h
		create_CCP_Generic_Service_Param_h();
		cout<<"create CFG_Param.h!"<<endl;

		//�������빤�̵�CCP_Generic_Service_Param.c
		create_CCP_Generic_Service_Param_c();
		cout<<"create CFG_Param.c!"<<endl;*/

		//�����VCL���ļ����Ƶ�ָ����ĿĿ¼
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
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ����ӡ���������˳�����
		//2010��3��10���޸�
		cerr<<"error : create extra files : "<<str<<endl;
		exit(-1);
	}
}

