#include "gen_utils.h"
#include "expression.h"
#include "var.h"
#include "gen_global_var.h"

// 定义在文件: src/main_utils.cpp
extern bool debug_flag;

extern string get_bind_local_var(string global_var,string func_name);
//定义在gen_compod_stmt.cpp
extern string function_name;

extern string int2HexStr_data(const int num);
extern void constant2Str(const int constant);

// 功能：函数调用时，输出对函数附加变量的编码。包括附加变量的高位和低位。
// 输入参数：fun_name-函数名
//			      v-动态签名表指针
// 输入参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void out_red_dF(string fun_name,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(fun_name,"function's name");
		check_pt_arg(v,"var_info list's pointer");
		int index = 0;
		int new_sig=get_sig_index(&index);//从签名表中取一个签名作为函数附加变量的新签名
		add_new_sig(v,"red_dF_"+fun_name,new_sig,"global");//将新签名插入动态签名表
			
		//计算函数附加变量的低位值
		long long new_low1 = -((long long) new_sig * (long long) K_NUM_P1)
				% (long long) P1 + new_sig;
		new_low1 = (new_low1 % P1 - P1) % P1;

		long long new_low2 = -((long long) new_sig * (long long) K_NUM_P2)
						% (long long) P2 + new_sig;
		new_low2 = (new_low2 % P2 - P2) % P2;

		//如果是调试模式，则输出调试信息：函数附加变量的签名
		if (true == debug_flag)
		{
			out_indent(indent_num, gen_out);
			gen_out << "//red_dF_" << fun_name << "'s sig : " << new_sig
					<< endl;
		}
		
		//输出函数附加变量的高位代码
		out_indent(indent_num, gen_out);
		//gen_out << "H_red_dF_" << fun_name << " = " << new_sig << ";" << endl;
		gen_out << "g_ExFun_" << fun_name << " = " << new_sig << ";" <<endl;

		//输出函数附加变量的低位代码
		//out_indent(indent_num, gen_out);
		//gen_out << "L_red_dF_" << fun_name << " = " << new_low << " + DT;"
		//		<< endl;
		//函数附加变量改为INT32后下面两句输出语句不需要增加
//		gen_out << "AC_df_" << fun_name << ".C.C1 = " << new_low1 << " + g_GS_TL1;" <<endl;
//		out_indent(indent_num, gen_out);
//		gen_out << "AC_df_" << fun_name << ".C.C2 = " << new_low2 << " + g_GS_TL2;" <<endl;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="out put red_df adjust code : "+str;
		throw error_info;
	}
}

// 功能：对函数调用时的实参进行编码。包括对函数参数附加变量的编码，签名的分配等。
// 输入参数：s-函数调用表达式对象指针
//			      v-动态签名表指针
// 输入参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void out_fun_args(Expression* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		string fun_name=s->get_c();//取出该函数名称
		int i=0;//对已输出的参数个数进行计数
		string str1[100];
		string str2[100];
		//参数附加变量的名字
		//遍历该函数实参列表
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			//判断实参是否为常数
			if(3==iter->get_stamp())
			{
				//如果为常数，则取出该常数字符串
				string arg_h=iter->get_c();
				str1[i]=arg_h+"*8193";
				str2[i]=arg_h+"*8191";
		}
			//判断实参是否为一般变量
			else if (4 == iter->get_stamp()
					&& iter->get_cvar()->get_stamp() == 0)
			{
				//如果为一般变量，则取出该变量的名称
				string arg_name=iter->get_cvar()->get_name();
				str1[i]=arg_name+"_1";
				str2[i]=arg_name+"_2";



			}
			else
			{
				string error_info="function call's argument is invalid! ";
				throw error_info;
			}
		}

		gen_out  << fun_name << "(";
		//输出函数的实参代码

		for(int j=0;j<i;j++)
		{
			if(j==i-1)
			{
				gen_out << "" << str1[j]<<",";
				gen_out << "" << str2[j];
			}
			else
			{
				gen_out << "" << str1[j] << ",";
				gen_out << "" << str2[j] << ",";
			}
		}
		gen_out<<");"<<endl;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="out put red_df adjust code : "+str;
		throw error_info;
	}
}

