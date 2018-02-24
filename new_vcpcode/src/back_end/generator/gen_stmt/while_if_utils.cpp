#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_utils.h"
#include <iostream>
#include "statement.h"
#include "expression.h"
#include "var.h"
#include "compound_stmt.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "gen_global_var.h"

#define TESTING_S1S2 0

//用于记录循环体内的需要调整的变量信息，包括名称，是否为左值还是右值
struct adj_var
{
	string name;//记录变量名字
	string stamp;//记录为左值还是右值
};

//存储循环体内需要调整的变量信息
list<adj_var>* adj_name_list=new list<adj_var>;


// 功能：将遍历得到的变量信息加入全局的待调整变量名链表
// 输入参数：l-待调整变量名链表指针
// 			     a-待调整的变量信息结构体
// 输出参数：l-待调整变量名链表指针
// 返回值：成功则返回void
// 作者：丁佳
void  add_adj_name(list<adj_var>* l, struct adj_var a)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(l,"adjust variable name list's pointer");
		
		list<adj_var>::iterator iter;//链表迭代器
		//遍历存储有待调整变量信息的链表
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//判断当前迭代器所指的变量的名字与待加入的变量名字是否一致
			if(a.name==iter->name)
			{
				//如果一致，则说明已经存在该变量，则修改其左右值信息
				iter->stamp=(a.stamp=="left")?"left":iter->stamp;
				return;
			}
		}
		//判断迭代器是否到了链表末端
		if(iter==l->end())
		{
			//如果是，则将此变量信息存入链表
			l->push_back(a);
		}
	}
	catch (string str)
	{
		string error_info="add adjust variable : "+str;
		throw error_info;
	}
}

// 功能：此模块分析赋值语句右边的赋值表达式，将表达式中的变量名均作为右值变量名存储起来
// 输入参数：s-表达式对象指针
// 输出参数：l-用来存放所要获取的变量名称的链表指针
// 返回值：成功则返回void
// 作者：丁佳，仝明哲
void get_var_names(Expression* s,list<adj_var>*  l)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"expression object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		//判断表达式的类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//如果是带有两个操作数的表达式，则分别遍历左值表达式和右值表达式
				get_var_names(s->get_cel(),l);
				get_var_names(s->get_cer(),l);
			}
			break;
					
			case 1:
			{
				//如果是只带右操作数的表达式，则遍历右值表达式
				get_var_names(s->get_cer(),l);	
			}
			break;
			
			case 2:
			{
				//如果是括号表达式，则遍历括号中的表达式
				get_var_names(s->get_exp(),l);	
			}
			break;
			
			case 4:
			{
				//如果是变量表达式
				//判断变量是否为一般变量，并且是否为临时变量
				if(0==s->get_cvar()->get_stamp())
				{
					if(!ExtraVarJudge(s->get_cvar()->get_name()))
					{
						//如果是一般的变量，并且不是临时变量，则存储
						struct adj_var a;
						a.name=s->get_cvar()->get_name();
						a.stamp="right";
						add_adj_name(l,a);
					}
				}
				else
				{
					//如果是数组变量
					get_var_names(s->get_cvar()->get_cexp(),l);
					struct adj_var array_ext={ARRAY_EXTR_NAME,"right"};
					add_adj_name(l,array_ext);
				}
			}
			break;
			default:
			{
				//如果是其他表达式，则忽略
			}
		}	
	}
	catch (string str)
	{
		string error_info="get variable names in expression : "+str;
		throw error_info;
	}
}

// 功能：此模块分析循环结构中赋值语句左边的变量，如果不为临时变量，则作为左值存储起来
// 输入参数：s-变量对象指针
// 输出参数：l-待调整变量名链表指针
// 返回值：成功则返回void
// 作者：丁佳，仝明哲
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
			//获取数组下标表达式
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

