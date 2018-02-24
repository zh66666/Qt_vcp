#include "CodedVarBuild_Util.h"
#include "program.h"
#include "declaration.h"
#include "var_declaration.h"
#include "fun_declaration.h"
#include "var_info.h"
#include "cmnt.h"
//#include "TmpVarIndexRange.h"
#include "gen_secondary.h"
#include "gen_utils.h"
#include "gen_global_var.h"

// ���¶������ļ� : src/main_utils.c
extern int fake_line_num;
extern int line_num;
extern list<Var_info>* var_infos;
extern list<Cmnt>* cmnts;
extern string red_path;
extern string current_file_name;
//2010��3��10��׷��
extern string file_info;
//2014-12-16
extern map<string, TFuncTmpRange> g_MapFuncTmpVar;

extern string int2str(int num);
extern string resolve_file_name(string& path_name);
extern void check_pt_arg(void *pt, string info);
extern void out_cmnt(list<Cmnt>* cmnt_list, int line, ofstream& out);

// �������ļ� : ./gen_dec/gen_var_dec.c
extern void gen_var_dec(Var_declaration*);
extern void gen_var_dec_datetime(string s);

// �������ļ� : ./gen_dec/gen_fun_dec.c
extern void gen_fun_dec(Fun_declaration*);

// �������ļ� : back_end_utils.c
extern int tempvar_num_max;
extern int tempcondvar_num_max;
//�������ļ�:
//��������������TC_a.Data.C.C1 = TC_a.Data.C.C1 + g_GS_TL1;
extern list<string>* datetime_var_list;
extern bool flag_var_dec;
extern vector<string> get_global_var_infunc(string func_name);
extern void gen_globalToProtect_dec(string strs, string fun_name,string var_scope);
extern void genProtectToGlobal(string strs, string fun_name, string var_scope);

// ���ܣ�����������������������������룬�������������ͷֱ���ò�ͬ�²㺯��
// ���������s-��������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_declist(list<Declaration>* s)
{
	try
	{
		//����������������ָ�����Ч��
		check_pt_arg(s, "declaration object's pointer");
		//������������
		for (list<Declaration>::iterator iter = s->begin(); iter != s->end();
				iter++)
		{
			line_num = iter->get_line_begin(); //���õ�ǰ����λ�ڵ��к�
			fake_line_num = iter->get_fake_line(); //���õ�ǰ�����α�к�
			//2010��3��10��׷��
			file_info = iter->get_file_info();			//���õ�ǰ������λ�ڵ��ļ���

			out_cmnt(cmnts, fake_line_num, gen_out);			//�������֮ǰ��ע����Ϣ

			//�ж������Ƿ�Ϊ��������
			if (0 == iter->get_stamp())
			{
				//���������ǩΪ0��˵���Ǳ����������������������
				flag_var_dec=true;
				gen_var_dec(iter->get_cvd());
			}
			else
			{
				//���������ǩΪ��Ϊ0��˵���Ǻ����������������������
				gen_fun_dec(iter->get_cfd());
			}
		}

		if(flag_var_dec)
		{
			if ("F_VCE_CheckVar" == var_scope || "F_VCE_CheckArr" == var_scope)
			{
				//�����У��ӿھͲ��Ժ������ڵ��������б���
			}
			else
			{
				vector<string> globalvar = get_global_var_infunc(var_scope);
				vector<string>::iterator itrvec;
				for (itrvec = globalvar.begin(); itrvec != globalvar.end();
						itrvec++)
				{
					gen_globalToProtect_dec(*itrvec, var_scope, var_scope);
				}
			}

			check_pt_arg(datetime_var_list,
					"declaration which needs datetime  object's pointer");
			//������������
			gen_out << endl;
			for (list<string>::iterator iter = datetime_var_list->begin();
					iter != datetime_var_list->end(); iter++)
			{
				//gen_var_dec_datetime(*iter);
			}
			//ȫ�ֱ�������
			if ("F_VCE_CheckVar" == var_scope || "F_VCE_CheckArr" == var_scope)
			{
				//�����У��ӿھͲ��Ժ������ڵ��������б���
			}
			else
			{
				vector<string> globalvar = get_global_var_infunc(var_scope);
				vector<string>::iterator itrvec;
				for (itrvec = globalvar.begin(); itrvec != globalvar.end();
						itrvec++)
				{
					genProtectToGlobal(*itrvec, var_scope, var_scope);
				}
			}
			datetime_var_list->clear();
			flag_var_dec=false;
		}

	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "generate declaration's redundant codes : " + str;
		throw error_info;
	}
}