// 功能：对校验接口调用时的实参（普通变量）进行编码。不包括对函数参数附加变量的编码，签名的分配等。将待校验的普通变量封装为T_code形式。
// 输入参数：s-函数调用表达式对象指针
//			      v-动态签名表指针
// 输入参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
static void out_fun_args_checkVar(Expression* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=s->get_c();//取出该函数名称
		int i=0;//对已输出的参数个数进行计数
		int j=0;//记录当前是第几个参数
		string fun_args = "";
		//得到参数个数
		for(list<Expression>::iterator iter=s->get_args()->begin();
				       iter!=s->get_args()->end();iter++)
		{
			j++;
		}
		if(j != 2)
		{
			string error_info =
					"function F_VCE_CheckVar must posess 2 arguments! ";
			throw error_info;
		}
		//遍历该函数实参列表
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			//判断实参是否为常数
			if(3==iter->get_stamp() && i == 1)
			{
				string arg_h=iter->get_c();
				fun_args.append(arg_h);
			}
			//判断实参是否为一般变量
			else if (4 == iter->get_stamp()
					&& iter->get_cvar()->get_stamp() == 0 && i == 0)
			{
				//如果为一般变量，则取出该变量的名称
				string arg_name=iter->get_cvar()->get_name();
				//int sig = get_current_sig(v,arg_name,var_scope);
				if(is_global_var(arg_name))
				{
					if(arg_name == "AC_Void_Func_RetVal")
					{
						fun_args.append("AC_Void_Func_RetVal");
					}
					else
						fun_args.append(get_bind_local_var(arg_name, function_name));
				}
				else
				{
					if(arg_name == "AC_Void_Func_RetVal")
					{
						fun_args.append("AC_Void_Func_RetVal");
					}
					else
						fun_args.append("TC_" + arg_name);
				}

				if (i < j - 1)
				{
					fun_args.append(", ");
				}
			}
			else
			{
				cout<<iter->get_stamp()<<endl;
				cout<<iter->get_cvar()->get_stamp()<<endl;
				string error_info = "function call's argument is invalid! ";
				throw error_info;
			}
		}
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_CheckCodeVar" << "(";
		gen_out << fun_args;
		gen_out << ");" << endl;
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "out put red_df adjust code : " + str;
		throw error_info;
	}
}

