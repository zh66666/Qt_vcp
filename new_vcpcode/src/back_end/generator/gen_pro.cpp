#include "CodedVarBuild_Util.h"
#include "program.h"
#include "declaration.h"
#include "var_declaration.h"
#include "fun_declaration.h"
#include "var_info.h"
#include "cmnt.h"
//#include "TmpVarIndexRange.h"
#include "gen_secondary.h"
#include "gen_utils.h"
#include "gen_global_var.h"

// 以下定义在文件 : src/main_utils.c
extern int fake_line_num;
extern int line_num;
extern list<Var_info>* var_infos;
extern list<Cmnt>* cmnts;
extern string red_path;
extern string current_file_name;
//2010年3月10日追加
extern string file_info;
//2014-12-16
extern map<string, TFuncTmpRange> g_MapFuncTmpVar;

extern string int2str(int num);
extern string resolve_file_name(string& path_name);
extern void check_pt_arg(void *pt, string info);
extern void out_cmnt(list<Cmnt>* cmnt_list, int line, ofstream& out);

// 定义在文件 : ./gen_dec/gen_var_dec.c
extern void gen_var_dec(Var_declaration*);
extern void gen_var_dec_datetime(string s);

// 定义在文件 : ./gen_dec/gen_fun_dec.c
extern void gen_fun_dec(Fun_declaration*);

// 定义在文件 : back_end_utils.c
extern int tempvar_num_max;
extern int tempcondvar_num_max;
//定义在文件:
//李晨欢增加用来输出TC_a.Data.C.C1 = TC_a.Data.C.C1 + g_GS_TL1;
extern list<string>* datetime_var_list;
extern bool flag_var_dec;
extern vector<string> get_global_var_infunc(string func_name);
extern void gen_globalToProtect_dec(string strs, string fun_name,string var_scope);
extern void genProtectToGlobal(string strs, string fun_name, string var_scope);

// 功能：遍历声明链表，生成声明的冗余代码，根据声明的类型分别调用不同下层函数
// 输入参数：s-声明链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_declist(list<Declaration>* s)
{
	try
	{
		//检查参数：声明对象指针的有效性
		check_pt_arg(s, "declaration object's pointer");
		//遍历声明链表
		for (list<Declaration>::iterator iter = s->begin(); iter != s->end();
				iter++)
		{
			line_num = iter->get_line_begin(); //设置当前处理位于的行号
			fake_line_num = iter->get_fake_line(); //设置当前处理的伪行号
			//2010年3月10日追加
			file_info = iter->get_file_info();			//设置当前处理所位于得文件名

			out_cmnt(cmnts, fake_line_num, gen_out);			//输出声明之前的注释信息

			//判断声明是否为变量声明
			if (0 == iter->get_stamp())
			{
				//如果声明标签为0，说明是变量声明，则遍历变量声明
				flag_var_dec=true;
				gen_var_dec(iter->get_cvd());
			}
			else
			{
				//如果声明标签为不为0，说明是函数声明，则遍历函数声明
				gen_fun_dec(iter->get_cfd());
			}
		}

		if(flag_var_dec)
		{
			if ("F_VCE_CheckVar" == var_scope || "F_VCE_CheckArr" == var_scope)
			{
				//如果是校验接口就不对函数体内的声明进行编码
			}
			else
			{
				vector<string> globalvar = get_global_var_infunc(var_scope);
				vector<string>::iterator itrvec;
				for (itrvec = globalvar.begin(); itrvec != globalvar.end();
						itrvec++)
				{
					gen_globalToProtect_dec(*itrvec, var_scope, var_scope);
				}
			}

			check_pt_arg(datetime_var_list,
					"declaration which needs datetime  object's pointer");
			//遍历声明链表
			gen_out << endl;
			for (list<string>::iterator iter = datetime_var_list->begin();
					iter != datetime_var_list->end(); iter++)
			{
				//gen_var_dec_datetime(*iter);
			}
			//全局变量调整
			if ("F_VCE_CheckVar" == var_scope || "F_VCE_CheckArr" == var_scope)
			{
				//如果是校验接口就不对函数体内的声明进行编码
			}
			else
			{
				vector<string> globalvar = get_global_var_infunc(var_scope);
				vector<string>::iterator itrvec;
				for (itrvec = globalvar.begin(); itrvec != globalvar.end();
						itrvec++)
				{
					genProtectToGlobal(*itrvec, var_scope, var_scope);
				}
			}
			datetime_var_list->clear();
			flag_var_dec=false;
		}

	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "generate declaration's redundant codes : " + str;
		throw error_info;
	}
}

