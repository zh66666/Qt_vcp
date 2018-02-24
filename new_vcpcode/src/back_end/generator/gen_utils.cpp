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

// 定义在文件 : src/main_utils.c
extern int A;
extern int indent_num;
extern ifstream fin;
extern void out_indent(int num, ofstream& out);
extern void check_pt_arg(void *pt, string info);
extern void check_str_arg(string arg_name, string info);
extern int getConstantSig(int constant);
static int sig_index = 1; //局部静态变量   存储签名索引   闪闪



//定义在文件：src/generator/gen_op/gen_relationcmp.cpp
extern void PreCalRelationCmpSig();

ofstream gen_out;	//冗余代码输出文件流
int iteration_level = -1;	//记录循环语句的层数
int select_level = -1;	//记录分支语句的层数
int dD_num = 0;	//dD的下标
long long K_NUM = 4294967296ll;	//2^32
#if BIT64

long long K_NUM_P1 =1106004ll; //P1:12970357 K为2^64   因为2^64无法表示 所以用了2^64模P后的值
long long K_NUM_P2 =799556ll; //P2:12239417 K为2^64

long long i_temp_P1=8224836ll;//求2^65在P1中逆 8224836  ，2^33与2^32的逆相乘取模P1  (4213657*8427314)%P1
long long i_temp_P2=11711720ll;//求2^65在P2中逆  11711720   ，2^33与2^32的逆相乘取模P2 (4949054*9898108)%P1

#else

long long i_temp_P1=4213657ll;//求2^33在P1中逆  4213657
long long i_temp_P2=4949054ll;//求2^33在P2中逆  4949054

long long K_NUM_P1=4294967296ll;//32位
long long K_NUM_P2=4294967296ll;//32位

#endif
string var_scope = "global";	//记录编码过程中所处的作用域

int Int32TmpVarCnt=0;//int 32类型的临时变量计数器
//时间标签参数
int TimeStampInit;//时间标签初始值
int DeltaTimeStamp;	//时间标签增量

//布尔签名值
int Bool_True_Sig;
int Bool_False_Sig;
//关系运算相关的签名值
int Diff1Sig[6];//记录差值签名 Diff1
int Diff2Sig[6];//记录差值签名Diff2
int CalOutSigK[6];//计算分支出口签名的K值
int VarSig1[6];//True分支中输出变量签名
int VarSig2[6];//False分支中输出变量签名

// 功能：从签名文件中读取签名，如果读到文件结尾，则抛出错误
// 参数：无
// 返回值：成功则返回读到的签名值，失败则抛出错误信息
// 作者：李刚
int get_sig()
{
	//检查全局文件流是否已经关联
	if (0 == fin)
	{
		//如果文件流为空，说明文件没有打开，抛出错误信息
		string error_info = "read signature  : signature file is not opened! ";
		throw error_info;
	}
	else
	{
		//如果文件流不为空，说明文件已经打开，则读取
		//判断是否已经到了文件的末尾
		if (!fin.eof())
		{
			//如果没有到文件末尾，则读签名
			int sig = 1;		//存储读到的签名
			fin >> sig;
			return sig;
		}
		else
		{
			//如果到了文件的末尾，则抛出错误信息
			string error_info = "read signature : it is end of the file!";
			throw error_info;
		}
	}
}

/*功能：从签名变中顺序读取签名，标记签名下标
 * 参数：索引指针 index
 返回值：成功则返回读到的签名，并更新静态变量签名索引值，失败则报出错信息
 作者：闪闪*/
int get_sig_index(int *index)
{
	//检查全局文件流是否已经关联
	if (0 == fin)
	{
		//如果文件流为空，说明文件没有打开，抛出错误信息
		string error_info = "read signature  : signature file is not opened! ";
		throw error_info;
	}
	else
	{
		//如果文件流不为空，说明文件已经打开，则读取
		//判断是否已经到了文件的末尾
		if (!fin.eof())
		{
			//如果没有到文件末尾，则读签名
			int sig = 1;		//存储读到的签名
			fin >> sig;
			if (index != NULL)
				*index = sig_index;		//指针存储签名索引

			sig_index++;		//签名索引下移
			return sig;
		}
		else
		{
			//如果到了文件的末尾，则抛出错误信息
			string error_info = "read signature : it is end of the file!";
			throw error_info;
		}
	}

}

