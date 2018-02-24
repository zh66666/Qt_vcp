#include "struct_array.h"

extern string compute_exp(Expression* s,int* result);

//�������ļ� : ./struct_array_dec.c
extern string stamp;
extern list<Struct_info>* structs;
extern list<struct_var>* struct_vars;
extern list<Array_info>* arrays;

// �������ļ� src/main_utils.cpp
extern int line_num;
extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);

static void scan_struct_array_var(Var* s);
static void scan_struct_array_exp(Expression* s);
void scan_struct_array_stmtlist(list<Statement>* s);

// ���ܣ�����ά����ת��Ϊһά���飬�������±���ʽ������ת��Ϊһά����ʱ���±�ֵ�������ش��±�
//			���ʽ
// ���������array_name-��������
// 			      l-�±���ʽ����ָ��
// ����ֵ���ɹ��򷵻�ת��������һά������±���ʽָ�룬ʧ�����׳�������Ϣ
// ���ߣ����
// ע�⣺��������ʹ����newΪ���صı��ʽ�������ڴ棬��Ҫ�ϲ㺯�����ͷ�
static Expression* array_high2low(string array_name,list<Expression>* l)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(array_name,"array's name");
		//�������������±���ʽ����ָ�����Ч��
		check_pt_arg(l,"subscript expression list's pointer");

		Expression* exp=NULL;//�洢���ر��ʽ��ָ�룬��ʼ��ΪNULL
		list<Array_info>::iterator iter;//������Ϣ���������
		//�����洢������������Ϣ��ȫ������
		for(iter=arrays->begin();iter!=arrays->end();iter++)
		{
			//�жϵ�ǰ��������ָ������������Ƿ��봫���������һ�£������������ı�ǩ��Ϣ
			//�Ƿ��뵱ǰ��ȫ�ֱ�ǩһ��
			if(array_name==iter->get_name() && stamp==iter->get_stamp())
			{
				//�����һ�£�˵���ҵ��˸��������Ϣ
				vector<int> v=*(iter->get_subs());//ȡ��������±�ֵ
			
				//�Դ���������±���ʽ����ת����ת���㷨Ϊ��
				//������������ʱΪint a[M][N][L];ʹ��ʱ������Ϊa[o][p][q].
				//��ת�����һά������±�Ϊo*N*L+p*N+L��Ϊ����ʽ�ĺ�
				
				int k=0;//��¼�����������±���ʽ�ĸ���
				int size=v.size();//��¼�����ά��
				string temp_stamp=stamp;//����ȫ�ֱ�ǩ��Ϣ
				stamp="common";//��ȫ�ֱ�ǩ��Ϣ��Ϊ"common"
				
				//���������±���ʽ����
				for(list<Expression>::iterator iter1=l->begin();iter1!=l->end();iter1++,k++)
				{
					scan_struct_array_exp(&(*iter1));//ת����ǰ��������ָ���±���ʽ
					
					//������δ�������±�ֵ�ĳ˻�temp
					int temp=1;
					for(int i=k+1;i<size;i++)
					{
						temp*=v[i];
					}
					
					Expression* exp2=NULL;//���ڴ洢����һά�±���ʽ�ĵ���ʱ����ʽ
					if(1==temp)
					{
						//��������tempֵΪ1����ֱ�ӽ�ת������±���ʽ��Ϊ����ʽ
						exp2=new Expression(*iter1);
					}
					else
					{
						//���tempֵ��Ϊ1����ת������±���ʽ��Ϊ�Ҳ�������tempֵ��Ϊ�����������
						//�˷����ʽ�����˱��ʽ��Ϊ����ʽ
						string str=int2str(temp);
						exp2=new Expression(new Expression(str),"*",&(*iter1));
					}
					//�жϵ�ǰ�Ƿ��ڴ����һ���±���ʽ
					if(k==0)
					{
						//����ǣ��򽫵�ǰ���ɵĶ���ʽ�������������ɵ�һά�����±���ʽ
						exp=new Expression(*exp2);
						delete exp2;
					}
					else
					{
						//������ǣ����Ѿ����ɵ��±���ʽ��Ϊ�����������ǰ���ɵĶ���ʽ��Ϊ�Ҳ���������
						//�ӷ����ʽ�������˱��ʽ���������յ��±���ʽ��������һ���±���ʽ�ı���
						Expression* exp4=new Expression(new Expression(*exp),"+",exp2);
						delete exp;
						exp=new Expression(*exp4);
						delete exp4;
					}
				}
				stamp=temp_stamp;//�ָ�ȫ�ֱ�ǩ��Ϣ	
				
				break;
			}
			else
			{
				//�����һ�£����������
			}
		}
		//�жϵ������Ƿ��������ĩ��
		if(arrays->end()==iter)
		{
			//����ǣ���˵��û���ҵ������飬�׳�������Ϣ
			string error_info="array "+array_name+" has not been declared!";
			throw error_info;
		}
		else
		{
			//������ǣ������
		}
		return exp;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan expression object : "+str;
		throw error_info;
	}
}

