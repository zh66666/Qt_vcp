#include "statement.h"
#include "expression.h"
#include "var.h"
#include "cmnt.h"
#include "return_stmt.h"
extern int S_last,S_this;
extern vector<int> sig_vector;
extern vector<int>::iterator iter;
class Var_info;

//用于记录循环体内的需要调整的变量信息，包括名称，是否为左值还是右值
struct adj_var
{
	string name;//记录变量名字
	string stamp;//记录为左值还是右值
};

//定义在文件: ./while_if_utils.cpp
extern list<adj_var>* adj_name_list;
extern void get_var_names(Statement* s,list<adj_var>* l);

// 定义在文件：./gen_utils.cpp
extern ofstream gen_out;
extern int iteration_level;
extern int select_level;

// 定义在文件 : src/main_utils.cpp
extern int fake_line_num;
extern int line_num;
extern int indent_num;
extern void out_indent(int,ofstream&);
extern list<Cmnt>* cmnts;
extern void check_pt_arg(void *pt,string info);
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);

// 定义在文件: ./gen_compd_stmt.c
extern void gen_compd_stmt(Compound_stmt*,list<Var_info>*);
// 定义在文件 : ./gen_ret_stmt.c
extern void gen_return_stmt(Return_stmt*);
// 定义在文件 : ./gen_var_stmt.c
extern void gen_var_assign(string,Expression*,list<Var_info>*);
// 定义在文件 : ./gen_array_stmt.c
extern void gen_array_assign(Var*,Expression*,list<Var_info>*);
// 定义在文件 : ./gen_fun_stmt.c
extern void gen_fun_call(Expression*,list<Var_info>*);
// 定义在文件 : ./gen_select_stmt.c
extern void gen_select_stmt(Selection_stmt* s,list<Var_info>* v);
// 定义在文件 : ./gen_iterat_stmt.c
extern void gen_iterat_stmt(Iteration_stmt* s,list<Var_info>* v);

// 功能：遍历语句，根据语句的类型调用不同的编码函数进行编码
// 输入参数：s-语句对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_stmt(Statement* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"statement object's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		//判断语句类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//如果为复合语句，则调用复合语句编码函数
				gen_compd_stmt(s->get_ccstmt(),v);
			}
			break;
			
			case 1:
			{
				//如果为分支语句，则调用分支语句编码函数
				gen_select_stmt(s->get_csstmt(),v);
			}
			break;
			
			case 2:
			{
				//如果为循环语句，则调用循环语句编码函数
				gen_iterat_stmt(s->get_cistmt(),v);
			}
			break;
			
			case 3:
			{
				//调整函数出口签名
				out_indent(indent_num,gen_out);
				gen_out<<"S_Fun_out=S_Fun_out_this;"<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"RS=RS+S_Fun_out_this-"<<*iter<<";"<<endl;
				gen_out<<endl;
				//如果为返回语句，则调用返回语句编码函数
				Return_stmt* ss = s->get_crs();
				if(0==ss->get_stamp())
				{
					//如果没有返回值则忽略
				}
				else
				{
					gen_return_stmt(s->get_crs());
				}

			}
			break;
			
			case 4:
			{
				//如果为赋值语句，则先判断左值变量是否为普通变量
				if(s->get_cvar()->get_stamp()==0)
				{
					//如果是普通变量，则调用普通变量赋值编码函数
					gen_var_assign(s->get_cvar()->get_name(),s->get_ce(),v);
				}
				else
				{
					//如果是数组变量，则调用数组赋值编码函数
					gen_array_assign(s->get_cvar(),s->get_ce(),v);
				}
			}
			break;
			
			case 5:
			{
				//如果为表达式语句，则此表达式必须为函数调用表达式
				//判断表达式是否为函数调用表达式
				if(5==s->get_ce()->get_stamp())
				{
					//如果是，则继续判断是否位于循环体内或者分支内
//					if(iteration_level>-1 || select_level>-1)
//					{
//						//如果位于循环或者分支内部，则抛出错误信息
//						string error_info="module function call can not appear in while or if structure!";
//						throw error_info;
//					}
					//对函数调用表达式进行编码
				//	if(s->get_cvar()->get_name())
						//gen_out<<s->get_cvar()->get_name()<<"=";
					gen_fun_call(s->get_ce(),v);
				}
				else
				{
					//如果不是，则抛出错误信息
					string error_info="expression statement is not a function call!";
					throw error_info;
				}
			}
			break;
			
			case 6:
			{
				//如果为分号语句，输出分号内容，如果为“；”，不输出
				if(";"!=s->get_semi())
				{
					out_indent(indent_num,gen_out);
					gen_out<<s->get_semi()<<endl;
				}
				else
				{
					//如果为分号，则不输出
				}
			}
			break;
			
			default:
			{
				//如果语句标签值为其他值，则非法，抛出错误信息
				string error_info="statement's stamp is invalid! ";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding statement : "+str;
		throw error_info;
	}
}

