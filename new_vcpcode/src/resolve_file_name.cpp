#include <string>
#include <iostream>
using namespace std;

// ���ܣ������ļ���·�������������ļ����ƣ������أ�ͬʱ���ļ�·�����е��ļ���ɾȥ
// ���磺����Ĳ���Ϊ /src/testcase/1.c���򷵻�1.c��ͬʱ�������޸�Ϊ/src/testcase/
// ע�⣺�����е�·���ָ������Ϊ��/��
// ���������path_name-�ļ�·����
// ���������path_name-�ļ�·����(·������../.../�����һ���ַ���/)
// ����ֵ���ɹ��򷵻ؽ����������ļ�����ʧ�����׳�������Ϣ
string resolve_file_name(string& path_name)
{
	//�ж��ļ�·�����ַ����Ƿ�Ϊ��
	if(path_name.empty())
	{
		//���Ϊ�գ����׳�������Ϣ
		string error_info="resolving file name from path_name , path name is empty!";
		throw error_info;
	}
	else
	{
		//�����Ϊ�գ���ʼ����
		string file_name="";		//�洢���������ļ���
		unsigned int pos=path_name.find_last_of("/");//��¼·�����ַ��������һ��"/"��λ��
		
		//�жϡ�/����λ���Ƿ�ΪĬ�ϵ�λ��
		if(pos==string::npos)
		{
			//ΪĬ�ϵ�λ�ã�˵��·������û��"/"��˵��Ϊ��ǰĿ¼�µ��ļ�
			file_name=path_name;	//·���������ļ���
			path_name.replace(0,path_name.length(),"./");	//��·�����޸ĳ�"./"
		}
		else
		{
			//��ΪĬ�ϵ�λ�ã�˵��·�����к��С�/��
			file_name = path_name.substr(pos+1);	//���һ����/��֮����ַ�����Ϊ�ļ���
			path_name.erase(pos+1,file_name.length());	//��·�����е��ļ�������ɾȥ
		}
		return file_name;
	}
}
