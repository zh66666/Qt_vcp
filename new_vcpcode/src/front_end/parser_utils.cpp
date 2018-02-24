#include "cmnt.h"
#include "expression.h"
#include "struct_specifier.h"
#include "statement.h"
#include "var.h"
#include "var_declaration.h"
#include "declaration_specifiers.h"
#include "type_specifier.h"
#include "init_declarator.h"
#include "declarator.h"
#include "program.h"

#include "vcp_yacc.h"
#include<map>
#include<list>
// 记录布尔声明注释关键字的出现次数
static int bool_cmnt_key_num = 0;

//记录NOADJ注释关键字出现的次数
static int noadj_cmnt_key_num = 0;

//记录NODT注释关键字出现的次数
static int nodt_cmnt_key_num = 0;

//记录unsigned注释关键字出现的次数  闪闪
static int unsign_cmnt_key_num = 0;

// 保存当前文件中用户自定义的类型名称
list<string>* type_names = new list<string>;

// 一个布尔开关,当第一次遇到非冗余编码注释关键字时，置true
bool enter_noparse = false;

//存储语法分析的结果，需要初始化为NULL
Program *front_end_pro = NULL;

//记录位于结构体的层数，进入一个结构体类型，就加1，退出时减1
//2010年3月4日追加
int struct_body = 0;

// 定义在文件 : src/main_utils.c
extern string int2str(int);
extern list<Cmnt>* cmnts;
extern int line_num;
extern void check_str_arg(string arg_name, string info);
extern void check_pt_arg(void *pt, string info);

// 定义在文件 : ./vcp_lex.cpp
extern FILE *yyin;

// 定义在文件 : ./vcp_yacc.cpp
extern int yyparse(void);

extern string compute_exp(Expression* s, int* result);

Var_declaration* get_var_declaration() {
	//todo
	/*功能 ：获得当前声明变量列表
	 输入参数：
	 返回值：v_d 变量声明列表
	 作者：闪闪*/

}

// 功能：分析扫描到的注释，设置相关全局变量，同时存储注释。
// 输入参数：cmnt-注释类对象指针
// 返回值：成功返回void,失败抛出错误信息
// 作者：李刚、倪嘉呈
void store_cmnts(Cmnt* cmnt) {
	try {
		//检查参数：注释对象指针的有效性
		check_pt_arg(cmnt, "comment object's pointer");

		string cmnt_content = cmnt->get_content();	//读取注释内容

		//匹配注释关键字
		if ("/*KWNOADJ*/" == cmnt_content) {
			//如果是NOADJ注释关键字，计数变量加1
			noadj_cmnt_key_num++;

			//2010年3月4日追加
			//判断是否位于结构体内
			if (struct_body != 0) {
				//如果位于结构体内，则报错
				string error_info =
						"//*KWNOADJ*// should not be in struct body!";
				throw error_info;
			}
		} else if ("/*KWNODT*/" == cmnt_content) {
			//如果是NODT注释关键字，计数变量加1
			nodt_cmnt_key_num++;

			//2010年3月4日追加
			//判断是否位于结构体内
			if (struct_body != 0) {
				//如果位于结构体内，则报错
				string error_info = "//*KWNODT*// should not be in struct body!";
				throw error_info;
			}

		} else if ("/*KWCODEDBOOL*/" == cmnt_content) {
			//如果是布尔注释关键字，计数变量加1
			bool_cmnt_key_num++;
		} else if ("/*KWNOPARSE*/" == cmnt_content) {
			//如果是非关键代码注释关键字，设置全局变量enter_noparse
			enter_noparse = true;
		} else {
			//其他注释忽略
		}

		cmnts->push_back(*cmnt);	//存储该注释
	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "analyzing comment : " + str;
		throw error_info;
	}
}

