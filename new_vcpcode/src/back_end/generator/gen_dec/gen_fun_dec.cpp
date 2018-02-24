#include "gen_utils.h"
#include "compound_stmt.h"
#include "param.h"
#include "fun_declaration.h"
#include "fun_info.h"
#include "declarator.h"
#include "type_specifier.h"
#include "declaration_specifiers.h"
#include "bool_var.h"
#include "gen_secondary.h"
//������ ../gen_utils.cpp
extern int dD_num;

// �������ļ�: src/main_utils.cpp
extern bool debug_flag;
extern list<Fun_info>* fun_infos;
extern list<string>* function_names;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;
extern map<string ,int> g_ExFun_Hash_map;
extern map<string ,string> AC_RetVal_map;
extern map<string ,string> AC_ParmVar_map;

// �������ļ� : ../gen_stmt/gen_compd_stmt.cpp
extern void gen_fun_body(Compound_stmt* ,Fun_info*);
extern void search_and_get_global_var(Fun_declaration* f_d);
//�������ļ�./gen_global_var.cpp
void search_and_get_global_var(Fun_declaration* f_d);

// ���ܣ��Ժ������βν��б��룬����βεĸ�λ�͵�λ����������β����������������׳�������Ϣ��
// 			ע�⣺�˺���һ�����������ǣ�����ĺ��������б��벻�ܺ��������βκͽṹ���βΡ�
// ���������s-�����βζ���ָ��
// ����ֵ���ɹ��򷵻��βε����֣�ʧ�����׳�������Ϣ
// ���ߣ���ա�������
static string gen_param(Param* s)
{
	try
	{
		//�������������βζ���ָ�����Ч��
		check_pt_arg(s,"parameter object's pointer");
		
		//ƥ�亯���βεı�ǩֵ���ж���������
		switch(s->get_stamp())
		{
			case 0:
			{
				//Ϊ0��˵�����������β�����������fun��int a)���е��β�
				//ע�⣺��������β���û������ͽṹ���β�
				//����βεĸߵ�λ
				//gen_out<<str_int32<<" H_"<<s->get_dec()->get_name()<<",";
				//gen_out<<str_int32<<" L_"<<s->get_dec()->get_name();
				string type_name=s->get_type_spec_list()->front().get_type_name();
				if(type_name=="int")
				{
				gen_out<<"long long "<<s->get_dec()->get_name()<<"_1,";
				gen_out<<"long long "<<s->get_dec()->get_name()<<"_2";
				}
				//�����βε�����
				return s->get_dec()->get_name();
			}
			break;	
				
			case 1:
			{
				//Ϊ1��˵���ǲ��������β�����������fun(int);���׳�������Ϣ
				string error_info="the function's parameter has no name! ";
				throw error_info;
			}
			break;
			
			default:
			{
				//��ǩΪ����ֵ���Ƿ����׳�������Ϣ
				string error_info="the function's parameter's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding function's parameters : "+str;
		throw error_info;
	}
}

// ���ܣ������������β������������βν��б��롣��������β�Ϊ�գ�����Ϊvoid����ֱ�����void��
// ���������s-�����β�����ָ��
// ���������f_i-������Ϣ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա�������
static void gen_paramlist(list<Param>* s,Fun_info* f_i)
{
	try
	{
		//�������������β�����ָ�����Ч��
		check_pt_arg(s,"parameter list's pointer");
		//��������������Ϣ����ָ�����Ч��
		check_pt_arg(f_i,"function information object's pointer");
		
		//�ж��β������Ƿ�Ϊ��
		if(s->size()==0)
		{
			//���Ϊ�գ�˵���ú�������Ϊ��
			f_i->set_para_stamp(0);//���ú�����Ϣ����Ĳ�����ǩΪ0�������ú����޲���
			gen_out<<"void";//���void
			return ;
		}
		//�жϺ����βθ����Ƿ�Ϊ1�����Ҵ��β��Ƿ���������
		else if(1==s->size() && s->front().get_stamp()==1)
		{
			//����βθ���Ϊ1���Ҵ��ββ����β�����������ж��βε������Ƿ�Ϊvoid
			if(s->front().get_type_spec_list()->size()==1 
			    && s->front().get_type_spec_list()->front().get_stamp()==0
				&& s->front().get_type_spec_list()->front().get_type_name()=="void")
			{
				//�����void�������ú�����Ϣ����Ĳ�����ǩΪ0�����������޲���
				f_i->set_para_stamp(0);	
				gen_out<<"void";//���void
				return ;
			}
			else
			{
				//�������������
				;
			}
		}
		else
		{
			//����βθ�������1������Ϊ1����Ϊvoid
			list<string>* para_names=new list<string>;//�洢�βε�����
			//�����β�����
			for(list<Param>::iterator iter=s->begin();iter!=s->end();iter++)
			{	
				//�жϵ�ǰԪ���Ƿ�Ϊ��������һ��
				if(iter == --s->end())
				{
					//��������һ������Ը��βν��б���󣬲��������
					string para_name=gen_param(&(*iter));
					//�洢���β���
					para_names->push_back(para_name);
				}	
				else
				{	
					//����������һ������Ը��βν��б�����������
					string para_name=gen_param(&(*iter));
					//�洢���β���
					para_names->push_back(para_name);
					gen_out<<", ";
				}
			}
	
			f_i->set_para_stamp(1);//���ú�����Ϣ����Ĳ�����ǩֵΪ1�������ú������β�
			f_i->set_para_num(s->size());//���ú�����Ϣ����ĳ�Ա�������βθ���
			f_i->set_paras(para_names);//���ú�����Ϣ����ĳ�Ա�������βε�����
			delete para_names;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding function's parameter list  : "+str;
		throw error_info;
	}
}

// ���ܣ�Ϊ�������ӱ�������ǩ������������붯̬ǩ������
// ���������fun_name-��������
// ����ֵ���ɹ��򷵻�Ϊ�������ӱ��������ǩ��ֵ
// ���ߣ���ա�������
static int  assign_fun_add_sig(string fun_name)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(fun_name,"function's name");
		
		string fun_add_name="red_dF_"+fun_name;//���캯�����ӱ���������
		int sigIndex = 0;
		int red_DF_sig=get_sig_index(&sigIndex);//Ϊ�������ӱ�������ǩ��
		
		//���������ӱ������붯̬ǩ������
		add_new_vars(var_infos,fun_add_name,red_DF_sig,"global","red_dF", sigIndex);
		
		return red_DF_sig;//���غ������ӱ�����ǩ��
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="assign function additonal variable's signature : "+str;
		throw error_info;
	}
}

// ���ܣ�Ϊ�����������ӱ����ͺ����Ĳ�������ǩ������������붯̬ǩ����
// ���������para_names-����к����������Ƶ�����ָ��
// 			      fun_name-������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա�������
static void assign_fun_para_sig(list<string>* para_names,string fun_name)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(fun_name,"function's name ");
		//������������������������ָ�����Ч��
		check_pt_arg(para_names,"function parameter's name list pointer ");
		
		//�����Ϊ�գ���������к���������������Ϊÿ����������ǩ��
		string str_para="AC_ParaVar_"+fun_name+"_";//���캯���������ӱ���������
		int i=0;//��¼�Ѿ�����ǩ���Ĳ����ĸ���
		//������������������
		for(list<string>::iterator iter=para_names->begin();
			   iter!=para_names->end();iter++,i++)
		{
			int sigIndex = 0;
			int sig=get_sig_index(&sigIndex);//Ϊ��ǰ��������ǩ��
			//���������ӱ������붯̬ǩ����
			add_new_vars(var_infos,str_para+int2str(i),sig,"global","para", sigIndex);
			//���������붯̬ǩ����
			add_new_vars(var_infos,(*iter),sig,fun_name,"para", sigIndex);
			string var_name = str_para+int2str(i);
			string C1 = int2str((sig % P1 - P1) % P1);
			string C2 = int2str((sig % P2 - P2) % P2);
			string var_value = "{{0,{"+ C1 +","+ C2 +"}},"+ int2str(sigIndex) +"};";
			AC_ParmVar_map[var_name] = var_value;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="assign function's parameter additonal variable's signature : "+str;
		throw error_info;
	}
}

