#include "struct_array.h"

//定义在文件：src/back_end/back_end_utils.cpp
extern string compute_exp(Expression* s,int* result);

// 定义在文件: src/main_utils.cpp
extern int line_num;

//2010年3月10日追加
extern string file_info;

extern int str2int(string);
extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);

// 定义在文件 ：./struct_array_stmt.cpp
extern void scan_struct_array_stmtlist(list<Statement>* s);

//记录作用域信息，默认为"global"，即全局，当对函数体进行遍历时，置为函数的名字
static string scope="global";
//记录结构体变量或者数组变量的声明时的标签信息，如果该变量是结构体内的成员，则将此标签设置为
//结构体类型的名称，如果不是结构体内的变量，则置为"common"
string stamp="common";
//记录对当前中间结构中的所有结构体类型信息的链表
list<Struct_info>* structs=new list<Struct_info>;
//记录当前中间结构中的所有结构体变量信息的链表，包括结构体中声明的结构体变量
list<struct_var>* struct_vars=new list<struct_var>;
//记录当前中间结构中的所有数组变量信息的链表，包括结构体中声明的数组变量
list<Array_info>* arrays=new list<Array_info>;

static void scan_struct_array_declist(list<Declaration>* d_s);
static void scan_struct_array_vardec(Var_declaration* v_d);

// 功能：在结构体信息链表中根据传入的结构体名称，查找其对应的结构体信息，如果找到，则返回"yes"
//			，并且将找到的信息拷贝一份，通过参数输出，否则返回"no"
// 输入参数：l-存储有结构体信息的链表指针
//			     struct_name-要查找的结构体类型名
// 输出参数：tmp-结构体信息对象指针
// 返回值：成功则返回查找的结果信息，失败则抛出错误信息
// 作者：李刚
// 注意：本函数如果返回"yse"，则使用new为返回的指针分配了内存，需要上层函数来释放，如果返回
//			"no"，则不需要释放
static string find_struct_info(list<Struct_info>* l,string struct_name,Struct_info*& tmp)
{
	try
	{
		//检查参数：结构体信息链表指针的有效性
		check_pt_arg(l,"struct  information list's pointer");
		//检查参数：结构体类型名称字符串的有效性
		check_str_arg(struct_name,"struct's name");
		
		string result="";//存储查找的结果信息
		list<Struct_info>::iterator iter;//结构体信息对象迭代器
		//遍历结构体信息链表
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//判断当前迭代器所指对象的名称是否与要查找的结构体名称一致
			if(struct_name==iter->get_struct_name())
			{
				//如果一致，则将其拷贝给传入的参数
				tmp=new Struct_info(*iter);
				result= "yes";//查找结果置为"yes"
				break;
			}
			else
			{
				//如果不一致，继续遍历
			}
		}
		//遍历结束后，判断迭代器是否指向了链表的末端
		if(iter==l->end())
		{
			//如果是，则将查结果置为"no"
			result= "no";
		}
		return result;
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="find struct information : "+str;
		throw error_info;
	}
}