// 功能：遍历语句链表，对语句进行编码，当发现该条语句为CSCLABEL语句时，则说明进入循环语句的
//           的循环体
// 输入参数：s-语句链表指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_stmtlist(list<Statement>* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"statement list's pointer");
		check_pt_arg(v,"var_info list's pointer");
		
		//如果均不为空，则遍历语句链表
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//设置当前编码所位于的行号
			fake_line_num=iter->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//输出本条语句之前的注释信息

			gen_stmt(&(*iter),v);//对链该语句进行编码
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding statement list : "+str;
		throw error_info;
	}
}

// 功能：遍历语句，输出RSC中语句
// 输入参数：s-语句对象指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
void print_stmt(Statement* s,list<Var_info>* v)
{
	try
	{
		//判断语句类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//如果为复合语句，则调用复合语句编码函数
				gen_compd_stmt(s->get_ccstmt(),v);
			}
			break;

			case 1:
			{
				//如果为分支语句，则调用分支语句编码函数
				gen_select_stmt(s->get_csstmt(),v);
			}
			break;

			case 2:
			{
				//如果为循环语句，则调用循环语句编码函数
				gen_iterat_stmt(s->get_cistmt(),v);
			}
			break;

			case 3:
			{
				//如果为返回语句，则调用返回语句编码函数
				gen_return_stmt(s->get_crs());
			}
			break;

			case 4:
			{
				//如果为赋值语句，则先判断左值变量是否为普通变量
				if(s->get_cvar()->get_stamp()==0)
				{
					//如果是普通变量，则调用普通变量赋值编码函数
					gen_var_assign(s->get_cvar()->get_name(),s->get_ce(),v);
				}
				else
				{
					//如果是数组变量，则调用数组赋值编码函数
					gen_array_assign(s->get_cvar(),s->get_ce(),v);
				}
			}
			break;

			case 5:
			{
				//如果为表达式语句，则此表达式必须为函数调用表达式
				//判断表达式是否为函数调用表达式
				if(5==s->get_ce()->get_stamp())
				{
					//如果是，则继续判断是否位于循环体内或者分支内
					if(iteration_level>-1 || select_level>-1)
					{
						//如果位于循环或者分支内部，则抛出错误信息
						string error_info="module function call can not appear in while or if structure!";
						throw error_info;
					}
					//对函数调用表达式进行编码
					gen_fun_call(s->get_ce(),v);
				}
				else
				{
					//如果不是，则抛出错误信息
					string error_info="expression statement is not a function call!";
					throw error_info;
				}
			}
			break;

			case 6:
			{
				//如果为分号语句，输出分号内容，如果为“；”，不输出
				if(";"!=s->get_semi())
				{
					out_indent(indent_num,gen_out);
					gen_out<<s->get_semi()<<endl;
				}
				else
				{
					//如果为分号，则不输出
				}
			}
			break;

			default:
			{
				//如果语句标签值为其他值，则非法，抛出错误信息
				string error_info="statement's stamp is invalid! ";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="coding statement : "+str;
		throw error_info;
	}
}

// 功能：遍历语句链表，生成RSC语句，不对其进行编码，当发现该条语句为CSCLABEL语句时，则说明进入循环语句的
//           的循环体
// 输入参数：s-语句链表指针
// 			      v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：奚凌然
void print_stmtlist(list<Statement>* s,list<Var_info>* v)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"statement list's pointer");
		check_pt_arg(v,"var_info list's pointer");

		//如果均不为空，则遍历语句链表
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//设置当前编码所位于的行号
			fake_line_num=iter->get_fake_line();
			out_cmnt(cmnts,fake_line_num,gen_out);//输出本条语句之前的注释信息

			//判断当前语句是否为循环语句前面的CSCLABEL语句
			if(iter->get_stamp()==6 &&  iter->get_semi().substr(0,1)=="C")
			{
				//如果是，则认为循环结构已经开始，设置循环层数全局变量
				iteration_level++;//循环层数加1
				//取出循环结构中的左值变量和右值变量
				list<Statement>::iterator iter1=iter;
				iter1++;
				//取出CSCLABEL语句至while语句之间的左值变量和右值变量
				while(2!=iter1->get_stamp())
				{
					get_var_names(&(*iter1),adj_name_list);
					iter1++;
				}
				//取出while语句中的左值变量和右值变量
				get_var_names(&(*iter1),adj_name_list);
			}
			else
			{
				//如果不是，则忽略
			}
			print_stmt(&(*iter),v);//对链该语句进行编码
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print stmtlist error : "+str;
		throw error_info;
	}
}