// 功能：对校验接口调用时的实参（数组）进行编码。不包括对函数参数附加变量的编码，签名的分配等。将待校验的普通变量封装为T_code形式。
// 输入参数：s-函数调用表达式对象指针
//			      v-动态签名表指针
// 输入参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
static void out_fun_args_checkArr(Expression* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=s->get_c();//取出该函数名称
		int i=0;//对已输出的参数个数进行计数
		int j=0;//记录当前是第几个参数
		string fun_args = "";
		string str1="AC_ParaVar_"+fun_name+"_";//参数附加变量的名字
		//得到参数个数
		for(list<Expression>::iterator iter=s->get_args()->begin();
						       iter!=s->get_args()->end();iter++)
		{
			j++;
		}
		if(j != 4)
		{
			string error_info =
					"function F_VCE_CheckArr must posess 2 arguments! ";
			throw error_info;
		}
		//遍历该函数实参列表
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			if(i == 0)
			{
				//第一个数组元素参数
				string arg_name=iter->get_cvar()->get_name();
				//string i_name = iter->get_cvar()->get_cexp()->get_c();//取出下标字符串名
				fun_args += "TC_" ;
				fun_args += arg_name;
				fun_args +=  ", ";
			}
			if(3==iter->get_stamp() && i == 1)
			{
				//第二个errno参数为立即数形式
				string arg_h=iter->get_c();
//				const int arg_h_int = str2int(arg_h);
//				fun_args.append("CONST_NUM_" + int2HexStr_data(arg_h_int) + ", ");
				fun_args.append(arg_h + ", ");
			}
			if(3==iter->get_stamp() && i == 2)
			{
				//第三个errno参数为立即数形式
				string arg_h=iter->get_c();
//				const int arg_h_int = str2int(arg_h);
//				fun_args.append("CONST_NUM_" + int2HexStr_data(arg_h_int) + ", ");
				fun_args.append(arg_h + ", ");
			}
			if(3==iter->get_stamp() && i == 3)
			{
				//第四个errno参数为立即数形式
				string arg_h=iter->get_c();
				fun_args.append(arg_h);
			}
			if ((4 == iter->get_stamp() && iter->get_cvar()->get_stamp() == 0)
					&& i == 1)
			{
				//第二个errno参数为变量形式
				string arg_name=iter->get_cvar()->get_name();
				if(is_global_var(arg_name))
				{
					fun_args.append(get_bind_local_var(arg_name, function_name));
					fun_args.append(", ");
				}
				else
				{
					fun_args.append("TC_" + arg_name + ".Data.F, ");
				}

			}
			if ((4 == iter->get_stamp() && iter->get_cvar()->get_stamp() == 0)
								&& i == 2)
			{
				//第二个errno参数为变量形式
				string arg_name=iter->get_cvar()->get_name();
				if(is_global_var(arg_name))
				{
					fun_args.append(get_bind_local_var(arg_name, function_name));
					fun_args.append(", ");
				}
				else
				{
					fun_args.append("TC_" + arg_name + ".Data.F, ");
				}
			}
			if ((4 == iter->get_stamp() && iter->get_cvar()->get_stamp() == 0)
								&& i == 3)
			{
				//第二个errno参数为变量形式
				string arg_name=iter->get_cvar()->get_name();
				if(is_global_var(arg_name))
				{
					fun_args.append(get_bind_local_var(arg_name, function_name));
				}
				else
				{
					fun_args.append("TC_" + arg_name + ".Data.F");
				}
			}

		}
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_CheckCodeArr" << "(";
		gen_out << fun_args;
		gen_out << ");" << endl;
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "out put red_df adjust code : " + str;
		throw error_info;
	}
}