// ���ܣ����ҽṹ���Ա��Ϣ�����ݴ���Ľṹ�����������Ա������Ϣ�����Ҹó�Ա�ڽṹ���е����λ��
//			�Լ��ó�Ա��������
// ���������struct_name-�ṹ������
// 			      mem_name-��Ա����
//			      stamp-��ǩ��Ϣ
// ���������loc-��¼��Ա�ڽṹ����λ�õ�ָ��
// ����ֵ���ɹ��򷵻ظó�Ա���������ƣ�ʧ�����׳�������Ϣ
// ���ߣ����
static string find_struct_member(string struct_name,string mem_name,string stamp,int *loc)
{
	try
	{
		//��������������Ч��
		check_str_arg(struct_name,"struct's name");
		check_str_arg(mem_name,"struct member's name");
		check_str_arg(stamp,"stamp information");
		
		string struct_type_name="";//�洢��Ҫ���ص���������
		list<struct_var>::iterator iter;//�ṹ��������������
		//�����洢���нṹ�������ȫ������
		for(iter=struct_vars->begin();iter!=struct_vars->end();iter++)
		{
			//�жϵ�ǰ��������ָ�Ľṹ��������Ƿ�ͬ����Ľṹ�������һ��
			//�����жϵ�ǰ��ָ�Ľṹ������ı�ǩ��Ϣ�Ƿ��뵱ǰȫ�ֵı�ǩ��Ϣһ��
			if(iter->var_name_==struct_name && iter->stamp_==stamp)
			{
				//�����һ�£�˵���ڽṹ����������ҵ��˸ýṹ�����
				list<mem_loc>::iterator iter1;//�ṹ���Ա���������
				//�����ṹ���Ա����
				for(iter1=iter->struct_info_->get_members()->begin();
					iter1!=iter->struct_info_->get_members()->end();iter1++)
				{
					//�жϵ�ǰ��������ָ�ĳ�Ա�����Ƿ�ͬҪ���ҵĳ�Ա��һ��
					if(iter1->name==mem_name)
					{
						//���һ�£���ȡ���ó�Ա�����λ�ã�������
						*loc=iter1->loc;
						struct_type_name=iter->struct_info_->get_struct_name();
						break;
					}
				}
				//�жϵ������Ƿ�ָ���������ĩ��
				if(iter->struct_info_->get_members()->end()==iter1)
				{
					//����ǣ���˵��û���ҵ��ó�Ա�����׳�������Ϣ
					string error_info="struct variable "+struct_name+" has no member named "+mem_name;
					throw error_info;
				}
				break;
			}
		}
		//�ж����������Ƿ�ָ���������ĩ��
		if(iter==struct_vars->end())
		{
			//����ǣ���˵���ýṹ�����δ���������׳�������Ϣ
			string error_info="struct variable "+struct_name+" has not been declared! ";
			throw error_info;
		}
			
		return struct_type_name;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="find struct member's information : "+str;
		throw error_info;
	}
}

