#include "program_struct.h"


//�������ļ� : src/main_utils.cpp
extern int line_num;

//2010��3��10��׷��
extern string file_info;

extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);

typedef list<Type_specifier>* TSLP;

//�洢��ǰ�������ڵ�����������ں����⣬����"global"��������ֵΪ��������
static string scope="global";	

//���ڴ洢�û��Զ������͵������Ϣ�������Զ������͵�ԭʼ���ͣ��Զ�����������
//���磺��������typedef unsigned int UINT_32; 
//��ԭʼ����Ϊunsigned int���Զ�����������ΪUINT_32
struct type_define
{
	TSLP type_spec_list_;				//ԭʼ����
	string type_name_;					//�Զ���������
};

typedef list<type_define>* TDLP;

// ���ڴ洢ɨ�赽���û��Զ���������Ϣ
static TDLP type_defines=new list<type_define>;

static void replace_typedef(TSLP t_s);
static void replace_typedef(Var_declaration* v_d);
static void replace_typedef(Fun_declaration* f_d);
void replace_typedef(Program *s);

// ���ܣ����ݴ�����Զ��������������Զ���������Ϣ���в��Ҷ�Ӧ��ԭʼ����
// ���������s-�洢���Զ���������Ϣ������ָ��
//                  str_name-�Զ���������
// ����ֵ���ɹ��򷵻�ԭʼ��������ָ�룬ʧ�����׳�������Ϣ
// ���ߣ����
static TSLP find_type_define(TDLP s,string str_name)
{
	try
	{
		//���������Զ���������Ϣ����ָ�����Ч��
		check_pt_arg(s,"typedef list's pointer");
		//���������Զ����������ַ�������Ч��
		check_str_arg(str_name,"user-defined name ");
		
		TSLP tmp=new list<Type_specifier>;							//�洢���ҵ���ԭʼ��������ָ��
		list<type_define>::iterator iter;	//�Զ���������Ϣ��ĵ�����
		for(iter=s->begin();iter!=s->end();iter++)
		{
			//�жϵ�ǰ��������ָԪ�ص��Զ����������봫��������Ƿ����
			if(iter->type_name_==str_name)
			{
				//�����ȣ�˵�����ҳɹ�
				for(list<Type_specifier>::iterator iter1=iter->type_spec_list_->begin();
				iter1!=iter->type_spec_list_->end();iter1++)
				{
					tmp->push_back(*iter1);
				}
	
				//tmp=iter->type_spec_list_;
				break;
			}
			else
			{
				//����ȣ����������
				;
			}
		}
		//ѭ������֮���жϵ������Ƿ��������ĩ��
		if(s->end()==iter)
		{
			//�������ĩ�ˣ�˵��û���ҵ����׳�������Ϣ	
			string error_info="can't find user define name: "+str_name;
			throw error_info;
		}
		else
		{
			//û��ĩ�ˣ�˵�����ҳɹ�������
			;
		}
		return tmp;	//�����ҵ���ԭʼ��������ָ��
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="finding user defined name : "+str;
		throw error_info;
	}
}

// ���ܣ����������������洢���滻�Զ������ͣ�ע�ⲻ���ھֲ���typedef����
// ���������v_d-������������ָ��
// ���������v_d-������������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_typedef(Var_declaration* v_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(v_d,"variable declaration's pointer");
		
		//�ȱ����ñ�������������
		replace_typedef(v_d->get_decl_spec()->get_type_spec_list());
		//�жϱ��������ı�ǩ�Ƿ�Ϊ1�����������Ĵ洢�����Ƿ�Ϊ��typedef��
		if(1==v_d->get_stamp() && "typedef" == v_d->get_decl_spec()->get_stor_spec())
		{
			//�ǣ���˵��������Ϊ�Զ�����������
			//�жϵ�ǰ�������Ƿ���ȫ��
			if("global"==scope)
			{
				//��ȫ�֣���洢�Զ���������Ϣ
				struct type_define td_tmp;		//����ʱ�ṹ�����ڴ洢�Զ���������Ϣ
				TSLP tslp_tmp=new list<Type_specifier>;	//����ʱ�������ڴ洢ԭʼ����
				//���������е�ԭʼ���Ͳ�����ʱ����
			tslp_tmp->insert(tslp_tmp->end(),v_d->get_decl_spec()->get_type_spec_list()->
											  begin(),v_d->get_decl_spec()->get_type_spec_list()->end());
											  
				//������ʱ�ṹ��ĳ�Ա
				td_tmp.type_spec_list_=tslp_tmp;	
				td_tmp.type_name_=v_d->get_inits()->front().get_dec()->get_name();
				
				type_defines->push_back(td_tmp);	//����ʱ�ṹ���������ȫ��������
				
				//delete tslp_tmp;
			}
			else
			{
				//����ȫ�֣����׳�������Ϣ
				string error_info="local typedef is not allowed!";
				throw error_info;
			}	
		}
		else
		{
			//���ǣ�˵�������������Զ���������������������
			;
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable declaration : "+str;
		throw error_info;
	}
}

