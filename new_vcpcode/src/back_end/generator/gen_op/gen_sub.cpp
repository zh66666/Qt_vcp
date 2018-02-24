
#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// 定义在文件: src/main_utils.cpp
extern bool debug_flag;

//定义在文件 : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);
/*功能：差异化编码的减法
输入参数：l_1 左操作数的第一组编码值
	   l_2左操作数的第二组编码值
	   r_1 右操作数的第一组编码值
	   r_2 右操作数的第二组编码值
	   var_name 运算结果值
输出参数：
返回值：
作者：周航
*/
static void new_sub(string l_1,string l_2,string r_1,string r_2,string var_name)
{	int R;
	S_this=*++iter;
	R=S_this-S_last;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_1="<<l_1<<"-"<<r_1<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<l_2<<"-"<<r_2<<"+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
	gen_out<<endl;

}
// 功能：对左右操作数都为变量的减法运算进行编码，属于减法运算编码的子模块。输出减法运算的高位代码
// 			和低位代码，其中低位区分循环体内与循环体外，编码时分配和更新签名
// 输入参数：var_name-左值变量名
//			      l_name-左操作数变量名
//			      r_name-右操作数变量名
// 			     v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void red_sub_I(string var_name,string l_name,string r_name,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_str_arg(l_name,"left operator's name");
		check_str_arg(r_name,"right operator's name");
		check_pt_arg(v,"var_info list's pointer");
		
		//int left_sig=get_current_sig(v,l_name,var_scope);//取出左操作数变量的签名
		//int right_sig=get_current_sig(v,r_name,var_scope);//取出左操作数变量的签名
		int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);
		int right_sig=get_current_sig(v,get_global_string_sig(r_name,var_scope),var_scope);//取出右操作数变量的签名
		
		//输出减法编码的高位代码
		/*out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" - H_"<<r_name<<";"<<endl;*/
		
		int new_sig=0;//存储左值变量的新签名
		int constant1=0;//存储低位代码中的补偿常数
		int constant2=0;//存储低位代码中的补偿常数
			//不位于循环体和位于循环体，则为左值变量分配新签名，并计算补偿常数
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
		//new_sig=get_sig();
			constant1=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
			                 %(long long)P1+(long long)P1)%(long long)P1;
			constant2=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
						                 %(long long)P2+(long long)P2)%(long long)P2;
			//输出减法运算在循环体外的低位代码
			out_indent(indent_num,gen_out);
		/*	red_Sub_VVVVV("L_"+var_name, "L_"+l_name,
                                			"L_"+r_name, int2str(constant), "DT");*/
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			string r_name_head=TmpVarJudge(r_name)?r_name:get_global_string(r_name,var_scope);
			gen_out<<var_head<<".Data = "<<"F_VCL_Sub("<<l_name_head<<".Data, "<<r_name_head
								<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
		
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding sub operation (operators are both variable) : "+str;
		throw error_info;
	}
}

// 功能：对左操作数为变量，右操作数为常数的减法运算进行编码，属于减法运算编码的子模块。
// 			输出减法运算的高位代码和低位代码，其中低位区分循环体内与循环体外，编码时分配和更新签名
// 输入参数：var_name-左值变量名
//			      left_name-左操作数变量的名字
//			      right_const-右操作数常数的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void red_sub_II(string var_name,string left_name,int right_const ,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_str_arg(left_name,"left operand's name");
		check_pt_arg(v,"var_info list's pointer");
		
		int left_sig=get_current_sig(v,get_global_string_sig(left_name,var_scope),var_scope);//取出左操作数的签名
		//int const_sig=get_sig();//为右操作数分配新签名
		int const_sig=getConstantSig(right_const);//为常数操作数分配新签名
		//计算右操作数的低位
		//long long const_low=-((long long)(right_const)*(long long)K_NUM)%A+(long long)const_sig;
		
		int new_sig=0;//存储左值变量的新签名
		int constant1=0;//存储低位代码中的补偿常数
		int constant2=0;//存储低位代码中的补偿常数
