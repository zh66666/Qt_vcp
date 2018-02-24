#include "gen_utils.h"
#include "bool_var.h"
#include "var_declaration.h"
#include "initializer.h"
#include "init_declarator.h"
#include "declaration_specifiers.h"
#include "declarator.h"
#include "expression.h"
#include "global_var.h"
#include <map>

//static int index2sig=0;
// 定义在文件 : ../gen_utils.c
extern int get_bool_value(string name, list<Bool_var>* bool_vars, int stamp);

// 定义在文件 : src/main_utils.cpp
extern bool debug_flag;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;
extern list<string>* global_var_names;
extern list<Global_var>* global_vars;
extern map<string, int>* pre_sigs;
extern void check_pt_arg(void *pt, string info);
extern string int2HexStr(const int num);
extern list<string>* datetime_var_list;
// 定义在文件 : src/back_end/back_end_utils.cpp
extern string compute_exp(Expression* s, int* result);
extern const int P1;
extern const int P2;

//设置数组附加变量初始值
//定义在create_extra_files.cpp
extern void SetArrayExtrInitSig(int Sig, int SigIndex);

//IO数组声明输出
static void gen_IO_array_dec(string array_name, int sub);


//记录当前被编码的变量声明的存储类型
static string storage_type = "";

//记录当前被编码的变量声明的noadj标签值，该标签值为1时说明此变量编码时不需要初始化，
//为0时，说明当前编码的变量必须要进行初始化。该值默认初始为-1
static int noadj_stamp = -1;

//记录当前被编码的变量声明的nodt标签值，该标签值为1时说明当前的变量在被编码时不需要加DT，
//为0时，说明当前的变量在被编码时需要加DT。该值默认初始为-1
static int nodt_stamp = -1;

//记录当前被编码的变量声明的bool标签值，该标签值为1时说明当前编码的变量为布尔变量，
//为0时，说明当前编码的变量不是布尔变量。该值默认初始为-1
static int bool_stamp = -1;



extern const string ARRAY_EXTR_NAME;
string storage_typef = "T_CODE";
int arr_check1 = 0;
int arr_check2 = 0;
int init_str = 0; //附加变量初始值
// 功能：遍历初始化链表，将其中的每个表达式的值计算出来，并存入指定的链表中。
// 输入参数：l-初始化链表指针
// 输出参数：v_tmp-用来存储计算结果的链表指针
// 返回值：执行成功则返回计算结果，计算出来的值存放在输出参数中，失败则抛出错误信息
// 作者：李刚、李夫收
string compute_init_list(list<Initializer>* l, vector<int>* v_tmp)
{
	try
	{
		//检查参数：初始化链表指针的有效性
		check_pt_arg(l, "initializer list's pointer");

		list<Initializer>::iterator iter; //初始化链表迭代器
		for (iter = l->begin(); iter != l->end(); iter++)
		{
			//判断当前迭代器所指的对象是否为简单表达式初始化
			if (0 == iter->get_stamp())
			{
				//如果标签为0，说明是简单表达式初始化
				int value = 0; //存储该表达式的值
				string result = compute_exp(iter->get_exp(), &value); //计算该表达式
				//判断是否计算成功
				if (result == "wrong")
				{
					//如果计算失败，则程序返回wrong
					return "wrong";
				}
				else
				{
					//如果计算成功，则将值存入输出参数中
					v_tmp->push_back(value);
				}
			}
			else
			{
				//如果标签不为0，说明为复杂链表初始化，例如 a[2][3]={{1,2,3},{4,5,6}};
				//递归调用计算初始化链表函数，对复杂链表进行计算
				string result = compute_init_list(iter->get_init_list(), v_tmp);
				//判断该复杂链表有无计算成功
				if (result == "wrong")
				{
					//如果计算失败，则返回wrong
					return "wrong";
				}
				else
				{
					//如果计算成功，则继续遍历
					;
				}
			}
		}
		//初始化链表遍历完毕，并且中间无返回，说明所有元素全部计算成功
		return "right";
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "compute initializer list : " + str;
		throw error_info;
	}
}