// 功能：为动态签名表中的某个变量添加新签名，根据变量名及当前位于的作用域来检索该变量的签名栈，
//           在栈中加入新签名。
// 输入参数：s-动态签名表指针
// 		         str-变量名称
//                 sig-变量新签名
//                 scope-变量所在的作用域
// 输出参数：s-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void add_new_sig(list<Var_info>* s, string str, int sig, string scope)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s, "var_info list's pointer");
		check_str_arg(str, "variable's name");
		check_str_arg(scope, "variable's scope");

		//查找动态签名表，更新签名
		list<Var_info>::iterator iter;		//动态签名表迭代器
		for (iter = s->begin(); iter != s->end(); ++iter)
		{
			//判断当前迭代器所指对象的名字和作用域是否与要查找变量的一致
			if (scope == iter->get_scope() && str == iter->get_name())
			{
				//如果一致，则将新签名加入，同时返回
				iter->add_sigs(sig);
				return;
			}
			else
			{
				//如果不一致，则继续向后查找
			}
		}
		//如果没有找到，将作用域换成"global"再查找一次
		for (iter = s->begin(); iter != s->end(); ++iter)
		{
			//判断当前迭代器所指对象的名字是否和要查找的变量名一致，且作用域为"global"
			if ("global" == iter->get_scope() && str == iter->get_name())
			{
				//如果一致，将新签名加入，同时返回
				iter->add_sigs(sig);
				return;
			}
			else
			{
				//如果不一致，则继续向后查找
			}
		}
		//查找中没有跳出，说明没有找到该变量，抛出错误
		string error_info = "can not find varibale : " + str;
		throw error_info;
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "add new signature : " + str;
		throw error_info;
	}
}

// 功能：在动态签名表中添加新变量，包括变量名，初始签名，作用域和变量标签信息。如果在动态签名表
//           中，发现此作用域中此变量已经存在，则抛出错误
// 输入参数：s-动态签名表指针
//                  str-要增加的变量名
//                  sig-该变量的初始签名
//                  scope-该变量声明的作用域
//                  stamp-该变量的标签信息
// 输出参数：s-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void add_new_vars(list<Var_info>* s, string str, int sig, string scope,
		string stamp,int sigIndex)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s, "var_info list's pointer");
		check_str_arg(str, "variable's name");
		check_str_arg(scope, "variable's scope");
		check_str_arg(stamp, "variable's stamp");

		list<Var_info>::iterator iter;		//动态签名表迭代器
		for (iter = s->begin(); iter != s->end(); ++iter)
		{
			//判断当前迭代器所指对象的名字和作用域是否与要查找变量的一致
			if (scope == iter->get_scope() && str == iter->get_name())
			{
				//如果一致，说明该变量已经存在，则抛出错误信息
				string error_info = str + " has already exists in the scope : "
						+ scope;
				throw error_info;
			}
			else
			{
				//如果不一致，则继续向后查找
			}
		}

		//查找结束后，判断迭代器是否到了链表的末端
		if (iter == s->end())
		{
			//如果到了末端，则说明动态签名表中不存在该变量，则插入该变量的信息
			Var_info* var_info = new Var_info(str, sig, scope);		//构造该变量信息对象
			var_info->set_stamp(stamp);		//设置信息对象的标签信息
			var_info->set_sigIndex(sigIndex);
			s->push_back(*var_info);		//将含有变量信息的对象加入动态签名表
			delete var_info;
		}
		else
		{
			//如果未到末端，则忽略
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "add new variable : " + str;
		throw error_info;
	}
}

// 功能：在布尔真假值链表中，根据布尔量的名字查找其真假值，并根据参数来返回真值或者假值。如果
//           没有找到，则返回固定的真值或者假值
// 输入参数：name-布尔量的名字
//                  bool_vars-存放有布尔真假值信息的链表的指针
//                  stamp-表明查找的是真值还是假值，如果为0，则查找假值，为1，则查找真值
// 返回值：成功则返回布尔的真假值，失败则抛出错误信息
// 作者：李刚
int get_bool_value(string name, list<Bool_var>* bool_vars, int stamp)
{
	try
	{
		//检查参数：布尔真假值链表指针的有效性
		check_pt_arg(bool_vars, "bool value list's pointer");
		//检查参数：布尔变量名字符串的有效性
		check_str_arg(name, "variable's name");
		int value = 0;		//存储要返回的真值或者假值
		list<Bool_var>::iterator iter;		//布尔真假值链表迭代器
		for (iter = bool_vars->begin(); iter != bool_vars->end(); iter++)
		{
			//判断当前迭代器所指的对象的名字是否与要查找的布尔量名称一致
			if (iter->get_name() == name)
			{
				//如果一致，说明存在该布尔变量，根据stamp的值取出真值或者假值
				value = (0 == stamp) ? (iter->get_f()) : (iter->get_t());
				break;
			}
			else
			{
				//如果不一致，则继续查找
			}
		}
		//查找结束后，判断迭代器是否到了链表末端
		if (iter == bool_vars->end())
		{
			//如果到了末端，说明链表中没有该布尔量，即没有为该布尔量指定真假值，则返回一个固定的真值
			//或者假值，并且将此真假值作为该布尔量的真假值存入链表中
			value = (0 == stamp) ? (BOOL_F) : (BOOL_T);

			Bool_var* b_v = new Bool_var(name, var_scope, BOOL_T, BOOL_F);
			bool_vars->push_back(*b_v);
			delete b_v;
		}
		return value;
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "get bool  value : " + str;
		throw error_info;
	}
}

