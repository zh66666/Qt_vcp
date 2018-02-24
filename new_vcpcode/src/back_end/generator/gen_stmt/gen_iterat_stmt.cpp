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
int S_Iterat_in,S_Iterat_out;//循环入口签名和循环出口签名

//此结构用于保存编码工程中dD的名字和对应的值
struct dD_value
{
	string name;//记录dD的名字
	int value;//记录dD的值
};

//用于记录循环体内的需要调整的变量信息，包括名称，是否为左值还是右值
struct adj_var
{
	string name;//记录变量名字
	string stamp;//记录为左值还是右值
};

//记录循环入口编码时，需要用到的变量名称，索引号等参数
struct WhileNameIDEntr
{
	string CondVar;
	string BodySigName;
	CFSubVar SubVarName;
	int WhileABIndex;
};

//记录循环出口编码时，需要用到的变量名称，索引号等参数
struct WhileNameOut
{
	int WhileABIndex;
	string WhileOutSig;
};


// 定义在文件: src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern bool debug_flag;
extern list<dD_value>* dD_values;
extern list<Cmnt>* cmnts;
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);

// 定义在文件 : back_end_utils.c
extern int goto_label;

//定义在文件: ./gen_stmt.cpp
extern void gen_stmtlist(list<Statement>* s,list<Var_info>* v);
extern void gen_stmt(Statement* s,list<Var_info>* v);

//定义在文件: ./while_if_utils.cpp
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

// 功能：为当前循环挑选dD，从签名文件中取一个合适的值作为dD，保证当循环体的冗余代码在运行时，
// 			内存中不会出现两个变量有同样的签名
// 输入参数：l-待调整签名的变量名链表指针
// 			      v-动态签名表指针
// 返回值：成功则返回挑选到的dD值，失败则抛出错误信息
// 作者：丁佳
static int select_dD(list<adj_var>* l, list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(l,"variable name list's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		int dD=0;//存储为该循环语句挑选的dD值
		vector<int>* current_sigs=new vector<int>;//用于保存当前所有变量的签名
		//遍历动态签名表，保存工程中所有变量的当前签名
		for(list<Var_info>::iterator iter=v->begin();iter!=v->end();iter++)
		{
			current_sigs->push_back(iter->get_sigs()->back());
		}
		
		vector<int>* while_varsigs=new vector<int>; //用于保存循环内所有变量的初始签名
		//保存循环体中的需要调整的变量的当前签名
		for (list<adj_var>::iterator iter=l->begin();iter!=l->end();iter++)
		{
			while_varsigs->push_back(get_current_sig(v,iter->name,var_scope));
		}

		RE_ASSIGN:dD = get_sig();//从签名表中取dD
		//遍历循环体内所有变量的签名链表
		for(vector<int>::iterator iter=while_varsigs->begin();iter!=while_varsigs->end();iter++)
		{
			//循环1000次，每次对签名+dD，看有没有和工程中所有变量的当前签名重复
			for(int i = 1; i!=1000; ++i)
			{
				int tmp_sig=((long long)(*iter)+(long long)i*dD%A)%A;//签名加dD
				//判断是否有重复
				if(current_sigs->end()!=find(current_sigs->begin(),current_sigs->end(),tmp_sig))
				{
					//如果有，则重新分配
					goto RE_ASSIGN;
				}
				else
				{
					//如果没有重复，则继续增加dD比较
				}
			}
		}
		return dD;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="select dD : "+str;
		throw error_info;
	}
}

