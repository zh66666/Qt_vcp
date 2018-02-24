#include "gen_utils.h"
#include "return_stmt.h"
#include "expression.h"
#include "var.h"
#include "gen_global_var.h"

// 定义在文件: src/main_utils.cpp
extern bool debug_flag;

//定义在gen_compod_stmt.cpp
extern string function_name;

extern string get_bind_local_var(string global_var,string func_name);
extern string int2HexStr_data(const int num);

// 功能：对返回语句进行编码。根据返回表达式的不同类型，生成不同的冗余代码
// 输入参数：s-返回语句对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_return_stmt(Return_stmt* s)
{
	try
	{
		//检查参数：返回语句对象指针的有效性
		check_pt_arg(s,"return statement object's pointer");
		
		//输出返回值语句之前的全局变量调整代码
		int constant1 = 0, constant2 = 0, constant3 = 0, constant4 = 0;
		vector<string> global_vars = get_global_var_infunc(function_name);
		vector<string>::iterator it;
		for(it=global_vars.begin();it!=global_vars.end();it++)
		{
			out_indent(indent_num, gen_out);
			gen_out << "TC_" + *it + ".Data.F =" << get_bind_local_var(*it, function_name) + ".Data.F;" <<endl;
			int constant1 = 0, constant2 = 0;
			//全局变量签名
			constant1 = get_current_sig(var_infos, *it, "global");
			constant2 = get_current_sig(var_infos, get_bind_local_var(*it, function_name), function_name);
			constant3 = ((constant1 - constant2)%P1+P1)%P1;
			constant4 = ((constant1 - constant2)%P2+P2)%P2;
			//全局变量对应的临时变量签名
			//constant2 = get_current_sig(var_infos, get_bind_local_var(*it, fun_name), "global");
			out_indent(indent_num, gen_out);
			gen_out << "TC_" << *it + ".Data.C.C1 = " << "(" + get_bind_local_var(*it, function_name) + ".Data.C.C1 + " + int2str(constant3) + " - g_GS_TL1)%P1;"<<endl;
			out_indent(indent_num, gen_out);
			gen_out << "TC_" << *it + ".Data.C.C2 = " << "(" + get_bind_local_var(*it, function_name) + ".Data.C.C2 + " + int2str(constant4) + " - g_GS_TL2)%P2;"<<endl;
		}

		//判断返回语句是否带有返回值
		if(0==s->get_stamp())
		{
			//如果无返回值，则忽略
		}
		else
		{
			//如果有返回值，则判断返回表达式的类型
			//判断返回的是否是常数
			if(3==s->get_ce()->get_stamp())
			{
				//如果是常数
				string constant_name=s->get_ce()->get_c();//取出常数字符串
				int high=str2int(constant_name);//转换成对应的整数值
				int index = 0;

				//这里用给常数分配签名的方法代替
				//get_const_xxxxx

				int constant_sig = getConstantSig(high);


				//计算常数的低位
//				long long low = -((long long) high * K_NUM) % (long long) A
//						+ (long long) constant_sig;
//				low = (low % A + A) % A;
				long long low1 = -((long long) high * K_NUM_P1) % (long long) P1
						+ (long long) constant_sig;
				low1 = (low1 % P1 + P1) % P1;
				long long low2 = -((long long) high * K_NUM_P2) % (long long) P2
						+ (long long) constant_sig;
				low2 = (low2 % P2 + P2) % P2;
				//计算补偿常数
				long long constant = (long long) get_current_sig(var_infos,
						"retVal_" + var_scope, "global")
						- (long long) constant_sig;
				long long constant1 = (constant % (long long) P1 + (long long) P1)
						% (long long) P1;
				long long constant2 = (constant % (long long) P2 + (long long) P2)
										% (long long) P2;
				//如果是调试模式，则输出调试信息：返回的常数的签名，函数返回值的固定签名
				/*if(true==debug_flag)
				{
					out_indent(indent_num, gen_out);
					gen_out << "//" << constant_name << "'s  sig :"
							<< constant_sig << " retVal_" << var_scope
							<< "'s  sig :"
							<< get_current_sig(var_infos, "retVal_" + var_scope,
									"global") << endl;
				}*/
				gen_out << "return" << " "<<constant_name<< ";"<<endl;}
				//输出返回值语句编码的高位代码
				/*out_indent(indent_num, gen_out);
				gen_out << "AC_RetVal_" << var_scope << ".Data.F = "
						<<  "CONST_NUM_" + int2HexStr_data(high) << ".Data.F;" << endl;

				//输出返回值语句编码的低位代码
				out_indent(indent_num, gen_out);
				//不使用64位封装方法的输出结果
				gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<<"CONST_NUM_" + int2HexStr_data(high)<< ".Data.C.C1"<<" + g_GS_TL1 + g_ExFun_" + var_scope<<"  + "<<constant1<<")%P1;"<<endl;
				out_indent(indent_num, gen_out);
				gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<<"CONST_NUM_" + int2HexStr_data(high)<< ".Data.C.C2"<<" + g_GS_TL2 + g_ExFun_" + var_scope<<"  + "<<constant2<<")%P2;"<<endl;
			}*/
			else if (4 == s->get_ce()->get_stamp()
					&& 0 == s->get_ce()->get_cvar()->get_stamp())
			{
				//如果返回的是个普通变量
				string var_name = s->get_ce()->get_cvar()->get_name();//取出该变量名称
				if(!is_global_var(var_name))
				{
					//计算补偿常数
					long long constant = (long long) get_current_sig(var_infos,
							"retVal_" + var_scope, "global")
							- (long long) get_current_sig(var_infos, var_name,
									var_scope);
					long long constant1 = (constant % (long long) P1 + (long long) P1) % P1;
					long long constant2 = (constant % (long long) P2 + (long long) P2) % P2;
					//如果是调试模式，则输出调试信息：返回的变量的签名，函数返回值的固定签名
					/*if (true == debug_flag)
					{
						out_indent(indent_num, gen_out);
						gen_out << "//retVal_" << var_scope << "'s  sig : "
								<< get_current_sig(var_infos, "retVal_" + var_scope,
										"global") << endl;
						out_indent(indent_num, gen_out);
						gen_out << "//" << var_name << "'s  sig : "
								<< get_current_sig(var_infos, var_name, var_scope)
								<< endl;
					}*/
					if(var_name.size()>7 && var_name.substr(0,7) == "AC_Tmp_")
					{
						//输出返回值语句编码的高位代码
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_" << var_scope << ".Data.F = " << var_name
								<< ".Data.F;" << endl;

						//不使用64位封装方法的输出结果
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<< var_name<< ".Data.C.C1 +"<<"g_ExFun_" + var_scope<<" + "<<constant1<<")%P1;"<<endl;
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<< var_name<< ".Data.C.C2 +"<<"g_ExFun_" + var_scope<<" + "<<constant2<<")%P2;"<<endl;
					}
					else
					{

						/*<< ".Data.F = TC_" << var_name
								<< ".Data.F;" << endl;*/
						gen_out << "long long "<<var_name<<"="<<"check_decode (" <<var_name<<"_1,"<<var_name<<"_2);"<<endl;
						gen_out << "return" << " "<<var_name<< ";"<<endl;
						//不使用64位封装方法的输出结果
						out_indent(indent_num, gen_out);
						/*gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<<"TC_" + var_name<< ".Data.C.C1 +"<<"g_ExFun_" + var_scope<<" + "<<constant1<<")%P1;"<<endl;
						out_indent(indent_num, gen_out);
						gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<<"TC_" + var_name<< ".Data.C.C2 +"<<"g_ExFun_" + var_scope<<" + "<<constant2<<")%P2;"<<endl;
					*/
					}



				}
				else
				{
					//如果返回值是全局变量
					//计算补偿常数
					long long constant = (long long) get_current_sig(var_infos,
							"retVal_" + var_scope, "global")
							- (long long) get_current_sig(var_infos, get_bind_local_var(var_name, function_name),
									var_scope);
					long long constant1 = (constant % (long long) P1 + (long long) P1) % P1;
					long long constant2 = (constant % (long long) P2 + (long long) P2) % P2;
					//如果是调试模式，则输出调试信息：返回的变量的签名，函数返回值的固定签名
					if (true == debug_flag)
					{
						out_indent(indent_num, gen_out);
						gen_out << "//retVal_" << var_scope << "'s  sig : "
								<< get_current_sig(var_infos, "retVal_" + var_scope,
										"global") << endl;
						out_indent(indent_num, gen_out);
						gen_out << "//" << get_bind_local_var(var_name, function_name) << "'s  sig : "
								<< get_current_sig(var_infos, get_bind_local_var(var_name, function_name), var_scope)
								<< endl;
					}

					//输出返回值语句编码的高位代码
					out_indent(indent_num, gen_out);
					gen_out << "AC_RetVal_" << var_scope << ".Data.F =" << get_bind_local_var(var_name, function_name)
							<< ".Data.F;" << endl;
					//不使用64位封装方法的输出结果
					out_indent(indent_num, gen_out);
					gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C1 = ("<< get_bind_local_var(var_name, function_name)<< ".Data.C.C1 +"<<"g_ExFun_" + var_scope<<" + "<<constant1<<")%P1;"<<endl;
					out_indent(indent_num, gen_out);
					gen_out << "AC_RetVal_"<<(var_scope)<<".Data.C.C2 = ("<< get_bind_local_var(var_name, function_name)<< ".Data.C.C2 +"<<"g_ExFun_" + var_scope<<" + "<<constant2<<")%P2;"<<endl;
				}

			}
			else
			{
				//如果返回值表达式为其他类型，则抛出错误信息
				string error_info = "return expression is invalid!";
				throw error_info;
			}
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding return statement : " + str;
		throw error_info;
	}
}
