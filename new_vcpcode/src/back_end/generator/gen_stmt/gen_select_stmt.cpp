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
int S_if,S_else;//if和else分支中最后一条语句的语句签名
int S_out;//分支出口签名

//用于记录循环体内的需要调整的变量信息，包括名称，是否为左值还是右值
struct adj_var
{
	string name;//记录变量名字
	string stamp;//记录为左值还是右值，如果为左值，则stamp为"left"，否则为"right"
};

// 定义在文件: src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern bool debug_flag;
extern list<Cmnt>* cmnts;
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);

// 定义在文件 : ../gen_pro.cpp
extern void gen_declist(list<Declaration>*);

//定义在文件 : ./gen_stmt.cpp
extern void gen_stmt(Statement* s,list<Var_info>* v);
extern void gen_stmtlist(list<Statement>* s,list<Var_info>* v);

//定义在文件 : ./while_if_utils.cpp
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

// 功能：对两个list进行比较，将既出现在list1，又出现在list2中的元素，从list1中删除掉
// 输入参数：list1-链表1的指针
// 			      list2-链表2的指针
// 输出参数：list1-链表1的指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：仝明哲
static void del_same_names(list<string>* list1, list<string>* list2)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(list1,"list1's pointer");
		check_pt_arg(list2,"list2's pointer");
		
		//遍历链表list1
		for(list<string>::iterator iter=list1->begin();iter!=list1->end();)
		{
			//在链表list2中查找当前迭代器所指的字符串，看其是否存在
			if(list2->end()!=find(list2->begin(),list2->end(),*iter))
			{
				//如果存在，则将字符串删除
				iter=list1->erase(iter);
			}
			else
			{
				//如果不存在，则继续遍历
				iter++;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="delete same names : "+str;
		throw error_info;
	}
}

