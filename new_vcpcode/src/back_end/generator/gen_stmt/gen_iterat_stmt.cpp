#include "CodedVarBuild_Util.h"
#include "gen_utils.h"
#include "statement.h"
#include "expression.h"
#include "var.h"
#include "compound_stmt.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "cmnt.h"
#include "TmpVarIndexRange.h"
#include "ControlFlow_CodeUtil.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
int S_Iterat_in,S_Iterat_out;//ѭ�����ǩ����ѭ������ǩ��

//�˽ṹ���ڱ�����빤����dD�����ֺͶ�Ӧ��ֵ
struct dD_value
{
	string name;//��¼dD������
	int value;//��¼dD��ֵ
};

//���ڼ�¼ѭ�����ڵ���Ҫ�����ı�����Ϣ���������ƣ��Ƿ�Ϊ��ֵ������ֵ
struct adj_var
{
	string name;//��¼��������
	string stamp;//��¼Ϊ��ֵ������ֵ
};

//��¼ѭ����ڱ���ʱ����Ҫ�õ��ı������ƣ������ŵȲ���
struct WhileNameIDEntr
{
	string CondVar;
	string BodySigName;
	CFSubVar SubVarName;
	int WhileABIndex;
};

//��¼ѭ�����ڱ���ʱ����Ҫ�õ��ı������ƣ������ŵȲ���
struct WhileNameOut
{
	int WhileABIndex;
	string WhileOutSig;
};


// �������ļ�: src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern bool debug_flag;
extern list<dD_value>* dD_values;
extern list<Cmnt>* cmnts;
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);

// �������ļ� : back_end_utils.c
extern int goto_label;

//�������ļ�: ./gen_stmt.cpp
extern void gen_stmtlist(list<Statement>* s,list<Var_info>* v);
extern void gen_stmt(Statement* s,list<Var_info>* v);

//�������ļ�: ./while_if_utils.cpp
extern list<adj_var>* adj_name_list;
extern void copy_Var_infos(list<Var_info>* src,list<Var_info>* des);
extern int compute_S1S2(int sig, int k, int* s1,string op,bool stamp);
extern int* compute_CFSig_S1S2(int SubSig[],int k,int *S1,bool stamp);
extern int check_condition_entrance_exp(Expression *s);
extern void get_var_names(Statement* s,list<adj_var>* l);
extern vector<string> var_check_adjust_stmt(string Var,int *Constants);

map<CFCondVar,int> g_WhileMap_CondVar_SumVar;
int WhileSigCnt;
int MappingCondVar(string VarName,CFSubVar &WhileSub);

// ���ܣ�Ϊ��ǰѭ����ѡdD����ǩ���ļ���ȡһ�����ʵ�ֵ��ΪdD����֤��ѭ������������������ʱ��
// 			�ڴ��в����������������ͬ����ǩ��
// ���������l-������ǩ���ı���������ָ��
// 			      v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻���ѡ����dDֵ��ʧ�����׳�������Ϣ
// ���ߣ�����
static int select_dD(list<adj_var>* l, list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_pt_arg(l,"variable name list's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		int dD=0;//�洢Ϊ��ѭ�������ѡ��dDֵ
		vector<int>* current_sigs=new vector<int>;//���ڱ��浱ǰ���б�����ǩ��
		//������̬ǩ�������湤�������б����ĵ�ǰǩ��
		for(list<Var_info>::iterator iter=v->begin();iter!=v->end();iter++)
		{
			current_sigs->push_back(iter->get_sigs()->back());
		}
		
		vector<int>* while_varsigs=new vector<int>; //���ڱ���ѭ�������б����ĳ�ʼǩ��
		//����ѭ�����е���Ҫ�����ı����ĵ�ǰǩ��
		for (list<adj_var>::iterator iter=l->begin();iter!=l->end();iter++)
		{
			while_varsigs->push_back(get_current_sig(v,iter->name,var_scope));
		}

		RE_ASSIGN:dD = get_sig();//��ǩ������ȡdD
		//����ѭ���������б�����ǩ������
		for(vector<int>::iterator iter=while_varsigs->begin();iter!=while_varsigs->end();iter++)
		{
			//ѭ��1000�Σ�ÿ�ζ�ǩ��+dD������û�к͹��������б����ĵ�ǰǩ���ظ�
			for(int i = 1; i!=1000; ++i)
			{
				int tmp_sig=((long long)(*iter)+(long long)i*dD%A)%A;//ǩ����dD
				//�ж��Ƿ����ظ�
				if(current_sigs->end()!=find(current_sigs->begin(),current_sigs->end(),tmp_sig))
				{
					//����У������·���
					goto RE_ASSIGN;
				}
				else
				{
					//���û���ظ������������dD�Ƚ�
				}
			}
		}
		return dD;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="select dD : "+str;
		throw error_info;
	}
}