// 功能：遍历程序的中间结构，生成该程序对应的冗余代码，此模块为冗余代码生成模块的主模块，
//          在对当前中间结构遍历之前，先将拆分当前结构用到的临时变量的签名信息加入动态签名表，
//          结构遍历之后，再将这些临时变量从动态签名表中删除
// 输入参数：pro-经过转换的中间结构指针
// 返回值：成功则返回void，失败则抛出错误信息
//作者：李刚
void gen_pro(Program* pro)
{
	string solve_path;
	string solve_file;
	string output_tmp_path;
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(pro, "program struct's pointer");

		//先将拆分当前中间结构用到的临时变量加入动态签名表，因为后面在对该结构进行遍历生成
		//冗余代码的时候，会用到这些临时变量，包括普通临时变量和条件临时变量

		solve_path = current_file_name;		//存储当前被处理的文件路径名
		solve_file = resolve_file_name(solve_path);		//解析当前被处理的文件名
		output_tmp_path = red_path + "/red_tmp_" + solve_file;		//构造冗余代码文件名
		gen_out.open((char*) output_tmp_path.c_str());		//打开冗余代码文件流

		//判断文件是否创建成功
		if (!gen_out.is_open())
		{
			//为true，说明文件创建失败，抛出错误信息
			string error_info = output_tmp_path + " creating failed!";
			throw error_info;
		}
	} catch (string str)
	{
		cerr << "error : when coding file \"" << current_file_name
				<< "\" : generate redundant codes," << str << endl;
		exit(-1);
	}

	try
	{
		//输出冗余代码文件中的头文件引用
		gen_out << "#include \"Interface.h\" " << endl;
		gen_out << "#include \"Interface.c\" " << endl;
		int P_0=*iter;//初始签名by周航
		gen_out << "long long RS="<<P_0<<";"<< endl;
		gen_out << "int S_Fun_entry="<<P_0<<";"<< endl;
		gen_out << "int S_Fun_out;"<< endl;
		//gen_out << "#include \"CFG_Service.h\" " << endl;
/*
		gen_out << "#include \"stdio.h\" " << endl;
		gen_out << "long long A1=8193;" << endl;
		gen_out << "long long A2=8191;" << endl;
		gen_out << "long long P_pre_idx=0;" << endl;
		gen_out << "long long R=0;" << endl;
		gen_out << "long long mul_temp_A_1,mul_temp_A_2,mul_temp_B_1,mul_temp_B_2;" << endl;//乘法中用到的中间变量
		gen_out << "int div_temp_var_decode,div_temp_l,div_temp_r,div_temp_var;" << endl;//乘法中用到的中间变量
		gen_out << "int Select_decode,S_select,Select_mark;" << endl;//分支入口解码值 分支签名 分支入口高位标识符
		gen_out << "int Iterat_decode,Iterat_mark;" << endl;// 循环入口解码值 循环入口高位标识符
*/
		//遍历中间结构中的声明链表，生成冗余代码
		gen_declist(pro->get_declist());

		//冗余代码生成结束后，将当前动态签名表中的临时变量信息清除
		for (list<Var_info>::iterator iter = var_infos->begin();
				iter != var_infos->end();)
		{
			//判断当前迭代器所指对象的标签信息是否是"tmpArray"或者"tmpCondArray"
			if ("tmpArray" == iter->get_stamp()
					|| "tmpCondArray" == iter->get_stamp())
			{
				//如果是，则说明当前所指对象即为临时变量，将其删除
				iter = var_infos->erase(iter);
			}
			else
			{
				//如果不是，则继续查找动态签名表
				iter++;
			}
		}

		//清除存储的当前文件中的注释
		cmnts->clear();

		gen_out.close();		//关闭冗余代码文件流

		//进行二次输出
		string OutputFile_Path = red_path + "/red_" + solve_file;
		Second_Output_File(OutputFile_Path, output_tmp_path);

		//删除临时文件
		remove(output_tmp_path.c_str());

	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "generate program's redundant codes : " + str;
		throw error_info;
	}
}

