#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "gen_global_var.h"

// �������ļ�: src/main_utils.cpp
extern bool debug_flag;

extern string get_bind_local_var(string global_var,string func_name);
//������gen_compod_stmt.cpp
extern string function_name;

extern string int2HexStr_data(const int num);
extern void constant2Str(const int constant);

// ���ܣ���������ʱ������Ժ������ӱ����ı��롣�������ӱ����ĸ�λ�͵�λ��
// ���������fun_name-������
//			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void out_red_dF(string fun_name,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(fun_name,"function's name");
		check_pt_arg(v,"var_info list's pointer");
		int index = 0;
		int new_sig=get_sig_index(&index);//��ǩ������ȡһ��ǩ����Ϊ�������ӱ�������ǩ��
		add_new_sig(v,"red_dF_"+fun_name,new_sig,"global");//����ǩ�����붯̬ǩ����
			
		//���㺯�����ӱ����ĵ�λֵ
		long long new_low1 = -((long long) new_sig * (long long) K_NUM_P1)
				% (long long) P1 + new_sig;
		new_low1 = (new_low1 % P1 - P1) % P1;

		long long new_low2 = -((long long) new_sig * (long long) K_NUM_P2)
						% (long long) P2 + new_sig;
		new_low2 = (new_low2 % P2 - P2) % P2;

		//����ǵ���ģʽ�������������Ϣ���������ӱ�����ǩ��
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//red_dF_" << fun_name << "'s sig : " << new_sig
					<< endl;
		}
		
		//����������ӱ����ĸ�λ����
		out_indent(indent_num, gen_out);
		//gen_out << "H_red_dF_" << fun_name << " = " << new_sig << ";" << endl;
		gen_out << "g_ExFun_" << fun_name << " = " << new_sig << ";" <<endl;

		//����������ӱ����ĵ�λ����
		//out_indent(indent_num, gen_out);
		//gen_out << "L_red_dF_" << fun_name << " = " << new_low << " + DT;"
		//		<< endl;
		//�������ӱ�����ΪINT32���������������䲻��Ҫ����
//		gen_out << "AC_df_" << fun_name << ".C.C1 = " << new_low1 << " + g_GS_TL1;" <<endl;
//		out_indent(indent_num, gen_out);
//		gen_out << "AC_df_" << fun_name << ".C.C2 = " << new_low2 << " + g_GS_TL2;" <<endl;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="out put red_df adjust code : "+str;
		throw error_info;
	}
}

// ���ܣ��Ժ�������ʱ��ʵ�ν��б��롣�����Ժ����������ӱ����ı��룬ǩ���ķ���ȡ�
// ���������s-�������ñ��ʽ����ָ��
//			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void out_fun_args(Expression* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		string fun_name=s->get_c();//ȡ���ú�������
		int i=0;//��������Ĳ����������м���
		string str1[100];
		string str2[100];
		//�������ӱ���������
		//�����ú���ʵ���б�
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			//�ж�ʵ���Ƿ�Ϊ����
			if(3==iter->get_stamp())
			{
				//���Ϊ��������ȡ���ó����ַ���
				string arg_h=iter->get_c();
				str1[i]=arg_h+"*8193";
				str2[i]=arg_h+"*8191";
		}
			//�ж�ʵ���Ƿ�Ϊһ�����
			else if (4 == iter->get_stamp()
					&& iter->get_cvar()->get_stamp() == 0)
			{
				//���Ϊһ���������ȡ���ñ���������
				string arg_name=iter->get_cvar()->get_name();
				str1[i]=arg_name+"_1";
				str2[i]=arg_name+"_2";



			}
			else
			{
				string error_info="function call's argument is invalid! ";
				throw error_info;
			}
		}

		gen_out  << fun_name << "(";
		//���������ʵ�δ���

		for(int j=0;j<i;j++)
		{
			if(j==i-1)
			{
				gen_out << "" << str1[j]<<",";
				gen_out << "" << str2[j];
			}
			else
			{
				gen_out << "" << str1[j] << ",";
				gen_out << "" << str2[j] << ",";
			}
		}
		gen_out<<");"<<endl;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="out put red_df adjust code : "+str;
		throw error_info;
	}
}

