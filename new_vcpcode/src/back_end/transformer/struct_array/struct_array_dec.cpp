#include "struct_array.h"

//�������ļ���src/back_end/back_end_utils.cpp
extern string compute_exp(Expression* s,int* result);

// �������ļ�: src/main_utils.cpp
extern int line_num;

//2010��3��10��׷��
extern string file_info;

extern int str2int(string);
extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);

// �������ļ� ��./struct_array_stmt.cpp
extern void scan_struct_array_stmtlist(list<Statement>* s);

//��¼��������Ϣ��Ĭ��Ϊ"global"����ȫ�֣����Ժ�������б���ʱ����Ϊ����������
static string scope="global";
//��¼�ṹ����������������������ʱ�ı�ǩ��Ϣ������ñ����ǽṹ���ڵĳ�Ա���򽫴˱�ǩ����Ϊ
//�ṹ�����͵����ƣ�������ǽṹ���ڵı���������Ϊ"common"
string stamp="common";
//��¼�Ե�ǰ�м�ṹ�е����нṹ��������Ϣ������
list<Struct_info>* structs=new list<Struct_info>;
//��¼��ǰ�м�ṹ�е����нṹ�������Ϣ�����������ṹ���������Ľṹ�����
list<struct_var>* struct_vars=new list<struct_var>;
//��¼��ǰ�м�ṹ�е��������������Ϣ�����������ṹ�����������������
list<Array_info>* arrays=new list<Array_info>;

static void scan_struct_array_declist(list<Declaration>* d_s);
static void scan_struct_array_vardec(Var_declaration* v_d);

// ���ܣ��ڽṹ����Ϣ�����и��ݴ���Ľṹ�����ƣ��������Ӧ�Ľṹ����Ϣ������ҵ����򷵻�"yes"
//			�����ҽ��ҵ�����Ϣ����һ�ݣ�ͨ��������������򷵻�"no"
// ���������l-�洢�нṹ����Ϣ������ָ��
//			     struct_name-Ҫ���ҵĽṹ��������
// ���������tmp-�ṹ����Ϣ����ָ��
// ����ֵ���ɹ��򷵻ز��ҵĽ����Ϣ��ʧ�����׳�������Ϣ
// ���ߣ����
// ע�⣺�������������"yse"����ʹ��newΪ���ص�ָ��������ڴ棬��Ҫ�ϲ㺯�����ͷţ��������
//			"no"������Ҫ�ͷ�
static string find_struct_info(list<Struct_info>* l,string struct_name,Struct_info*& tmp)
{
	try
	{
		//���������ṹ����Ϣ����ָ�����Ч��
		check_pt_arg(l,"struct  information list's pointer");
		//���������ṹ�����������ַ�������Ч��
		check_str_arg(struct_name,"struct's name");
		
		string result="";//�洢���ҵĽ����Ϣ
		list<Struct_info>::iterator iter;//�ṹ����Ϣ���������
		//�����ṹ����Ϣ����
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//�жϵ�ǰ��������ָ����������Ƿ���Ҫ���ҵĽṹ������һ��
			if(struct_name==iter->get_struct_name())
			{
				//���һ�£����俽��������Ĳ���
				tmp=new Struct_info(*iter);
				result= "yes";//���ҽ����Ϊ"yes"
				break;
			}
			else
			{
				//�����һ�£���������
			}
		}
		//�����������жϵ������Ƿ�ָ���������ĩ��
		if(iter==l->end())
		{
			//����ǣ��򽫲�����Ϊ"no"
			result= "no";
		}
		return result;
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="find struct information : "+str;
		throw error_info;
	}
}

