#include "CodedVarBuild_Util.h"
#include "bool_var.h"
#include "var_info.h"
#include "cmnt.h"
#include "fun_info.h"
#include "global_var.h"
#include "TmpVarIndexRange.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_utils.h"
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
class Program;
//�˽ṹ���ڱ�����빤����dD�����ֺͶ�Ӧ��ֵ
struct dD_value
{
	string name;//��¼dD������
	int value;//��¼dD��ֵ
};

//��������Ϊϵͳ�е�ȫ������

int A = 0;												//�洢����A
int line_num=1;									//��¼ÿ�д�����ԭʼ�ļ��е��к�
int indent_num=0;								//��������ĸ���
int project_tempvar_num=0;				//��¼�������������ɵ���ʱ����tmp_Array_n �ĸ���
int project_tempcondvar_num=0;  	//��¼�������������ɵ���ʱ����tmp_CondArray_n �ĸ���
int macro_label=0;								//��¼�������ʱ��LABEL��ţ�ÿ���ļ���0��ʼ��ÿ���һ��������ã��ͼ�1
bool debug_flag=false;						//debugģʽ��ǣ�Ĭ��Ϊ��ֵ
string red_path="";							//��������ŵ�·��
string str_A="";									//�洢����A���ַ���
string enter_fun_name="";				//�洢�����빤���е���ں�������
string current_file_name="";				//�洢��ǰ��������ļ�������
ifstream fin;											//����ǩ���ļ��������ļ���

//2010��3��10��׷��
string  file_info="";	 //���ڼ�¼��ǰ�������Ĵ����������ļ�������
int fake_line_num=1;		//��¼ÿ�д�����չ������ļ��е��к�

string bool_config_file="/config/bool_values.txt";	//���������ļ���
string sig_config_file="/config/pre_sig.txt";				//Ԥ����ǩ���ļ���
string prime_config_file="/config/prime_value.txt";	//����A�����ļ���
string sig_list_file="/config/randNumSeqFile.txt";//ǩ���ļ���
string asm_macro_file="/config/asm.txt";//�������Դ�ļ���

//�洢���������빤����������������ȫ�ֱ���������
//�������в������ظ�������
list<string>* global_var_names = new list<string>;

//�洢���������빤���������������ĺ�������
//�������в������ظ�������
list<string>* function_names=new list<string>;

//�������������е���Ҫ��red_global_para_init()�е�����ȫ�ֱ���
list<Global_var>* global_vars=new list<Global_var>;

//��̬ǩ���������������������б��������ƣ�ǩ������Ϣ
//�Ժ���������ɺ󣬽�ǩ�����У��ú�����ľֲ����������
//��һ���ļ�������Ϻ󣬽��������ʱ���������
list<Var_info>* var_infos =new list<Var_info>;

//�洢���������е��������ĺ�����Ϣ,����������������������ֵ��
list<Fun_info>* fun_infos =new list<Fun_info>;

//�洢��ǰ�ļ��е�ע����Ϣ������ÿ��ע�͵����ݣ���ֹ�кŵ�
//��ǰ�ļ������������Ҫ�������ÿ�
list<Cmnt>* cmnts=new list<Cmnt>;

//�洢�������õ������в������������ֵ��Ϣ
list<Bool_var>* bool_vars=new list<Bool_var>;

//�洢�������õ������в������������ֵ��ǩ����Ϣ
list<Bool_var>* bool_sigs=new list<Bool_var>;

//�洢��Ԥ����ǩ���ļ��ж�ȡ����ǩ��
map<string, int>* pre_sigs=new map<string, int>;

//�洢�ڶ�ѭ���ṹ���б���ʱ��dD�����Ƽ����Ӧֵ
list<dD_value>* dD_values=new list<dD_value>;

//2014-12-15���� ÿ�������и�����ʱ�������������ݷ�Χ
map<string,TFuncTmpRange> g_MapFuncTmpVar;
//2014-12-29���� ��������ǩ�� ���
 map<int,int> constantMap;
 extern int get_sig_index(int *);
 extern ofstream gen_out;
extern void add_new_sig(list<Var_info>*, string, int, string);

extern void add_new_vars(list<Var_info>*,string,int,string,string,int);

