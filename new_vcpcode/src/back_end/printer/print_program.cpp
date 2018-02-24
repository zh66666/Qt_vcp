
#include "program_struct.h"

//�������ļ� : src/main_utils.c
extern int indent_num;
//2010��3��10��׷��
extern string file_info;
extern string current_file_name ;
extern int line_num;

extern list<Cmnt>* cmnts;
extern void out_indent(int,ofstream&);	
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);
extern string  int2str(int  num);

static ofstream print_out;		//����ļ���
static list<Cmnt>* temp_cmnts=new list<Cmnt>;//�洢Դ�����е�ע��

static void print_program(list<Declaration>* s);
static void print_program(Var_declaration* s);
static void print_program(Declaration_specifiers* s);
static void print_program(Type_specifier* s);
static void print_program(Struct_specifier* s);
static void print_program(Declarator* s);
static void print_program(Enum_specifier* s);
static void print_program(Enumerator* s);
static void print_program(Init_declarator* s);
static void print_program(Initializer* s);
static void print_program(Fun_declaration* s);
static void print_program(Param* s);
static void print_program(Compound_stmt* s);
static void print_program(list<Statement>* s);
static void print_program(Statement* s);
static void print_program(Expression* s);
static void print_program(Var* s);
static void print_program(Return_stmt* s);
static void print_program(Selection_stmt* s);
static void print_if_branch(Selection_stmt* s);
static void print_program(Iteration_stmt* s);

// ���ܣ�����������м�ṹ���������ӡ���ļ�
// ���������s-�м�ṹָ��
//                  path_name-����������ļ���
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
void print_program(Program *s, string path_name)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s,"program struct's pointer");
		//������������ļ����ַ�������Ч��
		check_str_arg(path_name,"file's name");
		
		//�����ļ�
		print_out.open(path_name.c_str());
		//�ж��ļ��Ƿ񴴽��ɹ�
		if(!print_out.is_open())
		{
			//Ϊtrue��˵���ļ�����ʧ�ܣ�����
			string error_info="out put file creating failed!";
			throw error_info;
		}
		else
		{
			//Ϊfalse���ļ������ɹ�����ʼ��ӡ
			//�Ƚ�Դ�����е�ע�Ϳ���һ��
			temp_cmnts->insert(temp_cmnts->end(),cmnts->begin(),cmnts->end());
			
			//2010��3��10���޸�
			try
			{
				//��ӡ�����б�
				print_program(s->get_declist());
			}
			catch(string s)
			{
				string error_info="on line "+int2str(line_num)+" of file "+file_info+": "+s;
				throw error_info;
			}
			
			temp_cmnts->clear();
		}
		print_out.close();
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ����ӡ������Ϣ�����˳�����
		//2010��3��10���޸�
		cerr<<"error : print file \""<<current_file_name<<"\", "<<str<<endl;
		exit(-1);
	}
}