// 功能：分析数组下标表达式链表，计算数组元素的个数，并将数组变量的相关信息存储起来
// 输入参数：exp_subs-下标表达式链表指针
// 			      array_name-数组变量名称
// 返回值：成功则返回计算出来的数组元素个数，失败则抛出错误信息
// 作者：李刚
static int compute_array_sub(list<Expression>* exp_subs,string array_name)
{
	try
	{
		//检查参数：表达式链表指针的有效性
		check_pt_arg(exp_subs,"expression list's pointer");
		//检查参数：数组变量名称字符串的有效性
		check_str_arg(array_name,"array varibale's name");
		int num=1;//记录数组的元素个数，初始化为1
		vector<int>* subs=new vector<int>;//记录数组的下标值
		//遍历数组的下标表达式链表
		for(list<Expression>::iterator iter=exp_subs->begin();iter!=exp_subs->end();iter++)
		{
			int constant=0;//存储对当前下标表达式进行计算的结果
			string str_result=compute_exp(&(*iter),&constant);//对当前迭代器所指的表达式进行计算
			//判断是否计算成功
			if("right"==str_result)
			{
				//如果计算成功，则将计算出来的下标值压栈，并计算数组元素个数
				subs->push_back(constant);
				num=num*constant;
			}
			else
			{
				//如果计算失败，说明该数组下标不为常数，抛出错误信息
				string error_info="array "+array_name+"'s subscript is not constant!";
				throw error_info;
			}
		}
		//在全局的数组变量信息表中查找该数组变量，看是否存在，如果存在，则不存储，否则将其信息插入
		//全局的数组变量信息链表中
		list<Array_info>::iterator iter;//数组信息链表迭代器
		//遍历存放有数组信息的全局链表
		for(iter=arrays->begin();iter!=arrays->end();iter++)
		{
			//判断当前迭代器所指的数组的名称和标签信息与传入的数组变量是否一致
			if(array_name==iter->get_name() && stamp==iter->get_stamp())
			{
				//如果一致，说明该数组变量存储过，跳出遍历
				break;
			}
			else
			{
				//如果不一致，则继续遍历
			}
		}
		//判断迭代器是否指向链表的末端
		if(arrays->end()==iter)
		{
			//如果是，则说明该数组变量之前没有存储过，则存储
			Array_info* new_array=new Array_info(array_name,subs);//构造数组信息对象
			new_array->set_scope(scope);//设置数组信息对象的作用域
			new_array->set_stamp(stamp);//设置数组信息对象的标签
			arrays->push_back(*new_array);//存储该数组信息对象
			delete new_array;
		}
		delete subs;
		return num;//返回该数组的元素个数
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="compute array's subscript : "+str;
		throw error_info;
	}
}

// 功能：将传入的结构体信息插入到结构体信息链表中,如果已存在，则不插入
// 输出参数：l-存储结构体信息的链表指针
// 输入参数：tmp-结构体信息对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
 static void store_struct(list<Struct_info>* l,Struct_info* tmp)
{
	try
	{
		//检查参数：结构体信息对象指针的有效性
		check_pt_arg(tmp,"struct  information object's pointer");
		//检查参数：结构体信息链表指针的有效性
		check_pt_arg(l,"struct  information list's pointer");
		
		list<Struct_info>::iterator iter;//结构体信息对象迭代器
		//遍历结构体信息链表
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//判断当前迭代器所指的对象名称是否与传入的结构体的名称一致
			if(iter->get_struct_name()==tmp->get_struct_name())
			{
				//如果一样，则说明该结构体信息已经存储过，跳出
				break;
			}
			else
			{
				//如果不一样，则继续遍历
			}
		}
		//循环结束后，判断迭代器是否指向了链表的末端
		if(iter==l->end())
		{
			//如果是，则说明传入的结构体信息没有存储过，将其插入链表
			l->push_back(*tmp);
		}
		else
		{
			//如果没有到末端，忽略
		}
		return ;
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="store struct information : "+str;
		throw error_info;
	}
}

