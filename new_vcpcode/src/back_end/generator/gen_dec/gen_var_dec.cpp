#include "gen_utils.h"
#include "bool_var.h"
#include "var_declaration.h"
#include "initializer.h"
#include "init_declarator.h"
#include "declaration_specifiers.h"
#include "declarator.h"
#include "expression.h"
#include "global_var.h"
#include <map>

//static int index2sig=0;
// �������ļ� : ../gen_utils.c
extern int get_bool_value(string name, list<Bool_var>* bool_vars, int stamp);

// �������ļ� : src/main_utils.cpp
extern bool debug_flag;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;
extern list<string>* global_var_names;
extern list<Global_var>* global_vars;
extern map<string, int>* pre_sigs;
extern void check_pt_arg(void *pt, string info);
extern string int2HexStr(const int num);
extern list<string>* datetime_var_list;
// �������ļ� : src/back_end/back_end_utils.cpp
extern string compute_exp(Expression* s, int* result);
extern const int P1;
extern const int P2;

//�������鸽�ӱ�����ʼֵ
//������create_extra_files.cpp
extern void SetArrayExtrInitSig(int Sig, int SigIndex);

//IO�����������
static void gen_IO_array_dec(string array_name, int sub);


//��¼��ǰ������ı��������Ĵ洢����
static string storage_type = "";

//��¼��ǰ������ı���������noadj��ǩֵ���ñ�ǩֵΪ1ʱ˵���˱�������ʱ����Ҫ��ʼ����
//Ϊ0ʱ��˵����ǰ����ı�������Ҫ���г�ʼ������ֵĬ�ϳ�ʼΪ-1
static int noadj_stamp = -1;

//��¼��ǰ������ı���������nodt��ǩֵ���ñ�ǩֵΪ1ʱ˵����ǰ�ı����ڱ�����ʱ����Ҫ��DT��
//Ϊ0ʱ��˵����ǰ�ı����ڱ�����ʱ��Ҫ��DT����ֵĬ�ϳ�ʼΪ-1
static int nodt_stamp = -1;

//��¼��ǰ������ı���������bool��ǩֵ���ñ�ǩֵΪ1ʱ˵����ǰ����ı���Ϊ����������
//Ϊ0ʱ��˵����ǰ����ı������ǲ�����������ֵĬ�ϳ�ʼΪ-1
static int bool_stamp = -1;



extern const string ARRAY_EXTR_NAME;
string storage_typef = "T_CODE";
int arr_check1 = 0;
int arr_check2 = 0;
int init_str = 0; //���ӱ�����ʼֵ
// ���ܣ�������ʼ�����������е�ÿ�����ʽ��ֵ���������������ָ���������С�
// ���������l-��ʼ������ָ��
// ���������v_tmp-�����洢������������ָ��
// ����ֵ��ִ�гɹ��򷵻ؼ����������������ֵ�������������У�ʧ�����׳�������Ϣ
// ���ߣ���ա������
string compute_init_list(list<Initializer>* l, vector<int>* v_tmp)
{
	try
	{
		//����������ʼ������ָ�����Ч��
		check_pt_arg(l, "initializer list's pointer");

		list<Initializer>::iterator iter; //��ʼ�����������
		for (iter = l->begin(); iter != l->end(); iter++)
		{
			//�жϵ�ǰ��������ָ�Ķ����Ƿ�Ϊ�򵥱��ʽ��ʼ��
			if (0 == iter->get_stamp())
			{
				//�����ǩΪ0��˵���Ǽ򵥱��ʽ��ʼ��
				int value = 0; //�洢�ñ��ʽ��ֵ
				string result = compute_exp(iter->get_exp(), &value); //����ñ��ʽ
				//�ж��Ƿ����ɹ�
				if (result == "wrong")
				{
					//�������ʧ�ܣ�����򷵻�wrong
					return "wrong";
				}
				else
				{
					//�������ɹ�����ֵ�������������
					v_tmp->push_back(value);
				}
			}
			else
			{
				//�����ǩ��Ϊ0��˵��Ϊ���������ʼ�������� a[2][3]={{1,2,3},{4,5,6}};
				//�ݹ���ü����ʼ�����������Ը���������м���
				string result = compute_init_list(iter->get_init_list(), v_tmp);
				//�жϸø����������޼���ɹ�
				if (result == "wrong")
				{
					//�������ʧ�ܣ��򷵻�wrong
					return "wrong";
				}
				else
				{
					//�������ɹ������������
					;
				}
			}
		}
		//��ʼ�����������ϣ������м��޷��أ�˵������Ԫ��ȫ������ɹ�
		return "right";
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "compute initializer list : " + str;
		throw error_info;
	}
}