//2015-04-13��� �洢�������ӱ������������VCL��
map<string, int> g_ExFun_Hash_map;
//2015-04-13��� �洢��������ֵȫ�ֱ�����VCL��
map<string, string>  AC_RetVal_map;
//2015-04-28��� �洢��������ֵȫ�ֱ�����VCL��
map<string, string>  AC_ParmVar_map;
//2015-06-09�����ӱ�����datetime_var_list�У��������ʱ���ǩ
list<string>* datetime_var_list=new list<string>;
//2015-06-10������ӣ��ж��Ƿ�ɨ�����������ı�ʶ
bool flag_var_dec=false;

//���º���Ϊϵͳ��Ƶ��ʹ�õĿ⺯��

// ���ܣ����string���͵Ĳ�����Ч�ԣ������Ƿ�Ϊ�����ж��Ƿ���Ч�����Ϊ�գ��������׳�������Ϣ
// ���������arg_name-�ϲ㴫���Ҫ����string����
// 			      info-�ϲ㴫��ĸ��ӱ�����Ϣ
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void check_str_arg(string arg_name,string info)
{
	//�жϴ���Ĳ������ַ����Ƿ�Ϊ��
	if(arg_name.empty())
	{
		//���Ϊ�գ����׳�������Ϣ
		string error_info="invalid argument : "+info+" is empty!";
		throw error_info;
	}
	else
	{
		//�����Ϊ�գ������
		;
	}
}

// ���ܣ����ָ�����͵Ĳ�����Ч�ԣ������Ƿ�ΪNULL���ж��Ƿ���Ч�����ΪNULL���������׳�������Ϣ
// ���������pt-�ϲ㴫���Ҫ����ָ�����
// 			      info-�ϲ㴫��ĸ��ӱ�����Ϣ
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void check_pt_arg(void *pt,string info)
{
	//�жϴ���Ĳ������ַ����Ƿ�Ϊ��
	if(NULL==pt)
	{
		//���Ϊ�գ����׳�������Ϣ
		string error_info="invalid argument : "+info+" is NULL!";
		throw error_info;
	}
	else
	{
		//�����Ϊ�գ������
		;
	}
}

//���ܣ�Ϊ�˴����ʽ����Ҫ���������
//���������num-�������Ʊ��������out-����ļ���
//���������out-����ļ���
//����ֵ���ɹ�����void��ʧ�����׳�����
void out_indent(int num, ofstream& out)
{
	//�������Ʊ���ĸ����Ƿ�Ϊ��
	if(num<0)
	{
		//���Ϊ��������
		string error_info="output indent ��indent number is invalid!";
		throw error_info;
	}
	//����ļ����Ƿ��
	else if(!out.is_open())
	{
		//�������ļ�û�д򿪣�����
		string error_info="output indent ��ofstream is not open!";
		throw error_info;
	}
	else
	{
		//�������������Ϊ����������ļ��Ѿ���
		for(int i=0;i<num;i++)
		{
			out<<"  ";	//��������ո�
		}
	}
}

// ���ܣ�������ת�����ַ���
// ���������num-int���͵Ĵ�ת��������
// ����ֵ������ת�������ַ���
// ���ߣ����
string  int2str( int   num)
{
   stringstream stream("");	//�ַ�����
   string str="";					//���ת�����ַ���
   stream << num;				//��numֵ������
   stream >> str;				//��stream�г�ȡǰ������numֵ
   return str;
}

//��������תΪʮ�������ַ������������������
//���磺������16������� 0x00000010
string int2HexStr(const int num)
{
	char *num_str=new char[100];
	sprintf(num_str,"%d",num);
//	sprintf(num_str,"%08X",num);
//	char HexSymbol[16]={'0','1','2','3'
//						,'4','5','6','7'
//						,'8','9','A','B'
//						,'C','D','E','F'};
//	string HexStr(8,'0');
//	int NumMod=0;
//	int CurNum=num>=0? num:-num;
//	for(int i=7;i>=0;i--)
//	{
//		if(CurNum>0)
//		{
//			NumMod=CurNum%16;
//			HexStr[i]=HexSymbol[NumMod];
//			CurNum>>=4;
//		}
//		else
//			break;
//	}
//	HexStr=num>=0? "(0x"+HexStr+")":"(-0x"+HexStr+")";
	string HexStr=num_str;
	delete num_str;
	return HexStr;
}