// 功能：遍历结构体类型，分析其成员，将其信息存储起来，并返回
// 输入参数：s-结构体类型对象指针
// 返回值：成功则返回结构体信息对象指针，失败则抛出错误信息
// 作者：李刚
// 注意：本函数使用new为返回的指针分配了内存，需要上层函数来释放
static Struct_info* scan_struct_body(Struct_specifier* s)
{
	try
	{
		//检查参数：结构体类型对象指针的有效性
		check_pt_arg(s,"struct  specifier's pointer");
		
		Struct_info* s_info_temp=NULL;//用于存储结构体类型信息的指针，初始化为NULL
	
		//判断该结构体类型是否带有成员声明
		if(s->get_stamp()==0)
		{
			//如果带有成员声明，则分析成员
			string struct_name=s->get_name();//取出该结构体类型名称
			int mem_num=0;//记录结构体成员的相对位置，初始化位置为0			
			list<mem_loc>* mems=new list<mem_loc>;//该链表用于存储结构体内的成员信息
			
			string temp_stamp=stamp;//保存当前的全局标签信息
			stamp=struct_name;//将全局标签信息置为当前遍历的结构体类型名称
			
			//遍历结构体内的变量声明
			for(list<Var_declaration>::iterator iter=s->get_var_dec_list()->begin();
				  iter!=s->get_var_dec_list()->end();iter++)
			{
				line_num=iter->get_line_begin();//设置当前处理位于的行号
				//对当前迭代器所指的变量声明进行遍历，将其转换为简单的变量声明或者一维数组声明
				scan_struct_array_vardec(&(*iter));		
				
				//遍历被声明的变量链表
				for(list<Init_declarator>::iterator iter1=iter->get_inits()->begin();
					iter1!=iter->get_inits()->end();iter1++)
				{
					//判断当前迭代器所指的变量是否为简单变量
					if(iter1->get_dec()->get_stamp()==0)
					{
						//如果为简单变量，则记录该成员信息
						struct mem_loc temp;//用于记录成员信息的临时结构体
						temp.name=iter1->get_dec()->get_name();//记录该变量的名字
						temp.loc=mem_num;//记录该变量在结构体内的相对位置
						mems->push_back(temp);//将成员信息插入链表存储起来
						mem_num++;//相对位置加1
					}
					else
					{
						//如果不是简单变量，则为数组变量，记录数组成员的信息
						struct mem_loc temp;//用于记录成员信息的临时结构体
						temp.name=iter1->get_dec()->get_name();//记录该变量的名字
						temp.loc=mem_num;//记录该变量在结构体内的相对位置
						mems->push_back(temp);//将成员信息插入链表存储起来
						//取出数组变量的下标字符串
						string sub=iter1->get_dec()->get_array_subs()->front().get_c();
						//修改相对位置，增量为数组的下标值
						mem_num+=str2int(sub);
					}
				}
			}
			
			stamp=temp_stamp;//恢复全局的标签信息
			
			//构造结构体信息对象
			s_info_temp=new Struct_info(struct_name,mems,mem_num,"global");
			//将此对象存储到全局的结构体信息链表中
			store_struct(structs,s_info_temp);
			delete mems;
		}
		else
		{
			//如果结构体类型不带成员声明，说明该结构体肯定在前面定义过，并且存储过其相关信息
			string struct_name=s->get_name();//取出该结构体类型名称
			//根据结构体名称在结构体信息链表中查找该结构体信息
			string result=find_struct_info(structs,struct_name,s_info_temp);
			
			//判断有无找到该结构体的信息
			if("no"==result)
			{
				//如果找不到，则抛出错误信息
				string error_info="struct type "+struct_name+" has not been declared!";
				throw error_info;
			}
		}
		
		return s_info_temp;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan struct body : "+str;
		throw error_info;
	}
}

// 功能：遍历声明类型链表，检查链表中是否有结构体类型，如果有，则提取结构体信息并存储，同时返回
//			结构体信息对象指针。如果无结构体类型，则返回的指针为空，否则不为空。
// 输入参数：t_s-类型链表指针
// 返回值：成功则返回结构体信息对象指针，失败则抛出错误信息
// 作者：李刚
static Struct_info* scan_struct_array_tplist(list<Type_specifier>* t_s)
{
	try
	{
		//检查参数：类型链表指针的有效性
		check_pt_arg(t_s,"type spcifier list's pointer");
		Struct_info* si_temp=NULL;//存储结构体信息对象的指针，初始化为空
		//遍历类型链表
		for(list<Type_specifier>::iterator iter=t_s->begin();iter!=t_s->end();iter++)
		{
			//判断当前迭代器所指的类型是否为结构体类型
			if(2==iter->get_stamp())
			{
				//如果是，则遍历此结构体类型
				si_temp=scan_struct_body(iter->get_struct_spec());
				//将此结构体类型修改为int类型
				iter->set_stamp(0);//将结构体类型标签设置为0
				iter->set_type_name("int");//类型名称设置为"int"
			}
		}
		
		return si_temp;
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan type specifier list : "+str;
		throw error_info;
	}
}