// 功能：判断传入的变量是否为布尔变量，如果是，则为其分配真假值签名，并将此布尔真假值签名
//		   信息对象插入指定的链表中；如果不是，则不分配签名
// 输入参数：var_name-布尔变量的名字
// 			      bool_sigs-存储有布尔真假值签名信息的链表指针
//			      stamp-标记是否为布尔变量
// 输出参数：bool_sigs-存储有布尔真假值签名信息的链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、李夫收
static void assign_bool_sigs(string var_name, list<Bool_var>* bool_sigs,
		int stamp)
{
	try
	{
		//检查参数：布尔变量名字符串的有效性
		check_str_arg(var_name, "boolean variable's name");
		//检查参数：布尔真假值签名链表指针的有效性
		check_pt_arg(bool_sigs, "boolean TF signature list's pointer");

		//判断是否为布尔变量
		if (stamp == 1)
		{
			//如果为布尔变量，则为此布尔变量分配真假值签名，并将其插入链表
			int t = get_sig();				//分配真值签名
			int f = get_sig();				//分配假值签名
			//新建一个存储该布尔真假值签名信息的对象
			Bool_var* b_v = new Bool_var(var_name, var_scope, t, f);
			//将该对象插入链表
			bool_sigs->push_back(*b_v);
			delete b_v;

			//如果处于调试模式，则打印调试信息
			/*	if (true == debug_flag) {
			 out_indent(indent_num, gen_out);
			 gen_out << "//boolean varibale " << var_name
			 << "'s true value sig : " << t << ", false value sig : "
			 << f << endl;
			 }*/
		}
		else
		{
			//如果不为布尔变量，则不分配真假值签名
			;
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "assign booean vaiable's TF signatures : " + str;
		throw error_info;
	}
}

//功能：输出带有初始化数组变量的冗余代码，包括高位、校验位、签名标识，为代码美观，每输出2个数组元素信息后就换行

// 输入参数：array_name-数组变量的名称
//                  sub-数组的下标值
//		          v_tmp-存储有数组初始化值的vector的指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：阮闪闪
static void out_array_init(string array_name, int sub, vector<int>* v_tmp)
{
	try
	{
		string comp1, comp2;
		//检查参数：数组名称字符串的有效性
		check_str_arg(array_name, "array's name");
		//检查参数：数组初始化值vector指针的有效性
		check_pt_arg(v_tmp, "array initializer vector's pointer");

		//查找数组变量的签名
		int array_sig = get_current_sig(var_infos, array_name, var_scope);
		//根据作用域来设置变量数组变量低位的DT内容
		string GLOBAL_DT = var_scope == "global" ? "" : " + GLOBAL_DT";
		out_indent(indent_num, gen_out);
		gen_out << storage_typef << " " << "TC_" << array_name << "[" << sub << "]";
		unsigned int k = 0;	//对已经输出的数组元素进行计数
		if ("extern " == storage_type)
		{
			//如果是，则不输出数组的初始化部分
			gen_out << ";" << endl;
		}
		else
		{
			//如果不是，则输出数组的高位初始化部分
			gen_out << " = ";
			unsigned int k = 0;			//对已经输出的数组元素进行计数
			//输出每个元素的值
			for (vector<int>::iterator iter = v_tmp->begin();
					iter != v_tmp->end(); iter++)
			{
				//根据数组变量的签名以及数组元素的值，计算该元素的低位
				int low1 =
						((-((long long) (*iter) * K_NUM_P1) % P1
								+ ((long long) k * K_NUM_P1) % P1 + array_sig)
								% P1 + P1) % P1;
				int low2 =
						((-((long long) (*iter) * K_NUM_P2) % P2
								+ ((long long) k * K_NUM_P2) % P2 + array_sig)
								% P2 + P2) % P2;
				//先判断已输出低位的元素个数是否>=2，且为10的倍数
				if (iter == --v_tmp->end())
				{
					out_indent(indent_num, gen_out);
					out_indent(indent_num, gen_out);
					gen_out << "{{" << *iter << ", {" << low1 << ", " << low2 << "}}, " << array_sig << "}";

				}
				else
				{

					if (k == 0)
					{
						gen_out << "{{{" << *iter << ", {" << low1 << ", " << low2 << "}}, " << array_sig << "}, ";
						gen_out << endl;
					}
					else
					{
						out_indent(indent_num, gen_out);
						out_indent(indent_num, gen_out);
						gen_out << "{{" << *iter << ", {" << low1 << ", " << low2 << "}}, " << array_sig << "}, ";
						gen_out << endl;
					}
				}
				k++;
			}

			gen_out << "};" << endl << endl;
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "out put array's high value with initializers : "
				+ str;
		throw error_info;
	}
}

//功能：输出不带初始化数组变量的冗余代码，包括高位、校验位、签名标识，为代码美观，每输出2个数组元素信息后就换行

// 输入参数：array_name-数组变量的名称
//                  sub-数组的下标值
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：阮闪闪
//无用
static void out_array_init(string array_name, int sub)
{
	out_indent(indent_num, gen_out);
	gen_out << storage_typef << " " << "TC_" << array_name << "[" << sub << "]={0}";
}

// 功能：本函数对带有初始化值的普通变量声明进行编码，根据输入的变量名及其初始化值，
//           输出该变量声明的冗余代码。
// 输入参数：var_name-待编码的变量名字
//			      value-待编码的变量的初始化值
// 返回值：成功则返回void
// 作者：李刚、李夫收
static void out_var_dec(string var_name, int value)
{
	try
	{
		//检查参数：变量名称字符串的有效性
		check_str_arg(var_name, "variable's name");

		//查找该变量的当前签名
		int var_sig = get_current_sig(var_infos, var_name, var_scope);
		//根据变量的初始化值和签名计算其低位值
		int var_low_1 = ((-((long long) value * K_NUM_P1) % P1 + var_sig) % P1
				+ P1) % P1;
		int var_low_2 = ((-((long long) value * K_NUM_P2) % P2 + var_sig) % P2
				+ P2) % P2;
		//判断编码时低位末尾是否要输出"+DT"
//		string DT1 = var_scope == "global" ? "" : "+ g_GS_TL1";
//		string DT2 = var_scope == "global" ? "" : "+ g_GS_TL2";
		string DT1 = "";
		string DT2 = "";
		int sigIndex = get_current_sigIndex(var_infos, var_name, var_scope);
		//根据存储类型是否为extern来设置高位初始化字符串
		if ("extern " != storage_type)
		{
			//index2sig = index2sig + 1;
			string T_CODE_init_str = " = {{" + int2HexStr(value) + ", {"
					+ int2HexStr(var_low_1) + "" + DT1 + ", "
					+ int2HexStr(var_low_2) + "" + DT2 + "}}, "
					+ int2HexStr(sigIndex) + "}";
			//out_indent(indent_num, gen_out);
			if (TmpVarJudge(var_name))
			{
				datetime_var_list->push_back(var_name);
				if (storage_type == "")
//					gen_out << "T_CODE12" << " " << var_name << T_CODE_init_str << ";" << endl;
					{
					out_indent(indent_num,gen_out);
					gen_out << "long long "  << var_name<< "_1" <<"="<<value<<"*A1"<< ";" << endl;
					out_indent(indent_num,gen_out);
					gen_out << "long long " << var_name<< "_2"  <<"="<<value<<"*A2"<< ";" << endl;}
				else
					gen_out << storage_type << " T_CODE" << " " << var_name << T_CODE_init_str << ";" << endl;
			}
			else
			{
				if (var_scope != "global")
					datetime_var_list->push_back("TC_" + var_name);
				if (storage_type == "")
				{	//gen_out << "T_CODE" << " TC_" << var_name << T_CODE_init_str << ";" << endl;
					out_indent(indent_num,gen_out);
					gen_out << "long long "  << var_name<< "_1" <<"="<<value<<"*A1"<< ";" << endl;
					out_indent(indent_num,gen_out);
				gen_out << "long long " << var_name<< "_2"  <<"="<<value<<"*A2"<< ";" << endl;}
				else
					gen_out << storage_type << " T_CODE" << " TC_" << var_name << T_CODE_init_str << ";" << endl;
			}
		}
		else if ("const" == storage_type)
		{
			string T_CODE_init_str = " = {{(" + int2HexStr(value) + "), {("
					+ int2HexStr(var_low_1) + ")" + DT1 + ", ("
					+ int2HexStr(var_low_2) + ")" + DT2 + "}}, ("
					+ int2HexStr(sigIndex) + ")}";
			datetime_var_list->push_back(var_name);
			gen_out << storage_type << " T_CODE" << " TC_" << var_name << T_CODE_init_str << ";" << endl;
			gen_out << "T_CODE" << " AC_BIND_" << var_name << ";" << endl;
		}
		else
		{
			out_indent(indent_num, gen_out);
			if (TmpVarJudge(var_name))
			{
				if (storage_type == "")
					gen_out << "T_CODE" << " " << var_name << "" << ";" << endl;

				else
					gen_out << storage_type << " T_CODE" << " " << var_name << "" << ";" << endl;
			}
			else
			{
				if (storage_type == "")
					gen_out << "T_CODE" << " TC_" << var_name << "" << ";" << endl;

				else
					gen_out << storage_type << " T_CODE" << " TC_" << var_name << "" << ";" << endl;
			}
		}

		//根据存储类型是否为extern来设置低位初始化字符串
		//	string l_init_str=("extern "==storage_type) ? "":" = "+int2str(var_low)+DT1;

		//out_indent(indent_num,gen_out);
		//输出高位的代码
		//gen_out<<storage_type<<str_int32<<" H_"<<var_name<<h_init_str<<";"<<endl;
		/*out_indent(indent_num,gen_out);
		 //输出低位的代码
		 gen_out<<storage_type<<str_int32<<" L_"<<var_name<<l_init_str<<";"<<endl;*/
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info =
				"out put variable declaration's high and low codes with  initializers : "
						+ str;
		throw error_info;
	}
}

// 功能：输出数组附加变量的编码，包括高位和低位的编码。分全局数组和局部数组两种情况。
// 输入参数：array_name-数组名字。
// 返回值：成功则返回void。
// 作者：李刚、李夫收
// 闪闪追加
static void out_array_addvar(string array_name)
{
	try
	{
		//检查参数：数组名称字符串的有效性
		check_str_arg(array_name, "array's name");

		//string add_name="Array_EXTR";//存储附加变量的名称
		int add_sig = get_current_sig(var_infos, ARRAY_EXTR_NAME, var_scope);//获取附加变量的签名
		int init_add_str = 0;
		arr_check1 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig)
				% P1;
		arr_check2 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig)
				% P1;
		// low1=((-((long long)(*iter)*K_NUM_P1)%p1+((long long)k*K_NUM_P1)%p1+array_sig)%p1-p1)%p1;
		out_indent(indent_num, gen_out);
		gen_out << storage_typef << " " << ARRAY_EXTR_NAME << "={" << init_str << ",{" << arr_check1 << "," << arr_check2 << "}," << add_sig << "};" << endl;

	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "out put array's addtional variable  : " + str;
		throw error_info;
	}
}
void store_addvar(int add_sig)
{

	int init_add_str = 0;
	arr_check1 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig) % P1;
	arr_check2 = (-((long long) init_add_str * K_NUM_P1) % P1 + add_sig) % P1;
}

