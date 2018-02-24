#include "program_struct.h"

// 该结构体用于存储枚举成员的相关信息，包括其名字，其对应的整数值以及其声明的作用域
// 例如 : 在全局作用域有声明enum {a,b,c}; 那么用于存储枚举成员a的结构体成员为：名字为a，
// 值为0，作用域为“global”
struct enum_mem
{
	string name_;	//枚举成员的名字
	int value_;			//枚举成员对应的整数值
	string scope_;	//枚举成员的作用域
};

// 定义在文件： mian_utils.cpp
extern int line_num;
extern int str2int(string);
//2010年3月10日追加
extern string file_info;

extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);



// 定义在文件：src/back_end/back_end_utils.cpp
extern string compute_exp(Expression* s,int* result);

//定义在文件：./replace_enum.cpp
extern void replace_enumerator(Expression* s);

//存储当前处理所在的作用域，如果在函数外，则是"global"，否则，其值为函数名称
static string scope="global";	

//存储当前所处理的C文件中的所有枚举成员信息
list<enum_mem>* enum_mems=new list<enum_mem>;


static void scan_enum(Var_declaration* v_d);
static void scan_enum(list<Type_specifier>* t_s);
static void scan_enum(Struct_specifier* s);
static void scan_enum(Enum_specifier* s);
static void scan_enum(Fun_declaration* s);
void scan_enum(Program *s);

// 功能：在存储有枚举成员信息的链表中，根据名称，查找某枚举成员的整数值，
// 			将其作为参数返回，同时查找成功则返回“right”，否则返回“wrong”
// 输入参数：l-存储有所有枚举成员信息的链表指针
// 			      name-枚举成员的名字
// 输出参数：result-查找成功时，存储枚举成员对应的整数值
// 返回值：函数执行成功则返回string，失败则抛出错误信息
// 作者：李刚
string find_enumerator_value(list<enum_mem>* l,string name,int* result)
{
	try
	{
		//检查参数：枚举成员信息链表指针的有效性
		check_pt_arg(l,"enumerator information list 's pointer");
		//检查参数：枚举成员名称字符串的有效性
		check_str_arg(name,"enumerator 's name");
		
		list<enum_mem>::iterator iter;//枚举成员信息链表的迭代器
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//判断当前迭代器所指元素的名字与传入的枚举成员的名字是否一致
			if(iter->name_==name)
			{
				//如果相同，则取出其整数值，并返回"right"
				*result=iter->value_;
				return "right";
			}
			else
			{
				//如果不相同，则循环继续
				;
			}
		}
		//查找过程中没有跳出，说明没有找到该枚举成员对应的整数值，返回"wrong"
		return "wrong";
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="find enumerator : "+str;
		throw error_info;
	}
}


// 功能：将枚举成员信息存入链表。在存储时，会查找链表中是否已经有该枚举成员，
 // 			如果已有，则不保存，否则保存该枚举成员的相关信息
// 输入参数：l-用于存储所有枚举成员信息的链表指针
//                  e-含有某个枚举成员信息的结构体变量
// 输出参数：l-用于存储所有枚举成员信息的链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void store_enum(list<enum_mem>* l,struct enum_mem e)
{
	try
	{
		//检查参数：枚举成员信息链表指针的有效性
		check_pt_arg(l,"enumerator information list 's pointer");
		
		//先检查传入的枚举成员是否在链表中已经存在
		list<enum_mem>::iterator iter;
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//判断当前迭代器所指的元素的名字是否跟
			//参数中的枚举成员的名字一致
			if(iter->name_==e.name_ )
			{
				//一致，说明链表中已经存有该枚举成员的信息，跳出循环
				break;
			}
			else
			{
				//不一致，则循环继续
				;
			}
		}
		//链表查找之后，检查迭代器是否到了链表的末端
		if(iter==l->end())
		{
			//如果到了末端，说明链表中不存在该枚举成员的信息，则将其插入
			l->push_back(e);
		}
		else
		{
			//如果迭代器没有到末端，说明循环中途跳出，意味着链表中已有该枚举成员的信息
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="store enumerator : "+str;
		throw error_info;
	}
}

