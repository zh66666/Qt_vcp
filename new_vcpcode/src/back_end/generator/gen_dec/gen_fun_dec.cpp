#include "gen_utils.h"
#include "compound_stmt.h"
#include "param.h"
#include "fun_declaration.h"
#include "fun_info.h"
#include "declarator.h"
#include "type_specifier.h"
#include "declaration_specifiers.h"
#include "bool_var.h"
#include "gen_secondary.h"
//定义在 ../gen_utils.cpp
extern int dD_num;

// 定义在文件: src/main_utils.cpp
extern bool debug_flag;
extern list<Fun_info>* fun_infos;
extern list<string>* function_names;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;
extern map<string ,int> g_ExFun_Hash_map;
extern map<string ,string> AC_RetVal_map;
extern map<string ,string> AC_ParmVar_map;

// 定义在文件 : ../gen_stmt/gen_compd_stmt.cpp
extern void gen_fun_body(Compound_stmt* ,Fun_info*);
extern void search_and_get_global_var(Fun_declaration* f_d);
//定义在文件./gen_global_var.cpp
void search_and_get_global_var(Fun_declaration* f_d);

// 功能：对函数的形参进行编码，输出形参的高位和低位。如果发现形参声明不完整，则抛出错误信息。
// 			注意：此函的一个限制条件是，编码的函数参数中必须不能含有数组形参和结构体形参。
// 输入参数：s-函数形参对象指针
// 返回值：成功则返回形参的名字，失败则抛出错误信息
// 作者：李刚、梁盟磊
static string gen_param(Param* s)
{
	try
	{
		//检查参数：函数形参对象指针的有效性
		check_pt_arg(s,"parameter object's pointer");
		
		//匹配函数形参的标签值，判断它的类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//为0，说明是完整的形参声明，比如fun（int a)；中的形参
				//注意：这里假设形参中没有数组和结构体形参
				//输出形参的高低位
				//gen_out<<str_int32<<" H_"<<s->get_dec()->get_name()<<",";
				//gen_out<<str_int32<<" L_"<<s->get_dec()->get_name();
				string type_name=s->get_type_spec_list()->front().get_type_name();
				if(type_name=="int")
				{
				gen_out<<"long long "<<s->get_dec()->get_name()<<"_1,";
				gen_out<<"long long "<<s->get_dec()->get_name()<<"_2";
				}
				//返回形参的名称
				return s->get_dec()->get_name();
			}
			break;	
				
			case 1:
			{
				//为1，说明是不完整的形参声明，比如fun(int);，抛出错误信息
				string error_info="the function's parameter has no name! ";
				throw error_info;
			}
			break;
			
			default:
			{
				//标签为其他值，非法，抛出错误信息
				string error_info="the function's parameter's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding function's parameters : "+str;
		throw error_info;
	}
}

