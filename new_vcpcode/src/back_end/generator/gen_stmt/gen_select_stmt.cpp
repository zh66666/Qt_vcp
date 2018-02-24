#include "CodedVarBuild_Util.h"
#include "gen_utils.h"
#include "statement.h"
#include "expression.h"
#include "var.h"
#include "compound_stmt.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "cmnt.h"
#include "ControlFlow_CodeUtil.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_secondary.h"
#include "gen_global_var.h"
#include <map>

#define STR_BOOL_F  "2129587950"
#define TESTING_GEN_SELECT 0
#define TESTING_IF_C 0
#define TESTING_IF_LEFT_ADJ 0
#define TESTING_INNER_LEFT 0
#define TESTING_EX_ADJ 0
int S_if,S_else;//if��else��֧�����һ���������ǩ��
int S_out;//��֧����ǩ��

//���ڼ�¼ѭ�����ڵ���Ҫ�����ı�����Ϣ���������ƣ��Ƿ�Ϊ��ֵ������ֵ
struct adj_var
{
	string name;//��¼��������
	string stamp;//��¼Ϊ��ֵ������ֵ�����Ϊ��ֵ����stampΪ"left"������Ϊ"right"
};

// �������ļ�: src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern bool debug_flag;
extern list<Cmnt>* cmnts;
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);

// �������ļ� : ../gen_pro.cpp
extern void gen_declist(list<Declaration>*);

//�������ļ� : ./gen_stmt.cpp
extern void gen_stmt(Statement* s,list<Var_info>* v);
extern void gen_stmtlist(list<Statement>* s,list<Var_info>* v);

//�������ļ� : ./while_if_utils.cpp
extern void copy_Var_infos(list<Var_info>* src,list<Var_info>* des);
extern int compute_S1S2(int sig, int k, int* s1,string op,bool stamp);
extern void  add_adj_name(list<adj_var>* l, struct adj_var a);
extern void get_var_names(Expression* s,list<adj_var>*  l);
extern int* compute_CFSig_S1S2(int SubSig[],int k,int *S1,bool stamp);
extern int check_condition_entrance_exp(Expression *s);
extern vector<string> var_check_adjust_stmt(string LeftVar,int *Constants);


map<CFCondVar,CFSubVar> g_CFMap_CondVar_SubVar;
int SelectOutSigCnt;
void gen_compd_stmt_inSelectBranch(Compound_stmt* s,list<Var_info>* v,int __AddIndex);
void if_branch_left_adjust(list<string> **adj_names,list<Var_info> *sig_if,list<Var_info> *sig_else,int __AddIndex)  throw(string&);
void merge_if_else_branch_adjvar(list<string> **branch_names,list<string> *merge_names);