// 功能：提取声明中的自定义类型名称。分析该声明对象storage specifier，
//			如果它的storage specifier 是typedef，则说明该条声明为自定义类型，
//			取出类型名称，存入list中
// 输入参数 : v_d-Var_declaration类的对象指针
// 输出参数：l-自定义类型名链表指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚、倪嘉呈
void get_type_name(list<string>* l, Var_declaration* v_d) {
	try {
		//检查参数：自定义类型名链表指针的有效性
		check_pt_arg(l, "name list's pointer");
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d, "varibale declaration object's pointer");

		//判断该声明对象的标签值和声明对象的storage specifier内容
		if (1 == v_d->get_stamp()
				&& "typedef" == v_d->get_decl_spec()->get_stor_spec()) {
			//如果标签为1，且storage specifier为typedef，说明为自定义类型声明
			for (list<Init_declarator>::iterator iter =
					v_d->get_inits()->begin(); iter != v_d->get_inits()->end();
					iter++) {
				//将自定义类型名称存入链表中
				l->push_back(iter->get_dec()->get_name());
			}
		} else {
			//不是自定义类型声明，则忽略
		}

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "getting user-defined name : " + str;
		throw error_info;
	}
}

// 功能：判断扫描到的字符串是标识符，还是自定义数据类型名。在list中查找字符串，
//			如果有，则返回宏定义TYPE_NAME；否则返回宏定义IDENTIFIER。
// 			宏定义TYPE_NAME 和IDENTIFIER在bison生成的头文件中有定义
// 输入参数：yytext-扫描到的字符串
// 		      l-用于存储自定义数据类型名称的list指针
// 返回值：成功则返回整数值，失败则抛出错误
// 作者：李刚、倪嘉呈
int check_type(string yytext, list<string>* l) {
	try {
		//检查参数：扫描到的字符串的有效性
		check_str_arg(yytext, "string whic scanned");

		//检查参数：自定义类型名称链表指针的有效性
		check_pt_arg(l, "user-defined name list's pointer");

		list<string>::iterator result = find(l->begin(), l->end(), yytext);
		//判断list迭代器是否到达了list末尾
		if (result == l->end()) {
			//达到list末端，说明该字符串为标识符
			return IDENTIFIER;		// 返回标示符的宏定义
		} else {
			//未到list末端，说明该字符串为自定义类型名
			return TYPE_NAME;		// 返回自定义类型的宏定义
		}

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "checking type : " + str;
		throw error_info;
	}
}

// 功能：检查变量声明是否符合安全编码处理器子集规范，不允许定义结构体布尔变量和布尔数组变量
// 			不允许声明void类型的变量
// 输入参数：v_d-声明对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚
void check_var_dec(Var_declaration* v_d) {
	//检查参数：变量声明对象指针的有效性
	check_pt_arg(v_d, "varibale declaration object's pointer");

	//判断此变量声明的类型是否为void类型
	if (v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp() == 0
			&& v_d->get_decl_spec()->get_type_spec_list()->front().get_type_name()
					== "void") {
		string error_info = "void variable is not support!";
		throw error_info;
	}

	//判断是否是布尔变量声明
	if (v_d->get_stamp() == 1
			&& (bool_cmnt_key_num % 2 == 1
					|| (v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()
							== 0
							&& v_d->get_decl_spec()->get_type_spec_list()->front().get_type_name()
									== "bool"))) {
		//遍历被声明的变量链表
		for (list<Init_declarator>::iterator iter = v_d->get_inits()->begin();
				iter != v_d->get_inits()->end(); iter++) {
			//发现如果有数组变量，则抛出错误
			if (iter->get_dec()->get_stamp() == 1) {
				string error_info = "boolean array is not support!";
				throw error_info;
			}
		}
	}
	//判断是否是布尔结构体变量声明
	if (v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp() == 2
			&& bool_cmnt_key_num % 2 == 1) {
		string error_info = "boolean struct variable is not support!";
		throw error_info;
	}

	if (v_d->get_decl_spec()->get_stor_spec() != "typedef"
			&& v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()
					== 2) {
		string error_info ="struct type must modifier by typedef!";
		throw error_info;
	}

	//2010年3月5日追加
	//判断当前变量声明是否结构体变量声明
	if (1 == v_d->get_stamp()
			&& v_d->get_decl_spec()->get_stor_spec() != "typedef"
			&& v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()
					== 2) {
		//如果是结构体变量声明，则取出结构体的类型名称
		string type_name =
				v_d->get_decl_spec()->get_type_spec_list()->front().get_struct_spec()->get_name();
		//遍历被声明的结构体变量，检查是否有变量名同类型名相同
		for (list<Init_declarator>::iterator iter = v_d->get_inits()->begin();
				iter != v_d->get_inits()->end(); iter++) {
			//判断当前被声明的变量的名字是否与结构体类型名相同
			if (type_name == iter->get_dec()->get_name()) {
				//如果相同，则报错
				string error_info =
						"struct type name should not be same as the variable's name!";
				throw error_info;
			}
		}
	}
}