// 功能：输出不带初始化的数组变量声明的冗余代码，包括其高位和低位的冗余代码，以及
//			附加变量的冗余代码，如果该数组变量不是noadj变量，则要在冗余代码中对其初始化
// 输入参数：array_name-数组变量名
//			      sub-数组的下标
// 返回值：成功则返回void
// 作者：李刚、李夫收
static void out_array_redcode(string array_name, int sub)
{
	try
	{
		//检查参数：数组名称字符串的有效性
		check_str_arg(array_name, "array's name");

		vector<int>* init_value = new vector<int>;			//用于存储数组元素初始化的值
		for (int i = 0; i < sub; i++)
		{
			init_value->push_back(0);
		}
		out_array_init(array_name, sub, init_value);

	} catch (string str)
	{
		//判断数组变量是否为nodt数组变量，进而确定是否输出附加变量的编码
		if (0 == nodt_stamp)
		{
			//如果不是nodt数组变量，则需要输出其附加变量的编码
			//闪闪  附加变量在赋值时输出
			out_array_addvar(array_name);
		}
		else
		{
			//如果是nodt数组变量，则无附加变量
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info =
				"out put array declaration's vital codes with no initializers : "
						+ str;
		throw error_info;
	}
}

// 功能：输出带初始化的数组变量声明的冗余代码，包括其高位和低位的冗余代码，以及
//			附加变量的冗余代码
// 输入参数：array_name-数组变量名
//			      sub-数组的下标
//			      v_tmp-存储有数组元素初始化值的vector指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、李夫收
static void out_array_redcode(string array_name, int sub, vector<int>* v_tmp)
{
	try
	{
		//检查参数：数组名称字符串的有效性
		check_str_arg(array_name, "array's name");
		//检查参数：数组初始化值vector指针的有效性
		check_pt_arg(v_tmp, "array initializer vector's pointer");

		//out_array_high(array_name,sub,v_tmp);//输出数组变量声明的高位

		//如果该数组变量为全局变量且不为noadj变量
		int init_num = v_tmp->size();				//记录被初始化的数组元素的个数

		//2010年3月9日追加
		//判断初始化是否符合C子集规范
		if (init_num > sub)
		{
			//如果初始化个数超过了下标，报错
			string error_info = "initializers is more than declaration!";
			throw error_info;
		}

		if (init_num < sub)
		{
			//如果未完全初始化，报错
			string error_info = "initializers is less than declaration!";
			throw error_info;
		}
		if (sub <= 5000 && sub > init_num)
		{
			//如果下标值<=5000，并且数组元素未初始化完全，则将没有初始化的元素初始化为0
			for (int i = 0; i < sub - init_num; i++)
			{
				v_tmp->push_back(0);
			}
		}
		else
		{
			//如果数组下标>5000，或者<5000但是所有元素均以初始化时,则不修改vector中的内容
			;
		}
		//out_array_low(array_name,sub,v_tmp);//输出数组变量声明的低位
		out_array_init(array_name, sub, v_tmp);		//输出数组变量初始化结构体信息
		//判断数组变量是否为nodt数组变量，进而确定是否输出附加变量的编码
		if (0 == nodt_stamp)
		{
			//如果不是nodt数组变量，则需要输出其附加变量的编码

		}
		else
		{
			//如果是nodt数组变量，则无附加变量
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info =
				"out put array declaration's vital codes with initializers : "
						+ str;
		throw error_info;
	}
}

// 功能：此模块为变量声明冗余代码生成模块的初始化模块，根据传入的变量声明对象，分析该对象，
// 			设置本模块中的一些全局变量的值。
// 输入参数：v_d-变量声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、李夫收
static void init_gen_vardec(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d, "variable declaration object's pointer");

		//设置全局变量存储类型的值
		storage_type = v_d->get_decl_spec()->get_stor_spec();
		//判断存储类型是否为"extern"或者"static"
		if (storage_type == "extern")
		{
			//如果是，则加上一个空格
			storage_type = storage_type + " ";
		}
		else
		{
			//如果不是，则赋空值
			storage_type = "";
		}

		noadj_stamp = v_d->get_noadj_stamp();		//设置noadj标签的值
		bool_stamp = v_d->get_bool_stamp();		//设置bool标签的值
		nodt_stamp = v_d->get_nodt_stamp();		//设置nodt标签的值
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "initializing in coding variable declaration : "
				+ str;
		throw error_info;
	}
}