// ���ܣ���У��ӿڵ���ʱ��ʵ�Σ���ͨ���������б��롣�������Ժ����������ӱ����ı��룬ǩ���ķ���ȡ�����У�����ͨ������װΪT_code��ʽ��
// ���������s-�������ñ��ʽ����ָ��
//			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
static void out_fun_args_checkVar(Expression* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=s->get_c();//ȡ���ú�������
		int i=0;//��������Ĳ����������м���
		int j=0;//��¼��ǰ�ǵڼ�������
		string fun_args = "";
		//�õ���������
		for(list<Expression>::iterator iter=s->get_args()->begin();
				       iter!=s->get_args()->end();iter++)
		{
			j++;
		}
		if(j != 2)
		{
			string error_info =
					"function F_VCE_CheckVar must posess 2 arguments! ";
			throw error_info;
		}
		//�����ú���ʵ���б�
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			//�ж�ʵ���Ƿ�Ϊ����
			if(3==iter->get_stamp() && i == 1)
			{
				string arg_h=iter->get_c();
				fun_args.append(arg_h);
			}
			//�ж�ʵ���Ƿ�Ϊһ�����
			else if (4 == iter->get_stamp()
					&& iter->get_cvar()->get_stamp() == 0 && i == 0)
			{
				//���Ϊһ���������ȡ���ñ���������
				string arg_name=iter->get_cvar()->get_name();
				//int sig = get_current_sig(v,arg_name,var_scope);
				if(is_global_var(arg_name))
				{
					if(arg_name == "AC_Void_Func_RetVal")
					{
						fun_args.append("AC_Void_Func_RetVal");
					}
					else
						fun_args.append(get_bind_local_var(arg_name, function_name));
				}
				else
				{
					if(arg_name == "AC_Void_Func_RetVal")
					{
						fun_args.append("AC_Void_Func_RetVal");
					}
					else
						fun_args.append("TC_" + arg_name);
				}

				if (i < j - 1)
				{
					fun_args.append(", ");
				}
			}
			else
			{
				cout<<iter->get_stamp()<<endl;
				cout<<iter->get_cvar()->get_stamp()<<endl;
				string error_info = "function call's argument is invalid! ";
				throw error_info;
			}
		}
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_CheckCodeVar" << "(";
		gen_out << fun_args;
		gen_out << ");" << endl;
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "out put red_df adjust code : " + str;
		throw error_info;
	}
}