// 功能：遍历变量声明，包括该声明的类型链表以及被声明的变量
// 输入参数：v_d-变量声明对象指针
// 输出参数：v_d-变量声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_vardec(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d,"variable declaration object's pointer");
		
		//遍历变量声明的类型链表，提取结构体类型信息
		Struct_info* si_temp=scan_struct_array_tplist(v_d->get_decl_spec()->get_type_spec_list());

		//判断该变量声明是否在声明变量，并且是否为自定义类型声明
		if(v_d->get_stamp()==1 && v_d->get_decl_spec()->get_stor_spec()!="typedef" )
		{
			//如果是在声明变量，并且不为自定义类型声明，则遍历其声明的变量链表
			for(list<Init_declarator>::iterator iter=v_d->get_inits()->begin();
				  iter!=v_d->get_inits()->end();iter++)
			{
				//判断当前迭代器所指的变量是否为数组变量
				if(iter->get_dec()->get_stamp()==1)
				{
					//如果为数组变量，则将该数组变量转换为一维数组
					//分析数组的下标链表，将数组元素个数计算出来
					int sub=compute_array_sub(iter->get_dec()->get_array_subs(),iter->get_dec()->get_name());
					//判断从类型链表中提取出的结构体信息对象指针是否为空
					if(NULL!=si_temp)
					{
						//如果不为空，则说明该变量为结构体类型变量，存储该结构体变量，并计算将该结构体变量
						//转换为一维数组后的下标值
						struct_var temp;//用于存储该结构体变量的信息
						temp.var_name_=iter->get_dec()->get_name();//存储变量名
						temp.struct_info_=si_temp;//存储类型信息
						temp.scope_=scope;//存储作用域
						temp.stamp_=stamp;//存储标签信息
						struct_vars->push_back(temp);//将该结构体变量的信息插入全局链表中
						sub=si_temp->get_num()*sub;//计算转换为一维数组后的下标值
					}
					else
					{
						//如果为空，则说明该变量为一般类型的变量，忽略
					}
					
					//使用计算出来的数组元素个数构造新的下标表达式
					Expression* tmp_exp=new Expression(int2str(sub));
					//清空原来的的下标链表
					iter->get_dec()->get_array_subs()->clear();
					//将新的下标表达式插入链表
					iter->get_dec()->get_array_subs()->push_back(*tmp_exp);
					delete tmp_exp;
				}
				else
				{
					//如果为普通变量，则根据其类型是否为结构体类型来对其转换，如果是结构体类型，则转换为
					//一维数组，数组下标即为结构体的大小，如果不为结构体类型，则不转换
					//判断从类型链表中提取出的结构体信息对象指针是否为空
					if(NULL!=si_temp)
					{
						//如果不为空，则说明为结构体变量，将其转换为一维数组
						//构造下标表达式链表
						string sub=int2str(si_temp->get_num());//取出结构体的大小
						Expression* tmp_exp=new Expression(sub);//构造数组下标表达式
						list<Expression>* sub_list=new list<Expression>;//存储下标表达式的链表
						sub_list->push_back(*tmp_exp);//插入新的下标表达式
						delete tmp_exp;
						//修改普通变量为数组变量
						iter->get_dec()->set_stamp(1);//修改标签
						iter->get_dec()->set_array_subs(sub_list);//修改下标表达式链表
						
						struct_var temp;//存储该结构体变量的信息
						temp.var_name_=iter->get_dec()->get_name();//存储变量名
						temp.struct_info_=si_temp;//存储类型信息
						temp.scope_=scope;//存储作用域
						temp.stamp_=stamp;//存储标签信息
						struct_vars->push_back(temp);//将该结构体变量的信息插入全局链表中	
					}
					else
					{
						//如果为空，则忽略
					}
				}
			}
		}
		else
		{
			//其他情况的变量声明，忽略
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable declaration : "+str;
		throw error_info;
	}
}