//����ת16���ƣ������������������󶨱���
string int2HexStr_data(const int num)
{
	char *num_str=new char[100];
	sprintf(num_str,"%08X",num);
	string hex_data="0x"+string(num_str);
	delete num_str;
	return hex_data;
}

// ���ܣ����ַ���ת��������
// ���������str-��ת�����ַ���
// ����ֵ���ɹ��򷵻�ת����������ֵ��ʧ�����׳�������Ϣ
// ���ߣ����
int   str2int( string   str)
{
	try
	{
		check_str_arg(str,"string ");//����������ת�����ַ����Ƿ�Ϊ��
		
		int num=0;//�洢ת����������ֵ
		//�ж��ַ����е������Ƿ�Ϊ16����
		if("0x"==str.substr(0,2) || "0X"==str.substr(0,2))
		{
			//����ǣ�������16���������룬��ת��Ϊʮ��������
			sscanf((char*)(str.c_str()),"%x",&num);    
		}
		else
		{
			//�ж��ַ����е������Ƿ�Ϊ8����
			if("0"==str.substr(0,1))
			{
				//����ǣ�������8���������룬��ת��Ϊʮ��������
				sscanf((char*)(str.c_str()),"%o",&num); 
			}
			else
			{
				//������ǣ�������10���������룬��ת��Ϊʮ��������
				sscanf((char*)(str.c_str()),"%d",&num); 
			}
		}
		return num;
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="convert string to int : "+s;
		throw error_info;
	}
} 


// ���ܣ��ڶ�̬ǩ�����У����ݱ��������Ƽ��䵱ǰλ�ڵ������򣬲��Ҹñ����ĵ�ǰǩ��
// ���������s-��̬ǩ����ָ��
// 		          str-��������
//                  scope-�ñ�����ǰ���ڵ�������
// ����ֵ���ɹ��򷵻ز��ҵ���ǩ��ֵ��ʧ�����׳�������Ϣ
// ���ߣ����
int get_current_sig(list<Var_info>* s,string str,string scope)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"var_info's list pointer ");
		check_str_arg(str,"variable's name ");
		check_str_arg(scope,"varibale's scope ");
		
		//��Ϊ�գ�����Ҷ�̬ǩ����
		list<Var_info>::iterator iter;//��̬ǩ���������
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//�жϵ�ǰ��������ָ��������ֺ��������Ƿ���Ҫ���ұ�����һ��
			if(scope==iter->get_scope() && str==iter->get_name())
			{
				//���һ�£��򷵻ص�ǰǩ��
				return iter->get_sigs()->back();
			}
			else
			{
			}
		}
		//���û���ҵ����������򻻳�"global"�ٲ���һ��
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//�жϵ�ǰ��������ָ����������Ƿ��Ҫ���ҵı�����һ�£���������Ϊ"global"
			if("global"==iter->get_scope() && str==iter->get_name())
			{
				//���һ�£��򷵻ص�ǰǩ��
				return iter->get_sigs()->back();
			}
			else
			{
			}
		}
		//������û�з��أ�˵��û���ҵ��ñ������׳�����
		string error_info="can not find varibale : "+str+"in var_info lis!";
		throw error_info;
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="get  current signature : "+s;
		throw error_info;
	}
}