// 功能：遍历中间结构，扫描枚举类型，并存储枚举成员信息，同时将枚举类型改成int型
// 输入参数：s-中间结构指针
// 输出参数：s-中间结构指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
void scan_enum(Program *s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s,"program struct 's pointer");
		
		enum_mems->clear();	//将存储有枚举成员信息的全局链表清空
		
		//遍历中间结构中的声明链表
		for(list<Declaration>::iterator iter=s->get_declist()->begin();
		   iter!=s->get_declist()->end();iter++)
		{
			//2010年3月10日追加
			file_info=iter->get_file_info();//设置当前处理所位于得文件名
			
			//判断是否为变量声明
			if(0==iter->get_stamp())
			{
				//为0说明是变量声明
				line_num=iter->get_line_begin();	//修改当前处理所位于的行号
				scan_enum(iter->get_cvd());//遍历变量声明
			}
			else
			{
				//不为0，说明是函数声明
				line_num=iter->get_line_begin();	//修改当前处理所位于的行号
				scan_enum(iter->get_cfd());//遍历函数声明
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan enum : "+str;
		throw error_info;
	}
}

// 功能：遍历函数声明，扫描枚举类型
// 输入参数：v_d-变量声明对象指针
// 输出参数：v_d-变量声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_enum(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d,"variable declaration object 's pointer");
		//遍历变量声明的类型链表
		scan_enum(v_d->get_decl_spec()->get_type_spec_list());
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable declaration : "+str;
		throw error_info;
	}
}

