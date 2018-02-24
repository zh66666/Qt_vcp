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
void create_CCP_Generic_Service_Constants_c(void)
{
	extra_out.open((char*)(red_path+"/CFG_Constants.c").c_str());//���ļ�
	//�ļ��Ƿ񴴽��ɹ�

	if(!extra_out.is_open())
	{
		//Ϊtrue��˵���ļ�����ʧ�ܣ��׳�������Ϣ
		string error_info="CCP_Generic_Service_Constants.c creating failed!";
		throw error_info;
	}
	else
	{
		extra_out<<"#include \"CFG_Constants.h\""<<endl;
		extra_out<<endl;
		//����ļ������ɹ������������
		/******************modidy by zou**************************/
		//����������ı��� By Zou
		int bool_sig[2]={Bool_True_Sig,Bool_False_Sig};
		int bool_value[2]={TRUE_VALUE,FALSE_VALUE};
		string Name[2]={"TRUE","FALSE"};

		for(int i=0;i<2;i++)
		{
			int sig=bool_sig[i];
			int value=bool_value[i];
			//���ݱ����ĳ�ʼ��ֵ��ǩ���������λֵ
			int C1 = ((-((long long) value * K_NUM_P1) % P1 + sig) % P1 - P1) % P1;
			int C2 = ((-((long long) value * K_NUM_P2) % P2 + sig) % P2 - P2) % P2;
			extra_out<<"const T_CODE CONST_BOOL_"<<Name[i]<<" = {{"<<
					int2HexStr(value)<<",{"<<int2HexStr(C1)<<","<<int2HexStr(C2)<<"}},"<<int2HexStr(i+2)<<"};"<<endl;
			extra_out<<"T_CODE "<<CONST_BOOL_BIND_PREFIX<<Name[i]<<";"<<endl;
		}
		/*****************zou modify end***************************/
		//����������ı���
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
			extra_out<<"const T_CODE CONST_NUM_"<<constantBindedStr(value)<<" = {{"<<
					int2HexStr(iter->first)<<",{"<<int2HexStr(C1)<<","<<int2HexStr(C2)<<"}},"<<int2HexStr(sigIndex)<<"};"<<endl;
			extra_out<<"T_CODE AC_BIND_CONST_NUM_"<<constantBindedStr(value)<<";"<<endl;
		}
		extra_out<<endl;
		extra_out.close();
	}

}