// 功能：遍历函数声明，判断该函数声明是否带有函数体，如果有，则遍历函数体内的声明和语句
// 输入参数：f_d-函数声明对象指针
// 输出参数：f_d-函数声明对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_fundec(Fun_declaration* f_d)
{
	try
	{
		//检查参数：函数声明对象指针的有效性
		check_pt_arg(f_d,"function declaration object's pointer");
		
		//判断函数声明是否带有函数体
		if(0==f_d->get_stamp())
		{
			//如果带有函数体，则对函数体内的语句和声明进行遍历
			scope=f_d->get_fun_name();//设置作用域为函数的名称
			scan_struct_array_declist(f_d->get_ccstmt()->get_cdl());
			scan_struct_array_stmtlist(f_d->get_ccstmt()->get_csl());
			
			//函数体遍历结束后，清除该函数中的局部结构体变量信息
			for(list<struct_var>::iterator iter=struct_vars->begin();iter!=struct_vars->end();)
			{
				if(iter->scope_==scope)
				{
					iter=struct_vars->erase(iter);
				}
				else
				{
					iter++;
				}
			}
			//清除该函数中的局部数组变量信息
			for(list<Array_info>::iterator iter=arrays->begin();iter!=arrays->end();)
			{
				if(iter->get_scope()==scope)
				{
					iter=arrays->erase(iter);
				}
				else
				{
					iter++;
				}
			}
			//函数体遍历结束之后，将作用域重置为"global"
			scope="global";
		}
		else
		{
			//如果不带函数体，则忽略
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan function's declaration : "+str;
		throw error_info;
	}
}

// 功能：遍历声明链表，对变量声明和函数声明进行遍历，转换数组和结构体变量
// 输入参数：d_s-声明链表指针
// 输出参数：d_s-声明链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_declist(list<Declaration>* d_s)
{
	try
	{
		//检查参数：声明链表指针的有效性
		check_pt_arg(d_s,"declaration list's pointer");
		
		//遍历声明链表
		for(list<Declaration>::iterator iter=d_s->begin();iter!=d_s->end();iter++)
		{
			line_num=iter->get_line_begin();//设置当前处理位于的行号
			
			//2010年3月10日追加
			file_info=iter->get_file_info();//设置当前处理所位于得文件名
			
			//判断声明是否为变量声明
			if(0==iter->get_stamp())
			{
				//如果为变量声明，则遍历变量声明
				scan_struct_array_vardec(iter->get_cvd());
			}
			else
			{
				//如果为函数声明，则遍历函数声明
				scan_struct_array_fundec(iter->get_cfd());
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan declaration list : "+str;
		throw error_info;
	}
}

// 功能：遍历中间结构，对数组和结构体声明、或者数组变量结构体变量的使用进行转换，将结构体变量转换为
// 			一维数组变量，将高维数组变量转换为一维数组变量
// 输入参数：s-中间结构指针
// 输出参数：s-中间结构指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void scan_struct_array_pro(Program *s)
{
	try
	{
		//检查参数：中间结构指针的有效性
		check_pt_arg(s,"program struct's pointer");
		
		structs->clear();//将存储结构体信息的链表清空
		struct_vars->clear();//将存储结构体变量信息的链表清空
		arrays->clear();//将存储所有数组信息的链表清空
		
		scan_struct_array_declist(s->get_declist());//遍历中间结构中的声明链表
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="transform struct and array : "+str;
		throw error_info;
	}
}





