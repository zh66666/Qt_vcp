#include "program_struct.h"

// 该结构体用于存储枚举成员的相关信息，包括其名字，其对应的整数值以及其声明的作用域
// 例如 : 在全局作用域有声明enum {a,b,c}; 那么用于存储枚举成员a的结构体成员为：名字为a，
// 值为0，作用域为“global”
struct enum_mem
{
	string name_;	//枚举成员的名字
	int value_;			//枚举成员对应的整数值
	string scope_;	//i枚举成员的作用域
};

//定义在文件：src/main_utils.cpp
extern string int2str(int);
extern int line_num;
extern void check_pt_arg(void *pt,string info);
//2010年3月10日追加
extern string file_info;

//定义在文件 : ./scan_enum.c
extern string find_enumerator_value(list<enum_mem>*,string,int*);
extern list<enum_mem>* enum_mems;

//存储当前处理所在的作用域，如果在函数外，则是"global"，否则，其值为函数名称
static string scope="global";	

static void replace_enumerator(Var_declaration* v_d);
static void replace_enumerator(list<Type_specifier>* t_s);
static void replace_enumerator(Init_declarator* i_d);
static void replace_enumerator(Initializer* i);
static void replace_enumerator(Var* v);
void replace_enumerator(Expression* s);
static void replace_enumerator(Fun_declaration* f_d);
static void replace_enumerator(Compound_stmt* c);
static void replace_enumerator(Statement* s);


// 功能：遍历中间结构，将所有的枚举成员替换为对应的整数值
// 输入参数：s-中间结构指针
// 输入参数：s-中间结构指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void replace_enumerator(Program *s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s,"program struct 's pointer");
		
		//遍历中间结构中的声明链表
		for(list<Declaration>::iterator iter=s->get_declist()->begin();
			   iter!=s->get_declist()->end();iter++)
		{
			line_num=iter->get_line_begin();//设置当前处理所位于的行号
			//2010年3月10日追加
			file_info=iter->get_file_info();//设置当前处理所位于得文件名
			
			//判断声明是否为变量申明
			if(0==iter->get_stamp())
			{
				//如果声明对象的标签为0，说明为变量声明
				replace_enumerator(iter->get_cvd());//遍历该变量声明
				
			}
			else
			{
				//如果声明对象的标签不为0，说明为函数声明
				replace_enumerator(iter->get_cfd());//遍历该函数声明
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="replace enumerator : "+str;
		throw error_info;
	}
}

// 功能：遍历变量声明，包括变量声明中的类型链表，被声明的变量链表
// 输入参数：v_d-变量声明对象指针
// 输出参数：v_d-变量声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d,"variable declaration object's pointer");
		
		//先遍历变量声明的类型链表
		replace_enumerator(v_d->get_decl_spec()->get_type_spec_list());
		
		//判断变量声明对象是否带有变量链表
		if(v_d->get_stamp()==1)
		{
			//如果标签为1，说明带有变量链表，遍历变量链表
			for(list<Init_declarator>::iterator iter=v_d->get_inits()->begin();
			iter!=v_d->get_inits()->end();iter++)
			{
				replace_enumerator(&(*iter));//遍历被声明的变量
			}
		}
		else
		{
			//如果标签为其他值，说明不含变量链表
			;
		}
		
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable declaration: "+str;
		throw error_info;
	}
}

