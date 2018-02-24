#include "gen_utils.h"
#include "return_stmt.h"
#include "expression.h"
#include "var.h"
#include "gen_global_var.h"

// �������ļ�: src/main_utils.cpp
extern bool debug_flag;

//������gen_compod_stmt.cpp
extern string function_name;

extern string get_bind_local_var(string global_var,string func_name);
extern string int2HexStr_data(const int num);

// ���ܣ��Է��������б��롣���ݷ��ر��ʽ�Ĳ�ͬ���ͣ����ɲ�ͬ���������
// ���������s-����������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_return_stmt(Return_stmt* s)
{
	try
	{
		//������������������ָ�����Ч��
		check_pt_arg(s,"return statement object's pointer");
		
		//�������ֵ���֮ǰ��ȫ�ֱ�����������
		int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
		vector<string> global_vars = get_global_var_infunc(function_name);
		vector<string>::iterator it;
		for(it=global_vars.begin();it!=global_vars.end();it++)
		{
			out_indent(indent_num, gen_out);
			gen_out << "TC_" + *it + ".Data.F =" << get_bind_local_var(*it, function_name) + ".Data.F;" <<endl;
			int constant1 = 0, constant2 = 0;
			//ȫ�ֱ���ǩ��
			constant1 = get_current_sig(var_infos, *it, "global");
			constant2 = get_current_sig(var_infos, get_bind_local_var(*it, function_name), function_name);
			constant3 = ((constant1 - constant2)%P1+P1)%P1;
			constant4 = ((constant1 - constant2)%P2+P2)%P2;
			//ȫ�ֱ�����Ӧ����ʱ����ǩ��
			//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
			out_indent(indent_num, gen_out);
			gen_out << "TC_" << *it + ".Data.C.C1 = " << "(" + get_bind_local_var(*it, function_name) + ".Data.C.C1 + " + int2str(constant3) + " - g_GS_TL1)%P1;"<<endl;
			out_indent(indent_num, gen_out);
			gen_out << "TC_" << *it + ".Data.C.C2 = " << "(" + get_bind_local_var(*it, function_name) + ".Data.C.C2 + " + int2str(constant4) + " - g_GS_TL2)%P2;"<<endl;
		}

		//�жϷ�������Ƿ���з���ֵ
		if(0==s->get_stamp())
		{
			//����޷���ֵ�������
		}
		else
		{
			//����з���ֵ�����жϷ��ر��ʽ������
			//�жϷ��ص��Ƿ��ǳ���
			if(3==s->get_ce()->get_stamp())
			{
				//����ǳ���
				string constant_name=s->get_ce()->get_c();//ȡ�������ַ���
				int high=str2int(constant_name);//ת���ɶ�Ӧ������ֵ
				int index = 0;

				//�����ø���������ǩ���ķ�������
				//get_const_xxxxx

				int constant_sig = getConstantSig(high);


				//���㳣���ĵ�λ
//				long long low = -((long long) high * K_NUM) % (long long) A
//						+ (long long) constant_sig;
//				low = (low % A + A) % A;
				long long low1 = -((long long) high * K_NUM_P1) % (long long) P1
						+ (long long) constant_sig;
				low1 = (low1 % P1 + P1) % P1;
				long long low2 = -((long long) high * K_NUM_P2) % (long long) P2
						+ (long long) constant_sig;
				low2 = (low2 % P2 + P2) % P2;
				//���㲹������
				long long constant = (long long) get_current_sig(var_infos,
						"retVal_" + var_scope, "global")
						- (long long) constant_sig;
				long long constant1 = (constant % (long long) P1 + (long long) P1)
						% (long long) P1;
				long long constant2 = (constant % (long long) P2 + (long long) P2)
										% (long long) P2;
				//����ǵ���ģʽ�������������Ϣ�����صĳ�����ǩ������������ֵ�Ĺ̶�ǩ��
				/*if(true==debug_flag)
				{
					out_indent(indent_num, gen_out);
					gen_out << "//" << constant_name << "'s  sig :"
							<< constant_sig << " retVal_" << var_scope
							<< "'s  sig :"
							<< get_current_sig(var_infos, "retVal_" + var_scope,
									"global") << endl;
				}*/
				gen_out << "return" << " "<<constant_name<< ";"<<endl;}
				//�������ֵ������ĸ�λ����
				/*out_indent(indent_num, gen_out);
				gen_out << "AC_RetVal_" << var_scope << ".Data.F = "
						<<  "CONST_NUM_" + int2HexStr_data(high) << ".Data.F;" << endl;

				//�������ֵ������ĵ�λ����
				out_indent(indent_num, gen_out);
				//��ʹ��64λ��װ������������
				gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<<"CONST_NUM_" + int2HexStr_data(high)<< ".Data.C.C1"<<" + g_GS_TL1 + g_ExFun_" + var_scope<<"  + "<<constant1<<")%P1;"<<endl;
				out_indent(indent_num, gen_out);
				gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<<"CONST_NUM_" + int2HexStr_data(high)<< ".Data.C.C2"<<" + g_GS_TL2 + g_ExFun_" + var_scope<<"  + "<<constant2<<")%P2;"<<endl;
			}*/
			else if (4 == s->get_ce()->get_stamp()
					&& 0 == s->get_ce()->get_cvar()->get_stamp())
			{
				//������ص��Ǹ���ͨ����
				string var_name = s->get_ce()->get_cvar()->get_name();//ȡ���ñ�������
				if(!is_global_var(var_name))
				{
					//���㲹������
					long long constant = (long long) get_current_sig(var_infos,
							"retVal_" + var_scope, "global")
							- (long long) get_current_sig(var_infos, var_name,
									var_scope);
					long long constant1 = (constant % (long long) P1 + (long long) P1) % P1;
					long long constant2 = (constant % (long long) P2 + (long long) P2) % P2;
					//����ǵ���ģʽ�������������Ϣ�����صı�����ǩ������������ֵ�Ĺ̶�ǩ��
					/*if (true == debug_flag)
					{
						out_indent(indent_num, gen_out);
						gen_out << "//retVal_" << var_scope << "'s  sig : "
								<< get_current_sig(var_infos, "retVal_" + var_scope,
										"global") << endl;
						out_indent(indent_num, gen_out);
						gen_out << "//" << var_name << "'s  sig : "
								<< get_current_sig(var_infos, var_name, var_scope)
								<< endl;
					}*/
					if(var_name.size()>7 && var_name.substr(0,7) == "AC_Tmp_")
					{
						//�������ֵ������ĸ�λ����
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_" << var_scope << ".Data.F = " << var_name
								<< ".Data.F;" << endl;

						//��ʹ��64λ��װ������������
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<< var_name<< ".Data.C.C1 +"<<"g_ExFun_" + var_scope<<" + "<<constant1<<")%P1;"<<endl;
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<< var_name<< ".Data.C.C2 +"<<"g_ExFun_" + var_scope<<" + "<<constant2<<")%P2;"<<endl;
					}
					else
					{

						/*<< ".Data.F = TC_" << var_name
								<< ".Data.F;" << endl;*/
						gen_out << "long long "<<var_name<<"="<<"check_decode (" <<var_name<<"_1,"<<var_name<<"_2);"<<endl;
						gen_out << "return" << " "<<var_name<< ";"<<endl;
						//��ʹ��64λ��װ������������
						out_indent(indent_num, gen_out);
						/*gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<<"TC_" + var_name<< ".Data.C.C1 +"<<"g_ExFun_" + var_scope<<" + "<<constant1<<")%P1;"<<endl;
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<<"TC_" + var_name<< ".Data.C.C2 +"<<"g_ExFun_" + var_scope<<" + "<<constant2<<")%P2;"<<endl;
					*/
					}



				}
				else
				{
					//�������ֵ��ȫ�ֱ���
					//���㲹������
					long long constant = (long long) get_current_sig(var_infos,
							"retVal_" + var_scope, "global")
							- (long long) get_current_sig(var_infos, get_bind_local_var(var_name, function_name),
									var_scope);
					long long constant1 = (constant % (long long) P1 + (long long) P1) % P1;
					long long constant2 = (constant % (long long) P2 + (long long) P2) % P2;
					//����ǵ���ģʽ�������������Ϣ�����صı�����ǩ������������ֵ�Ĺ̶�ǩ��
					if (true == debug_flag)
					{
						out_indent(indent_num, gen_out);
						gen_out << "//retVal_" << var_scope << "'s  sig : "
								<< get_current_sig(var_infos, "retVal_" + var_scope,
										"global") << endl;
						out_indent(indent_num, gen_out);
						gen_out << "//" << get_bind_local_var(var_name, function_name) << "'s  sig : "
								<< get_current_sig(var_infos, get_bind_local_var(var_name, function_name), var_scope)
								<< endl;
					}

					//�������ֵ������ĸ�λ����
					out_indent(indent_num, gen_out);
					gen_out << "AC_RetVal_" << var_scope << ".Data.F =" << get_bind_local_var(var_name, function_name)
							<< ".Data.F;" << endl;
					//��ʹ��64λ��װ������������
					out_indent(indent_num, gen_out);
					gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<< get_bind_local_var(var_name, function_name)<< ".Data.C.C1 +"<<"g_ExFun_" + var_scope<<" + "<<constant1<<")%P1;"<<endl;
					out_indent(indent_num, gen_out);
					gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<< get_bind_local_var(var_name, function_name)<< ".Data.C.C2 +"<<"g_ExFun_" + var_scope<<" + "<<constant2<<")%P2;"<<endl;
				}

			}
			else
			{
				//�������ֵ���ʽΪ�������ͣ����׳�������Ϣ
				string error_info = "return expression is invalid!";
				throw error_info;
			}
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding return statement : " + str;
		throw error_info;
	}
}