// 功能：在布尔真假值签名链表中，根据布尔量的名字查找其真假值的签名，并根据参数来选择返回真值
//          的签名还是假值的签名。如果没有找到，则分配新签名同时返回
// 输入参数：name-布尔量的名字
//                  bool_sigs-存放有布尔真假值签名信息的链表的指针
//                  stamp-表明查找的是真值签名还是假值签名，如果为0，则查找假值的签名，为1则查找真值签名
// 返回值：成功则返回布尔的真假值签名，失败则抛出错误信息
// 作者：李刚
int get_bool_sig(string name, list<Bool_var>* bool_sigs, int stamp)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(bool_sigs, "bool signature list's pointer");//检查参数：布尔真假值签名链表指针的有效性
		check_str_arg(name, "variable's name");			//检查参数：布尔变量名字符串的有效性

		int sig = 0;			//记录要返回的签名值
		list<Bool_var>::iterator iter;			//布尔真假值签名链表迭代器
		//遍历布尔真假值签名链表
		for (iter = bool_sigs->begin(); iter != bool_sigs->end(); iter++)
		{
			//判断当前迭代器所指的对象的名字是否与要查找的布尔量名称一致
			if (iter->get_name() == name)
			{
				//如果一致，说明存在该布尔变量，根据传入的标签值来取出真值签名或者假值签名
				sig = (0 == stamp) ? (iter->get_f()) : (iter->get_t());
				break;
			}
			else
			{
				//如果不一致，则继续遍历
			}
		}
		//退出循环后，判断迭代器是否指向了链表的末端
		if (bool_sigs->end() == iter)
		{
			//如果到了链表的末端，说明没有找到该布尔量，说明为临时布尔量，为其分配随机真假值签名：
			//将新分配的真假值签名信息存入真假值签名链表
			int t_sig = get_sig();			//分配真值签名
			int f_sig = get_sig();			//分配假值签名
			Bool_var* b_v = new Bool_var(name, "global", t_sig, f_sig);
			bool_sigs->push_back(*b_v);
			delete b_v;

			sig = (0 == stamp) ? (f_sig) : (t_sig);
		}

		return sig;
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "get bool  signature : " + str;
		throw error_info;
	}
}

//判断变量名是否是临时变量---此临时变量是拆分过程中新增的临时变量
//包括：1、语句拆分附加变量
//2、数组附加变量
//3、全局变量绑定的局部变量
//此类变量编码时不需要加上前缀"TC_"
//返回True:是临时变量 不需要加"TC_"
//返回False：不是临时变量，需要加"TC_"
bool TmpVarJudge(string VarName)
{
	return 	VarName==ARRAY_EXTR_NAME || VarName.substr(0,::VOID_FUNC_RETVAL.size()) == ::VOID_FUNC_RETVAL ||
			ExtraVarJudge(VarName) || is_canbe_convert2global(VarName,var_scope);
}

//判断变量名是否是附加变量---此附加变量包括
//1、语句拆分过程中新增的变量(语句拆分附加变量)
//这些附加变量不需要在分支、循环中进行补偿
//True-是附加变量，不需要补偿
//False-不是附加变量，需要补偿
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

//为编码的相关参数分配签名
//包括：布尔值签名
//关系运算编码相关的签名值
void AssignCodeSig()
{
	//时间标签参数
	TimeStampInit=0;
	DeltaTimeStamp=get_sig_index(NULL);

	//布尔值参数
	Bool_True_Sig=get_sig_index(NULL);
	Bool_False_Sig=get_sig_index(NULL);

	//立即数0的签名
	int Const_0_Sig=getConstantSig(0);

	//关系运算相关的签名值
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

