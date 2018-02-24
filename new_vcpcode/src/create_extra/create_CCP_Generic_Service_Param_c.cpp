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
void create_CCP_Generic_Service_Param_c(void)
{
	extra_out.open((char*)(red_path+"/CFG_Param.c").c_str());//���ļ�
	//�ļ��Ƿ񴴽��ɹ�
	if(!extra_out.is_open())
	{
		//Ϊtrue��˵���ļ�����ʧ�ܣ��׳�������Ϣ
		string error_info="create_CCP_Generic_Service_Param_h.h creating failed!";
		throw error_info;
	}
	else
	{
		extra_out<<"#include \"CFG_Param.h\""<<endl;
		extra_out<<endl;

		//����ļ������ɹ������������
		map<string,int>::iterator iter;
		for (iter = g_ExFun_Hash_map.begin(); iter != g_ExFun_Hash_map.end(); iter++ )
		{
			string key = "g_ExFun_" + iter->first;
			int sig = iter->second;
			extra_out<<"INT32 " + key<<" = "<< sig <<";"<<endl;
		}

		map<string,string>::iterator iter2;
		for (iter2 = AC_RetVal_map.begin(); iter2 != AC_RetVal_map.end(); iter2++ )
		{
			string key = "AC_RetVal_" + iter2->first;
			string value = iter2->second;
			extra_out<<"T_CODE " + key<<" = "<< value <<";"<<endl;
		}

		for (iter2 = AC_ParmVar_map.begin(); iter2 != AC_ParmVar_map.end(); iter2++ )
		{
			string key = iter2->first;
			string value = iter2->second;
			extra_out<<"T_CODE " + key<<" = "<< value <<";"<<endl;
		}

		//IO���鶨�����
		extra_out << endl;
		ifstream streamIOArray(str_IO_TMP_SRC.c_str());
		string strInput;
		if(streamIOArray.is_open())
		{
			while(getline(streamIOArray, strInput))
			{


				extra_out << strInput << endl;
			}
			streamIOArray.close();
			remove(str_IO_TMP_SRC.c_str());
		}
		else
		{
			log_error("IO Array declaration Temp Header File open failed.\n")
			exit(1);
		}


		extra_out<<endl;
		extra_out.close();
	}

}