// ���ܣ����������������Զ�����������滻��ԭʼ����
// ���������t_s-��������ָ��
// ���������t_s-��������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_typedef(TSLP t_s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(t_s,"type specifier list's pointer");
		
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();)
		{
			//ƥ�����͵ı�ǩֵ
			switch(iter->get_stamp())
			{
				case 1:
				{
					//��ǩΪ1��˵�����Զ������ͣ����滻֮
					string type_name=iter->get_type_name();		//�洢�Զ���������
					//��ȫ���Զ���������Ϣ�����У����Ҹ��Զ�����������Ӧ��ԭʼ��������
					TSLP ts_tmp=find_type_define(type_defines,type_name);
					iter=t_s->erase(iter);//����ǰ���Զ���������ɾ��
					t_s->insert(iter,ts_tmp->begin(),ts_tmp->end());//�ڵ�ǰλ�ò���ԭʼ��������
				}
				break;
				case 2:
				{
					//��ǩΪ2��˵���ǽṹ�����ͣ�������б���
					int i=iter->get_struct_spec()->get_stamp();	//�洢�ṹ�����͵ı�ǩ
					//�жϱ�ǩ��ֵ
					if(i==0 || i==1)
					{
						//��ǩֵΪ0����1��˵���ýṹ�庬�г�Ա�������������Ա����
						for(list<Var_declaration>::iterator iter_in=iter->get_struct_spec()->
							  get_var_dec_list()->begin();iter_in!=iter->get_struct_spec()->
							  get_var_dec_list()->end();iter_in++)
						{
							line_num=iter_in->get_line_begin();//���õ�ǰ����λ�ڵ��к�
							replace_typedef(&(*iter_in));	//�Խṹ���ڵĳ�Ա�������б���
						}
					}
					else
					{
						//��ǩֵΪ����ֵ��˵��������Ա��������������
						;
					}
					iter++;
					
				}
				break;
				
				default:
				{
					//��ǩΪ����ֵʱ������������������1
					iter++;
				}	
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan type specifier list : "+str;
		throw error_info;
	}
}

// ���ܣ������������������Զ��������滻��ԭʼ���͡�������Ҫ�������к����ķ���ֵ���ͣ�
//           �����Ĳ������ͣ��������ڵı�������
// ���������f_d-������������ָ��
// ���������f_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void replace_typedef(Fun_declaration* f_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(f_d,"function declaration object's pointer");
		
		//�ȱ��������ķ���ֵ��������
		replace_typedef(f_d->get_decl_spec()->get_type_spec_list());
		
		//���������Ĳ�����������
		for(list<Param>::iterator iter=f_d->get_params()->begin();
		       iter!=f_d->get_params()->end();iter++)
		{
			replace_typedef(iter->get_type_spec_list());//����ÿ����������������
		}
		//�жϺ��������ı�ǩ
		if(0==f_d->get_stamp())
		{ 
			//��ǩΪ0��˵�����������к������壬����������
			scope=f_d->get_fun_name();//��������������óɸú���������
			
			//�����������еı����������滻�Զ�������
			for(list<Declaration>::iterator iter=f_d->get_ccstmt()->get_cdl()->begin();
				   iter!=f_d->get_ccstmt()->get_cdl()->end();iter++)
			{
				line_num=iter->get_line_begin();	//�޸ĵ�ǰ������λ�ڵ��к�
				//�ж������ı�ǩ
				if(0==iter->get_stamp())
				{
					//Ϊ0��˵���Ǳ���������������������
					replace_typedef(iter->get_cvd());
				}
				else
				{
					//������ǩֵ��˵�����Ǳ�������������
					;
				}
			}
			scope="global";//�������еı��������������֮���������������Ϊ��global��
		}
		else
		{
			//��ǩ��Ϊ0��˵��������û�ж��岿�֣�����
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan function declaration : "+str;
		throw error_info;
	}
}

// ���ܣ����м�ṹ�е��Զ������ͽ����滻�������滻��ԭʼ����
// ���������s-�м�ṹ����ָ��
// ���������s-�м�ṹ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void replace_typedef(Program *s)
{
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(s,"program struct 's pointer");
		
		type_defines->clear();	//���洢�Զ���������Ϣ���������
		//�����ṹ�е����������滻�Զ�������
		for(list<Declaration>::iterator iter=s->get_declist()->begin();
		       iter!=s->get_declist()->end();iter++)
		{
			line_num=iter->get_line_begin();	//�޸ĵ�ǰ������λ�ڵ��к�
			//2010��3��10��׷��
			file_info=iter->get_file_info();//�޸ĵ�ǰ������λ�ڵ��ļ���
			
			//�ж��Ƿ�Ϊ��������
			if(0==iter->get_stamp())
			{
				//Ϊ0��˵���Ǳ���������������������
				replace_typedef(iter->get_cvd());
			}
			else
			{	
				//��Ϊ0��˵���Ǻ���������������������
				replace_typedef(iter->get_cfd());
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="replace typedef  : "+str;
		throw error_info;
	}
}



