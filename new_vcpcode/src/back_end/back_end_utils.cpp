#include "expression.h"
#include "var.h"
#include "program.h"
#include <fstream>

// �洢�ڲ�ֵ�ǰ�ṹʱ�õ���һ����ʱ���������ֵ
int tempvar_num_max=0;

// �洢�ڲ�ֵ�ǰ�ṹʱ�õ���������ʱ���������ֵ
int tempcondvar_num_max=0;

//��¼while����goto����ţ�ÿ����һ��while�ṹ���ͼ�1
int goto_label=0;									
// �������ļ� : src/main_utils.c
extern int str2int(string str_num);
extern void check_pt_arg(void *pt,string info);

// �������ļ� : ./ transformer/trans_struct.cpp
extern void trans_struct(Program *s);

// �������ļ� : ./generator/gen_pro.cpp
extern void gen_pro(Program *s); 


// ���ܣ��������ʽ���󣬼�������ֵ�����ܼ����򷵻ء�right��
//           ���򣬷��ء�wrong��
// ���������s-���ʽ����ָ��
// ���������result-������ָ��
// ����ֵ���������гɹ��򷵻�string��ʧ�����׳�������Ϣ
// ���ߣ����
string compute_exp(Expression* s,int* result)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		
		//ƥ����ʽ����ı�ǩ
		switch(s->get_stamp())
		{
			case 0:
			{
				// ���������������ı��ʽ
				int left=0;	//�洢����ʽ�����ֵ
				string str_l=compute_exp(s->get_cel(),&left);	//��������ʽ
				
				int right=0;	//�洢�ұ��ʽ�����ֵ
				string str_r=compute_exp(s->get_cer(),&right);	//�����ұ��ʽ
				
				//�ж�����ʽ���ұ��ʽ�ܷ�������
				if(str_l=="right" && str_r=="right")
				{
					//������ұ��ʽ���ܼ�����������ñ��ʽ��ֵ
					//ƥ�������
					if(s->get_op()=="+")
					{
						//����ǼӺţ�����мӷ��������ؼ�����
						*result=left+right;
						return "right";
					}
					else if(s->get_op()=="-")
					{
						//����Ǽ��ţ�����м����������ؼ�����
						*result=left-right;
						return "right";
					}
					else if(s->get_op()=="*")
					{
						//����ǳ˺ţ�����г˷��������ؼ�����
						*result=left*right;
						return "right";
					}
					else if(s->get_op()=="/")
					{
						//����ǳ��ţ�����г����������ؼ�����
						*result=left/right;
						return "right";
					}
					else if(s->get_op()=="<<")
					{
						//��������Ʒ��ţ���������ƣ������ؼ�����
						*result=left<<right;
						return "right";
					}
					else if(s->get_op()==">>")
					{
						//��������Ʒ��ţ���������ƣ������ؼ�����
						*result=left>>right;
						return "right";
					}
					else if(s->get_op()=="&&")
					{
						//�������������ţ�����������㣬�����ؼ�����
						*result=left&&right;
						return "right";
					}
					else if(s->get_op()=="||")
					{
						//����ǻ�������ţ�����л����㣬�����ؼ�����
						*result=left||right;
						return "right";
					}
					else
					{
						//����δ֪�Ĳ��������׳�������Ϣ
						string error_info="operation is invalid!";
						throw error_info;
					}
				}
				else
				{
					//���ұ��ʽֻҪ��һ������ʧ�ܣ�������ñ��ʽҲ�޷�����
					return "wrong";
				}
			}
			break;
			
			case 1:
			{
				//ֻ����һ���������ı��ʽ
				int right=0;	//�洢�ұ��ʽ�����ֵ
				string str_r=compute_exp(s->get_cer(),&right);	//�����ұ��ʽ
				
				//�ж��ұ��ʽ�Ƿ��ܼ������
				if(str_r=="right")
				{
					//ƥ��������Ƿ���ȡ������
					if(s->get_op()=="!")
					{
						//�����ȡ�����������ִ��ȡ�����㣬�����ؼ�����
						*result=!right;
						return "right";
					}
					else
					{
						//δ֪���������׳�������Ϣ
						string error_info="operation is invalid!";
						throw error_info;
					}
				}
				else
				{
					//�ұ��ʽ����ʧ�ܣ�������ñ��ʽҲ�޷�����
					return "wrong";
				}
			}
			break;
			
			case 2:
			{
				//���ű��ʽ
				int tmp=0;		//�洢����������ʽ�����ֵ
				string str_r=compute_exp(s->get_exp(),&tmp);		//������������ı��ʽ
				
				//�ж���������ı��ʽ�Ƿ����ɹ�
				if(str_r=="right")
				{
					//�ɹ�������ֵ��Ϊ�����ʽ��ֵ
					*result=tmp;
					return "right";
				}
				else
				{
					//���ɹ�����˵�������ʽҲ���ɼ���
					return "wrong";
				}
			}
			break;
			
			case 3:
			{
				//�������ʽ�����س�����ֵ��Ϊ�����ʽ��ֵ
				*result=str2int(s->get_c());
				return "right";
			}
			break;
			
			case 4:
			case 5:
			{
				//�������ñ��ʽ�ͱ������ʽ���ɱ�����
				return "wrong";
			}
			break;
			
			default:
			{
				//���ʽ����ı�ǩ�Ƿ������׳�������Ϣ
				string error_info="expression's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="compute expression's value : "+str;
		throw error_info;
	}
}

//���������������ʽ��ֵ
int compute_exp(int left,int right,string op)
{
	try
	{
		int result=0;
		if(op=="+")
		{
			result=left+right;
		}
		else if(op=="-")
		{
			result=left-right;
		}
		else if(op=="*")
		{
			result=left*right;
		}
		else if(op=="/")
		{
			result=left/right;
		}
		else
		{
			//����δ֪�Ĳ��������׳�������Ϣ
			string error_info="operation is invalid!";
			throw error_info;
		}
		return result;
	}
	catch(string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="compute expression's value : "+str;
		throw error_info;
	}
}

// ���ܣ���ǰ�˴������õ����м�ṹ����ת����Ȼ�������������
// ���������s-�м�ṹָ�������
// ���������s-�м�ṹָ�������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void back_end(Program*& s)
{
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(s,"program struct 's pointer");

		trans_struct(s);	//ת���м�ṹ

		gen_pro(s);		//�����������
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="back end : "+str;
		throw error_info;
	}
}