// ���ܣ����������±���ʽ������������Ԫ�صĸ�����������������������Ϣ�洢����
// ���������exp_subs-�±���ʽ����ָ��
// 			      array_name-�����������
// ����ֵ���ɹ��򷵻ؼ������������Ԫ�ظ�����ʧ�����׳�������Ϣ
// ���ߣ����
static int compute_array_sub(list<Expression>* exp_subs,string array_name)
{
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(exp_subs,"expression list's pointer");
		//��������������������ַ�������Ч��
		check_str_arg(array_name,"array varibale's name");
		int num=1;//��¼�����Ԫ�ظ�������ʼ��Ϊ1
		vector<int>* subs=new vector<int>;//��¼������±�ֵ
		//����������±���ʽ����
		for(list<Expression>::iterator iter=exp_subs->begin();iter!=exp_subs->end();iter++)
		{
			int constant=0;//�洢�Ե�ǰ�±���ʽ���м���Ľ��
			string str_result=compute_exp(&(*iter),&constant);//�Ե�ǰ��������ָ�ı��ʽ���м���
			//�ж��Ƿ����ɹ�
			if("right"==str_result)
			{
				//�������ɹ����򽫼���������±�ֵѹջ������������Ԫ�ظ���
				subs->push_back(constant);
				num=num*constant;
			}
			else
			{
				//�������ʧ�ܣ�˵���������±겻Ϊ�������׳�������Ϣ
				string error_info="array "+array_name+"'s subscript is not constant!";
				throw error_info;
			}
		}
		//��ȫ�ֵ����������Ϣ���в��Ҹ�������������Ƿ���ڣ�������ڣ��򲻴洢����������Ϣ����
		//ȫ�ֵ����������Ϣ������
		list<Array_info>::iterator iter;//������Ϣ���������
		//���������������Ϣ��ȫ������
		for(iter=arrays->begin();iter!=arrays->end();iter++)
		{
			//�жϵ�ǰ��������ָ����������ƺͱ�ǩ��Ϣ�봫�����������Ƿ�һ��
			if(array_name==iter->get_name() && stamp==iter->get_stamp())
			{
				//���һ�£�˵������������洢������������
				break;
			}
			else
			{
				//�����һ�£����������
			}
		}
		//�жϵ������Ƿ�ָ�������ĩ��
		if(arrays->end()==iter)
		{
			//����ǣ���˵�����������֮ǰû�д洢������洢
			Array_info* new_array=new Array_info(array_name,subs);//����������Ϣ����
			new_array->set_scope(scope);//����������Ϣ�����������
			new_array->set_stamp(stamp);//����������Ϣ����ı�ǩ
			arrays->push_back(*new_array);//�洢��������Ϣ����
			delete new_array;
		}
		delete subs;
		return num;//���ظ������Ԫ�ظ���
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="compute array's subscript : "+str;
		throw error_info;
	}
}

// ���ܣ�������Ľṹ����Ϣ���뵽�ṹ����Ϣ������,����Ѵ��ڣ��򲻲���
// ���������l-�洢�ṹ����Ϣ������ָ��
// ���������tmp-�ṹ����Ϣ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
 static void store_struct(list<Struct_info>* l,Struct_info* tmp)
{
	try
	{
		//���������ṹ����Ϣ����ָ�����Ч��
		check_pt_arg(tmp,"struct  information object's pointer");
		//���������ṹ����Ϣ����ָ�����Ч��
		check_pt_arg(l,"struct  information list's pointer");
		
		list<Struct_info>::iterator iter;//�ṹ����Ϣ���������
		//�����ṹ����Ϣ����
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//�жϵ�ǰ��������ָ�Ķ��������Ƿ��봫��Ľṹ�������һ��
			if(iter->get_struct_name()==tmp->get_struct_name())
			{
				//���һ������˵���ýṹ����Ϣ�Ѿ��洢��������
				break;
			}
			else
			{
				//�����һ�������������
			}
		}
		//ѭ���������жϵ������Ƿ�ָ���������ĩ��
		if(iter==l->end())
		{
			//����ǣ���˵������Ľṹ����Ϣû�д洢���������������
			l->push_back(*tmp);
		}
		else
		{
			//���û�е�ĩ�ˣ�����
		}
		return ;
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="store struct information : "+str;
		throw error_info;
	}
}