// 2010年3月5日追加
// 功能：检查所规约的数组下标表达式是否符合C子集规范，即如果发现下标表达式不是简单变量或者常数，
//           就报错
// 输入参数：s-数组下标表达式对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚
void check_array_sub(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");

	//判断表达式类型
	switch (s->get_stamp()) {
	case 0:	//带有两个操作数的表达式

	case 1:	//带有一个操作数的表达式
	{
		int i = 0;
		string result = compute_exp(s, &i);
		if ("wrong" == result) {
			string error_info = "array's subscript expression is invalid!";
			throw error_info;
		}
	}
		break;
	case 5:	//函数调用表达式
	{
		string error_info = "array's subscript expression is invalid!";
		throw error_info;
	}
		break;

	case 2: {
		//如果是括号表达式，则检查括号中的表达式
		check_array_sub(s->get_exp());
	}
		break;

	case 3:	//常数表达式
	case 4:	//变量表达式
	{
	}
		break;
	}
}

// 2010年3月9日追加
// 功能：检查计算函数是否在表达式中被调用，如果是，则报错
// 输入参数：s-表达式对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚
void check_fun(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");

	//判断表达式类型
	switch (s->get_stamp()) {
	case 0: {
		//带有两个操作数的表达式，继续检查左操作数和右操作数
		check_fun(s->get_cel());
		check_fun(s->get_cer());
	}
		break;
	case 1: {
		//带有一个操作数的表达式，继续检查右操作数
		check_fun(s->get_cer());
	}
		break;
	case 2: {
		//括号表达式，检查括号中的表达式
		check_fun(s->get_exp());
	}
		break;
	case 5: {
		//函数调用表达式，报错
		string error_info = "compute function calling is invalid!";
		throw error_info;
	}
		break;
	}
}

// 2010年3月9日追加
// 功能：检查赋值语句的左值变量的类型，如果是简单变量、下标为常数的数组变量、结构体变量、下标为常数
//		   的结构体数组变量，则返回0，否则返回1
// 输入参数：s-变量对象指针
//返回值：成功返回一个整数 ，失败抛出错误
// 作者：李刚
int check_left_var(Var* s) {
	//检查参数：变量对象指针的有效性
	check_pt_arg(s, "variable object's pointer");

	//判断变量的类型
	switch (s->get_stamp()) {
	case 0: {
		//如果是一般的简单变量
		return 0;
	}
		break;

	case 2: {
		//如果是数组变量，则分析数组的下标是不是常数
		list<Expression>::iterator iter;
		for (iter = s->get_exp_list()->begin();
				iter != s->get_exp_list()->end(); iter++) {
			//判断下标表达式是否是常数
			if (3 != iter->get_stamp()) {
				//如果不是常数，则返回1
				return 1;
			}
		}
		//判断是否已经遍历完整个下标链表
		if (iter == s->get_exp_list()->end()) {
			//如果已经遍历完，并且每个表达式都为常数
			return 0;
		}
	}
		break;

	case 3: {
		//如果是结构体变量，则分析结构体变量
		int i = check_left_var(s->get_var());
		return i;

	}
		break;

	case 4: {
		//如果是结构体数组变量，则先分析数组的下标
		list<Expression>::iterator iter;
		for (iter = s->get_exp_list()->begin();
				iter != s->get_exp_list()->end(); iter++) {
			//判断下标表达式是否是常数
			if (3 != iter->get_stamp()) {
				//如果不是常数，则返回1
				return 1;
			}
		}
		//判断是否已经遍历完整个下标链表
		if (iter == s->get_exp_list()->end()) {
			//如果已经遍历完，并且每个表达式都为常数，则继续遍历结构体变量
			int i = check_left_var(s->get_var());
			return i;
		}
	}
		break;
	}
}

