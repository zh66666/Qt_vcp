
#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// 定义在文件: src/main_utils.cpp
extern bool debug_flag;

extern long long i_temp_P1;

extern long long i_temp_P2;

//static long long i_temp_P1=0;//存储对乘法编码时用到的逆值
//static long long i_temp_P2=0;//存储对乘法编码时用到的逆值
//定义在文件 : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);
//定义在文件 : ./inverse.c
extern long long inverse(long long b,long long prime);
/*
 功能：差异化编码的乘法
 输入参数：l_1 左操作数的第一组编码值
	   l_2左操作数的第二组编码值
	   r_1 右操作数的第一组编码值
 	   r_2 右操作数的第二组编码值
	   var_name 运算结果值
 输出参数：
 返回值：
 作者：周航
*/
static void new_mul(string l_1,string l_2,string r_1,string r_2,string var_name)
{	int R;
	S_this=*++iter;
	R=S_this-S_last;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_A_1="<<l_1<<"*"<<r_1<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_A_2="<<l_1<<"*"<<r_2<<"+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_B_1="<<l_2<<"*"<<r_1<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"mul_temp_B_2="<<l_2<<"*"<<r_2<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_1=(mul_temp_A_1+mul_temp_A_2-("<<R<<"))>>14;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2=((mul_temp_B_1-mul_temp_B_2)>>1)+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
	gen_out<<endl;
}
// 功能：对左右操作数都为变量的乘法运算进行编码，属于乘法运算编码的子模块。输出乘法运算的高位代码
// 			和低位代码，其中低位区分循环体内与循环体外，编码时分配和更新签名
// 输入参数：var_name-左值变量名
//			      l_name-左操作数变量名
//			      r_name-右操作数变量名
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void red_mul_I(string var_name,string l_name,string r_name,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_str_arg(l_name,"left operator's name");
		check_str_arg(r_name,"right operator's name");
		check_pt_arg(v,"var_info list's pointer");
		
		int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//取出左操作数变量的签名
		int right_sig=get_current_sig(v,get_global_string_sig(r_name,var_scope),var_scope);//取出右操作数变量的签名
		
		int const_1_1=((-(long long)2*(long long)left_sig)%P1+P1)%P1;//补偿常数1：左值签名的2倍
		int const_2_1=((-(long long)2*(long long)right_sig)%P1+P1)%P1;//补偿常数2：右值签名的2倍
		
		int const_1_2=((-(long long)2*(long long)left_sig)%P2+P2)%P2;//补偿常数1：左值签名的2倍
		int const_2_2=((-(long long)2*(long long)right_sig)%P2+P2)%P2;//补偿常数2：右值签名的2倍

		int new_sig=0;//存储左值变量的新签名
		int const_3_1=0;//存储在循环外部对低位编码时的签名补偿常数,有限域为P1
		int const_3_2=0;//存储在循环外部对低位编码时的签名补偿常数,有限域为P2
		//string str_const_3="";//存储签名补偿常数字符串
		//位于循环体和非循环体，则为左值变量分配新签名，并计算签名补偿常数
		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		const_3_1=(((long long)new_sig-(((long long)left_sig*(long long)right_sig)%P1*i_temp_P1)%P1*2)%P1+P1)%P1;
		const_3_2=(((long long)new_sig-(((long long)left_sig*(long long)right_sig)%P2*i_temp_P2)%P2*2)%P2+P2)%P2;
		//str_const_3=" + "+int2str(const_3);//设置补偿常数字符串
		
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
		
		//输出乘法编码的高位代码
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
		string r_name_head=TmpVarJudge(r_name)?r_name:get_global_string(r_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Mul("<<l_name_head<<".Data, "<<r_name_head
				<<".Data, "<<""<<int2HexStr(left_sig)<<", "<<int2HexStr(right_sig)<<", "
				<<int2HexStr(const_3_1)<<", "<<int2HexStr(const_3_2)<<");"<<endl;
		//输出签名Id调整语句
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;
		//如果是调试模式，则输出调试信息：左值的签名，左操作数和右操作数的签名
		if(true==debug_flag)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<l_name
			<<"'s sig :"<<left_sig<<" "<<r_name<<"'s sig :"<<right_sig<<endl<<endl;
		}
		/*red_Sub_VVVV(red_Tmp3_INT32, "L_"+l_name, "DT", int2str(const_1));
		out_indent(indent_num,gen_out);
		red_Sub_VVV(red_Tmp4_INT32,"DT", "L_"+r_name);
		out_indent(indent_num,gen_out);
		gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<red_Tmp3_INT32
		<<" * "<<red_Tmp4_INT32<<";"<<endl;
		out_indent(indent_num,gen_out);
		red_INT64MOD(red_Tmp1_INT32, red_Tmp_INT64);
		out_indent(indent_num,gen_out);
		red_Sub_VVVV(red_Tmp4_INT32, "L_"+r_name, "DT",int2str(const_2));
		out_indent(indent_num,gen_out);
		red_Sub_VVV(red_Tmp3_INT32, "DT", "L_"+l_name);
		out_indent(indent_num,gen_out);
		gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<red_Tmp3_INT32
		<<" * "<<red_Tmp4_INT32<<";"<<endl;
		out_indent(indent_num,gen_out);
		red_INT64MOD(red_Tmp2_INT32, red_Tmp_INT64);
		out_indent(indent_num,gen_out);
		red_Add_VVV("L_"+var_name, red_Tmp1_INT32, red_Tmp2_INT32);
		out_indent(indent_num,gen_out);
		gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")L_"<<var_name<<" * "
		<<i_temp<<str_const_3<<" + DT;"<<endl;
		out_indent(indent_num,gen_out);
		red_INT64MOD("L_"+var_name, red_Tmp_INT64);*/
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding sub operation (operators are both variable) : "+str;
		throw error_info;
	}
}

