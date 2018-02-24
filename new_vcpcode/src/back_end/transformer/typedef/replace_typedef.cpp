#include "program_struct.h"


//定义在文件 : src/main_utils.cpp
extern int line_num;

//2010年3月10日追加
extern string file_info;

extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);

typedef list<Type_specifier>* TSLP;

//存储当前处理所在的作用域，如果在函数外，则是"global"，否则，其值为函数名称
static string scope="global";	

//用于存储用户自定义类型的相关信息，包括自定义类型的原始类型，自定义类型名称
//例如：有声明：typedef unsigned int UINT_32; 
//则原始类型为unsigned int，自定义类型名称为UINT_32
struct type_define
{
	TSLP type_spec_list_;				//原始类型
	string type_name_;					//自定义类型名
};

typedef list<type_define>* TDLP;

// 用于存储扫描到的用户自定义类型信息
static TDLP type_defines=new list<type_define>;

static void replace_typedef(TSLP t_s);
static void replace_typedef(Var_declaration* v_d);
static void replace_typedef(Fun_declaration* f_d);
void replace_typedef(Program *s);

// 功能：根据传入的自定义类型名，在自定义类型信息表中查找对应的原始类型
// 输入参数：s-存储有自定义类型信息的链表指针
//                  str_name-自定义类型名
// 返回值：成功则返回原始类型链表指针，失败则抛出错误信息
// 作者：李刚
static TSLP find_type_define(TDLP s,string str_name)
{
	try
	{
		//检查参数：自定义类型信息链表指针的有效性
		check_pt_arg(s,"typedef list's pointer");
		//检查参数：自定义类型名字符串的有效性
		check_str_arg(str_name,"user-defined name ");
		
		TSLP tmp=new list<Type_specifier>;							//存储查找到的原始类型链表指针
		list<type_define>::iterator iter;	//自定义类型信息表的迭代器
		for(iter=s->begin();iter!=s->end();iter++)
		{
			//判断当前迭代器所指元素的自定义类型名与传入的名称是否相等
			if(iter->type_name_==str_name)
			{
				//如果相等，说明查找成功
				for(list<Type_specifier>::iterator iter1=iter->type_spec_list_->begin();
				iter1!=iter->type_spec_list_->end();iter1++)
				{
					tmp->push_back(*iter1);
				}
	
				//tmp=iter->type_spec_list_;
				break;
			}
			else
			{
				//不相等，则继续查找
				;
			}
		}
		//循环结束之后，判断迭代器是否到了链表的末端
		if(s->end()==iter)
		{
			//如果到了末端，说明没有找到，抛出错误信息	
			string error_info="can't find user define name: "+str_name;
			throw error_info;
		}
		else
		{
			//没到末端，说明查找成功，忽略
			;
		}
		return tmp;	//返回找到的原始类型链表指针
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="finding user defined name : "+str;
		throw error_info;
	}
}

// 功能：遍历变量声明，存储并替换自定义类型，注意不能在局部有typedef声明
// 输入参数：v_d-变量声明对象指针
// 输出参数：v_d-变量声明对象指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void replace_typedef(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d,"variable declaration's pointer");
		
		//先遍历该变量的类型链表
		replace_typedef(v_d->get_decl_spec()->get_type_spec_list());
		//判断变量声明的标签是否为1，变量声明的存储类型是否为“typedef”
		if(1==v_d->get_stamp() && "typedef" == v_d->get_decl_spec()->get_stor_spec())
		{
			//是，则说明该声明为自定义类型声明
			//判断当前作用域是否是全局
			if("global"==scope)
			{
				//是全局，则存储自定义类型信息
				struct type_define td_tmp;		//该临时结构体用于存储自定义类型信息
				TSLP tslp_tmp=new list<Type_specifier>;	//该临时链表用于存储原始类型
				//将该声明中的原始类型插入临时链表
			tslp_tmp->insert(tslp_tmp->end(),v_d->get_decl_spec()->get_type_spec_list()->
											  begin(),v_d->get_decl_spec()->get_type_spec_list()->end());
											  
				//设置临时结构体的成员
				td_tmp.type_spec_list_=tslp_tmp;	
				td_tmp.type_name_=v_d->get_inits()->front().get_dec()->get_name();
				
				type_defines->push_back(td_tmp);	//将临时结构体变量插入全局链表中
				
				//delete tslp_tmp;
			}
			else
			{
				//不是全局，则抛出错误信息
				string error_info="local typedef is not allowed!";
				throw error_info;
			}	
		}
		else
		{
			//不是，说明该声明不是自定义类型声明，不作处理
			;
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable declaration : "+str;
		throw error_info;
	}
}

