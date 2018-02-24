#include "var_info.h"
#include "bool_var.h"
#include "CodedVarBuild_Util.h"
#include "gen_utils.h"
#include "gen_global_var.h"
#include <string>
#include <vector>
#include <list>
#include <fstream>
using namespace std;

#define BOOL_T 1896943889
#define BOOL_F 2129587950

#define BIT64 1

// �������ļ� : src/main_utils.c
extern int A;
extern int indent_num;
extern ifstream fin;
extern void out_indent(int num, ofstream& out);
extern void check_pt_arg(void *pt, string info);
extern void check_str_arg(string arg_name, string info);
extern int getConstantSig(int constant);
static int sig_index = 1; //�ֲ���̬����   �洢ǩ������   ����



//�������ļ���src/generator/gen_op/gen_relationcmp.cpp
extern void PreCalRelationCmpSig();

ofstream gen_out;	//�����������ļ���
int iteration_level = -1;	//��¼ѭ�����Ĳ���
int select_level = -1;	//��¼��֧���Ĳ���
int dD_num = 0;	//dD���±�
long long K_NUM = 4294967296ll;	//2^32
#if BIT64

long long K_NUM_P1 =1106004ll; //P1:12970357 KΪ2^64   ��Ϊ2^64�޷���ʾ ��������2^64ģP���ֵ
long long K_NUM_P2 =799556ll; //P2:12239417 KΪ2^64

long long i_temp_P1=8224836ll;//��2^65��P1���� 8224836  ��2^33��2^32�������ȡģP1  (4213657*8427314)%P1
long long i_temp_P2=11711720ll;//��2^65��P2����  11711720   ��2^33��2^32�������ȡģP2 (4949054*9898108)%P1

#else

long long i_temp_P1=4213657ll;//��2^33��P1����  4213657
long long i_temp_P2=4949054ll;//��2^33��P2����  4949054

long long K_NUM_P1=4294967296ll;//32λ
long long K_NUM_P2=4294967296ll;//32λ

#endif
string var_scope = "global";	//��¼���������������������

int Int32TmpVarCnt=0;//int 32���͵���ʱ����������
//ʱ���ǩ����
int TimeStampInit;//ʱ���ǩ��ʼֵ
int DeltaTimeStamp;	//ʱ���ǩ����

//����ǩ��ֵ
int Bool_True_Sig;
int Bool_False_Sig;
//��ϵ������ص�ǩ��ֵ
int Diff1Sig[6];//��¼��ֵǩ�� Diff1
int Diff2Sig[6];//��¼��ֵǩ��Diff2
int CalOutSigK[6];//�����֧����ǩ����Kֵ
int VarSig1[6];//True��֧���������ǩ��
int VarSig2[6];//False��֧���������ǩ��

// ���ܣ���ǩ���ļ��ж�ȡǩ������������ļ���β�����׳�����
// ��������
// ����ֵ���ɹ��򷵻ض�����ǩ��ֵ��ʧ�����׳�������Ϣ
// ���ߣ����
int get_sig()
{
	//���ȫ���ļ����Ƿ��Ѿ�����
	if (0 == fin)
	{
		//����ļ���Ϊ�գ�˵���ļ�û�д򿪣��׳�������Ϣ
		string error_info = "read signature  : signature file is not opened! ";
		throw error_info;
	}
	else
	{
		//����ļ�����Ϊ�գ�˵���ļ��Ѿ��򿪣����ȡ
		//�ж��Ƿ��Ѿ������ļ���ĩβ
		if (!fin.eof())
		{
			//���û�е��ļ�ĩβ�����ǩ��
			int sig = 1;		//�洢������ǩ��
			fin >> sig;
			return sig;
		}
		else
		{
			//��������ļ���ĩβ�����׳�������Ϣ
			string error_info = "read signature : it is end of the file!";
			throw error_info;
		}
	}
}

/*���ܣ���ǩ������˳���ȡǩ�������ǩ���±�
 * ����������ָ�� index
 ����ֵ���ɹ��򷵻ض�����ǩ���������¾�̬����ǩ������ֵ��ʧ���򱨳�����Ϣ
 ���ߣ�����*/