// ���ܣ��жϴ���ı����Ƿ�Ϊ��������������ǣ���Ϊ��������ֵǩ���������˲������ֵǩ��
//		   ��Ϣ�������ָ���������У�������ǣ��򲻷���ǩ��
// ���������var_name-��������������
// 			      bool_sigs-�洢�в������ֵǩ����Ϣ������ָ��
//			      stamp-����Ƿ�Ϊ��������
// ���������bool_sigs-�洢�в������ֵǩ����Ϣ������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա������
static void assign_bool_sigs(string var_name, list<Bool_var>* bool_sigs,
		int stamp)
{
	try
	{
		//�������������������ַ�������Ч��
		check_str_arg(var_name, "boolean variable's name");
		//���������������ֵǩ������ָ�����Ч��
		check_pt_arg(bool_sigs, "boolean TF signature list's pointer");

		//�ж��Ƿ�Ϊ��������
		if (stamp == 1)
		{
			//���Ϊ������������Ϊ�˲��������������ֵǩ�����������������
			int t = get_sig();				//������ֵǩ��
			int f = get_sig();				//�����ֵǩ��
			//�½�һ���洢�ò������ֵǩ����Ϣ�Ķ���
			Bool_var* b_v = new Bool_var(var_name, var_scope, t, f);
			//���ö����������
			bool_sigs->push_back(*b_v);
			delete b_v;

			//������ڵ���ģʽ�����ӡ������Ϣ
			/*	if (true == debug_flag) {
			 out_indent(indent_num, gen_out);
			 gen_out << "//boolean varibale " << var_name
			 << "'s true value sig : " << t << ", false value sig : "
			 << f << endl;
			 }*/
		}
		else
		{
			//�����Ϊ�����������򲻷������ֵǩ��
			;
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "assign booean vaiable's TF signatures : " + str;
		throw error_info;
	}
}

//���ܣ�������г�ʼ�����������������룬������λ��У��λ��ǩ����ʶ��Ϊ�������ۣ�ÿ���2������Ԫ����Ϣ��ͻ���

// ���������array_name-�������������
//                  sub-������±�ֵ
//		          v_tmp-�洢�������ʼ��ֵ��vector��ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�������
static void out_array_init(string array_name, int sub, vector<int>* v_tmp)
{
	try
	{
		string comp1, comp2;
		//�����������������ַ�������Ч��
		check_str_arg(array_name, "array's name");
		//�������������ʼ��ֵvectorָ�����Ч��
		check_pt_arg(v_tmp, "array initializer vector's pointer");

		//�������������ǩ��
		int array_sig = get_current_sig(var_infos, array_name, var_scope);
		//���������������ñ������������λ��DT����
		string GLOBAL_DT = var_scope == "global" ? "" : " + GLOBAL_DT";
		out_indent(indent_num, gen_out);
		gen_out << storage_typef << " " << "TC_" << array_name << "[" << sub << "]";
		unsigned int k = 0;	//���Ѿ����������Ԫ�ؽ��м���
		if ("extern " == storage_type)
		{
			//����ǣ����������ĳ�ʼ������
			gen_out << ";" << endl;
		}
		else
		{
			//������ǣ����������ĸ�λ��ʼ������
			gen_out << " = ";
			unsigned int k = 0;			//���Ѿ����������Ԫ�ؽ��м���
			//���ÿ��Ԫ�ص�ֵ
			for (vector<int>::iterator iter = v_tmp->begin();
					iter != v_tmp->end(); iter++)
			{
				//�������������ǩ���Լ�����Ԫ�ص�ֵ�������Ԫ�صĵ�λ
				int low1 =
						((-((long long) (*iter) * K_NUM_P1) % P1
								+ ((long long) k * K_NUM_P1) % P1 + array_sig)
								% P1 + P1) % P1;
				int low2 =
						((-((long long) (*iter) * K_NUM_P2) % P2
								+ ((long long) k * K_NUM_P2) % P2 + array_sig)
								% P2 + P2) % P2;
				//���ж��������λ��Ԫ�ظ����Ƿ�>=2����Ϊ10�ı���
				if (iter == --v_tmp->end())
				{
					out_indent(indent_num, gen_out);
					out_indent(indent_num, gen_out);
					gen_out << "{{" << *iter << ", {" << low1 << ", " << low2 << "}}, " << array_sig << "}";

				}
				else
				{

					if (k == 0)
					{
						gen_out << "{{{" << *iter << ", {" << low1 << ", " << low2 << "}}, " << array_sig << "}, ";
						gen_out << endl;
					}
					else
					{
						out_indent(indent_num, gen_out);
						out_indent(indent_num, gen_out);
						gen_out << "{{" << *iter << ", {" << low1 << ", " << low2 << "}}, " << array_sig << "}, ";
						gen_out << endl;
					}
				}
				k++;
			}

			gen_out << "};" << endl << endl;
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "out put array's high value with initializers : "
				+ str;
		throw error_info;
	}
}

//���ܣ����������ʼ�����������������룬������λ��У��λ��ǩ����ʶ��Ϊ�������ۣ�ÿ���2������Ԫ����Ϣ��ͻ���

// ���������array_name-�������������
//                  sub-������±�ֵ
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�������
//����
static void out_array_init(string array_name, int sub)
{
	out_indent(indent_num, gen_out);
	gen_out << storage_typef << " " << "TC_" << array_name << "[" << sub << "]={0}";
}

// ���ܣ��������Դ��г�ʼ��ֵ����ͨ�����������б��룬��������ı����������ʼ��ֵ��
//           ����ñ���������������롣
// ���������var_name-������ı�������
//			      value-������ı����ĳ�ʼ��ֵ
// ����ֵ���ɹ��򷵻�void
// ���ߣ���ա������
static void out_var_dec(string var_name, int value)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(var_name, "variable's name");

		//���Ҹñ����ĵ�ǰǩ��
		int var_sig = get_current_sig(var_infos, var_name, var_scope);
		//���ݱ����ĳ�ʼ��ֵ��ǩ���������λֵ
		int var_low_1 = ((-((long long) value * K_NUM_P1) % P1 + var_sig) % P1
				+ P1) % P1;
		int var_low_2 = ((-((long long) value * K_NUM_P2) % P2 + var_sig) % P2
				+ P2) % P2;
		//�жϱ���ʱ��λĩβ�Ƿ�Ҫ���"+DT"
//		string DT1 = var_scope == "global" ? "" : "+ g_GS_TL1";
//		string DT2 = var_scope == "global" ? "" : "+ g_GS_TL2";
		string DT1 = "";
		string DT2 = "";
		int sigIndex = get_current_sigIndex(var_infos, var_name, var_scope);
		//���ݴ洢�����Ƿ�Ϊextern�����ø�λ��ʼ���ַ���
		if ("extern " != storage_type)
		{
			//index2sig = index2sig + 1;
			string T_CODE_init_str = " = {{" + int2HexStr(value) + ", {"
					+ int2HexStr(var_low_1) + "" + DT1 + ", "
					+ int2HexStr(var_low_2) + "" + DT2 + "}}, "
					+ int2HexStr(sigIndex) + "}";
			//out_indent(indent_num, gen_out);
			if (TmpVarJudge(var_name))
			{
				datetime_var_list->push_back(var_name);
				if (storage_type == "")
//					gen_out << "T_CODE12" << " " << var_name << T_CODE_init_str << ";" << endl;
					{
					out_indent(indent_num,gen_out);
					gen_out << "long long "  << var_name<< "_1" <<"="<<value<<"*A1"<< ";" << endl;
					out_indent(indent_num,gen_out);
					gen_out << "long long " << var_name<< "_2"  <<"="<<value<<"*A2"<< ";" << endl;}
				else
					gen_out << storage_type << " T_CODE" << " " << var_name << T_CODE_init_str << ";" << endl;
			}
			else
			{
				if (var_scope != "global")
					datetime_var_list->push_back("TC_" + var_name);
				if (storage_type == "")
				{	//gen_out << "T_CODE" << " TC_" << var_name << T_CODE_init_str << ";" << endl;
					out_indent(indent_num,gen_out);
					gen_out << "long long "  << var_name<< "_1" <<"="<<value<<"*A1"<< ";" << endl;
					out_indent(indent_num,gen_out);
				gen_out << "long long " << var_name<< "_2"  <<"="<<value<<"*A2"<< ";" << endl;}
				else
					gen_out << storage_type << " T_CODE" << " TC_" << var_name << T_CODE_init_str << ";" << endl;
			}
		}
		else if ("const" == storage_type)
		{
			string T_CODE_init_str = " = {{(" + int2HexStr(value) + "), {("
					+ int2HexStr(var_low_1) + ")" + DT1 + ", ("
					+ int2HexStr(var_low_2) + ")" + DT2 + "}}, ("
					+ int2HexStr(sigIndex) + ")}";
			datetime_var_list->push_back(var_name);
			gen_out << storage_type << " T_CODE" << " TC_" << var_name << T_CODE_init_str << ";" << endl;
			gen_out << "T_CODE" << " AC_BIND_" << var_name << ";" << endl;
		}
		else
		{
			out_indent(indent_num, gen_out);
			if (TmpVarJudge(var_name))
			{
				if (storage_type == "")
					gen_out << "T_CODE" << " " << var_name << "" << ";" << endl;

				else
					gen_out << storage_type << " T_CODE" << " " << var_name << "" << ";" << endl;
			}
			else
			{
				if (storage_type == "")
					gen_out << "T_CODE" << " TC_" << var_name << "" << ";" << endl;

				else
					gen_out << storage_type << " T_CODE" << " TC_" << var_name << "" << ";" << endl;
			}
		}

		//���ݴ洢�����Ƿ�Ϊextern�����õ�λ��ʼ���ַ���
		//	string l_init_str=("extern "==storage_type) ? "":" = "+int2str(var_low)+DT1;

		//out_indent(indent_num,gen_out);
		//�����λ�Ĵ���
		//gen_out<<storage_type<<str_int32<<" H_"<<var_name<<h_init_str<<";"<<endl;
		/*out_indent(indent_num,gen_out);
		 //�����λ�Ĵ���
		 gen_out<<storage_type<<str_int32<<" L_"<<var_name<<l_init_str<<";"<<endl;*/
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info =
				"out put variable declaration's high and low codes with  initializers : "
						+ str;
		throw error_info;
	}
}