// ���ܣ���ӡ��������
// ���������s-��������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(list<Declaration>* s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s,"declaration list's pointer");
	
		//������������
		for(list<Declaration>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			out_cmnt(temp_cmnts,iter->get_fake_line(),print_out);//�����������֮ǰ��ע��
			
			//2010��3��10��׷��
			line_num=iter->get_line_begin();//���õ�ǰҪ��ӡ���������к�
			file_info=iter->get_file_info();//���õ�ǰҪ��ӡ�������������ļ�����
			
			//�ж������Ƿ�Ϊ��������
			if(0==iter->get_stamp())
			{
				//���Ϊ�������������ӡ��������
				print_program(iter->get_cvd());
				print_out<<endl;
			}
			else
			{
				//���Ϊ�������������ӡ��������
				print_program(iter->get_cfd());
				print_out<<endl;	
			}
		}
		
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print declaration list   : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ��������
// ���������s-������������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Var_declaration* s)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(s,"variable declaration  object's pointer");
		//�жϱ��������Ƿ���������ı���
		if(0==s->get_stamp())
		{
			//����������������� struct abc {int a; int b;};��ֻ��ӡ����˵����
			print_program(s->get_decl_spec());
			print_out<<";"<<endl;
		}
		else
		{
			//������б��������� struct abc {int a; int b;} var;��ӡ����˵�����ͱ���
			print_program(s->get_decl_spec());//��ӡ����˵����
			print_out<<" ";
			//���������ı�������
			for(list<Init_declarator>::iterator iter=s->get_inits()->begin();
				iter!=s->get_inits()->end();iter++)
			{
				//����Ƿ�Ϊ���һ������
				if(iter==--s->get_inits()->end())
				{
					//���Ϊ���һ����������ӡ�ñ���
					print_program(&(*iter));
				}
				else
				{
					//�����û�����һ����������ӡ�ñ������������
					print_program(&(*iter));
					print_out<<",";
				}
						
			}
			print_out<<";"<<endl;	
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print variable declaration   : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ��������˵����
// ���������s-��������˵��������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Declaration_specifiers* s)
{
	try
	{
		//����������������˵��������ָ�����Ч��
		check_pt_arg(s,"declaration specifier  object's pointer");
		
		//ȡ���洢����
		string stor_spec=(""!=s->get_stor_spec()) ? (s->get_stor_spec()+" ") : ("");
		out_indent(indent_num,print_out);
		print_out<<stor_spec;//����洢����
		
		//��ӡ��������
		for(list<Type_specifier>::iterator iter=s->get_type_spec_list()->begin();
		iter!=s->get_type_spec_list()->end();iter++)
		{
			//��ӡ���ͣ��� ��unsigned long long���������֮���ÿո�ֿ�
			print_program(&(*iter));
			print_out<<" ";
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print declaration specifier   : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ��������
// ���������s-�������Ͷ���ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Type_specifier* s)
{
	try
	{
		//���������������Ͷ���ָ�����Ч��
		check_pt_arg(s,"type specifier  object's pointer");
		//ƥ�����Ͷ���ı�ǩ
		switch(s->get_stamp())
		{
			case 0:
			case 1:
			{
				//0��1˵����һ������ͺ��Զ������ͣ���int��ֱ�Ӵ�ӡ������
				print_out<<s->get_type_name();
			}
			break;
			case 2:
			{	
				//Ϊ2˵���ǽṹ�����ͣ����ýṹ�����ʹ�ӡ����
				print_program(s->get_struct_spec());
			}
			break;
			case 3:
			{
				// Ϊ3˵����ö�����ͣ�����ö�����ʹ�ӡ����
				print_program(s->get_enum_spec());
			}
			break;
			default:
			{
				//��ǩΪ����ֵ���Ƿ�������
				string error_info="stamp is invalid!";
				throw error_info;
			}		
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print type specifier   : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�ṹ������
// ���������s-�ṹ�����Ͷ���ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Struct_specifier* s)
{
	try
	{
		//���������ṹ�����Ͷ���ָ�����Ч��
		check_pt_arg(s,"struct type specifier  object's pointer");
		//ƥ��ṹ�����Ͷ���ı�ǩ
		switch(s->get_stamp())
		{
			case 0:
			{
				//Ϊ0��˵���ṹ�庬���������ͽṹ���Ա�����磺struct abc {int a��int b}
				print_out<<"struct "<<s->get_name()<<endl;	//��ӡ�ṹ����������
				out_indent(indent_num,print_out);
				print_out<<"{"<<endl;
				indent_num++;
				//�����ṹ���еı�����������
				for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
					iter!=s->get_var_dec_list()->end();iter++)
				{
					//2010��3��10��׷��
					line_num=iter->get_line_begin();//���õ�ǰҪ��ӡ���������ڵ��к�
					
					print_program(&(*iter));//��ӡ�ṹ���еĳ�Ա����
				}
				indent_num--;
				out_indent(indent_num,print_out);
				print_out<<"}";
			}
			break;
			case 1:
			{
				//Ϊ1.˵���ṹ�岻�������������г�Ա���������磺struct {int a; int b}
				print_out<<"struct"<<endl;
				out_indent(indent_num,print_out);
				print_out<<"{"<<endl;
				indent_num++;
				//�����ṹ���еı�����������
				for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
					iter!=s->get_var_dec_list()->end();iter++)
				{
					//2010��3��10��׷��
					line_num=iter->get_line_begin();//���õ�ǰҪ��ӡ���������ڵ��к�
					
					print_program(&(*iter));	//��ӡ�ṹ���еĳ�Ա����
				}
				indent_num--;
				out_indent(indent_num,print_out);
				print_out<<"}";
			}
			break;
			case 2:
			{
				//Ϊ2��˵���ṹ��ֻ�����������޳�Ա���������磺struct abc
				print_out<<"struct "<<s->get_name();//��ӡ�ṹ����������
			}
			break;
			default:
			{
				//��ǩΪ����ֵ���Ƿ����׳�������Ϣ
				string error_info="stamp is invalid!";
				throw error_info;
			}		
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print struct type specifier   : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�������ı���
// ���������s-�����������Ͷ���ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Declarator* s)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(s,"declaring variable object's pointer");
		//�жϸñ����Ƿ�Ϊ��ͨ����
		if(0==s->get_stamp())
		{
			//�������ͨ���������磺int a ���� struct abc a�����ӡ������
			print_out<<s->get_name();
		}
		else
		{
			//�����������������磺int a[2]����struct abc a[3][4];
			print_out<<s->get_name();	//��ӡ������
			//���������±�����
			for(list<Expression>::iterator iter=s->get_array_subs()->begin();
				iter!=s->get_array_subs()->end();iter++)
			{	
				print_out<<"[";
				print_program(&(*iter));	//��ӡ�����±���ʽ
				print_out<<"]";
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print declaring variable  : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡö������
// ���������s-ö�����Ͷ���ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Enum_specifier* s)
{
	try
	{
		//��������ö�����Ͷ���ָ�����Ч��
		check_pt_arg(s,"enum type specifier object's pointer");
		
		string enum_name="";	//���ö�����͵�����
		//�ж�ö�������Ƿ�������
		if(1==s->get_stamp() || 2==s->get_stamp())
		{
			//����У�������ָ���enum_name
			enum_name=s->get_name();
		}
		else
		{
			//û�У���enum_name����
			enum_name="";
		}
		
		//��ӡö�����͵�����
		print_out<<"enum "<<s->get_name()<<endl;
		
		//�ж�ö�������Ƿ��г�Ա
		if(0==s->get_stamp() || 1==s->get_stamp())
		{
			//����У����磺enum abc {a,b,c}�����ӡ��Ա
			out_indent(indent_num,print_out);
			print_out<<"{"<<endl;
			indent_num++;
			//����ö�ٳ�Ա����
			for(list<Enumerator>::iterator iter = s->get_enum_list()->begin(); 
				   iter !=s->get_enum_list()->end(); ++iter)
			{
				//�ж��Ƿ������һ��ö�ٳ�Ա
				if(--s->get_enum_list()->end()==iter)
				{
					//�ǣ����ӡö�ٳ�Ա�����������
					print_program(&(*iter));
					print_out<<endl;
				}
				else
				{
					//���ǣ����ӡö�ٳ�Ա���������
					print_program(&(*iter));
					print_out<<","<<endl;
				}
			}
			indent_num--;
			out_indent(indent_num,print_out);
			print_out<<"}";
		}
		else
		{
			//���û��ö�ٳ�Ա��������
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print enum type specifier  : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡö�ٳ�Ա����Ϊ����ֵ��ö�ٳ�Ա�Ͳ�����ֵ��ö�ٳ�Ա
// ���������s-ö�ٳ�Ա����ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Enumerator* s)
{
	try
	{
		//��������ö�ٳ�Ա����ָ�����Ч��
		check_pt_arg(s,"enumerator object's pointer");
		//�жϸ�ö�ٳ�Ա�Ƿ���и�ֵ���ʽ
		if(0==s->get_stamp())
		{
			//��������и�ֵ���磺enum abc {a��b=3,c}�еĳ�Աa
			out_indent(indent_num,print_out);
			print_out<<s->get_element();	//����ó�Ա��
		}
		else
		{
			//������и�ֵ���磺enum abc {a,b=3,c}�еĳ�Աb
			out_indent(indent_num,print_out);
			print_out<<s->get_element()<<"=";	//����ó�Ա��
			print_program(s->get_exp());				//�����ֵ���ʽ
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print enumerator : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�����е�Init_declarator
//           ���������� int a�� int b=3; a ��b=3��ΪInit_declarator
// ���������s-Init_declarator����ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Init_declarator* s)
{
	try
	{
		//��������Init_declarator����ָ�����Ч��
		check_pt_arg(s,"Init_declarator object's pointer");
		
		//�ж�Init_declarator�����Ƿ���г�ʼ��
		if(0==s->get_stamp())
		{
			//���������ʼ�������磺int a��ֱ�Ӵ�ӡ�����ı���
			print_program(s->get_dec());
		}
		else
		{
			//������г�ʼ�������磺int a=3��
			print_program(s->get_dec());	//��ӡ�����ı���
			print_out<<" = ";
			print_program(s->get_init());	//��ӡ��ʼ������
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print Init_declarator : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�����еĳ�ʼ������
// ���������s-��ʼ�����ֶ���ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Initializer* s)
{
	try
	{
		//��������������ʼ������ָ�����Ч��
		check_pt_arg(s,"Initializer object's pointer");
		
		//�жϳ�ʼ�������Ƿ�����ͨ�����ĳ�ʼ��
		if(0==s->get_stamp())
		{
			//�������ͨ�����ĳ�ʼ������int a=3;
			print_program(s->get_exp());	//��ӡ��ʼ�����ʽ
		}
		else
		{
			//����Ƕ�����������߽ṹ������ĳ�ʼ��
			//��int a[3][2]={{1,2},{3,4},{5,6}}; struct abc a={4,5,6};
			print_out<<"{ ";
			for(list<Initializer>::iterator iter=s->get_init_list()->begin();
				iter!=s->get_init_list()->end();iter++)
			{
				//�жϳ�ʼ��ֵ�Ƿ�Ϊ�������е����һ��
				if(iter==--s->get_init_list()->end())
				{
					//����ǣ�����������ݣ����������
					print_program(&(*iter));
				}
				else
				{
					//������ǣ�����������ݺͶ���
					print_program(&(*iter));
					print_out<<",";
				}
			}
			print_out<<"}";
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print Initializer : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ��������
// ���������s-������������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Fun_declaration* s)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(s,"function declaration object's pointer");
		//�������������ֵ���������Ͳ�������
		print_program(s->get_decl_spec());	//�������ֵ����
		print_out<<" "<<s->get_fun_name()<<"(";	//���������
		//�����β�����
		for(list<Param>::iterator iter=s->get_params()->begin();
			   iter!=s->get_params()->end();iter++)
		{
			//�ж��Ƿ������һ������
			if(iter==--s->get_params()->end())
			{
				//�ǣ���������������������
				print_program(&(*iter));
			}
			else
			{
				//���ǣ����������ͬʱ���ж���
				print_program(&(*iter));
				print_out<<",";
			}
		}
		print_out<<")"; 
		
		//�жϺ��������Ƿ���к�������
		if(0==s->get_stamp())
		{
			//������к������岿�֣���ӡ�����ĸ������
			print_out<<endl;
			out_indent(indent_num,print_out);
			print_program(s->get_ccstmt());	//��ӡ�������岿��
		}
		else
		{
			//��������������岿�֣�������ֺ�
			print_out<<";"<<endl;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print function declaration : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�����β�
// ���������s-�����βζ���ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Param* s)
{
	try
	{
		//���������βζ���ָ�����Ч��
		check_pt_arg(s,"parameter object's pointer");
		//��ӡ�βε���������
		for(list<Type_specifier>::iterator iter=s->get_type_spec_list()->begin();
			   iter!=s->get_type_spec_list()->end();iter++)
		{
			print_program(&(*iter));
			print_out<<" ";
		}
		//�ж��β��Ƿ���������
		if(0==s->get_stamp())
		{
			//����β������������磺fun��int a������ӡ�β���
			print_program(s->get_dec());
		}
		else
		{
			//����β��������������磺fun��int������ִ���κβ���
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print parameter : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�������
// ���������s-����������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ����
static void print_program(Compound_stmt* s)
{	
	try
	{
		//������������������ָ�����Ч��
		check_pt_arg(s,"compound statement object's pointer");
		
		out_indent(indent_num,print_out);
		print_out<<"{"<<endl;
		indent_num++;	//������1
		
		//��ӡ��������е���������
		print_program(s->get_cdl());	
		//��ӡ��������е��������
		for(list<Statement>::iterator iter=s->get_csl()->begin();
		       iter!=s->get_csl()->end();iter++)
		{
			out_cmnt(temp_cmnts,iter->get_fake_line(),print_out);//����������֮ǰ��ע����Ϣ
			
			//2010��3��10��׷��
			line_num=iter->get_line_begin();//���õ�ǰ��ӡ�������λ�ڵ��к�
			
			print_program(&(*iter));	//��ӡstatement
		}	
		//���ܸø������Ϊ�գ����������"}"ǰ��ע�����
		out_cmnt(temp_cmnts,s->get_fake_line(),print_out);
		
		indent_num--;	//������1
		out_indent(indent_num,print_out);
		print_out<<"}";
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print compound statement : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ��䣬��ͬ�������ò�ͬ�Ĵ�ӡ����
// ���������s-������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Statement* s)
{
	try
	{
		//��������������ָ�����Ч��
		check_pt_arg(s,"statement object's pointer");
		//ƥ��������ı�ǩ
		switch(s->get_stamp())
		{
			case 0:
			{	
				//Ϊ0˵���Ǹ�����䣬���ø�������ӡ����
				print_program(s->get_ccstmt());
			}
			break;
			case 1:
			{
				//Ϊ1˵���Ƿ�֧��䣬���÷�֧����ӡ����
				print_program(s->get_csstmt());
			}
			break;
			case 2:
			{
				//Ϊ2˵����ѭ����䣬����ѭ������ӡ����
				print_program(s->get_cistmt());
			}
			break;
			case 3:
			{
				//Ϊ3˵���Ƿ�����䣬���÷�������ӡ����
				print_program(s->get_crs());
			}
			break;
			case 4:
			{
				//Ϊ4˵���Ǹ�ֵ��䣬��ӡ��ֵ�͸�ֵ���ʽ
				out_indent(indent_num,print_out);
				print_program(s->get_cvar());	//��ӡ��ֵ����
				print_out<<" = ";
				print_program(s->get_ce());		//��ӡ��ֵ���ʽ
				print_out<<";"<<endl;
			}
			break;
			case 5:
			{
				//Ϊ5˵���Ǻ������ã���ӡ��������
				out_indent(indent_num,print_out);
				print_program(s->get_ce());
				print_out<<";"<<endl;
			}
			break;
			case 6:
			{
				//Ϊ6˵���Ƿֺ���䣬��ӡ�ֺ�
				out_indent(indent_num,print_out);
				print_out<<s->get_semi()<<endl;
			}
			break;
			default:
			{
				//��ǩΪ����ֵ���Ƿ�������
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print statement : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ���ʽ�����ݱ��ʽ���͵Ĳ�ͬ�����ò�ͬ�Ĵ�ӡ����
// ���������s-���ʽ����ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Expression* s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		
		//ƥ����ʽ�ı�ǩ
		switch(s->get_stamp())
		{
			case 0:
			{
				//Ϊ0˵�����ʽ���������������磺a+b
				print_program(s->get_cel());	//��ӡ����ʽ
				print_out<<s->get_op();			//��ӡ������
				print_program(s->get_cer());	//��ӡ�ұ��ʽ
			}
			break;
			case 1:
			{
				//Ϊ1��˵��ֻ��һ�����������磡a
				print_out<<s->get_op();	//��ӡ������
				print_program(s->get_cer());	//��ӡ�ұ��ʽ
			}
			break;
			case 2:
			{
				//Ϊ2��˵��Ϊ���ű��ʽ����(a+b)
				print_out<<"(";
				print_program(s->get_exp());	//��ӡ�����еı��ʽ
				print_out<<")";
			}  
			break;
			case 3:	
			{
				//Ϊ3��˵��Ϊ�������ʽ����3
				print_out<<s->get_c();	//��ӡ�ó���
			}
			break;
			case 4:
			{
				//Ϊ4��˵��Ϊ�������ʽ����x ,b[3]
				print_program(s->get_cvar());	//���ñ�����ӡ����
			}
			break;	
			case 5:
			{
				//Ϊ5��˵��Ϊ�������ñ��ʽ
				print_out<<s->get_c()<<"(";	//��ӡ������
				for(list<Expression>::iterator iter=s->get_args()->begin();
					iter!=s->get_args()->end();++iter)
				{
					//�ж��Ƿ�Ϊ���һ��ʵ��
					if(iter==--s->get_args()->end())
					{
						//�ǣ����ӡʵ�α��ʽ�����������
						print_program(&(*iter));
					}
					else
					{
						//���ǣ����ӡʵ�α��ʽ�Ͷ���
						print_program(&(*iter));
						print_out<<",";
					}
				}	
				print_out<<")";
			}
			break;
			default:
			{
				//��ǩΪ����ֵ���Ƿ�������
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print expression : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ����
// ���������s-��������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Var* s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s,"variable object's pointer");

		//ƥ���������ı�ǩ
		switch(s->get_stamp())
		{
			case 0:
			{
				//Ϊ0��˵������ͨ�������� a����ӡ������
				print_out<<s->get_name();
			}
			break;
			case 1:
			{
				//Ϊ1��˵����һά�����������a[x+y]
				print_out<<s->get_name()<<"[";	//��ӡ������
				print_program(s->get_cexp());		//��ӡ�±���ʽ
				print_out<<"]";
			}
			break;
			case 2:
			{
				//Ϊ2��˵���Ƕ�ά�����������a[3][b][d+e]
				print_out<<s->get_name();	//��ӡ������
				for(list<Expression>::iterator iter=s->get_exp_list()->begin();
				       iter!=s->get_exp_list()->end();iter++)
				{
					print_out<<"[";
					print_program(&(*iter));	//��ӡ�±���ʽ
					print_out<<"]";
				}
			}
			break;
			case 3:
			{
				//Ϊ3��˵���ǽṹ���������a.c
				print_out<<s->get_name()<<".";	//��ӡ�ṹ����
				print_program(s->get_var());			//��ӡ�ṹ���Ա����
			}
			break;
			case 4:
			{
				//Ϊ4��˵���ǽṹ�������������a[3][4].c
				print_out<<s->get_name();			//��ӡ�ṹ��������
				for(list<Expression>::iterator iter=s->get_exp_list()->begin();
				iter!=s->get_exp_list()->end();iter++)
				{
					print_out<<"[";
					print_program(&(*iter));			//��ӡ�����±���ʽ
					print_out<<"]";
				}
				print_out<<".";									
				print_program(s->get_var());		//��ӡ��Ա����
			}
			break;
			default:
			{
				//��ǩΪ����ֵ���Ƿ�������
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print variable : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ�������
// ���������s-����������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Return_stmt* s)
{
	try
	{
		//������������������ָ�����Ч��
		check_pt_arg(s,"return statement object's pointer");
		
		//��ӡreturn �ؼ���
		out_indent(indent_num,print_out);
		print_out<<"return ";
		
		//�жϷ�������Ƿ���з���ֵ
		if(0==s->get_stamp())
		{
			//����޷���ֵ����ӡ�ֺ�
			print_out<<";"<<endl;
		}
		else
		{
			//������з���ֵ�����ӡ���ر��ʽ
			print_program(s->get_ce());
            print_out<<";"<<endl;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print return statement : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡ��֧���
// ���������s-��֧������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Selection_stmt* s)
{
	try
	{
		//����������֧������ָ�����Ч��
		check_pt_arg(s,"selection statement object's pointer");
		
		//�жϷ�֧����Ƿ�ֻ����if��֧
		if(0==s->get_stamp())
		{
			//���ֻ����if��֧����ӡif��֧�е����
			print_if_branch(s);
		}
		else
		{
			//�������if��else��֧��������֧�е�������ӡ
			print_if_branch(s);	//��ӡif��֧�е����
			out_indent(indent_num,print_out);
			//��ӡelse��֧���ж�else��֧�е�����Ƿ��Ǹ������
			if(0!=s->get_cs_else()->get_stamp())
			{
				//���Ǹ�����䣬������ж��ǲ��Ƿ�֧���
				if(1==s->get_cs_else()->get_stamp())
				{
					//�Ƿ�֧��䣬����else��ֱ�Ӵ�ӡ��䣬���������ü�1
					//��һ����Ϊ�˴�ӡ else if������ʽ�ķ�֧���
					print_out<<"else";
					print_program(s->get_cs_else());
				}
				else
				{
					//���Ƿ�֧��䣬����else�ؼ��������������1
					//ʹelse��֧�е��������һ��
					print_out<<"else"<<endl;
					indent_num++;
					print_program(s->get_cs_else());
					indent_num--;
				}
			}
			else
			{
				//�Ǹ�����䣬�����else�ؼ��ֺ�ֱ�ӵ�������ӡ����
				print_out<<"else"<<endl;
				print_program(s->get_cs_else());
			}
			print_out<<endl;
		}	
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print selection statement : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡѭ�����
// ���������s-ѭ��������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_program(Iteration_stmt* s)
{
	try
	{
		//��������ѭ��������ָ�����Ч��
		check_pt_arg(s,"iteration statement object's pointer");
		
		out_indent(indent_num,print_out);
		print_out<<"while(";
		print_program(s->get_ce());	//��ӡѭ�������ڱ��ʽ
		print_out<<")"<<endl;
		
		//�ж�ѭ���������������Ƿ��Ǹ������
		if(0!=s->get_cs()->get_stamp())
		{
			//������ǣ���������1
			indent_num++;
			print_program(s->get_cs());	//��ӡѭ�����е����
			indent_num--;
		}
		else
		{
			//����ǣ����������ü�1
			print_program(s->get_cs());	//��ӡѭ�����е����
		}
		print_out<<endl;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print iteration statement : "+str;
		throw error_info;
	}
}

// ���ܣ���ӡif��֧�е����
// ���������s-��֧������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�������Ϣ
// ���ߣ����
static void print_if_branch(Selection_stmt* s)
{
	try
	{
		//����������֧������ָ�����Ч��
		check_pt_arg(s,"selection statement object's pointer");
		
		out_indent(indent_num,print_out);
		print_out<<"if(";
		print_program(s->get_ce());	//��ӡ��֧��ڱ��ʽ
		print_out<<")"<<endl;

		//�ж�if��֧�е�����Ƿ��Ǵ��д����ŵĸ������
		if(0!=s->get_cs_if()->get_stamp())
		{
			//���Ǹ�����䣬��Ҫ����������ٴ�ӡ���
			indent_num++;
			print_program(s->get_cs_if());
			indent_num--;
		}
		else
		{
			//�Ǹ�����䣬��ֱ�ӵ�������ӡ����
			print_program(s->get_cs_if());
		}
		print_out<<endl;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print if branch : "+str;
		throw error_info;
	}
}


