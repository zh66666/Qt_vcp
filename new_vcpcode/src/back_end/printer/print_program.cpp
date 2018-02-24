
#include "program_struct.h"

//定义在文件 : src/main_utils.c
extern int indent_num;
//2010年3月10日追加
extern string file_info;
extern string current_file_name ;
extern int line_num;

extern list<Cmnt>* cmnts;
extern void out_indent(int,ofstream&);	
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);
extern void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out);
extern string  int2str(int  num);

static ofstream print_out;		//输出文件流
static list<Cmnt>* temp_cmnts=new list<Cmnt>;//存储源程序中的注释

static void print_program(list<Declaration>* s);
static void print_program(Var_declaration* s);
static void print_program(Declaration_specifiers* s);
static void print_program(Type_specifier* s);
static void print_program(Struct_specifier* s);
static void print_program(Declarator* s);
static void print_program(Enum_specifier* s);
static void print_program(Enumerator* s);
static void print_program(Init_declarator* s);
static void print_program(Initializer* s);
static void print_program(Fun_declaration* s);
static void print_program(Param* s);
static void print_program(Compound_stmt* s);
static void print_program(list<Statement>* s);
static void print_program(Statement* s);
static void print_program(Expression* s);
static void print_program(Var* s);
static void print_program(Return_stmt* s);
static void print_program(Selection_stmt* s);
static void print_if_branch(Selection_stmt* s);
static void print_program(Iteration_stmt* s);

// 功能：遍历程序的中间结构，将程序打印到文件
// 输入参数：s-中间结构指针
//                  path_name-程序输出的文件名
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
void print_program(Program *s, string path_name)
{
	try
	{
		//检查参数：声明链表指针的有效性
		check_pt_arg(s,"program struct's pointer");
		//检查参数：输出文件名字符串的有效性
		check_str_arg(path_name,"file's name");
		
		//创建文件
		print_out.open(path_name.c_str());
		//判断文件是否创建成功
		if(!print_out.is_open())
		{
			//为true，说明文件创建失败，报错
			string error_info="out put file creating failed!";
			throw error_info;
		}
		else
		{
			//为false，文件创建成功，开始打印
			//先将源程序中的注释拷贝一份
			temp_cmnts->insert(temp_cmnts->end(),cmnts->begin(),cmnts->end());
			
			//2010年3月10日修改
			try
			{
				//打印声明列表
				print_program(s->get_declist());
			}
			catch(string s)
			{
				string error_info="on line "+int2str(line_num)+" of file "+file_info+": "+s;
				throw error_info;
			}
			
			temp_cmnts->clear();
		}
		print_out.close();
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，打印错误信息，并退出程序
		//2010年3月10日修改
		cerr<<"error : print file \""<<current_file_name<<"\", "<<str<<endl;
		exit(-1);
	}
}

// 功能：打印声明链表
// 输入参数：s-声明链表指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(list<Declaration>* s)
{
	try
	{
		//检查参数：声明链表指针的有效性
		check_pt_arg(s,"declaration list's pointer");
	
		//遍历声明链表
		for(list<Declaration>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			out_cmnt(temp_cmnts,iter->get_fake_line(),print_out);//输出此条声明之前的注释
			
			//2010年3月10日追加
			line_num=iter->get_line_begin();//设置当前要打印的声明的行号
			file_info=iter->get_file_info();//设置当前要打印的声明所属的文件名称
			
			//判断声明是否为变量声明
			if(0==iter->get_stamp())
			{
				//如果为变量声明，则打印变量声明
				print_program(iter->get_cvd());
				print_out<<endl;
			}
			else
			{
				//如果为函数声明，则打印函数声明
				print_program(iter->get_cfd());
				print_out<<endl;	
			}
		}
		
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print declaration list   : "+str;
		throw error_info;
	}
}

