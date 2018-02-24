
#include "gen_utils.h"
#include "expression.h"
#include "bool_var.h"
#include "var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// 定义在文件: src/main_utils.cpp
extern bool debug_flag;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;

//定义在文件: ../gen_utils.c
extern int get_bool_value(string name,list<Bool_var>* bool_vars,int stamp);
extern int get_bool_sig(string name,list<Bool_var>* bool_sigs,int stamp);

//定义在文件: ./inverse.c
extern long long inverse(long long, const long long);

// 功能：对逻辑非运算进行编码，输出运算的高位代码和低位代码，循环内与循环外采用同样的编码方法
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_bit_inverse(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");//检查参数：左值变量名称字符串的有效性
		check_pt_arg(exp,"expression object's pointer");//检查参数：赋值表达式对象指针的有效性
		check_pt_arg(v,"var_info list's pointer");//检查参数：动态签名表指针的有效性

		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		//判断右操作数是否为变量
		if(4==right)
		{
			//如果是，则进行编码
			int SigID=0;
			int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//int new_sig=get_sig();//为左值分配新签名
			int const_sig=getConstantSig(-1);//为常数-1操作数分配新签名
			string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字
			int right_sig=get_current_sig(v,get_global_string_sig(r_name,var_scope),var_scope);//取出右操作数变量的签名
			int constant1=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
								                 %(long long)P1+(long long)P1)%(long long)P1;
			int constant2=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
								                 %(long long)P2+(long long)P2)%(long long)P2;

			add_new_sig(v,var_name,new_sig,var_scope);//将左值变量新签名加入动态签名表

			out_indent(indent_num, gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string r_name_head=TmpVarJudge(r_name)?r_name:get_global_string(r_name,var_scope);
			gen_out << var_head << ".Data = " << "F_VCL_BitInverse("<<r_name_head
				<<".Data, " << int2HexStr(constant1)
					<< ", " << int2HexStr(constant2) << ");" << endl;
			//输出签名Id调整语句
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//如果是调试模式，则输出调试信息
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<"-1"
							<<"'s sig :"<<const_sig<<" "<<r_name<<"'s sig :"<<const_sig<<endl<<endl;
			}
		}

		else if(3==right)
		{
			int SigID=0;
			int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//int new_sig=get_sig();//为左值分配新签名
			string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字
			int right_sig=getConstantSig(str2int(r_name));////取出右操作数变量的签名
			int const_sig=getConstantSig(-1);//为常数-1操作数分配新签名

			int constant1=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
	                 %(long long)P1+(long long)P1)%(long long)P1;
			int constant2=(((long long)new_sig-(long long)const_sig+(long long)right_sig)
	                 %(long long)P2+(long long)P2)%(long long)P2;
			add_new_sig(v,var_name,new_sig,var_scope);//将左值变量新签名加入动态签名表
			//对立即数初始化
			constant2Str(str2int(r_name));
			out_indent(indent_num, gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitInverse(AC_BIND_CONST_NUM_"
					<< constantBindedStr(str2int(r_name))<<".Data, " << int2HexStr(constant1)
					<< "," << int2HexStr(constant2) << ");" << endl;
			//输出签名Id调整语句
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig :"<<new_sig<<" "<<"-1"
							<<"'s sig :"<<const_sig<<" "<<r_name<<"'s sig :"<<const_sig<<endl<<endl;
			}
		}
		else
		{
			//操作数为其他情况时，抛出错误信息
			string error_info="operand is invalid!";
			throw error_info;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding bitinverse operation : "+str;
		throw error_info;
	}
}