// 功能：遍历函数的形参链表，对所有形参进行编码。如果函数形参为空，或者为void，则直接输出void。
// 输入参数：s-函数形参链表指针
// 输出参数：f_i-函数信息对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、梁盟磊
static void gen_paramlist(list<Param>* s,Fun_info* f_i)
{
	try
	{
		//检查参数：函数形参链表指针的有效性
		check_pt_arg(s,"parameter list's pointer");
		//检查参数：函数信息对象指针的有效性
		check_pt_arg(f_i,"function information object's pointer");
		
		//判断形参链表是否为空
		if(s->size()==0)
		{
			//如果为空，说明该函数参数为空
			f_i->set_para_stamp(0);//设置函数信息对象的参数标签为0，表明该函数无参数
			gen_out<<"void";//输出void
			return ;
		}
		//判断函数形参个数是否为1，并且此形参是否声明完整
		else if(1==s->size() && s->front().get_stamp()==1)
		{
			//如果形参个数为1，且此形参不带形参名，则继续判断形参的类型是否为void
			if(s->front().get_type_spec_list()->size()==1 
			    && s->front().get_type_spec_list()->front().get_stamp()==0
				&& s->front().get_type_spec_list()->front().get_type_name()=="void")
			{
				//如果是void，则设置函数信息对象的参数标签为0，表明函数无参数
				f_i->set_para_stamp(0);	
				gen_out<<"void";//输出void
				return ;
			}
			else
			{
				//其他情况，忽略
				;
			}
		}
		else
		{
			//如果形参个数大于1，或者为1但不为void
			list<string>* para_names=new list<string>;//存储形参的名称
			//遍历形参链表
			for(list<Param>::iterator iter=s->begin();iter!=s->end();iter++)
			{	
				//判断当前元素是否为链表的最后一个
				if(iter == --s->end())
				{
					//如果是最后一个，则对该形参进行编码后，不输出逗号
					string para_name=gen_param(&(*iter));
					//存储该形参名
					para_names->push_back(para_name);
				}	
				else
				{	
					//如果不是最后一个，则对该形参进行编码后，输出逗号
					string para_name=gen_param(&(*iter));
					//存储该形参名
					para_names->push_back(para_name);
					gen_out<<", ";
				}
			}
	
			f_i->set_para_stamp(1);//设置函数信息对象的参数标签值为1，表明该函数有形参
			f_i->set_para_num(s->size());//设置函数信息对象的成员：函数形参个数
			f_i->set_paras(para_names);//设置函数信息对象的成员：函数形参的名称
			delete para_names;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding function's parameter list  : "+str;
		throw error_info;
	}
}

// 功能：为函数附加变量分配签名，并将其加入动态签名表中
// 输入参数：fun_name-函数名称
// 返回值：成功则返回为函数附加变量分配的签名值
// 作者：李刚、梁盟磊
static int  assign_fun_add_sig(string fun_name)
{
	try
	{
		//检查参数：函数名称字符串的有效性
		check_str_arg(fun_name,"function's name");
		
		string fun_add_name="red_dF_"+fun_name;//构造函数附加变量的名字
		int sigIndex = 0;
		int red_DF_sig=get_sig_index(&sigIndex);//为函数附加变量分配签名
		
		//将函数附加变量加入动态签名表中
		add_new_vars(var_infos,fun_add_name,red_DF_sig,"global","red_dF", sigIndex);
		
		return red_DF_sig;//返回函数附加变量的签名
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="assign function additonal variable's signature : "+str;
		throw error_info;
	}
}

// 功能：为函数参数附加变量和函数的参数分配签名，并将其加入动态签名表
// 输入参数：para_names-存放有函数参数名称的链表指针
// 			      fun_name-函数名
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、梁盟磊
static void assign_fun_para_sig(list<string>* para_names,string fun_name)
{
	try
	{
		//检查参数：函数名称字符串的有效性
		check_str_arg(fun_name,"function's name ");
		//检查参数：函数参数名称链表指针的有效性
		check_pt_arg(para_names,"function parameter's name list pointer ");
		
		//如果不为空，则遍历存有函数参数名的链表，为每个参数分配签名
		string str_para="AC_ParaVar_"+fun_name+"_";//构造函数参数附加变量的名字
		int i=0;//记录已经分配签名的参数的个数
		//遍历函数参数名链表
		for(list<string>::iterator iter=para_names->begin();
			   iter!=para_names->end();iter++,i++)
		{
			int sigIndex = 0;
			int sig=get_sig_index(&sigIndex);//为当前参数分配签名
			//将参数附加变量加入动态签名表
			add_new_vars(var_infos,str_para+int2str(i),sig,"global","para", sigIndex);
			//将参数加入动态签名表
			add_new_vars(var_infos,(*iter),sig,fun_name,"para", sigIndex);
			string var_name = str_para+int2str(i);
			string C1 = int2str((sig % P1 - P1) % P1);
			string C2 = int2str((sig % P2 - P2) % P2);
			string var_value = "{{0,{"+ C1 +","+ C2 +"}},"+ int2str(sigIndex) +"};";
			AC_ParmVar_map[var_name] = var_value;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="assign function's parameter additonal variable's signature : "+str;
		throw error_info;
	}
}

// 功能：为函数分配签名，根据函数有无参数，有无返回值，来具体分配函数的签名，包括其附加变量的签名，
// 参数附加变量的签名，返回值附加变量的签名等，并将这些签名信息加入动态签名表中
// 输入参数：f_i-函数信息对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、梁盟磊
static void assign_fun_sigs(Fun_info* f_i)
{
	try
	{
		//检查参数：函数信息对象指针的有效性
		check_pt_arg(f_i,"function information object's pointer");
		
		int para_tmp=f_i->get_para_stamp();//记录该函数信息对象的参数标签
		int return_tmp=f_i->get_return_stamp();//记录该函数信息对象的返回值标签
		string fun_name=f_i->get_name();//记录该函数的名称
		//根据参数标签和返回值标签来分配函数的相关签名
		//判断该函数是否既有参数又有返回值
		if(para_tmp==1 && return_tmp==1)
		{
			//为函数附加变量分配签名，并存储
			int red_DF_sig=assign_fun_add_sig(fun_name);//记录函数附加变量的签名
			f_i->set_sig(red_DF_sig);//设置函数信息对象签名成员的值为函数附加变量的签名
			g_ExFun_Hash_map[fun_name] = red_DF_sig;

			//如果既有参数又有返回值，先为函数返回值附加变量分配签名，并将其加入动态签名表
			int sigIndex = 0;
			int ret_sig=get_sig_index(&sigIndex);//为返回值分配固定签名
			add_new_vars(var_infos,"retVal_"+fun_name,ret_sig,"global","retVal", sigIndex);
			string C1 = int2str((ret_sig % P1 - P1) % P1);
			string C2 = int2str((ret_sig % P2 - P2) % P2);
			string retValTCODE = "{{0,{"+ C1 +","+ C2 +"}},"+ int2str(sigIndex) +"};";
			AC_RetVal_map[fun_name] = retValTCODE;

			//为函数参数分配签名
			assign_fun_para_sig(f_i->get_paras(),fun_name);
		}
		//判断该函数是否只有返回值而无参数
		else if(para_tmp==0 && return_tmp==1)
		{
			//为函数附加变量分配签名
			int red_DF_sig=assign_fun_add_sig(fun_name);//记录函数附加变量的签名
			f_i->set_sig(red_DF_sig);//设置函数信息对象签名成员的值为函数附加变量的签名
			g_ExFun_Hash_map[fun_name] = red_DF_sig;

			int sigIndex = 0;
			int ret_sig = get_sig_index(&sigIndex);
			//如果该函数只有返回值，先为函数返回值附加变量分配签名，并将其加入动态签名表
			add_new_vars(var_infos,"retVal_"+fun_name, ret_sig, "global", "retVal", sigIndex);
			string C1 = int2str((ret_sig % P1 - P1) % P1);
			string C2 = int2str((ret_sig % P2 - P2) % P2);
			string retValTCODE = "{{0,{"+ C1 +","+ C2 +"}},"+ int2str(sigIndex) +"};";
			AC_RetVal_map[fun_name] = retValTCODE;
		}
		//判断该函数是否只有参数而无返回值
		else if(para_tmp==1 && return_tmp==0)
		{
			//为函数附加变量分配签名
			int red_DF_sig=assign_fun_add_sig(fun_name);//记录函数附加变量的签名
			f_i->set_sig(red_DF_sig);//设置函数信息对象签名成员的值为函数附加变量的签名
			g_ExFun_Hash_map[fun_name] = red_DF_sig;

			//如果该函数只有参数，为函数参数分配签名
			assign_fun_para_sig(f_i->get_paras(),fun_name);
		}
		else
		{
			//如果该函数无参无返回值，则不分配任何签名
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="assign function's signatures : "+str;
		throw error_info;
	}
}

// 功能：根据函数名称，清除动态签名表中的该函数的局部变量信息。包括该函数中声明的一般变量和所有的
// 			数组变量，这些变量在对该函数编码结束后不再会使用，故清除之。
// 输入参数：v_i-动态签名链表指针
// 			      fun_name-函数名称
// 输出参数：v_i-动态签名链表指针
// 返回值：成功则返回void，失败则抛出错误信息。
// 作者：李刚、梁盟磊
static void  clear_local_vars(list<Var_info>* v_i,string fun_name)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(v_i,"var_info list's pointer");
		check_str_arg(fun_name,"function's name");
		
		//遍历动态签名表
		for(list<Var_info>::iterator iter=v_i->begin();iter!=v_i->end();)
		{
			//判断当前迭代器所指的对象的作用域是否与传入的函数名一致
			if(fun_name==iter->get_scope())
			{
				//如果一致，说明当前对象是该函数内的局部变量
				//继续判断当前对象是否为普通变量或者数组变量
				if("common"==iter->get_stamp() || "array"==iter->get_stamp()
				   || "nodt_array"==iter->get_stamp())
				{
					//如果为一般变量或者数组变量，则删除该变量
					iter=v_i->erase(iter);
				}
				else
				{
					//如果不为一般变量和数组变量，则继续遍历
					iter++;
				}
			}
			else
			{
				//如果不是该函数中的局部变量，则继续遍历
				iter++;
			}	
		}
		
		//遍历布尔变量真假值链表
		for(list<Bool_var>::iterator iter=bool_vars->begin();iter!=bool_vars->end();)
		{
			//判断当前布尔量的作用域是否为当前函数名
			if(fun_name==iter->get_scope())
			{
				//如果是，则将此布尔变量删除
				iter=bool_vars->erase(iter);
			}
			else
			{
				//如果不是，则继续向前遍历
				iter++;
			}
		}
		
		//遍历布尔真假值签名链表
		for(list<Bool_var>::iterator iter=bool_sigs->begin();iter!=bool_sigs->end();)
		{
			//判断当前布尔量的作用域是否为当前函数名
			if(fun_name==iter->get_scope())
			{
				//如果是，则将此布尔变量删除
				iter=bool_sigs->erase(iter);
			}
			else
			{
				//如果不是，则继续向前遍历
				iter++;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="clear local var_infos : "+str;
		throw error_info;
	}
}

// 功能：对函数声明进行编码，包括函数名，函数参数以及函数体的编码。同时为函数分配签名，
// 			包括函数的附加变量的签名，函数参数的签名等。函数编码结束后，还要将函数体内的
//			局部变量信息从动态签名表中清除
// 输入参数：f_d-函数声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚、梁盟磊
void gen_fun_dec(Fun_declaration* f_d)
{
	try
	{
		//检查参数：函数声明对象指针的有效性
		check_pt_arg(f_d,"function declaration object's pointer");

		string fun_name=f_d->get_fun_name();//存储函数名
		string fun_type_name =f_d->get_decl_spec()->get_type_spec_list()->front().get_type_name();//存储函数返回值类型
		var_scope=fun_name;//设置作用域为该函数名称
		string param_error = "fun_type_nameThe check or IO interface's paramlist error!";
		
		//新建一个函数信息对象，存储该函数的相关信息
		Fun_info* fun_info_tmp=new Fun_info(fun_name);
		//判断该函数的返回值类型是否为void
		if(f_d->get_decl_spec()->get_type_spec_list()->size()==1
			&& f_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()==0
			&& f_d->get_decl_spec()->get_type_spec_list()->front().get_type_name()=="void")
		{
			//如果是void，则设置函数信息对象的成员：返回值标签为0，表明该函数无返回值
			fun_info_tmp->set_return_stamp(0);
		}
		else
		{
			//如果返回不为void，则忽略
			;
		}
		out_indent(indent_num,gen_out);
		if("F_VCE_CheckVar" == fun_name)
		{
			if(f_d->get_params()->size() == 2)
			{
				gen_out<<"void F_VCL_CheckCodeVar(";
				//输出形参的编码，同时设置函数信息对象的相关成员
				gen_out<<"T_CODE inVar, INT32 inErrNo";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}

		}
		else if("F_VCE_CheckArr" == fun_name)
		{
			if(f_d->get_params()->size() == 2)
			{
				gen_out<<"void F_VCL_CheckCodeArr(";
				//输出形参的编码，同时设置函数信息对象的相关成员
				gen_out<<"T_CODE inArr[], INT32 inStart, INT32 inSize, INT32 inErrNo";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataFetch" == fun_name)
		{
			if(f_d->get_params()->size() ==1)
			{
				gen_out<<"extern void F_USR_DataFetch(";
				//输出形参的编码，同时设置函数信息对象的相关成员
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataRecv" == fun_name)
		{
			if(f_d->get_params()->size() == 1)
			{
				gen_out<<"extern void F_USR_DataRecv(";
				//输出形参的编码，同时设置函数信息对象的相关成员
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataSend" == fun_name)
		{
			if(f_d->get_params()->size() == 1)
			{
				gen_out<<"extern void F_USR_DataSend(";
				//输出形参的编码，同时设置函数信息对象的相关成员
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else if("F_USR_DataDeliver" == fun_name)
		{
			if(f_d->get_params()->size() == 1)
			{
				gen_out<<"extern void F_USR_DataDeliver(";
				//输出形参的编码，同时设置函数信息对象的相关成员
				gen_out<<"INT32 inBufId";
				gen_out<<")";
			}
			else
			{
				throw param_error;
			}
		}
		else
		{
			gen_out<<fun_type_name <<" "<<fun_name<<"(";
			//输出形参的编码，同时设置函数信息对象的相关成员
			gen_paramlist(f_d->get_params(),fun_info_tmp);
			gen_out<<")";
		}
		
		//判断是否在全局函数名链表中找到了该函数的名字
		if(function_names->end()==find(function_names->begin(),
		                                                           function_names->end(),fun_name))
		{
			if("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name)
			{
				if(f_d->get_params()->size() == 2)
				{
					//校验和外部输入输出接口函数不需要进行编码
				}
				else
				{
					throw param_error;
				}
			}
			else if("F_USR_DataFetch" == fun_name
								|| "F_USR_DataRecv" == fun_name || "F_USR_DataSend" == fun_name || "F_USR_DataDeliver" == fun_name)
			{
				if(f_d->get_params()->size() == 1)
				{
					//校验和外部输入输出接口函数不需要进行编码
				}
				else
				{
					throw param_error;
				}
			}
			else
			{

				//如果没有找到，则说明是首次遇到该函数，需要分配函数的相关签名
				assign_fun_sigs(fun_info_tmp);
				function_names->push_back(fun_name);//将该函数名加入全局函数名链表
				fun_infos->push_back(*fun_info_tmp);//将该函数信息对象加入全局函数信息链表

			}
		}
		else
		{
			//如果找到了，说明不是首次遇到该函数，不需要再分配签名
			;
		}
		
		//判断函数声明是否带有函数体
		if(0==f_d->get_stamp())
		{
			//如果带有函数体，则对函数体进行编码
			dD_num = 0;//将dD下标置0
			//闪闪追加 5/20
			search_and_get_global_var(f_d);
			gen_out<<endl;
			gen_fun_body(f_d->get_ccstmt(),fun_info_tmp);
		}
		else
		{
			//如果不带有复合语句，则输出分号
			gen_out<<";"<<endl;
		}
		//该函数编码结束后，在动态签名表中，清除该函数内的局部变量
		clear_local_vars(var_infos,fun_name);
		//将作用域置为"global"
		var_scope="global";
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding function declaration : "+str;
		throw error_info;
	}
}