// 功能：遍历类型链表，扫描枚举类型
// 输入参数：t_s-类型链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_enum(list<Type_specifier>* t_s)
{
	try
	{
		//检查参数：类型链表指针的有效性
		check_pt_arg(t_s,"type specifier list 's pointer");
		//遍历类型链表
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();iter++)
		{
			//判断各个类型的标签
			if(2==iter->get_stamp())
			{
				//标签为2，说明该类型为结构体类型
				scan_enum(iter->get_struct_spec());//遍历该结构体类型
			}
			else if(3==iter->get_stamp())
			{
				//标签为3，说明该类型为枚举类型
				scan_enum(iter->get_enum_spec());//遍历该枚举类型
				
				//枚举类型遍历结束之后，修改该枚举类型为int类型
				iter->set_stamp(0);
				iter->set_type_name("int");
			}
			else
			{
				//其他标签，不进行任何操作
				;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan type specifier list : "+str;
		throw error_info;
	}
}

// 功能：遍历枚举类型，扫描枚举成员
// 输入参数：s-枚举类型对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_enum(Enum_specifier* s)
{
	try
	{
		//检查参数：枚举类型对象指针的有效性
		check_pt_arg(s,"enumerator specifier object's pointer");
		
		//判断枚举类型是否带有枚举成员声明
		if(0==s->get_stamp() || 1==s->get_stamp())
		{
			//标签为0或者1，说明带有枚举成员声明，则遍历枚举成员
			int k=0;		//记录枚举成员对应的整数值
			
			//扫描枚举成员
			for(list<Enumerator>::iterator iter=s->get_enum_list()->begin();
			       iter!=s->get_enum_list()->end();iter++,k++)
			{
				//判断该枚举成员是否带有初始化
				if(0==iter->get_stamp())
				{
					//标签为0，说明该枚举成员不带初始化
					//例如 enum abc {a,b=3,c}中的成员a和c
					struct enum_mem e1;						//存储该枚举成员的信息
					e1.name_=iter->get_element();	//存储该枚举成员的名字
					e1.value_=k;										//存储该枚举成员对应的整数值
					e1.scope_=scope;							//存储该枚举成员的作用域
					
					store_enum(enum_mems,e1);		//将该枚举成员信息存入全局链表
				}
				else
				{
					//标签不为0，说明该枚举成员带有初始化，例如enum abc {a,b=3,c}中的成员b
					//先将初始化表达式替换成常量表达式
					//例如：enum abc {a,b=a+3,c};中的成员b的初始化表达式为a+3，需要将其替换成1+3												 
					replace_enumerator(iter->get_exp());	//替换初始化表达式
					int result=0;	//存储初始化表达式的值
					string str=compute_exp(iter->get_exp(),&result);//计算初始化表达式的值
					
					//判断初始化表达式计算是否成功
					if(str=="right")
					{
						//说明计算成功
						struct enum_mem e1;						//用于存储该枚举成员的信息
						e1.name_=iter->get_element();	//存储该枚举成员的名字
						e1.value_=result;								//存储该枚举成员对应的整数值
						e1.scope_=scope;							//存储该枚举成员的作用域
						
						store_enum(enum_mems,e1);		//将该枚举成员信息存入全局链表
						
						k=result;											//将k的值置为当前枚举成员的整数值
					}
					else
					{
						//如果计算不成功，则抛出错误信息
						string error_info="invalid enumerator!";
						throw error_info;
					}
				}
			}
		}
		else
		{
			//其他标签说明该枚举类型不带枚举成员声明，就不进行遍历
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan enumerator specifier : "+str;
		throw error_info;
	}
}

// 功能：遍历结构体类型，结构体中的枚举类型
// 输入参数：s-结构体类型对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_enum(Struct_specifier* s)
{
	try
	{
		//检查参数：结构体类型对象指针的有效性
		check_pt_arg(s,"struct specifier object's pointer");
		//判断该结构体类型是否含有结构体成员变量声明
		if(s->get_stamp()==0 || s->get_stamp()==1)
		{
			//如果标签为0或者1，说明该结构体类型含有成员变量声明
			//遍历结构体的成员变量声明
			for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
				iter!=s->get_var_dec_list()->end();iter++)
			{
				line_num=iter->get_line_begin();	//修改当前处理所位于的行号
				scan_enum(&(*iter));						//遍历结构体中的变量声明
			}
		}
		else
		{
			//如果标签为其他值，说明无成员变量声明
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan struct specifier : "+str;
		throw error_info;
	}
}

// 功能：遍历函数声明，扫描枚举类型，包括遍历函数的返回值类型链表，函数形参类型链表，
// 			函数带有定义的时候，还要遍历函数体中的变量声明
// 输入参数：s-函数声明对象指针
// 输出参数：s-函数声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_enum(Fun_declaration* s)
{
	try
	{
		//检查参数：函数声明对象指针的有效性
		check_pt_arg(s,"function declaration object's pointer");
		
		scan_enum(s->get_decl_spec()->get_type_spec_list());//遍历函数返回值类型链表
		
		//遍历函数的形参链表
		for(list<Param>::iterator iter=s->get_params()->begin();
			   iter!=s->get_params()->end();iter++)
		{
			scan_enum(iter->get_type_spec_list());//遍历函数形参类型链表
		}
		//判断该函数声明是否带有定义部分
		if(s->get_stamp()==0)
		{
			//如果其标签为0，说明带有定义部分
			scope=s->get_fun_name();//将作用域置为该函数的名字
			
			//遍历函数体中的声明
			for(list<Declaration>::iterator iter=s->get_ccstmt()->get_cdl()->begin();
		       iter!=s->get_ccstmt()->get_cdl()->end();iter++)
			{
				//判断是否为变量声明
				if(0==iter->get_stamp())
				{
					//为0说明是变量声明
					line_num=iter->get_line_begin();	//修改当前处理所位于的行号
					scan_enum(iter->get_cvd());//遍历变量声明
				}
				else
				{
					//非法标签值，抛出错误信息
					string error_info="in function's body ,declaration 's stamp is invalid!";
					throw error_info;
				}
			}
			
			scope="global";//函数体遍历结束后，将作用域重置为"global"
		}
		else
		{
			//如果其标签不为0，说明不带定义部分
			;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan function declaration : "+str;
		throw error_info;
	}
}