// 2010年3月9日追加
// 功能：检查赋值语句中是否出现了计算函数的非法调用，如果存在，则报错
// 输入参数：s-语句对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚
void check_assign_stmt(Statement* s) {
	//检查参数：语句对象指针的有效性
	check_pt_arg(s, "statement object's pointer");

	//分析左值变量
	int i = check_left_var(s->get_cvar());

	//判断左值变量是否符合调用计算函数的规范
	if (0 == i) {
		//如果左值变量符合规范，则判断右表达式是否为计算函数调用
		if (5 == s->get_ce()->get_stamp()) {
			//如果赋值表达式是函数调用，则分析函数参数
			for (list<Expression>::iterator iter =
					s->get_ce()->get_args()->begin();
					iter != s->get_ce()->get_args()->end(); iter++) {
				check_fun(&(*iter));
			}
		} else {
			//如果赋值表达式不是函数调用，则继续分析
			check_fun(s->get_ce());
		}
	} else {
		//如果左值变量不符合规范，则直接分析赋值表达式
		check_fun(s->get_ce());
	}
}

void check_assign_mix() {

}
// 2010年3月9日追加
// 功能：检查非逻辑运算符两边的表达式是否含有逻辑运算，如果有，则报错
// 输入参数：s-表达式对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚
void check_unlogic_exp(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");

	//判断表达式类型
	switch (s->get_stamp()) {
	case 0: {
		//如果是带有两个操作数的表达式，先判断操作符是否为逻辑运算符
		if ("&&" == s->get_op() || "||" == s->get_op()) {
			//如果为逻辑运算符，则报错
			string error_info = "&& ,|| expression is invalid!";
			throw error_info;
		} else {
			//如果不为逻辑运算符，则继续分析左右操作数
			check_unlogic_exp(s->get_cel());
			check_unlogic_exp(s->get_cer());
		}
	}
		break;

	case 1: {
		//如果是逻辑非运算、按位取反运算，则报错
		if ("!" == s->get_op()) {
//			string error_info = "! expression is invalid!";
//			throw error_info;
		}
		if ("~" == s->get_op()) {
//			string error_info = "~ expression is invalid!";
//			throw error_info;
		}	//闪闪追加
	}
		break;

	case 2: {
		//如果是括号表达式，则继续分析括号中的表达式
		check_unlogic_exp(s->get_exp());
	}
		break;
	}
}

/*功能：不允许减法运算中进行无符号操作
 输入参数：sl-sr 表达式对象指针
 返回值:成功返回void,失败抛出异常
 作者：闪闪*/
void check_subunsig_exp(Expression* sl, Expression* sr, map<string, string> v) {
	if (sl->get_stamp() == 4 && sr->get_stamp() == 4) {
		//均是普通变量
		int tl = 0;
		int tr = 0;
		string l_name = sl->get_cvar()->get_name();
		string r_name = sr->get_cvar()->get_name();
		map<string, string>::iterator scope_nt_lerator;
		string first = "";
		string second = "";

		for (scope_nt_lerator = v.begin(); scope_nt_lerator != v.end();
				++scope_nt_lerator) {
			first = scope_nt_lerator->first;
			second = scope_nt_lerator->second;

			if (second == "unsigned" && first == l_name)
				tl = 1;
			if (second == "unsigned" && first == r_name)
				tr = 1;
		}
		if (tl == 1 || tr == 1) {
			//yyerror("unsigned signed mixed is not support!");
			string error_info = "\n unsigned is not support in sub!";
			throw error_info;
		}

	}
}

/*2014年12月14日追加
 功能：检查基本运算符两边表达式是否有无符号有符号混合运算，如果有，则报错
 输入参数：s-表达式对象指针
 返回值:成功返回void,失败抛出异常
 作者：闪闪*/