// 功能：此模块分析分支内部赋值语句的左值变量，将其名称作为左值存储起来，如果发现是一般临时变量
//			则不存储
// 输入参数：s-变量对象指针
// 输出参数：l-待调整变量名链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：仝明哲
static void get_var_names(Var* s,list<adj_var>* l)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"variable object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		
		string name="";//存储要存放的变量名称
		//判断该变量是否为数组变量
		if(1==s->get_stamp())
		{
			//遍历该数组变量的下标表达式
			get_var_names(s->get_cexp(),l);
		}
		else
		{
			//如果不是，则变量名即为要存储的名称
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

// 功能：此模块分析分支内部的语句，解析出左值变量和右值变量，并将其存储起来，同时将不需要调
//			整的变量的名称也存储起来
// 输入参数：s-语句对象指针
// 输出参数：l-待调整变量名链表指针
//			      n-不需要调整的变量名链表指针
// 返回值：成功则返回void，，失败则抛出错误信息
// 作者：仝明哲
static void get_var_names(Statement* s,list<adj_var>* l,list<string>* n)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"statement object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		
		//判断语句的类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//如果为复合语句，则遍历复合语句中的语句链表
				for(list<Statement>::iterator iter=s->get_ccstmt()->get_csl()->begin();
					   iter!=s->get_ccstmt()->get_csl()->end();iter++)
				{
					get_var_names(&(*iter),l,n);//遍历每条语句
				}
			}
			break;	
			case 1:
			{
				//如果为分支语句，则先遍历if分支入口处的表达式的左操作数
				//取出左操作数名称
//				string l_name=s->get_csstmt()->get_ce()->get_cel()->get_cvar()->get_name();
//				if("tmpArray"!=l_name.substr(0,8))
//				{
//					//如果不是，则将此变量名作为右值存储起来
//					struct adj_var a;
//					a.name=l_name;
//					a.stamp="right";
//					add_adj_name(l,a);
//				}
				
				//遍历if分支中的语句
				get_var_names(s->get_csstmt()->get_cs_if(),l,n);

				//判断是否含有else分支
				if(1==s->get_csstmt()->get_stamp())
				{
					//如果含有else分支，则遍历else分支中的语句
					get_var_names(s->get_csstmt()->get_cs_else(),l,n);
				}
				else
				{
					//如果无else分支，则忽略
				}
			}
			break;	
			case 2:
			{
				//如果为循环语句，则先遍历循环入口条件表达式的左操作数
				//get_var_names(s->get_cistmt()->get_ce()->get_cel()->get_cvar(),l);
				
				//遍历循环体中的语句
				get_var_names(s->get_cistmt()->get_cs(),l,n);
			}
			break;	
			case 4:
			{
				//赋值语句，首先分析右值变量  分析顺序--先右后左
				string left_name;
				//判断左值是否是数组
				//不是数组
				if(s->get_cvar()->get_stamp()==0)
				{
					//如果为赋值语句，先判断该语句是否为函数赋值语句
					if(s->get_ce()->get_stamp()==5)
					{
						//分析函数参数表达式列表，每个参数均作为右值存储起来
//						for(list<Expression>::iterator  iter=s->get_ce()->get_args()->begin();
//							   iter!=s->get_ce()->get_args()->end();iter++)
//						{
//							get_var_names(&(*iter),l);
//						}
						//将本函数中的全局变量作为左值保存
						vector<string> global_var_list=get_global_var_infunc(var_scope);
						for(vector<string>::iterator it=global_var_list.begin();it!=global_var_list.end();it++)
						{
							struct adj_var adj_global_var={*it,"left"};
							add_adj_name(l,adj_global_var);
						}
					}
					else
					{
						//如果不为函数调用，则遍历该赋值语句右边的表达式
						//get_var_names(s->get_ce(),l);
					}

					left_name=s->get_cvar()->get_name();//存储左值变量名
					if(ExtraVarJudge(left_name))
					{
						//临时变量不需要补偿签名
//						if(n->end()==find(n->begin(),n->end(),left_name))
//						{
//							n->push_back(left_name);
//						}
					}
					else
					{
						//如果不是，则分析左值变量
						 get_var_names(s->get_cvar(),l);
					}
				}
				//数组赋值
				else
				{
					//先下标
					//get_var_names(s->get_cvar(),l);
					//右值
					//get_var_names(s->get_ce(),l);
					//附加变量
					struct adj_var array_ext={ARRAY_EXTR_NAME,"left"};
					add_adj_name(l,array_ext);
				}

			}
			break;
			
			default:
			{
				//其他语句不遍历，忽略
			}
		}
	}
	catch (string str)
	{
		string error_info="get variable names in statement : "+str;
		throw error_info;
	}
}

// 功能：输出分支编码结束后，出口处的签名调整代码
// 输入参数：adj_var-需要补偿的变量名链表指针
// 			      v_in-分支内部的动态签名表
// 			      v_out-分支将要向外输出的动态签名表
// 			      depth-分支当前的层数
// 			      s1-分支编码时用到的S1值
// 返回值：成功则返回void
// 作者：仝明哲
static void ex_sig_adj(list<string>* adj_var,list<Var_info>* v_in,list<Var_info>* v_out,string IfOutSigName, int *s1)
{
	int constant[2]={0,0};
	int Prime[2]={P1,P2};
	string AdjVarChkFieldStr[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string IfOutSigSet[2]={IfOutSigName+IF_WHILE_SIG_S1,IfOutSigName+IF_WHILE_SIG_S2};
	string PrimeStr[2]={"P1","P2"};
	try
	{
		//检查输入参数的有效性
		check_pt_arg(adj_var,"varibale name  list's pointer");
		check_pt_arg(v_in,"inner var_info  list's pointer");
		check_pt_arg(v_out,"export var_info  list's pointer");
		
		string left_adj_var;
		//遍历需要补偿的变量名链表
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
			int out_sig_id=0;//签名Id
			int out_sig=get_sig_index(&out_sig_id);//分配一个出口签名
			int in_sig=get_current_sig(v_in,left_adj_var,var_scope);//记录当前签名
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
			//计算低位调整时的补偿常数
			for(int i=0;i<2;i++)
				constant[i] = (((long long)out_sig-(long long)in_sig-(long long)s1[i])%Prime[i]+Prime[i])%Prime[i];
#if TESTING_EX_ADJ
			cout<<"constant0 is "<<constant[0]<<endl;
			cout<<"constant1 is "<<constant[1]<<endl;
#endif

			//输出低位补偿代码
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

			//输出签名Id调整语句
			string SigIDAdjStr=BuildSigIdAdjust_str(left_adj_var,out_sig_id);
			gen_out<<OutIndentStr(indent_num)+SigIDAdjStr<<endl;

			//如果是调试模式，则输出调试信息：分支出口的新签名
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<(*iter)<<"'s new sig :"<<out_sig<<endl;
			}

			 //将新签名加入要向外输出的动态签名表
			add_new_sig(v_out,left_adj_var,out_sig,var_scope);
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="adjust variable's export signatures : "+str;
		throw error_info;
	}
}