int get_sig_index(int *index)
{
	//���ȫ���ļ����Ƿ��Ѿ�����
	if (0 == fin)
	{
		//����ļ���Ϊ�գ�˵���ļ�û�д򿪣��׳�������Ϣ
		string error_info = "read signature  : signature file is not opened! ";
		throw error_info;
	}
	else
	{
		//����ļ�����Ϊ�գ�˵���ļ��Ѿ��򿪣����ȡ
		//�ж��Ƿ��Ѿ������ļ���ĩβ
		if (!fin.eof())
		{
			//���û�е��ļ�ĩβ�����ǩ��
			int sig = 1;		//�洢������ǩ��
			fin >> sig;
			if (index != NULL)
				*index = sig_index;		//ָ��洢ǩ������

			sig_index++;		//ǩ����������
			return sig;
		}
		else
		{
			//��������ļ���ĩβ�����׳�������Ϣ
			string error_info = "read signature : it is end of the file!";
			throw error_info;
		}
	}

}

// ���ܣ�Ϊ��̬ǩ�����е�ĳ�����������ǩ�������ݱ���������ǰλ�ڵ��������������ñ�����ǩ��ջ��
//           ��ջ�м�����ǩ����
// ���������s-��̬ǩ����ָ��
// 		         str-��������
//                 sig-������ǩ��
//                 scope-�������ڵ�������
// ���������s-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void add_new_sig(list<Var_info>* s, string str, int sig, string scope)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s, "var_info list's pointer");
		check_str_arg(str, "variable's name");
		check_str_arg(scope, "variable's scope");

		//���Ҷ�̬ǩ��������ǩ��
		list<Var_info>::iterator iter;		//��̬ǩ���������
		for (iter = s->begin(); iter != s->end(); ++iter)
		{
			//�жϵ�ǰ��������ָ��������ֺ��������Ƿ���Ҫ���ұ�����һ��
			if (scope == iter->get_scope() && str == iter->get_name())
			{
				//���һ�£�����ǩ�����룬ͬʱ����
				iter->add_sigs(sig);
				return;
			}
			else
			{
				//�����һ�£������������
			}
		}
		//���û���ҵ����������򻻳�"global"�ٲ���һ��
		for (iter = s->begin(); iter != s->end(); ++iter)
		{
			//�жϵ�ǰ��������ָ����������Ƿ��Ҫ���ҵı�����һ�£���������Ϊ"global"
			if ("global" == iter->get_scope() && str == iter->get_name())
			{
				//���һ�£�����ǩ�����룬ͬʱ����
				iter->add_sigs(sig);
				return;
			}
			else
			{
				//�����һ�£������������
			}
		}
		//������û��������˵��û���ҵ��ñ������׳�����
		string error_info = "can not find varibale : " + str;
		throw error_info;
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "add new signature : " + str;
		throw error_info;
	}
}

// ���ܣ��ڶ�̬ǩ����������±�������������������ʼǩ����������ͱ�����ǩ��Ϣ������ڶ�̬ǩ����
//           �У����ִ��������д˱����Ѿ����ڣ����׳�����
// ���������s-��̬ǩ����ָ��
//                  str-Ҫ���ӵı�����
//                  sig-�ñ����ĳ�ʼǩ��
//                  scope-�ñ���������������
//                  stamp-�ñ����ı�ǩ��Ϣ
// ���������s-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void add_new_vars(list<Var_info>* s, string str, int sig, string scope,
		string stamp,int sigIndex)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s, "var_info list's pointer");
		check_str_arg(str, "variable's name");
		check_str_arg(scope, "variable's scope");
		check_str_arg(stamp, "variable's stamp");

		list<Var_info>::iterator iter;		//��̬ǩ���������
		for (iter = s->begin(); iter != s->end(); ++iter)
		{
			//�жϵ�ǰ��������ָ��������ֺ��������Ƿ���Ҫ���ұ�����һ��
			if (scope == iter->get_scope() && str == iter->get_name())
			{
				//���һ�£�˵���ñ����Ѿ����ڣ����׳�������Ϣ
				string error_info = str + " has already exists in the scope : "
						+ scope;
				throw error_info;
			}
			else
			{
				//�����һ�£������������
			}
		}

		//���ҽ������жϵ������Ƿ��������ĩ��
		if (iter == s->end())
		{
			//�������ĩ�ˣ���˵����̬ǩ�����в����ڸñ����������ñ�������Ϣ
			Var_info* var_info = new Var_info(str, sig, scope);		//����ñ�����Ϣ����
			var_info->set_stamp(stamp);		//������Ϣ����ı�ǩ��Ϣ
			var_info->set_sigIndex(sigIndex);
			s->push_back(*var_info);		//�����б�����Ϣ�Ķ�����붯̬ǩ����
			delete var_info;
		}
		else
		{
			//���δ��ĩ�ˣ������
		}
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "add new variable : " + str;
		throw error_info;
	}
}

