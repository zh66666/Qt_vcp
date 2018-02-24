#include "statement.h"
#include "expression.h"
#include "var.h"
#include "cmnt.h"
#include "return_stmt.h"
extern int S_last,S_this;
extern vector<int> sig_vector;
extern vector<int>::iterator iter;
class Var_info;

//���ڼ�¼ѭ�����ڵ���Ҫ�����ı�����Ϣ���������ƣ��Ƿ�Ϊ��ֵ������ֵ
struct adj_var
{
	string name;//��¼��������
	string stamp;//��¼Ϊ��ֵ������ֵ
};

//�������ļ�: ./while_if_utils.cpp
extern list<adj_var>* adj_name_list;
extern void get_var_names(Statement* s,list<adj_var>* l);

// �������ļ���./gen_utils.cpp
extern ofstream gen_out;
extern int iteration_level;
extern int select_level;

// �������ļ� : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern int indent_num;
extern void out_indent(int,ofstream&);
extern list<Cmnt>* cmnts;
extern void check_pt_arg(void *pt,string info);
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);

// �������ļ�: ./gen_compd_stmt.c
extern void gen_compd_stmt(Compound_stmt*,list<Var_info>*);
// �������ļ� : ./gen_ret_stmt.c
extern void gen_return_stmt(Return_stmt*);
// �������ļ� : ./gen_var_stmt.c
extern void gen_var_assign(string,Expression*,list<Var_info>*);
// �������ļ� : ./gen_array_stmt.c
extern void gen_array_assign(Var*,Expression*,list<Var_info>*);
// �������ļ� : ./gen_fun_stmt.c
extern void gen_fun_call(Expression*,list<Var_info>*);
// �������ļ� : ./gen_select_stmt.c
extern void gen_select_stmt(Selection_stmt* s,list<Var_info>* v);
// �������ļ� : ./gen_iterat_stmt.c
extern void gen_iterat_stmt(Iteration_stmt* s,list<Var_info>* v);

// ���ܣ�������䣬�����������͵��ò�ͬ�ı��뺯�����б���
// ���������s-������ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_stmt(Statement* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"statement object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		//�ж��������
		switch(s->get_stamp())
		{
			case 0:
			{
				//���Ϊ������䣬����ø��������뺯��
				gen_compd_stmt(s->get_ccstmt(),v);
			}
			break;
			
			case 1:
			{
				//���Ϊ��֧��䣬����÷�֧�����뺯��
				gen_select_stmt(s->get_csstmt(),v);
			}
			break;
			
			case 2:
			{
				//���Ϊѭ����䣬�����ѭ�������뺯��
				gen_iterat_stmt(s->get_cistmt(),v);
			}
			break;
			
			case 3:
			{
				//������������ǩ��
				out_indent(indent_num,gen_out);
				gen_out<<"S_Fun_out=S_Fun_out_this;"<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"RS=RS+S_Fun_out_this-"<<*iter<<";"<<endl;
				gen_out<<endl;
				//���Ϊ������䣬����÷��������뺯��
				Return_stmt* ss = s->get_crs();
				if(0==ss->get_stamp())
				{
					//���û�з���ֵ�����
				}
				else
				{
					gen_return_stmt(s->get_crs());
				}

			}
			break;
			
			case 4:
			{
				//���Ϊ��ֵ��䣬�����ж���ֵ�����Ƿ�Ϊ��ͨ����
				if(s->get_cvar()->get_stamp()==0)
				{
					//�������ͨ�������������ͨ������ֵ���뺯��
					gen_var_assign(s->get_cvar()->get_name(),s->get_ce(),v);
				}
				else
				{
					//����������������������鸳ֵ���뺯��
					gen_array_assign(s->get_cvar(),s->get_ce(),v);
				}
			}
			break;
			
			case 5:
			{
				//���Ϊ���ʽ��䣬��˱��ʽ����Ϊ�������ñ��ʽ
				//�жϱ��ʽ�Ƿ�Ϊ�������ñ��ʽ
				if(5==s->get_ce()->get_stamp())
				{
					//����ǣ�������ж��Ƿ�λ��ѭ�����ڻ��߷�֧��
//					if(iteration_level>-1 || select_level>-1)
//					{
//						//���λ��ѭ�����߷�֧�ڲ������׳�������Ϣ
//						string error_info="module function call can not appear in while or if structure!";
//						throw error_info;
//					}
					//�Ժ������ñ��ʽ���б���
				//	if(s->get_cvar()->get_name())
						//gen_out<<s->get_cvar()->get_name()<<"=";
					gen_fun_call(s->get_ce(),v);
				}
				else
				{
					//������ǣ����׳�������Ϣ
					string error_info="expression statement is not a function call!";
					throw error_info;
				}
			}
			break;
			
			case 6:
			{
				//���Ϊ�ֺ���䣬����ֺ����ݣ����Ϊ�������������
				if(";"!=s->get_semi())
				{
					out_indent(indent_num,gen_out);
					gen_out<<s->get_semi()<<endl;
				}
				else
				{
					//���Ϊ�ֺţ������
				}
			}
			break;
			
			default:
			{
				//�������ǩֵΪ����ֵ����Ƿ����׳�������Ϣ
				string error_info="statement's stamp is invalid! ";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding statement : "+str;
		throw error_info;
	}
}

