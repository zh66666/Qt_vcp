#include "gen_utils.h"
#include "var.h"
#include "expression.h"

// 定义在文件: src/main_utils.cpp
extern bool debug_flag;
extern string get_var_stamp(list<Var_info>* s, string var_name,
		string var_scope);
extern const int P1;
extern const int P2;
extern const string ARRAY_EXTR_NAME;
extern struct TAG_DATA T_DATA;
//定义在文件gen_global_var.cpp
extern string get_bind_local_var(string global_var, string func_name);
extern bool is_global_var(string var_name);
//定义在文件main_utils.cpp
extern string int2HexStr_data(const int num);
extern int str2int(string str);
// 功能：输出数组赋值运算冗余代码中的前三条代码，包括数组的高位赋值，数组的低位赋值，
// 			数组附加变量的高位赋值。
// 闪闪  数组下标的高位赋值
// 输入参数：array_name-数组名
// 			      h_sub-数组下标的高位名称
// 			      h_exp-数组赋值表达式的高位名称
// 			      l_exp-数组赋值表达式的低位名称
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
static void out_array_assign_three(string array_name, string h_sub,
		string h_exp, string l_exp) {
	try {
		//检查输入参数的有效性
		check_str_arg(array_name, "array's name");
		check_str_arg(h_sub, "array subscript's high name");
		check_str_arg(h_exp, "assign expression 's high name");
		check_str_arg(l_exp, "assign expression 's low name");

		//输出数组赋值的高位代码
		out_indent(indent_num, gen_out);
		gen_out << "H_" << array_name << "[" << h_sub << "] = " << h_exp << ";"
				<< endl;
		//输出数组赋值的低位代码
		out_indent(indent_num, gen_out);
		gen_out << "L_" << array_name << "[" << h_sub << "] = " << l_exp << ";"
				<< endl;
		//输出数组附加变量的高位代码
		//闪闪     输出数组下标高位
		out_indent(indent_num, gen_out);
		gen_out << "H_Array_EXTR_" << array_name << " = H_Array_EXTR_"
				<< array_name << " + " << h_sub << ";" << endl;
	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "output array assign's part codes : " + str;
		throw error_info;
	}
}

bool StrCompare(const char* str1, const char* str2, int size) {
	for (int i = 0; i < size; i++) {
		if (*str1 != *str2)
			return false;
		str1++;
		str2++;
	}
	return true;
}

