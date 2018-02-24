
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

//定义在文件 : ./inverse.c
extern long long inverse(long long b,long long prime);
/*
 功能：差异化编码的除法
 输入参数：l_1 左操作数的第一组编码值
	   l_2左操作数的第二组编码值
	   r_1 右操作数的第一组编码值
 	   r_2 右操作数的第二组编码值
	   var_name 运算结果值
 输出参数：
 返回值：
 作者：周航
*/
static void new_div(string l_1,string l_2,string r_1,string r_2,string var_name)
{	int R;
	S_this=*++iter;
	R=S_this-S_last;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_1="<<"("<<l_1<<")/("<<r_1<<")*A1;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<"("<<l_2<<")/("<<r_2<<")*A2+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_var_decode=check_decode("<<var_name<<"_1,"<<var_name<<"_2-("<<R<<"))+("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_l=check_decode("<<l_1<<","<<l_2<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_r=check_decode("<<r_1<<","<<r_2<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"div_temp_var="<<"div_temp_l/div_temp_r;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"RS=RS+div_temp_var"<<"-"<<"div_temp_var_decode;"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<endl;
}
// 功能：对除法运算进行编码，输出除法运算的高位代码和低位代码，注意，只能对除数为常数的除
//			法进行编码，同时区分循环内外，采用不同的编码方法。
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_div(string var_name,Expression* exp,list<Var_info>* v)
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

		new_div(l_1,l_2,r_1,r_2,var_name);
		/*int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		//判断左操作数是否为变量，右操作数是否为常数
		if((4==left)&&(4==right))
				{
					//如果是，则进行编码
					string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数的名字
					string r_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名字
					//对左右操作数都为变量的乘法运算进行编码
					gen_out<<var_name<<"_1="<<"div_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< l_name<<"_1,"<<r_name<<"_1,"<< l_name<<"_2,"<<r_name<<"_2,"<<"A2);"<<endl;
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
					gen_out<<var_name<<"_1="<<"div_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< l_name<<"_1,"<< right_const<<"*A1,"<< l_name<<"_2,"<<right_const<<"*A2,"<<"A2);"<<endl;
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
					gen_out<<var_name<<"_1="<<"div_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< str2int(left_const)<<"*A1,"<< r_name<<"_1,"<<str2int(left_const)<<"*A2,"<< r_name<<"_2,"<<"A2);"<<endl;
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
					gen_out<<var_name<<"_1="<<"div_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A1);"<<endl;
					gen_out<<var_name<<"_2="<<"div_enc("<< str2int(left_name)<<"*A1,"<< str2int(right_name)<<"*A1,"<< str2int(left_name)<<"*A2,"<< str2int(right_name)<<"*A2,"<<"A2);"<<endl;
					//red_mul_IIII(var_name,str2int(left_name),str2int(right_name),v);
				}
	if((4==left)&&(3==right))
		{
			//如果是，则进行编码
			string l_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数变量的名字
			string right_name=exp->get_cer()->get_c();//取出右操作数常数名
			int left_sig=get_current_sig(v,get_global_string_sig(l_name,var_scope),var_scope);//取出左操作数的签名
			int right_value=str2int(right_name);//将右操作数字符串转换为整数

			int const_sig=getConstantSig(right_value);//为常数操作数分配新签名

			//int const_sig=get_sig();//为常数分配签名，但并不使用，以备扩展
			long long h_temp_P1=inverse((long long)right_value,(long long)P1);//计算除数在A中的逆
			long long h_temp_P2=inverse((long long)right_value,(long long)P2);//计算除数在A中的逆
			int new_sig=0;//存储左值变量的新签名
			int constant1=0;//存储低位代码中的补偿常数
			int constant2=0;//存储低位代码中的补偿常数
			string str_const="";//存储低位代码中补偿常数字符串
			//位于循环体和非循环体，则为左值变量分配新签名，并计算补偿常数
			int SigID=0;
			new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			//new_sig=get_sig();
			constant1=(((long long)new_sig-((long long)left_sig*h_temp_P1)%P1)%P1+P1)%P1;
			constant2=(((long long)new_sig-((long long)left_sig*h_temp_P2)%P2)%P2+P2)%P2;
			str_const=" + "+int2str(constant1);//设置补偿常数字符串的内容
			
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量新签名加入动态签名表
			//输出除法运算的高位代码
			
			/*string str_divided="H_"+l_name;//被除数名字的高位字符串
			//判断被除数变量名是否同左值变量名相同
			if(var_name==l_name)
			{
				//如果相同，先输出一条临时变量替换被除数的代码
				out_indent(indent_num,gen_out);
				gen_out<<red_Tmp1_INT32<<" = H_"<<l_name<<";"<<endl;
				//将被除数的高位名字修改为临时变量名
				str_divided=red_Tmp1_INT32;
			}
			else
			{
				//如果不相同，忽略
			}
			//对立即数初始化
//			constant2Str(right_value);
			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(l_name)?l_name:get_global_string(l_name,var_scope);
			gen_out<<var_head<<".Data = "<<"F_VCL_Div("<<l_name_head<<".Data, "<<int2HexStr(right_value)
								<<", "<<int2HexStr(h_temp_P1)<<", "<<int2HexStr(h_temp_P2)<<", "<<int2HexStr(constant1)<<", "<<int2HexStr(constant2)<<");"<<endl;
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
		}
		//判断左右操作数是否均为常数
		else if((3==left)&&(3==right))
		{
			//如果被除数和除数均为常数，则直接将商算出来
			string left_name = exp->get_cel()->get_c();			//取出被除数的名字
			string right_name = exp->get_cer()->get_c();			//取出除数的名字
			//int high=str2int(left_name)/str2int(right_name);//计算商
			//red_assign_const(var_name,high,v);//调用常数赋值编码函数进行编码
			int left_sig = getConstantSig(str2int(left_name));	//为常数操作数分配新签名
			int right_value = str2int(right_name);			//将右操作数字符串转换为整数

			int const_sig=getConstantSig(right_value);//为常数操作数分配新签名

			long long h_temp_P1 = inverse((long long) right_value, (long long) P1);	//计算除数在P1中的逆
			long long h_temp_P2 = inverse((long long) right_value, (long long) P2);	//计算除数在P2中的逆
			int new_sig = 0;			//存储左值变量的新签名
			int constant1 = 0;			//存储低位代码中的补偿常数
			int constant2 = 0;			//存储低位代码中的补偿常数
			//位于循环体和非循环体，则为左值变量分配新签名，并计算补偿常数
			//new_sig = get_sig();
			int SigID=0;
			new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			constant1 = (((long long) new_sig
					- ((long long) left_sig * h_temp_P1) % P1) % P1 + P1) % P1;
			constant2 = (((long long) new_sig
					- ((long long) left_sig * h_temp_P2) % P2) % P2 + P2) % P2;

			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);	//将左值变量新签名加入动态签名表
			//对立即数初始化
			constant2Str(str2int(left_name));
			//对立即数初始化
//			constant2Str(right_value);
			out_indent(indent_num, gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			gen_out << var_head  << ".Data = " << "F_VCL_Div(AC_BIND_CONST_NUM_"
					<< constantBindedStr(str2int(left_name)) << ".Data, " << int2HexStr(right_value) << ","
					<< int2HexStr(h_temp_P1) << ", " << int2HexStr(h_temp_P2) << ", " << int2HexStr(constant1)
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
						<< left_name << "'s sig :" << left_sig  << endl << endl;
			}
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
		string error_info="coding div operation : "+str;
		throw error_info;
	}
}