// ���ܣ�������������������б��룬�����ָ������ΪCSCLABEL���ʱ����˵������ѭ������
//           ��ѭ����
// ���������s-�������ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_stmtlist(list<Statement>* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"statement list's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		//�������Ϊ�գ�������������
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//���õ�ǰ������λ�ڵ��к�
			fake_line_num=iter->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//����������֮ǰ��ע����Ϣ

			gen_stmt(&(*iter),v);//�����������б���
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding statement list : "+str;
		throw error_info;
	}
}

// ���ܣ�������䣬���RSC�����
// ���������s-������ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
void print_stmt(Statement* s,list<Var_info>* v)
{
	try
	{
		//�ж��������
		switch(s->get_stamp())
		{
			case 0:
			{
				//���Ϊ������䣬����ø��������뺯��
				gen_compd_stmt(s->get_ccstmt(),v);
			}
			break;

			case 1:
			{
				//���Ϊ��֧��䣬����÷�֧�����뺯��
				gen_select_stmt(s->get_csstmt(),v);
			}
			break;

			case 2:
			{
				//���Ϊѭ����䣬�����ѭ�������뺯��
				gen_iterat_stmt(s->get_cistmt(),v);
			}
			break;

			case 3:
			{
				//���Ϊ������䣬����÷��������뺯��
				gen_return_stmt(s->get_crs());
			}
			break;

			case 4:
			{
				//���Ϊ��ֵ��䣬�����ж���ֵ�����Ƿ�Ϊ��ͨ����
				if(s->get_cvar()->get_stamp()==0)
				{
					//�������ͨ�������������ͨ������ֵ���뺯��
					gen_var_assign(s->get_cvar()->get_name(),s->get_ce(),v);
				}
				else
				{
					//����������������������鸳ֵ���뺯��
					gen_array_assign(s->get_cvar(),s->get_ce(),v);
				}
			}
			break;

			case 5:
			{
				//���Ϊ���ʽ��䣬��˱��ʽ����Ϊ�������ñ��ʽ
				//�жϱ��ʽ�Ƿ�Ϊ�������ñ��ʽ
				if(5==s->get_ce()->get_stamp())
				{
					//����ǣ�������ж��Ƿ�λ��ѭ�����ڻ��߷�֧��
					if(iteration_level>-1 || select_level>-1)
					{
						//���λ��ѭ�����߷�֧�ڲ������׳�������Ϣ
						string error_info="module function call can not appear in while or if structure!";
						throw error_info;
					}
					//�Ժ������ñ��ʽ���б���
					gen_fun_call(s->get_ce(),v);
				}
				else
				{
					//������ǣ����׳�������Ϣ
					string error_info="expression statement is not a function call!";
					throw error_info;
				}
			}
			break;

			case 6:
			{
				//���Ϊ�ֺ���䣬����ֺ����ݣ����Ϊ�������������
				if(";"!=s->get_semi())
				{
					out_indent(indent_num,gen_out);
					gen_out<<s->get_semi()<<endl;
				}
				else
				{
					//���Ϊ�ֺţ������
				}
			}
			break;

			default:
			{
				//�������ǩֵΪ����ֵ����Ƿ����׳�������Ϣ
				string error_info="statement's stamp is invalid! ";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding statement : "+str;
		throw error_info;
	}
}

// ���ܣ����������������RSC��䣬��������б��룬�����ָ������ΪCSCLABEL���ʱ����˵������ѭ������
//           ��ѭ����
// ���������s-�������ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
void print_stmtlist(list<Statement>* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"statement list's pointer");
		check_pt_arg(v,"var_info list's pointer");

		//�������Ϊ�գ�������������
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//���õ�ǰ������λ�ڵ��к�
			fake_line_num=iter->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//����������֮ǰ��ע����Ϣ

			//�жϵ�ǰ����Ƿ�Ϊѭ�����ǰ���CSCLABEL���
			if(iter->get_stamp()==6 &&  iter->get_semi().substr(0,1)=="C")
			{
				//����ǣ�����Ϊѭ���ṹ�Ѿ���ʼ������ѭ������ȫ�ֱ���
				iteration_level++;//ѭ��������1
				//ȡ��ѭ���ṹ�е���ֵ��������ֵ����
				list<Statement>::iterator iter1=iter;
				iter1++;
				//ȡ��CSCLABEL�����while���֮�����ֵ��������ֵ����
				while(2!=iter1->get_stamp())
				{
					get_var_names(&(*iter1),adj_name_list);
					iter1++;
				}
				//ȡ��while����е���ֵ��������ֵ����
				get_var_names(&(*iter1),adj_name_list);
			}
			else
			{
				//������ǣ������
			}
			print_stmt(&(*iter),v);//�����������б���
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print stmtlist error : "+str;
		throw error_info;
	}
}