// ���ܣ����ڶ�ѭ���ṹ�������ģ�飬�����������ʽ�����������ͣ������Ӧ��ѭ����ڵ�������룬
// 			�������ʽ��ֵ�ĵ�λ���������������Ӧ��WHILE_K�������
// ���������s-ѭ������������ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������S1-ѭ���������õ���S1ֵ�ĵ�ַ
//			      k-ѭ�����������ڼ���S1ֵ��Kֵ�ĵ�ַ
// ����ֵ���ɹ��򷵻ؼ��������S2ֵ��ʧ�����׳�������Ϣ
// ���ߣ�����
static int *gen_while_exp(Expression* s,list<Var_info>* v,int* S1,int *k,const WhileNameIDEntr &name_index)
{
	int Prime[2]={P1,P2};
	string PrimeStr[2]={"P1","P2"};
	string S1S2Str[2]={IF_WHILE_SIG_S1,IF_WHILE_SIG_S2};
	try
	{
		//��������������Ч��
		check_pt_arg(s,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		//�ж���ڱ��ʽ�����Ƿ�Ϸ�
		if(1)
		{
			//���ѭ��������by�ܺ�
			string cel_name=s->get_cel()->get_cvar()->get_name();//���ʽ��ֵ������
			out_indent(indent_num,gen_out);
			gen_out<<"Iterat_decode="<<"check_decode("<<cel_name<<"_1,"<<cel_name<<"_2);"<<endl;//У�����ѭ�����
			out_indent(indent_num,gen_out);
			gen_out<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<"while(Iterat_decode>=0)"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<"{"<<endl;
			indent_num++;

			return 0;
		}
		else
		{
			//�������������ʽ�Ƿ������׳�������Ϣ
			string error_info="the entrance expression is invalid! ";
			throw error_info;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding entrance expression :  "+str;
		throw error_info;
	}
	
}

// ���ܣ���ģ��Ϊѭ���ṹ�����������ģ�����ģ�顣��������ѭ�����������������ֵ������ǩ��
// 			�������룬Ϊ��ֵ�����������ǩ��
// ���������name-����������ֵ������
// 			     VarNameSet-��ر�������--WHile_A+While_B��ֵ  ѭ������ǩ��ֵ
// 			     S2-����ǩ��������S2ֵ
// 			     v_old-ѭ������Ķ�̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void
// ���ߣ�����
static void ex_left_adj(string name,const WhileNameOut &VarNameSet, int *S2,list<Var_info>* v_old)
{
	int constants[2]={0};
	int Prime[2]={P1,P2};
	string PrimeStr[2]={"P1","P2"};
	string ChkField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string WhileOutSigSet[2]={VarNameSet.WhileOutSig+IF_WHILE_SIG_S1,VarNameSet.WhileOutSig+IF_WHILE_SIG_S2};
	string WhileAStr,WhileBStr;
	string A_plus_B_str[2];
	try
	{
		//���������������������ַ�������Ч��
		check_str_arg(name,"variable's name");
		//����������̬ǩ����ָ�����Ч��
		check_pt_arg(v_old,"var_info list's pointer");
		
		int old_sig=get_current_sig(v_old,name,var_scope);//��ȡ��ǰ�����ľ�ǩ��
		int new_sig_id=0;
		int new_sig=get_sig_index(&new_sig_id);//������ǩ����Ϊ����ǩ��

		string T_CHECK_Str[2]={IF_WHILE_SIG_S1,IF_WHILE_SIG_S2};
		for(int i=0;i<2;i++)
		{
			//���㲹������
			constants[i]=(((long long)new_sig-old_sig-S2[i])%Prime[i]+Prime[i])%Prime[i];
			//���While_A+While_B ���ַ���
			WhileAStr=WHILE_AC_A_PRE+int2str(VarNameSet.WhileABIndex)+T_CHECK_Str[i];
			WhileBStr=WHILE_AC_B_PRE+int2str(VarNameSet.WhileABIndex)+T_CHECK_Str[i];
			A_plus_B_str[i]="("+WhileAStr+" + "+WhileBStr+")";
		}
		if(name!=ARRAY_EXTR_NAME)
		{
			for(int i=0;i<2;i++)
			{
				//���ǩ����������
				//TC_x.Data.C.C1=(TC_x.Data.C.C1-(WHile_A_1+While_B_1)+SWIout+C)%P
				string LeftVar=TmpVarJudge(name)? name+ChkField[i]:CODED_VAR_PREFIX+name+ChkField[i];
				string VarAdjustExpStr=BuildOperationExp_str(LeftVar,"-++",PrimeStr[i],LeftVar.c_str(),
						A_plus_B_str[i].c_str(),WhileOutSigSet[i].c_str(),int2HexStr(constants[i]).c_str());
				gen_out<<OutIndentStr(indent_num)+VarAdjustExpStr<<endl;
			}
		}
		else
		{
			string array_adjust_para[2]={"("+WhileOutSigSet[0]+" - "+A_plus_B_str[0]+" + "+int2HexStr(constants[0])+")",
										"("+WhileOutSigSet[1]+" - "+A_plus_B_str[1]+" + "+int2HexStr(constants[1])+")"};
			string array_adjust_func=BuildOutputFunc_str("",FUNC_ARRAYEXT_CHK_UPDATE,array_adjust_para[0].c_str(),
					array_adjust_para[1].c_str(),NULL);
			gen_out<<OutIndentStr(indent_num)+array_adjust_func.substr(3)<<endl;
		}

		//���ǩ��Id�������
		string SigIDAdjStr=BuildSigIdAdjust_str(name,new_sig_id);
		gen_out<<OutIndentStr(indent_num)+SigIDAdjStr<<endl;

		//����ǵ���ģʽ�������������Ϣ��ѭ�����ڵ���ǩ��
		if(true==debug_flag)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"//"<<name<<"'s new sig :"<<new_sig<<endl;
		}
		//����ǩ�����붯̬ǩ������
		add_new_sig(v_old,name,new_sig,var_scope);
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="adjust  left variable��s export signatures : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ��Ϊѭ���ṹ�����������ģ�����ģ�顣��������ѭ�����������������ֵ������ǩ��
// 			��������
// ���������name-����������ֵ������
// 			      VarNameSet-��ر���������
// ����ֵ���ɹ��򷵻�void
// ���ߣ�����
static void ex_right_adj(string name,const WhileNameOut &VarNameSet)
{
	string ChkField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string PrimeStr[2]={"P1","P2"};
	try
	{
		//���������������������ַ�������Ч��
		check_str_arg(name,"variable's name");

		string T_CHECK_Str[2]={IF_WHILE_SIG_S1,IF_WHILE_SIG_S2};
		string A_plus_B[2];
		for(int i=0;i<2;i++)
		{
			string WhileAStr=WHILE_AC_A_PRE+int2str(VarNameSet.WhileABIndex)+T_CHECK_Str[i];
			string WhileBStr=WHILE_AC_B_PRE+int2str(VarNameSet.WhileABIndex)+T_CHECK_Str[i];
			string A_plus_B_str="("+WhileAStr+" + "+WhileBStr+")";
			A_plus_B[i]=A_plus_B_str;
		}
		if(name!=ARRAY_EXTR_NAME)
		{
			for(int i=0;i<2;i++)
			{
				//�������ʱ���ǩ��������
				//TC_a.Data.C.C1=(TC_a.Data.C.C1-(While_A+While_B))%P
				string LeftVar=TmpVarJudge(name)? name+ChkField[i]:CODED_VAR_PREFIX+name+ChkField[i];
				string VarAdjustFuncStr=BuildOperationExp_str(LeftVar,"-",PrimeStr[i],LeftVar.c_str(),A_plus_B[i].c_str());
				out_indent(indent_num,gen_out);
				gen_out<<VarAdjustFuncStr<<endl;
			}
		}
		else
		{
			string array_adjust_func=BuildOutputFunc_str("",FUNC_ARRAYEXT_CHK_UPDATE,("(-"+A_plus_B[0]+")").c_str(),
								("(-"+A_plus_B[1]+")").c_str(),NULL);
			gen_out<<OutIndentStr(indent_num)+array_adjust_func.substr(3)<<endl;
		}

	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="adjust  right variable��s export signatures : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ��Ϊѭ������������������ģ�顣��ɶ�ѭ��������ڱ��ʽ���б��룬��������е�S1,S2ֵ
// 			����ѡdD����ѭ���е������б��룬ѭ�����ڵ�ǩ������������
// ���������s-ѭ��������ָ��
// ���������v_out-ѭ������Ķ�̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�����   �޸�by�ܺ�
void gen_iterat_stmt(Iteration_stmt* s,list<Var_info>* v_out)
{
	string TLStr[2]={TL1_NAME,TL2_NAME};
	string PrimeStr[2]={"P1","P2"};
	string CheckFieldStr[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string iterator_adj_var;//������������

	int Prime[2]={P1,P2};
	int constant[2]={0,0};
	vector<string> adj_stmt;
	try
	{
		//��������������Ч��
		check_pt_arg(s,"iteration statement object's pointer");
		check_pt_arg(v_out,"var_infos list's pointer");
		
		string WhileBodySigName=BuildTmpVarName(WHILE_BODYSIG_PREFIX,"",++WhileSigCnt);
		string WhileOutSigName=BuildTmpVarName(WHILE_OUTSIG_PREFIX,"",WhileSigCnt);
		//��������ѭ������Ҫ�õ������ƺ�������

		string CelVarName=s->get_ce()->get_cel()->get_cvar()->get_name();

		CFSubVar CurWhileSubVar;
		int WhileABIndex=MappingCondVar(CelVarName,CurWhileSubVar);
		WhileNameIDEntr CurWhileEntrNameId={CelVarName,WhileBodySigName,CurWhileSubVar,WhileABIndex};


		list<adj_var>* temp_adj_names=new list<adj_var>;//�洢����ѭ���ṹ����Ҫ��������������
		
		//ȡ��while����е���ֵ��������ֵ����
		get_var_names(s->get_cs(),temp_adj_names);

		//��ӱ���ѭ����AC_Sub_�������������������
		

		list<Var_info>* v_in=new list<Var_info>;//���ڼ�¼ѭ�����ڱ���ʱǩ���仯����Ķ�̬ǩ����
		copy_Var_infos(v_out,v_in);//�Ƚ��ⲿǩ��������ѭ���ڵ�ǩ����
		
		string dD_name=DELTA_T_NAME;//dD����������
		
		//S1 S2 ��Ϊѭ����ǩ�� �����˷����� K
		int *S1=new int[2];
		int K=0;
		string cel_name=s->get_ce()->get_cel()->get_cvar()->get_name();
		int *S2=gen_while_exp(s->get_ce(),v_out,S1,&K,CurWhileEntrNameId);//��ѭ����ڱ��ʽ���б���
		S_Iterat_in=*iter;//��ȡ���ǩ��by�ܺ�

		//�ж�ѭ�����е�����Ƿ�Ϊ�������
		if(0==s->get_cs()->get_stamp())
		{	


			//����ǣ���ֱ�ӶԸ�������е����������б���
			gen_stmtlist(s->get_cs()->get_ccstmt()->get_csl(),v_in);
			//2010��3��10��׷��
			//���Դ���и���������ǰ��ע��
			out_cmnt(cmnts,s->get_cs()->get_ccstmt()->get_fake_line(),gen_out);
		}
		else
		{
			//������ǣ���ֱ�ӶԸ������б���
			
			//2010��3��10��׷��
			fake_line_num=s->get_cs()->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//������֮ǰ��ע����Ϣ
				
			gen_stmt(s->get_cs(),v_in);
		}
		
		S_Iterat_out=*iter;//��ȡ����ǩ��
		//��������ѭ��������ǩ��by�ܺ�
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS-"<<"("<<S_Iterat_out<<")+("<<S_Iterat_in<<"&(~Iterat_mark))^("<<S_Iterat_out<<"&Iterat_mark);"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		//���㵥��ѭ���������λ��ʶ����ѭ����ڽ���ֵby�ܺ�
		out_indent(indent_num,gen_out);
		gen_out<<"Iterat_decode="<<"check_decode("<<cel_name<<"_1,"<<cel_name<<"_2);"<<endl;//У�����ѭ�����
		out_indent(indent_num,gen_out);
		gen_out<<"Iterat_mark="<<"(long long) Iterat_decode>>32;"<<endl;//����ѭ���ṹ��λ���
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		indent_num--;
		out_indent(indent_num,gen_out);
		gen_out<<"}"<<endl;
		//��������ѭ��������ǩ��by�ܺ�

		gen_out<<"RS=RS-"<<"("<<S_Iterat_in<<")+("<<S_Iterat_in<<"&(~Iterat_mark))^("<<S_Iterat_out<<"&Iterat_mark);"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<endl;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding iteration statement : "+str;
		throw error_info;
	}
}
//����ѭ��������������ӳ����������
//1-WHile_A/B��������
//2-While_SUb������
int MappingCondVar(string VarName,CFSubVar &WhileSub)
{
	CFCondVar CurWhileCndVar;

	CurWhileCndVar.CondVar.VarName=VarName;
	CurWhileCndVar.CondVar.FuncName=var_scope;
	map<CFCondVar,CFSubVar>::iterator Map_iter=g_CFMap_CondVar_SubVar.find(CurWhileCndVar);
	/*if(Map_iter==g_CFMap_CondVar_SubVar.end())
	{
		string error_map_not_find="cannot find subvar in func"+CurWhileCndVar.CondVar.FuncName+"\n"
				+"and condVar is "+CurWhileCndVar.CondVar.VarName+"\n";
		throw error_map_not_find;
	}
	else
	{
		WhileSub=Map_iter->second;
	}*/

	int Index=0;
	map<CFCondVar,int>::iterator Map_VarInt_iter=g_WhileMap_CondVar_SumVar.find(CurWhileCndVar);
	/*if(Map_iter==g_CFMap_CondVar_SubVar.end())
	{
		string error_map_not_find="cannot find While_A/b Index in func"+CurWhileCndVar.CondVar.FuncName+"\n"
				+"and condVar is "+CurWhileCndVar.CondVar.VarName+"\n";
		throw error_map_not_find;
	}
	else
	{
		Index=Map_VarInt_iter->second;
	}*/
	return Index;
}
