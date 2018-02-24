#include "program_struct.h"

// �ýṹ�����ڴ洢ö�ٳ�Ա�������Ϣ�����������֣����Ӧ������ֵ�Լ���������������
// ���� : ��ȫ��������������enum {a,b,c}; ��ô���ڴ洢ö�ٳ�Աa�Ľṹ���ԱΪ������Ϊa��
// ֵΪ0��������Ϊ��global��
struct enum_mem
{
	string name_;	//ö�ٳ�Ա������
	int value_;			//ö�ٳ�Ա��Ӧ������ֵ
	string scope_;	//iö�ٳ�Ա��������
};

//�������ļ���src/main_utils.cpp
extern string int2str(int);
extern int line_num;
extern void check_pt_arg(void *pt,string info);
//2010��3��10��׷��
extern string file_info;

//�������ļ� : ./scan_enum.c
extern string find_enumerator_value(list<enum_mem>*,string,int*);
extern list<enum_mem>* enum_mems;

//�洢��ǰ�������ڵ�����������ں����⣬����"global"��������ֵΪ��������
static string scope="global";	

static void replace_enumerator(Var_declaration* v_d);
static void replace_enumerator(list<Type_specifier>* t_s);
static void replace_enumerator(Init_declarator* i_d);
static void replace_enumerator(Initializer* i);
static void replace_enumerator(Var* v);
void replace_enumerator(Expression* s);
static void replace_enumerator(Fun_declaration* f_d);
static void replace_enumerator(Compound_stmt* c);
static void replace_enumerator(Statement* s);


// ���ܣ������м�ṹ�������е�ö�ٳ�Ա�滻Ϊ��Ӧ������ֵ
// ���������s-�м�ṹָ��
// ���������s-�м�ṹָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void replace_enumerator(Program *s)
{
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(s,"program struct 's pointer");
		
		//�����м�ṹ�е���������
		for(list<Declaration>::iterator iter=s->get_declist()->begin();
			   iter!=s->get_declist()->end();iter++)
		{
			line_num=iter->get_line_begin();//���õ�ǰ������λ�ڵ��к�
			//2010��3��10��׷��
			file_info=iter->get_file_info();//���õ�ǰ������λ�ڵ��ļ���
			
			//�ж������Ƿ�Ϊ��������
			if(0==iter->get_stamp())
			{
				//�����������ı�ǩΪ0��˵��Ϊ��������
				replace_enumerator(iter->get_cvd());//�����ñ�������
				
			}
			else
			{
				//�����������ı�ǩ��Ϊ0��˵��Ϊ��������
				replace_enumerator(iter->get_cfd());//�����ú�������
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="replace enumerator : "+str;
		throw error_info;
	}
}

// ���ܣ����������������������������е����������������ı�������
// ���������v_d-������������ָ��
// ���������v_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Var_declaration* v_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(v_d,"variable declaration object's pointer");
		
		//�ȱ���������������������
		replace_enumerator(v_d->get_decl_spec()->get_type_spec_list());
		
		//�жϱ������������Ƿ���б�������
		if(v_d->get_stamp()==1)
		{
			//�����ǩΪ1��˵�����б�������������������
			for(list<Init_declarator>::iterator iter=v_d->get_inits()->begin();
			iter!=v_d->get_inits()->end();iter++)
			{
				replace_enumerator(&(*iter));//�����������ı���
			}
		}
		else
		{
			//�����ǩΪ����ֵ��˵��������������
			;
		}
		
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable declaration: "+str;
		throw error_info;
	}
}