// ���ܣ�������鸽�ӱ����ı��룬������λ�͵�λ�ı��롣��ȫ������;ֲ��������������
// ���������array_name-�������֡�
// ����ֵ���ɹ��򷵻�void��
// ���ߣ���ա������
// ����׷��
static void out_array_addvar(string array_name)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(array_name, "array's name");

		//string add_name="Array_EXTR";//�洢���ӱ���������
		int add_sig = get_current_sig(var_infos, ARRAY_EXTR_NAME, var_scope);//��ȡ���ӱ�����ǩ��
		int init_add_str = 0;
		arr_check1 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig)
				% P1;
		arr_check2 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig)
				% P1;
		// low1=((-((long long)(*iter)*K_NUM_P1)%p1+((long long)k*K_NUM_P1)%p1+array_sig)%p1-p1)%p1;
		out_indent(indent_num, gen_out);
		gen_out << storage_typef << " " << ARRAY_EXTR_NAME << "={" << init_str << ",{" << arr_check1 << "," << arr_check2 << "}," << add_sig << "};" << endl;

	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "out put array's addtional variable  : " + str;
		throw error_info;
	}
}
void store_addvar(int add_sig)
{

	int init_add_str = 0;
	arr_check1 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig) % P1;
	arr_check2 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig) % P1;
}

