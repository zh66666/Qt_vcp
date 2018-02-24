#include "program_struct.h"

// �ýṹ�����ڴ洢ö�ٳ�Ա�������Ϣ�����������֣����Ӧ������ֵ�Լ���������������
// ���� : ��ȫ��������������enum {a,b,c}; ��ô���ڴ洢ö�ٳ�Աa�Ľṹ���ԱΪ������Ϊa��
// ֵΪ0��������Ϊ��global��
struct enum_mem
{
	string name_;	//ö�ٳ�Ա������
	int value_;			//ö�ٳ�Ա��Ӧ������ֵ
	string scope_;	//ö�ٳ�Ա��������
};

// �������ļ��� mian_utils.cpp
extern int line_num;
extern int str2int(string);
//2010��3��10��׷��
extern string file_info;

extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);



// �������ļ���src/back_end/back_end_utils.cpp
extern string compute_exp(Expression* s,int* result);

//�������ļ���./replace_enum.cpp
extern void replace_enumerator(Expression* s);

//�洢��ǰ�������ڵ�����������ں����⣬����"global"��������ֵΪ��������
static string scope="global";	

//�洢��ǰ�������C�ļ��е�����ö�ٳ�Ա��Ϣ
list<enum_mem>* enum_mems=new list<enum_mem>;


static void scan_enum(Var_declaration* v_d);
static void scan_enum(list<Type_specifier>* t_s);
static void scan_enum(Struct_specifier* s);
static void scan_enum(Enum_specifier* s);
static void scan_enum(Fun_declaration* s);
void scan_enum(Program *s);

// ���ܣ��ڴ洢��ö�ٳ�Ա��Ϣ�������У��������ƣ�����ĳö�ٳ�Ա������ֵ��
// 			������Ϊ�������أ�ͬʱ���ҳɹ��򷵻ء�right�������򷵻ء�wrong��
// ���������l-�洢������ö�ٳ�Ա��Ϣ������ָ��
// 			      name-ö�ٳ�Ա������
// ���������result-���ҳɹ�ʱ���洢ö�ٳ�Ա��Ӧ������ֵ
// ����ֵ������ִ�гɹ��򷵻�string��ʧ�����׳�������Ϣ
// ���ߣ����
string find_enumerator_value(list<enum_mem>* l,string name,int* result)
{
	try
	{
		//��������ö�ٳ�Ա��Ϣ����ָ�����Ч��
		check_pt_arg(l,"enumerator information list 's pointer");
		//��������ö�ٳ�Ա�����ַ�������Ч��
		check_str_arg(name,"enumerator 's name");
		
		list<enum_mem>::iterator iter;//ö�ٳ�Ա��Ϣ����ĵ�����
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//�жϵ�ǰ��������ָԪ�ص������봫���ö�ٳ�Ա�������Ƿ�һ��
			if(iter->name_==name)
			{
				//�����ͬ����ȡ��������ֵ��������"right"
				*result=iter->value_;
				return "right";
			}
			else
			{
				//�������ͬ����ѭ������
				;
			}
		}
		//���ҹ�����û��������˵��û���ҵ���ö�ٳ�Ա��Ӧ������ֵ������"wrong"
		return "wrong";
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="find enumerator : "+str;
		throw error_info;
	}
}


// ���ܣ���ö�ٳ�Ա��Ϣ���������ڴ洢ʱ��������������Ƿ��Ѿ��и�ö�ٳ�Ա��
 // 			������У��򲻱��棬���򱣴��ö�ٳ�Ա�������Ϣ
// ���������l-���ڴ洢����ö�ٳ�Ա��Ϣ������ָ��
//                  e-����ĳ��ö�ٳ�Ա��Ϣ�Ľṹ�����
// ���������l-���ڴ洢����ö�ٳ�Ա��Ϣ������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void store_enum(list<enum_mem>* l,struct enum_mem e)
{
	try
	{
		//��������ö�ٳ�Ա��Ϣ����ָ�����Ч��
		check_pt_arg(l,"enumerator information list 's pointer");
		
		//�ȼ�鴫���ö�ٳ�Ա�Ƿ����������Ѿ�����
		list<enum_mem>::iterator iter;
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//�жϵ�ǰ��������ָ��Ԫ�ص������Ƿ��
			//�����е�ö�ٳ�Ա������һ��
			if(iter->name_==e.name_ )
			{
				//һ�£�˵���������Ѿ����и�ö�ٳ�Ա����Ϣ������ѭ��
				break;
			}
			else
			{
				//��һ�£���ѭ������
				;
			}
		}
		//�������֮�󣬼��������Ƿ��������ĩ��
		if(iter==l->end())
		{
			//�������ĩ�ˣ�˵�������в����ڸ�ö�ٳ�Ա����Ϣ���������
			l->push_back(e);
		}
		else
		{
			//���������û�е�ĩ�ˣ�˵��ѭ����;��������ζ�����������и�ö�ٳ�Ա����Ϣ
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="store enumerator : "+str;
		throw error_info;
	}
}