// ���ܣ��ڶ�̬ǩ�����У����ݱ��������Ƽ��䵱ǰλ�ڵ������򣬲��Ҹñ����ĵ�ǰǩ������
// ���������s-��̬ǩ����ָ��
// 		          str-��������
//                  scope-�ñ�����ǰ���ڵ�������
// ����ֵ���ɹ��򷵻ز��ҵ���ǩ��ֵ��ʧ�����׳�������Ϣ
// ���ߣ����
int get_current_sigIndex(list<Var_info>* s,string str,string scope)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"var_info's list pointer ");
		check_str_arg(str,"variable's name ");
		check_str_arg(scope,"varibale's scope ");

		//��Ϊ�գ�����Ҷ�̬ǩ����
		list<Var_info>::iterator iter;//��̬ǩ���������
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//�жϵ�ǰ��������ָ��������ֺ��������Ƿ���Ҫ���ұ�����һ��
			if(scope==iter->get_scope() && str==iter->get_name())
			{
				//���һ�£��򷵻ص�ǰǩ��
				return iter->getSigIndex();
			}
			else
			{
			}
		}
		//���û���ҵ����������򻻳�"global"�ٲ���һ��
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//�жϵ�ǰ��������ָ����������Ƿ��Ҫ���ҵı�����һ�£���������Ϊ"global"
			if("global"==iter->get_scope() && str==iter->get_name())
			{
				//���һ�£��򷵻ص�ǰǩ��
				return iter->getSigIndex();
			}
			else
			{
			}
		}
		//������û�з��أ�˵��û���ҵ��ñ������׳�����
		string error_info="can not find varibale : "+str+"in var_info lis!";
		throw error_info;
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="get  current signature : "+s;
		throw error_info;
	}
}
// ���ܣ��ڶ�̬ǩ�����У����ݱ��������Ƽ��䵱ǰλ�ڵ������򣬲��Ҹñ����ı�ǩ��Ϣ
// ���������s-��̬ǩ����ָ��
// 		          var_name-��������
//                  var_scope-�ñ�����ǰ���ڵ�������
// ����ֵ���ɹ��򷵻ز��ҵ��ı�ǩ��Ϣ��ʧ�����׳�������Ϣ
// ���ߣ����
string get_var_stamp(list<Var_info>* s,string var_name,string var_scope)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"var_info list's pointer");
		check_str_arg(var_name,"variable's name");
		check_str_arg(var_scope,"variable's scope");
		
		list<Var_info>::iterator iter;//��̬ǩ���������
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//�жϵ�ǰ��������ָ��������ֺ��������Ƿ���Ҫ���ұ�����һ��
			if(var_scope==iter->get_scope() && var_name==iter->get_name())
			{
				//���һ�£��򷵻ص�ǰ����ı�ǩ��Ϣ
				return iter->get_stamp();
			}
			else
			{
			}
		}
		//���û���ҵ����������򻻳�"global"�ٲ���һ��
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//�жϵ�ǰ��������ָ����������Ƿ��Ҫ���ҵı�����һ�£���������Ϊ"global"
			if("global"==iter->get_scope() && var_name==iter->get_name())
			{
				//���һ�£��򷵻ص�ǰ����ı�ǩ��Ϣ
				return iter->get_stamp();
			}
			else
			{
			}
		}
		
		//������û�з��أ�˵��û���ҵ��ñ������׳�����
		string error_info="can not find varibale : "+var_name+"in var_info lis!";
		throw error_info;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="get variable's stamp : "+str;
		throw error_info;
	}
}

// ���ܣ������кţ����Ҵ��к�֮ǰ�Ƿ���ע�ͣ�����У����ӡע�ͣ����ұ�֤�������ȷ��λ�á�
// ���������cmnt_list-���������ע����Ϣ������ָ��
//                  line-��ǰ���к�
//			      out-�ļ������
//���������out-�ļ������
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out)
{
	try
	{
		//��������ע������ָ���Ƿ�Ϊ��
		check_pt_arg(cmnt_list,"comment list's pointer");
		
		//����ע������
		for(list<Cmnt>::iterator iter=cmnt_list->begin();iter!=cmnt_list->end();iter++)
		{
			//�жϵ�ǰ��������ָ�Ķ������ʼ�к��Ƿ�С�ڴ�����кţ��������־�Ƿ�Ϊfalse
			if(iter->get_line_begin()<=line && iter->get_output()==false)
			{
				//˵���ö���������ע����Ϣ��δ��ӡ������Ҫ��ӡ
				out_indent(indent_num, out);//�������
				out<<iter->output_cmnt(indent_num)<<endl;//���ע��
				
				//ע�ʹ�ӡ֮�󣬽��ö���������־��Ϊtrue��������ע���ѱ���ӡ
				iter->set_output(true);
			}
			else
			{
				//�������ж�����������ԣ���������
			}
		}
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print comment : "+s;
		throw error_info;
	}
}

