
#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include <cmath>
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// 定义在文件: src/main_utils.cpp
extern bool debug_flag;

//定义在文件 : ./gen_assign.c
extern void red_assign_const(string,int,list<Var_info>*);
//定义在文件 : ./inverse.c
extern long long inverse(long long b,const long long prime);

// 功能：对左移运算进行编码，输出左移运算的高位代码和低位代码，注意，只能对位移为常数的
//			移位运算进行编码，同时区分循环内外，采用不同的编码方法。
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_shift_l(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		//判断左操作数是否为变量，右操作数是否为常数
		if((4==left)&&(3==right))
		{
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数变量的名字
			string r_name=exp->get_cer()->get_c();//取出右操作数常数名
			int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//取出左操作数的签名
			int high=str2int(r_name);//将右操作数字符串转换为整数

			int const_sig=getConstantSig(high);//为常数操作数分配新签名


			//int const_sig=get_sig();//为常数分配签名
			int m=(int)pow(2.0,(double)high);//将左移量转换成整数	
			int new_sig=0;//存储左值变量的新签名
			int constant1=0;//存储低位代码中的补偿常数
			int constant2=0;//存储低位代码中的补偿常数
			string str_const="";//存储低位代码中补偿常数字符串
			int SigID=0;
			new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*(long long)m)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*(long long)m)%P2)%P2+P2)%P2;
			//str_const=" + "+int2str(constant);//设置补偿常数字符串的内容
			
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表

		/*	//输出左移运算的高位代码
			
			string str_divided="H_"+l_name;//被移变量名字的高位字符串
			//判断被移变量名是否同左值变量名相同
			if(var_name==l_name)
			{
				//如果相同，先输出一条临时变量替换被移变量的代码
				out_indent(indent_num,gen_out);
				gen_out<<red_Tmp1_INT32<<" = H_"<<l_name<<";"<<endl;
				//将被移变量的高位名字修改为临时变量名
				str_divided=red_Tmp1_INT32;
			}
			else
			{
				//如果不相同，忽略
			}*/
			//对立即数初始化
//			constant2Str(high);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitShiftL("<<l_name_head
								 << ".Data, " << int2HexStr(high) << ", "
								<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
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
				<<"'s sig :"<<left_sig<<endl<<endl;
			}
			//gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" << "<<r_name<<";"<<endl;
			//输出左移运算的低位代码
		/*	out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")(("<<str_int64<<")("
			<<str_divided<<" >> "<<32-high<<") + (("<<str_divided<<" >> "<<31-high<<
			") & 1)) * "<<((long long)RK*RK)%A<<";"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD(red_Tmp1_INT32, red_Tmp_INT64);
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")((("<<str_int64<<")L_"
			<<l_name<<" - DT) << "<<r_name<<") + "<<red_Tmp1_INT32<<str_const
			<<" + DT;"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD("L_"+var_name, red_Tmp_INT64);*/
		}
		//判断左右操作数是否均为常数	
		else if((3==left)&&(3==right))
		{
			//如果被除数和除数均为常数，则直接将结果算出来
			string left_name=exp->get_cel()->get_c();//取出左操作数的名字
			string right_name=exp->get_cer()->get_c();//取出右操作数的名字
			//int high=str2int(left_name)<<str2int(right_name);//算出移位运算后的结果
			//red_assign_const(var_name,high,v);//调用常数赋值编码函数进行编码
			int left_sig=getConstantSig(str2int(left_name));//取出左操作数的签名
			int high=str2int(right_name);//将右操作数字符串转换为整数

			int const_sig=getConstantSig(high);//为常数操作数分配新签名


			//int const_sig=get_sig();//为常数分配签名
			int m=(int)pow(2.0,(double)high);//将左移量转换成整数
			int new_sig=0;//存储左值变量的新签名
			int constant1=0;//存储低位代码中的补偿常数
			int constant2=0;//存储低位代码中的补偿常数
			string str_const="";//存储低位代码中补偿常数字符串
			int SigID=0;
			new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*(long long)m)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*(long long)m)%P2)%P2+P2)%P2;

			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
			//对立即数初始化
			constant2Str(str2int(left_name));
			//对立即数初始化
//			constant2Str(high);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitShiftL(AC_BIND_CONST_NUM_"
								<< constantBindedStr(str2int(left_name)) << ".Data, " << int2HexStr(high) << ", "
								<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
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
				<<"'s sig :"<<left_sig<<endl<<endl;
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
		string error_info="coding shift left operation : "+str;
		throw error_info;
	}
}

