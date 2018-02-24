
#include "gen_utils.h"//此模块中用到的部分全局数据引用在此头文件中
#include "expression.h"
#include "var.h"
#include "bool_var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// 定义在文件 : src/main_utils.c
extern bool debug_flag;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;

// 定义在文件 : ../gen_utils.c
extern int get_bool_value(string name,list<Bool_var>* bool_vars,int stamp);
extern int get_bool_sig(string name,list<Bool_var>* bool_sigs,int stamp);

//定义在文件 : ./inverse.c
extern long long inverse(long long ,const long long);

// 功能：对逻辑或运算进行编码，输出运算的高位代码和低位代码，循环内与循环外采用同样的编码方法
// 输入参数：var_name-左值变量名
//			      exp-赋值表达式对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void red_or(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");//检查参数：左值变量名称字符串的有效性
		check_pt_arg(exp,"expression object's pointer");//检查参数：赋值表达式对象指针的有效性
		check_pt_arg(v,"var_info list's pointer");//检查参数：动态签名表指针的有效性
		
		int left=exp->get_cel()->get_stamp();//取出左操作数表达式的标签
		int right=exp->get_cer()->get_stamp();//取出右操作数表达式的标签
		//判断左右操作数是否均为变量
		if(4==left && 4==right)
		{
			//如果是，则进行编码
			//int new_sig=get_sig();//为左值分配新签名
			int SigID=0;
			int new_sig=get_sig_index(&SigID);//存储左值变量的新签名
			
			//取出左值变量的真假值，真假值的签名，并计算真假值的低位
//			int var_true_h=get_bool_value(var_name,bool_vars,1);//取出左值变量的真值
//			int var_true_sig=get_bool_sig(var_name,bool_sigs,1);//取出左值变量真值的签名
			int var_true_h=TRUE_VALUE;
			int var_true_sig=Bool_True_Sig;
//			long long var_true_l_1=((-((long long)var_true_h*K_NUM_P1)%(long long)P1//计算左值变量真值的低位
//												  +(long long)var_true_sig)%(long long)P1+(long long)P1)%(long long)P1;
//			long long var_true_l_2=((-((long long)var_true_h*K_NUM_P2)%(long long)P2//计算左值变量真值的低位
//												+(long long)var_true_sig)%(long long)P2+(long long)P2)%(long long)P2;
//			int var_false_h=get_bool_value(var_name,bool_vars,0);//取出左值变量的假值
//			int var_false_sig=get_bool_sig(var_name,bool_sigs,0);//取出左值变量假值的签名
//			long long var_false_l=((-((long long)var_false_h*K_NUM)%(long long)A//计算左值变量假值的低位
//													  +(long long)var_false_sig)%(long long)A+(long long)A)%(long long)A;
													  
			//取出左操作数的真假值，以及真假值的签名，并以此计算其真假值的低位
			string expl_name=exp->get_cel()->get_cvar()->get_name();//取出左操作数的名称
//			int expl_true_h=get_bool_value(expl_name,bool_vars,1);//取出左操作数的真值
//			int expl_true_sig=get_bool_sig(expl_name,bool_sigs,1);//取出左操作数真值的签名
//			long long expl_true_l=((-((long long)expl_true_h*K_NUM)%(long long)A//计算左操作数真值的低位
//												  +(long long)expl_true_sig)%(long long)A+(long long)A)%(long long)A;
//			int expl_false_h=get_bool_value(expl_name,bool_vars,0);//取出左操作数的假值
//			int expl_false_sig=get_bool_sig(expl_name,bool_sigs,0);//取出左操作数假值的签名
//			long long expl_false_l=((-((long long)expl_false_h*K_NUM)%(long long)A//计算左操作数假值的低位
//													 +(long long)expl_false_sig)%(long long)A+(long long)A)%(long long)A;
			
			//取出右操作数的真假值，以及真假值的签名，并以此计算其真假值的低位
			string expr_name=exp->get_cer()->get_cvar()->get_name();//取出右操作数的名称
//			int expr_true_h=get_bool_value(expr_name,bool_vars,1);//取出右操作数的真值
//			int expr_true_sig=get_bool_sig(expr_name,bool_sigs,1);//取出右操作数真值的签名
//			long long expr_true_l=((-((long long)expr_true_h*K_NUM)%(long long)A//计算右操作数真值的低位
//													 +(long long)expr_true_sig)%(long long)A+(long long)A)%(long long)A;
//			int expr_false_h=get_bool_value(expr_name,bool_vars,0);//取出右操作数的假值
//			int expr_false_sig=get_bool_sig(expr_name,bool_sigs,0);//取出右操作数假值的签名
//			long long expr_false_l=((-((long long)expr_false_h*K_NUM)%(long long)A//计算右操作数假值的低位
//													  +(long long)expr_false_sig)%(long long)A+(long long)A)%(long long)A;
			
			//取出左右操作数变量的签名
			int right_sig=get_current_sig(v,get_global_string_sig(expr_name,var_scope),var_scope);//取出右操作数变量的签名
			int left_sig=get_current_sig(v,get_global_string_sig(expl_name,var_scope),var_scope);//取出左操作数变量的签名
//
			//以下变量均为计算高位编码中的逆时用到的临时变量
//			long long temp1=((long long)expl_false_h-(long long)expl_true_h)*((long long)expr_false_h-(long long)expr_true_h);
//			long long temp2=(long long)var_false_h-(long long)var_true_h;
			//计算高位编码中的逆值
//			int M_1=(inverse(temp1,(long long)A)%A)*temp2%A;
//			string str_M1=(M_1>=0)?int2str(M_1):("("+int2str(M_1)+")");//将M1转换为字符串
			/*
			//以下变量均为计算低位中的逆时用到的临时变量
			long long templ=expl_false_l - expl_true_l + expl_true_sig - expl_false_sig;
			long long tempr=expr_false_l - expr_true_l + expr_true_sig - expr_false_sig;
			long long tempv=var_false_l - var_true_l + var_true_sig - var_false_sig;
			//计算低位编码时用到的逆值
			int M_5=((templ % A)*(tempr % A)%A+A)%A;
			M_5=(inverse(M_5,(long long)A)*(tempv % A) % A )%A;
			string str_M5=(M_5>=0)?int2str(M_5):("("+int2str(M_5)+")");//将M5转换为字符串*/
			
			//计算低位编码中用到的补偿常数
			long long constant1=(((long long)new_sig-(long long)var_true_sig)
			                                  %(long long)P1+(long long)P1)%(long long)P1;
			//计算低位编码中用到的补偿常数
			long long constant2=(((long long)new_sig-(long long)var_true_sig)
						                      %(long long)P2+(long long)P2)%(long long)P2;
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//将左值变量的新签名加入动态签名表
			//将左右操作数的真值和左值变量的真值，分别转换为字符串
//			string str_expl_true_h=(expl_true_h<0)?"("+int2str(expl_true_h)+")":int2str(expl_true_h);
//			string str_expr_true_h=(expr_true_h<0)?"("+int2str(expr_true_h)+")":int2str(expr_true_h);
//			string str_var_true_h=(var_true_h<0)?"("+int2str(var_true_h)+")":int2str(var_true_h);
			

			
		/*	//输出逻辑或运算的高位代码
			out_indent(indent_num,gen_out);
			gen_out<<"H_"<<var_name<<" = ((("<<str_int64<<")H_"<<expl_name<<" - "
			<<str_expl_true_h<<") % "<<str_A<<"* (("<<str_int64<<")H_"<<expr_name<<" - "<<str_expr_true_h
			<<") % "<<str_A<<" + "<<str_A<<") * "<<str_M1<<" % "<<str_A<<" + "
			<<str_var_true_h<<";"<<endl;
			
			//输出附加全局变量tmpLogicCalVar1的计算代码
			out_indent(indent_num,gen_out);
			gen_out<<tmpLogicCalVar1<<" = "<<expl_true_sig-left_sig<<";"<<endl;
			
			//输出附加全局变量tmpLogicCalVar2的计算代码
			out_indent(indent_num,gen_out);
			gen_out<<tmpLogicCalVar2<<" = "<<expr_true_sig-right_sig<<";"<<endl;
				
			//输出逻辑与运算的低位代码
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ((("<<str_int64<<")L_"<<expl_name<<" - DT - "
			<<expl_true_l<<" + "<<tmpLogicCalVar1<<") % "<<str_A<<" * ((("<<str_int64<<")L_"
			<<expr_name<<" - DT - "<<expr_true_l<<" + "<<tmpLogicCalVar2<<") % "<<str_A
			<<") % "<<str_A<<" + "<<str_A<<") * "<<str_M5<<" % "<<str_A<<" + "<<constant<<" + DT;"<<endl;
			
			//输出逻辑与运算的低位模A代码
			out_indent(indent_num,gen_out);
			red_INT64MOD("L_"+var_name,red_Tmp_INT64);*/

			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(expl_name)?expl_name:get_global_string(expl_name,var_scope);
			string r_name_head=TmpVarJudge(expr_name)?expr_name:get_global_string(expr_name,var_scope);
						gen_out << var_head  << ".Data = " << "F_VCL_LogicOr("<<l_name_head
								 << ".Data, "<<r_name_head  << ".Data, " << int2HexStr(left_sig)
								<< ", " << int2HexStr(right_sig) << ", " << int2HexStr(constant1) << ", "
								<< int2HexStr(constant2) << ");" << endl;
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
				gen_out<<"//"<<var_name<<"'s new sig : "<<new_sig<<", true value :"<<var_true_h
				<<", true sig : "<<var_true_sig<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<expl_name<<"'s sig : "<<left_sig<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<expr_name<<"'s  sig : "<<right_sig<<endl;
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
		string error_info="coding logical or operation : "+str;
		throw error_info;
	}
}