// ���ܣ���У��ӿڵ���ʱ��ʵ�Σ����飩���б��롣�������Ժ����������ӱ����ı��룬ǩ���ķ���ȡ�����У�����ͨ������װΪT_code��ʽ��
// ���������s-�������ñ��ʽ����ָ��
//			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
static void out_fun_args_checkArr(Expression* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=s->get_c();//ȡ���ú�������
		int i=0;//��������Ĳ����������м���
		int j=0;//��¼��ǰ�ǵڼ�������
		string fun_args = "";
		string str1="AC_ParaVar_"+fun_name+"_";//�������ӱ���������
		//�õ���������
		for(list<Expression>::iterator iter=s->get_args()->begin();
						       iter!=s->get_args()->end();iter++)
		{
			j++;
		}
		if(j != 4)
		{
			string error_info =
					"function F_VCE_CheckArr must posess 2 arguments! ";
			throw error_info;
		}
		//�����ú���ʵ���б�
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			if(i == 0)
			{
				//��һ������Ԫ�ز���
				string arg_name=iter->get_cvar()->get_name();
				//string i_name = iter->get_cvar()->get_cexp()->get_c();//ȡ���±��ַ�����
				fun_args += "TC_" ;
				fun_args += arg_name;
				fun_args +=  ", ";
			}
			if(3==iter->get_stamp() && i == 1)
			{
				//�ڶ���errno����Ϊ��������ʽ
				string arg_h=iter->get_c();
//				const int arg_h_int = str2int(arg_h);
//				fun_args.append("CONST_NUM_" + int2HexStr_data(arg_h_int) + ", ");
				fun_args.append(arg_h + ", ");
			}
			if(3==iter->get_stamp() && i == 2)
			{
				//������errno����Ϊ��������ʽ
				string arg_h=iter->get_c();
//				const int arg_h_int = str2int(arg_h);
//				fun_args.append("CONST_NUM_" + int2HexStr_data(arg_h_int) + ", ");
				fun_args.append(arg_h + ", ");
			}
			if(3==iter->get_stamp() && i == 3)
			{
				//���ĸ�errno����Ϊ��������ʽ
				string arg_h=iter->get_c();
				fun_args.append(arg_h);
			}
			if ((4 == iter->get_stamp() && iter->get_cvar()->get_stamp() == 0)
					&& i == 1)
			{
				//�ڶ���errno����Ϊ������ʽ
				string arg_name=iter->get_cvar()->get_name();
				if(is_global_var(arg_name))
				{
					fun_args.append(get_bind_local_var(arg_name, function_name));
					fun_args.append(", ");
				}
				else
				{
					fun_args.append("TC_" + arg_name + ".Data.F, ");
				}

			}
			if ((4 == iter->get_stamp() && iter->get_cvar()->get_stamp() == 0)
								&& i == 2)
			{
				//�ڶ���errno����Ϊ������ʽ
				string arg_name=iter->get_cvar()->get_name();
				if(is_global_var(arg_name))
				{
					fun_args.append(get_bind_local_var(arg_name, function_name));
					fun_args.append(", ");
				}
				else
				{
					fun_args.append("TC_" + arg_name + ".Data.F, ");
				}
			}
			if ((4 == iter->get_stamp() && iter->get_cvar()->get_stamp() == 0)
								&& i == 3)
			{
				//�ڶ���errno����Ϊ������ʽ
				string arg_name=iter->get_cvar()->get_name();
				if(is_global_var(arg_name))
				{
					fun_args.append(get_bind_local_var(arg_name, function_name));
				}
				else
				{
					fun_args.append("TC_" + arg_name + ".Data.F");
				}
			}

		}
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_CheckCodeArr" << "(";
		gen_out << fun_args;
		gen_out << ");" << endl;
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "out put red_df adjust code : " + str;
		throw error_info;
	}
}

