
#include "fun_info.h"
#include <fstream>

//�������ļ���src/main_utils.cpp
extern int project_tempvar_num;
extern int project_tempcondvar_num;
extern list<Fun_info>* fun_infos;
extern int indent_num;
extern string int2str(int);
extern int A;
extern void out_indent(int,ofstream& out);

ofstream extra_out;	//�������ɸ����ļ��������õ�������ļ���

// ���ܣ���redGlobal.h��redGlobal.c�ļ��У�������빤�������к����ĸ��ӱ���������������߶���
//			������߳�ʼ�����룬�������޲��޷���ֵ���������ݴ���Ĳ�����ȷ��������塢���������
//			�������ʼ��
// ���������stamp-�����Ҫ������廹��������������������ʼ�������Ϊ"dec"��˵��Ҫ�������;
//			���Ϊ"def"��˵��Ҫ������壻���Ϊ"init"��˵��Ҫ�����ʼ������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void out_fun_addvar(string stamp)
{
	string type_temp="";//�洢�����ַ���
	string assign_temp="";//�洢��ֵ�ַ���
	
	//�ж��Ƿ�Ҫ�������
	if("dec"==stamp)
	{
		//�����Ҫ�����������
		type_temp="extern INT_32 ";
		assign_temp="";
	}
	//�ж��Ƿ�Ҫ�������
	else if("def"==stamp)
	{
		//�����Ҫ����������
		type_temp="INT_32 ";
		assign_temp=" = 0";
	}
	else
	{
		//�����Ҫ�����ʼ������
		type_temp="";
		assign_temp=" = 0";
	}
		
	//����������Ϣ����
	for(list<Fun_info>::iterator iter=fun_infos->begin();iter!=fun_infos->end();iter++)
	{
		int ret_stamp=iter->get_return_stamp();//ȡ����ǰ����ķ���ֵ��ǩ
		int para_stamp=iter->get_para_stamp();//ȡ������ǰ����Ĳ�����ǩ
		string name=iter->get_name();//ȡ��������
	
		//�жϺ����Ƿ��޲��޷���ֵ
		if(0==ret_stamp && 0==para_stamp)
		{
			//����ǣ���������ӱ���
			;
		}
		else
		{
			//������вλ����з���ֵ��������������ӱ���
			//�ж��Ƿ��з���ֵ
			if(1==ret_stamp)
			{
				//����з���ֵ�����������ֵ���ӱ���
				out_indent(indent_num,extra_out);
				extra_out<<type_temp<<"H_retVal_"<<name<<assign_temp<<";"<<endl;
				out_indent(indent_num,extra_out);
				extra_out<<type_temp<<"L_retVal_"<<name<<assign_temp<<";"<<endl;
			}
			else
			{
				//����޷���ֵ�����������ֵ���ӱ���
				;
			}
			//�ж��Ƿ��в���
			if(1==para_stamp)
			{
				//����в�����������������ӱ���
				int para_num=iter->get_para_num();//ȡ����������
				string para_str="paraVar_"+name+"_";//�������ӱ�������
				//������в����ĸ��ӱ���
				for(int i=0;i<para_num;i++)
				{
					out_indent(indent_num,extra_out);
					extra_out<<type_temp<<"H_"<<para_str<<i<<assign_temp<<";"<<endl;
					out_indent(indent_num,extra_out);
					extra_out<<type_temp<<"L_"<<para_str<<i<<assign_temp<<";"<<endl;
				}
			}
			else
			{
				//����޲���������������ĸ��ӱ���
				;
			}
			//����вλ��з���ֵ������red_dF���ӱ���
			//�ж��ϲ�ģ���Ƿ���Ҫ���red_dF���ӱ���������
			if("dec"==stamp)
			{
				//����ǣ�������ߵ�λ������
				out_indent(indent_num,extra_out);
				extra_out<<"extern INT_32 H_red_dF_"<<name<<";"<<endl;
				out_indent(indent_num,extra_out);
				extra_out<<"extern INT_32 L_red_dF_"<<name<<";"<<endl;
			}
			//�ж��ϲ�ģ���Ƿ�Ҫ���red_dF���ӱ����Ķ���
			else if("def"==stamp)
			{
				//����ǣ�������ߵ�λ�Ķ���
				out_indent(indent_num,extra_out);
				extra_out<<"INT_32 H_red_dF_"<<name<<" = 0;"<<endl;
				out_indent(indent_num,extra_out);
				extra_out<<"INT_32 L_red_dF_"<<name<<" = "<<iter->get_sig()-A<<";"<<endl;
			}
			else
			{
				//����ǳ�ʼ����������������
			}
		}
	}
} 

// ���ܣ���redGlobal.h��redGlobal.c�ļ���������빤�������õ���ȫ����ʱ�������������߶��壬
//			����tmpArray_n�� tmpCondArray_n���������߶��壬���ݴ���Ĳ�����ȷ���������
//			�����������
// ���������stamp-�����Ҫ������廹��������������Ϊtrue��˵��Ҫ��������������������
// ����ֵ���ɹ��򷵻�void
// ���ߣ����
void out_temp_var(bool stamp)
{
	string dec_temp=(stamp==true)?"extern ":"";//���ݱ�ǩ���������ַ���
	string def_temp=(stamp==true)?"":" = 0";//���ݱ�ǩ���ö����ַ���
	
	//���tmpArray_n��ʱ����
	for(int i=0;i<project_tempvar_num;i++)
	{
		string str_tmp_h="H_tmpArray_"+int2str(i);//��ͨ��ʱ�����ĸ�λ
		string str_tmp_l="L_tmpArray_"+int2str(i);//��ͨ��ʱ�����ĵ�λ
		//�����ͨ��ʱ�����ĸ�λ�͵�λ����
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_h<<def_temp<<";"<<endl;
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_l<<def_temp<<";"<<endl;
	}
	extra_out<<endl;
	
	//���tmpCondArray_n��ʱ����
	for(int i=0;i<project_tempcondvar_num;i++)
	{
		string str_tmp_h="H_tmpCondArray_"+int2str(i);//������ʱ�����ĸ�λ
		string str_tmp_l="L_tmpCondArray_"+int2str(i);//������ʱ�����ĵ�λ
		//���������ʱ�����ĸ�λ�͵�λ����
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_h<<def_temp<<";"<<endl;
		extra_out<<dec_temp<<"INT_32 "<<str_tmp_l<<def_temp<<";"<<endl;
	}
}