// 功能：此模块分析循环内部的语句，解析出左值变量和右值变量，并将其存储起来
// 输入参数：s-语句对象指针
// 输出参数：l-待调整变量名链表指针
// 返回值：成功则返回void
// 作者：丁佳，仝明哲
void get_var_names(Statement* s,list<adj_var>* l)
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
					get_var_names(&(*iter),l);//遍历每条语句
				}
			}
			break;	
			case 1:
			{
				//遍历if分支中的语句
				get_var_names(s->get_csstmt()->get_cs_if(),l);

				//判断是否含有else分支
				if(1==s->get_csstmt()->get_stamp())
				{
					//如果含有else分支，则遍历else分支中的语句
					get_var_names(s->get_csstmt()->get_cs_else(),l);
				}
				else
				{
					//如果无else分支，则忽略
				}
			}
			break;	
			case 2:
			{
				//遍历循环体中的语句
				get_var_names(s->get_cistmt()->get_cs(),l);
			}
			break;	
			case 4:
			{
				//为赋值语句
				//遍历赋值表达式的左值变量
				string left_name;
				//左值是否为数组
				//不是数组
				if(s->get_cvar()->get_stamp()==0)
				{
					//先判断赋值表达式是否为函数调用
					if(s->get_ce()->get_stamp()==5)
					{
						//遍历函数参数表达式链表，将其作为右值存储起来
						for(list<Expression>::iterator  iter=s->get_ce()->get_args()->begin();
							   iter!=s->get_ce()->get_args()->end();iter++)
						{
							get_var_names(&(*iter),l);
						}
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
						//如果不为函数调用，则遍历该赋值表达式
						get_var_names(s->get_ce(),l);
					}

					left_name=s->get_cvar()->get_name();//存储左值变量名
					if(!ExtraVarJudge(left_name))
					{
						 get_var_names(s->get_cvar(),l);
					}
				}
				//数组赋值
				else
				{
					//先下标
					get_var_names(s->get_cvar(),l);
					//右值
					get_var_names(s->get_ce(),l);
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



// 功能：从源动态签名表中，将所有变量，以及它们当前的信息拷贝一份到目的动态签名表
// 输入参数：src-源动态签名表指针
// 输出参数：des-目的动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：丁佳、仝明哲
void copy_Var_infos(list<Var_info>* src,list<Var_info>* des)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(src,"source var_info  list's pointer");
		check_pt_arg(des,"destination var_info  list's pointer");
		
		//遍历源动态签名表
		for(list<Var_info>::iterator iter=src->begin();iter!=src->end();iter++)
		{		
			//复制当前迭代器所指的对象，并存入目的链表
			Var_info temp=Var_info(iter->get_name(),iter->get_sigs()->back(),iter->get_scope());
			temp.set_stamp(iter->get_stamp());
			des->push_back(temp);
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="copy var_infos : "+str;
		throw error_info;
	}
}

// 功能：计算入口条件表达式的S1,S2的值，根据表达式操作符的不同，采用不同计算算法
//			S1的值通过参数返回，S2的值通过返回值返回
// 输入参数：sig-条件表达式左值的签名
// 			      k-随机选取的随机数
// 			      op-条件表达式的操作符
//			      stamp-标记是循环入口还是分支入口，为true则为分支入口，为false则为循环入口
// 输出参数：s1-存有S1值的地址
// 返回值：成功则返回一个整型值，失败则抛出错误信息
// 作者：丁佳、仝明哲
int  compute_S1S2(int sig, int k, int* s1,string op,bool stamp)
{
	//判断是不是大于等于操作符
	if(">="==op)
	{
		//如果是，则计算相应的S1,S2
		*s1=(stamp)?(((long long)k * sig ) & (long long)2147483647)%A : sig ;//计算S1
		int s2=(((long long)k * (((long long)sig%A+(K_NUM%A)*(K_NUM%A)%A)%A)) 
					& (long long)2147483647)%A;//计算S2
		return s2;
	}
	//判断是不是小于操作符
	else if("<"==op)
	{
		//如果是，则计算相应的S1,S2
		 int s2=(((long long)k * sig)  & (long long)2147483647)%A;//计算S2
		*s1=(stamp)? (((long long)k * (((long long)sig%A+(K_NUM%A)*(K_NUM%A)%A)%A))
					& (long long)2147483647)%A : 
					(((long long)sig%A+(K_NUM%A)*(K_NUM%A)%A)%A);//计算S1
		return s2;
	}
	//判断是不是等于操作符
   else  if("=="==op)
	{
		//如果是，则计算相应的S1,S2
		 *s1=(stamp)?(((long long)k * sig)  & (long long)2147483647)%A: sig;//计算S1
		int s2=(((long long)k * (((long long)sig+(K_NUM%A))%A))
					& (long long)2147483647)%A;//计算S2
		return s2;
	}
	else if("!="==op)
	{
		//如果是不等于操作，则计算相应的S1,S2
		int s2=(((long long)k * sig)  & (long long)2147483647)%A;//计算S2
		*s1=(stamp)?(((long long)k * (((long long)sig+(K_NUM%A))%A))
		& (long long)2147483647)%A:((long long)sig+K_NUM%A)%A;//计算S1
		return s2;
	}
	else
	{
		//如果不是，则抛出错误信息
		string error_info="compute S1 S2 :  operator is invalid!";
		throw error_info;
	}
}

// 功能：计算入口条件表达式的S1,S2的值，根据表达式操作符的不同，采用不同计算算法
//			S1的值通过参数返回，S2的值通过返回值返回
// 输入参数：SubSig-转成减法后两个减法左值变量的签名
// 			 k-随机选取的随机数
//			 stamp-标记是循环入口还是分支入口，为true则为分支入口，为false则为循环入口
// 输出参数：S1-存有S1值的地址,True分支签名
//          S2-False分支签名
// 返回值：成功则返回一个整型值，失败则抛出错误信息
#define CF_SIG_TEST 0
#if CF_SIG_TEST
int* compute_CFSig_S1S2(int SubSig[],int k,int *S1,bool stamp)
{
	int Prime[2]={P1,P2};
	int TwoPower64ModP[2]={0,0};//2^{2K} Mod P
	TwoPower64ModP[0]=((K_NUM%P1)*(K_NUM%P1))%P1;
	TwoPower64ModP[1]=((K_NUM%P2)*(K_NUM%P2))%P2;

	int SubSig1[6]={P1-1,1,P1-1,1,1,P1-1};
	int SubSig2[6]={1,P1-1,1,P1-1,P1-1,1};
	int Candidate_K[6]={1,1,P1-1,P1-1,1,1};
	bool stamp_arr[6]={true,true,true,true,false,false};

	int *S2=new int[2];
	for(int j=0;j<6;j++)
	{
		SubSig[0]=SubSig1[j];
		SubSig[1]=SubSig2[j];
		k=Candidate_K[j];
		stamp=stamp_arr[j];
		if(!stamp)
			k=1;

		cout<<"Var1 Sig="<<SubSig[0]<<endl;
		cout<<"Var2 Sig="<<SubSig[1]<<endl;
		cout<<"K="<<k<<endl;

		for(int i=0;i<2;i++)
		{
			int R1=((long long)SubSig[0]+(long long)SubSig[1])%Prime[i];
			int R2=((long long)SubSig[0]+(long long)SubSig[1]+(long long)TwoPower64ModP[i])%Prime[i];

			cout<<"Prime is "<<(i+1)<<" R1="<<R1<<endl;
			cout<<"Prime is "<<(i+1)<<" R2="<<R2<<endl;

			S1[i]=((long long)k*(long long)R1)%Prime[i];
			S2[i]=((long long)k*(long long)R2)%Prime[i];
		}

		cout<<"S1[0]="<<S1[0]<<" "<<"S1[1]="<<S1[1]<<endl;
		cout<<"S2[0]="<<S2[0]<<" "<<"S2[1]="<<S2[1]<<endl;
	}


	return S2;
}
#else

int* compute_CFSig_S1S2(int SubSig[],int k,int *S1,bool stamp)
{
	int Prime[2]={P1,P2};
	int TwoPower64ModP[2]={0,0};//2^{2K} Mod P
	TwoPower64ModP[0]=((K_NUM_P1)*(K_NUM%P1))%P1;
	TwoPower64ModP[1]=((K_NUM_P2)*(K_NUM%P2))%P2;

	int *S2=new int[2];
	if(!stamp)
		k=1;
#if TESTING_S1S2
	cout<<"Var1 Sig="<<SubSig[0]<<endl;
	cout<<"Var2 Sig="<<SubSig[1]<<endl;
#endif
	for(int i=0;i<2;i++)
	{
		int R1=((long long)SubSig[0]+(long long)SubSig[1])%Prime[i];
		int R2=((long long)SubSig[0]+(long long)SubSig[1]+(long long)TwoPower64ModP[i])%Prime[i];
#if TESTING_S1S2
		cout<<"Prime is "<<(i+1)<<" R1="<<R1<<endl;
		cout<<"Prime is "<<(i+1)<<" R2="<<R2<<endl;
#endif
		S1[i]=((long long)k*(long long)R1)%Prime[i];
		S2[i]=((long long)k*(long long)R2)%Prime[i];
	}
#if TESTING_S1S2
	cout<<"S1[0]="<<S1[0]<<" "<<"S1[1]="<<S1[1]<<endl;
	cout<<"S2[0]="<<S2[0]<<" "<<"S2[1]="<<S2[1]<<endl;
#endif

	return S2;
}
#endif



//判断分支、循环的入口条件表达式是否合法
int check_condition_entrance_exp(Expression *s)
{
	int flag=0;
	try
	{
//		if(0==s->get_stamp() && 4==s->get_cel()->get_stamp()
//			&& 3== s->get_cer()->get_stamp() && "0"==s->get_cer()->get_c()
//			&& (">="==s->get_op() || "<"==s->get_op() || "=="==s->get_op() || "!="==s->get_op()))
//		{
//			flag=0;
//		}
//		else
//		{
//			//如果入口条件表达式非法，则抛出错误信息
//			string error_info="the entrance expression  is invalid! ";
//			throw error_info;
//		}

		if(s->get_stamp()==0 && s->get_cel()->get_stamp()==4
			&& s->get_cer()->get_stamp()==3 && s->get_cer()->get_c()=="e_TRUE")
		{
			flag=1;
		}
		else
		{
			//如果入口条件表达式非法，则抛出错误信息
			string error_info="the entrance expression  is invalid! ";
			throw error_info;
		}
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding entrance expression : "+str;
		throw error_info;
	}
}
//校验域调整语句输出，适用于普通变量： A=(A+C)%P的形式
//适用于数组附加变量，F_VCL_ArrayCheckUpdate(C1,C2)的形式
//普通变量：函数输出两条语句，对应校验域1和校验域2
//数组附加变量：输出一条语句。
vector<string> var_check_adjust_stmt(string Var,int *Constants)
{
	string CheckField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string PrimeStr[2]={"P1","P2"};

	vector<string> stmt;
	if(Var!=ARRAY_EXTR_NAME)
	{
		for(int i=0;i<2;i++)
		{
			string AdjVarName=TmpVarJudge(Var)? Var+CheckField[i]:"TC_"+Var+CheckField[i];
			string adj_opexp=BuildOperationExp_str(AdjVarName,"+",PrimeStr[i],
					AdjVarName.c_str(),string(int2HexStr(Constants[i])).c_str());
			adj_opexp=OutIndentStr(indent_num)+adj_opexp;
			stmt.push_back(adj_opexp);
		}
	}
	else
	{
		string ArrayAdjustFuncCall=BuildOutputFunc_str("",FUNC_ARRAYEXT_CHK_UPDATE,
				string(int2HexStr(Constants[0])).c_str(),string(int2HexStr(Constants[1])).c_str(),NULL);
		ArrayAdjustFuncCall=OutIndentStr(indent_num)+ArrayAdjustFuncCall.substr(3);
		stmt.push_back(ArrayAdjustFuncCall);
	}

	return stmt;
}