// 功能：对数组下标和赋值表达式都是常数的数组赋值运算进行编码，包括签名的分配和补偿
// 输入参数：array_name-数组名称
// 			      sub-数组下标值
// 			      exp-数组赋值表达式的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
static void gen_array_assign_I(string array_name, int sub, int exp,
		list<Var_info>* v) {
	try {
		//检查输入参数的有效性
		check_str_arg(array_name, "array's name");
		check_pt_arg(v, "var_info list's pointer");

		int i_sig = getConstantSig(sub); //为下标分配签名
		long long low_i = (-((long long) sub * K_NUM_P1) % P1 + i_sig) % P1; //计算下标的低位
		long long low_i2 = (-((long long) sub * K_NUM_P2) % P2 + i_sig) % P2; //计算下标的低位

		int exp_sig = getConstantSig(exp); //为赋值常数分配签名
		long long low_exp = ((-((long long) exp * K_NUM_P1) % P1 + exp_sig) % P1
				+ P1) % P1; //计算赋值常数的低位
		long long low_exp2 = ((-((long long) exp * K_NUM_P2) % P2 + exp_sig)
				% P2 + P2) % P2; //计算赋值常数的低位

		//string arrExtr_name = "arrExtr_" + array_name; //构造数组附加变量名称
		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope); //取出数组附加变量签名
		int new_arr_index = 0;
		int new_sig = get_sig_index(&new_arr_index); //为数组附加变量分配新签名
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global"); //将新签名加入动态签名表
		//计算附加变量低位代码中用到的补偿常数
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//取出数组的签名
//		计算数组低位调整代码中用到的补偿常数
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;

		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;
		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;

		//如果是调试模式，则输出调试信息：赋值常数的签名，下标常数的签名，附加变量的新签名，数组的签名
		if (true == debug_flag) {
			//out_indent(indent_num, gen_out);
//			gen_out << "/*" << exp << "'s sig :" << exp_sig << " " << sub
//					<< "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << array_name
//					<< "'s sig:" << array_sig << "*/" << endl << endl;
		}

		//输出数组下标、赋值表达式为常量的调用形式	//闪闪
		string sub_h = int2HexStr_data(sub);
		string exp_h = int2HexStr_data(exp);
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.F = CONST_NUM_"
				<< sub_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;				//修正校验域1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;				//修正校验域2
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.F = CONST_NUM_"
				<< exp_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;				//修正校验域1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;				//修正校验域2
		out_indent(indent_num, gen_out);

		string CSCArrayName = GetArrayNameInCSC(array_name);
		gen_out << CSCArrayName << "[AC_BIND_CONST_NUM_" << sub_h
				<< ".Data.F].Data = F_VCL_ArrayAssign(AC_BIND_CONST_NUM_"
				<< exp_h << ".Data, AC_BIND_CONST_NUM_" << sub_h << ".Data, "
				<< array_const1 << ", " << array_const2 << ");" << endl;

		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt("<< CSCArrayName
				<< "[AC_BIND_CONST_NUM_" << sub_h
				<< ".Data.F].Data.C, AC_BIND_CONST_NUM_" << exp_h
				<< ".Data.C, AC_BIND_CONST_NUM_" << sub_h << ".Data, "
				<< arrExtr_const1 << ", " << arrExtr_const2 << ");" << endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);
		//增加赋值变量更新签名
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding array assign (subscript is constant, "
				"assign expression is constant): " + str;
		throw error_info;
	}
}