// 功能：打印变量声明
// 输入参数：s-变量声明对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Var_declaration* s)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(s,"variable declaration  object's pointer");
		//判断变量声明是否带有声明的变量
		if(0==s->get_stamp())
		{
			//如果不带变量，例如 struct abc {int a; int b;};则只打印类型说明符
			print_program(s->get_decl_spec());
			print_out<<";"<<endl;
		}
		else
		{
			//如果带有变量，例如 struct abc {int a; int b;} var;打印类型说明符和变量
			print_program(s->get_decl_spec());//打印类型说明符
			print_out<<" ";
			//遍历声明的变量链表
			for(list<Init_declarator>::iterator iter=s->get_inits()->begin();
				iter!=s->get_inits()->end();iter++)
			{
				//检查是否为最后一个变量
				if(iter==--s->get_inits()->end())
				{
					//如果为最后一个变量，打印该变量
					print_program(&(*iter));
				}
				else
				{
					//如果还没到最后一个变量，打印该变量后输出逗号
					print_program(&(*iter));
					print_out<<",";
				}
						
			}
			print_out<<";"<<endl;	
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print variable declaration   : "+str;
		throw error_info;
	}
}

// 功能：打印声明类型说明符
// 输入参数：s-声明类型说明符对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Declaration_specifiers* s)
{
	try
	{
		//检查参数：声明类型说明符对象指针的有效性
		check_pt_arg(s,"declaration specifier  object's pointer");
		
		//取出存储类型
		string stor_spec=(""!=s->get_stor_spec()) ? (s->get_stor_spec()+" ") : ("");
		out_indent(indent_num,print_out);
		print_out<<stor_spec;//输出存储类型
		
		//打印类型链表
		for(list<Type_specifier>::iterator iter=s->get_type_spec_list()->begin();
		iter!=s->get_type_spec_list()->end();iter++)
		{
			//打印类型，如 ：unsigned long long，多个类型之间用空格分开
			print_program(&(*iter));
			print_out<<" ";
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print declaration specifier   : "+str;
		throw error_info;
	}
}

// 功能：打印变量类型
// 输入参数：s-变量类型对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Type_specifier* s)
{
	try
	{
		//检查参数：变量类型对象指针的有效性
		check_pt_arg(s,"type specifier  object's pointer");
		//匹配类型对象的标签
		switch(s->get_stamp())
		{
			case 0:
			case 1:
			{
				//0和1说明是一般的类型和自定义类型，如int，直接打印类型名
				print_out<<s->get_type_name();
			}
			break;
			case 2:
			{	
				//为2说明是结构体类型，调用结构体类型打印函数
				print_program(s->get_struct_spec());
			}
			break;
			case 3:
			{
				// 为3说明是枚举类型，调用枚举类型打印函数
				print_program(s->get_enum_spec());
			}
			break;
			default:
			{
				//标签为其他值，非法，报错
				string error_info="stamp is invalid!";
				throw error_info;
			}		
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print type specifier   : "+str;
		throw error_info;
	}
}

// 功能：打印结构体类型
// 输入参数：s-结构体类型对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Struct_specifier* s)
{
	try
	{
		//检查参数：结构体类型对象指针的有效性
		check_pt_arg(s,"struct type specifier  object's pointer");
		//匹配结构体类型对象的标签
		switch(s->get_stamp())
		{
			case 0:
			{
				//为0，说明结构体含有类型名和结构体成员，例如：struct abc {int a；int b}
				print_out<<"struct "<<s->get_name()<<endl;	//打印结构体类型名称
				out_indent(indent_num,print_out);
				print_out<<"{"<<endl;
				indent_num++;
				//遍历结构体中的变量声明链表
				for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
					iter!=s->get_var_dec_list()->end();iter++)
				{
					//2010年3月10日追加
					line_num=iter->get_line_begin();//设置当前要打印的声明所在的行号
					
					print_program(&(*iter));//打印结构体中的成员声明
				}
				indent_num--;
				out_indent(indent_num,print_out);
				print_out<<"}";
			}
			break;
			case 1:
			{
				//为1.说明结构体不含类型名，但有成员声明，例如：struct {int a; int b}
				print_out<<"struct"<<endl;
				out_indent(indent_num,print_out);
				print_out<<"{"<<endl;
				indent_num++;
				//遍历结构体中的变量声明链表
				for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
					iter!=s->get_var_dec_list()->end();iter++)
				{
					//2010年3月10日追加
					line_num=iter->get_line_begin();//设置当前要打印的声明所在的行号
					
					print_program(&(*iter));	//打印结构体中的成员声明
				}
				indent_num--;
				out_indent(indent_num,print_out);
				print_out<<"}";
			}
			break;
			case 2:
			{
				//为2，说明结构体只含类型名，无成员声明，例如：struct abc
				print_out<<"struct "<<s->get_name();//打印结构体类型名称
			}
			break;
			default:
			{
				//标签为其他值，非法，抛出错误信息
				string error_info="stamp is invalid!";
				throw error_info;
			}		
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print struct type specifier   : "+str;
		throw error_info;
	}
}

// 功能：打印被声明的变量
// 输入参数：s-声明变量类型对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Declarator* s)
{
	try
	{
		//检查参数：声明变量对象指针的有效性
		check_pt_arg(s,"declaring variable object's pointer");
		//判断该变量是否为普通变量
		if(0==s->get_stamp())
		{
			//如果是普通变量，例如：int a 或者 struct abc a；则打印变量名
			print_out<<s->get_name();
		}
		else
		{
			//如果是数组变量，例如：int a[2]或者struct abc a[3][4];
			print_out<<s->get_name();	//打印数组名
			//遍历数组下标链表
			for(list<Expression>::iterator iter=s->get_array_subs()->begin();
				iter!=s->get_array_subs()->end();iter++)
			{	
				print_out<<"[";
				print_program(&(*iter));	//打印数组下标表达式
				print_out<<"]";
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print declaring variable  : "+str;
		throw error_info;
	}
}

// 功能：打印枚举类型
// 输入参数：s-枚举类型对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Enum_specifier* s)
{
	try
	{
		//检查参数：枚举类型对象指针的有效性
		check_pt_arg(s,"enum type specifier object's pointer");
		
		string enum_name="";	//存放枚举类型的名字
		//判断枚举类型是否含有名字
		if(1==s->get_stamp() || 2==s->get_stamp())
		{
			//如果有，则把名字赋给enum_name
			enum_name=s->get_name();
		}
		else
		{
			//没有，则enum_name赋空
			enum_name="";
		}
		
		//打印枚举类型的名字
		print_out<<"enum "<<s->get_name()<<endl;
		
		//判断枚举类型是否含有成员
		if(0==s->get_stamp() || 1==s->get_stamp())
		{
			//如果有，例如：enum abc {a,b,c}，则打印成员
			out_indent(indent_num,print_out);
			print_out<<"{"<<endl;
			indent_num++;
			//遍历枚举成员链表
			for(list<Enumerator>::iterator iter = s->get_enum_list()->begin(); 
				   iter !=s->get_enum_list()->end(); ++iter)
			{
				//判断是否到了最后一个枚举成员
				if(--s->get_enum_list()->end()==iter)
				{
					//是，则打印枚举成员，不输出逗号
					print_program(&(*iter));
					print_out<<endl;
				}
				else
				{
					//不是，则打印枚举成员且输出逗号
					print_program(&(*iter));
					print_out<<","<<endl;
				}
			}
			indent_num--;
			out_indent(indent_num,print_out);
			print_out<<"}";
		}
		else
		{
			//如果没有枚举成员，不操作
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print enum type specifier  : "+str;
		throw error_info;
	}
}

// 功能：打印枚举成员，分为带赋值的枚举成员和不带赋值的枚举成员
// 输入参数：s-枚举成员对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Enumerator* s)
{
	try
	{
		//检查参数：枚举成员对象指针的有效性
		check_pt_arg(s,"enumerator object's pointer");
		//判断该枚举成员是否带有赋值表达式
		if(0==s->get_stamp())
		{
			//如果不带有赋值，如：enum abc {a，b=3,c}中的成员a
			out_indent(indent_num,print_out);
			print_out<<s->get_element();	//输出该成员名
		}
		else
		{
			//如果带有赋值，如：enum abc {a,b=3,c}中的成员b
			out_indent(indent_num,print_out);
			print_out<<s->get_element()<<"=";	//输出该成员名
			print_program(s->get_exp());				//输出赋值表达式
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print enumerator : "+str;
		throw error_info;
	}
}

// 功能：打印声明中的Init_declarator
//           例如声明： int a； int b=3; a 和b=3均为Init_declarator
// 输入参数：s-Init_declarator对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Init_declarator* s)
{
	try
	{
		//检查参数：Init_declarator对象指针的有效性
		check_pt_arg(s,"Init_declarator object's pointer");
		
		//判断Init_declarator对象是否带有初始化
		if(0==s->get_stamp())
		{
			//如果不带初始化，例如：int a；直接打印声明的变量
			print_program(s->get_dec());
		}
		else
		{
			//如果带有初始化，例如：int a=3；
			print_program(s->get_dec());	//打印声明的变量
			print_out<<" = ";
			print_program(s->get_init());	//打印初始化部分
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print Init_declarator : "+str;
		throw error_info;
	}
}

// 功能：打印声明中的初始化部分
// 输入参数：s-初始化部分对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Initializer* s)
{
	try
	{
		//检查参数：声明初始化对象指针的有效性
		check_pt_arg(s,"Initializer object's pointer");
		
		//判断初始化对象是否是普通变量的初始化
		if(0==s->get_stamp())
		{
			//如果是普通变量的初始化，如int a=3;
			print_program(s->get_exp());	//打印初始化表达式
		}
		else
		{
			//如果是对数组变量或者结构体变量的初始化
			//如int a[3][2]={{1,2},{3,4},{5,6}}; struct abc a={4,5,6};
			print_out<<"{ ";
			for(list<Initializer>::iterator iter=s->get_init_list()->begin();
				iter!=s->get_init_list()->end();iter++)
			{
				//判断初始化值是否为大括号中的最后一个
				if(iter==--s->get_init_list()->end())
				{
					//如果是，则输出其内容，不输出逗号
					print_program(&(*iter));
				}
				else
				{
					//如果不是，则输出其内容和逗号
					print_program(&(*iter));
					print_out<<",";
				}
			}
			print_out<<"}";
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print Initializer : "+str;
		throw error_info;
	}
}

// 功能：打印函数声明
// 输入参数：s-函数声明对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Fun_declaration* s)
{
	try
	{
		//检查参数：函数声明对象指针的有效性
		check_pt_arg(s,"function declaration object's pointer");
		//先输出函数返回值，函数名和参数部分
		print_program(s->get_decl_spec());	//输出返回值类型
		print_out<<" "<<s->get_fun_name()<<"(";	//输出函数名
		//遍历形参链表
		for(list<Param>::iterator iter=s->get_params()->begin();
			   iter!=s->get_params()->end();iter++)
		{
			//判断是否是最后一个参数
			if(iter==--s->get_params()->end())
			{
				//是，则输出参数，不输出逗号
				print_program(&(*iter));
			}
			else
			{
				//不是，则输出参数同时还有逗号
				print_program(&(*iter));
				print_out<<",";
			}
		}
		print_out<<")"; 
		
		//判断函数声明是否带有函数定义
		if(0==s->get_stamp())
		{
			//如果带有函数定义部分，打印函数的复合语句
			print_out<<endl;
			out_indent(indent_num,print_out);
			print_program(s->get_ccstmt());	//打印函数定义部分
		}
		else
		{
			//如果不带函数定义部分，则输出分号
			print_out<<";"<<endl;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print function declaration : "+str;
		throw error_info;
	}
}

// 功能：打印函数形参
// 输入参数：s-函数形参对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Param* s)
{
	try
	{
		//检查参数：形参对象指针的有效性
		check_pt_arg(s,"parameter object's pointer");
		//打印形参的类型链表
		for(list<Type_specifier>::iterator iter=s->get_type_spec_list()->begin();
			   iter!=s->get_type_spec_list()->end();iter++)
		{
			print_program(&(*iter));
			print_out<<" ";
		}
		//判断形参是否声明完整
		if(0==s->get_stamp())
		{
			//如果形参声明完整，如：fun（int a），打印形参名
			print_program(s->get_dec());
		}
		else
		{
			//如果形参声明不完整，如：fun（int），不执行任何操作
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print parameter : "+str;
		throw error_info;
	}
}

// 功能：打印复合语句
// 输入参数：s-复合语句对象指针
// 返回值：成功返回void，失败则抛出错误
// 作者：李刚
static void print_program(Compound_stmt* s)
{	
	try
	{
		//检查参数：复合语句对象指针的有效性
		check_pt_arg(s,"compound statement object's pointer");
		
		out_indent(indent_num,print_out);
		print_out<<"{"<<endl;
		indent_num++;	//缩进加1
		
		//打印复合语句中的声明链表
		print_program(s->get_cdl());	
		//打印复合语句中的语句链表
		for(list<Statement>::iterator iter=s->get_csl()->begin();
		       iter!=s->get_csl()->end();iter++)
		{
			out_cmnt(temp_cmnts,iter->get_fake_line(),print_out);//输出本条语句之前的注释信息
			
			//2010年3月10日追加
			line_num=iter->get_line_begin();//设置当前打印的语句所位于的行号
			
			print_program(&(*iter));	//打印statement
		}	
		//可能该复合语句为空，将复合语句"}"前的注释输出
		out_cmnt(temp_cmnts,s->get_fake_line(),print_out);
		
		indent_num--;	//缩进减1
		out_indent(indent_num,print_out);
		print_out<<"}";
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print compound statement : "+str;
		throw error_info;
	}
}

// 功能：打印语句，不同的语句调用不同的打印函数
// 输入参数：s-语句对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Statement* s)
{
	try
	{
		//检查参数：语句对象指针的有效性
		check_pt_arg(s,"statement object's pointer");
		//匹配语句对象的标签
		switch(s->get_stamp())
		{
			case 0:
			{	
				//为0说明是复合语句，调用复合语句打印函数
				print_program(s->get_ccstmt());
			}
			break;
			case 1:
			{
				//为1说明是分支语句，调用分支语句打印函数
				print_program(s->get_csstmt());
			}
			break;
			case 2:
			{
				//为2说明是循环语句，调用循环语句打印函数
				print_program(s->get_cistmt());
			}
			break;
			case 3:
			{
				//为3说明是返回语句，调用返回语句打印函数
				print_program(s->get_crs());
			}
			break;
			case 4:
			{
				//为4说明是赋值语句，打印左值和赋值表达式
				out_indent(indent_num,print_out);
				print_program(s->get_cvar());	//打印左值变量
				print_out<<" = ";
				print_program(s->get_ce());		//打印赋值表达式
				print_out<<";"<<endl;
			}
			break;
			case 5:
			{
				//为5说明是函数调用，打印函数调用
				out_indent(indent_num,print_out);
				print_program(s->get_ce());
				print_out<<";"<<endl;
			}
			break;
			case 6:
			{
				//为6说明是分号语句，打印分号
				out_indent(indent_num,print_out);
				print_out<<s->get_semi()<<endl;
			}
			break;
			default:
			{
				//标签为其他值，非法，报错
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print statement : "+str;
		throw error_info;
	}
}

// 功能：打印表达式，根据表达式类型的不同，调用不同的打印函数
// 输入参数：s-表达式对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Expression* s)
{
	try
	{
		//检查参数：变量对象指针的有效性
		check_pt_arg(s,"expression object's pointer");
		
		//匹配表达式的标签
		switch(s->get_stamp())
		{
			case 0:
			{
				//为0说明表达式有两个操作数，如：a+b
				print_program(s->get_cel());	//打印左表达式
				print_out<<s->get_op();			//打印操作符
				print_program(s->get_cer());	//打印右表达式
			}
			break;
			case 1:
			{
				//为1，说明只有一个操作数，如！a
				print_out<<s->get_op();	//打印操作符
				print_program(s->get_cer());	//打印右表达式
			}
			break;
			case 2:
			{
				//为2，说明为括号表达式，如(a+b)
				print_out<<"(";
				print_program(s->get_exp());	//打印括号中的表达式
				print_out<<")";
			}  
			break;
			case 3:	
			{
				//为3，说明为常数表达式，如3
				print_out<<s->get_c();	//打印该常数
			}
			break;
			case 4:
			{
				//为4，说明为变量表达式，如x ,b[3]
				print_program(s->get_cvar());	//调用变量打印函数
			}
			break;	
			case 5:
			{
				//为5，说明为函数调用表达式
				print_out<<s->get_c()<<"(";	//打印函数名
				for(list<Expression>::iterator iter=s->get_args()->begin();
					iter!=s->get_args()->end();++iter)
				{
					//判断是否为最后一个实参
					if(iter==--s->get_args()->end())
					{
						//是，则打印实参表达式，不输出逗号
						print_program(&(*iter));
					}
					else
					{
						//不是，则打印实参表达式和逗号
						print_program(&(*iter));
						print_out<<",";
					}
				}	
				print_out<<")";
			}
			break;
			default:
			{
				//标签为其他值，非法，报错
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print expression : "+str;
		throw error_info;
	}
}

// 功能：打印变量
// 输入参数：s-变量对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Var* s)
{
	try
	{
		//检查参数：变量对象指针的有效性
		check_pt_arg(s,"variable object's pointer");

		//匹配变量对象的标签
		switch(s->get_stamp())
		{
			case 0:
			{
				//为0，说明是普通变量，如 a，打印其名称
				print_out<<s->get_name();
			}
			break;
			case 1:
			{
				//为1，说明是一维数组变量，如a[x+y]
				print_out<<s->get_name()<<"[";	//打印数组名
				print_program(s->get_cexp());		//打印下标表达式
				print_out<<"]";
			}
			break;
			case 2:
			{
				//为2，说明是多维数组变量，如a[3][b][d+e]
				print_out<<s->get_name();	//打印数组名
				for(list<Expression>::iterator iter=s->get_exp_list()->begin();
				       iter!=s->get_exp_list()->end();iter++)
				{
					print_out<<"[";
					print_program(&(*iter));	//打印下标表达式
					print_out<<"]";
				}
			}
			break;
			case 3:
			{
				//为3，说明是结构体变量，如a.c
				print_out<<s->get_name()<<".";	//打印结构体名
				print_program(s->get_var());			//打印结构体成员变量
			}
			break;
			case 4:
			{
				//为4，说明是结构体数组变量，如a[3][4].c
				print_out<<s->get_name();			//打印结构体数组名
				for(list<Expression>::iterator iter=s->get_exp_list()->begin();
				iter!=s->get_exp_list()->end();iter++)
				{
					print_out<<"[";
					print_program(&(*iter));			//打印数组下标表达式
					print_out<<"]";
				}
				print_out<<".";									
				print_program(s->get_var());		//打印成员变量
			}
			break;
			default:
			{
				//标签为其他值，非法，报错
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print variable : "+str;
		throw error_info;
	}
}

// 功能：打印返回语句
// 输入参数：s-返回语句对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Return_stmt* s)
{
	try
	{
		//检查参数：返回语句对象指针的有效性
		check_pt_arg(s,"return statement object's pointer");
		
		//打印return 关键字
		out_indent(indent_num,print_out);
		print_out<<"return ";
		
		//判断返回语句是否带有返回值
		if(0==s->get_stamp())
		{
			//如果无返回值，打印分号
			print_out<<";"<<endl;
		}
		else
		{
			//如果带有返回值，则打印返回表达式
			print_program(s->get_ce());
            print_out<<";"<<endl;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print return statement : "+str;
		throw error_info;
	}
}

// 功能：打印分支语句
// 输入参数：s-分支语句对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Selection_stmt* s)
{
	try
	{
		//检查参数：分支语句对象指针的有效性
		check_pt_arg(s,"selection statement object's pointer");
		
		//判断分支语句是否只含有if分支
		if(0==s->get_stamp())
		{
			//如果只含有if分支，打印if分支中的语句
			print_if_branch(s);
		}
		else
		{
			//如果含有if和else分支，两个分支中的语句均打印
			print_if_branch(s);	//打印if分支中的语句
			out_indent(indent_num,print_out);
			//打印else分支，判断else分支中的语句是否是复合语句
			if(0!=s->get_cs_else()->get_stamp())
			{
				//不是复合语句，则继续判断是不是分支语句
				if(1==s->get_cs_else()->get_stamp())
				{
					//是分支语句，则在else后直接打印语句，且缩进不用加1
					//这一步是为了打印 else if这种形式的分支语句
					print_out<<"else";
					print_program(s->get_cs_else());
				}
				else
				{
					//不是分支语句，则在else关键字输出后，缩进加1
					//使else分支中的语句另起一行
					print_out<<"else"<<endl;
					indent_num++;
					print_program(s->get_cs_else());
					indent_num--;
				}
			}
			else
			{
				//是复合语句，则输出else关键字后，直接调用语句打印函数
				print_out<<"else"<<endl;
				print_program(s->get_cs_else());
			}
			print_out<<endl;
		}	
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print selection statement : "+str;
		throw error_info;
	}
}

// 功能：打印循环语句
// 输入参数：s-循环语句对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_program(Iteration_stmt* s)
{
	try
	{
		//检查参数：循环语句对象指针的有效性
		check_pt_arg(s,"iteration statement object's pointer");
		
		out_indent(indent_num,print_out);
		print_out<<"while(";
		print_program(s->get_ce());	//打印循环语句入口表达式
		print_out<<")"<<endl;
		
		//判断循环体中语句的类型是否是复合语句
		if(0!=s->get_cs()->get_stamp())
		{
			//如果不是，则缩进加1
			indent_num++;
			print_program(s->get_cs());	//打印循环体中的语句
			indent_num--;
		}
		else
		{
			//如果是，则缩进不用加1
			print_program(s->get_cs());	//打印循环体中的语句
		}
		print_out<<endl;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print iteration statement : "+str;
		throw error_info;
	}
}

// 功能：打印if分支中的语句
// 输入参数：s-分支语句对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void print_if_branch(Selection_stmt* s)
{
	try
	{
		//检查参数：分支语句对象指针的有效性
		check_pt_arg(s,"selection statement object's pointer");
		
		out_indent(indent_num,print_out);
		print_out<<"if(";
		print_program(s->get_ce());	//打印分支入口表达式
		print_out<<")"<<endl;

		//判断if分支中的语句是否是带有大括号的复合语句
		if(0!=s->get_cs_if()->get_stamp())
		{
			//不是复合语句，则要输出缩进，再打印语句
			indent_num++;
			print_program(s->get_cs_if());
			indent_num--;
		}
		else
		{
			//是复合语句，则直接调用语句打印函数
			print_program(s->get_cs_if());
		}
		print_out<<endl;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print if branch : "+str;
		throw error_info;
	}
}