// ���ܣ������̬ǩ�����е�ǩ�����ļ��У�ά��ʱʹ��
// ���������s-��̬ǩ����
//                  file_name-��Ҫ������ļ�
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void out_var_sigs(list<Var_info>* s, string file_name)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"var_info list's pointer");
		check_str_arg(file_name,"file's name");
		
		//��������ļ���
		ofstream out;
		out.open(file_name.c_str());
		
		//�ж��ļ��Ƿ񴴽��ɹ�
		if(!out.is_open())
		{
			//Ϊtrue��˵���ļ�����ʧ�ܣ�����
			string error_info="out put file creating failed!";
			throw error_info;
		}
		else
		{
			//Ϊfalse���ļ������ɹ�����ʼ������̬ǩ�����������ǩ��
			for(list<Var_info>::iterator iter=s->begin();iter!=s->end();++iter)
			{
				out<<iter->get_name()<<"'s signature history : ";
				unsigned int k=0;//���Ѿ������ǩ�����м���
				//����ǩ����
				for(vector<int>::iterator iter1=iter->get_sigs()->begin();iter1!=iter->get_sigs()->end();iter1++,k++)
				{
					//���ж��������ǩ�������Ƿ�>=10����Ϊ10�ı���
					if(k>=10 && k%10==0 )
					{
						//�����������س�����ʹ�任��
						out<<"\n";
					}
					out<<*iter1<<" ";
				}
				out<<endl;
			}
		}
		out.close();
	}
	catch( string s)
	{
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="print signatures : "+s;
		throw error_info;
	}
}

string BuildOutputFunc_str(string LeftVar,string FuncName,...)
{
	va_list paraList;
	va_start(paraList,FuncName);
	string BuildFuncName=string(FuncName)+"(";
	const char *CurPara=va_arg(paraList,const char*);
	while(CurPara!=NULL)
	{
		BuildFuncName=BuildFuncName+string(CurPara);
		CurPara=va_arg(paraList,const char*);
		if(CurPara!=NULL)
			BuildFuncName=BuildFuncName+", ";
	}
	BuildFuncName=BuildFuncName+")";
	va_end(paraList);
	string FuncCallStmt=LeftVar+" = "+BuildFuncName+";";
	return FuncCallStmt;
}

string BuildOperationExp_str(string& LeftVar,const string& Op,string& Mod,...)
{
	va_list paralist;
	va_start(paralist,Mod);
	int Op_i=0;
	const char *cur_para=va_arg(paralist,const char *);
	string opExp=cur_para;
	while(Op_i<Op.size())
	{
		opExp=opExp+" "+Op[Op_i++];
		cur_para=va_arg(paralist,const char *);
		opExp=opExp+" "+cur_para;
	}
	opExp=LeftVar+" = ("+opExp+") % "+Mod+";";
	return opExp;
}

string BuildSigIdAdjust_str(string LeftVar,int sig_id)
{
	string SigIDAdjStr;
	if(LeftVar!=ARRAY_EXTR_NAME)
	{
		LeftVar=TmpVarJudge(LeftVar)? LeftVar+VAR_CHK_ID_NAME:CODED_VAR_PREFIX+LeftVar+VAR_CHK_ID_NAME;
		SigIDAdjStr=BuildOutputFunc_str(LeftVar,SIG_REG_ITF,int2HexStr(sig_id).c_str(),NULL);
	}
	else
	{
		SigIDAdjStr=BuildOutputFunc_str("",FUNC_ARRAYEXT_SIG_UPDATE,int2HexStr(sig_id).c_str(),NULL);
		SigIDAdjStr=SigIDAdjStr.substr(3);
	}
	return SigIDAdjStr;
}

//�����޸�
void constant2Str(const int constant)
{
	string value=int2HexStr_data(constant);
	string bind_str="AC_BIND_CONST_NUM_"+value;
	string const_str="CONST_NUM_"+value;
//	if(constant<0)
//		value="NEG_"+int2str(-constant);
//	else
//		value=int2str(constant);
	out_indent(indent_num,gen_out);
	gen_out<<bind_str+".Data.F"<<" = "<<const_str+".Data.F"<<";"<<endl;

	out_indent(indent_num,gen_out);
	gen_out<<bind_str+".Data.C.C1"<<" = "<<const_str+".Data.C.C1 + g_GS_TL1;"<<endl;

	out_indent(indent_num,gen_out);
	gen_out<<bind_str+".Data.C.C2"<<" = "<<const_str+".Data.C.C2 + g_GS_TL2;"<<endl;
}