// ���ܣ������ṹ�����ͣ��������Ա��������Ϣ�洢������������
// ���������s-�ṹ�����Ͷ���ָ��
// ����ֵ���ɹ��򷵻ؽṹ����Ϣ����ָ�룬ʧ�����׳�������Ϣ
// ���ߣ����
// ע�⣺������ʹ��newΪ���ص�ָ��������ڴ棬��Ҫ�ϲ㺯�����ͷ�
static Struct_info* scan_struct_body(Struct_specifier* s)
{
	try
	{
		//���������ṹ�����Ͷ���ָ�����Ч��
		check_pt_arg(s,"struct  specifier's pointer");
		
		Struct_info* s_info_temp=NULL;//���ڴ洢�ṹ��������Ϣ��ָ�룬��ʼ��ΪNULL
	
		//�жϸýṹ�������Ƿ���г�Ա����
		if(s->get_stamp()==0)
		{
			//������г�Ա�������������Ա
			string struct_name=s->get_name();//ȡ���ýṹ����������
			int mem_num=0;//��¼�ṹ���Ա�����λ�ã���ʼ��λ��Ϊ0			
			list<mem_loc>* mems=new list<mem_loc>;//���������ڴ洢�ṹ���ڵĳ�Ա��Ϣ
			
			string temp_stamp=stamp;//���浱ǰ��ȫ�ֱ�ǩ��Ϣ
			stamp=struct_name;//��ȫ�ֱ�ǩ��Ϣ��Ϊ��ǰ�����Ľṹ����������
			
			//�����ṹ���ڵı�������
			for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
				  iter!=s->get_var_dec_list()->end();iter++)
			{
				line_num=iter->get_line_begin();//���õ�ǰ����λ�ڵ��к�
				//�Ե�ǰ��������ָ�ı����������б���������ת��Ϊ�򵥵ı�����������һά��������
				scan_struct_array_vardec(&(*iter));		
				
				//�����������ı�������
				for(list<Init_declarator>::iterator iter1=iter->get_inits()->begin();
					iter1!=iter->get_inits()->end();iter1++)
				{
					//�жϵ�ǰ��������ָ�ı����Ƿ�Ϊ�򵥱���
					if(iter1->get_dec()->get_stamp()==0)
					{
						//���Ϊ�򵥱��������¼�ó�Ա��Ϣ
						struct mem_loc temp;//���ڼ�¼��Ա��Ϣ����ʱ�ṹ��
						temp.name=iter1->get_dec()->get_name();//��¼�ñ���������
						temp.loc=mem_num;//��¼�ñ����ڽṹ���ڵ����λ��
						mems->push_back(temp);//����Ա��Ϣ��������洢����
						mem_num++;//���λ�ü�1
					}
					else
					{
						//������Ǽ򵥱�������Ϊ�����������¼�����Ա����Ϣ
						struct mem_loc temp;//���ڼ�¼��Ա��Ϣ����ʱ�ṹ��
						temp.name=iter1->get_dec()->get_name();//��¼�ñ���������
						temp.loc=mem_num;//��¼�ñ����ڽṹ���ڵ����λ��
						mems->push_back(temp);//����Ա��Ϣ��������洢����
						//ȡ������������±��ַ���
						string sub=iter1->get_dec()->get_array_subs()->front().get_c();
						//�޸����λ�ã�����Ϊ������±�ֵ
						mem_num+=str2int(sub);
					}
				}
			}
			
			stamp=temp_stamp;//�ָ�ȫ�ֵı�ǩ��Ϣ
			
			//����ṹ����Ϣ����
			s_info_temp=new Struct_info(struct_name,mems,mem_num,"global");
			//���˶���洢��ȫ�ֵĽṹ����Ϣ������
			store_struct(structs,s_info_temp);
			delete mems;
		}
		else
		{
			//����ṹ�����Ͳ�����Ա������˵���ýṹ��϶���ǰ�涨��������Ҵ洢���������Ϣ
			string struct_name=s->get_name();//ȡ���ýṹ����������
			//���ݽṹ�������ڽṹ����Ϣ�����в��Ҹýṹ����Ϣ
			string result=find_struct_info(structs,struct_name,s_info_temp);
			
			//�ж������ҵ��ýṹ�����Ϣ
			if("no"==result)
			{
				//����Ҳ��������׳�������Ϣ
				string error_info="struct type "+struct_name+" has not been declared!";
				throw error_info;
			}
		}
		
		return s_info_temp;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan struct body : "+str;
		throw error_info;
	}
}