// ���ܣ����ҽṹ��Ĵ�С���ڶԽṹ���������ת��ʱ����������������ǩ��Ϣ�����Ҹýṹ�����͵Ĵ�С
// ���������array_name-���������
//			      stamp-��ǩ��Ϣ
// ����ֵ���ɹ��򷵻ؽṹ��Ĵ�С��ʧ�����׳�������Ϣ
// ���ߣ����
static int find_struct_size(string array_name,string stamp)
{
	try
	{
		//��������������Ч��
		check_str_arg(array_name,"struct array's name");
		check_str_arg(stamp,"stamp information");
		
		int num=0;//��¼��Ҫ���صĽṹ��Ĵ�С
		list<struct_var>::iterator iter;
		//����ȫ�ֵĽṹ���������
		for(iter=struct_vars->begin();iter!=struct_vars->end();iter++)
		{
			//�����ǰ��������ָ�Ķ���������봫���������һ�£����ұ�ǩ��Ϣһ�£�
			//˵���ҵ��˸ýṹ���������
			if(iter->var_name_==array_name && iter->stamp_==stamp)
			{
				//ȡ���ýṹ��Ĵ�С
				num=iter->struct_info_->get_num();
				break;
			}
			else
			{
				//�������һ�£����������
			}
		}
		//�����������жϵ������Ƿ�ָ��������ĩ��
		if(iter==struct_vars->end())
		{
			//����ǣ���˵���ýṹ�����δ���������׳�������Ϣ
			string error_info="struct  variable"+array_name+" has not been declared! ";
			throw error_info;
		}
		return num;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="find struct 's size : "+str;
		throw error_info;
	}
}

// ���ܣ����ڽṹ�����ת������ģ�飬�Խṹ���������ת�������䴫��Ϊһά���飬ͨ��������
//			ת�����õ��±���ʽ���
// ���������s-�ṹ���Ա��������
// ���������exp_sub-�±���ʽ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_var_I(Var* s,Expression* exp_sub)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"struct member variable's pointer");
		check_pt_arg(exp_sub,"subscript expression's pointer");
		
		//ƥ�����������
		switch(s->get_stamp())
		{
			case 0://��ԱΪ�򵥱���������
			case 1://��ԱΪһά���飬����
			break;
			case 2:
			{
				//���Ϊ��ά���飬����ת��Ϊһά��������
				string array_name=s->get_name();//ȡ��������
				//���±���ʽ�����һά���±���ʽ
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				//���Զ�ת�������±���ʽ���м���
				int num=0;
				string result=compute_exp(exp_one,&num);
				//�ж��Ƿ����ɹ������Ҽ����ֵΪ0
				if("right"==result && 0==num)
				{
					//����ǣ������
					;
				}
				else
				{
					//������ܼ��㣬���߼������õ�ֵ��Ϊ0�����������յ�һά�����±���ʽ
					Expression* exp_1=new Expression(*exp_sub);
					exp_sub->set_stamp(0);
					exp_sub->set_op("+");
					exp_sub->set_cel(exp_1);
					exp_sub->set_cer(exp_one);
				}
			}
			break;
			case 3:
			{
				//���Ϊ�ṹ���������struct_var.abc������ת��Ϊһά�������
				string var_name=s->get_name();//ȡ���ṹ�����������
				int loc=0;
				//���ҽṹ���Ա�����ڽṹ���е����λ��
				string struct_type=find_struct_member(var_name,s->get_var()->get_name(),stamp,&loc);
				//�ж����λ���Ƿ�Ϊ0
				if(0!=loc)
				{
					//�����Ϊ0������ϴ�ƫ����
					Expression* exp_1=new Expression(*exp_sub);
					exp_sub->set_stamp(0);
					exp_sub->set_cel(exp_1);
					exp_sub->set_cer(new Expression(int2str(loc)));
					exp_sub->set_op("+");
				}
				else
				{
					//���Ϊ0���򲻼�
				}
				string temp_stamp=stamp;
				stamp=struct_type;
				//����������һ��ṹ�����
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;
			}
			break;
			case 4:
			{
				//���Ϊ�ṹ���������������ת�����±�������ת�����Ա����
				string array_name=s->get_name();//ȡ���ṹ��������
				//���ҽṹ��Ĵ�С
				int size=find_struct_size(array_name,stamp);
				//��������±�����ת��Ϊһά�±�
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				//�����²�ṹ���Ա��λ��
				int loc=0;
				string struct_type=find_struct_member(array_name,s->get_var()->get_name(),stamp,&loc);
				//���㱾��ṹ��������±���ʽ
				int sub=0;
				string result=compute_exp(exp_one,&sub);
				//�жϼ����Ƿ�ɹ�
				if("right"==result )
				{
					
					int temp=size*sub+loc;
					if(0!=temp)
					{
						Expression* exp_1=new Expression(*exp_sub);
						exp_sub->set_stamp(0);
						exp_sub->set_cel(exp_1);
						exp_sub->set_cer(new Expression(int2str(temp)));
						exp_sub->set_op("+");
					}
				}
				else
				{
					if(0==loc)
					{
						Expression* exp_1=new Expression(*exp_sub);
						Expression* exp_2=new Expression(new Expression(int2str(size)),"*",exp_one);
						exp_sub->set_stamp(0);
						exp_sub->set_cel(exp_1);
						exp_sub->set_cer(exp_2);
						exp_sub->set_op("+");
					}
					else
					{
						Expression* exp_1=new Expression(*exp_sub);
						Expression* exp_2=new Expression(new Expression(int2str(size)),"*",exp_one);
						Expression* exp_3=new Expression(exp_1,"+",exp_2);
		
						exp_sub->set_stamp(0);
						exp_sub->set_cel(exp_3);
						exp_sub->set_op("+");
						exp_sub->set_cer(new Expression(int2str(loc)));
					}
					
				}
				
				string temp_stamp=stamp;
				stamp=struct_type;
				//���������²�ṹ���Ա
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;
			}
			break;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="transform struct member : "+str;
		throw error_info;
	}
	
}