// 功能：为变量分配签名，并存入动态签名表中。包括对一般变量和数组变量的签名分配，局部变量
// 			和全局变量的签名分配。如果该变量之前已经遇到过，则返回的签名值为-1。
// 输入参数：name-变量名称
// 			      var_stamp-该变量的标签信息
// 返回值：成功则返回所分配的签名值，失败则抛出错误信息
// 作者：李刚、李夫收
int assign_var_sig(string name, string var_stamp)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(name, "variable's name");
		check_str_arg(var_stamp, "variable's stamp");

		int sig = -1;		//存储该变量的签名，初始化为-1
		//判断当前作用域是否在全局
		if (var_scope == "global")
		{
			//如果在全局，先在全局变量名链表中查找该变量的名字
			//判断是否在全局变量名链表中找到了该变量
			if (global_var_names->end()
					== find(global_var_names->begin(), global_var_names->end(),
							name))
			{
				//如果没有找到，说明是第一次遇到该全局变量，将其名字加入全局变量名链表
				global_var_names->push_back(name);

				//在预配置签名链表中查找该全局变量，看是否为其预置了签名
				map<string, int>::iterator it = pre_sigs->find(name);
				int sigIndex = -1;
				//判断查找函数返回的迭代器是否到了链表的末端
				if (pre_sigs->end() == it)
				{
					//如果到了链表的末端，则说明该全局变量没有预置签名
					sig = get_sig_index(&sigIndex);				//为其分配一个签名
				}
				else
				{
					//如果没到链表的末端，说明该全局变量有预置签名
					sig = it->second;			//将预置签名作为其签名
				}
				//将该全局变量插入动态签名表
				add_new_vars(var_infos, name, sig, var_scope, var_stamp,
						sigIndex);

				//如果处于调试模式，则输出调试信息
				/*if (true == debug_flag) {
				 out_indent(indent_num, gen_out);
				 gen_out << "//variable " << name << "'s sig : " << sig
				 << endl;
				 }*/

				//判断该变量是否是布尔变量，并为其分配真假值签名
				assign_bool_sigs(name, bool_sigs, bool_stamp);
			}
			else
			{
				//如果找到了，说明之前遇到过此全局变量，则不再分配签名
				//返回值为-1 的签名
				return sig;
			}
		}
		else
		{
			//当前作用域不是全局，则说名当前扫描到的变量声明为局部变量声明
			//为该变量分配签名，并将该变量加入动态签名表中
			int sigIndex = -1;
			sig = get_sig_index(&sigIndex);
			add_new_vars(var_infos, name, sig, var_scope, var_stamp, sigIndex);
			//判断该变量是否是布尔变量，并为其分配真假值签名
			assign_bool_sigs(name, bool_sigs, bool_stamp);
		}
		return sig;
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "assign signatures for variable declaration : "
				+ str;
		throw error_info;
	}
}