// ���ܣ����ⲿ��������ӿڵ���ʱ��ʵ�Σ����飩���б��롣�������Ժ����������ӱ����ı��룬ǩ���ķ���ȡ�
// ���������s-�������ñ��ʽ����ָ��
//			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����Ȼ
static void out_fun_args_dataTrans(Expression* s,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=s->get_c();//ȡ���ú�������
		int i=0;//��������Ĳ����������м���
		string fun_args = "";
		//�����ú���ʵ���б�
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			if(3==iter->get_stamp() && i == 0)
			{
				//�ڶ���errno����Ϊ��������ʽ
				string arg_h=iter->get_c();
				fun_args.append(arg_h);
			}
			else if (i == 0 && 4 == iter->get_stamp()
					&& iter->get_cvar()->get_stamp() == 0)
			{
				//��һ������Ԫ�ز���
				string arg_name = iter->get_cvar()->get_name();
				fun_args.append("TC_" + arg_name +".Data.F");
			}
			if (i > 0)
			{
				string error_info = "F_USR_DataTrans have more than 1 args!";
				throw error_info;
			}
		}
		out_indent(indent_num,gen_out);
		gen_out<<fun_name<<"(";
		gen_out<<fun_args;
		gen_out<<");"<<endl;

		//���ɶ�Ӧ��F_ITF_DataConvert����
		if ("F_USR_DataFetch" == fun_name)
		{
			out_indent(indent_num, gen_out);
			gen_out << "F_ITF_DataConvertFetch" << "(";
			gen_out << fun_args;
			gen_out << ");" << endl;
		}
		else if("F_USR_DataDeliver" == fun_name)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"F_ITF_DataConvertDeliver"<<"(";
			gen_out<<fun_args;
			gen_out<<");"<<endl;
		}
		else if("F_USR_DataSend" == fun_name)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"F_ITF_DataConvertSend"<<"(";
			gen_out<<fun_args;
			gen_out<<");"<<endl;
		}
		else if("F_USR_DataRecv" == fun_name)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"F_ITF_DataConvertRecv"<<"(";
			gen_out<<fun_args;
			gen_out<<");"<<endl;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="out put red_df adjust code : "+str;
		throw error_info;
	}
}
// ���ܣ��Ժ������������б��롣���ݺ����Ƿ��д���ʵ�Σ����ɲ�ͬ���������
// ���������s-�������ñ��ʽ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_fun_call(Expression* s,list<Var_info>* v)
{//�����������ں����ǩ��

	out_indent(indent_num,gen_out);
	gen_out<<"S_Fun_entry="<<*iter<<";"<<endl;
	S_this=*++iter;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"S_Fun_out="<<S_this<<";"<<endl;
	gen_out<<endl;

	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"function call expression's pointer");
		
		string fun_name=s->get_c();//��¼�����õĺ�����
		//�ж��Ƿ���У��ģ��ӿ�
		if("F_VCE_CheckVar" == fun_name)
		{
			//�жϸú�������ʵ�δ���
			if(2 == s->get_args()->size())
			{
				//�����ʵ�δ���
				out_fun_args_checkVar(s,var_infos);//�����ʵ�εı���
			}
			else
			{
				//У��ӿ�û�в���
				string error_info =
						"No args for F_VCE_Check function, wrong usage";
				throw error_info;
			}
		}
		else if("F_VCE_CheckArr" == fun_name)
		{
			//�жϸú�������ʵ�δ���
			if(4 == s->get_args()->size())
			{
				//�����ʵ�δ���
				out_fun_args_checkArr(s,var_infos);//�����ʵ�εı���
			}
			else
			{
				//У��ӿ�û�в���
				string error_info =
						"No args for F_VCE_Check function, wrong usage";
				throw error_info;
			}
		}
		else if ("F_USR_DataFetch" == fun_name
				|| "F_USR_DataDeliver" == fun_name
				|| "F_USR_DataRecv" == fun_name || "F_USR_DataSend" == fun_name)
		{
			//�жϸú�������ʵ�δ���
			if(1 == s->get_args()->size())
			{
				//�����ʵ�δ���
				out_fun_args_dataTrans(s,var_infos);//�����ʵ�εı���
			}
			else
			{
				//У��ӿ�û�в���
				string error_info="No args for F_USR_DataFetch function, wrong usage";
				throw error_info;
			}
		}
		else
		{
			//�жϸú�������ʵ�δ���
			if(0!=s->get_args()->size())
			{
				//�����ʵ�δ�
				//out_red_dF(fun_name,var_infos);//����������ӱ����ĵ�������
				out_fun_args(s,var_infos);//�����ʵ�εı���
			}
			else
			{
				//�����ʵ�δ���
				/*out_red_dF(fun_name,var_infos);//����������ӱ����ĵ�������
				//�˴��������ȫ�ֱ����Ĵ���
				int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
				vector<string> global_vars = get_global_var_infunc(function_name);
				vector<string>::iterator it;
				for(it=global_vars.begin();it!=global_vars.end();it++)
				{
					out_indent(indent_num, gen_out);
					gen_out << "TC_" + *it + ".Data.F =" << get_bind_local_var(*it, function_name) + ".Data.F;" <<endl;
					int constant1 = 0, constant2 = 0;
					//ȫ�ֱ���ǩ��
					out_indent(indent_num, gen_out);

					constant1 = get_current_sig(var_infos, *it, "global");
					gen_out<<"//" +*it+"'s sig: "+int2str(constant1)<<endl;
					out_indent(indent_num, gen_out);

					constant2 = get_current_sig(v, get_bind_local_var(*it, function_name), function_name);
					gen_out<<"//" +get_bind_local_var(*it, function_name)+"'s sig: "+int2str(constant2)<<endl;
					constant3 = ((constant1 - constant2)%P1+P1)%P1;
					constant4 = ((constant1 - constant2)%P2+P2)%P2;
					//ȫ�ֱ�����Ӧ����ʱ����ǩ��
					//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
					out_indent(indent_num, gen_out);
					gen_out << "TC_" << *it << ".Data.C.C1 = " << "(" << get_bind_local_var(*it, function_name) << ".Data.C.C1 +" << constant3 << "- g_GS_TL1)%P1;"<<endl;
					out_indent(indent_num, gen_out);
					gen_out << "TC_" << *it << ".Data.C.C2 = " << "(" << get_bind_local_var(*it, function_name) << ".Data.C.C2 +" << constant4 << "- g_GS_TL2)%P2;"<<endl;
					out_indent(indent_num, gen_out);
				}*/

				out_indent(indent_num, gen_out);
				gen_out  << fun_name << "();" << endl;


			}
			gen_out << endl;

			int R;
			S_this=*++iter;
			R=S_this-S_last;
			S_last=S_this;
			idx++;
			out_indent(indent_num,gen_out);
			gen_out<<"P_pre_idx="<<idx<<";"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<"RS=RS"<<"+("<<R<<");"<<endl;
			out_indent(indent_num, gen_out);
			gen_out << endl;


			int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
			vector<string> global_vars = get_global_var_infunc(function_name);
			vector<string>::iterator it;
			//������ɺ�ˢ��ȫ�ֱ����󶨵ľֲ�����
			for(it=global_vars.begin();it!=global_vars.end();it++)
			{/*
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(*it, function_name) + ".Data.F =" << "TC_" + *it  + ".Data.F;" <<endl;
				//������ǩ��
				int sigIndex = 0;
				int ret_sig=get_sig_index(&sigIndex);//Ϊ����ֵ����̶�ǩ��
				//add_new_vars(var_infos, get_bind_local_var(*it, function_name),ret_sig,"global","retVal", sigIndex);
				add_new_sig(v, get_bind_local_var(*it, function_name),ret_sig, var_scope);
				out_indent(indent_num, gen_out);
				gen_out<<"//" + get_bind_local_var(*it, function_name) + "'s new sig:" + int2str(ret_sig) <<endl;
				constant1 = ret_sig;
				//constant2 = get_current_sig(var_infos, *it, "global");
				constant2 = get_current_sig(var_infos, *it, "global");
				constant3 = ((constant1 - constant2)%P1+P1)%P1;
				constant4 = ((constant1 - constant2)%P2+P2)%P2;
				//ȫ�ֱ�����Ӧ����ʱ����ǩ��
				//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
				out_indent(indent_num, gen_out);
				gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C1 = " << "(" << "TC_" << *it << ".Data.C.C1 + " << constant3 << ")%P1 + g_GS_TL1;"<<endl;
				out_indent(indent_num, gen_out);
				gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C2 = " << "(" << "TC_" << *it << ".Data.C.C2 + " << constant4 << ")%P2 + g_GS_TL2;"<<endl;
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(*it, function_name) <<".Id = " <<"F_VCL_RegisterCodeId(" <<ret_sig <<");"<<endl;
			*/}
		}
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding function call: "+str;
		throw error_info;
	}
}