// 功能：属于对循环结构编码的子模块，根据条件表达式操作符的类型，输出相应的循环入口的冗余代码，
// 			包括表达式左值的低位调整代码和生成相应的WHILE_K计算语句
// 输入参数：s-循环入口条件表达式对象指针
// 			      v-动态签名表指针
// 输出参数：S1-循环编码中用到的S1值的地址
//			      k-循环编码中用于计算S1值的K值的地址
// 返回值：成功则返回计算出来的S2值，失败则抛出错误信息
// 作者：丁佳
static int *gen_while_exp(Expression* s,list<Var_info>* v,int* S1,int *k,const WhileNameIDEntr &name_index)
{
	int Prime[2]={P1,P2};
	string PrimeStr[2]={"P1","P2"};
	string S1S2Str[2]={IF_WHILE_SIG_S1,IF_WHILE_SIG_S2};
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		//判断入口表达式对象是否合法
		if(1)
		{
			//输出循环入口语句by周航
			string cel_name=s->get_cel()->get_cvar()->get_name();//表达式左值变量名
			out_indent(indent_num,gen_out);
			gen_out<<"Iterat_decode="<<"check_decode("<<cel_name<<"_1,"<<cel_name<<"_2);"<<endl;//校验解码循环入口
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
			//如果入口条件表达式非法，则抛出错误信息
			string error_info="the entrance expression is invalid! ";
			throw error_info;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding entrance expression :  "+str;
		throw error_info;
	}
	
}

// 功能：本模块为循环结构冗余代码生成模块的子模块。作用是在循环体编码结束后，输出左值变量的签名
// 			调整代码，为左值变量分配出口签名
// 输入参数：name-待调整的左值变量名
// 			     VarNameSet-相关变量名称--WHile_A+While_B的值  循环出口签名值
// 			     S2-用于签名调整的S2值
// 			     v_old-循环体外的动态签名表指针
// 返回值：成功则返回void
// 作者：丁佳
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
		//检查参数：待调整变量名字符串的有效性
		check_str_arg(name,"variable's name");
		//检查参数：动态签名表指针的有效性
		check_pt_arg(v_old,"var_info list's pointer");
		
		int old_sig=get_current_sig(v_old,name,var_scope);//获取当前变量的旧签名
		int new_sig_id=0;
		int new_sig=get_sig_index(&new_sig_id);//分配新签名作为出口签名

		string T_CHECK_Str[2]={IF_WHILE_SIG_S1,IF_WHILE_SIG_S2};
		for(int i=0;i<2;i++)
		{
			//计算补偿常数
			constants[i]=(((long long)new_sig-old_sig-S2[i])%Prime[i]+Prime[i])%Prime[i];
			//求出While_A+While_B 的字符串
			WhileAStr=WHILE_AC_A_PRE+int2str(VarNameSet.WhileABIndex)+T_CHECK_Str[i];
			WhileBStr=WHILE_AC_B_PRE+int2str(VarNameSet.WhileABIndex)+T_CHECK_Str[i];
			A_plus_B_str[i]="("+WhileAStr+" + "+WhileBStr+")";
		}
		if(name!=ARRAY_EXTR_NAME)
		{
			for(int i=0;i<2;i++)
			{
				//输出签名调整代码
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

		//输出签名Id调整语句
		string SigIDAdjStr=BuildSigIdAdjust_str(name,new_sig_id);
		gen_out<<OutIndentStr(indent_num)+SigIDAdjStr<<endl;

		//如果是调试模式，则输出调试信息：循环出口的新签名
		if(true==debug_flag)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"//"<<name<<"'s new sig :"<<new_sig<<endl;
		}
		//将新签名加入动态签名表中
		add_new_sig(v_old,name,new_sig,var_scope);
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="adjust  left variable’s export signatures : "+str;
		throw error_info;
	}
}

// 功能：本模块为循环结构冗余代码生成模块的子模块。作用是在循环体编码结束后，输出右值变量的签名
// 			调整代码
// 输入参数：name-待调整的右值变量名
// 			      VarNameSet-相关变量名集合
// 返回值：成功则返回void
// 作者：丁佳
static void ex_right_adj(string name,const WhileNameOut &VarNameSet)
{
	string ChkField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string PrimeStr[2]={"P1","P2"};
	try
	{
		//检查参数：待调整变量名字符串的有效性
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
				//输出变量时间标签调整代码
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
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="adjust  right variable’s export signatures : "+str;
		throw error_info;
	}
}

