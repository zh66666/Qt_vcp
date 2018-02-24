#include "CodedVarBuild_Util.h"
#include "expression.h"
#include "var.h"
#include "statement.h"


/*************2014-12添加无返回值函数强制返回的值**************************/
const int VOID_RET_VALUE = 0;
/**************************************************************/

int if_while_deep=0;//记录当前中间结构中分支和循环嵌套的的层数
int while_deep_num=0;//记录当前中间结构中循环嵌套的层数
int if_deep=0;//记录当前中间结构中分支嵌套的层数
int temp_var_num=0;//记录对当前中间结构进行拆分时用到的普通临时变量的个数
int tempcond_var_num=0;//记录对当前中间结构进行拆分时用到的条件临时变量的个数

vector<int>* tmpconds=new vector<int>;//记录对每个条件表达式拆分后条件临时变量的个数


//定义在文件 ：./split_exp.cpp
extern void split_exp_start(Expression* s,list<Statement>* l);
extern Expression *split_exp(Expression* s,list<Statement>* l);
extern Expression *split_sub_exp(Expression* s,list<Statement>* l);
extern void modify_sub_exp_II(Expression* s);
extern void modify_sub_exp_I(Expression* s);
//定义在文件 ：src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern void check_pt_arg(void *pt,string info);
extern string int2str(int num);
extern int str2int(string name);

/*****2014-12增加****************/
list<string> tmp_var_style;//临时变量类型

string CurFuncNameHash;//由函数哈希得到的临时变量前缀

string CurSplitFuncName;//当前拆分语句时的所在函数名

string SplitArithmTmpVarPrefix;//拆分表达式时，临时变量前缀，既包括算术运算(包括数组)临时变量，也包括逻辑运算临时变量，但不包括条件表达式的临时变量
string SplitCondTmpVarPrefix;//拆分表达式时，条件表达式拆分时临时变量前缀
string CondSubVarPrefix;//逻辑值减去TRUE的前缀

int IfTmpBoolCnt;//当前函数下分支语句的组合条件表达式的逻辑运算的结果
int IfBoolSubCnt;//分支语句减去TRUE值的差值的计数器

int WhileTmpBoolCnt;//当前函数下循环语句的组合条件表达式的逻辑运算的结果
int WhileBoolSubCnt;//循环语句减去TRUE值的差值的计数器

int TmpRelationBoolCnt;//当前函数下分支语句条件判断表达式中，关系运算的布尔值结果计数器

int *SplitCondTmpVarCntPtr;//条件表达式临时变量索引计数器指针
int *CondSubVarCntPtr;//逻辑值减去TRUE的计数器指针

int WhileAC_AB_Cnt;//While语句累加器变量计数器

int *SplitArithmTmpVarCntPtr;//算术运算(包括数组)、逻辑运算表达式临时变量计数器指针
int ACIntTmpCnt;//算术运算表达式计数器 有符号数
int ACUIntTmpCnt;//算术运算表达式计数器 无符号数

int void_func_counts;//无返回值函数强制返回的索引值


// 功能：判断表达式的类型，如果该表达式为普通变量表达式或者常数表达式，则返回0，否则其他类型
// 			均返回1
// 输入参数：s-表达式对象指针
// 返回值：成功则返回int类型的值，失败则抛出错误信息
// 作者：李刚
int  judge_exp_type(Expression* s)
{
	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(s,"expression object's pointer");
		
		int stamp = s->get_stamp();//取出该表达式的标签值
		// 判断表达式是否为普通变量表达式，或者是常数表达式
		if((4==stamp && 0==s->get_cvar()->get_stamp() )||  3==stamp)
		{
			//如果是，则返回0
			return 0;
		}
		else
		{
			//如果不是，则返回1
			return 1;
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="judge expression's type : "+str;
		throw error_info;
	}
}