// 功能：对外部输入输出接口调用时的实参（数组）进行编码。不包括对函数参数附加变量的编码，签名的分配等。
// 输入参数：s-函数调用表达式对象指针
//			      v-动态签名表指针
// 输入参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
static void out_fun_args_dataTrans(Expression* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"function calling expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=s->get_c();//取出该函数名称
		int i=0;//对已输出的参数个数进行计数
		string fun_args = "";
		//遍历该函数实参列表
		for(list<Expression>::iterator iter=s->get_args()->begin();
		       iter!=s->get_args()->end();iter++,i++)
		{
			if(3==iter->get_stamp() && i == 0)
			{
				//第二个errno参数为立即数形式
				string arg_h=iter->get_c();
				fun_args.append(arg_h);
			}
			else if (i == 0 && 4 == iter->get_stamp()
					&& iter->get_cvar()->get_stamp() == 0)
			{
				//第一个数组元素参数
				string arg_name = iter->get_cvar()->get_name();
				fun_args.append("TC_" + arg_name +".Data.F");
			}
			if (i > 0)
			{
				string error_info = "F_USR_DataTrans have more than 1 args!";
				throw error_info;
			}
		}
		out_indent(indent_num,gen_out);
		gen_out<<fun_name<<"(";
		gen_out<<fun_args;
		gen_out<<");"<<endl;

		//生成对应的F_ITF_DataConvert函数
		if ("F_USR_DataFetch" == fun_name)
		{
			out_indent(indent_num, gen_out);
			gen_out << "F_ITF_DataConvertFetch" << "(";
			gen_out << fun_args;
			gen_out << ");" << endl;
		}
		else if("F_USR_DataDeliver" == fun_name)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"F_ITF_DataConvertDeliver"<<"(";
			gen_out<<fun_args;
			gen_out<<");"<<endl;
		}
		else if("F_USR_DataSend" == fun_name)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"F_ITF_DataConvertSend"<<"(";
			gen_out<<fun_args;
			gen_out<<");"<<endl;
		}
		else if("F_USR_DataRecv" == fun_name)
		{
			out_indent(indent_num,gen_out);
			gen_out<<"F_ITF_DataConvertRecv"<<"(";
			gen_out<<fun_args;
			gen_out<<");"<<endl;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="out put red_df adjust code : "+str;
		throw error_info;
	}
}
// 功能：对函数调用语句进行编码。根据函数是否有传入实参，生成不同的冗余代码
// 输入参数：s-函数调用表达式对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_fun_call(Expression* s,list<Var_info>* v)
{//调整函数出口和入口签名

	out_indent(indent_num,gen_out);
	gen_out<<"S_Fun_entry="<<*iter<<";"<<endl;
	S_this=*++iter;
	S_last=S_this;
	idx++;
	out_indent(indent_num,gen_out);
	gen_out<<"P_pre_idx="<<idx<<";"<<endl;
	out_indent(indent_num,gen_out);
	gen_out<<"S_Fun_out="<<S_this<<";"<<endl;
	gen_out<<endl;

	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(s,"function call expression's pointer");
		
		string fun_name=s->get_c();//记录被调用的函数名
		//判断是否是校验模块接口
		if("F_VCE_CheckVar" == fun_name)
		{
			//判断该函数有无实参传入
			if(2 == s->get_args()->size())
			{
				//如果有实参传入
				out_fun_args_checkVar(s,var_infos);//输出对实参的编码
			}
			else
			{
				//校验接口没有参数
				string error_info =
						"No args for F_VCE_Check function, wrong usage";
				throw error_info;
			}
		}
		else if("F_VCE_CheckArr" == fun_name)
		{
			//判断该函数有无实参传入
			if(4 == s->get_args()->size())
			{
				//如果有实参传入
				out_fun_args_checkArr(s,var_infos);//输出对实参的编码
			}
			else
			{
				//校验接口没有参数
				string error_info =
						"No args for F_VCE_Check function, wrong usage";
				throw error_info;
			}
		}
		else if ("F_USR_DataFetch" == fun_name
				|| "F_USR_DataDeliver" == fun_name
				|| "F_USR_DataRecv" == fun_name || "F_USR_DataSend" == fun_name)
		{
			//判断该函数有无实参传入
			if(1 == s->get_args()->size())
			{
				//如果有实参传入
				out_fun_args_dataTrans(s,var_infos);//输出对实参的编码
			}
			else
			{
				//校验接口没有参数
				string error_info="No args for F_USR_DataFetch function, wrong usage";
				throw error_info;
			}
		}
		else
		{
			//判断该函数有无实参传入
			if(0!=s->get_args()->size())
			{
				//如果有实参传
				//out_red_dF(fun_name,var_infos);//输出函数附加变量的调整代码
				out_fun_args(s,var_infos);//输出对实参的编码
			}
			else
			{
				//如果无实参传入
				/*out_red_dF(fun_name,var_infos);//输出函数附加变量的调整代码
				//此处添加修正全局变量的代码
				int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
				vector<string> global_vars = get_global_var_infunc(function_name);
				vector<string>::iterator it;
				for(it=global_vars.begin();it!=global_vars.end();it++)
				{
					out_indent(indent_num, gen_out);
					gen_out << "TC_" + *it + ".Data.F =" << get_bind_local_var(*it, function_name) + ".Data.F;" <<endl;
					int constant1 = 0, constant2 = 0;
					//全局变量签名
					out_indent(indent_num, gen_out);

					constant1 = get_current_sig(var_infos, *it, "global");
					gen_out<<"//" +*it+"'s sig: "+int2str(constant1)<<endl;
					out_indent(indent_num, gen_out);

					constant2 = get_current_sig(v, get_bind_local_var(*it, function_name), function_name);
					gen_out<<"//" +get_bind_local_var(*it, function_name)+"'s sig: "+int2str(constant2)<<endl;
					constant3 = ((constant1 - constant2)%P1+P1)%P1;
					constant4 = ((constant1 - constant2)%P2+P2)%P2;
					//全局变量对应的临时变量签名
					//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
					out_indent(indent_num, gen_out);
					gen_out << "TC_" << *it << ".Data.C.C1 = " << "(" << get_bind_local_var(*it, function_name) << ".Data.C.C1 +" << constant3 << "- g_GS_TL1)%P1;"<<endl;
					out_indent(indent_num, gen_out);
					gen_out << "TC_" << *it << ".Data.C.C2 = " << "(" << get_bind_local_var(*it, function_name) << ".Data.C.C2 +" << constant4 << "- g_GS_TL2)%P2;"<<endl;
					out_indent(indent_num, gen_out);
				}*/

				out_indent(indent_num, gen_out);
				gen_out  << fun_name << "();" << endl;


			}
			gen_out << endl;

			int R;
			S_this=*++iter;
			R=S_this-S_last;
			S_last=S_this;
			idx++;
			out_indent(indent_num,gen_out);
			gen_out<<"P_pre_idx="<<idx<<";"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<"RS=RS"<<"+("<<R<<");"<<endl;
			out_indent(indent_num, gen_out);
			gen_out << endl;


			int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
			vector<string> global_vars = get_global_var_infunc(function_name);
			vector<string>::iterator it;
			//调用完成后刷新全局变量绑定的局部变量
			for(it=global_vars.begin();it!=global_vars.end();it++)
			{/*
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(*it, function_name) + ".Data.F =" << "TC_" + *it  + ".Data.F;" <<endl;
				//分配新签名
				int sigIndex = 0;
				int ret_sig=get_sig_index(&sigIndex);//为返回值分配固定签名
				//add_new_vars(var_infos, get_bind_local_var(*it, function_name),ret_sig,"global","retVal", sigIndex);
				add_new_sig(v, get_bind_local_var(*it, function_name),ret_sig, var_scope);
				out_indent(indent_num, gen_out);
				gen_out<<"//" + get_bind_local_var(*it, function_name) + "'s new sig:" + int2str(ret_sig) <<endl;
				constant1 = ret_sig;
				//constant2 = get_current_sig(var_infos, *it, "global");
				constant2 = get_current_sig(var_infos, *it, "global");
				constant3 = ((constant1 - constant2)%P1+P1)%P1;
				constant4 = ((constant1 - constant2)%P2+P2)%P2;
				//全局变量对应的临时变量签名
				//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
				out_indent(indent_num, gen_out);
				gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C1 = " << "(" << "TC_" << *it << ".Data.C.C1 + " << constant3 << ")%P1 + g_GS_TL1;"<<endl;
				out_indent(indent_num, gen_out);
				gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C2 = " << "(" << "TC_" << *it << ".Data.C.C2 + " << constant4 << ")%P2 + g_GS_TL2;"<<endl;
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(*it, function_name) <<".Id = " <<"F_VCL_RegisterCodeId(" <<ret_sig <<");"<<endl;
			*/}
		}
	}
	catch(string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding function call: "+str;
		throw error_info;
	}
}