void check_mixsign_exp(Expression* sl, Expression* sr, map<string, string> v) {
	if (sl->get_stamp() == 4 && sr->get_stamp() == 4) {
		//均是普通变量
		int tl = 0;
		int tr = 0;
		string l_name = sl->get_cvar()->get_name();
		string r_name = sr->get_cvar()->get_name();
		map<string, string>::iterator scope_nt_lerator;
		string first = "";
		string second = "";

		for (scope_nt_lerator = v.begin(); scope_nt_lerator != v.end();
				++scope_nt_lerator) {
			first = scope_nt_lerator->first;
			second = scope_nt_lerator->second;

			if (second == "unsigned" && first == l_name)
				tl = 1;
			if (second == "unsigned" && first == r_name)
				tr = 1;
		}
		if ((tl != tr) && (tl == 1 || tr == 1)) {
			//yyerror("unsigned signed mixed is not support!");
			string error_info = "\n unsigned signed mixed is not support!";
			throw error_info;
		}

	}
}

//功能：语法分析中已知变量名获得变量类型
//阮闪闪 2015年1月
list<string> get_type_var(string name, map<string, string> v) {
	map<string, string>::iterator scope_nt_lerator;
	string first = "";
	string second = "";
	list<string> list_type;
	for (scope_nt_lerator = v.begin(); scope_nt_lerator != v.end();
			++scope_nt_lerator) {
		first = scope_nt_lerator->first;
		second = scope_nt_lerator->second;
		if (first == name)
			list_type.push_back(second);
	}
	if (v.empty()) {
		string errorstr = " typelist is empty";
		throw errorstr;
	}
	if (list_type.empty()) {
		string errorstr = name + " var type is empty";
		throw errorstr;
	}

	return list_type;
}

void check_mixsign_exp(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");
	switch (s->get_stamp()) {
	case 0: {
		//如果是带两个操作数的表达式，则先判断操作符是否是基本运算
		if ("+" == s->get_op() || "-" == s->get_op() || "*" == s->get_op()
				|| "/" == s->get_op()) {
			//如果为基本运算符，则继续分析左右操作数
			check_mixsign_exp(s->get_cel());
			check_mixsign_exp(s->get_cer());
		}
	}
		break;
	case 2: {
		//如果是括号表达式，则继续分析括号中的表达式
		check_unlogic_exp(s->get_exp());
	}
		break;
	case 3: {
		//如果是常数
	}
		break;

	case 4: {
		//如果是变量，则分析变量，判断变量是否为一般的普通变量
		/*Var_declaration* leftv = s->get_cel();
		 Var_declaration* leftr = s->get_cer();
		 int lsign = leftv->get_signed_stamp();
		 int rvsign = leftr->get_signed_stamp();
		 if (lsign != rvsign) {
		 //unsigned
		 string error_info = "signed unsigned mixed do not support!";
		 throw error_info;
		 } else {

		 }*/
	}
		break;

	}
}

// 2010年3月9日追加
// 功能：检查逻辑运算符两边的表达式是否满足C子集规范，如果不满足，则报错
// 输入参数：s-表达式对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚
void check_logic_exp(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");

	//判断表达式类型
	switch (s->get_stamp()) {
	case 0: {
		//如果是带有两个操作数的表达式，先判断操作符是否为逻辑运算符
		if ("&&" == s->get_op() || "||" == s->get_op()) {
			//如果为逻辑运算符，则继续分析左右操作数
			check_logic_exp(s->get_cel());
			check_logic_exp(s->get_cer());
		} else if (">=" == s->get_op() || ">" == s->get_op()
				|| "<=" == s->get_op() || "<" == s->get_op()
				|| "==" == s->get_op() || "!=" == s->get_op()) {
			//如果为关系运算符，则继续分析左右操作数
			check_unlogic_exp(s->get_cel());
			check_unlogic_exp(s->get_cer());
		} else {
			//如果不为逻辑运算符，则报错
			string error_info = "logical expression is invalid!";
			throw error_info;
		}
	}
		break;

	case 1: {
		//如果是逻辑非运算、按位取反运算，则继续分析右操作数
		check_logic_exp(s->get_cer());
	}
		break;

	case 2: {
		//如果是括号表达式，则继续分析括号中的表达式
		check_logic_exp(s->get_exp());
	}
		break;

	case 3: {
		//如果是常数，则报错
		string error_info = "logical expression has constant!";
		throw error_info;
	}
		break;

	case 4: {
		//如果是变量，则分析变量，判断变量是否为一般的普通变量
		//if (0 != s->get_cvar()->get_stamp()) {
			//如果不是普通变量，则报错
			string error_info = "logical operand expression has var or array!";
			throw error_info;
		//}
	}
		break;

	case 5: {
		//如果是函数调用，则报错
		string error_info = "logical expression is invalid!";
		throw error_info;
	}
		break;
	}
}