// ���ܣ������������󣬸��ݱ��������ͣ����ò�ͬ���㷨������Ҫ����ת��Ϊһά����
// ���������s-��������ָ��
// ���������s-��������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_var(Var* s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s,"variable object's pointer");
		//ƥ�����������
		switch(s->get_stamp())
		{
			case -1://�ձ�������������
			case 0://�򵥱�������������
			case 1: //һά���飬��������
			break;
			case 2:
			{
				//���Ϊ��ά���飬����ת��Ϊһά��������
				string array_name=s->get_name();//ȡ��������
				//���±���ʽ�����һά���±���ʽ
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				s->set_stamp(1);//�޸ı����ı�ǩ
				s->set_cexp(exp_one);//�޸�������±���ʽ
			}
			break;
			case 3:
			{	
				string var_name=s->get_name();//ȡ���ṹ�����������
				int loc=0;
				//���ҽṹ���Ա�����ڽṹ���е����λ��
				string struct_type=find_struct_member(var_name,s->get_var()->get_name(),stamp,&loc);
				Expression* exp_sub=new Expression(int2str(loc));
				
				string temp_stamp=stamp;
				stamp=struct_type;
				//������Ա
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;
				//���ṹ�����ת��Ϊһά�������
				s->set_stamp(1);//�޸ı����ı�ǩ
				s->set_cexp(exp_sub);//�޸��±���ʽ

			}
			break;
			case 4:
			{
				//���Ϊ�ṹ���������������ת�����±�������ת�����Ա����
				string array_name=s->get_name();//ȡ���ṹ��������
				int size=0;
				Expression* exp_sub=NULL;
				list<struct_var>::iterator iter;
				for(iter=struct_vars->begin();iter!=struct_vars->end();iter++)
				{
					if(iter->var_name_==array_name&&iter->stamp_==stamp)
					{
						size=iter->struct_info_->get_num();
						break;
					}
				}
				if(iter==struct_vars->end())
				{
					string error_info="struct variable "+array_name+" has not been declared! ";
					throw error_info;
				}
				
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				
				int loc=0;
				string struct_type=find_struct_member(array_name,s->get_var()->get_name(),stamp,&loc);
				
				int sub=0;
				string result=compute_exp(exp_one,&sub);
				if("right"==result )
				{
					int temp=size*sub+loc;
					exp_sub=new Expression(int2str(temp));
				}
				else
				{
					if(0==loc)
					{
						exp_sub=new Expression(new Expression(int2str(size)),"*",exp_one);
					}
					else
					{
						Expression* exp_1=new Expression(new Expression(int2str(size)),"*",exp_one);
						exp_sub=new Expression(exp_1,"+",new Expression(int2str(loc)));
					}
				}
				
				string temp_stamp=stamp;
				stamp=struct_type;
				//�����ṹ���Ա
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;

				s->set_stamp(1);//�޸Ĵ˱����ı�ǩ
				s->set_cexp(exp_sub);//�޸�������±���ʽ
			}
			break;
			default :
			{
				//������ǩΪ����ֵ�����׳�������Ϣ
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan variable  : "+str;
		throw error_info;
	}
}