// 功能：遍历类型链表，将自定义的类型名替换成原始类型
// 输入参数：t_s-类型链表指针
// 输出参数：t_s-类型链表指针
// 返回值：成功返回void，失败则抛出错误信息
// 作者：李刚
static void replace_typedef(TSLP t_s)
{
	try
	{
		//检查参数：类型链表指针的有效性
		check_pt_arg(t_s,"type specifier list's pointer");
		
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();)
		{
			//匹配类型的标签值
			switch(iter->get_stamp())
			{
				case 1:
				{
					//标签为1，说明是自定义类型，则替换之
					string type_name=iter->get_type_name();		//存储自定义类型名
					//在全局自定义类型信息链表中，查找该自定义类型名对应的原始类型链表
					TSLP ts_tmp=find_type_define(type_defines,type_name);
					iter=t_s->erase(iter);//将当前的自定义类型名删除
					t_s->insert(iter,ts_tmp->begin(),ts_tmp->end());//在当前位置插入原始类型链表
				}
				break;
				case 2:
				{
					//标签为2，说明是结构体类型，对其进行遍历
					int i=iter->get_struct_spec()->get_stamp();	//存储结构体类型的标签
					//判断标签的值
					if(i==0 || i==1)
					{
						//标签值为0或者1，说明该结构体含有成员声明，遍历其成员声明
						for(list<Var_declaration>::iterator iter_in=iter->get_struct_spec()->
							  get_var_dec_list()->begin();iter_in!=iter->get_struct_spec()->
							  get_var_dec_list()->end();iter_in++)
						{
							line_num=iter_in->get_line_begin();//设置当前处理位于的行号
							replace_typedef(&(*iter_in));	//对结构体内的成员声明进行遍历
						}
					}
					else
					{
						//标签值为其他值，说明不含成员声明，不作操作
						;
					}
					iter++;
					
				}
				break;
				
				default:
				{
					//标签为其他值时，不作处理，迭代器加1
					iter++;
				}	
			}
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan type specifier list : "+str;
		throw error_info;
	}
}

// 功能：遍历函数声明，将自定义类型替换成原始类型。函数需要遍历的有函数的返回值类型，
//           函数的参数类型，函数体内的变量声明
// 输入参数：f_d-函数声明对象指针
// 输出参数：f_d-函数声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void replace_typedef(Fun_declaration* f_d)
{
	try
	{
		//检查参数：函数声明对象指针的有效性
		check_pt_arg(f_d,"function declaration object's pointer");
		
		//先遍历函数的返回值类型链表
		replace_typedef(f_d->get_decl_spec()->get_type_spec_list());
		
		//遍历函数的参数类型链表
		for(list<Param>::iterator iter=f_d->get_params()->begin();
		       iter!=f_d->get_params()->end();iter++)
		{
			replace_typedef(iter->get_type_spec_list());//遍历每个参数的类型链表
		}
		//判断函数声明的标签
		if(0==f_d->get_stamp())
		{ 
			//标签为0，说明该声明带有函数定义，遍历函数体
			scope=f_d->get_fun_name();//将作用域变量设置成该函数的名字
			
			//遍历函数体中的变量声明，替换自定义类型
			for(list<Declaration>::iterator iter=f_d->get_ccstmt()->get_cdl()->begin();
				   iter!=f_d->get_ccstmt()->get_cdl()->end();iter++)
			{
				line_num=iter->get_line_begin();	//修改当前处理所位于的行号
				//判断声明的标签
				if(0==iter->get_stamp())
				{
					//为0，说明是变量声明，遍历变量声明
					replace_typedef(iter->get_cvd());
				}
				else
				{
					//其他标签值，说明不是变量声明，忽略
					;
				}
			}
			scope="global";//函数体中的变量声明遍历完成之后，重置作用域变量为“global”
		}
		else
		{
			//标签不为0，说明该声明没有定义部分，忽略
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

// 功能：对中间结构中的自定义类型进行替换，将其替换成原始类型
// 输入参数：s-中间结构对象指针
// 输出参数：s-中间结构对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void replace_typedef(Program *s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s,"program struct 's pointer");
		
		type_defines->clear();	//将存储自定义类型信息的链表清空
		//遍历结构中的声明链表，替换自定义类型
		for(list<Declaration>::iterator iter=s->get_declist()->begin();
		       iter!=s->get_declist()->end();iter++)
		{
			line_num=iter->get_line_begin();	//修改当前处理所位于的行号
			//2010年3月10日追加
			file_info=iter->get_file_info();//修改当前处理所位于的文件名
			
			//判断是否为变量声明
			if(0==iter->get_stamp())
			{
				//为0，说明是变量声明，遍历变量声明
				replace_typedef(iter->get_cvd());
			}
			else
			{	
				//不为0，说明是函数声明，遍历函数声明
				replace_typedef(iter->get_cfd());
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="replace typedef  : "+str;
		throw error_info;
	}
}