// 功能：遍历类型链表，如果发现有结构体类型，则遍历结构体中的变量声明
// 输入参数：t_s-类型链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(list<Type_specifier>* t_s)
{
	try
	{
		//检查参数：类型链表指针的有效性
		check_pt_arg(t_s,"type specifier list's pointer");
		
		//遍历类型链表
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();iter++)
		{
			//判断当前类型是否为结构体类型，并且该结构体类型是否带有成员变量声明
			if(2==iter->get_stamp() 
			    &&(iter->get_struct_spec()->get_stamp()==0 
				|| iter->get_struct_spec()->get_stamp()==1))
			{
				//如果是结构体类型，并且含有成员声明，则遍历成员变量声明
				for(list<Var_declaration>::iterator iter1=iter->get_struct_spec()->
						 get_var_dec_list()->begin();iter1!=iter->get_struct_spec()->
						 get_var_dec_list()->end();iter1++)
				{
					line_num=iter1->get_line_begin();//设置当前处理所位于的行号
					replace_enumerator(&(*iter1));//替换变量声明中的枚举成员
				}
			}
			else
			{
				//如果不是结构体类型，或者是结构体类型但无成员声明，则忽略
				;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan type specifier list: "+str;
		throw error_info;
	}
}

// 功能：遍历变量声明中被声明的变量部分，如果该变量声明时，带有初始化，还需遍历初始化部分
// 输入参数：i_d-被声明的变量对象指针
// 输出参数：i_d-被声明的变量对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Init_declarator* i_d)
{
	try
	{
		//检查参数：被声明的变量对象指针的有效性
		check_pt_arg(i_d,"init_declarator object's pointer");
		
		//判断声明的变量是不是数组变量
		if(1==i_d->get_dec()->get_stamp())
		{
			//如果标签为1，说明声明的是数组变量，遍历数组的下标表达式
			for(list<Expression>::iterator iter=i_d->get_dec()->get_array_subs()->begin();
				iter!=i_d->get_dec()->get_array_subs()->end();iter++)
			{
				replace_enumerator(&(*iter));//替换表达式中的枚举成员
			}
		}
		else
		{
			//如果标签不为1，不做操作
			;
		}
		
		//判断声明的变量是否带有初始化
		if(i_d->get_stamp()==1)
		{
			//标签为1，说明带有初始化部分，遍历初始化部分
			replace_enumerator(i_d->get_init());
		}
		else
		{
			//标签不为1，说明不带初始化，不作操作
			;
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan init_declarator : "+str;
		throw error_info;
	}
}

// 功能：遍历变量声明中的初始化部分，分为一般变量的初始化和对数组、结构体变量的初始化
// 输入参数：i-初始化部分对象指针
// 输出参数：i-初始化部分对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Initializer* i)
{
	try
	{
		//检查参数：初始化部分对象指针的有效性
		check_pt_arg(i,"initializer object's pointer");
		
		//判断初始化部分是否为一般变量的初始化
		if(i->get_stamp()==0)
		{
			//如果标签为0，说明是对一般变量的初始化
			replace_enumerator(i->get_exp());//遍历用于初始化的表达式
		}
		else
		{
			//如果标签为不为0，说明是对数组变量或者结构体变量的初始化，遍历初始化链表
			for(list<Initializer>::iterator iter=i->get_init_list()->begin();
				iter!=i->get_init_list()->end();iter++)
			{
				replace_enumerator(&(*iter));//遍历初始化链表中的每个元素
			}
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan initializer : "+str;
		throw error_info;
	}
}

// 功能：遍历语句或者表达式中的变量，如果是枚举成员，则进行替换
// 输入参数：v-变量对象指针
// 输出参数：v-变量对象指针
// 返回值：成功则返回void ,失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Var* v)
{
	try
	{
		//检查参数：变量对象指针的有效性
		check_pt_arg(v,"variable object's pointer");
		
		//匹配变量对象的标签
		switch(v->get_stamp())
		{
			case 0:
			{
				//标签为0，说明是一般的变量，不处理
				;
			}
			break;
			
			case 1:
			{
				//标签为1，说明是一维数组变量，遍历其下标表达式
				replace_enumerator(v->get_cexp());
			}
			break;
			
			case 2:
			{
				//标签为2，说明是高维数组变量，遍历其下标表达式
				 for(list<Expression>::iterator iter=v->get_exp_list()->begin();
					iter!=v->get_exp_list()->end();iter++)
				 {
					replace_enumerator(&(*iter));
				 }
			}
			break;
			
			case 3:
			{
				//标签为3，说明是结构体变量，遍历结构体变量
				replace_enumerator(v->get_var());
			}
			break;
			
			case 4:
			{
				//标签为4，说明是多维结构体数组，遍历结构体变量和数组下标
				replace_enumerator(v->get_var());//遍历结构体变量
				
				for(list<Expression>::iterator iter=v->get_exp_list()->begin();
					iter!=v->get_exp_list()->end();iter++)
				 {
					replace_enumerator(&(*iter));//遍历数组下标表达式
				 }
			}
			break;
			
			default:
			{
				//标签为其他值，非法，抛出错误
				string error_info="variable object 's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable : "+str;
		throw error_info;
	}
}

// 功能：遍历表达式，替换表达式中的枚举成员
// 输入参数：s-表达式对象指针
// 输出参数：s-表达式对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void replace_enumerator(Expression* s)
{
	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(s,"expression object's pointer");
		//匹配表达式标签
		switch(s->get_stamp())
		{
			case 0:
			{
				//标签为0，说明是含有两个操作数的表达式
				replace_enumerator(s->get_cel());//遍历左操作数表达式
				replace_enumerator(s->get_cer());//遍历右操作数表达式
			}
			break;
				
			case 1:
			{
				//标签为1，说明是含有单操作数的表达式
				replace_enumerator(s->get_cer());//遍历右操作数表达式
			}
			break;
				
			case 2:
			{
				//标签为2，说明是括号表达式
				replace_enumerator(s->get_exp());//遍历括号中的表达式
			}
			break;
				
			case 3:
			{
				//标签为3，说明是常数表达式，不做处理
				;
			}
			break;
				
			case 4:
			{
				//标签为4，说明是变量表达式
				//判断该变量的类型
				if(s->get_cvar()->get_stamp()==0)
				{
					//变量标签为0，说明是一般的变量，查找该变量是不是枚举成员，是则替换成一个常数
					int tmp=0;	//存储该变量对应的整数值
					string str1="";//存储查找的结果
					//在全局的枚举成员信息表中查找该变量
               str1=find_enumerator_value(enum_mems,s->get_cvar()->get_name(),&tmp);
					//判断查找结果
					if(str1=="right")
					{
						//如果查找成功，则将此变量表达式修改成常数表达式
						s->set_stamp(3);//修改该表达式的标签
						s->set_c(int2str(tmp));//修改该表达式常数的内容
					}
					else
					{
						//如果查找不成功，说明该变量不是枚举成员，不用替换
						;
					}
				}
				else
				{
					//变量标签不为0，则调用遍历变量函数继续遍历
					replace_enumerator(s->get_cvar());
				}
			}
			break;
				
			case 5:
			{
				//标签为5，说明为函数调用表达式，遍历函数的参数表达式
				for(list<Expression>::iterator iter=s->get_args()->begin();
					iter!=s->get_args()->end();iter++)
				 {
					replace_enumerator(&(*iter));//遍历参数表达式
				 }
			}
			break;
			
			default:
			{
				//标签为其他值，非法，抛出错误
				string error_info="expression object's stamp is invalid!";
				throw error_info;
			}
		}	
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan expression : "+str;
		throw error_info;
	}
}

// 功能：遍历函数声明对象，如果函数声明带有函数体，遍历函数体中的声明，替换枚举成员
// 输入参数：f_d-函数声明对象指针
// 输出参数：f_d-函数声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Fun_declaration* f_d)
{
	try
	{
		//检查参数：函数声明对象指针的有效性
		check_pt_arg(f_d,"function declaration object's pointer");
		
		//判断函数声明是否带有定义
		if(f_d->get_stamp()==0)
		{
			//函数声明标签为0，说明带有定义部分
			scope=f_d->get_fun_name();//将作用域设置成函数名称
			replace_enumerator(f_d->get_ccstmt());//遍历函数的定义部分
			scope="global";//作用域重置为"global"
		}
		else
		{
			//函数声明标签不为0，说明不含定义部分，忽略
			;
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan function declaration : "+str;
		throw error_info;
	}
}

// 功能：遍历复合语句，包括复合语句中的变量声明和语句，替换枚举成员
// 输入参数：c-复合语句对象指针
// 输出参数：c-复合语句对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Compound_stmt* c)
{
	try
	{
		//检查参数：复合语句对象指针的有效性
		check_pt_arg(c,"compound statement object's pointer");
		
		//遍历复合语句中的声明链表
		for(list<Declaration>::iterator iter=c->get_cdl()->begin();
			   iter!=c->get_cdl()->end();iter++)
		{
			//判断声明对象是否为变量声明
			if(0==iter->get_stamp())
			{
				//如果声明对象的标签为0，说明为变量声明
				replace_enumerator(iter->get_cvd());//遍历该变量声明	
			}
			else
			{
				//标签不为0，说明不是变量声明，忽略
				;
			}
		}
		//遍历复合语句中的语句链表
		for(list<Statement>::iterator iter=c->get_csl()->begin();
		       iter!=c->get_csl()->end();iter++)
		{
			replace_enumerator(&(*iter));//遍历语句
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan compound statement : "+str;
		throw error_info;
	}
}

// 功能：遍历语句，替换不同语句中的枚举成员
// 输入参数：s-语句对象指针
// 输出参数：s-语句对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_enumerator(Statement* s)
{
	try
	{
		//检查参数：语句对象指针的有效性
		check_pt_arg(s,"statement object's pointer");
		
		//判断语句类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//标签为0，说明是复合语句，遍历复合语句
				replace_enumerator(s->get_ccstmt());
			}
			break;
			case 1:
			{
				//标签为1，说明是分支语句，遍历分支语句的各个部分
				//遍历分支语句的入口表达式
				replace_enumerator(s->get_csstmt()->get_ce());
				//判断分支语句是否为单分支
				if(s->get_csstmt()->get_stamp()==0)
				{
					//标签为0，说明只含有if分支
					replace_enumerator(s->get_csstmt()->get_cs_if());//遍历if分支中的语句
				}
				else
				{
					//标签不为0，说明含有if和else分支
					replace_enumerator(s->get_csstmt()->get_cs_if());//遍历if分支中的语句
					replace_enumerator(s->get_csstmt()->get_cs_else());//遍历else分支中的语句
				}
			}
			break;
			case 2:
			{
				//标签为2，说明是循环语句，遍历循环语句的各个部分
				replace_enumerator(s->get_cistmt()->get_ce());//遍历循环入口表达式
				replace_enumerator(s->get_cistmt()->get_cs());//遍历循环体内的语句
			}
			break;
			case 3:
			{
				//标签为3，说明是返回语句
				//判断返回语句是否有返回值
				if(s->get_crs()->get_stamp()==1)
				{
					//如果标签为1，说明有返回值，遍历返回的表达式
					replace_enumerator(s->get_crs()->get_ce());
				}
				else
				{
					//如果标签不为1，说明没有返回值，忽略
					;
				}
			}
			break;
			case 4:
			{
				//标签为4，说明是赋值语句，遍历赋值语句的左值变量和右表达式
				replace_enumerator(s->get_cvar());//遍历左值变量
				replace_enumerator(s->get_ce());//遍历右表达式
			}
			break;
			case 5:
			{
				//标签为5，说明是函数调用语句，遍历函数调用
				replace_enumerator(s->get_ce());
			}
			break;
			case 6:
			{
				//标签为6，说明是分号语句，忽略
				;
			}
			break;
			default:
			{
				//标签为其他值，非法，抛出错误
				string error_info="statement object's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan statement : "+str;
		throw error_info;
	}
}