// ���ܣ��ڲ������ֵ�����У����ݲ����������ֲ��������ֵ�������ݲ�����������ֵ���߼�ֵ�����
//           û���ҵ����򷵻ع̶�����ֵ���߼�ֵ
// ���������name-������������
//                  bool_vars-����в������ֵ��Ϣ�������ָ��
//                  stamp-�������ҵ�����ֵ���Ǽ�ֵ�����Ϊ0������Ҽ�ֵ��Ϊ1���������ֵ
// ����ֵ���ɹ��򷵻ز��������ֵ��ʧ�����׳�������Ϣ
// ���ߣ����
int get_bool_value(string name, list<Bool_var>* bool_vars, int stamp)
{
	try
	{
		//���������������ֵ����ָ�����Ч��
		check_pt_arg(bool_vars, "bool value list's pointer");
		//�������������������ַ�������Ч��
		check_str_arg(name, "variable's name");
		int value = 0;		//�洢Ҫ���ص���ֵ���߼�ֵ
		list<Bool_var>::iterator iter;		//�������ֵ���������
		for (iter = bool_vars->begin(); iter != bool_vars->end(); iter++)
		{
			//�жϵ�ǰ��������ָ�Ķ���������Ƿ���Ҫ���ҵĲ���������һ��
			if (iter->get_name() == name)
			{
				//���һ�£�˵�����ڸò�������������stamp��ֵȡ����ֵ���߼�ֵ
				value = (0 == stamp) ? (iter->get_f()) : (iter->get_t());
				break;
			}
			else
			{
				//�����һ�£����������
			}
		}
		//���ҽ������жϵ������Ƿ�������ĩ��
		if (iter == bool_vars->end())
		{
			//�������ĩ�ˣ�˵��������û�иò���������û��Ϊ�ò�����ָ�����ֵ���򷵻�һ���̶�����ֵ
			//���߼�ֵ�����ҽ������ֵ��Ϊ�ò����������ֵ����������
			value = (0 == stamp) ? (BOOL_F) : (BOOL_T);

			Bool_var* b_v = new Bool_var(name, var_scope, BOOL_T, BOOL_F);
			bool_vars->push_back(*b_v);
			delete b_v;
		}
		return value;
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "get bool  value : " + str;
		throw error_info;
	}
}

// ���ܣ��ڲ������ֵǩ�������У����ݲ����������ֲ��������ֵ��ǩ���������ݲ�����ѡ�񷵻���ֵ
//          ��ǩ�����Ǽ�ֵ��ǩ�������û���ҵ����������ǩ��ͬʱ����
// ���������name-������������
//                  bool_sigs-����в������ֵǩ����Ϣ�������ָ��
//                  stamp-�������ҵ�����ֵǩ�����Ǽ�ֵǩ�������Ϊ0������Ҽ�ֵ��ǩ����Ϊ1�������ֵǩ��
// ����ֵ���ɹ��򷵻ز��������ֵǩ����ʧ�����׳�������Ϣ
// ���ߣ����
int get_bool_sig(string name, list<Bool_var>* bool_sigs, int stamp)
{
	try
	{
		//��������������Ч��
		check_pt_arg(bool_sigs, "bool signature list's pointer");//���������������ֵǩ������ָ�����Ч��
		check_str_arg(name, "variable's name");			//�������������������ַ�������Ч��

		int sig = 0;			//��¼Ҫ���ص�ǩ��ֵ
		list<Bool_var>::iterator iter;			//�������ֵǩ�����������
		//�����������ֵǩ������
		for (iter = bool_sigs->begin(); iter != bool_sigs->end(); iter++)
		{
			//�жϵ�ǰ��������ָ�Ķ���������Ƿ���Ҫ���ҵĲ���������һ��
			if (iter->get_name() == name)
			{
				//���һ�£�˵�����ڸò������������ݴ���ı�ǩֵ��ȡ����ֵǩ�����߼�ֵǩ��
				sig = (0 == stamp) ? (iter->get_f()) : (iter->get_t());
				break;
			}
			else
			{
				//�����һ�£����������
			}
		}
		//�˳�ѭ�����жϵ������Ƿ�ָ���������ĩ��
		if (bool_sigs->end() == iter)
		{
			//������������ĩ�ˣ�˵��û���ҵ��ò�������˵��Ϊ��ʱ��������Ϊ�����������ֵǩ����
			//���·�������ֵǩ����Ϣ�������ֵǩ������
			int t_sig = get_sig();			//������ֵǩ��
			int f_sig = get_sig();			//�����ֵǩ��
			Bool_var* b_v = new Bool_var(name, "global", t_sig, f_sig);
			bool_sigs->push_back(*b_v);
			delete b_v;

			sig = (0 == stamp) ? (f_sig) : (t_sig);
		}

		return sig;
	} catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "get bool  signature : " + str;
		throw error_info;
	}
}