// 功能：对函数赋值语句进行编码。根据函数是否有传入实参，生成不同的冗余代码
// 输入参数：var_name-左值变量名
//			      exp-函数调用表达式对象指针
//			      v-动态签名表指针
// 输入参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_fun_assign(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{	//调整函数出口和入口签名

		out_indent(indent_num,gen_out);
		gen_out<<"S_Fun_entry="<<*iter<<";"<<endl;
		S_this=*++iter;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);

		gen_out<<"S_Fun_out="<<S_this<<";"<<endl;
		gen_out<<endl;
		//检查输入参数的有效性
		check_str_arg(var_name,"left variable's name");
		check_pt_arg(exp,"function call expression's pointer");
		check_pt_arg(v,"var_info list's pointer");

		string fun_name=exp->get_c();//取出被调用函数名称
//		out_red_dF(fun_name,v);//输出该函数的附加变量调整代码
		out_indent(indent_num, gen_out);

		//判断该函数有无实参传入
		if(0!=exp->get_args()->size())
		{gen_out<<"FUN_RET=";
			//如果有实参传入
			out_fun_args(exp,v);//输出对实参的编码
			out_indent(indent_num, gen_out);

		}
		else
		{
			//如果无实参传入
			//此处添加修正全局变量的代码
			int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
			vector<string> global_vars = get_global_var_infunc(function_name);
			vector<string>::iterator it;
			gen_out<<"FUN_RET=";
			gen_out  << fun_name << "();" << endl;
		}
		out_indent(indent_num, gen_out);
		gen_out  << endl;
		int R;
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS"<<"+("<<R<<");"<<endl;
		out_indent(indent_num, gen_out);
		gen_out << endl;
		//重新为函数返回值编码
		S_this=*++iter;
		R=S_this-S_last;
		S_last=S_this;
		idx++;
		out_indent(indent_num,gen_out);
		gen_out<<"P_pre_idx="<<idx<<";"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_1="<<"FUN_RET*8193;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<"FUN_RET*8191+("<<R<<");"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+"<<var_name<<"_2+"<<var_name<<"_1;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<var_name<<"_2="<<var_name<<"_2-("<<R<<");"<<endl;
		gen_out<<endl;
		//此处添加修正全局变量的代码