// 功能：根据传入的表达式，构造一条新的临时变量赋值语句，并返回，同时将传入的表达式
//			修改为临时变量表达式
// 输入参数：exp-表达式对象指针
// 			      str-临时变量名字
// 			      num-当前临时变量使用的个数
// 返回值：成功则返回语句对象指针，失败则抛出错误信息
// 作者：李刚
// 注意：本函数里使用new为语句分配了内存，需要上层函数来释放
Statement* generate_newstmt(Expression* exp,string str,int* num)
{
	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(exp,"expression object's pointer");
		
		Var* new_var=new Var(str+"_"+int2str(*num),::tmp_var_style);//构造临时变量对象
		Expression *temp_exp=new Expression(*exp);//拷贝传入的表达式对象
		Statement* new_stmt=new Statement(new_var,temp_exp);//构造临时变量语句
		Expression *new_exp=new Expression(new_var);//使用临时变量构造新表达式
		*exp=*new_exp;//将传入的表达式设置成临时变量表达式
		
		delete new_exp;
		return new_stmt;
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="generate a new statement : "+str;
		throw error_info;
	}
}

// 功能：拆分变量，分析变量，如果是数组变量，则分析其下标表达式，对其下标进行拆分
// 输入参数：s-变量对象指针
// 输出参数：s-变量对象指针，l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void split_var(Var* s,list<Statement>* l)
{
	try
	{
		//检查参数：变量对象指针的有效性
		check_pt_arg(s,"variable object's pointer");
		//检查参数：语句链表指针的有效性
		check_pt_arg(l,"statement list's pointer");
		//判断该变量是否为数组变量
		if(1==s->get_stamp())//array
		{
			//modify_sub_exp_I(s->get_cexp());
			//modify_sub_exp_II(s->get_cexp());
			//如果是数组变量，则遍历下标表达式
			Expression *array_sub=split_sub_exp(s->get_cexp(),l);//拆分下标表达式
			
			*s->get_cexp()=*array_sub;
			delete array_sub;
		}
		else
		{
			//如果不是数组变量，则忽略
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable : "+str;
		throw error_info;
	}
}

// 功能：对函数调用表达式中的实参表达式链表进行遍历，对每个实参表达式进行拆分
// 输入参数：s-表达式链表指针
// 输出参数：s-表达式链表指针，l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void split_args(list<Expression>* s,list<Statement>* l)
{
	try
	{
		//检查参数：表达式链表指针的有效性
		check_pt_arg(s,"expression list's pointer");
		//检查参数：语句链表指针的有效性
		check_pt_arg(l,"statement list's pointer");
		
		//存储每个表达式经过拆分生成的语句
		list<Statement>* new_stmts= new list<Statement>;
		//遍历表达式链表
		for(list<Expression>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			//设置临时变量拆分的工具参数
			set_tmpvar_split_utils_exp(&(*iter));
			//对当前迭代器所指的表达式进行拆分
			Expression *ArgsExp=split_exp(&(*iter),new_stmts);
			//修改函数实参表达式
			*iter=*ArgsExp;

			//将拆分生成的语句插入到通过参数传入的语句链表中
			l->insert(l->end(),new_stmts->begin(),new_stmts->end());
			new_stmts->clear();

			delete ArgsExp;
		}
		delete new_stmts;
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan function's arguments : "+str;
		throw error_info;
	}
}

// 功能：对函数调用表达式中的实参表达式链表进行遍历，对每个实参表达式进行拆分
// 输入参数：s-表达式链表指针
// 输出参数：s-表达式链表指针，l-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
void split_args_check(list<Expression>* s,list<Statement>* l)
{
	try
	{
		//检查参数：表达式链表指针的有效性
		check_pt_arg(s,"expression list's pointer");
		//检查参数：语句链表指针的有效性
		check_pt_arg(l,"statement list's pointer");

		//存储每个表达式经过拆分生成的语句
		list<Statement>* new_stmts= new list<Statement>;
		//遍历表达式链表
		for(list<Expression>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			//设置临时变量拆分的工具参数
			set_tmpvar_split_utils_exp(&(*iter));
			//对当前迭代器所指的表达式进行拆分
			split_exp(&(*iter),new_stmts);

			//将拆分生成的语句插入到通过参数传入的语句链表中
			l->insert(l->end(),new_stmts->begin(),new_stmts->end());
			new_stmts->clear();
		}
		delete new_stmts;
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan function's arguments : "+str;
		throw error_info;
	}
}
/*
 * 初始化分支结构中的判断条件变量计数器
 * 初始化临时变量的下标索引
 *
 */
void InitSplitTempVarCnt()
{
	ACIntTmpCnt=0;
	ACUIntTmpCnt=0;

	IfTmpBoolCnt=0;
	IfBoolSubCnt=0;

	WhileTmpBoolCnt=0;
	WhileBoolSubCnt=0;

	TmpRelationBoolCnt=0;

	WhileAC_AB_Cnt=0;

	void_func_counts = 0;//将无赋值函数调用语句的计数器归零
}

/*
 * 临时附加变量命名程序
 * 输入参数：前缀+函数哈希字符串+索引号
 */
string BuildTmpVarName(string __Prefix,string _FuncHash,int VarStyleCnt)
{
	string VarName=__Prefix+int2str(VarStyleCnt);
	return VarName;
}

//获取表达式变量是否为有符号 true-有符号 false-无符号
bool get_tmpvar_style(Var *var)
{
	::tmp_var_style=var->get_var_type();

	list<string>::iterator iter=::tmp_var_style.begin();
	for(;iter!=::tmp_var_style.end();iter++)
	{
		if(*iter=="unsigned" || *iter=="UINT32")
		{
			break;
		}
	}
	if(iter!=::tmp_var_style.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

//获取函数实参表达式是否为有符号 true-有符号 false-无符号
bool get_args_style(Expression *s)
{
	int final_bool=false;
	if(s->get_stamp()==4)
		final_bool=get_tmpvar_style(s->get_cvar());
	else if(s->get_stamp()==2)
		final_bool=get_args_style(s->get_exp());
	else if(s->get_stamp()==1)
		final_bool=get_args_style(s->get_cer());
	else if(s->get_stamp()==0)
	{
		int left_bool=get_args_style(s->get_cel());
		int right_bool=get_args_style(s->get_cer());

		final_bool=left_bool && right_bool;
	}
	else
	{
		final_bool=true;
	}

	return final_bool;
}

//表达式拆分(不包括赋值表达式)时临时变量构建工具
//包括类型：函数实参、函数返回值
void set_tmpvar_split_utils_exp(Expression *s)
{
	if(get_args_style(s))
	{
		SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
	}
	else
	{
		SplitArithmTmpVarCntPtr=&::ACUIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_UINTTMP_PRE;
	}
}

//算术运算拆分时临时变量构建工具
void set_tmpvar_split_utils_arithm(Var *var)
{
	if(get_tmpvar_style(var))
	{
		SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
	}
	else
	{
		SplitArithmTmpVarCntPtr=&::ACUIntTmpCnt;
		SplitArithmTmpVarPrefix=::ARITHM_UINTTMP_PRE;
	}
}

//分支条件表达式拆分时临时变量构建工具
void set_tmpvar_split_utils_select()
{
	CondSubVarPrefix=IF_LOGICSUB_PRE;
	CondSubVarCntPtr=&IfBoolSubCnt;

	SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
	SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
}

//循环条件表达式拆分时临时变量构建
void set_tmpvar_split_utils_while()
{
	CondSubVarPrefix=WHILE_LOGICSUB_PRE;
	CondSubVarCntPtr=&WhileBoolSubCnt;

	SplitArithmTmpVarCntPtr=&::ACIntTmpCnt;
	SplitArithmTmpVarPrefix=::ARITHM_INTTMP_PRE;
}