// 功能：对源程序中的普通变量声明进行编码，包括变量签名的分配，冗余代码的输出。
// 输入参数：var_name-待编码的变量的名称
//			      value-如果该变量声明时的初始化值
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、李夫收
static void gen_common_dec(string var_name, int value)
{
	try
	{
		//检查参数：变量名字符串的有效性
		check_str_arg(var_name, "variable's name");

		int var_sig = assign_var_sig(var_name, "common");			//先为变量分配签名

		//判断当前作用域是否为全局，是否为首次遇到该变量
		if (var_scope == "global" && var_sig != -1)
		{
			//如果在全局，并且是首次扫描到该变量声明
			//构造Global_var对象，并将其插入全局Global_var链表中
			Global_var* new_gv = new Global_var(var_name, value, var_sig, 1);
			//如果此变量为noadj类型变量，则将对象adj标签置为"noadj"
			if (1 == noadj_stamp)
			{
				new_gv->set_adj_stamp("noadj");
			}
			global_vars->push_back(*new_gv);
			delete new_gv;

			//调用带初始化的变量声明编码函数进行编码
			out_var_dec(var_name, value);
		}
		else
		{
			//当前作用域不是全局，或者不是首次遇到该变量，则
			//不构造Global_var对象，直接输出冗余代码
			out_var_dec(var_name, value);
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding  common variable declaration : " + str;
		throw error_info;
	}
}

//为附加变量分配签名，附加变量为全局变量
//允许同一作用域内，某变量不断分配签名
//闪闪追加 2015/3/16
int assign_extra_sig(string name, string var_stamp)
{
	//检查输入参数的有效性
	check_str_arg(name, "variable's name");
	check_str_arg(var_stamp, "variable's stamp");
	if (global_var_names->end()
			== find(global_var_names->begin(), global_var_names->end(), name))
	{
		//如果没有找到，说明是第一次遇到该全局变量，将其名字加入全局变量名链表
		global_var_names->push_back(name);
		//在预配置签名链表中查找该全局变量，看是否为其预置了签名
		map<string, int>::iterator it = pre_sigs->find(name);
		int sig = -1;		//存储该变量的签名，初始化为-1
		int sig_index = -1;
		//判断查找函数返回的迭代器是否到了链表的末端
		if (pre_sigs->end() == it)
		{
			//如果到了链表的末端，则说明该全局变量没有预置签名
			sig = get_sig_index(&sig_index);				//为其分配一个签名
//计算校验域
			int init_add_str = 0;
			arr_check1 = (-((long long) init_add_str * K_NUM_P1) % P1 + sig)
					% P1;
			arr_check2 = (-((long long) init_add_str * K_NUM_P1) % P1 + sig)
					% P1;

			if(name == ARRAY_EXTR_NAME)
			{
				SetArrayExtrInitSig(sig, sig_index);
			}
			/*out_indent(indent_num, gen_out);
			 gen_out << "//" << name << " : (0," << arr_check1 << ","
			 << arr_check2 << "," << sig << ");" << endl;*/

		}
		else
		{
			//如果没到链表的末端，说明该全局变量有预置签名
			sig = it->second;			//将预置签名作为其签名
		}
		//将该全局变量插入动态签名表
		add_new_vars(var_infos, name, sig, "global", var_stamp, sig_index);

		//如果处于调试模式，则输出调试信息
		/*if (true == debug_flag) {
		 out_indent(indent_num, gen_out);
		 gen_out << "//variable " << name << "'s sig : " << sig << endl;
		 }*/
		return sig;
	}
}

// 功能：生成数组变量声明的冗余代码，其中包括对数组及其附加变量签名的分配，高低位的编码，
// 			本函数能够对源码中带有初始化和不带初始化的数组声明进行编码，根据传入的bool参数来区分
// 输入参数：array_name-数组变量名
// 			      sub-数组的下标
// 			      init_values-数组声明带有初始化时，存放元素初始化值的vector指针
// 			      non_init-标记该数组声明是否带有初始化的布尔标签
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、李夫收
static void gen_array_dec(string array_name, int sub, vector<int>* init_values,
		bool non_init)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(array_name, "array's name");
		check_pt_arg(init_values, "array initializer vector's pointer");

		int array_sig = 0;			//存储数组的签名
		int add_sig = 0;			//存储数组附加变量的签名

		if (1 == nodt_stamp)
		{
			//如果是nodt数组变量，则为只数组分配签名
			array_sig = assign_var_sig(array_name, "nodt_array");
		}
		else
		{
			//如果不是nodt数组变量，则为数组变量及其附加变量分配签名
			array_sig = assign_var_sig(array_name, "array");	//为数组变量分配签名
			//add_name="Array_EXTR";//构造数组附加变量的名字
			add_sig = assign_extra_sig(ARRAY_EXTR_NAME, "common");//为数组附加变量分配签名
		}
		//判断作用域是否为全局，并且该数组是否第一次遇到，进而确定是否将该变量存入Global_var链表
		if (var_scope == "global" && array_sig != -1)
		{
			//如果为全局，且是第一次遇到该数组变量
			//则先新建一个Global_var对象，存储该数组变量的信息，并插入全局Global_var链表
			//判断源码中该数组变量声明是否带有初始化
			if (true == non_init)
			{
				//如果源码中声明该数组变量时没有初始化，则构造Global_var对象时不传入初始化值
				Global_var* new_gv = new Global_var(array_name, sub, array_sig,
						2);
				//如果此变量为noadj变量，则将对象adj标签置为"noadj"
				if (1 == noadj_stamp)
				{
					new_gv->set_adj_stamp("noadj");
				}
				//将新建的Global_var对象插入全局链表
				global_vars->push_back(*new_gv);
				delete new_gv;
			}
			else
			{
				//如果源码中声明该数组变量带有初始化，则构造Global_var对象时传入初始化值
				Global_var* new_gv = new Global_var(array_name, sub, array_sig,
						init_values);
				//如果此变量为noadj变量，则将对象adj标签置为"noadj"
				if (1 == noadj_stamp)
				{
					new_gv->set_adj_stamp("noadj");
				}
				//将新建的Global_var对象插入全局链表
				global_vars->push_back(*new_gv);
				delete new_gv;
			}

			//判断该数组变量有无附加变量
			if (0 == nodt_stamp)
			{
				//如果不是nodt数组变量，说明有附加变量，则再新建一个Global_var对象，
				//存储该数组附加变量的信息，并插入全局Global_var链表
				//判断Global_var链表中是否存在ARRAY_EXTR_NAME，没有则添加 闪闪追加
				int extr_sig = 0;
				list<Global_var>::iterator iterator_extr;
				iterator_extr = global_vars->begin();
				while (iterator_extr != global_vars->end())
				{
					if (iterator_extr->get_stamp() == 0
							&& iterator_extr->get_var_name() == ARRAY_EXTR_NAME)
						extr_sig = 1;
					iterator_extr++;
				}

				if (extr_sig == 0)
				{
					Global_var* new_gv_add = new Global_var(ARRAY_EXTR_NAME,
							add_sig);

					if (1 == noadj_stamp)
					{
						new_gv_add->set_adj_stamp("noadj");
					}
					global_vars->push_back(*new_gv_add);
					delete new_gv_add;
				}
			}
			else
			{
				//如果是nodt数组变量，说明无附加变量
				;
			}
		}
		//如果此数组变量为全局数组变量，根据是否为extern变量来设置它的外部变量标签
		if (var_scope == "global" && "extern " != storage_type)
		{
			//遍历全局变量链表
			for (list<Global_var>::iterator iter = global_vars->begin();
					iter != global_vars->end(); iter++)
			{
				if (array_name == iter->get_var_name())
				{
					iter->set_ex(false);	//设置外部变量标签为假值，表明此数组变量不是外部引用变量
					break;
				}
			}
		}
		//根据是否为extern 输出编码信息
		if ("extern " == storage_type)
		{
			//全局中第一次且是extern修饰
			out_indent(indent_num, gen_out);
			gen_out << "extern T_CODE " << array_name << "[" << sub << "];" << endl;
		}
		else
		{
			//全局第一次且无extern或者局部，输出数组定义编码
			if (true == non_init)
			{
				//如果源码中声明该数组变量时没有初始化，则调用不带初始化的数组变量声明编码函数
				out_array_redcode(array_name, sub);
			}
			else
			{
				//如果源码中声明该数组变量带有初始化，则调用带初始化的数组变量声明编码函数
				out_array_redcode(array_name, sub, init_values);
			}
		}

	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding  array declaration : " + str;
		throw error_info;
	}
}