//�����޸�
void bool2Str(ofstream &out,bool inbool)
{
	string bind_str=CONST_BOOL_BIND_PREFIX;
	string const_str=CONST_BOOL_PREFIX;
	if(inbool=true)
	{
		bind_str+="TRUE";
		const_str+="TRUE";
	}
	else
	{
		bind_str+="FALSE";
		const_str+="FALSE";
	}
	out_indent(indent_num,out);
	out<<bind_str+".Data.F"<<" = "<<const_str+".Data.F"<<";"<<endl;

	out_indent(indent_num,out);
	out<<bind_str+".Data.C.C1"<<" = "<<const_str+".Data.C.C1 + g_GS_TL1;"<<endl;

	out_indent(indent_num,out);
	out<<bind_str+".Data.C.C2"<<" = "<<const_str+".Data.C.C2 + g_GS_TL2;"<<endl;
}

string constantBindedStr(const int constant){
	string valueStr=int2HexStr_data(constant);
	return valueStr;
}

int getConstantSig(int constant,int *sigIndex){
	map<int,int>::iterator it=constantMap.find(constant);
//	string headConstant=constant>0?"CONST_NUM_":"CONST_NUM__NEG_";
	string value=int2HexStr_data(constant);
	string headConstant="CONST_NUM_"+value;
 	if(it==constantMap.end())
	//û�ҵ�
 	{
 		int index=0;
 		constantMap[constant]=get_sig_index(&index);
 		if(sigIndex!=NULL)
 			*sigIndex=index;
 		add_new_vars(var_infos, headConstant+int2str(constant), constantMap[constant], "global", "constant",index);//��������stamp����constant
	 	return constantMap[constant];
	 }
	 else
	 {
		 if(sigIndex!=NULL)
		  	*sigIndex=get_current_sigIndex(var_infos,headConstant+int2str(constant),"global");
		 return constantMap[constant];
	 }
 }
int getConstantSig(int constant){
	map<int,int>::iterator it=constantMap.find(constant);
 	if(it==constantMap.end())
	//û�ҵ�
 	{
 		int sigIndex=-1;
 		constantMap[constant]=get_sig_index(&sigIndex);
// 		string headConstant=constant>0?"CONST_NUM_":"CONST_NUM__NEG_";
 		string value=int2HexStr_data(constant);
 		string headConstant="CONST_NUM_"+value;
 		add_new_vars(var_infos, headConstant+int2str(constant), constantMap[constant], "global", "constant",sigIndex);//��������stamp����constant
 		return constantMap[constant];
	 }
	 else
	 {
		 return constantMap[constant];
	 }
 }

//�����ļ�����
void Tranfile(string strFileCopeIn, string strFileCopeOut)
{
	ofstream OutFile(strFileCopeOut.c_str());
	ifstream InFile(strFileCopeIn.c_str());

	string buf;
	string strInput;
	if(InFile.is_open())
	{
		while(getline(InFile, strInput))
		{


			OutFile << strInput << endl;
		}
		InFile.close();
	}
	else
	{
		log_error("Copy Config File failed! Reason:open Src File failed.\n")
		exit(1);
	}
}

class IOArrayMap
{
private:
	IOArrayMap()
	{
		m_NameMap["g_USR_BufRecv"] = "g_ITF_BufMsgRecv";
		m_NameMap["g_USR_BufFetch"] = "g_ITF_BufMsgFetch";
		m_NameMap["g_USR_BufSend"] = "g_ITF_BufMsgSend";
		m_NameMap["g_USR_BufDeliver"] = "g_ITF_BufMsgDeliver";
	}
	std::map<string, string> m_NameMap;
	static IOArrayMap *m_pInstance;
public:
	static IOArrayMap* GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new IOArrayMap();
		}
		return m_pInstance;
	}

	std::map<string, string>& GetMap()
	{
		return m_NameMap;
	}
};
IOArrayMap *IOArrayMap::m_pInstance = NULL;

//��ȡ��������CSC�е�����
string GetArrayNameInCSC(string ArrayName)
{
	//�ж��Ƿ�IO����
	std::map<string, string>& refMap = IOArrayMap::GetInstance()->GetMap();

	std::map<string, string>::iterator it = refMap.begin();

	for(; it != refMap.end(); ++it)
	{
		string SubStr = ArrayName.substr(0, it->first.size());
		if(SubStr == it->first)
		{
			string StrInCSC = it->second + ArrayName.substr(it->first.size());
			return StrInCSC;
		}
	}

	//�����ǣ�ֱ�Ӽ�ǰ׺"TC_"
	return "TC_"+ArrayName;
}