// 2015年1月
// 功能：检查按位运算符两边的表达式是否满足C子集规范，如果不满足，则报错
// 输入参数：s-表达式对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：闪闪
void check_bit_exp(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");

	//判断表达式类型
	switch (s->get_stamp()) {
	case 1: {
		//如果是逻辑非运算，则报错
		string error_info = " bit_inverse expression is invalid!";
		throw error_info;
	}
		break;

	case 2: {
		//如果是括号表达式，则继续分析括号中的表达式
		check_bit_exp(s->get_exp());
	}
		break;
	}
}
//按位取反约束条件
//阮闪闪 添加
void check_inverse_exp(Expression* s) {
	//检查参数：表达式对象指针的有效性
	check_pt_arg(s, "expression object's pointer");

	//判断表达式类型
	switch (s->get_stamp()) {
	case 0: {
		//如果是带有两个操作数的表达式，先判断操作符是否为逻辑运算符
		if ("&&" == s->get_op() || "||" == s->get_op()) {
			//如果为逻辑运算符，则继续分析左右操作数
			check_logic_exp(s->get_cel());
			check_logic_exp(s->get_cer());
		} else if (">=" == s->get_op() || ">" == s->get_op()
				|| "<=" == s->get_op() || "<" == s->get_op()
				|| "==" == s->get_op() || "!=" == s->get_op()) {
			//如果为关系运算符，则继续分析左右操作数
			check_unlogic_exp(s->get_cel());
			check_unlogic_exp(s->get_cer());
		} else {
			//如果不为逻辑运算符，则报错
//			string error_info = "inver logical expression is invalid!";
//			throw error_info;
			check_inverse_exp(s->get_cel());
			check_inverse_exp(s->get_cer());
		}
	}
		break;

	case 1: {
		//按位取反运算，则继续分析右操作数
		check_inverse_exp(s->get_cer());
	}
		break;

	case 2: {
		//如果是括号表达式，则继续分析括号中的表达式
		check_inverse_exp(s->get_exp());
	}
		break;

	case 3: {
		//如果是常数，则报错
//		string error_info = "logical expression has constant!";
//		throw error_info;
	}
		break;

	case 4: {
		//如果是变量，则分析变量，判断变量是否为一般的普通变量
//		if (0 != s->get_cvar()->get_stamp()) {
//			//如果不是普通变量，则报错
//			string error_info = "logical operand is invalid!";
//			throw error_info;
//		}
	}
		break;

	case 5: {
		//如果是函数调用，则报错
		string error_info = "function logical expression is invalid!";
		throw error_info;
	}
		break;
	}
}

//enum约束规范
//阮闪闪 追加
void check_enum_dec(string str) {
	string error_info = "enum declaration error : " + str;
	throw error_info;
}

void check_if_while(Statement* s){
	 //检查参数：语句对象指针的有效性
	check_pt_arg(s, "statement object's pointer");
	if(s->get_stamp() != 0) {
		string error_info = "if/while do not with {}";
		throw error_info;
		}
}
//约束if、else、while其后紧跟{}
//闪闪
void check_else_if(Statement* s){
		check_pt_arg(s, "statement object's pointer");
		if(s->get_stamp() != 0) {
			if(s->get_stamp() == 1){

			}else{
				string error_info = "else/esle if  do not with {}";
				throw error_info;
			}
		}

}