//输出IO数组的声明
static void gen_IO_array_dec(string array_name, int sub)
{
	out_indent(indent_num, gen_out);
	gen_out << "extern INT32 " << array_name << "[" << sub << "];" << endl;
}

// 功能：遍历变量声明，生成变量声明的冗余代码，包括输出高位和低位，签名的分配等。
// 输入参数：v_d-变量声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、李夫收
void gen_var_dec(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d, "variable declaration object's pointer");

		//判断该变量声明是否带有变量部分，并且是否是自定义类型声明
		if (v_d->get_stamp() == 1
				&& "typedef" != v_d->get_decl_spec()->get_stor_spec())
		{
			//如果带有变量部分，并且不是自定义类型声明，则进行本模块的初始化
			init_gen_vardec(v_d);

			//遍历被声明的变量部分
			for (list<Init_declarator>::iterator iter =
					v_d->get_inits()->begin(); iter != v_d->get_inits()->end();
					iter++)
			{
				//判断当前迭代器所指的变量是否带有初始化
				if (0 == iter->get_stamp())
				{
					//标签为0，说明被声明的变量不带初始化，比如int a;
					//判断被声明的变量是否为普通变量
					if (0 == iter->get_dec()->get_stamp())
					{
						//如果是普通变量，则先取出该变量名称
						string var_name = iter->get_dec()->get_name();
						//判断该变量是否为布尔变量
						if (1 == bool_stamp)
						{
							//如果是，则取其假值
							int f_v = get_bool_value(var_name, bool_vars, 0);
							//使用假值对其初始化
							gen_common_dec(var_name, f_v);
						}
						else
						{
							//如果不是布尔量，则使用0对其初始化
							gen_common_dec(var_name, 0);
						}
					}
					else
					{
						//如果不是普通变量，则是数组变量，对数组变量声明进行编码
						string array_name = iter->get_dec()->get_name();//取出数组变量名
						int sub = 0;					//存储数组下标表达式的值
						//计算数组下表达式
						string sub_result = compute_exp(
								&(iter->get_dec()->get_array_subs()->front()),
								&sub);
						//判断是否计算成功
						if (sub_result == "wrong")
						{
							//如果计算失败，则抛出错误信息
							string error_info =
									"array's subscript is not constant! ";
							throw error_info;
						}
						else
						{
							//如果计算成功，则对不带初始化的数组变量声明进行编码
							//判断是否是外部输入输出数组
							if(!IsIOArrayName(array_name))
							{
								vector<int>* init_values = new vector<int>;
								gen_array_dec(array_name, sub, init_values, true);
								delete init_values;
							}
							else
							{
								//IO数组，直接输出声明形式
								gen_IO_array_dec(array_name, sub);
							}

						}
					}
				}
				else
				{
					//标签为其他值，说明被声明的变量带有初始化，比如int a=9;
					//判断被声明的变量是否为普通变量
					if (0 == iter->get_dec()->get_stamp())
					{
						//如果为普通变量，则先取出该变量名称
						string var_name = iter->get_dec()->get_name();
						int value = 0;					//存储该变量的初始化值
						//计算初始化表达式的值
						string init_result = compute_exp(
								iter->get_init()->get_exp(), &value);
						//判断计算是否成功
						if (init_result == "wrong")
						{
							//如果计算失败，则抛出错误信息
							string error_info =
									"variable's initializer is not constant! ";
							throw error_info;
						}
						else
						{
							//如果计算成功，则对该变量进行编码

							//判断该变量是否为布尔变量
							if (1 == bool_stamp)
							{
								//如果是，则先取出其真假值
								int f_v = get_bool_value(var_name, bool_vars,
										0);						//假值
								int t_v = get_bool_value(var_name, bool_vars,
										1);						//真值
								if (0 == value)
								{
									//使用假值对其初始化
									gen_common_dec(var_name, f_v);
								}
								else if (f_v == value || t_v == value)
								{
									//使用初始化值对其初始化
									gen_common_dec(var_name, value);
								}
								else
								{
									//使用真值对其初始化
									gen_common_dec(var_name, t_v);
								}
							}
							else
							{
								//如果不是布尔量，则使用计算出来的初始化值进行初始化
								gen_common_dec(var_name, value);
							}
						}
					}
					else
					{
						//如果不是普通变量，则是数组变量，对带初始化的数组变量声明进行编码
						string array_name = iter->get_dec()->get_name();//取出该数组名称
						//存储数组下标表达式的值
						int sub = 0;
						//计算数组下表达式
						string sub_result = compute_exp(
								&(iter->get_dec()->get_array_subs()->front()),
								&sub);

						//存储数组各个元素的初始化值
						vector<int>* init_values = new vector<int>;
						//计算数组初始化链表的值
						string init_result = compute_init_list(
								iter->get_init()->get_init_list(), init_values);

						//判断下标表达式是否计算成功
						if (sub_result == "wrong")
						{
							//如果计算失败，则抛出错误信息
							string error_info =
									"array's subscript is not constant! ";
							throw error_info;
						}
						//判断初始化链表是否计算成功
						else if (init_result == "wrong")
						{
							//如果计算失败，则抛出错误信息
							string error_info = "initializing is not constant! ";
							throw error_info;
						}
						else
						{
							//如果均计算成功，则对带有初始化的数组变量声明进行编码
							gen_array_dec(array_name, sub, init_values, false);
							delete init_values;
						}
					}
				}
			}

		}
		else
		{
			//如果不带有变量部分，或者是自定义类型声明，则不编码
			;
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding variable declaration : " + str;
		throw error_info;
	}
}
//是否是IO数组的名称
bool IsIOArrayName(string ArrayName)
{
	string substring_array_name = ArrayName.substr(0, ArrayName.length() - 1);
	string num = ArrayName.substr(ArrayName.length() - 1, 1);
	if ("g_USR_BufFetch" == substring_array_name || "g_USR_BufRecv" == substring_array_name
			|| "g_USR_BufSend" == substring_array_name || "g_USR_BufDeliver" == substring_array_name)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void gen_var_dec_datetime(string var_name)
{
	out_indent(indent_num, gen_out);
	gen_out << var_name << ".Data.C.C1 = " << var_name << ".Data.C.C1 + " << "g_GS_TL1;" << endl;
	out_indent(indent_num, gen_out);
	gen_out << var_name << ".Data.C.C2 = " << var_name << ".Data.C.C2 + " << "g_GS_TL2;" << endl;
}