// 功能：属于对分支结构编码的子模块，根据条件表达式操作符的类型，输出相应的分支入口的冗余代码，
// 			包括表达式左值的低位调整代码，SI的计算代码等
// 输入参数：s-分支入口条件表达式对象指针
// 			      v-动态签名表指针
//          Select_OutSigName-分支出口签名名称
// 输出参数：v-动态签名表指针
// 			      S1-分支中用到的S1值的地址
// 返回值：成功则返回计算出来的C值，失败则抛出错误信息
// 作者：仝明哲
static int *gen_if_exp(Expression* s,list<Var_info>* v,int* S1,string& Select_OutSigName)
{
	CFCondVar CurIfCndVar;
	CFSubVar CurIfSubVar;
	int Prime[2]={P1,P2};
	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(s,"expression object's pointer");
		//检查参数：动态签名表指针的有效性
		check_pt_arg(v,"var_info  list's pointer");
		//判断入口表达式对象是否合法
		//if(check_condition_entrance_exp(s))
		{
			//如果合法，则输出表达式的冗余代码
			//获得与条件判断变量相关的减法差值变量名
			string cel_name=s->get_cel()->get_cvar()->get_name();//表达式左值变量名
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
//			//AC_IfSub_1和AC_IfSub_2的签名
//			int BT[2]={0,0};
//			for(int i=0;i<2;i++)
//				BT[i]=get_current_sig(v,CurIfSubVar.SubVar[i].VarName,var_scope);
//
//			int k=get_sig_index(NULL);//分配一个签名，用于计算S值，即算法中随机数k

//			string ce_op=s->get_op();//提取表达式操作符

			//计算S1和S2的值 分支签名S1-True S2-False
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
//				C[i]=((long long)(S1[i])-(long long)(S2[i]) + Prime[i] ) % Prime[i];//计算C的值
//			}
//
//			//输出分支出口签名计算式
//			Select_OutSigName=BuildTmpVarName(IF_SELECT_OUTSIG_PREFIX,"",++SelectOutSigCnt);
//			string OutSigFuncCall=BuildOutputFunc_str(Select_OutSigName,FUNC_OUTSIG,
//					string(CurIfSubVar.SubVar[0].VarName+".Data").c_str(),
//					string(CurIfSubVar.SubVar[1].VarName+".Data").c_str(),
//					string(int2HexStr(k)).c_str(),NULL);
//			out_indent(indent_num,gen_out);
//			gen_out<<OutSigFuncCall<<endl;
//
//			//如果是调试模式，则输出调试信息：扩散因子的值，S1,S2的值
//			if(true==debug_flag)
//			{
//				out_indent(indent_num,gen_out);
//				gen_out<<"// K:"<<k<<" S1[0]:"<<S1[0]<<" S1[1]:"<<S1[1]<<endl;
//				out_indent(indent_num,gen_out);
//				gen_out<<"// K:"<<k<<" S2[0]:"<<S2[0]<<" S2[1]:"<<S2[1]<<endl;
//			}

			//输出分支入口高位调整by周航
			out_indent(indent_num,gen_out);
			gen_out<<"Select_decode="<<"check_decode("<<cel_name<<"_1,"<<cel_name<<"_2);"<<endl;//校验解码分支入口
			out_indent(indent_num,gen_out);
			gen_out<<"Select_mark="<<"(long long) Select_decode>>32;"<<endl;//计算分支结构高位标记
			out_indent(indent_num,gen_out);
			gen_out<<endl;
			//输出分支入口语句by周航
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
//			//如果入口条件表达式非法，则抛出错误信息
//			string error_info="the entrance expression  is invalid! ";
//			throw error_info;
//		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding entrance expression : "+str;
		throw error_info;
	}
}