// ���ܣ���������������������������Ƿ��нṹ�����ͣ�����У�����ȡ�ṹ����Ϣ���洢��ͬʱ����
//			�ṹ����Ϣ����ָ�롣����޽ṹ�����ͣ��򷵻ص�ָ��Ϊ�գ�����Ϊ�ա�
// ���������t_s-��������ָ��
// ����ֵ���ɹ��򷵻ؽṹ����Ϣ����ָ�룬ʧ�����׳�������Ϣ
// ���ߣ����
static Struct_info* scan_struct_array_tplist(list<Type_specifier>* t_s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(t_s,"type spcifier list's pointer");
		Struct_info* si_temp=NULL;//�洢�ṹ����Ϣ�����ָ�룬��ʼ��Ϊ��
		//������������
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();iter++)
		{
			//�жϵ�ǰ��������ָ�������Ƿ�Ϊ�ṹ������
			if(2==iter->get_stamp())
			{
				//����ǣ�������˽ṹ������
				si_temp=scan_struct_body(iter->get_struct_spec());
				//���˽ṹ�������޸�Ϊint����
				iter->set_stamp(0);//���ṹ�����ͱ�ǩ����Ϊ0
				iter->set_type_name("int");//������������Ϊ"int"
			}
		}
		
		return si_temp;
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan type specifier list : "+str;
		throw error_info;
	}
}

// ���ܣ������������������������������������Լ��������ı���
// ���������v_d-������������ָ��
// ���������v_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_vardec(Var_declaration* v_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(v_d,"variable declaration object's pointer");
		
		//������������������������ȡ�ṹ��������Ϣ
		Struct_info* si_temp=scan_struct_array_tplist(v_d->get_decl_spec()->get_type_spec_list());

		//�жϸñ��������Ƿ������������������Ƿ�Ϊ�Զ�����������
		if(v_d->get_stamp()==1 && v_d->get_decl_spec()->get_stor_spec()!="typedef" )
		{
			//��������������������Ҳ�Ϊ�Զ�������������������������ı�������
			for(list<Init_declarator>::iterator iter=v_d->get_inits()->begin();
				  iter!=v_d->get_inits()->end();iter++)
			{
				//�жϵ�ǰ��������ָ�ı����Ƿ�Ϊ�������
				if(iter->get_dec()->get_stamp()==1)
				{
					//���Ϊ����������򽫸��������ת��Ϊһά����
					//����������±�����������Ԫ�ظ����������
					int sub=compute_array_sub(iter->get_dec()->get_array_subs(),iter->get_dec()->get_name());
					//�жϴ�������������ȡ���Ľṹ����Ϣ����ָ���Ƿ�Ϊ��
					if(NULL!=si_temp)
					{
						//�����Ϊ�գ���˵���ñ���Ϊ�ṹ�����ͱ������洢�ýṹ������������㽫�ýṹ�����
						//ת��Ϊһά�������±�ֵ
						struct_var temp;//���ڴ洢�ýṹ���������Ϣ
						temp.var_name_=iter->get_dec()->get_name();//�洢������
						temp.struct_info_=si_temp;//�洢������Ϣ
						temp.scope_=scope;//�洢������
						temp.stamp_=stamp;//�洢��ǩ��Ϣ
						struct_vars->push_back(temp);//���ýṹ���������Ϣ����ȫ��������
						sub=si_temp->get_num()*sub;//����ת��Ϊһά�������±�ֵ
					}
					else
					{
						//���Ϊ�գ���˵���ñ���Ϊһ�����͵ı���������
					}
					
					//ʹ�ü������������Ԫ�ظ��������µ��±���ʽ
					Expression* tmp_exp=new Expression(int2str(sub));
					//���ԭ���ĵ��±�����
					iter->get_dec()->get_array_subs()->clear();
					//���µ��±���ʽ��������
					iter->get_dec()->get_array_subs()->push_back(*tmp_exp);
					delete tmp_exp;
				}
				else
				{
					//���Ϊ��ͨ������������������Ƿ�Ϊ�ṹ������������ת��������ǽṹ�����ͣ���ת��Ϊ
					//һά���飬�����±꼴Ϊ�ṹ��Ĵ�С�������Ϊ�ṹ�����ͣ���ת��
					//�жϴ�������������ȡ���Ľṹ����Ϣ����ָ���Ƿ�Ϊ��
					if(NULL!=si_temp)
					{
						//�����Ϊ�գ���˵��Ϊ�ṹ�����������ת��Ϊһά����
						//�����±���ʽ����
						string sub=int2str(si_temp->get_num());//ȡ���ṹ��Ĵ�С
						Expression* tmp_exp=new Expression(sub);//���������±���ʽ
						list<Expression>* sub_list=new list<Expression>;//�洢�±���ʽ������
						sub_list->push_back(*tmp_exp);//�����µ��±���ʽ
						delete tmp_exp;
						//�޸���ͨ����Ϊ�������
						iter->get_dec()->set_stamp(1);//�޸ı�ǩ
						iter->get_dec()->set_array_subs(sub_list);//�޸��±���ʽ����
						
						struct_var temp;//�洢�ýṹ���������Ϣ
						temp.var_name_=iter->get_dec()->get_name();//�洢������
						temp.struct_info_=si_temp;//�洢������Ϣ
						temp.scope_=scope;//�洢������
						temp.stamp_=stamp;//�洢��ǩ��Ϣ
						struct_vars->push_back(temp);//���ýṹ���������Ϣ����ȫ��������	
					}
					else
					{
						//���Ϊ�գ������
					}
				}
			}
		}
		else
		{
			//��������ı�������������
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable declaration : "+str;
		throw error_info;
	}
}