// 功能：此模块为循环语句冗余代码生成主模块。完成对循环语句的入口表达式进行编码，计算编码中的S1,S2值
// 			，挑选dD，对循环中的语句进行编码，循环出口的签名调整等任务
// 输入参数：s-循环语句对象指针
// 输出参数：v_out-循环体外的动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：丁佳   修改by周航
void gen_iterat_stmt(Iteration_stmt* s,list<Var_info>* v_out)
{
	string TLStr[2]={TL1_NAME,TL2_NAME};
	string PrimeStr[2]={"P1","P2"};
	string CheckFieldStr[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string iterator_adj_var;//待调整变量名

	int Prime[2]={P1,P2};
	int constant[2]={0,0};
	vector<string> adj_stmt;
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"iteration statement object's pointer");
		check_pt_arg(v_out,"var_infos list's pointer");
		
		string WhileBodySigName=BuildTmpVarName(WHILE_BODYSIG_PREFIX,"",++WhileSigCnt);
		string WhileOutSigName=BuildTmpVarName(WHILE_OUTSIG_PREFIX,"",WhileSigCnt);
		//构建本层循编码需要用到的名称和索引号

		string CelVarName=s->get_ce()->get_cel()->get_cvar()->get_name();

		CFSubVar CurWhileSubVar;
		int WhileABIndex=MappingCondVar(CelVarName,CurWhileSubVar);
		WhileNameIDEntr CurWhileEntrNameId={CelVarName,WhileBodySigName,CurWhileSubVar,WhileABIndex};


		list<adj_var>* temp_adj_names=new list<adj_var>;//存储本层循环结构中需要调整变量的名字
		
		//取出while语句中的左值变量和右值变量
		get_var_names(s->get_cs(),temp_adj_names);

		//添加本层循环的AC_Sub_进入待调整变量链表中
		

		list<Var_info>* v_in=new list<Var_info>;//用于记录循环体内编码时签名变化情况的动态签名表
		copy_Var_infos(v_out,v_in);//先将外部签名表拷贝给循环内的签名表
		
		string dD_name=DELTA_T_NAME;//dD变量的名字
		
		//S1 S2 均为循环体签名 不含乘法因子 K
		int *S1=new int[2];
		int K=0;
		string cel_name=s->get_ce()->get_cel()->get_cvar()->get_name();
		int *S2=gen_while_exp(s->get_ce(),v_out,S1,&K,CurWhileEntrNameId);//对循环入口表达式进行编码
		S_Iterat_in=*iter;//获取入口签名by周航

		//判断循环体中的语句是否为复合语句
		if(0==s->get_cs()->get_stamp())
		{	


			//如果是，则直接对复合语句中的语句链表进行编码
			gen_stmtlist(s->get_cs()->get_ccstmt()->get_csl(),v_in);
			//2010年3月10日追加
			//输出源码中复合语句结束前的注释
			out_cmnt(cmnts,s->get_cs()->get_ccstmt()->get_fake_line(),gen_out);
		}
		else
		{
			//如果不是，则直接对该语句进行编码
			
			//2010年3月10日追加
			fake_line_num=s->get_cs()->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//输出语句之前的注释信息
				
			gen_stmt(s->get_cs(),v_in);
		}
		
		S_Iterat_out=*iter;//获取出口签名
		//调整单次循环结束后签名by周航
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS-"<<"("<<S_Iterat_out<<")+("<<S_Iterat_in<<"&(~Iterat_mark))^("<<S_Iterat_out<<"&Iterat_mark);"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		//计算单次循环结束后高位标识符及循环入口解码值by周航
		out_indent(indent_num,gen_out);
		gen_out<<"Iterat_decode="<<"check_decode("<<cel_name<<"_1,"<<cel_name<<"_2);"<<endl;//校验解码循环入口
		out_indent(indent_num,gen_out);
		gen_out<<"Iterat_mark="<<"(long long) Iterat_decode>>32;"<<endl;//计算循环结构高位标记
		out_indent(indent_num,gen_out);
		gen_out<<endl;
		indent_num--;
		out_indent(indent_num,gen_out);
		gen_out<<"}"<<endl;
		//调整所有循环结束后签名by周航

		gen_out<<"RS=RS-"<<"("<<S_Iterat_in<<")+("<<S_Iterat_in<<"&(~Iterat_mark))^("<<S_Iterat_out<<"&Iterat_mark);"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<endl;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding iteration statement : "+str;
		throw error_info;
	}
}
//根据循环条件变量名，映射两个参数
//1-WHile_A/B的索引号
//2-While_SUb的名称
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