/*		int tempConstant1 = 0;
		int tempConstant2 = 0;
		int constant3 = 0;
		int constant4 = 0;*/
		vector<string> global_vars = get_global_var_infunc(function_name);
		vector<string>::iterator it;
		//调用完成后刷新全局变量绑定的局部变量
		/*for(it=global_vars.begin();it!=global_vars.end();it++)
		{
			out_indent(indent_num, gen_out);
			gen_out <<get_bind_local_var(*it, function_name) + ".Data.F =" << "TC_" + *it  + ".Data.F;" <<endl;
			//分配新签名
			int sigIndex = 0;
			int ret_sig=get_sig_index(&sigIndex);//为返回值分配固定签名
			//add_new_vars(var_infos, get_bind_local_var(*it, function_name),ret_sig,"global","retVal", sigIndex);
			add_new_sig(v, get_bind_local_var(*it, function_name),ret_sig, var_scope);
			out_indent(indent_num, gen_out);
			gen_out<<"//" + get_bind_local_var(*it, function_name) + "'s new sig:" + int2str(ret_sig) <<endl;
			tempConstant1 = ret_sig;
			//constant2 = get_current_sig(var_infos, *it, "global");
			tempConstant2 = get_current_sig(var_infos, *it, "global");
			constant3 = ((tempConstant1 - tempConstant2)%P1+P1)%P1;
			constant4 = ((tempConstant1 - tempConstant2)%P2+P2)%P2;
			//全局变量对应的临时变量签名
			//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
			out_indent(indent_num, gen_out);
			gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C1 = " << "(" << "TC_" << *it << ".Data.C.C1 + " << constant3 << ")%P1 + g_GS_TL1;"<<endl;
			out_indent(indent_num, gen_out);
			gen_out << get_bind_local_var(*it, function_name) << ".Data.C.C2 = " << "(" << "TC_" << *it << ".Data.C.C2 + " << constant4 << ")%P2 + g_GS_TL2;"<<endl;
			out_indent(indent_num, gen_out);
			gen_out <<get_bind_local_var(*it, function_name) <<".Id = " <<"F_VCL_RegisterCodeId(" <<ret_sig <<");"<<endl;
		}*/
		int index = 0;
		int new_sig=get_sig_index(&index);//为左值变量分配新签名

		//如果左值为全局变量，对其绑定的局部变量进行签名更新操作
		if(is_global_var(var_name))
			add_new_sig(v, get_bind_local_var(var_name, function_name),new_sig,var_scope);//将新签名插入动态签名表
		else
			add_new_sig(v,var_name,new_sig,var_scope);//将新签名插入动态签名表
		
		//计算补偿常数
		long long constant1 = (long long) new_sig
				- get_current_sig(v, "retVal_" + fun_name, "global")
				- get_current_sig(v, "red_dF_" + fun_name, "global");
		constant1 = ((constant1 % P1) + P1) % P1;
		long long constant2 = (long long) new_sig
						- get_current_sig(v, "retVal_" + fun_name, "global")
						- get_current_sig(v, "red_dF_" + fun_name, "global");
		constant2 = ((constant2 % P2) + P2) % P2;

		//如果是调试模式，则输出调试信息：左值变量的新签名
		if (true == debug_flag)
		{/*
			out_indent(indent_num, gen_out);
			if(is_global_var(var_name))
				gen_out << "//" << get_bind_local_var(var_name, function_name) << "'s new sig : " << new_sig << endl;
			else
				gen_out << "//" << var_name << "'s new sig : " << new_sig << endl;

		*/}

		//输出函数赋值的高位代码
		if(var_name == "AC_Void_Func_RetVal")
		{/*
			out_indent(indent_num, gen_out);
			gen_out << "AC_Void_Func_RetVal" << ".Data.F = AC_RetVal_" << fun_name << ".Data.F;" <<endl;
			//输出函数赋值的低位代码
			out_indent(indent_num, gen_out);
			gen_out <<  "AC_Void_Func_RetVal" << ".Data.C.C1 = (AC_RetVal_" << fun_name << ".Data.C.C1 + "+int2str(constant1) +")%P1;" <<endl;
			out_indent(indent_num, gen_out);
			gen_out <<  "AC_Void_Func_RetVal" << ".Data.C.C2 = (AC_RetVal_" << fun_name << ".Data.C.C2 + "+int2str(constant2) +")%P2;" <<endl;
			out_indent(indent_num, gen_out);
			gen_out<<"AC_Void_Func_RetVal"<<".Id = F_VCL_RegisterCodeId("<< new_sig <<");"<<endl;
		*/}
		else
		{/*
			if(is_global_var(var_name))
			{
				//左值为全局变量

				out_indent(indent_num, gen_out);
				gen_out << get_bind_local_var(var_name, function_name) << ".Data.F = AC_RetVal_" << fun_name << ".Data.F;" <<endl;
				//输出函数赋值的低位代码
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(var_name, function_name) << ".Data.C.C1 = (AC_RetVal_" << fun_name << ".Data.C.C1 + "+int2str(constant1) +")%P1;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out <<get_bind_local_var(var_name, function_name) << ".Data.C.C2 = (AC_RetVal_" << fun_name << ".Data.C.C2 + "+int2str(constant2) +")%P2;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out<<get_bind_local_var(var_name, function_name)<<".Id = F_VCL_RegisterCodeId("<< new_sig <<");"<<endl;
			}
			else
			{
				out_indent(indent_num, gen_out);
				gen_out << "TC_" << var_name << ".Data.F = AC_RetVal_" << fun_name << ".Data.F;" <<endl;
				//输出函数赋值的低位代码
				out_indent(indent_num, gen_out);
				gen_out << "TC_" << var_name << ".Data.C.C1 = (AC_RetVal_" << fun_name << ".Data.C.C1 + "+int2str(constant1) +")%P1;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out << "TC_" << var_name << ".Data.C.C2 = (AC_RetVal_" << fun_name << ".Data.C.C2 + "+int2str(constant2) +")%P2;" <<endl;
				out_indent(indent_num, gen_out);
				gen_out<<"TC_"<<var_name<<".Id = F_VCL_RegisterCodeId("<< new_sig <<");"<<endl;
			}
		*/}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding function assignment : " + str;
		throw error_info;
	}

}