// 功能：对左右操作数中有一个为常数，一个为变量的乘法运算进行编码，属于乘法运算编码的子模块。
// 			输出乘法运算的低位代码，不区分循环体内与循环体外，编码时分配和更新签名
// 输入参数：var_name-左值变量名
//			      right_var-变量操作数的名字
//			      right_const-常数操作数的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void red_mul_II(string var_name,string left_var,int right_const ,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name, "left variable's name");
		check_str_arg(left_var, "varibale operator's name");
		check_pt_arg(v, "var_info list's pointer");
		int const_sig = getConstantSig(right_const);		//为常数操作数分配新签名
		int var_sig = get_current_sig(v, get_global_string_sig(left_var,var_scope), var_scope);		//取出变量操作数的签名
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		//int new_sig = get_sig();		//为左值变量分配新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表

		int constant1 = 0;		//存储在循环外部对低位编码时的签名补偿常数
		int constant2 = 0;		//存储在循环外部对低位编码时的签名补偿常数
		constant1 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P1 * i_temp_P1)
						% P1 * 2) % P1 + P1) % P1;
		constant2 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P2 * i_temp_P2)
						% P2 * 2) % P2 + P2) % P2;
		//计算低位代码中的补偿常数
		//int constant=((((long long)right_const*(long long)var_sig)%A-(long long)new_sig)%A+A)%A;
		//对立即数初始化
		constant2Str(right_const);
		//输出乘法编码的高位代码
		out_indent(indent_num, gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(left_var)?left_var:get_global_string(left_var,var_scope);
		gen_out << var_head  << ".Data = " << "F_VCL_Mul("<<l_name_head
			 << ".Data, AC_BIND_CONST_NUM_" << constantBindedStr(right_const)
				<< ".Data, " << "" << int2HexStr(var_sig) << ", " << int2HexStr(const_sig) << ", "
				<< int2HexStr(constant1) << ", " << int2HexStr(constant2) << ");" << endl;
		//输出签名Id调整语句
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;
		/*	//输出乘法运算的低位代码
		 out_indent(indent_num,gen_out);
		 red_Sub_VVV(red_Tmp3_INT32, "L_"+right_var, "DT");
		 out_indent(indent_num,gen_out);
		 gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<right_const
		 <<" * "<<red_Tmp3_INT32<<";"<<endl;
		 out_indent(indent_num,gen_out);
		 red_INT64MOD(red_Tmp3_INT32, red_Tmp_INT64);
		 out_indent(indent_num,gen_out);
		 red_Sub_VVVV("L_"+var_name, red_Tmp3_INT32, int2str(constant), "DT");*/

		//如果是调试模式，则输出调试信息：左值的签名，左操作数和右操作数的签名
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//" << var_name << "'s new sig :" << new_sig << " "
					<< left_var << "'s sig :" << var_sig << " "<<right_const
					<<"'s sig :"<<const_sig<<endl << endl;
		}

	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding mul operation (one operator is variable,one is constant) : "+str;
		throw error_info;
	}
}