// 功能：在对if结构进行编码时，输出结构内的低位调整代码
// 输入参数：names-存有要调整的变量名字的链表指针
// 			      v1-动态签名表1指针
// 			      v2-动态签名表2指针
// 			      c-调整中用到的常数
// 返回值：成功则返回void
// 作者：仝明哲
static void inner_left_adjust(list<string>* names,list<Var_info>* v1,list<Var_info>* v2,int *c)
{
	int constant[2]={0,0};
	int Prime[2]={P1,P2};
	string CheckField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string PrimeStr[2]={"P1","P2"};
	try
	{
		//检查输入参数的有效性
		check_pt_arg(names,"variable name list's pointer");
		check_pt_arg(v1,"var_info  list1's pointer");
		check_pt_arg(v2,"var_info  list2's pointer");
		
		string left_adj_var;

		//遍历存有要调整低位的变量名字的链表
		for(list<string>::iterator iter=names->begin();iter!=names->end();iter++)
		{
			if(is_global_var(*iter))
				left_adj_var=get_bind_local_var(*iter,var_scope);
			else
				left_adj_var=*iter;
			int sig1=get_current_sig(v1,left_adj_var,var_scope);//变量在动态签名表1中的签名
			int sig2=get_current_sig(v2,left_adj_var,var_scope);//变量在动态签名表2中的签名

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
			//计算调整用到的补偿常数
			for(int i=0;i<2;i++)
				constant[i]=(((long long)sig1-(long long)sig2+(long long)c[i])%Prime[i]+Prime[i])%Prime[i];

#if TESTING_INNER_LEFT
			cout<<"constant0 is "<<constant[0]<<endl;
			cout<<"constant1 is "<<constant[1]<<endl;
#endif
			//输出调整代码
			vector<string> adj_stmt=var_check_adjust_stmt(left_adj_var,constant);
			for(int i=0;i<adj_stmt.size();i++)
				gen_out<<adj_stmt[i]<<endl;

		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="adjust left variable's inner signatures : "+str;
		throw error_info;
	}
}

// 功能：分支语句冗余代码生成的主模块。对分支结构进行编码，包括分支入口表达式，分支中的语句的编码，
// 			在else分支和分支结构出口按照算法进行签名补偿
// 输入参数：s-分支语句对象指针
// 输出参数：v_out-经过分支结构之后的动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：仝明哲 修改by周航
void gen_select_stmt(Selection_stmt* s,list<Var_info>* v_out)
{
	try
	{
		//检查输入参数的有效性
#if TESTING_GEN_SELECT
		s->get_ce()->set_cer(new Expression("FALSE"));
		s=NULL;
		v_out=NULL;
#endif

		check_pt_arg(s,"selection statement object's pointer");
		check_pt_arg(v_out,"var_info  list's pointer");
		
		int AddIfStmtIndex=SecondAddIndex++;//设定本if语句层的二次代码生成索引号

		string Select_OutSig;//在线计算的分支出口签名名称

		select_level++;//分支层数加1
		int *S_1=new int[2];//保存分支编码时用到的S1值
		int *C=gen_if_exp(s->get_ce(),v_out,S_1,Select_OutSig);//对分支入口表达式进行编码，并记录C值

		
		list<Var_info>* v_if=new list<Var_info>;//保存if分支语句中签名变化情况的动态签名表
		list<Var_info>* v_else=new list<Var_info>;//保存else分支语句中签名变化情况的动态签名表
		copy_Var_infos(v_out,v_if);//将当前的签名拷贝一份给if分支中的动态签名表
		copy_Var_infos(v_out,v_else);//将当前的签名拷贝一份给else分支中的动态签名表
		
		//存储分支内部的左值变量和右值变量名
		//0-if 分支
		//1-else 分支
		list<adj_var>* temp_adj_var[2]={new list<adj_var>,new list<adj_var>};
		list<string>* non_adj_names=new list<string>;//存储分支内部的不需要调整的条件临时变量名
	
		//遍历if分支中的语句，获取需要补偿和不需要补偿的变量的名字
		get_var_names(s->get_cs_if(),temp_adj_var[0],non_adj_names);
		//判断是否有else分支
		if(1==s->get_stamp())
		{	
			//如果为if-else结构，则获取else分支中的变量名
			get_var_names(s->get_cs_else(),temp_adj_var[1],non_adj_names);
		}
		else
		{
			//如果没有else分支，则忽略
		}
		
		//存储需要进行调整的变量名
		//0-if branch
		//1-else branch
		list<string>* adj_names[2]={new list<string>,new list<string>};
		//遍历存有左值变量名和右值变量名的链表，将左值变量名取出
		for(int i=0;i<2;i++)
			for(list<adj_var>::iterator iter=temp_adj_var[i]->begin();iter!=temp_adj_var[i]->end();iter++)
			{
				if(iter->stamp=="left")
				{
					adj_names[i]->push_back(iter->name);
				}
			}
		//去除不需要调整的条件临时变量名
		for(int i=0;i<2;i++)
			del_same_names(adj_names[i], non_adj_names);

		/*****开始分支语句内的语句编码*****/
		//存储分支入口签名by周航
		int S_selectin=S_last;

		//判断if分支中的语句是否为复合语句
		if(0==s->get_cs_if()->get_stamp())
		{
			//如果是，则对该复合语句进行编码
			gen_compd_stmt_inSelectBranch(s->get_cs_if()->get_ccstmt(),v_if,AddIfStmtIndex);
		}
		else
		{
			//如果不是，则在冗余代码中需要将其输出成复合语句的形式
			out_indent(indent_num,gen_out);
			gen_out<<"{"<<endl;
			indent_num++;
			
			//2010年3月10日追加
			fake_line_num=s->get_cs_if()->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//输出语句之前的注释信息

			gen_stmt(s->get_cs_if(),v_if);//对if分支中的语句进行编码

			//输出二次编码特殊字符串
/*			string AddStmtSpecStr=SECOND_ADD_SPEC_STR+int2str(AddIfStmtIndex);
			out_indent(indent_num,gen_out);
			gen_out<<AddStmtSpecStr<<endl;*/

			indent_num--;
			out_indent(indent_num,gen_out);
			gen_out<<"}"<<endl;

		}
		S_if=*iter;
		S_else=0;
		//输出else分支的冗余代码
		out_indent(indent_num,gen_out);
		gen_out<<"else"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"{"<<endl;

		indent_num++;
		//判断该分支结构是否为if-else结构
		S_last=S_selectin;
		if(1==s->get_stamp())
		{
			//如果是，则判断else分支中的语句是否为复合语句
			if(0==s->get_cs_else()->get_stamp())
			{	
				//如果是，则直接对该复合语句中的语句链表进行编码
				gen_stmtlist(s->get_cs_else()->get_ccstmt()->get_csl(),v_else);
				
				//2010年3月10日追加
				//输出源码中复合语句结束前的注释
				out_cmnt(cmnts,s->get_cs_else()->get_ccstmt()->get_fake_line(),gen_out);
			}
			else
			{
				//如果不是，则对此语句进行编码
				
				//2010年3月10日追加
				fake_line_num=s->get_cs_else()->get_fake_line();
				out_cmnt(cmnts,fake_line_num,gen_out);//输出语句之前的注释信息
				gen_stmt(s->get_cs_else(),v_else);
			}
			S_else=*iter;
		}
		else
		{
			//如果不是if-else结构，则忽略
		}

		/**************分支语句内编码结束******************/
		//输出if分支中的签名调整代码
		if_branch_left_adjust(adj_names,v_if,v_else,AddIfStmtIndex);

		//合并两个分支中的变量
		list<string> *merge_adj_names=new list<string>;
		merge_if_else_branch_adjvar(adj_names,merge_adj_names);
		//输出else分支中的签名调整代码
		//inner_left_adjust(merge_adj_names,v_if,v_else,C);
		
		indent_num--;
		out_indent(indent_num,gen_out);
		gen_out<<"}"<<endl;
		//S_out=(S_if&(~U_h))^(S_3&U_h);
		out_indent(indent_num,gen_out);
		gen_out<<"S_select=("<<S_if<<"&(~Select_mark))^("<<S_else<<"&Select_mark);"<<endl;//输出分支签名运算语句by周航
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		//输出分支出口签名调整语句by周航

		S_this=*++iter;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<"("<<S_this<<")-S_select;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		//输出分支出口签名调整代码
		//ex_sig_adj(merge_adj_names,v_if,v_out,Select_OutSig,S_1);
		
		select_level--;//分支层数减1
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
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
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
			//else 分支中有的变量，但if分支中没有，需要补偿
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
			//添加补偿语句
			vector<string> adj_stmt=var_check_adjust_stmt(left_adj_var,constants);
			for(int i=0;i<adj_stmt.size();i++)
				AddStmtSet.push_back(adj_stmt[i]);

			//更新if分支签名表中的签名
			add_new_sig(sig_if,left_adj_var,var_else_sig,var_scope);
		}
	}

	AddNewStmtSet(AddStmtSet,__AddIndex);
}