/*		string str_const=(right_const>=0)?(" - "+int2str(right_const))://将常数操作数转换为字符串
                                       (" - ("+int2str(right_const)+")");
		
		//输出减法编码的高位代码
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = H_"<<left_name<<str_const<<";"<<endl;*/

		//如果不位于循环体，则为左值变量分配新签名，并计算补偿常数
		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表

		//constant=(((long long)new_sig-(long long)left_sig+(long long)const_sig-(long long)const_low)
		//				  %(long long)A+(long long)A)%(long long)A;
		constant1=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
					                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
							                 %(long long)P2+(long long)P2)%(long long)P2;
	/*	//输出减法运算在循环体外的低位代码
		out_indent(indent_num,gen_out);
		red_Add_VVI("L_"+var_name, "L_"+left_name,int2str(constant));
		*/
		//对立即数初始化
		constant2Str(right_const);
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(left_name)?left_name:get_global_string(left_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub("<<l_name_head<<".Data, AC_BIND_CONST_NUM_"<<constantBindedStr(right_const)
					<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_name
			<<"'s sig :"<<left_sig<<" "<<right_const<<"'s sig :"<<const_sig<<endl<<endl;
		}
		

	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding sub operation (left operator is variable,right is constant) : "+str;
		throw error_info;
	}
}

static void red_sub_II(string var_name,string left_name,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_str_arg(left_name,"left operand's name");
		check_pt_arg(v,"var_info list's pointer");

		int left_sig=get_current_sig(v,get_global_string_sig(left_name,var_scope),var_scope);//取出左操作数的签名
		//int const_sig=get_sig();//为右操作数分配新签名
		int const_sig=Bool_True_Sig;//为常数操作数分配新签名
		//计算右操作数的低位
		//long long const_low=-((long long)(right_const)*(long long)K_NUM)%A+(long long)const_sig;

		int new_sig=0;//存储左值变量的新签名
		int constant1=0;//存储低位代码中的补偿常数
		int constant2=0;//存储低位代码中的补偿常数
/*		string str_const=(right_const>=0)?(" - "+int2str(right_const))://将常数操作数转换为字符串
                                       (" - ("+int2str(right_const)+")");

		//输出减法编码的高位代码
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = H_"<<left_name<<str_const<<";"<<endl;*/

		//如果不位于循环体，则为左值变量分配新签名，并计算补偿常数
		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表

		//constant=(((long long)new_sig-(long long)left_sig+(long long)const_sig-(long long)const_low)
		//				  %(long long)A+(long long)A)%(long long)A;
		constant1=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
					                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)left_sig+(long long)const_sig)
							                 %(long long)P2+(long long)P2)%(long long)P2;
	/*	//输出减法运算在循环体外的低位代码
		out_indent(indent_num,gen_out);
		red_Add_VVI("L_"+var_name, "L_"+left_name,int2str(constant));
		*/
		//对立即数初始化
		out_indent(indent_num,gen_out);
		gen_out<<"AC_BIND_CONST_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;"<<endl;

		out_indent(indent_num,gen_out);
		gen_out<<"AC_BIND_CONST_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;"<<endl;

		out_indent(indent_num,gen_out);
		gen_out<<"AC_BIND_CONST_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;"<<endl;
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string l_name_head=TmpVarJudge(left_name)?left_name:get_global_string(left_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub("<<l_name_head<<".Data, AC_BIND_CONST_BOOL_TRUE"
					<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;

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
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_name
			<<"'s sig :"<<left_sig<<" "<<"TRUE"<<"'s sig :"<<const_sig<<endl<<endl;
		}


	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding sub operation (left operator is variable,right is constant) : "+str;
		throw error_info;
	}
}