// ���ܣ��Ժ�����ֵ�����б��롣���ݺ����Ƿ��д���ʵ�Σ����ɲ�ͬ���������
// ���������var_name-��ֵ������
//			      exp-�������ñ��ʽ����ָ��
//			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_fun_assign(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{	//�����������ں����ǩ��

		out_indent(indent_num,gen_out);
		gen_out<<"S_Fun_entry="<<*iter<<";"<<endl;
		S_this=*++iter;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);

		gen_out<<"S_Fun_out="<<S_this<<";"<<endl;
		gen_out<<endl;
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"function call expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=exp->get_c();//ȡ�������ú�������
//		out_red_dF(fun_name,v);//����ú����ĸ��ӱ�����������
		out_indent(indent_num, gen_out);

		//�жϸú�������ʵ�δ���
		if(0!=exp->get_args()->size())
		{gen_out<<"FUN_RET=";
			//�����ʵ�δ���
			out_fun_args(exp,v);//�����ʵ�εı���
			out_indent(indent_num, gen_out);

		}
		else
		{
			//�����ʵ�δ���
			//�˴��������ȫ�ֱ����Ĵ���
			int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
			vector<string> global_vars = get_global_var_infunc(function_name);
			vector<string>::iterator it;
			gen_out<<"FUN_RET=";
			gen_out  << fun_name << "();" << endl;
		}
		out_indent(indent_num, gen_out);
		gen_out  << endl;
		int R;
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS"<<"+("<<R<<");"<<endl;
		out_indent(indent_num, gen_out);
		gen_out << endl;
		//����Ϊ��������ֵ����
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_1="<<"FUN_RET*8193;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<"FUN_RET*8191+("<<R<<");"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
		gen_out<<endl;
		//�˴��������ȫ�ֱ����Ĵ���
/*		int tempConstant1 = 0;
		int tempConstant2 = 0;
		int constant3 = 0;
		int constant4 = 0;*/
		vector<string> global_vars = get_global_var_infunc(function_name);
		vector<string>::iterator it;
		//������ɺ�ˢ��ȫ�ֱ����󶨵ľֲ�����
		/*for(it=global_vars.begin();it!=global_vars.end();it++)
		{
			out_indent(indent_num, gen_out);
			gen_out <<get_bind_local_var(*it, function_name) + ".Data.F =" << "TC_" + *it  + ".Data.F;" <<endl;
			//������ǩ��
			int sigIndex = 0;
			int ret_sig=get_sig_index(&sigIndex);//Ϊ����ֵ����̶�ǩ��
			//add_new_vars(var_infos, get_bind_local_var(*it, function_name),ret_sig,"global","retVal", sigIndex);
			add_new_sig(v, get_bind_local_var(*it, function_name),ret_sig, var_scope);
			out_indent(indent_num, gen_out);
			gen_out<<"//" + get_bind_local_var(*it, function_name) + "'s new sig:" + int2str(ret_sig) <<endl;
			tempConstant1 = ret_sig;
			//constant2 = get_current_sig(var_infos, *it, "global");
			tempConstant2 = get_current_sig(var_infos, *it, "global");
			constant3 = ((tempConstant1 - tempConstant2)%P1+P1)%P1;
			constant4 = ((tempConstant1 - tempConstant2)%P2+P2)%P2;
			//ȫ�ֱ�����Ӧ����ʱ����ǩ��
			//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
			out_indent(indent_num, gen_out);
			gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C1 = " << "(" << "TC_" << *it << ".Data.C.C1 + " << constant3 << ")%P1 + g_GS_TL1;"<<endl;
			out_indent(indent_num, gen_out);
			gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C2 = " << "(" << "TC_" << *it << ".Data.C.C2 + " << constant4 << ")%P2 + g_GS_TL2;"<<endl;
			out_indent(indent_num, gen_out);
			gen_out <<get_bind_local_var(*it, function_name) <<".Id = " <<"F_VCL_RegisterCodeId(" <<ret_sig <<");"<<endl;
		}*/
		int index = 0;
		int new_sig=get_sig_index(&index);//Ϊ��ֵ����������ǩ��

		//�����ֵΪȫ�ֱ���������󶨵ľֲ���������ǩ�����²���
		if(is_global_var(var_name))
			add_new_sig(v, get_bind_local_var(var_name, function_name),new_sig,var_scope);//����ǩ�����붯̬ǩ����
		else
			add_new_sig(v,var_name,new_sig,var_scope);//����ǩ�����붯̬ǩ����
		
		//���㲹������
		long long constant1 = (long long) new_sig
				- get_current_sig(v, "retVal_" + fun_name, "global")
				- get_current_sig(v, "red_dF_" + fun_name, "global");
		constant1 = ((constant1 % P1) + P1) % P1;
		long long constant2 = (long long) new_sig
						- get_current_sig(v, "retVal_" + fun_name, "global")
						- get_current_sig(v, "red_dF_" + fun_name, "global");
		constant2 = ((constant2 % P2) + P2) % P2;

		//����ǵ���ģʽ�������������Ϣ����ֵ��������ǩ��
		if (true == debug_flag)
		{/*
			out_indent(indent_num, gen_out);
			if(is_global_var(var_name))
				gen_out << "//" << get_bind_local_var(var_name, function_name) << "'s new sig : " << new_sig << endl;
			else
				gen_out << "//" << var_name << "'s new sig : " << new_sig << endl;

		*/}

		//���������ֵ�ĸ�λ����
		if(var_name == "AC_Void_Func_RetVal")
		{/*
			out_indent(indent_num, gen_out);
			gen_out << "AC_Void_Func_RetVal" << ".Data.F = AC_RetVal_" << fun_name << ".Data.F;" <<endl;
			//���������ֵ�ĵ�λ����
			out_indent(indent_num, gen_out);
			gen_out <<  "AC_Void_Func_RetVal" << ".Data.C.C1 = (AC_RetVal_" << fun_name << ".Data.C.C1 + "+int2str(constant1) +")%P1;" <<endl;
			out_indent(indent_num, gen_out);
			gen_out <<  "AC_Void_Func_RetVal" << ".Data.C.C2 = (AC_RetVal_" << fun_name << ".Data.C.C2 + "+int2str(constant2) +")%P2;" <<endl;
			out_indent(indent_num, gen_out);
			gen_out<<"AC_Void_Func_RetVal"<<".Id = F_VCL_RegisterCodeId("<< new_sig <<");"<<endl;
		*/}
		else
		{/*
			if(is_global_var(var_name))
			{
				//��ֵΪȫ�ֱ���

				out_indent(indent_num, gen_out);
				gen_out << get_bind_local_var(var_name, function_name) << ".Data.F = AC_RetVal_" << fun_name << ".Data.F;" <<endl;
				//���������ֵ�ĵ�λ����
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(var_name, function_name) << ".Data.C.C1 = (AC_RetVal_" << fun_name << ".Data.C.C1 + "+int2str(constant1) +")%P1;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(var_name, function_name) << ".Data.C.C2 = (AC_RetVal_" << fun_name << ".Data.C.C2 + "+int2str(constant2) +")%P2;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out<<get_bind_local_var(var_name, function_name)<<".Id = F_VCL_RegisterCodeId("<< new_sig <<");"<<endl;
			}
			else
			{
				out_indent(indent_num, gen_out);
				gen_out << "TC_" << var_name << ".Data.F = AC_RetVal_" << fun_name << ".Data.F;" <<endl;
				//���������ֵ�ĵ�λ����
				out_indent(indent_num, gen_out);
				gen_out << "TC_" << var_name << ".Data.C.C1 = (AC_RetVal_" << fun_name << ".Data.C.C1 + "+int2str(constant1) +")%P1;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out << "TC_" << var_name << ".Data.C.C2 = (AC_RetVal_" << fun_name << ".Data.C.C2 + "+int2str(constant2) +")%P2;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out<<"TC_"<<var_name<<".Id = F_VCL_RegisterCodeId("<< new_sig <<");"<<endl;
			}
		*/}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding function assignment : " + str;
		throw error_info;
	}

}