// 功能：设置声明对象的三个数据成员，这三个数据成员表示该声明是否是输入类型变量，
//           是否是布尔变量，是否是NODT变量 ,是否是有符号变量
// 输入参数：v_d-声明对象指针
//返回值：成功返回void ，失败抛出错误
// 作者：李刚、倪嘉呈
//
void set_var_dec(Var_declaration* v_d) {
	try {
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d, "varibale declaration object's pointer");

		v_d->set_noadj_stamp(noadj_cmnt_key_num % 2);	//设置该变量声明的noadj标签
		v_d->set_nodt_stamp(nodt_cmnt_key_num % 2);	 //设置该变量声明的nodt标签
		v_d->set_bool_stamp(bool_cmnt_key_num % 2);	 //设置该变量声明的bool标签
		//v_d->set_unsigned_stamp(unsign_cmnt_key_num % 2);//设置该变量声明的unsigned标签  闪闪
		//对于不使用布尔注释关键字，而使用bool关键字声明的布尔变量，应对其分析
		//根据分析结果设置相应的数据标签
		//使用 unsigned 关键字声明的整形变量
		for (list<Type_specifier>::iterator iter =
				v_d->get_decl_spec()->get_type_spec_list()->begin();
				iter != v_d->get_decl_spec()->get_type_spec_list()->end();
				iter++) {
			//判断类型的标签与类型的名称
			if (iter->get_stamp() == 0 && iter->get_type_name() == "bool") {
				//如果变量类型为一般类型，且关键字为"bool"，则设置布尔标签为1
				v_d->set_bool_stamp(1);
			} else if (iter->get_stamp() == 0
					&& iter->get_type_name() == "unsigned") {
				//无符号类型   闪闪
				//v_d->set_unsigned_stamp(1);
			}
		}
	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "setting variable declaration : " + str;
		throw error_info;
	}
}

// 功能：继续向上层抛出语法词法分析中的错误信息
// 输入参数：str-错误提示信息
// 返回值：void
// 作者：李刚、倪嘉呈
void yyerror(string str) {
	//在传入的信息前加上本模块的信息，继续向上抛出
	string error_info = "\n flex and bison : " + str;

	throw error_info;
}

// 功能：初始化前端模块中的全局变量
// 参数：无
// 返回值：无
// 作者：李刚、倪嘉呈
static void initial_front_end(void) {
	yyin = NULL;							//文件指针初始化为NULL
	front_end_pro = NULL;			//结构指针初始化为NULL
	bool_cmnt_key_num = 0;		//布尔注释关键字个数初始化为0
	nodt_cmnt_key_num = 0;		//NODT注释关键字个数初始化为0
	noadj_cmnt_key_num = 0;	//NOADJ注释关键字个数初始化为0
	unsign_cmnt_key_num = 0;	//sign注释关键字个数初始化为0  闪闪
	enter_noparse = false;			//非关键代码词法分析开关初始化为false
	type_names->clear();			//自定义类型名链表初始化时清空
}

// 功能：前端处理，包括前端数据初始化和词法语法分析，得到中间结构指针
// 输入参数：file_name-待分析的文件路径名，
// 输出参数：pro-中间结构指针的引用
// 返回值：
// 作者：李刚、倪嘉呈
void front_end(string file_name, Program*& pro) {
	try {
		//检查参数：文件名字符串的有效性
		check_str_arg(file_name, "file's name");

		initial_front_end();	//初始化前端模块

		yyin = fopen(file_name.c_str(), "r");	//打开要处理的文件
		//判断文件指针是否为NULL
		if (yyin == NULL) {
			// 如果文件打开失败，抛出错误
			string error_info = "file  open faild!";
			throw error_info;
		} else {
			//文件打开成功，进行语法分析
			do {
				yyparse(); //语法分析，生成中间结构
			} while (!feof(yyin));
		}

		//判断语法分析规约的中间结构指针是否为空
		if (NULL == front_end_pro) {
			// 如果规约出的中间结构指针为空，说明语法分析失败
			string error_info = "file  parsing faild!";
			throw error_info;
		} else {
			//不为空，则将其赋给参数pro
			pro = front_end_pro;
		}
		fclose(yyin); //关闭文件
	} catch (string str) {
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "front end : " + str;
		throw error_info;
	}
}