// 功能：对数组下标为常数，赋值表达式为简单变量的的数组赋值运算进行编码，包括签名的分配和补偿
// 输入参数：array_name-数组名称
// 			      sub-数组下标值
// 			      exp_name-数组赋值表达式的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
static void gen_array_assign_II(string array_name, int sub, string exp_name,
		list<Var_info>* v) {
	try {
		//检查输入参数的有效性
		check_str_arg(array_name, "array's name");
		check_str_arg(exp_name, "variable's name");
		check_pt_arg(v, "var_info list's pointer");
		const char* name = exp_name.data();
		//判断是否是中间变量"AC_Tmp_"
		bool result = StrCompare("AC_Tmp_", name, 7);

		int i_sig = getConstantSig(sub);		//为数组下标分配签名
		long long low_i = (-((long long) sub * K_NUM_P1) % P1 + i_sig) % P1;//计算数组下标的低位
		long long low_i2 = (-((long long) sub * K_NUM_P2) % P2 + i_sig) % P2;//计算数组下标的低位
		//string arrExtr_name = "arrExtr_" + array_name;		//构造数组附加变量名称

		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope);//取出数组附加变量签名
		int sig_index = 0;
		int new_sig = get_sig_index(&sig_index);		//为数组附加变量分配新签名
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global");	//将新签名加入动态签名表
		//计算附加变量低位代码中用到的补偿常数
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//取出数组的签名

		string expl_name;
		bool expGlobal = is_global_var(exp_name);
		if (expGlobal == true) {
			expl_name = get_bind_local_var(exp_name, var_scope);
		} else {
			expl_name = exp_name;
		}

		int exp_sig = get_current_sig(v, expl_name, var_scope);		//取出赋值变量的签名
		//计算数组低位调整代码中用到的补偿常数
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;
		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;

		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;

		//如果是调试模式，则输出调试信息：赋值变量的签名，下标常数的签名，附加变量的新签名，数组的签名
		if (true == debug_flag) {
//			out_indent(indent_num, gen_out);
//			gen_out << "//" << exp_name << "'s sig :" << exp_sig << " " << sub
//					<< "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << array_name
//					<< "'s sig:" << array_sig << endl << endl;
		}
		string sub_h = int2HexStr_data(sub);
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.F = CONST_NUM_"
				<< sub_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;		//修正校验域1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;		//修正校验域2

		string CSCArrayName = GetArrayNameInCSC(array_name);
		out_indent(indent_num, gen_out);
		gen_out << CSCArrayName << "[AC_BIND_CONST_NUM_" << sub_h
				<< ".Data.F].Data = F_VCL_ArrayAssign(";
		if (result == true) {
			//中间变量输出无TC
		} else {
			if (expGlobal == true) {
				//全局变量输出无TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data, AC_BIND_CONST_NUM_" << sub_h
				<< ".Data, " << array_const1 << ", " << array_const2 << ");"
				<< endl;
//////////////////////////////////////////////////////////
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt(" << CSCArrayName
				<< "[AC_BIND_CONST_NUM_" << sub_h << ".Data.F].Data.C, ";

		if (result == true) {
			//输出无TC
		} else {
			if (expGlobal == true) {
				//全局变量输出无TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data.C, AC_BIND_CONST_NUM_" << sub_h
				<< ".Data, " << arrExtr_const1 << ", " << arrExtr_const2 << ");"
				<< endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);

		//增加赋值变量更新签名
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding array assign (subscript is constant, "
				"assign expression is variable): " + str;
		throw error_info;
	}
}

// 功能：对数组下标为简单变量、赋值表达式是常数的数组赋值运算进行编码，包括签名的分配和补偿
// 输入参数：array_name-数组名称
// 			      i_name-数组下标名称
// 			      exp-数组赋值表达式的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
static void gen_array_assign_III(string array_name, string i_name, int exp,
		list<Var_info>* v) {
	try {
		//检查输入参数的有效性
		check_str_arg(array_name, "array's name");
		check_str_arg(i_name, "array subscript's name");
		check_pt_arg(v, "var_info list's pointer");

		int exp_sig = getConstantSig(exp);		//为赋值常数分配签名
		long long low_exp = ((-((long long) exp * K_NUM_P1) % P1 + exp_sig) % P1
				+ P1) % P1;		//计算赋值常数的低位
		long long low_exp2 = ((-((long long) exp * K_NUM_P2) % P2 + exp_sig)
				% P2 + P2) % P2;		//计算赋值常数的低位

		//string arrExtr_name = "arrExtr_" + array_name;		//构造数组附加变量名称
		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope);//取出数组附加变量签名
		int sig_index = 0;
		int new_sig = get_sig_index(&sig_index);		//为数组附加变量分配新签名
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global");	//将新签名加入动态签名表

		string il_name;
		const char* iname = i_name.data();
		bool iresult = StrCompare("AC_Tmp_", iname, 7);		//i是否是局部变量判断
		bool indexGlobal = is_global_var(i_name);
		if (indexGlobal == true) {
			il_name = get_bind_local_var(i_name, var_scope);
		} else {
			il_name = i_name;
		}

		int i_sig = get_current_sig(v, il_name, var_scope);		//取出下标变量的签名
		//计算附加变量低位代码中用到的补偿常数
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//取出数组的签名
		//计算数组低位调整代码中用到的补偿常数
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;
		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;

		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;
		//如果是调试模式，则输出调试信息：赋值常数的签名，下标变量的签名，附加变量的新签名，数组的签名
		if (true == debug_flag) {
//			out_indent(indent_num, gen_out);
//			gen_out << "//" << exp << "'s sig :" << exp_sig << " " << i_name
//					<< "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << array_name
//					<< "'s sig:" << array_sig << endl << endl;
		}
		string exp_h = int2HexStr_data(exp);
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.F = CONST_NUM_"
				<< exp_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;		//修正校验域1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;		//修正校验域2
		out_indent(indent_num, gen_out);
		string CSCArrayName = GetArrayNameInCSC(array_name);
		gen_out << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name
				<< ".Data.F].Data = F_VCL_ArrayAssign(AC_BIND_CONST_NUM_"
				<< exp_h << ".Data,";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data, " << array_const1 << ", " << array_const2
				<< ");" << endl;
/////////////////////////////////////////////////////////////////
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt(" << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data.F].Data.C, AC_BIND_CONST_NUM_" << exp_h
				<< ".Data.C, ";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data, " << arrExtr_const1 << ", "
				<< arrExtr_const2 << ");" << endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);

		//增加赋值变量更新签名
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding array assign (subscript is variable, "
				"assign expression is constant): " + str;
		throw error_info;
	}
}

// 功能：对数组下标和赋值表达式都为简单变量的数组赋值运算进行编码，包括签名的分配和补偿
// 输入参数：array_name-数组名称
// 			      i_name-数组下标变量名
// 			     exp_name-数组赋值变量名
// 			     v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
static void gen_array_assign_IV(string array_name, string i_name,
		string exp_name, list<Var_info>* v) {
	try {
		//检查输入参数的有效性
		check_str_arg(array_name, "array's name");
		check_str_arg(i_name, "array subscript's name");
		check_str_arg(exp_name, "expression's name");
		check_pt_arg(v, "var_info list's pointer");
		const char* name = exp_name.data();
		//判断是否是中间变量"AC_Tmp_"
		bool result = StrCompare("AC_Tmp_", name, 7);

		//string arrExtr_name = "arrExtr_" + array_name;		//构造数组附加变量名称
		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope);//取出数组附加变量签名
		int sig_index = 0;
		int new_sig = get_sig_index(&sig_index);		//为数组附加变量分配新签名
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global");	//将新签名加入动态签名表
		//下标是否是全局变量
		string il_name;
		const char* iname = i_name.data();
		bool iresult = StrCompare("AC_Tmp_", iname, 7);		//i是否是局部变量判断
		bool indexGlobal = is_global_var(i_name);
		if (indexGlobal == true) {
			il_name = get_bind_local_var(i_name, var_scope);
		} else {
			il_name = i_name;
		}

		int i_sig = get_current_sig(v, il_name, var_scope);		//取出下标变量的签名
		//计算附加变量低位代码中用到的补偿常数
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//取出数组的签名
		//判断赋值变量是否是全局变量
		string expl_name;
		bool expGlobal = is_global_var(exp_name);
		if (expGlobal == true) {
			expl_name = get_bind_local_var(exp_name, var_scope);
		} else {
			expl_name = exp_name;
		}

		int exp_sig = get_current_sig(v, expl_name, var_scope);	//取出赋值表达式变量的签名
		//计算数组低位调整代码中用到的补偿常数
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;
		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;

		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;

		//如果是调试模式，则输出调试信息：赋值变量的签名，下标变量的签名，附加变量的新签名，数组的签名
		if (true == debug_flag) {
			out_indent(indent_num, gen_out);
//			gen_out << "/*" << exp_name << "'s sig :" << exp_sig << " "
//					<< i_name << "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << ARRAY_EXTR_NAME
//					<< "'s sig:" << array_sig << "*/" << endl << endl;
		}

		gen_out << endl;
		string CSCArrayName = GetArrayNameInCSC(array_name);
		out_indent(indent_num, gen_out);
		gen_out << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data.F].Data = F_VCL_ArrayAssign(";
		if (result == true) {
			//输出无TC
		} else {

			if (expGlobal == true) {
				//全局变量输出无TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data,";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data, " << array_const1 << ", " << array_const2
				<< ");" << endl;
////////////////////////////////////////////////////////////
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt(" << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data.F].Data.C, ";
		if (result == true) {
			//输出无TC
		} else {
			if (expGlobal == true) {
				//全局变量输出无TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data.C, ";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data," << arrExtr_const1 << ", "
				<< arrExtr_const2 << ");" << endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);

		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding array assign (subscript is variable, "
				"assign expression is variable): " + str;
		throw error_info;
	}
}

// 功能：对数组的赋值进行编码，主要区分数组下标为常数或者变量，数组赋值表达式为常数或者变量，
//			四种主要情况，按照算法和冗余代码规范输出数组赋值的安全代码
// 输入参数：var-数组变量对象指针
// 			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
void gen_array_assign(Var* var, Expression* exp, list<Var_info>* v) {
	try {
		//检查输入参数的有效性
		check_pt_arg(var, "variable object's pointer");
		check_pt_arg(exp, "expression object's pointer");
		check_pt_arg(v, "var_info list's pointer");

		string array_name = "";		//存储数组名
		string i_name = "";		//存储下标名
		string exp_name = "";		//存储赋值表达式名字

		//判断数组的下标是否为常数，赋值表达式是否为常数
		if (3 == var->get_cexp()->get_stamp() && 3 == exp->get_stamp()) {
			//如果数组下标为常数，且是常数赋值，例如a[2]=3;
			array_name = var->get_name();		//取出数组变量名
			i_name = var->get_cexp()->get_c();		//数组下标名称
			exp_name = exp->get_c();		//取出赋值常数的名称

			//对数组下标和赋值都为常数的数组赋值运算进行编码
			gen_array_assign_I(array_name, str2int(i_name), str2int(exp_name),
					v);

		}
		//判断数组下标是否为常数，赋值表达式是否为简单变量
		else if (3 == var->get_cexp()->get_stamp() && 4 == exp->get_stamp()
				&& 0 == exp->get_cvar()->get_stamp()) {
			//如果数组下标为常数，且是简单变量赋值，例如a[2]=x;
			array_name = var->get_name();		//取出数组变量名
			i_name = var->get_cexp()->get_c();		//取出数组下标名
			exp_name = exp->get_cvar()->get_name();		//取出赋值变量的名称

			//对数组下标为常数、赋值表达式为简单变量的数组赋值运算进行编码
			gen_array_assign_II(array_name, str2int(i_name), exp_name, v);

		}
		//判断数组下标是否为简单变量，同时赋值表达式是否为常数
		else if (4 == var->get_cexp()->get_stamp()
				&& 0 == var->get_cexp()->get_cvar()->get_stamp()
				&& 3 == exp->get_stamp()) {
			//如果数组下标为简单变量，同时为常数赋值，例如a[x]=3;
			array_name = var->get_name();		//取出数组变量名
			i_name = var->get_cexp()->get_cvar()->get_name();		//取出数组下标变量名
			exp_name = exp->get_c();		//取出赋值常数的名字

			//对数组下标为简单变量、赋值表达式为常数的数组赋值运算进行编码
			gen_array_assign_III(array_name, i_name, str2int(exp_name), v);
		}
		//判断数组下标和赋值表达式是否均为简单变量
		else if (4 == var->get_cexp()->get_stamp()
				&& 0 == var->get_cexp()->get_cvar()->get_stamp()
				&& 4 == exp->get_stamp() && 0 == exp->get_cvar()->get_stamp()) {
			//如果数组下标和赋值表达式均为为简单变量，例如a[x]=y;
			array_name = var->get_name();		//取出数组变量名
			i_name = var->get_cexp()->get_cvar()->get_name();		//取出数组下标变量名
			exp_name = exp->get_cvar()->get_name();		//取出赋值变量的名字

			//对数组下标和赋值表达式均为简单变量的数组赋值运算进行编码
			gen_array_assign_IV(array_name, i_name, exp_name, v);
		} else {
			//其他情况的数组赋值均视为非法，抛出错误信息
			string error_info = "array assign statement is invalid!";
			throw error_info;
		}
	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding array assign : " + str;
		throw error_info;
	}
}
// 功能：对数组的引用进行编码，主要区分数组下标为常数或者变量，数组位于循环体内还是循环体外，数组
// 			是否为nodt类型这几种情况，按照算法和冗余代码规范输出数组引用的安全代码
// 输入参数：var_name-左值变量名
// 			      var-数组变量对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：倪嘉呈
void gen_array_refer(string var_name, Var* var, list<Var_info>* v) {
	try {
		//检查输入参数的有效性
		check_str_arg(var_name, "variable's name");		//检查参数：左值变量名字符串的有效性
		check_pt_arg(var, "array object's pointer");		//检查参数：数组变量指针的有效性
		check_pt_arg(v, "var_info list's pointer");		//检查参数：动态签名表指针的有效性

		const char* name = var_name.data();
		//判断是否是中间变量"AC_Tmp_"
		bool result = StrCompare("AC_Tmp_", name, 7);
		//判断右值变量是否为数组变量
		if (1 == var->get_stamp()) {
			//如果是数组变量，则判断数组的下标是否为常数
			if (3 == var->get_cexp()->get_stamp()) {
				//如果数组下标为常数，例如x=a[3];
				string array_name = var->get_name();		//取出数组名
				string i_name = var->get_cexp()->get_c();		//取出下标字符串名
				int high_i = str2int(i_name);		//将下标转换成整数

				int i_sig = getConstantSig(high_i);		//为下标分配签名
				long long low_i =
						(-((long long) high_i * K_NUM_P1) % P1 + i_sig) % P1;//计算下标的低位
				long long low_i2 = (-((long long) high_i * K_NUM_P2) % P2
						+ i_sig) % P2;
				int array_sig = get_current_sig(v, array_name, var_scope);//取出数组的签名

				int sig_index = 0;
				int new_sig = get_sig_index(&sig_index);		//存储左值的新签名

				string varl_name;
				bool varGlobal = is_global_var(var_name);
				if (varGlobal == true) {
					varl_name = get_bind_local_var(var_name, var_scope);
				} else {
					varl_name = var_name;
				}
				add_new_sig(v, varl_name, new_sig, var_scope);//将左值变量的新签名加入动态签名表

				int inComp1 = (((long long) new_sig - array_sig - i_sig) % P1
						+ P1) % P1;	//计算补偿常数
				int inComp2 = (((long long) new_sig - array_sig - i_sig) % P2
						+ P2) % P2;	//计算补偿常数

//				string inC1=int2HexStr(inComp1);转换为16进制
//				string inC2=int2HexStr(inComp2);

				//如果是调试模式，则输出调试信息：左值变量的新签名，下标的签名，数组的签名
				if (true == debug_flag) {
//					out_indent(indent_num, gen_out);
//					gen_out << "/*" << var_name << "'s new sig :" << new_sig
//							<< " " << i_name << "'s sig :" << i_sig << " "
//							<< array_name << "'s sig:" << array_sig << "*/"
//							<< endl << endl;
				}

//闪闪
				string high_h = int2HexStr_data(high_i);
				out_indent(indent_num, gen_out);
				gen_out << "AC_BIND_CONST_NUM_" << high_h
						<< ".Data.F = CONST_NUM_" << high_h << ".Data.F;"
						<< endl;
				out_indent(indent_num, gen_out);
				gen_out << "AC_BIND_CONST_NUM_" << high_h << ".Data.C.C1 = "
						<< "CONST_NUM_" << high_h << ".Data.C.C1"
						<< " + g_GS_TL1;" << endl;	//修正校验域1
				out_indent(indent_num, gen_out);
				gen_out << "AC_BIND_CONST_NUM_" << high_h << ".Data.C.C2 = "
						<< "CONST_NUM_" << high_h << ".Data.C.C2"
						<< " + g_GS_TL2;" << endl;	//修正校验域2
				out_indent(indent_num, gen_out);
				if (result == true) {
					//输出无TC
				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				string CSCArrayName = GetArrayNameInCSC(array_name);
				gen_out << varl_name << ".Data = F_VCL_ArrayRef(" << CSCArrayName
						<< "[AC_BIND_CONST_NUM_" << high_h
						<< ".Data.F].Data, AC_BIND_CONST_NUM_" << high_h
						<< ".Data, " << inComp1 << ", " << inComp2 << ");"
						<< endl;				//CON1和CON2均为16进制立即数
				out_indent(indent_num, gen_out);
				if (result == true) {

				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << varl_name << ".Id = F_VCL_RegisterCodeId(" << new_sig
						<< ");" << endl << endl;

			}
			//判断数组的下标是否为简单变量
			else if ((4 == var->get_cexp()->get_stamp())
					&& (0 == var->get_cexp()->get_cvar()->get_stamp())) {
				//如果数组变量下标为变量
				string array_name = var->get_name();				//取出数组变量名称
				string i_name = var->get_cexp()->get_cvar()->get_name();//取出下标名称
				const char* iname = i_name.data();
				bool indexGlobal = is_global_var(i_name);
				bool varGlobal = is_global_var(var_name);
				bool i_result = StrCompare("AC_Tmp_", iname, 7);
				//数组下标i_name 和左值变量var_name
				string il_name;
				string varl_name;
				if (indexGlobal == true) {
					il_name = get_bind_local_var(i_name, var_scope);
				} else {
					il_name = i_name;
				}
				if (varGlobal == true) {
					varl_name = get_bind_local_var(var_name, var_scope);
				} else {
					varl_name = var_name;
				}

				int i_sig = get_current_sig(v, il_name, var_scope);	//取出下标变量的签名
				int array_sig = get_current_sig(v, array_name, var_scope);//取出数组的签名

				int sig_index = 0;
				int new_sig = get_sig_index(&sig_index);	//分配新签名作为左值的签名
				add_new_sig(v, varl_name, new_sig, var_scope);//将左值变量的新签名加入动态签名表

				int inComp1 = (((long long) new_sig - array_sig - i_sig) % P1
						+ P1) % P1;	//计算补偿常数
				int inComp2 = (((long long) new_sig - array_sig - i_sig) % P2
						+ P2) % P2;	//计算补偿常数

				//				string inC1=int2HexStr(inComp1);转换为16进制
				//				string inC2=int2HexStr(inComp2);

				//如果是调试模式，则输出调试信息：左值变量的新签名，下标的签名，数组的签名
				if (true == debug_flag) {
					//					out_indent(indent_num, gen_out);
					//					gen_out << "/*" << var_name << "'s new sig :" << new_sig
					//							<< " " << i_name << "'s sig :" << i_sig << " "
					//							<< array_name << "'s sig:" << array_sig << "*/"
					//							<< endl << endl;
				}
				//闪闪

				out_indent(indent_num, gen_out);
				if (result == true) {

				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				string CSCArrayName = GetArrayNameInCSC(array_name);
				gen_out << varl_name << ".Data = F_VCL_ArrayRef(" << CSCArrayName
						<< "[";
				if (i_result == true) {

				} else {
					if (indexGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << il_name << ".Data.F].Data, ";
				if (i_result == true) {

				} else {
					if (indexGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << il_name << ".Data, " << inComp1 << ", " << inComp2
						<< ");" << endl;					//CON1和CON2均为16进制立即数
				out_indent(indent_num, gen_out);
				if (result == true) {

				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << varl_name << ".Id = F_VCL_RegisterCodeId(" << new_sig
						<< ");" << endl << endl;
				//end of 不是全局变量的操作

			} else {
				string error_info = "array's subscript is invalid!";
				throw error_info;
			}
		} else {
			string error_info = "it is not an array reference!";
			throw error_info;
		}

	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding array refer : " + str;
		throw error_info;
	}
}

