#include "gen_utils.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// 定义在文件: src/main_utils.cpp
extern bool debug_flag;

// 功能：对常数赋值运算进行编码，输出运算的高位代码和低位代码，循环内与循环外采用同样的编码方法
// 输入参数：var_name-左值变量名
//			      high-常数的值
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚修改by周航
void red_assign_const(string var_name,int high,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(v,"var_info list's pointer");
		int R;
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_1="<<high<<"*A1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<high<<"*A2+("<<R<<");"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
		gen_out<<endl;

		//int const_sig=get_sig();//为常数分配一个签名
/*		int const_sig=getConstantSig(high);
		constantMap[high]=const_sig;
		int sig_num_0=getConstantSig(0);//为0分配一个签名，Z=1==>Z=1+0;
		constantMap[0]=sig_num_0;
		long long low1=-((long long)high*K_NUM_P1)%P1+(long long)const_sig;//计算常数的低位
		long long low2=-((long long)high*K_NUM_P2)%P2+(long long)const_sig;//计算常数的低位
		int SigID=0;
		int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
		//int new_sig=get_sig();//为左值变量分配一个新签名
		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将新签名添加到动态签名表中
		//计算低位代码中的补偿常数
		int constant1=(((long long)new_sig-(long long)const_sig-(long long)sig_num_0)%(long long)P1+(long long)P1)%(long long)P1;
		int constant2=(((long long)new_sig-(long long)const_sig-(long long)sig_num_0)%(long long)P2+(long long)P2)%(long long)P2;

			
		//输出常数赋值运算的高位代码
		out_indent(indent_num,gen_out);
		gen_out<<"H_"<<var_name<<" = "<<high<<";"<<endl;

		//输出常数赋值运算的低位代码
		out_indent(indent_num,gen_out);
		gen_out<<"L_"<<var_name<<" = "<<constant<<" + DT;"<<endl;
		constant2Str(high);
		out_indent(indent_num,gen_out);
		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
		gen_out << var_head  << ".Data = " << "F_VCL_Assign(AC_BIND_CONST_NUM_"
				<< constantBindedStr(high) << ".Data, " << int2HexStr(constant1) << ", "
				<< int2HexStr(constant2) << ");" << endl;
		//输出签名Id调整语句
		string LeftVar=var_head;
		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
				int2HexStr(SigID).c_str(),NULL);
		out_indent(indent_num,gen_out);
		gen_out<<CodedFuncCallStr<<endl;*/
	/*	//如果是调试模式，则输出调试信息：左值的签名，常数的签名
		if(true==debug_flag)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<high<<"'s sig :"
			<<const_sig<<endl<<endl;
		}*/
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding const assign operation : "+str;
		throw error_info;
	}
	
}

// 功能：对变量赋值运算进行编码，输出运算的高位代码和低位代码，循环内与循环外采用同样的编码方法
// 输入参数：var_name-左值变量名
//			      right_name-表达式变量名
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_assign_var(string var_name, string right_name,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_str_arg(right_name,"right variable's name");
		check_pt_arg(v,"var_info list's pointer");
		int R;
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_1="<<right_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<right_name<<"_2+("<<R<<");"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
		gen_out<<endl;
//		int var_sig=get_current_sig(v,get_global_string_sig(right_name,var_scope),var_scope);//取出赋值变量的签名
//		int SigID=0;
//		int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
//		//int new_sig=get_sig();//为左值变量分配新签名
//		add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
//		int sig_num_0=getConstantSig(0);//为0分配一个签名，z=a==>z=a+0;
//		constantMap[0]=sig_num_0;
//		int constant1=(((long long)new_sig-(long long)var_sig-(long long)sig_num_0)%(long long)P1+(long long)P1)%(long long)P1;//计算低位用到的补偿常数
//		int constant2=(((long long)new_sig-(long long)var_sig-(long long)sig_num_0)%(long long)P2+(long long)P2)%(long long)P2;//计算低位用到的补偿常数
//
//		/*//输出变量赋值的高位代码
//		out_indent(indent_num,gen_out);
//		gen_out<<"H_"<<var_name<<" = H_"<<right_name<<";"<<endl;
//
//		//输出变量赋值的低位代码
//		out_indent(indent_num,gen_out);
//		red_Add_VVI("L_"+var_name, "L_"+right_name, int2str(constant));*/
//		out_indent(indent_num,gen_out);
//		string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
//		string r_name_head=TmpVarJudge(right_name)?right_name:get_global_string(right_name,var_scope);
//
//		gen_out << var_head << ".Data = " << "F_VCL_Assign("
//				<< r_name_head << ".Data, " << int2HexStr(constant1) << ", "
//				<< int2HexStr(constant2) << ");" << endl;
//		//输出签名Id调整语句
//		string LeftVar=var_head;
//		string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
//				int2HexStr(SigID).c_str(),NULL);
//		out_indent(indent_num,gen_out);
//		gen_out<<CodedFuncCallStr<<endl;
//
//		//如果是调试模式，则输出调试信息：左值的签名，常数的签名
//		if(true==debug_flag)
//		{
//			out_indent(indent_num,gen_out);
//			gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<right_name<<"'s sig :"
//			<<var_sig<<endl<<endl;
//		}

	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding variable assign operation : "+str;
		throw error_info;
	}
}


