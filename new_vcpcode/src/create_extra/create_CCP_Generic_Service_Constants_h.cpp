#include <app_utils.h>
#include "global_var.h"
#include "var_info.h"
#include "gen_utils.h"
#include "CodedVarBuild_Util.h"
#include <list>
#include <fstream>
#include <map>

using namespace std;

// �������ļ� : src/main_utils.cpp
extern map<int,int> constantMap;
extern int getConstantSig(int constant,int *sigIndex);
//�������ļ���gen_utils.cpp
extern long long  K_NUM;
extern const int P1;
extern const int P2;

//�������ļ���./create_extra_utils.cpp
extern ofstream extra_out;

// �������ļ� : src/main_utils.cpp
extern int A;
extern string red_path;
extern string int2HexStr(const int num);
// ���ܣ����ɱ��빤���еĸ���ͷ�ļ�-CCP_Generic_Service_Constants.h����ͷ�ļ��а����г����Ķ��塣
// ��������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void create_CCP_Generic_Service_Constants_h(void)
{
	extra_out.open((char*)(red_path+"/CFG_Constants.h").c_str());//���ļ�
	//�ļ��Ƿ񴴽��ɹ�
	if(!extra_out.is_open())
	{
		//Ϊtrue��˵���ļ�����ʧ�ܣ��׳�������Ϣ
		string error_info="CCP_Generic_Service_Constants.h creating failed!";
		throw error_info;
	}
	else
	{
		extra_out<<"#ifndef CFG_CONSTANTS_H"<<endl;
		extra_out<<"#define CFG_CONSTANTS_H"<<endl<<endl;
		extra_out<<"#include \"VCL_Generic_Service_Root.h\""<<endl<<endl;

		/****����ֵ��������******bu Zou********************/
		extra_out<<"extern const T_CODE CONST_BOOL_TRUE;"<<endl;
		extra_out<<"extern T_CODE "<<CONST_BOOL_BIND_PREFIX<<"TRUE;"<<endl;

		extra_out<<"extern const T_CODE CONST_BOOL_FALSE;"<<endl;
		extra_out<<"extern T_CODE "<<CONST_BOOL_BIND_PREFIX<<"FALSE;"<<endl;
		/****����ֵ��������******end***********************/

		//����ļ������ɹ������������
		map<int,int>::iterator iter;
		for (iter = constantMap.begin(); iter != constantMap.end(); iter++ )
		{
			int value=iter->first;
			int sig=iter->second;
			//���ݱ����ĳ�ʼ��ֵ��ǩ���������λֵ
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