//�жϱ������Ƿ�����ʱ����---����ʱ�����ǲ�ֹ�������������ʱ����
//������1������ָ��ӱ���
//2�����鸽�ӱ���
//3��ȫ�ֱ����󶨵ľֲ�����
//�����������ʱ����Ҫ����ǰ׺"TC_"
//����True:����ʱ���� ����Ҫ��"TC_"
//����False��������ʱ��������Ҫ��"TC_"
bool TmpVarJudge(string VarName)
{
	return 	VarName==ARRAY_EXTR_NAME || VarName.substr(0,::VOID_FUNC_RETVAL.size()) == ::VOID_FUNC_RETVAL ||
			ExtraVarJudge(VarName) || is_canbe_convert2global(VarName,var_scope);
}

//�жϱ������Ƿ��Ǹ��ӱ���---�˸��ӱ�������
//1������ֹ����������ı���(����ָ��ӱ���)
//��Щ���ӱ�������Ҫ�ڷ�֧��ѭ���н��в���
//True-�Ǹ��ӱ���������Ҫ����
//False-���Ǹ��ӱ�������Ҫ����
bool ExtraVarJudge(string VarName)
{
	return VarName.substr(0, ARITHM_INTTMP_PRE.size()) == ARITHM_INTTMP_PRE
			|| VarName.substr(0,ARITHM_UINTTMP_PRE.size())==ARITHM_UINTTMP_PRE
			|| VarName.substr(0, IF_TMP_BOOL_PRE.size()) == IF_TMP_BOOL_PRE
			|| VarName.substr(0, WHILE_TMP_BOOL_PRE.size())
					== WHILE_TMP_BOOL_PRE
			|| VarName.substr(0, RELATION_CMP_PRE.size()) == RELATION_CMP_PRE
			|| VarName.substr(0, IF_LOGICSUB_PRE.size()) == IF_LOGICSUB_PRE
			|| VarName.substr(0, WHILE_LOGICSUB_PRE.size())
					== WHILE_LOGICSUB_PRE;
}

//Ϊ�������ز�������ǩ��
//����������ֵǩ��
//��ϵ���������ص�ǩ��ֵ
void AssignCodeSig()
{
	//ʱ���ǩ����
	TimeStampInit=0;
	DeltaTimeStamp=get_sig_index(NULL);

	//����ֵ����
	Bool_True_Sig=get_sig_index(NULL);
	Bool_False_Sig=get_sig_index(NULL);

	//������0��ǩ��
	int Const_0_Sig=getConstantSig(0);

	//��ϵ������ص�ǩ��ֵ
	for(int i=0;i<6;i++)
	{
		Diff1Sig[i]=get_sig_index(NULL);
		if(i>=4)
			Diff2Sig[i]=get_sig_index(NULL);
		CalOutSigK[i]=get_sig_index(NULL);
		VarSig1[i]=get_sig_index(NULL);
		VarSig2[i]=get_sig_index(NULL);
	}
}

void InitSigInfo()
{
	AssignCodeSig();
	PreCalRelationCmpSig();
}

