#include <app_utils.h>
#include "global_var.h"
#include "var_info.h"
#include "gen_utils.h"
#include <list>
#include <fstream>
#include <map>
#include <cstdlib>

using namespace std;

// �������ļ� : src/main_utils.cpp
extern map<string ,int> g_ExFun_Hash_map;
extern map<string ,string> AC_RetVal_map;
extern map<string ,string> AC_ParmVar_map;
//�������ļ���gen_utils.cpp
extern long long  K_NUM;
extern const int P1;
extern const int P2;

//�������ļ���./create_extra_utils.cpp
extern ofstream extra_out;

// �������ļ� : src/main_utils.cpp
extern string red_path;
extern string int2HexStr(const int num);
// ���ܣ����ɱ��빤���еĸ���ͷ�ļ�-CCP_Generic_Service_Param.h����ͷ�ļ��а����г����Ķ��塣
// ��������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
void create_CCP_Generic_Service_Param_h(void)
{
	extra_out.open((char*)(red_path+"/CFG_Param.h").c_str());//���ļ�
	//�ļ��Ƿ񴴽��ɹ�
	if(!extra_out.is_open())
	{
		//Ϊtrue��˵���ļ�����ʧ�ܣ��׳�������Ϣ
		string error_info="create_CCP_Generic_Service_Param_h.h creating failed!";
		throw error_info;
	}
	else
	{
		//����ļ������ɹ������������
		extra_out<<"#ifndef CFG_PARAM_H"<<endl;
		extra_out<<"#define CFG_PARAM_H"<<endl<<endl;
		extra_out<<"#include \"VCL_Generic_Service_Root.h\""<<endl<<endl;

		//���ӱ����������
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

		//IO�����������
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