// 功能：对右移运算进行编码，输出右移运算的高位代码和低位代码，注意，只能对位移为常数的
//			移位运算进行编码，同时区分循环内外，采用不同的编码方法。
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_shift_r(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"expression object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		
		//判断左操作数是否为变量，右操作数是否为常数
		if((4==left)&&(3==right))
		{
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数变量的名字
			string r_name=exp->get_cer()->get_c();//取出右操作数常数名
			int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//取出左操作数的签名
			int high=str2int(r_name);//将右操作数字符串转换为整数

			int const_sig=getConstantSig(high);//为常数操作数分配新签

			//int const_sig=get_sig();//为常数操作数分配签名
			int m=(int)pow(2.0,(double)high);
			long long tmp1=inverse(m,(long long)P1);//有限域中的逆
			long long tmp2=inverse(m,(long long)P2);//有限域中的逆
			
			int new_sig=0;//存储左值变量的新签名
			int constant1=0;//存储低位代码中的补偿常数
			int constant2=0;//存储低位代码中的补偿常数
			string str_const="";//存储低位代码中补偿常数字符串
			int SigID=0;
			new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*tmp1)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*tmp2)%P2)%P2+P2)%P2;
			//str_const=" + "+int2str(constant);//设置补偿常数字符串的内容
			
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表

			//输出右移运算的高位代码

			/*string str_divided="H_"+l_name;//被移变量名字的高位字符串
			//判断被移变量名是否同左值变量名相同
			if(var_name==l_name)
			{
				//如果相同，先输出一条临时变量替换被移变量的代码
				out_indent(indent_num,gen_out);
				gen_out<<red_Tmp1_INT32<<" = H_"<<l_name<<";"<<endl;
				//将被移变量的高位名字修改为临时变量名
				str_divided=red_Tmp1_INT32;
			}
			else
			{
				//如果不相同，忽略
			}*/
			//对立即数初始化
//			constant2Str(high);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			out_indent(indent_num,gen_out);
			gen_out << var_head << ".Data = " << "F_VCL_BitShiftR("<<l_name_head
					 << ".Data, " << int2HexStr(high) << ", "
					<< int2HexStr(tmp1) << ", "<< int2HexStr(tmp2) << ", "
					<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
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
				<<"'s sig :"<<left_sig<<endl<<endl;
			}

			/*gen_out<<"H_"<<var_name<<" = H_"<<l_name<<" >> "<<r_name<<";"<<endl;

			//输出右移运算的低位代码
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<RK<<" * ("<<str_divided
			<<" & "<<m-1<<");"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD(red_Tmp1_INT32, red_Tmp_INT64);
			out_indent(indent_num,gen_out);
			red_Add_VVVV(red_Tmp1_INT32, "L_"+l_name, red_Tmp1_INT32, "DT");
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ("<<str_int64<<")"<<red_Tmp1_INT32<<
			" * "<<tmp<<str_const<<" + DT;"<<endl;
			out_indent(indent_num,gen_out);
			red_INT64MOD("L_"+var_name, red_Tmp_INT64);*/
		}
		//判断左右操作数是否均为常数
		else if((3==left)&&(3==right))//if the operands are both constants
		{
			//如果被除数和除数均为常数，则直接将结果算出来
			string left_name=exp->get_cel()->get_c();//取出左操作数的名字
			string right_name=exp->get_cer()->get_c();//取出右操作数的名字
			int left_sig=getConstantSig(str2int(left_name));//取出左操作数的签名
			int high=str2int(right_name);//将右操作数字符串转换为整数

			int const_sig=getConstantSig(high);//为常数操作数分配新签

			//int const_sig=get_sig();//为常数操作数分配签名
			int m=(int)pow(2.0,(double)high);
			long long tmp1=inverse(m,(long long)P1);//有限域中的逆
			long long tmp2=inverse(m,(long long)P2);//有限域中的逆

			int new_sig=0;//存储左值变量的新签名
			int constant1=0;//存储低位代码中的补偿常数
			int constant2=0;//存储低位代码中的补偿常数
			string str_const="";//存储低位代码中补偿常数字符串
			//如果不位于循环体，则为左值变量分配新签名，并计算补偿常数
			int SigID=0;
			new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*tmp1)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*tmp2)%P2)%P2+P2)%P2;
			//str_const=" + "+int2str(constant);//设置补偿常数字符串的内容

			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
			//对立即数初始化
			constant2Str(str2int(left_name));
			//对立即数初始化
//			constant2Str(high);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_BitShiftR(AC_BIND_CONST_NUM_"
					<< constantBindedStr(str2int(left_name)) << ".Data, " << int2HexStr(high) << ", "
					<< int2HexStr(tmp1) << ", "<< int2HexStr(tmp2) << ", "
					<< int2HexStr(constant1)<<", " << int2HexStr(constant2) << ");" << endl;
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
				<<"'s sig :"<<left_sig<<endl<<endl;
			}

			/*int high=str2int(left_name)>>str2int(right_name);//算出移位运算后的结果
			red_assign_const(var_name,high,v);//调用常数赋值编码函数进行编码*/

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
		string error_info="coding shift right operation : "+str;
		throw error_info;
	}
}