// ���ܣ����������ʼ�����������������������룬�������λ�͵�λ��������룬�Լ�
//			���ӱ�����������룬����������������noadj��������Ҫ����������ж����ʼ��
// ���������array_name-���������
//			      sub-������±�
// ����ֵ���ɹ��򷵻�void
// ���ߣ���ա������
static void out_array_redcode(string array_name, int sub)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(array_name, "array's name");

		vector<int>* init_value = new vector<int>;			//���ڴ洢����Ԫ�س�ʼ����ֵ
		for (int i = 0; i < sub; i++)
		{
			init_value->push_back(0);
		}
		out_array_init(array_name, sub, init_value);

	} catch (string str)
	{
		//�ж���������Ƿ�Ϊnodt�������������ȷ���Ƿ�������ӱ����ı���
		if (0 == nodt_stamp)
		{
			//�������nodt�������������Ҫ����丽�ӱ����ı���
			//����  ���ӱ����ڸ�ֵʱ���
			out_array_addvar(array_name);
		}
		else
		{
			//�����nodt������������޸��ӱ���
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info =
				"out put array declaration's vital codes with no initializers : "
						+ str;
		throw error_info;
	}
}

// ���ܣ��������ʼ�����������������������룬�������λ�͵�λ��������룬�Լ�
//			���ӱ������������
// ���������array_name-���������
//			      sub-������±�
//			      v_tmp-�洢������Ԫ�س�ʼ��ֵ��vectorָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա������
static void out_array_redcode(string array_name, int sub, vector<int>* v_tmp)
{
	try
	{
		//�����������������ַ�������Ч��
		check_str_arg(array_name, "array's name");
		//�������������ʼ��ֵvectorָ�����Ч��
		check_pt_arg(v_tmp, "array initializer vector's pointer");

		//out_array_high(array_name,sub,v_tmp);//���������������ĸ�λ

		//������������Ϊȫ�ֱ����Ҳ�Ϊnoadj����
		int init_num = v_tmp->size();				//��¼����ʼ��������Ԫ�صĸ���

		//2010��3��9��׷��
		//�жϳ�ʼ���Ƿ����C�Ӽ��淶
		if (init_num > sub)
		{
			//�����ʼ�������������±꣬����
			string error_info = "initializers is more than declaration!";
			throw error_info;
		}

		if (init_num < sub)
		{
			//���δ��ȫ��ʼ��������
			string error_info = "initializers is less than declaration!";
			throw error_info;
		}
		if (sub <= 5000 && sub > init_num)
		{
			//����±�ֵ<=5000����������Ԫ��δ��ʼ����ȫ����û�г�ʼ����Ԫ�س�ʼ��Ϊ0
			for (int i = 0; i < sub - init_num; i++)
			{
				v_tmp->push_back(0);
			}
		}
		else
		{
			//��������±�>5000������<5000��������Ԫ�ؾ��Գ�ʼ��ʱ,���޸�vector�е�����
			;
		}
		//out_array_low(array_name,sub,v_tmp);//���������������ĵ�λ
		out_array_init(array_name, sub, v_tmp);		//������������ʼ���ṹ����Ϣ
		//�ж���������Ƿ�Ϊnodt�������������ȷ���Ƿ�������ӱ����ı���
		if (0 == nodt_stamp)
		{
			//�������nodt�������������Ҫ����丽�ӱ����ı���

		}
		else
		{
			//�����nodt������������޸��ӱ���
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info =
				"out put array declaration's vital codes with initializers : "
						+ str;
		throw error_info;
	}
}

// ���ܣ���ģ��Ϊ�������������������ģ��ĳ�ʼ��ģ�飬���ݴ���ı����������󣬷����ö���
// 			���ñ�ģ���е�һЩȫ�ֱ�����ֵ��
// ���������v_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա������
static void init_gen_vardec(Var_declaration* v_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(v_d, "variable declaration object's pointer");

		//����ȫ�ֱ����洢���͵�ֵ
		storage_type = v_d->get_decl_spec()->get_stor_spec();
		//�жϴ洢�����Ƿ�Ϊ"extern"����"static"
		if (storage_type == "extern")
		{
			//����ǣ������һ���ո�
			storage_type = storage_type + " ";
		}
		else
		{
			//������ǣ��򸳿�ֵ
			storage_type = "";
		}

		noadj_stamp = v_d->get_noadj_stamp();		//����noadj��ǩ��ֵ
		bool_stamp = v_d->get_bool_stamp();		//����bool��ǩ��ֵ
		nodt_stamp = v_d->get_nodt_stamp();		//����nodt��ǩ��ֵ
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "initializing in coding variable declaration : "
				+ str;
		throw error_info;
	}
}