// ���ܣ�������list���бȽϣ����ȳ�����list1���ֳ�����list2�е�Ԫ�أ���list1��ɾ����
// ���������list1-����1��ָ��
// 			      list2-����2��ָ��
// ���������list1-����1��ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�������
static void del_same_names(list<string>* list1, list<string>* list2)
{
	try
	{
		//��������������Ч��
		check_pt_arg(list1,"list1's pointer");
		check_pt_arg(list2,"list2's pointer");
		
		//��������list1
		for(list<string>::iterator iter=list1->begin();iter!=list1->end();)
		{
			//������list2�в��ҵ�ǰ��������ָ���ַ����������Ƿ����
			if(list2->end()!=find(list2->begin(),list2->end(),*iter))
			{
				//������ڣ����ַ���ɾ��
				iter=list1->erase(iter);
			}
			else
			{
				//��������ڣ����������
				iter++;
			}
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="delete same names : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ�������֧�ڲ���ֵ������ֵ����������������Ϊ��ֵ�洢���������������һ����ʱ����
//			�򲻴洢
// ���������s-��������ָ��
// ���������l-����������������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ�������
static void get_var_names(Var* s,list<adj_var>* l)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"variable object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		
		string name="";//�洢Ҫ��ŵı�������
		//�жϸñ����Ƿ�Ϊ�������
		if(1==s->get_stamp())
		{
			//����������������±���ʽ
			get_var_names(s->get_cexp(),l);
		}
		else
		{
			//������ǣ����������ΪҪ�洢������
			name=s->get_name();
			struct adj_var a={name,"left"};
			add_adj_name(l,a);
		}
	}
	catch (string str)
	{
		string error_info="get variable names in variable object : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ�������֧�ڲ�����䣬��������ֵ��������ֵ������������洢������ͬʱ������Ҫ��
//			���ı���������Ҳ�洢����
// ���������s-������ָ��
// ���������l-����������������ָ��
//			      n-����Ҫ�����ı���������ָ��
// ����ֵ���ɹ��򷵻�void����ʧ�����׳�������Ϣ
// ���ߣ�������
static void get_var_names(Statement* s,list<adj_var>* l,list<string>* n)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"statement object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		
		//�ж���������
		switch(s->get_stamp())
		{
			case 0:
			{
				//���Ϊ������䣬�������������е��������
				for(list<Statement>::iterator iter=s->get_ccstmt()->get_csl()->begin();
					   iter!=s->get_ccstmt()->get_csl()->end();iter++)
				{
					get_var_names(&(*iter),l,n);//����ÿ�����
				}
			}
			break;	
			case 1:
			{
				//���Ϊ��֧��䣬���ȱ���if��֧��ڴ��ı��ʽ���������
				//ȡ�������������
//				string l_name=s->get_csstmt()->get_ce()->get_cel()->get_cvar()->get_name();
//				if("tmpArray"!=l_name.substr(0,8))
//				{
//					//������ǣ��򽫴˱�������Ϊ��ֵ�洢����
//					struct adj_var a;
//					a.name=l_name;
//					a.stamp="right";
//					add_adj_name(l,a);
//				}
				
				//����if��֧�е����
				get_var_names(s->get_csstmt()->get_cs_if(),l,n);

				//�ж��Ƿ���else��֧
				if(1==s->get_csstmt()->get_stamp())
				{
					//�������else��֧�������else��֧�е����
					get_var_names(s->get_csstmt()->get_cs_else(),l,n);
				}
				else
				{
					//�����else��֧�������
				}
			}
			break;	
			case 2:
			{
				//���Ϊѭ����䣬���ȱ���ѭ������������ʽ���������
				//get_var_names(s->get_cistmt()->get_ce()->get_cel()->get_cvar(),l);
				
				//����ѭ�����е����
				get_var_names(s->get_cistmt()->get_cs(),l,n);
			}
			break;	
			case 4:
			{
				//��ֵ��䣬���ȷ�����ֵ����  ����˳��--���Һ���
				string left_name;
				//�ж���ֵ�Ƿ�������
				//��������
				if(s->get_cvar()->get_stamp()==0)
				{
					//���Ϊ��ֵ��䣬���жϸ�����Ƿ�Ϊ������ֵ���
					if(s->get_ce()->get_stamp()==5)
					{
						//���������������ʽ�б�ÿ����������Ϊ��ֵ�洢����
//						for(list<Expression>::iterator  iter=s->get_ce()->get_args()->begin();
//							   iter!=s->get_ce()->get_args()->end();iter++)
//						{
//							get_var_names(&(*iter),l);
//						}
						//���������е�ȫ�ֱ�����Ϊ��ֵ����
						vector<string> global_var_list=get_global_var_infunc(var_scope);
						for(vector<string>::iterator it=global_var_list.begin();it!=global_var_list.end();it++)
						{
							struct adj_var adj_global_var={*it,"left"};
							add_adj_name(l,adj_global_var);
						}
					}
					else
					{
						//�����Ϊ�������ã�������ø�ֵ����ұߵı��ʽ
						//get_var_names(s->get_ce(),l);
					}

					left_name=s->get_cvar()->get_name();//�洢��ֵ������
					if(ExtraVarJudge(left_name))
					{
						//��ʱ��������Ҫ����ǩ��
//						if(n->end()==find(n->begin(),n->end(),left_name))
//						{
//							n->push_back(left_name);
//						}
					}
					else
					{
						//������ǣ��������ֵ����
						 get_var_names(s->get_cvar(),l);
					}
				}
				//���鸳ֵ
				else
				{
					//���±�
					//get_var_names(s->get_cvar(),l);
					//��ֵ
					//get_var_names(s->get_ce(),l);
					//���ӱ���
					struct adj_var array_ext={ARRAY_EXTR_NAME,"left"};
					add_adj_name(l,array_ext);
				}

			}
			break;
			
			default:
			{
				//������䲻����������
			}
		}
	}
	catch (string str)
	{
		string error_info="get variable names in statement : "+str;
		throw error_info;
	}
}

// ���ܣ������֧��������󣬳��ڴ���ǩ����������
// ���������adj_var-��Ҫ�����ı���������ָ��
// 			      v_in-��֧�ڲ��Ķ�̬ǩ����
// 			      v_out-��֧��Ҫ��������Ķ�̬ǩ����
// 			      depth-��֧��ǰ�Ĳ���
// 			      s1-��֧����ʱ�õ���S1ֵ
// ����ֵ���ɹ��򷵻�void
// ���ߣ�������
static void ex_sig_adj(list<string>* adj_var,list<Var_info>* v_in,list<Var_info>* v_out,string IfOutSigName, int *s1)
{
	int constant[2]={0,0};
	int Prime[2]={P1,P2};
	string AdjVarChkFieldStr[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string IfOutSigSet[2]={IfOutSigName+IF_WHILE_SIG_S1,IfOutSigName+IF_WHILE_SIG_S2};
	string PrimeStr[2]={"P1","P2"};
	try
	{
		//��������������Ч��
		check_pt_arg(adj_var,"varibale name  list's pointer");
		check_pt_arg(v_in,"inner var_info  list's pointer");
		check_pt_arg(v_out,"export var_info  list's pointer");
		
		string left_adj_var;
		//������Ҫ�����ı���������
		for(list<string>::iterator iter=adj_var->begin();iter!=adj_var->end();iter++)
		{
			if(is_global_var(*iter))
			{
				left_adj_var=get_bind_local_var(*iter,var_scope);
			}
			else
			{
				left_adj_var=*iter;
			}
			int out_sig_id=0;//ǩ��Id
			int out_sig=get_sig_index(&out_sig_id);//����һ������ǩ��
			int in_sig=get_current_sig(v_in,left_adj_var,var_scope);//��¼��ǰǩ��
#if TESTING_EX_ADJ
			static int test_ex_adj_index=0;
			static int test_S_new[8]={P1-1,P2-1,1,1,1,1,P1-1,P2-1};
			static int test_S_if[8]={1,1,P1-1,P2-1,P1-1,P2-1,1,1};
			static int test_S1[8][2]={{P1-1,P2-1},{P1-1,P2-1},{P1-1,P2-1},{P1-1,P2-1},{1,1},{1,1},{1,1},{1,1}};

			out_sig=test_S_new[test_ex_adj_index];
			in_sig=test_S_if[test_ex_adj_index];
			s1[0]=test_S1[test_ex_adj_index][0];
			s1[1]=test_S1[test_ex_adj_index][1];

			test_ex_adj_index++;
#endif
			//�����λ����ʱ�Ĳ�������
			for(int i=0;i<2;i++)
				constant[i] = (((long long)out_sig-(long long)in_sig-(long long)s1[i])%Prime[i]+Prime[i])%Prime[i];
#if TESTING_EX_ADJ
			cout<<"constant0 is "<<constant[0]<<endl;
			cout<<"constant1 is "<<constant[1]<<endl;
#endif

			//�����λ��������
			if(*iter!=ARRAY_EXTR_NAME)
			{
				for(int i=0;i<2;i++)
				{
					string AdjVarName=TmpVarJudge(left_adj_var)? left_adj_var+AdjVarChkFieldStr[i]:"TC_"+left_adj_var+AdjVarChkFieldStr[i];
					string adj_opexp=BuildOperationExp_str(AdjVarName,"++",PrimeStr[i],AdjVarName.c_str(),
							IfOutSigSet[i].c_str(),string(int2HexStr(constant[i])).c_str());
					adj_opexp=OutIndentStr(indent_num)+adj_opexp;
					gen_out<<adj_opexp<<endl;
				}
			}
			else
			{
				string adj_para[2]={"("+IfOutSigSet[0]+" + "+int2HexStr(constant[0])+")","("+IfOutSigSet[1]+" + "+int2HexStr(constant[1])+")"};
				string array_adj=BuildOutputFunc_str("",FUNC_ARRAYEXT_CHK_UPDATE,adj_para[0].c_str(),adj_para[1].c_str(),NULL);
				array_adj=OutIndentStr(indent_num)+array_adj.substr(3);
				gen_out<<array_adj<<endl;
			}

			//���ǩ��Id�������
			string SigIDAdjStr=BuildSigIdAdjust_str(left_adj_var,out_sig_id);
			gen_out<<OutIndentStr(indent_num)+SigIDAdjStr<<endl;

			//����ǵ���ģʽ�������������Ϣ����֧���ڵ���ǩ��
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<(*iter)<<"'s new sig :"<<out_sig<<endl;
			}

			 //����ǩ������Ҫ��������Ķ�̬ǩ����
			add_new_sig(v_out,left_adj_var,out_sig,var_scope);
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="adjust variable's export signatures : "+str;
		throw error_info;
	}
}

// ���ܣ����ڶԷ�֧�ṹ�������ģ�飬�����������ʽ�����������ͣ������Ӧ�ķ�֧��ڵ�������룬
// 			�������ʽ��ֵ�ĵ�λ�������룬SI�ļ�������
// ���������s-��֧����������ʽ����ָ��
// 			      v-��̬ǩ����ָ��
//          Select_OutSigName-��֧����ǩ������
// ���������v-��̬ǩ����ָ��
// 			      S1-��֧���õ���S1ֵ�ĵ�ַ
// ����ֵ���ɹ��򷵻ؼ��������Cֵ��ʧ�����׳�������Ϣ
// ���ߣ�������
static int *gen_if_exp(Expression* s,list<Var_info>* v,int* S1,string& Select_OutSigName)
{
	CFCondVar CurIfCndVar;
	CFSubVar CurIfSubVar;
	int Prime[2]={P1,P2};
	try
	{
		//�����������ʽ����ָ�����Ч��
		check_pt_arg(s,"expression object's pointer");
		//����������̬ǩ����ָ�����Ч��
		check_pt_arg(v,"var_info  list's pointer");
		//�ж���ڱ��ʽ�����Ƿ�Ϸ�
		//if(check_condition_entrance_exp(s))
		{
			//����Ϸ�����������ʽ���������
			//����������жϱ�����صļ�����ֵ������
			string cel_name=s->get_cel()->get_cvar()->get_name();//���ʽ��ֵ������
			CurIfCndVar.CondVar.VarName=cel_name;
			CurIfCndVar.CondVar.FuncName=var_scope;

			map<CFCondVar,CFSubVar>::iterator Map_iter=g_CFMap_CondVar_SubVar.find(CurIfCndVar);
//			if(Map_iter==g_CFMap_CondVar_SubVar.end())
//			{
//				string error_map_not_find="cannot find subvar in func"+CurIfCndVar.CondVar.FuncName+"\n"
//						+"and condVar is "+CurIfCndVar.CondVar.VarName+"\n";
//				throw error_map_not_find;
//			}
//			else
//			{
//				CurIfSubVar=Map_iter->second;
//			}
//
//			//AC_IfSub_1��AC_IfSub_2��ǩ��
//			int BT[2]={0,0};
//			for(int i=0;i<2;i++)
//				BT[i]=get_current_sig(v,CurIfSubVar.SubVar[i].VarName,var_scope);
//
//			int k=get_sig_index(NULL);//����һ��ǩ�������ڼ���Sֵ�����㷨�������k

//			string ce_op=s->get_op();//��ȡ���ʽ������

			//����S1��S2��ֵ ��֧ǩ��S1-True S2-False
//			int *S2=compute_CFSig_S1S2(BT,k,S1,true);
#if TESTING_IF_C
			static int test_index=0;
			static int test_dataS1[6][2]={{0,0},{P1-1,P2-1},{0,0},{P1-1,P2-1},{P1-2,P2-3},{P1-100,P2-120}};
			static int test_dataS2[6][2]={{P1-1,P2-1},{0,0},{0,0},{P1-1,P2-1},{P1-100,P2-120},{P1-4,P2-6}};
			
			S1[0]=test_dataS1[test_index][0];
			S1[1]=test_dataS1[test_index][1];

			S2[0]=test_dataS2[test_index][0];
			S2[1]=test_dataS2[test_index][1];

			test_index++;
#endif
//			int *C=new int[2];
//			for(int i=0;i<2;i++)
//			{
//				C[i]=((long long)(S1[i])-(long long)(S2[i]) + Prime[i] ) % Prime[i];//����C��ֵ
//			}
//
//			//�����֧����ǩ������ʽ
//			Select_OutSigName=BuildTmpVarName(IF_SELECT_OUTSIG_PREFIX,"",++SelectOutSigCnt);
//			string OutSigFuncCall=BuildOutputFunc_str(Select_OutSigName,FUNC_OUTSIG,
//					string(CurIfSubVar.SubVar[0].VarName+".Data").c_str(),
//					string(CurIfSubVar.SubVar[1].VarName+".Data").c_str(),
//					string(int2HexStr(k)).c_str(),NULL);
//			out_indent(indent_num,gen_out);
//			gen_out<<OutSigFuncCall<<endl;
//
//			//����ǵ���ģʽ�������������Ϣ����ɢ���ӵ�ֵ��S1,S2��ֵ
//			if(true==debug_flag)
//			{
//				out_indent(indent_num,gen_out);
//				gen_out<<"// K:"<<k<<" S1[0]:"<<S1[0]<<" S1[1]:"<<S1[1]<<endl;
//				out_indent(indent_num,gen_out);
//				gen_out<<"// K:"<<k<<" S2[0]:"<<S2[0]<<" S2[1]:"<<S2[1]<<endl;
//			}

			//�����֧��ڸ�λ����by�ܺ�
			out_indent(indent_num,gen_out);
			gen_out<<"Select_decode="<<"check_decode("<<cel_name<<"_1,"<<cel_name<<"_2);"<<endl;//У������֧���
			out_indent(indent_num,gen_out);
			gen_out<<"Select_mark="<<"(long long) Select_decode>>32;"<<endl;//�����֧�ṹ��λ���
			out_indent(indent_num,gen_out);
			gen_out<<endl;
			//�����֧������by�ܺ�
			out_indent(indent_num,gen_out);
			gen_out<<"if(Select_decode>=0)"<<endl;


//			delete []S2;
//#if TESTING_IF_C
//			cout<<"C[0]:"<<C[0]<<endl;
//			cout<<"C[1]:"<<C[1]<<endl;
//#endif
//
//			return C;
		}
//		else
//		{
//			//�������������ʽ�Ƿ������׳�������Ϣ
//			string error_info="the entrance expression  is invalid! ";
//			throw error_info;
//		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding entrance expression : "+str;
		throw error_info;
	}
}

// ���ܣ��ڶ�if�ṹ���б���ʱ������ṹ�ڵĵ�λ��������
// ���������names-����Ҫ�����ı������ֵ�����ָ��
// 			      v1-��̬ǩ����1ָ��
// 			      v2-��̬ǩ����2ָ��
// 			      c-�������õ��ĳ���
// ����ֵ���ɹ��򷵻�void
// ���ߣ�������
static void inner_left_adjust(list<string>* names,list<Var_info>* v1,list<Var_info>* v2,int *c)
{
	int constant[2]={0,0};
	int Prime[2]={P1,P2};
	string CheckField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string PrimeStr[2]={"P1","P2"};
	try
	{
		//��������������Ч��
		check_pt_arg(names,"variable name list's pointer");
		check_pt_arg(v1,"var_info  list1's pointer");
		check_pt_arg(v2,"var_info  list2's pointer");
		
		string left_adj_var;

		//��������Ҫ������λ�ı������ֵ�����
		for(list<string>::iterator iter=names->begin();iter!=names->end();iter++)
		{
			if(is_global_var(*iter))
				left_adj_var=get_bind_local_var(*iter,var_scope);
			else
				left_adj_var=*iter;
			int sig1=get_current_sig(v1,left_adj_var,var_scope);//�����ڶ�̬ǩ����1�е�ǩ��
			int sig2=get_current_sig(v2,left_adj_var,var_scope);//�����ڶ�̬ǩ����2�е�ǩ��

#if TESTING_INNER_LEFT
			static int test_inner_index=0;
			static int test_S_if[8]={P1-1,P2-1,1,1,1,1,P1-1,P2-1};
			static int test_S_else[8]={1,1,P1-1,P2-1,P1-1,P2-1,1,1};
			static int test_S_C[8][2]={{P1-1,P2-1},{P1-1,P2-1},{P1-1,P2-1},{P1-1,P2-1},{0,0},{0,0},{0,0},{0,0}};

			sig1=test_S_if[test_inner_index];
			sig2=test_S_else[test_inner_index];
			c[0]=test_S_C[test_inner_index][0];
			c[1]=test_S_C[test_inner_index][1];

			test_inner_index++;
#endif
			//��������õ��Ĳ�������
			for(int i=0;i<2;i++)
				constant[i]=(((long long)sig1-(long long)sig2+(long long)c[i])%Prime[i]+Prime[i])%Prime[i];

#if TESTING_INNER_LEFT
			cout<<"constant0 is "<<constant[0]<<endl;
			cout<<"constant1 is "<<constant[1]<<endl;
#endif
			//�����������
			vector<string> adj_stmt=var_check_adjust_stmt(left_adj_var,constant);
			for(int i=0;i<adj_stmt.size();i++)
				gen_out<<adj_stmt[i]<<endl;

		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="adjust left variable's inner signatures : "+str;
		throw error_info;
	}
}

// ���ܣ���֧�������������ɵ���ģ�顣�Է�֧�ṹ���б��룬������֧��ڱ��ʽ����֧�е����ı��룬
// 			��else��֧�ͷ�֧�ṹ���ڰ����㷨����ǩ������
// ���������s-��֧������ָ��
// ���������v_out-������֧�ṹ֮��Ķ�̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ������� �޸�by�ܺ�
void gen_select_stmt(Selection_stmt* s,list<Var_info>* v_out)
{
	try
	{
		//��������������Ч��
#if TESTING_GEN_SELECT
		s->get_ce()->set_cer(new Expression("FALSE"));
		s=NULL;
		v_out=NULL;
#endif

		check_pt_arg(s,"selection statement object's pointer");
		check_pt_arg(v_out,"var_info  list's pointer");
		
		int AddIfStmtIndex=SecondAddIndex++;//�趨��if����Ķ��δ�������������

		string Select_OutSig;//���߼���ķ�֧����ǩ������

		select_level++;//��֧������1
		int *S_1=new int[2];//�����֧����ʱ�õ���S1ֵ
		int *C=gen_if_exp(s->get_ce(),v_out,S_1,Select_OutSig);//�Է�֧��ڱ��ʽ���б��룬����¼Cֵ

		
		list<Var_info>* v_if=new list<Var_info>;//����if��֧�����ǩ���仯����Ķ�̬ǩ����
		list<Var_info>* v_else=new list<Var_info>;//����else��֧�����ǩ���仯����Ķ�̬ǩ����
		copy_Var_infos(v_out,v_if);//����ǰ��ǩ������һ�ݸ�if��֧�еĶ�̬ǩ����
		copy_Var_infos(v_out,v_else);//����ǰ��ǩ������һ�ݸ�else��֧�еĶ�̬ǩ����
		
		//�洢��֧�ڲ�����ֵ��������ֵ������
		//0-if ��֧
		//1-else ��֧
		list<adj_var>* temp_adj_var[2]={new list<adj_var>,new list<adj_var>};
		list<string>* non_adj_names=new list<string>;//�洢��֧�ڲ��Ĳ���Ҫ������������ʱ������
	
		//����if��֧�е���䣬��ȡ��Ҫ�����Ͳ���Ҫ�����ı���������
		get_var_names(s->get_cs_if(),temp_adj_var[0],non_adj_names);
		//�ж��Ƿ���else��֧
		if(1==s->get_stamp())
		{	
			//���Ϊif-else�ṹ�����ȡelse��֧�еı�����
			get_var_names(s->get_cs_else(),temp_adj_var[1],non_adj_names);
		}
		else
		{
			//���û��else��֧�������
		}
		
		//�洢��Ҫ���е����ı�����
		//0-if branch
		//1-else branch
		list<string>* adj_names[2]={new list<string>,new list<string>};
		//����������ֵ����������ֵ����������������ֵ������ȡ��
		for(int i=0;i<2;i++)
			for(list<adj_var>::iterator iter=temp_adj_var[i]->begin();iter!=temp_adj_var[i]->end();iter++)
			{
				if(iter->stamp=="left")
				{
					adj_names[i]->push_back(iter->name);
				}
			}
		//ȥ������Ҫ������������ʱ������
		for(int i=0;i<2;i++)
			del_same_names(adj_names[i], non_adj_names);

		/*****��ʼ��֧����ڵ�������*****/
		//�洢��֧���ǩ��by�ܺ�
		int S_selectin=S_last;

		//�ж�if��֧�е�����Ƿ�Ϊ�������
		if(0==s->get_cs_if()->get_stamp())
		{
			//����ǣ���Ըø��������б���
			gen_compd_stmt_inSelectBranch(s->get_cs_if()->get_ccstmt(),v_if,AddIfStmtIndex);
		}
		else
		{
			//������ǣ����������������Ҫ��������ɸ���������ʽ
			out_indent(indent_num,gen_out);
			gen_out<<"{"<<endl;
			indent_num++;
			
			//2010��3��10��׷��
			fake_line_num=s->get_cs_if()->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//������֮ǰ��ע����Ϣ

			gen_stmt(s->get_cs_if(),v_if);//��if��֧�е������б���

			//������α��������ַ���
/*			string AddStmtSpecStr=SECOND_ADD_SPEC_STR+int2str(AddIfStmtIndex);
			out_indent(indent_num,gen_out);
			gen_out<<AddStmtSpecStr<<endl;*/

			indent_num--;
			out_indent(indent_num,gen_out);
			gen_out<<"}"<<endl;

		}
		S_if=*iter;
		S_else=0;
		//���else��֧���������
		out_indent(indent_num,gen_out);
		gen_out<<"else"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"{"<<endl;

		indent_num++;
		//�жϸ÷�֧�ṹ�Ƿ�Ϊif-else�ṹ
		S_last=S_selectin;
		if(1==s->get_stamp())
		{
			//����ǣ����ж�else��֧�е�����Ƿ�Ϊ�������
			if(0==s->get_cs_else()->get_stamp())
			{	
				//����ǣ���ֱ�ӶԸø�������е����������б���
				gen_stmtlist(s->get_cs_else()->get_ccstmt()->get_csl(),v_else);
				
				//2010��3��10��׷��
				//���Դ���и���������ǰ��ע��
				out_cmnt(cmnts,s->get_cs_else()->get_ccstmt()->get_fake_line(),gen_out);
			}
			else
			{
				//������ǣ���Դ������б���
				
				//2010��3��10��׷��
				fake_line_num=s->get_cs_else()->get_fake_line();
				out_cmnt(cmnts,fake_line_num,gen_out);//������֮ǰ��ע����Ϣ
				gen_stmt(s->get_cs_else(),v_else);
			}
			S_else=*iter;
		}
		else
		{
			//�������if-else�ṹ�������
		}

		/**************��֧����ڱ������******************/
		//���if��֧�е�ǩ����������
		if_branch_left_adjust(adj_names,v_if,v_else,AddIfStmtIndex);

		//�ϲ�������֧�еı���
		list<string> *merge_adj_names=new list<string>;
		merge_if_else_branch_adjvar(adj_names,merge_adj_names);
		//���else��֧�е�ǩ����������
		//inner_left_adjust(merge_adj_names,v_if,v_else,C);
		
		indent_num--;
		out_indent(indent_num,gen_out);
		gen_out<<"}"<<endl;
		//S_out=(S_if&(~U_h))^(S_3&U_h);
		out_indent(indent_num,gen_out);
		gen_out<<"S_select=("<<S_if<<"&(~Select_mark))^("<<S_else<<"&Select_mark);"<<endl;//�����֧ǩ���������by�ܺ�
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		//�����֧����ǩ���������by�ܺ�

		S_this=*++iter;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<"("<<S_this<<")-S_select;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		//�����֧����ǩ����������
		//ex_sig_adj(merge_adj_names,v_if,v_out,Select_OutSig,S_1);
		
		select_level--;//��֧������1
		delete []S_1;
//		delete []C;
		delete adj_names[0];
		delete adj_names[1];
		delete merge_adj_names;
		delete non_adj_names;
		delete temp_adj_var[0];
		delete temp_adj_var[1];
		delete v_if;
		delete v_else;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding selection statement :"+str;
		throw error_info;
	}
}

void if_branch_left_adjust(list<string> **adj_names,list<Var_info> *sig_if,list<Var_info> *sig_else,int __AddIndex)
	throw(string&)
{

#if TESTING_IF_LEFT_ADJ
//	adj_names=NULL;
//	adj_names[0]=NULL;
//	adj_names[1]=NULL;
	sig_if=NULL;
	sig_else=NULL;
#endif
	check_pt_arg(adj_names,"adj_var_names_gen object's pointer");
	check_pt_arg(adj_names[0],"adj_var_names0 object's pointer");
	check_pt_arg(adj_names[1],"adj_var_names1 object's pointer");
	check_pt_arg(sig_if,"sig_if  list's pointer");
	check_pt_arg(sig_else,"sig_else  list's pointer");

	int Prime[2]={P1,P2};
	string PrimeStr[2]={"P1","P2"};
	string ChkField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	StmtSet AddStmtSet;

	int constants[2]={0};
	list<string>::iterator if_iter=adj_names[0]->begin();
	list<string>::iterator else_iter=adj_names[1]->begin();
	string left_adj_var;
	for(;else_iter!=adj_names[1]->end();else_iter++)
	{
		if(find(adj_names[0]->begin(),adj_names[0]->end(),*else_iter)==adj_names[0]->end())
		{
			if(is_global_var(*else_iter))
				left_adj_var=get_bind_local_var(*else_iter,var_scope);
			else
				left_adj_var=*else_iter;
			//else ��֧���еı�������if��֧��û�У���Ҫ����
			int var_if_sig=get_current_sig(sig_if,left_adj_var,var_scope);
			int var_else_sig=get_current_sig(sig_else,left_adj_var,var_scope);
#if TESTING_IF_LEFT_ADJ
			static int test_if_left_adj_index=0;
			static int test_S_if[5]={1,1,P1-1,P2-1,100};
			static int test_S_else[5]={P1-1,P2-1,1,1,100};

			var_if_sig=test_S_if[test_if_left_adj_index];
			var_else_sig=test_S_else[test_if_left_adj_index];

			test_if_left_adj_index++;
#endif
		/*	if(var_if_sig==var_else_sig)
			{
				throw string("var'sigature in if branch is equal with it in else branch!\n");
			}*/
			for(int i=0;i<2;i++)
			{
				constants[i]=(((long long)var_else_sig-(long long)var_if_sig)%Prime[i]+Prime[i])%Prime[i];
#if TESTING_IF_LEFT_ADJ
				cout<<"if branch C"<<i<<" is "<<constants[i]<<endl;
#endif
			}
			//��Ӳ������
			vector<string> adj_stmt=var_check_adjust_stmt(left_adj_var,constants);
			for(int i=0;i<adj_stmt.size();i++)
				AddStmtSet.push_back(adj_stmt[i]);

			//����if��֧ǩ�����е�ǩ��
			add_new_sig(sig_if,left_adj_var,var_else_sig,var_scope);
		}
	}

	AddNewStmtSet(AddStmtSet,__AddIndex);
}

// ���ܣ���if��֧�еĸ��������б��룬������������е�����������������ͬʱ��Ӷ��������������ʱ�õ��������ַ���
// ���������s-����������ָ��
//                  v-��̬ǩ����ָ��
//           __AddIndex-��������������ɵ�����
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
void gen_compd_stmt_inSelectBranch(Compound_stmt* s,list<Var_info>* v,int __AddIndex)
{
	try
	{
		//������������������ָ�����Ч��
		check_pt_arg(s,"compound statement object's pointer");
		//����������̬ǩ������ָ�����Ч��
		check_pt_arg(v,"var_info list's pointer");

		//�������Ϊ�գ�����б���
		out_indent(indent_num,gen_out);
		gen_out<<"{"<<endl;
		indent_num++;//������1

		//gen_declist(s->get_cdl());//������������б���

		gen_stmtlist(s->get_csl(),v);//�����������б���

		//������α��������ַ���
		/*string AddStmtSpecStr=SECOND_ADD_SPEC_STR+int2str(__AddIndex);
		out_indent(indent_num,gen_out);
		gen_out<<AddStmtSpecStr<<endl;
*/
		//���Դ���и���������ǰ��ע��
		out_cmnt(cmnts,s->get_fake_line(),gen_out);

		indent_num--;//���������������1
		out_indent(indent_num,gen_out);
		gen_out<<"}"<<endl;
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding compound statement :  "+str;
		throw error_info;
	}
}

//�ϲ�if else ������֧�еı�����
void merge_if_else_branch_adjvar(list<string> **branch_names,list<string> *merge_names)
{
	list<string>::iterator else_var_iter=branch_names[1]->begin();
	merge_names->insert(merge_names->end(),branch_names[0]->begin(),branch_names[0]->end());
	for(;else_var_iter!=branch_names[1]->end();else_var_iter++)
	{
		if(find(branch_names[0]->begin(),branch_names[0]->end(),*else_var_iter)==branch_names[0]->end())
		{
			merge_names->push_back(*else_var_iter);
		}
	}
}