// ���ܣ��������ʽ���󣬸��ݱ��ʽ���͵Ĳ�ͬ���в�ͬ�ı���������Ҫת������������ͽṹ�����
//			ת��Ϊһά�������
// ���������s-���ʽ����ָ��
// ���������s-���ʽ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_exp(Expression* s)
{
	try
	{
		//����������֧������ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		//ƥ����ʽ����
		switch(s->get_stamp())
		{
			case -1://�ձ��ʽ������
			case 3://�������ʽ������
			break;
			
			case 0:
			{
				//���Ϊ�������������ı��ʽ����������ұ��ʽ
				scan_struct_array_exp(s->get_cel());//��������ʽ
				scan_struct_array_exp(s->get_cer());//�����ұ��ʽ
			}
			break;
			case 1:
			{
				//���Ϊֻ��һ���������ı��ʽ��������ұ��ʽ
				scan_struct_array_exp(s->get_cer());
			}
			break;
			case 2:
			{
				//���Ϊ���ű��ʽ������������еı��ʽ
				scan_struct_array_exp(s->get_exp());
			}  
			break;
			case 4:
			{
				//���Ϊ�������ʽ�����������
				scan_struct_array_var(s->get_cvar());
			}	
			break;	
			case 5:
			{
				//���Ϊ�������ñ��ʽ�����������ʵ�α��ʽ����
				for(list<Expression>::iterator iter=s->get_args()->begin();iter!=s->get_args()->end();iter++)
				{
					scan_struct_array_exp(&(*iter));//������ǰ��������ָ��ʵ�α��ʽ
				}
			}
			break;
			default:
			{
				//�����ǩΪ����ֵ���Ƿ����׳�������Ϣ
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan expression object : "+str;
		throw error_info;
	}
}

// ���ܣ�������䣬��������еı����ͱ��ʽ����������õ��ĸ�ά����������߽ṹ�����ת��Ϊ
//			һά�������
// ���������s-������ָ��
// ���������s-������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void scan_struct_array_stmt(Statement* s)
{
	try
	{
		//��������������ָ�����Ч��
		check_pt_arg(s,"statement object's pointer");
		//ƥ����������
		switch(s->get_stamp())
		{
			case 0:
			{
				//���Ϊ������䣬�������������е��������
				scan_struct_array_stmtlist(s->get_ccstmt()->get_csl());
			}
			break;
			case 1:
			{
				//���Ϊ��֧��䣬�������֧���
				scan_struct_array_exp(s->get_csstmt()->get_ce());//������֧��ڱ��ʽ
				scan_struct_array_stmt(s->get_csstmt()->get_cs_if());//����if��֧�е����
				//�жϸ÷�֧����Ƿ���else��֧
				if(1==s->get_csstmt()->get_stamp())
				{
					//�������else��֧�������else��֧�е����
					scan_struct_array_stmt(s->get_csstmt()->get_cs_else());
				}
				{
					//�������else��֧������
				}
			}
			break;
			case 2:
			{
				//���Ϊѭ����䣬�����ѭ�����
				scan_struct_array_exp(s->get_cistmt()->get_ce());//����ѭ��������ڱ��ʽ
				scan_struct_array_stmt(s->get_cistmt()->get_cs());//����ѭ�����е����
			}
			break;
			case 3:
			{
				//���Ϊ������䣬������������
				//�жϸ÷�������Ƿ��з���ֵ
				if(1==s->get_crs()->get_stamp())
				{
					//����У�������������ķ��ر��ʽ
					scan_struct_array_exp(s->get_crs()->get_ce());
				}
				else
				{
					//�޷���ֵ�����
				}
			}
			break;
			case 4:
			{
				//���Ϊ��ֵ��䣬�������ֵ�����͸�ֵ���ʽ
				scan_struct_array_var(s->get_cvar());//������ֵ����
				scan_struct_array_exp(s->get_ce());//������ֵ���ʽ
			}
			break;
			case 5:
			{
				//���Ϊ���ʽ��䣬��������ʽ
				scan_struct_array_exp(s->get_ce());
			}
			break;
			case -1://��������
			case 6://�ֺ�������
			break;
			default :
			{
				//����ǩΪ����ֵ���Ƿ����׳�������Ϣ
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan statement  : "+str;
		throw error_info;
	}
}

// ���ܣ��������������ÿ�������б�����ת��������õ�������ͽṹ�����
// ���������s-�������ָ��
// ���������s-�������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void scan_struct_array_stmtlist(list<Statement>* s)
{
	try
	{
		//���������������ָ�����Ч��
		check_pt_arg(s,"statement list's pointer");
		//�����������
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//���õ�ǰ������λ�ڵ��к�
			//�Ե�ǰ��������ָ����������б���
			scan_struct_array_stmt(&(*iter));
		}	
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="scan statement list : "+str;
		throw error_info;
	}
}