// ���ܣ�Ϊ��������ǩ���������붯̬ǩ�����С�������һ����������������ǩ�����䣬�ֲ�����
// 			��ȫ�ֱ�����ǩ�����䡣����ñ���֮ǰ�Ѿ����������򷵻ص�ǩ��ֵΪ-1��
// ���������name-��������
// 			      var_stamp-�ñ����ı�ǩ��Ϣ
// ����ֵ���ɹ��򷵻��������ǩ��ֵ��ʧ�����׳�������Ϣ
// ���ߣ���ա������
int assign_var_sig(string name, string var_stamp)
{
	try
	{
		//��������������Ч��
		check_str_arg(name, "variable's name");
		check_str_arg(var_stamp, "variable's stamp");

		int sig = -1;		//�洢�ñ�����ǩ������ʼ��Ϊ-1
		//�жϵ�ǰ�������Ƿ���ȫ��
		if (var_scope == "global")
		{
			//�����ȫ�֣�����ȫ�ֱ����������в��Ҹñ���������
			//�ж��Ƿ���ȫ�ֱ������������ҵ��˸ñ���
			if (global_var_names->end()
					== find(global_var_names->begin(), global_var_names->end(),
							name))
			{
				//���û���ҵ���˵���ǵ�һ��������ȫ�ֱ������������ּ���ȫ�ֱ���������
				global_var_names->push_back(name);

				//��Ԥ����ǩ�������в��Ҹ�ȫ�ֱ��������Ƿ�Ϊ��Ԥ����ǩ��
				map<string, int>::iterator it = pre_sigs->find(name);
				int sigIndex = -1;
				//�жϲ��Һ������صĵ������Ƿ��������ĩ��
				if (pre_sigs->end() == it)
				{
					//������������ĩ�ˣ���˵����ȫ�ֱ���û��Ԥ��ǩ��
					sig = get_sig_index(&sigIndex);				//Ϊ�����һ��ǩ��
				}
				else
				{
					//���û�������ĩ�ˣ�˵����ȫ�ֱ�����Ԥ��ǩ��
					sig = it->second;			//��Ԥ��ǩ����Ϊ��ǩ��
				}
				//����ȫ�ֱ������붯̬ǩ����
				add_new_vars(var_infos, name, sig, var_scope, var_stamp,
						sigIndex);

				//������ڵ���ģʽ�������������Ϣ
				/*if (true == debug_flag) {
				 out_indent(indent_num, gen_out);
				 gen_out << "//variable " << name << "'s sig : " << sig
				 << endl;
				 }*/

				//�жϸñ����Ƿ��ǲ�����������Ϊ��������ֵǩ��
				assign_bool_sigs(name, bool_sigs, bool_stamp);
			}
			else
			{
				//����ҵ��ˣ�˵��֮ǰ��������ȫ�ֱ��������ٷ���ǩ��
				//����ֵΪ-1 ��ǩ��
				return sig;
			}
		}
		else
		{
			//��ǰ��������ȫ�֣���˵����ǰɨ�赽�ı�������Ϊ�ֲ���������
			//Ϊ�ñ�������ǩ���������ñ������붯̬ǩ������
			int sigIndex = -1;
			sig = get_sig_index(&sigIndex);
			add_new_vars(var_infos, name, sig, var_scope, var_stamp, sigIndex);
			//�жϸñ����Ƿ��ǲ�����������Ϊ��������ֵǩ��
			assign_bool_sigs(name, bool_sigs, bool_stamp);
		}
		return sig;
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "assign signatures for variable declaration : "
				+ str;
		throw error_info;
	}
}