// ���ܣ���������������������нṹ�����ͣ�������ṹ���еı�������
// ���������t_s-��������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(list<Type_specifier>* t_s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(t_s,"type specifier list's pointer");
		
		//������������
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();iter++)
		{
			//�жϵ�ǰ�����Ƿ�Ϊ�ṹ�����ͣ����Ҹýṹ�������Ƿ���г�Ա��������
			if(2==iter->get_stamp() 
			    &&(iter->get_struct_spec()->get_stamp()==0 
				|| iter->get_struct_spec()->get_stamp()==1))
			{
				//����ǽṹ�����ͣ����Һ��г�Ա�������������Ա��������
				for(list<Var_declaration>::iterator iter1=iter->get_struct_spec()->
						 get_var_dec_list()->begin();iter1!=iter->get_struct_spec()->
						 get_var_dec_list()->end();iter1++)
				{
					line_num=iter1->get_line_begin();//���õ�ǰ������λ�ڵ��к�
					replace_enumerator(&(*iter1));//�滻���������е�ö�ٳ�Ա
				}
			}
			else
			{
				//������ǽṹ�����ͣ������ǽṹ�����͵��޳�Ա�����������
				;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan type specifier list: "+str;
		throw error_info;
	}
}

// ���ܣ��������������б������ı������֣�����ñ�������ʱ�����г�ʼ�������������ʼ������
// ���������i_d-�������ı�������ָ��
// ���������i_d-�������ı�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Init_declarator* i_d)
{
	try
	{
		//���������������ı�������ָ�����Ч��
		check_pt_arg(i_d,"init_declarator object's pointer");
		
		//�ж������ı����ǲ����������
		if(1==i_d->get_dec()->get_stamp())
		{
			//�����ǩΪ1��˵�����������������������������±���ʽ
			for(list<Expression>::iterator iter=i_d->get_dec()->get_array_subs()->begin();
				iter!=i_d->get_dec()->get_array_subs()->end();iter++)
			{
				replace_enumerator(&(*iter));//�滻���ʽ�е�ö�ٳ�Ա
			}
		}
		else
		{
			//�����ǩ��Ϊ1����������
			;
		}
		
		//�ж������ı����Ƿ���г�ʼ��
		if(i_d->get_stamp()==1)
		{
			//��ǩΪ1��˵�����г�ʼ�����֣�������ʼ������
			replace_enumerator(i_d->get_init());
		}
		else
		{
			//��ǩ��Ϊ1��˵��������ʼ������������
			;
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan init_declarator : "+str;
		throw error_info;
	}
}

// ���ܣ��������������еĳ�ʼ�����֣���Ϊһ������ĳ�ʼ���Ͷ����顢�ṹ������ĳ�ʼ��
// ���������i-��ʼ�����ֶ���ָ��
// ���������i-��ʼ�����ֶ���ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Initializer* i)
{
	try
	{
		//����������ʼ�����ֶ���ָ�����Ч��
		check_pt_arg(i,"initializer object's pointer");
		
		//�жϳ�ʼ�������Ƿ�Ϊһ������ĳ�ʼ��
		if(i->get_stamp()==0)
		{
			//�����ǩΪ0��˵���Ƕ�һ������ĳ�ʼ��
			replace_enumerator(i->get_exp());//�������ڳ�ʼ���ı��ʽ
		}
		else
		{
			//�����ǩΪ��Ϊ0��˵���Ƕ�����������߽ṹ������ĳ�ʼ����������ʼ������
			for(list<Initializer>::iterator iter=i->get_init_list()->begin();
				iter!=i->get_init_list()->end();iter++)
			{
				replace_enumerator(&(*iter));//������ʼ�������е�ÿ��Ԫ��
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan initializer : "+str;
		throw error_info;
	}
}

// ���ܣ����������߱��ʽ�еı����������ö�ٳ�Ա��������滻
// ���������v-��������ָ��
// ���������v-��������ָ��
// ����ֵ���ɹ��򷵻�void ,ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Var* v)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(v,"variable object's pointer");
		
		//ƥ���������ı�ǩ
		switch(v->get_stamp())
		{
			case 0:
			{
				//��ǩΪ0��˵����һ��ı�����������
				;
			}
			break;
			
			case 1:
			{
				//��ǩΪ1��˵����һά����������������±���ʽ
				replace_enumerator(v->get_cexp());
			}
			break;
			
			case 2:
			{
				//��ǩΪ2��˵���Ǹ�ά����������������±���ʽ
				 for(list<Expression>::iterator iter=v->get_exp_list()->begin();
					iter!=v->get_exp_list()->end();iter++)
				 {
					replace_enumerator(&(*iter));
				 }
			}
			break;
			
			case 3:
			{
				//��ǩΪ3��˵���ǽṹ������������ṹ�����
				replace_enumerator(v->get_var());
			}
			break;
			
			case 4:
			{
				//��ǩΪ4��˵���Ƕ�ά�ṹ�����飬�����ṹ������������±�
				replace_enumerator(v->get_var());//�����ṹ�����
				
				for(list<Expression>::iterator iter=v->get_exp_list()->begin();
					iter!=v->get_exp_list()->end();iter++)
				 {
					replace_enumerator(&(*iter));//���������±���ʽ
				 }
			}
			break;
			
			default:
			{
				//��ǩΪ����ֵ���Ƿ����׳�����
				string error_info="variable object 's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable : "+str;
		throw error_info;
	}
}

// ���ܣ��������ʽ���滻���ʽ�е�ö�ٳ�Ա
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void replace_enumerator(Expression* s)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		//ƥ����ʽ��ǩ
		switch(s->get_stamp())
		{
			case 0:
			{
				//��ǩΪ0��˵���Ǻ��������������ı��ʽ
				replace_enumerator(s->get_cel());//��������������ʽ
				replace_enumerator(s->get_cer());//�����Ҳ��������ʽ
			}
			break;
				
			case 1:
			{
				//��ǩΪ1��˵���Ǻ��е��������ı��ʽ
				replace_enumerator(s->get_cer());//�����Ҳ��������ʽ
			}
			break;
				
			case 2:
			{
				//��ǩΪ2��˵�������ű��ʽ
				replace_enumerator(s->get_exp());//���������еı��ʽ
			}
			break;
				
			case 3:
			{
				//��ǩΪ3��˵���ǳ������ʽ����������
				;
			}
			break;
				
			case 4:
			{
				//��ǩΪ4��˵���Ǳ������ʽ
				//�жϸñ���������
				if(s->get_cvar()->get_stamp()==0)
				{
					//������ǩΪ0��˵����һ��ı��������Ҹñ����ǲ���ö�ٳ�Ա�������滻��һ������
					int tmp=0;	//�洢�ñ�����Ӧ������ֵ
					string str1="";//�洢���ҵĽ��
					//��ȫ�ֵ�ö�ٳ�Ա��Ϣ���в��Ҹñ���
               str1=find_enumerator_value(enum_mems,s->get_cvar()->get_name(),&tmp);
					//�жϲ��ҽ��
					if(str1=="right")
					{
						//������ҳɹ����򽫴˱������ʽ�޸ĳɳ������ʽ
						s->set_stamp(3);//�޸ĸñ��ʽ�ı�ǩ
						s->set_c(int2str(tmp));//�޸ĸñ��ʽ����������
					}
					else
					{
						//������Ҳ��ɹ���˵���ñ�������ö�ٳ�Ա�������滻
						;
					}
				}
				else
				{
					//������ǩ��Ϊ0������ñ�������������������
					replace_enumerator(s->get_cvar());
				}
			}
			break;
				
			case 5:
			{
				//��ǩΪ5��˵��Ϊ�������ñ��ʽ�����������Ĳ������ʽ
				for(list<Expression>::iterator iter=s->get_args()->begin();
					iter!=s->get_args()->end();iter++)
				 {
					replace_enumerator(&(*iter));//�����������ʽ
				 }
			}
			break;
			
			default:
			{
				//��ǩΪ����ֵ���Ƿ����׳�����
				string error_info="expression object's stamp is invalid!";
				throw error_info;
			}
		}	
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan expression : "+str;
		throw error_info;
	}
}

// ���ܣ�������������������������������к����壬�����������е��������滻ö�ٳ�Ա
// ���������f_d-������������ָ��
// ���������f_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Fun_declaration* f_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(f_d,"function declaration object's pointer");
		
		//�жϺ��������Ƿ���ж���
		if(f_d->get_stamp()==0)
		{
			//����������ǩΪ0��˵�����ж��岿��
			scope=f_d->get_fun_name();//�����������óɺ�������
			replace_enumerator(f_d->get_ccstmt());//���������Ķ��岿��
			scope="global";//����������Ϊ"global"
		}
		else
		{
			//����������ǩ��Ϊ0��˵���������岿�֣�����
			;
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan function declaration : "+str;
		throw error_info;
	}
}