// ���ܣ�����������м�ṹ�����ɸó����Ӧ��������룬��ģ��Ϊ�����������ģ�����ģ�飬
//          �ڶԵ�ǰ�м�ṹ����֮ǰ���Ƚ���ֵ�ǰ�ṹ�õ�����ʱ������ǩ����Ϣ���붯̬ǩ����
//          �ṹ����֮���ٽ���Щ��ʱ�����Ӷ�̬ǩ������ɾ��
// ���������pro-����ת�����м�ṹָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
//���ߣ����
void gen_pro(Program* pro)
{
	string solve_path;
	string solve_file;
	string output_tmp_path;
	try
	{
		//���������м�ṹ����ָ�����Ч��
		check_pt_arg(pro, "program struct's pointer");

		//�Ƚ���ֵ�ǰ�м�ṹ�õ�����ʱ�������붯̬ǩ������Ϊ�����ڶԸýṹ���б�������
		//��������ʱ�򣬻��õ���Щ��ʱ������������ͨ��ʱ������������ʱ����

		solve_path = current_file_name;		//�洢��ǰ��������ļ�·����
		solve_file = resolve_file_name(solve_path);		//������ǰ��������ļ���
		output_tmp_path = red_path + "/red_tmp_" + solve_file;		//������������ļ���
		gen_out.open((char*) output_tmp_path.c_str());		//����������ļ���

		//�ж��ļ��Ƿ񴴽��ɹ�
		if (!gen_out.is_open())
		{
			//Ϊtrue��˵���ļ�����ʧ�ܣ��׳�������Ϣ
			string error_info = output_tmp_path + " creating failed!";
			throw error_info;
		}
	} catch (string str)
	{
		cerr << "error : when coding file \"" << current_file_name
				<< "\" : generate redundant codes," << str << endl;
		exit(-1);
	}

	try
	{
		//�����������ļ��е�ͷ�ļ�����
		gen_out << "#include \"Interface.h\" " << endl;
		gen_out << "#include \"Interface.c\" " << endl;
		int P_0=*iter;//��ʼǩ��by�ܺ�
		gen_out << "long long RS="<<P_0<<";"<< endl;
		gen_out << "int S_Fun_entry="<<P_0<<";"<< endl;
		gen_out << "int S_Fun_out;"<< endl;
		//gen_out << "#include \"CFG_Service.h\" " << endl;
/*
		gen_out << "#include \"stdio.h\" " << endl;
		gen_out << "long long A1=8193;" << endl;
		gen_out << "long long A2=8191;" << endl;
		gen_out << "long long P_pre_idx=0;" << endl;
		gen_out << "long long R=0;" << endl;
		gen_out << "long long mul_temp_A_1,mul_temp_A_2,mul_temp_B_1,mul_temp_B_2;" << endl;//�˷����õ����м����
		gen_out << "int div_temp_var_decode,div_temp_l,div_temp_r,div_temp_var;" << endl;//�˷����õ����м����
		gen_out << "int Select_decode,S_select,Select_mark;" << endl;//��֧��ڽ���ֵ ��֧ǩ�� ��֧��ڸ�λ��ʶ��
		gen_out << "int Iterat_decode,Iterat_mark;" << endl;// ѭ����ڽ���ֵ ѭ����ڸ�λ��ʶ��
*/
		//�����м�ṹ�е��������������������
		gen_declist(pro->get_declist());

		//����������ɽ����󣬽���ǰ��̬ǩ�����е���ʱ������Ϣ���
		for (list<Var_info>::iterator iter = var_infos->begin();
				iter != var_infos->end();)
		{
			//�жϵ�ǰ��������ָ����ı�ǩ��Ϣ�Ƿ���"tmpArray"����"tmpCondArray"
			if ("tmpArray" == iter->get_stamp()
					|| "tmpCondArray" == iter->get_stamp())
			{
				//����ǣ���˵����ǰ��ָ����Ϊ��ʱ����������ɾ��
				iter = var_infos->erase(iter);
			}
			else
			{
				//������ǣ���������Ҷ�̬ǩ����
				iter++;
			}
		}

		//����洢�ĵ�ǰ�ļ��е�ע��
		cmnts->clear();

		gen_out.close();		//�ر���������ļ���

		//���ж������
		string OutputFile_Path = red_path + "/red_" + solve_file;
		Second_Output_File(OutputFile_Path, output_tmp_path);

		//ɾ����ʱ�ļ�
		remove(output_tmp_path.c_str());

	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "generate program's redundant codes : " + str;
		throw error_info;
	}
}