static void red_mul_III(string var_name,int left_const,string right_var ,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name, "left variable's name");
		check_str_arg(right_var, "varibale operator's name");
		check_pt_arg(v, "var_info list's pointer");
		int const_sig = getConstantSig(left_const);		//为常数操作数分配新签名
		int var_sig = get_current_sig(v, get_global_string_sig(right_var,var_scope), var_scope);		//取出变量操作数的签名
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		//int new_sig = get_sig();		//为左值变量分配新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
		int constant1 = 0;		//存储在循环外部对低位编码时的签名补偿常数
		int constant2 = 0;		//存储在循环外部对低位编码时的签名补偿常数
		constant1 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P1 * i_temp_P1)
						% P1 * 2) % P1 + P1) % P1;
		constant2 = (((long long) new_sig
				- (((long long) var_sig * (long long) const_sig) % P2 * i_temp_P2)
						% P2 * 2) % P2 + P2) % P2;
		//计算低位代码中的补偿常数
		//int constant=((((long long)right_const*(long long)var_sig)%A-(long long)new_sig)%A+A)%A;
		//对立即数初始化
		constant2Str(left_const);
		//输出乘法编码的高位代码
		out_indent(indent_num, gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string r_name_head=TmpVarJudge(right_var)?right_var:get_global_string(right_var,var_scope);
		/*gen_out << var_head << (var_name) << ".Data= " << "F_VCL_Mul("<<r_name_head
				<< (right_var) << ".Data,AC_BIND_CONST_NUM_" << (left_const)
				<< ".Data," << "" << int2HexStr(var_sig) << "," << int2HexStr(const_sig) << ","
				<< int2HexStr(constant1) << "," << int2HexStr(constant2) << ");" << endl;*/
		gen_out << var_head  << ".Data = "
				<< "F_VCL_Mul(AC_BIND_CONST_NUM_" << constantBindedStr(left_const) << ".Data, "
				<< r_name_head  << ".Data, " << ""
				<<int2HexStr(const_sig)  << ", " <<  int2HexStr(var_sig)<< ", "
				<< int2HexStr(constant1) << ", " << int2HexStr(constant2) << ");"
				<< endl;
		//输出签名Id调整语句
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;
		/*	//输出乘法运算的低位代码
		 out_indent(indent_num,gen_out);
		 red_Sub_VVV(red_Tmp3_INT32, "L_"+right_var, "DT");
		 out_indent(indent_num,gen_out);
		 gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<right_const
		 <<" * "<<red_Tmp3_INT32<<";"<<endl;
		 out_indent(indent_num,gen_out);
		 red_INT64MOD(red_Tmp3_INT32, red_Tmp_INT64);
		 out_indent(indent_num,gen_out);
		 red_Sub_VVVV("L_"+var_name, red_Tmp3_INT32, int2str(constant), "DT");*/

		//如果是调试模式，则输出调试信息：左值的签名，左操作数和右操作数的签名
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//" << var_name << "'s new sig :" << new_sig << " "
					<< right_var << "'s sig :" << var_sig << " "<<left_const
					<<"'s sig :"<<const_sig<<endl << endl;
		}

	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding mul operation (one operator is variable,one is constant) : "+str;
		throw error_info;
	}
}

static void red_mul_IIII(string var_name,int left_const,int right_const,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name, "left variable's name");
		check_pt_arg(v, "var_info list's pointer");

		int left_sig = getConstantSig(left_const);		//为常数操作数分配新签名
		int right_sig = getConstantSig(right_const);		//为常数操作数分配新签名

		int new_sig = 0;		//存储左值变量的新签名
		int constant1 = 0;		//存储在循环外部对低位编码时的签名补偿常数
		int constant2 = 0;		//存储在循环外部对低位编码时的签名补偿常数
		//string str_const_3="";//存储签名补偿常数字符串
		//位于循环体和非循环体，则为左值变量分配新签名，并计算签名补偿常数
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		//new_sig = get_sig();
		constant1 = (((long long) new_sig
				- (((long long) left_sig * (long long) right_sig) % P1 * i_temp_P1)
						% P1 * 2) % P1 + P1) % P1;
		constant2 = (((long long) new_sig
				- (((long long) left_sig * (long long) right_sig) % P2 * i_temp_P2)
						% P2 * 2) % P2 + P2) % P2;

		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);		//将左值变量新签名加入动态签名表
		//对立即数初始化
		constant2Str(left_const);
		constant2Str(right_const);

		//输出乘法编码的高位代码
		out_indent(indent_num, gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		gen_out << var_head << ".Data = "
				<< "F_VCL_Mul(AC_BIND_CONST_NUM_" << constantBindedStr(left_const)
				<< ".Data, AC_BIND_CONST_NUM_" << constantBindedStr(right_const) << ".Data, "
				<< "" << int2HexStr(left_sig) << ", " << int2HexStr(right_sig) << ", " << int2HexStr(constant1)
				<< ", " << int2HexStr(constant2) << ");" << endl;
		//输出签名Id调整语句
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;
		//如果是调试模式，则输出调试信息：左值的签名，左操作数和右操作数的签名
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//" << var_name << "'s new sig :" << new_sig << " "
					<< left_const << "'s sig :" << left_sig << " "
					<< right_const << "'s sig :" << right_sig << endl << endl;
		}

	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info =
				"coding sub operation (operators are both variable) : " + str;
		throw error_info;
	}
}