// 功能：对if分支中的复合语句进行编码，包括复合语句中的声明链表和语句链表，同时添加二次冗余代码生成时用到的特殊字符串
// 输入参数：s-复合语句对象指针
//                  v-动态签名表指针
//           __AddIndex-二次冗余代码生成的索引
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
void gen_compd_stmt_inSelectBranch(Compound_stmt* s,list<Var_info>* v,int __AddIndex)
{
	try
	{
		//检查参数：复合语句对象指针的有效性
		check_pt_arg(s,"compound statement object's pointer");
		//检查参数：动态签名链表指针的有效性
		check_pt_arg(v,"var_info list's pointer");

		//如果均不为空，则进行编码
		out_indent(indent_num,gen_out);
		gen_out<<"{"<<endl;
		indent_num++;//缩进加1

		//gen_declist(s->get_cdl());//对声明链表进行编码

		gen_stmtlist(s->get_csl(),v);//对语句链表进行编码

		//输出二次编码特殊字符串
		/*string AddStmtSpecStr=SECOND_ADD_SPEC_STR+int2str(__AddIndex);
		out_indent(indent_num,gen_out);
		gen_out<<AddStmtSpecStr<<endl;
*/
		//输出源码中复合语句结束前的注释
		out_cmnt(cmnts,s->get_fake_line(),gen_out);

		indent_num--;//编码结束后，缩进减1
		out_indent(indent_num,gen_out);
		gen_out<<"}"<<endl;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding compound statement :  "+str;
		throw error_info;
	}
}

//合并if else 两个分支中的变量名
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