// ���ܣ�����������䣬������������еı�����������䣬�滻ö�ٳ�Ա
// ���������c-����������ָ��
// ���������c-����������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Compound_stmt* c)
{
	try
	{
		//������������������ָ�����Ч��
		check_pt_arg(c,"compound statement object's pointer");
		
		//������������е���������
		for(list<Declaration>::iterator iter=c->get_cdl()->begin();
			   iter!=c->get_cdl()->end();iter++)
		{
			//�ж����������Ƿ�Ϊ��������
			if(0==iter->get_stamp())
			{
				//�����������ı�ǩΪ0��˵��Ϊ��������
				replace_enumerator(iter->get_cvd());//�����ñ�������	
			}
			else
			{
				//��ǩ��Ϊ0��˵�����Ǳ�������������
				;
			}
		}
		//������������е��������
		for(list<Statement>::iterator iter=c->get_csl()->begin();
		       iter!=c->get_csl()->end();iter++)
		{
			replace_enumerator(&(*iter));//�������
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan compound statement : "+str;
		throw error_info;
	}
}

// ���ܣ�������䣬�滻��ͬ����е�ö�ٳ�Ա
// ���������s-������ָ��
// ���������s-������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_enumerator(Statement* s)
{
	try
	{
		//��������������ָ�����Ч��
		check_pt_arg(s,"statement object's pointer");
		
		//�ж��������
		switch(s->get_stamp())
		{
			case 0:
			{
				//��ǩΪ0��˵���Ǹ�����䣬�����������
				replace_enumerator(s->get_ccstmt());
			}
			break;
			case 1:
			{
				//��ǩΪ1��˵���Ƿ�֧��䣬������֧���ĸ�������
				//������֧������ڱ��ʽ
				replace_enumerator(s->get_csstmt()->get_ce());
				//�жϷ�֧����Ƿ�Ϊ����֧
				if(s->get_csstmt()->get_stamp()==0)
				{
					//��ǩΪ0��˵��ֻ����if��֧
					replace_enumerator(s->get_csstmt()->get_cs_if());//����if��֧�е����
				}
				else
				{
					//��ǩ��Ϊ0��˵������if��else��֧
					replace_enumerator(s->get_csstmt()->get_cs_if());//����if��֧�е����
					replace_enumerator(s->get_csstmt()->get_cs_else());//����else��֧�е����
				}
			}
			break;
			case 2:
			{
				//��ǩΪ2��˵����ѭ����䣬����ѭ�����ĸ�������
				replace_enumerator(s->get_cistmt()->get_ce());//����ѭ����ڱ��ʽ
				replace_enumerator(s->get_cistmt()->get_cs());//����ѭ�����ڵ����
			}
			break;
			case 3:
			{
				//��ǩΪ3��˵���Ƿ������
				//�жϷ�������Ƿ��з���ֵ
				if(s->get_crs()->get_stamp()==1)
				{
					//�����ǩΪ1��˵���з���ֵ���������صı��ʽ
					replace_enumerator(s->get_crs()->get_ce());
				}
				else
				{
					//�����ǩ��Ϊ1��˵��û�з���ֵ������
					;
				}
			}
			break;
			case 4:
			{
				//��ǩΪ4��˵���Ǹ�ֵ��䣬������ֵ������ֵ�������ұ��ʽ
				replace_enumerator(s->get_cvar());//������ֵ����
				replace_enumerator(s->get_ce());//�����ұ��ʽ
			}
			break;
			case 5:
			{
				//��ǩΪ5��˵���Ǻ���������䣬������������
				replace_enumerator(s->get_ce());
			}
			break;
			case 6:
			{
				//��ǩΪ6��˵���Ƿֺ���䣬����
				;
			}
			break;
			default:
			{
				//��ǩΪ����ֵ���Ƿ����׳�����
				string error_info="statement object's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan statement : "+str;
		throw error_info;
	}
}