// 功能：对乘法运算进行编码，属于乘法运算编码的主模块。输出乘法运算的高位代码和低位代码，
//			根据左右操作数为变量或者常数的不同，分四种情况进行编码
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_mul(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		string l_1;
		string l_2;
		string r_1;
		string r_2;
		int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		if(left==4)
		{
			l_1=exp->get_cel()->get_cvar()->get_name()+"_1";
			l_2=exp->get_cel()->get_cvar()->get_name()+"_2";
		}
		else
		{
			l_1=exp->get_cel()->get_c()+"*A1";
			l_2=exp->get_cel()->get_c()+"*A2";
		}
		if(right==4)
		{
			r_1=exp->get_cer()->get_cvar()->get_name()+"_1";
			r_2=exp->get_cer()->get_cvar()->get_name()+"_2";
		}
		else
		{
			r_1=exp->get_cer()->get_c()+"*A1";
			r_2=exp->get_cer()->get_c()+"*A2";
		}

		new_mul(l_1,l_2,r_1,r_2,var_name);
		

		//i_temp_P1=inverse(8589934592ll,(long long)P1);//求2^33在P1中逆 K值有关参数
		//i_temp_P2=inverse(8589934592ll,(long long)P2);//求2^33在P2中逆
		//判断左右操作数是否均为变量
		/*if((4==left)&&(4==right))
		{	
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数的名字
			string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字
			//对左右操作数都为变量的乘法运算进行编码
			gen_out<<var_name<<"_1="<<"mul_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A2);"<<endl;
			//red_mul_I(var_name,l_name,r_name,v);
		}
		//判断左操作数是否为变量，右操作数是否为常数
		else if((4==left)&&(3==right))
		{
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数名字
			string right_name=exp->get_cer()->get_c();//取出右操作数的名字
			int right_const=str2int(right_name);//将右操作数转换为整数
			//将常数操作数转换为字符串
			string str_const=(right_const>=0)?(right_name):(" ("+right_name+")");
			//输出乘法编码的高位代码
			out_indent(indent_num,gen_out);
			gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" * "<<str_const<<";"<<endl;
			
			//输出低位代码
			gen_out<<var_name<<"_1="<<"mul_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A2);"<<endl;
			//red_mul_II(var_name,l_name,right_const,v);
		}
		//判断左操作数是否为常数，右操作数是否为变量
		else if((3==left)&&(4==right))
		{
			//如果是，则进行编码
			string left_const=exp->get_cel()->get_c();//取出左操作数常数的名字
			string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字
			
			//输出乘法编码的高位代码
			out_indent(indent_num,gen_out);
			gen_out<<"H_"<<var_name<<" = "<<left_name<<" * H_"<<r_name<<";"<<endl;
			
			//输出低位代码
			gen_out<<var_name<<"_1="<<"mul_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A2);"<<endl;
			//red_mul_III(var_name,str2int(left_const),r_name,v);
		}
		//判断左右操作数是否均为常数
		else if((3==left)&&(3==right))
		{
			//如果左右操作数均为常数，则将其积计算出来
			string left_name=exp->get_cel()->get_c();//取出左操作数的名字
			string right_name=exp->get_cer()->get_c();//取出右操作数的名字
			//int high=str2int(left_name)+str2int(right_name);//计算出左右操作数之积
			//red_assign_const(var_name,high,v);//调用常数赋值编码函数进行编码
			gen_out<<var_name<<"_1="<<"mul_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A1);"<<endl;
			gen_out<<var_name<<"_2="<<"mul_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A2);"<<endl;
			//red_mul_IIII(var_name,str2int(left_name),str2int(right_name),v);
		}
		else
		{
			//操作数为其他情况时，抛出错误信息
			string error_info="operand is invalid!";
			throw error_info;
		}*/
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding mul operation : "+str;
		throw error_info;
	}
}