// ���ܣ���Դ�����е���ͨ�����������б��룬��������ǩ���ķ��䣬�������������
// ���������var_name-������ı���������
//			      value-����ñ�������ʱ�ĳ�ʼ��ֵ
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա������
static void gen_common_dec(string var_name, int value)
{
	try
	{
		//���������������ַ�������Ч��
		check_str_arg(var_name, "variable's name");

		int var_sig = assign_var_sig(var_name, "common");			//��Ϊ��������ǩ��

		//�жϵ�ǰ�������Ƿ�Ϊȫ�֣��Ƿ�Ϊ�״������ñ���
		if (var_scope == "global" && var_sig != -1)
		{
			//�����ȫ�֣��������״�ɨ�赽�ñ�������
			//����Global_var���󣬲��������ȫ��Global_var������
			Global_var* new_gv = new Global_var(var_name, value, var_sig, 1);
			//����˱���Ϊnoadj���ͱ������򽫶���adj��ǩ��Ϊ"noadj"
			if (1 == noadj_stamp)
			{
				new_gv->set_adj_stamp("noadj");
			}
			global_vars->push_back(*new_gv);
			delete new_gv;

			//���ô���ʼ���ı����������뺯�����б���
			out_var_dec(var_name, value);
		}
		else
		{
			//��ǰ��������ȫ�֣����߲����״������ñ�������
			//������Global_var����ֱ������������
			out_var_dec(var_name, value);
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding  common variable declaration : " + str;
		throw error_info;
	}
}

//Ϊ���ӱ�������ǩ�������ӱ���Ϊȫ�ֱ���
//����ͬһ�������ڣ�ĳ�������Ϸ���ǩ��
//����׷�� 2015/3/16
int assign_extra_sig(string name, string var_stamp)
{
	//��������������Ч��
	check_str_arg(name, "variable's name");
	check_str_arg(var_stamp, "variable's stamp");
	if (global_var_names->end()
			== find(global_var_names->begin(), global_var_names->end(), name))
	{
		//���û���ҵ���˵���ǵ�һ��������ȫ�ֱ������������ּ���ȫ�ֱ���������
		global_var_names->push_back(name);
		//��Ԥ����ǩ�������в��Ҹ�ȫ�ֱ��������Ƿ�Ϊ��Ԥ����ǩ��
		map<string, int>::iterator it = pre_sigs->find(name);
		int sig = -1;		//�洢�ñ�����ǩ������ʼ��Ϊ-1
		int sig_index = -1;
		//�жϲ��Һ������صĵ������Ƿ��������ĩ��
		if (pre_sigs->end() == it)
		{
			//������������ĩ�ˣ���˵����ȫ�ֱ���û��Ԥ��ǩ��
			sig = get_sig_index(&sig_index);				//Ϊ�����һ��ǩ��
//����У����
			int init_add_str = 0;
			arr_check1 = (-((long long) init_add_str * K_NUM_P1) % P1 + sig)
					% P1;
			arr_check2 = (-((long long) init_add_str * K_NUM_P1) % P1 + sig)
					% P1;

			if(name == ARRAY_EXTR_NAME)
			{
				SetArrayExtrInitSig(sig, sig_index);
			}
			/*out_indent(indent_num, gen_out);
			 gen_out << "//" << name << " : (0," << arr_check1 << ","
			 << arr_check2 << "," << sig << ");" << endl;*/

		}
		else
		{
			//���û�������ĩ�ˣ�˵����ȫ�ֱ�����Ԥ��ǩ��
			sig = it->second;			//��Ԥ��ǩ����Ϊ��ǩ��
		}
		//����ȫ�ֱ������붯̬ǩ����
		add_new_vars(var_infos, name, sig, "global", var_stamp, sig_index);

		//������ڵ���ģʽ�������������Ϣ
		/*if (true == debug_flag) {
		 out_indent(indent_num, gen_out);
		 gen_out << "//variable " << name << "'s sig : " << sig << endl;
		 }*/
		return sig;
	}
}

// ���ܣ������������������������룬���а��������鼰�丽�ӱ���ǩ���ķ��䣬�ߵ�λ�ı��룬
// 			�������ܹ���Դ���д��г�ʼ���Ͳ�����ʼ���������������б��룬���ݴ����bool����������
// ���������array_name-���������
// 			      sub-������±�
// 			      init_values-�����������г�ʼ��ʱ�����Ԫ�س�ʼ��ֵ��vectorָ��
// 			      non_init-��Ǹ����������Ƿ���г�ʼ���Ĳ�����ǩ
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա������
static void gen_array_dec(string array_name, int sub, vector<int>* init_values,
		bool non_init)
{
	try
	{
		//��������������Ч��
		check_str_arg(array_name, "array's name");
		check_pt_arg(init_values, "array initializer vector's pointer");

		int array_sig = 0;			//�洢�����ǩ��
		int add_sig = 0;			//�洢���鸽�ӱ�����ǩ��

		if (1 == nodt_stamp)
		{
			//�����nodt�����������Ϊֻ�������ǩ��
			array_sig = assign_var_sig(array_name, "nodt_array");
		}
		else
		{
			//�������nodt�����������Ϊ����������丽�ӱ�������ǩ��
			array_sig = assign_var_sig(array_name, "array");	//Ϊ�����������ǩ��
			//add_name="Array_EXTR";//�������鸽�ӱ���������
			add_sig = assign_extra_sig(ARRAY_EXTR_NAME, "common");//Ϊ���鸽�ӱ�������ǩ��
		}
		//�ж��������Ƿ�Ϊȫ�֣����Ҹ������Ƿ��һ������������ȷ���Ƿ񽫸ñ�������Global_var����
		if (var_scope == "global" && array_sig != -1)
		{
			//���Ϊȫ�֣����ǵ�һ���������������
			//�����½�һ��Global_var���󣬴洢�������������Ϣ��������ȫ��Global_var����
			//�ж�Դ���и�������������Ƿ���г�ʼ��
			if (true == non_init)
			{
				//���Դ�����������������ʱû�г�ʼ��������Global_var����ʱ�������ʼ��ֵ
				Global_var* new_gv = new Global_var(array_name, sub, array_sig,
						2);
				//����˱���Ϊnoadj�������򽫶���adj��ǩ��Ϊ"noadj"
				if (1 == noadj_stamp)
				{
					new_gv->set_adj_stamp("noadj");
				}
				//���½���Global_var�������ȫ������
				global_vars->push_back(*new_gv);
				delete new_gv;
			}
			else
			{
				//���Դ��������������������г�ʼ��������Global_var����ʱ�����ʼ��ֵ
				Global_var* new_gv = new Global_var(array_name, sub, array_sig,
						init_values);
				//����˱���Ϊnoadj�������򽫶���adj��ǩ��Ϊ"noadj"
				if (1 == noadj_stamp)
				{
					new_gv->set_adj_stamp("noadj");
				}
				//���½���Global_var�������ȫ������
				global_vars->push_back(*new_gv);
				delete new_gv;
			}

			//�жϸ�����������޸��ӱ���
			if (0 == nodt_stamp)
			{
				//�������nodt���������˵���и��ӱ����������½�һ��Global_var����
				//�洢�����鸽�ӱ�������Ϣ��������ȫ��Global_var����
				//�ж�Global_var�������Ƿ����ARRAY_EXTR_NAME��û������� ����׷��
				int extr_sig = 0;
				list<Global_var>::iterator iterator_extr;
				iterator_extr = global_vars->begin();
				while (iterator_extr != global_vars->end())
				{
					if (iterator_extr->get_stamp() == 0
							&& iterator_extr->get_var_name() == ARRAY_EXTR_NAME)
						extr_sig = 1;
					iterator_extr++;
				}

				if (extr_sig == 0)
				{
					Global_var* new_gv_add = new Global_var(ARRAY_EXTR_NAME,
							add_sig);

					if (1 == noadj_stamp)
					{
						new_gv_add->set_adj_stamp("noadj");
					}
					global_vars->push_back(*new_gv_add);
					delete new_gv_add;
				}
			}
			else
			{
				//�����nodt���������˵���޸��ӱ���
				;
			}
		}
		//������������Ϊȫ����������������Ƿ�Ϊextern���������������ⲿ������ǩ
		if (var_scope == "global" && "extern " != storage_type)
		{
			//����ȫ�ֱ�������
			for (list<Global_var>::iterator iter = global_vars->begin();
					iter != global_vars->end(); iter++)
			{
				if (array_name == iter->get_var_name())
				{
					iter->set_ex(false);	//�����ⲿ������ǩΪ��ֵ��������������������ⲿ���ñ���
					break;
				}
			}
		}
		//�����Ƿ�Ϊextern ���������Ϣ
		if ("extern " == storage_type)
		{
			//ȫ���е�һ������extern����
			out_indent(indent_num, gen_out);
			gen_out << "extern T_CODE " << array_name << "[" << sub << "];" << endl;
		}
		else
		{
			//ȫ�ֵ�һ������extern���߾ֲ���������鶨�����
			if (true == non_init)
			{
				//���Դ�����������������ʱû�г�ʼ��������ò�����ʼ������������������뺯��
				out_array_redcode(array_name, sub);
			}
			else
			{
				//���Դ��������������������г�ʼ��������ô���ʼ������������������뺯��
				out_array_redcode(array_name, sub, init_values);
			}
		}

	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding  array declaration : " + str;
		throw error_info;
	}
}

//���IO���������
static void gen_IO_array_dec(string array_name, int sub)
{
	out_indent(indent_num, gen_out);
	gen_out << "extern INT32 " << array_name << "[" << sub << "];" << endl;
}

// ���ܣ������������������ɱ���������������룬���������λ�͵�λ��ǩ���ķ���ȡ�
// ���������v_d-������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ���ա������
void gen_var_dec(Var_declaration* v_d)
{
	try
	{
		//��������������������ָ�����Ч��
		check_pt_arg(v_d, "variable declaration object's pointer");

		//�жϸñ��������Ƿ���б������֣������Ƿ����Զ�����������
		if (v_d->get_stamp() == 1
				&& "typedef" != v_d->get_decl_spec()->get_stor_spec())
		{
			//������б������֣����Ҳ����Զ�����������������б�ģ��ĳ�ʼ��
			init_gen_vardec(v_d);

			//�����������ı�������
			for (list<Init_declarator>::iterator iter =
					v_d->get_inits()->begin(); iter != v_d->get_inits()->end();
					iter++)
			{
				//�жϵ�ǰ��������ָ�ı����Ƿ���г�ʼ��
				if (0 == iter->get_stamp())
				{
					//��ǩΪ0��˵���������ı���������ʼ��������int a;
					//�жϱ������ı����Ƿ�Ϊ��ͨ����
					if (0 == iter->get_dec()->get_stamp())
					{
						//�������ͨ����������ȡ���ñ�������
						string var_name = iter->get_dec()->get_name();
						//�жϸñ����Ƿ�Ϊ��������
						if (1 == bool_stamp)
						{
							//����ǣ���ȡ���ֵ
							int f_v = get_bool_value(var_name, bool_vars, 0);
							//ʹ�ü�ֵ�����ʼ��
							gen_common_dec(var_name, f_v);
						}
						else
						{
							//������ǲ���������ʹ��0�����ʼ��
							gen_common_dec(var_name, 0);
						}
					}
					else
					{
						//���������ͨ�����������������������������������б���
						string array_name = iter->get_dec()->get_name();//ȡ�����������
						int sub = 0;					//�洢�����±���ʽ��ֵ
						//���������±��ʽ
						string sub_result = compute_exp(
								&(iter->get_dec()->get_array_subs()->front()),
								&sub);
						//�ж��Ƿ����ɹ�
						if (sub_result == "wrong")
						{
							//�������ʧ�ܣ����׳�������Ϣ
							string error_info =
									"array's subscript is not constant! ";
							throw error_info;
						}
						else
						{
							//�������ɹ�����Բ�����ʼ������������������б���
							//�ж��Ƿ����ⲿ�����������
							if(!IsIOArrayName(array_name))
							{
								vector<int>* init_values = new vector<int>;
								gen_array_dec(array_name, sub, init_values, true);
								delete init_values;
							}
							else
							{
								//IO���飬ֱ�����������ʽ
								gen_IO_array_dec(array_name, sub);
							}

						}
					}
				}
				else
				{
					//��ǩΪ����ֵ��˵���������ı������г�ʼ��������int a=9;
					//�жϱ������ı����Ƿ�Ϊ��ͨ����
					if (0 == iter->get_dec()->get_stamp())
					{
						//���Ϊ��ͨ����������ȡ���ñ�������
						string var_name = iter->get_dec()->get_name();
						int value = 0;					//�洢�ñ����ĳ�ʼ��ֵ
						//�����ʼ�����ʽ��ֵ
						string init_result = compute_exp(
								iter->get_init()->get_exp(), &value);
						//�жϼ����Ƿ�ɹ�
						if (init_result == "wrong")
						{
							//�������ʧ�ܣ����׳�������Ϣ
							string error_info =
									"variable's initializer is not constant! ";
							throw error_info;
						}
						else
						{
							//�������ɹ�����Ըñ������б���

							//�жϸñ����Ƿ�Ϊ��������
							if (1 == bool_stamp)
							{
								//����ǣ�����ȡ�������ֵ
								int f_v = get_bool_value(var_name, bool_vars,
										0);						//��ֵ
								int t_v = get_bool_value(var_name, bool_vars,
										1);						//��ֵ
								if (0 == value)
								{
									//ʹ�ü�ֵ�����ʼ��
									gen_common_dec(var_name, f_v);
								}
								else if (f_v == value || t_v == value)
								{
									//ʹ�ó�ʼ��ֵ�����ʼ��
									gen_common_dec(var_name, value);
								}
								else
								{
									//ʹ����ֵ�����ʼ��
									gen_common_dec(var_name, t_v);
								}
							}
							else
							{
								//������ǲ���������ʹ�ü�������ĳ�ʼ��ֵ���г�ʼ��
								gen_common_dec(var_name, value);
							}
						}
					}
					else
					{
						//���������ͨ��������������������Դ���ʼ������������������б���
						string array_name = iter->get_dec()->get_name();//ȡ������������
						//�洢�����±���ʽ��ֵ
						int sub = 0;
						//���������±��ʽ
						string sub_result = compute_exp(
								&(iter->get_dec()->get_array_subs()->front()),
								&sub);

						//�洢�������Ԫ�صĳ�ʼ��ֵ
						vector<int>* init_values = new vector<int>;
						//���������ʼ�������ֵ
						string init_result = compute_init_list(
								iter->get_init()->get_init_list(), init_values);

						//�ж��±���ʽ�Ƿ����ɹ�
						if (sub_result == "wrong")
						{
							//�������ʧ�ܣ����׳�������Ϣ
							string error_info =
									"array's subscript is not constant! ";
							throw error_info;
						}
						//�жϳ�ʼ�������Ƿ����ɹ�
						else if (init_result == "wrong")
						{
							//�������ʧ�ܣ����׳�������Ϣ
							string error_info = "initializing is not constant! ";
							throw error_info;
						}
						else
						{
							//���������ɹ�����Դ��г�ʼ������������������б���
							gen_array_dec(array_name, sub, init_values, false);
							delete init_values;
						}
					}
				}
			}

		}
		else
		{
			//��������б������֣��������Զ��������������򲻱���
			;
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding variable declaration : " + str;
		throw error_info;
	}
}
//�Ƿ���IO���������
bool IsIOArrayName(string ArrayName)
{
	string substring_array_name = ArrayName.substr(0, ArrayName.length() - 1);
	string num = ArrayName.substr(ArrayName.length() - 1, 1);
	if ("g_USR_BufFetch" == substring_array_name || "g_USR_BufRecv" == substring_array_name
			|| "g_USR_BufSend" == substring_array_name || "g_USR_BufDeliver" == substring_array_name)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void gen_var_dec_datetime(string var_name)
{
	out_indent(indent_num, gen_out);
	gen_out << var_name << ".Data.C.C1 = " << var_name << ".Data.C.C1 + " << "g_GS_TL1;" << endl;
	out_indent(indent_num, gen_out);
	gen_out << var_name << ".Data.C.C2 = " << var_name << ".Data.C.C2 + " << "g_GS_TL2;" << endl;
}