// 功能：对左操作数为常数，右操作数为变量的减法运算进行编码，属于减法运算编码的子模块。
// 			输出减法运算的高位代码和低位代码，其中低位区分循环体内与循环体外，编码时分配和更新签名
// 输入参数：var_name-左值变量名
//			      left_const-左操作数常数的值
//			      right_name-右操作数变量的名字
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void red_sub_III(string var_name,int left_const,string right_name ,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_str_arg(right_name,"right operand's name");
		check_pt_arg(v,"var_info list's pointer");
		
		int right_sig=get_current_sig(v,get_global_string_sig(right_name,var_scope),var_scope);//取出右操作数的签名
		//int const_sig=get_sig();//为左操作数分配新签名
		int const_sig=getConstantSig(left_const);//为常数操作数分配新签名
		//计算左操作数的低位
		//long long const_low=-((long long)(left_const)*(long long)K_NUM)%(long long)A+(long long)const_sig;
		
		int new_sig=0;//存储左值变量的新签名
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
		int constant1=0;//存储低位代码中的补偿常数
		int constant2=0;//存储低位代码中的补偿常数
		constant1=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
							                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
									                 %(long long)P2+(long long)P2)%(long long)P2;

		//new_sig=get_sig();
		//constant=(((long long)new_sig+(long long)right_sig-(long long)const_sig
		//				  +(long long)const_low)%(long long)A+(long long)A)%(long long)A;
		/*//输出减法编码的高位代码
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = "<<left_const<<" - H_"<<right_name<<";"<<endl;
		
		//输出减法运算的低位代码
		out_indent(indent_num,gen_out);
		red_Sub_VVVVV("L_"+var_name, int2str(constant), "L_"+right_name, "DT", "DT");
		*/

		/*//对立即数初始化
		constant2Str(left_const);
		out_indent(indent_num,gen_out);
		gen_out<<"TC_"<<(var_name)<<".Data= "<<"F_VCL_Sub(AC_BIND_CONST_NUM_"<<(left_const)<<".Data,TC_"<<right_name
					<<".Data,("<<(constant1)<<"),("<<(constant2)<<"));"<<endl;*/

		//对立即数初始化
		constant2Str(left_const);
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		string r_name_head=TmpVarJudge(right_name)?right_name:get_global_string(right_name,var_scope);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub(AC_BIND_CONST_NUM_"<<constantBindedStr(left_const)<<".Data, "<<r_name_head
					<<".Data, " <<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_const
			<<"'s sig :"<<const_sig<<" "<<right_name<<"'s sig :"<<right_sig<<endl<<endl;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding sub operation (left operator is constant,right is variable) : "+str;
		throw error_info;
	}
}
// 功能：对左右操作数均为常数的减法运算进行编码，属于减法运算编码的子模块。
// 输入参数：var_name-左值变量名
//			      left_const-左操作数常数的值
//			      right_const-右操作数常量的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void red_sub_IIII(string var_name,int left_const,int right_const ,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(v,"var_info list's pointer");
		int left_sig=getConstantSig(left_const);
		int right_sig=getConstantSig(right_const);
		int new_sig=0;//存储左值变量的新签名
		int constant1=0;//存储低位代码中的补偿常数
		int constant2=0;//存储低位代码中的补偿常数

		//new_sig=get_sig();
		int SigID=0;
		new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		//constant=(((long long)new_sig+(long long)right_sig-(long long)const_sig
		//				  +(long long)const_low)%(long long)A+(long long)A)%(long long)A;
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
		constant1=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
									                 %(long long)P1+(long long)P1)%(long long)P1;
		constant2=(((long long)new_sig-(long long)left_sig+(long long)right_sig)
													 %(long long)P2+(long long)P2)%(long long)P2;
		/*//输出减法编码的高位代码
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = "<<left_const<<" - H_"<<right_name<<";"<<endl;

		//输出减法运算的低位代码
		out_indent(indent_num,gen_out);
		red_Sub_VVVVV("L_"+var_name, int2str(constant), "L_"+right_name, "DT", "DT");
		*/
		//对立即数初始化
		constant2Str(left_const);
		constant2Str(right_const);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		out_indent(indent_num,gen_out);
		gen_out<<var_head<<".Data = "<<"F_VCL_Sub(AC_BIND_CONST_NUM_"<<constantBindedStr(left_const)<<".Data, AC_BIND_CONST_NUM_"<<constantBindedStr(right_const)
					<<".Data, "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<left_const
			<<"'s sig :"<<left_sig<<" "<<right_const<<"'s sig :"<<right_sig<<endl<<endl;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding sub operation (left operator is constant,right is variable) : "+str;
		throw error_info;
	}
}
// 功能：对减法运算进行编码，属于减法运算编码的主模块。输出减法运算的高位代码和低位代码，
//			根据左右操作数为变量或者常数的不同，分四种情况进行编码
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_sub(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//检查参数：左值变量名称字符串的有效性
		check_str_arg(var_name,"left variable's name");
		//检查参数：赋值表达式对象指针的有效性
		check_pt_arg(exp,"expression object's pointer");
		//检查参数：动态签名表指针的有效性
		check_pt_arg(v,"var_info list's pointer");
		int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
				int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
				//判断左右操作数是否均为变量
				string l_1;
				string l_2;
				string r_1;
				string r_2;
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
				new_sub(l_1,l_2,r_1,r_2,var_name);
		/*int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		//判断左右操作数是否均为变量
		if((4==left)&&(4==right))
		{	
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数的名字
			string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字
			//对左右操作数都为变量的减法运算进行编码
			gen_out<<var_name<<"_1="<<"sub_enc("<<l_name<<"_1,"<<r_name<<"_1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<l_name<<"_2,"<<r_name<<"_2"<<");"<<endl;
			//red_sub_I(var_name,l_name,r_name,v);
		}
		//判断左操作数是否为变量，右操作数是否为常数
		else if((4==left)&&(3==right))
		{
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数名字
			string right_name=exp->get_cer()->get_c();//取出右操作数的名字
			//对左操作数为变量，右操作数为常数的减法运算进行编码
			gen_out<<var_name<<"_1="<<"sub_enc("<<l_name<<"_1,"<<str2int(right_name)<<"*A1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<l_name<<"_2,"<<str2int(right_name)<<"*A2"<<");"<<endl;
			if(right_name=="e_TRUE")
				red_sub_II(var_name,l_name,v);
			else
				red_sub_II(var_name,l_name,str2int(right_name),v);

		}
		//判断左操作数是否为常数，右操作数是否为变量
		else if((3==left)&&(4==right))
		{
			//如果是，则进行编码
			string left_name=exp->get_cel()->get_c();//取出左操作数常数的名字
			string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字

			//对左操作数为常数，右操作数为变量的减法运算进行编码
			gen_out<<var_name<<"_1="<<"sub_enc("<<str2int(left_name)<<"*A1,"<<r_name<<"_1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<str2int(left_name)<<"*A2,"<<r_name<<"_2"<<");"<<endl;
			cout<<1;
			//red_sub_III(var_name,str2int(left_name),r_name,v);
		}
		else if((3==left)&&(3==right))
		{
			//如果左右操作数均为常数，则将其差计算出来
			string left_name=exp->get_cel()->get_c();//取出左操作数的名字
			string right_name=exp->get_cer()->get_c();//取出右操作数的名字
			gen_out<<var_name<<"_1="<<"sub_enc("<<str2int(left_name)<<"*A1,"<<str2int(right_name)<<"*A1"<<");"<<endl;
			gen_out<<var_name<<"_2="<<"sub_enc("<<str2int(left_name)<<"*A2,"<<str2int(right_name)<<"*A2"<<");"<<endl;
			//red_sub_IIII(var_name,str2int(left_name),str2int(right_name),v);
			//int high=str2int(left_name)-str2int(right_name);//计算出左右操作数之差
			//red_assign_const(var_name,high,v);//调用常数赋值编码函数进行编码
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
		string error_info="coding sub operation : "+str;
		throw error_info;
	}
}