// ���ܣ������м�ṹ��ɨ��ö�����ͣ����洢ö�ٳ�Ա��Ϣ��ͬʱ��ö�����͸ĳ�int��
// ���������s-�м�ṹָ��
// ���������s-�м�ṹָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
void scan_enum(Program *s)
{
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(s,"program struct 's pointer");
		
		enum_mems->clear();	//���洢��ö�ٳ�Ա��Ϣ��ȫ���������
		
		//�����м�ṹ�е���������
		for(list<Declaration>::iterator iter=s->get_declist()->begin();
		   iter!=s->get_declist()->end();iter++)
		{
			//2010��3��10��׷��
			file_info=iter->get_file_info();//���õ�ǰ������λ�ڵ��ļ���
			
			//�ж��Ƿ�Ϊ��������
			if(0==iter->get_stamp())
			{
				//Ϊ0˵���Ǳ�������
				line_num=iter->get_line_begin();	//�޸ĵ�ǰ������λ�ڵ��к�
				scan_enum(iter->get_cvd());//������������
			}
			else
			{
				//��Ϊ0��˵���Ǻ�������
				line_num=iter->get_line_begin();	//�޸ĵ�ǰ������λ�ڵ��к�
				scan_enum(iter->get_cfd());//������������
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan enum : "+str;
		throw error_info;
	}
}

// ���ܣ���������������ɨ��ö������
// ���������v_d-������������ָ��
// ���������v_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_enum(Var_declaration* v_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(v_d,"variable declaration object 's pointer");
		//����������������������
		scan_enum(v_d->get_decl_spec()->get_type_spec_list());
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable declaration : "+str;
		throw error_info;
	}
}

// ���ܣ�������������ɨ��ö������
// ���������t_s-��������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_enum(list<Type_specifier>* t_s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(t_s,"type specifier list 's pointer");
		//������������
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();iter++)
		{
			//�жϸ������͵ı�ǩ
			if(2==iter->get_stamp())
			{
				//��ǩΪ2��˵��������Ϊ�ṹ������
				scan_enum(iter->get_struct_spec());//�����ýṹ������
			}
			else if(3==iter->get_stamp())
			{
				//��ǩΪ3��˵��������Ϊö������
				scan_enum(iter->get_enum_spec());//������ö������
				
				//ö�����ͱ�������֮���޸ĸ�ö������Ϊint����
				iter->set_stamp(0);
				iter->set_type_name("int");
			}
			else
			{
				//������ǩ���������κβ���
				;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan type specifier list : "+str;
		throw error_info;
	}
}