// ���ܣ����������������жϸú��������Ƿ���к����壬����У�������������ڵ����������
// ���������f_d-������������ָ��
// ���������f_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_fundec(Fun_declaration* f_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(f_d,"function declaration object's pointer");
		
		//�жϺ��������Ƿ���к�����
		if(0==f_d->get_stamp())
		{
			//������к����壬��Ժ������ڵ������������б���
			scope=f_d->get_fun_name();//����������Ϊ����������
			scan_struct_array_declist(f_d->get_ccstmt()->get_cdl());
			scan_struct_array_stmtlist(f_d->get_ccstmt()->get_csl());
			
			//�������������������ú����еľֲ��ṹ�������Ϣ
			for(list<struct_var>::iterator iter=struct_vars->begin();iter!=struct_vars->end();)
			{
				if(iter->scope_==scope)
				{
					iter=struct_vars->erase(iter);
				}
				else
				{
					iter++;
				}
			}
			//����ú����еľֲ����������Ϣ
			for(list<Array_info>::iterator iter=arrays->begin();iter!=arrays->end();)
			{
				if(iter->get_scope()==scope)
				{
					iter=arrays->erase(iter);
				}
				else
				{
					iter++;
				}
			}
			//�������������֮�󣬽�����������Ϊ"global"
			scope="global";
		}
		else
		{
			//������������壬�����
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan function's declaration : "+str;
		throw error_info;
	}
}

// ���ܣ��������������Ա��������ͺ����������б�����ת������ͽṹ�����
// ���������d_s-��������ָ��
// ���������d_s-��������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_declist(list<Declaration>* d_s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(d_s,"declaration list's pointer");
		
		//������������
		for(list<Declaration>::iterator iter=d_s->begin();iter!=d_s->end();iter++)
		{
			line_num=iter->get_line_begin();//���õ�ǰ����λ�ڵ��к�
			
			//2010��3��10��׷��
			file_info=iter->get_file_info();//���õ�ǰ������λ�ڵ��ļ���
			
			//�ж������Ƿ�Ϊ��������
			if(0==iter->get_stamp())
			{
				//���Ϊ�����������������������
				scan_struct_array_vardec(iter->get_cvd());
			}
			else
			{
				//���Ϊ�����������������������
				scan_struct_array_fundec(iter->get_cfd());
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan declaration list : "+str;
		throw error_info;
	}
}

// ���ܣ������м�ṹ��������ͽṹ��������������������ṹ�������ʹ�ý���ת�������ṹ�����ת��Ϊ
// 			һά�������������ά�������ת��Ϊһά�������
// ���������s-�м�ṹָ��
// ���������s-�м�ṹָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void scan_struct_array_pro(Program *s)
{
	try
	{
		//���������м�ṹָ�����Ч��
		check_pt_arg(s,"program struct's pointer");
		
		structs->clear();//���洢�ṹ����Ϣ���������
		struct_vars->clear();//���洢�ṹ�������Ϣ���������
		arrays->clear();//���洢����������Ϣ���������
		
		scan_struct_array_declist(s->get_declist());//�����м�ṹ�е���������
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="transform struct and array : "+str;
		throw error_info;
	}
}