// ���ܣ�Ϊ��������ǩ�������ݺ������޲��������޷���ֵ����������亯����ǩ���������丽�ӱ�����ǩ����
// �������ӱ�����ǩ��������ֵ���ӱ�����ǩ���ȣ�������Щǩ����Ϣ���붯̬ǩ������
// ���������f_i-������Ϣ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա�������
static void assign_fun_sigs(Fun_info* f_i)
{
	try
	{
		//��������������Ϣ����ָ�����Ч��
		check_pt_arg(f_i,"function information object's pointer");
		
		int para_tmp=f_i->get_para_stamp();//��¼�ú�����Ϣ����Ĳ�����ǩ
		int return_tmp=f_i->get_return_stamp();//��¼�ú�����Ϣ����ķ���ֵ��ǩ
		string fun_name=f_i->get_name();//��¼�ú���������
		//���ݲ�����ǩ�ͷ���ֵ��ǩ�����亯�������ǩ��
		//�жϸú����Ƿ���в������з���ֵ
		if(para_tmp==1 && return_tmp==1)
		{
			//Ϊ�������ӱ�������ǩ�������洢
			int red_DF_sig=assign_fun_add_sig(fun_name);//��¼�������ӱ�����ǩ��
			f_i->set_sig(red_DF_sig);//���ú�����Ϣ����ǩ����Ա��ֵΪ�������ӱ�����ǩ��
			g_ExFun_Hash_map[fun_name] = red_DF_sig;

			//������в������з���ֵ����Ϊ��������ֵ���ӱ�������ǩ������������붯̬ǩ����
			int sigIndex = 0;
			int ret_sig=get_sig_index(&sigIndex);//Ϊ����ֵ����̶�ǩ��
			add_new_vars(var_infos,"retVal_"+fun_name,ret_sig,"global","retVal", sigIndex);
			string C1 = int2str((ret_sig % P1 - P1) % P1);
			string C2 = int2str((ret_sig % P2 - P2) % P2);
			string retValTCODE = "{{0,{"+ C1 +","+ C2 +"}},"+ int2str(sigIndex) +"};";
			AC_RetVal_map[fun_name] = retValTCODE;

			//Ϊ������������ǩ��
			assign_fun_para_sig(f_i->get_paras(),fun_name);
		}
		//�жϸú����Ƿ�ֻ�з���ֵ���޲���
		else if(para_tmp==0 && return_tmp==1)
		{
			//Ϊ�������ӱ�������ǩ��
			int red_DF_sig=assign_fun_add_sig(fun_name);//��¼�������ӱ�����ǩ��
			f_i->set_sig(red_DF_sig);//���ú�����Ϣ����ǩ����Ա��ֵΪ�������ӱ�����ǩ��
			g_ExFun_Hash_map[fun_name] = red_DF_sig;

			int sigIndex = 0;
			int ret_sig = get_sig_index(&sigIndex);
			//����ú���ֻ�з���ֵ����Ϊ��������ֵ���ӱ�������ǩ������������붯̬ǩ����
			add_new_vars(var_infos,"retVal_"+fun_name, ret_sig, "global", "retVal", sigIndex);
			string C1 = int2str((ret_sig % P1 - P1) % P1);
			string C2 = int2str((ret_sig % P2 - P2) % P2);
			string retValTCODE = "{{0,{"+ C1 +","+ C2 +"}},"+ int2str(sigIndex) +"};";
			AC_RetVal_map[fun_name] = retValTCODE;
		}
		//�жϸú����Ƿ�ֻ�в������޷���ֵ
		else if(para_tmp==1 && return_tmp==0)
		{
			//Ϊ�������ӱ�������ǩ��
			int red_DF_sig=assign_fun_add_sig(fun_name);//��¼�������ӱ�����ǩ��
			f_i->set_sig(red_DF_sig);//���ú�����Ϣ����ǩ����Ա��ֵΪ�������ӱ�����ǩ��
			g_ExFun_Hash_map[fun_name] = red_DF_sig;

			//����ú���ֻ�в�����Ϊ������������ǩ��
			assign_fun_para_sig(f_i->get_paras(),fun_name);
		}
		else
		{
			//����ú����޲��޷���ֵ���򲻷����κ�ǩ��
			;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="assign function's signatures : "+str;
		throw error_info;
	}
}

// ���ܣ����ݺ������ƣ������̬ǩ�����еĸú����ľֲ�������Ϣ�������ú�����������һ����������е�
// 			�����������Щ�����ڶԸú�������������ٻ�ʹ�ã������֮��
// ���������v_i-��̬ǩ������ָ��
// 			      fun_name-��������
// ���������v_i-��̬ǩ������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ��
// ���ߣ���ա�������
static void  clear_local_vars(list<Var_info>* v_i,string fun_name)
{
	try
	{
		//��������������Ч��
		check_pt_arg(v_i,"var_info list's pointer");
		check_str_arg(fun_name,"function's name");
		
		//������̬ǩ����
		for(list<Var_info>::iterator iter=v_i->begin();iter!=v_i->end();)
		{
			//�жϵ�ǰ��������ָ�Ķ�����������Ƿ��봫��ĺ�����һ��
			if(fun_name==iter->get_scope())
			{
				//���һ�£�˵����ǰ�����Ǹú����ڵľֲ�����
				//�����жϵ�ǰ�����Ƿ�Ϊ��ͨ���������������
				if("common"==iter->get_stamp() || "array"==iter->get_stamp()
				   || "nodt_array"==iter->get_stamp())
				{
					//���Ϊһ��������������������ɾ���ñ���
					iter=v_i->erase(iter);
				}
				else
				{
					//�����Ϊһ�������������������������
					iter++;
				}
			}
			else
			{
				//������Ǹú����еľֲ����������������
				iter++;
			}	
		}
		
		//���������������ֵ����
		for(list<Bool_var>::iterator iter=bool_vars->begin();iter!=bool_vars->end();)
		{
			//�жϵ�ǰ���������������Ƿ�Ϊ��ǰ������
			if(fun_name==iter->get_scope())
			{
				//����ǣ��򽫴˲�������ɾ��
				iter=bool_vars->erase(iter);
			}
			else
			{
				//������ǣ��������ǰ����
				iter++;
			}
		}
		
		//�����������ֵǩ������
		for(list<Bool_var>::iterator iter=bool_sigs->begin();iter!=bool_sigs->end();)
		{
			//�жϵ�ǰ���������������Ƿ�Ϊ��ǰ������
			if(fun_name==iter->get_scope())
			{
				//����ǣ��򽫴˲�������ɾ��
				iter=bool_sigs->erase(iter);
			}
			else
			{
				//������ǣ��������ǰ����
				iter++;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="clear local var_infos : "+str;
		throw error_info;
	}
}

// ���ܣ��Ժ����������б��룬���������������������Լ�������ı��롣ͬʱΪ��������ǩ����
// 			���������ĸ��ӱ�����ǩ��������������ǩ���ȡ�������������󣬻�Ҫ���������ڵ�
//			�ֲ�������Ϣ�Ӷ�̬ǩ���������
// ���������f_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա�������
void gen_fun_dec(Fun_declaration* f_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(f_d,"function declaration object's pointer");

		string fun_name=f_d->get_fun_name();//�洢������
		string fun_type_name =f_d->get_decl_spec()->get_type_spec_list()->front().get_type_name();//�洢��������ֵ����
		var_scope=fun_name;//����������Ϊ�ú�������
		string param_error = "fun_type_nameThe check or IO interface's paramlist error!";
		
		//�½�һ��������Ϣ���󣬴洢�ú����������Ϣ
		Fun_info* fun_info_tmp=new Fun_info(fun_name);
		//�жϸú����ķ���ֵ�����Ƿ�Ϊvoid
		if(f_d->get_decl_spec()->get_type_spec_list()->size()==1
			&& f_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()==0
			&& f_d->get_decl_spec()->get_type_spec_list()->front().get_type_name()=="void")
		{
			//�����void�������ú�����Ϣ����ĳ�Ա������ֵ��ǩΪ0�������ú����޷���ֵ
			fun_info_tmp->set_return_stamp(0);
		}
		else
		{
			//������ز�Ϊvoid�������
			;
		}
		out_indent(indent_num,gen_out);
		if("F_VCE_CheckVar" == fun_name)
		{
			if(f_d->get_params()->size() == 2)
			{
				gen_out<<"void F_VCL_CheckCodeVar(";
				//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
				gen_out<<"T_CODE inVar, INT32 inErrNo";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}

		}
		else if("F_VCE_CheckArr" == fun_name)
		{
			if(f_d->get_params()->size() == 2)
			{
				gen_out<<"void F_VCL_CheckCodeArr(";
				//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
				gen_out<<"T_CODE inArr[], INT32 inStart, INT32 inSize, INT32 inErrNo";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataFetch" == fun_name)
		{
			if(f_d->get_params()->size() ==1)
			{
				gen_out<<"extern void F_USR_DataFetch(";
				//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataRecv" == fun_name)
		{
			if(f_d->get_params()->size() == 1)
			{
				gen_out<<"extern void F_USR_DataRecv(";
				//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataSend" == fun_name)
		{
			if(f_d->get_params()->size() == 1)
			{
				gen_out<<"extern void F_USR_DataSend(";
				//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataDeliver" == fun_name)
		{
			if(f_d->get_params()->size() == 1)
			{
				gen_out<<"extern void F_USR_DataDeliver(";
				//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else
		{
			gen_out<<fun_type_name <<" "<<fun_name<<"(";
			//����βεı��룬ͬʱ���ú�����Ϣ�������س�Ա
			gen_paramlist(f_d->get_params(),fun_info_tmp);
			gen_out<<")";
		}
		
		//�ж��Ƿ���ȫ�ֺ������������ҵ��˸ú���������
		if(function_names->end()==find(function_names->begin(),
		                                                           function_names->end(),fun_name))
		{
			if("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name)
			{
				if(f_d->get_params()->size() == 2)
				{
					//У����ⲿ��������ӿں�������Ҫ���б���
				}
				else
				{
					throw param_error;
				}
			}
			else if("F_USR_DataFetch" == fun_name
								|| "F_USR_DataRecv" == fun_name || "F_USR_DataSend" == fun_name || "F_USR_DataDeliver" == fun_name)
			{
				if(f_d->get_params()->size() == 1)
				{
					//У����ⲿ��������ӿں�������Ҫ���б���
				}
				else
				{
					throw param_error;
				}
			}
			else
			{

				//���û���ҵ�����˵�����״������ú�������Ҫ���亯�������ǩ��
				assign_fun_sigs(fun_info_tmp);
				function_names->push_back(fun_name);//���ú���������ȫ�ֺ���������
				fun_infos->push_back(*fun_info_tmp);//���ú�����Ϣ�������ȫ�ֺ�����Ϣ����

			}
		}
		else
		{
			//����ҵ��ˣ�˵�������״������ú���������Ҫ�ٷ���ǩ��
			;
		}
		
		//�жϺ��������Ƿ���к�����
		if(0==f_d->get_stamp())
		{
			//������к����壬��Ժ�������б���
			dD_num = 0;//��dD�±���0
			//����׷�� 5/20
			search_and_get_global_var(f_d);
			gen_out<<endl;
			gen_fun_body(f_d->get_ccstmt(),fun_info_tmp);
		}
		else
		{
			//��������и�����䣬������ֺ�
			gen_out<<";"<<endl;
		}
		//�ú�������������ڶ�̬ǩ�����У�����ú����ڵľֲ�����
		clear_local_vars(var_infos,fun_name);
		//����������Ϊ"global"
		var_scope="global";
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding function declaration : "+str;
		throw error_info;
	}
}