// ���ܣ�����ö�����ͣ�ɨ��ö�ٳ�Ա
// ���������s-ö�����Ͷ���ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_enum(Enum_specifier* s)
{
	try
	{
		//��������ö�����Ͷ���ָ�����Ч��
		check_pt_arg(s,"enumerator specifier object's pointer");
		
		//�ж�ö�������Ƿ����ö�ٳ�Ա����
		if(0==s->get_stamp() || 1==s->get_stamp())
		{
			//��ǩΪ0����1��˵������ö�ٳ�Ա�����������ö�ٳ�Ա
			int k=0;		//��¼ö�ٳ�Ա��Ӧ������ֵ
			
			//ɨ��ö�ٳ�Ա
			for(list<Enumerator>::iterator iter=s->get_enum_list()->begin();
			       iter!=s->get_enum_list()->end();iter++,k++)
			{
				//�жϸ�ö�ٳ�Ա�Ƿ���г�ʼ��
				if(0==iter->get_stamp())
				{
					//��ǩΪ0��˵����ö�ٳ�Ա������ʼ��
					//���� enum abc {a,b=3,c}�еĳ�Աa��c
					struct enum_mem e1;						//�洢��ö�ٳ�Ա����Ϣ
					e1.name_=iter->get_element();	//�洢��ö�ٳ�Ա������
					e1.value_=k;										//�洢��ö�ٳ�Ա��Ӧ������ֵ
					e1.scope_=scope;							//�洢��ö�ٳ�Ա��������
					
					store_enum(enum_mems,e1);		//����ö�ٳ�Ա��Ϣ����ȫ������
				}
				else
				{
					//��ǩ��Ϊ0��˵����ö�ٳ�Ա���г�ʼ��������enum abc {a,b=3,c}�еĳ�Աb
					//�Ƚ���ʼ�����ʽ�滻�ɳ������ʽ
					//���磺enum abc {a,b=a+3,c};�еĳ�Աb�ĳ�ʼ�����ʽΪa+3����Ҫ�����滻��1+3												 
					replace_enumerator(iter->get_exp());	//�滻��ʼ�����ʽ
					int result=0;	//�洢��ʼ�����ʽ��ֵ
					string str=compute_exp(iter->get_exp(),&result);//�����ʼ�����ʽ��ֵ
					
					//�жϳ�ʼ�����ʽ�����Ƿ�ɹ�
					if(str=="right")
					{
						//˵������ɹ�
						struct enum_mem e1;						//���ڴ洢��ö�ٳ�Ա����Ϣ
						e1.name_=iter->get_element();	//�洢��ö�ٳ�Ա������
						e1.value_=result;								//�洢��ö�ٳ�Ա��Ӧ������ֵ
						e1.scope_=scope;							//�洢��ö�ٳ�Ա��������
						
						store_enum(enum_mems,e1);		//����ö�ٳ�Ա��Ϣ����ȫ������
						
						k=result;											//��k��ֵ��Ϊ��ǰö�ٳ�Ա������ֵ
					}
					else
					{
						//������㲻�ɹ������׳�������Ϣ
						string error_info="invalid enumerator!";
						throw error_info;
					}
				}
			}
		}
		else
		{
			//������ǩ˵����ö�����Ͳ���ö�ٳ�Ա�������Ͳ����б���
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan enumerator specifier : "+str;
		throw error_info;
	}
}

// ���ܣ������ṹ�����ͣ��ṹ���е�ö������
// ���������s-�ṹ�����Ͷ���ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_enum(Struct_specifier* s)
{
	try
	{
		//���������ṹ�����Ͷ���ָ�����Ч��
		check_pt_arg(s,"struct specifier object's pointer");
		//�жϸýṹ�������Ƿ��нṹ���Ա��������
		if(s->get_stamp()==0 || s->get_stamp()==1)
		{
			//�����ǩΪ0����1��˵���ýṹ�����ͺ��г�Ա��������
			//�����ṹ��ĳ�Ա��������
			for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
				iter!=s->get_var_dec_list()->end();iter++)
			{
				line_num=iter->get_line_begin();	//�޸ĵ�ǰ������λ�ڵ��к�
				scan_enum(&(*iter));						//�����ṹ���еı�������
			}
		}
		else
		{
			//�����ǩΪ����ֵ��˵���޳�Ա��������
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan struct specifier : "+str;
		throw error_info;
	}
}

// ���ܣ���������������ɨ��ö�����ͣ��������������ķ���ֵ�������������β���������
// 			�������ж����ʱ�򣬻�Ҫ�����������еı�������
// ���������s-������������ָ��
// ���������s-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_enum(Fun_declaration* s)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(s,"function declaration object's pointer");
		
		scan_enum(s->get_decl_spec()->get_type_spec_list());//������������ֵ��������
		
		//�����������β�����
		for(list<Param>::iterator iter=s->get_params()->begin();
			   iter!=s->get_params()->end();iter++)
		{
			scan_enum(iter->get_type_spec_list());//���������β���������
		}
		//�жϸú��������Ƿ���ж��岿��
		if(s->get_stamp()==0)
		{
			//������ǩΪ0��˵�����ж��岿��
			scope=s->get_fun_name();//����������Ϊ�ú���������
			
			//�����������е�����
			for(list<Declaration>::iterator iter=s->get_ccstmt()->get_cdl()->begin();
		       iter!=s->get_ccstmt()->get_cdl()->end();iter++)
			{
				//�ж��Ƿ�Ϊ��������
				if(0==iter->get_stamp())
				{
					//Ϊ0˵���Ǳ�������
					line_num=iter->get_line_begin();	//�޸ĵ�ǰ������λ�ڵ��к�
					scan_enum(iter->get_cvd());//������������
				}
				else
				{
					//�Ƿ���ǩֵ���׳�������Ϣ
					string error_info="in function's body ,declaration 's stamp is invalid!";
					throw error_info;
				}
			}
			
			scope="global";//��������������󣬽�����������Ϊ"global"
		}
		else
		{
			//������ǩ��Ϊ0��˵���������岿��
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







