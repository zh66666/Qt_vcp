#include "struct_array.h"

extern string compute_exp(Expression* s,int* result);

//定义在文件 : ./struct_array_dec.c
extern string stamp;
extern list<Struct_info>* structs;
extern list<struct_var>* struct_vars;
extern list<Array_info>* arrays;

// 定义在文件 src/main_utils.cpp
extern int line_num;
extern string int2str(int);
extern void check_pt_arg(void *pt,string info);
extern void check_str_arg(string arg_name,string info);

static void scan_struct_array_var(Var* s);
static void scan_struct_array_exp(Expression* s);
void scan_struct_array_stmtlist(list<Statement>* s);

// 功能：将多维数组转换为一维数组，分析其下标表达式，计算转换为一维数组时的下标值，并返回此下标
//			表达式
// 输入参数：array_name-数组名称
// 			      l-下标表达式链表指针
// 返回值：成功则返回转换出来的一维数组的下标表达式指针，失败则抛出错误信息
// 作者：李刚
// 注意：本函数里使用了new为返回的表达式分配了内存，需要上层函数来释放
static Expression* array_high2low(string array_name,list<Expression>* l)
{
	try
	{
		//检查参数：数组名称字符串的有效性
		check_str_arg(array_name,"array's name");
		//检查参数：数组下标表达式链表指针的有效性
		check_pt_arg(l,"subscript expression list's pointer");

		Expression* exp=NULL;//存储返回表达式的指针，初始化为NULL
		list<Array_info>::iterator iter;//数组信息链表迭代器
		//遍历存储有所有数组信息的全局链表
		for(iter=arrays->begin();iter!=arrays->end();iter++)
		{
			//判断当前迭代器所指的数组对象名是否与传入的数组名一致，并且数组对象的标签信息
			//是否与当前的全局标签一致
			if(array_name==iter->get_name() && stamp==iter->get_stamp())
			{
				//如果都一致，说明找到了该数组的信息
				vector<int> v=*(iter->get_subs());//取出数组的下标值
			
				//对传入的数组下标表达式进行转换，转换算法为：
				//例如数组声明时为int a[M][N][L];使用时的数组为a[o][p][q].
				//则转换后的一维数组的下标为o*N*L+p*N+L，为多项式的和
				
				int k=0;//记录遍历的数组下标表达式的个数
				int size=v.size();//记录数组的维数
				string temp_stamp=stamp;//保存全局标签信息
				stamp="common";//将全局标签信息置为"common"
				
				//遍历数组下标表达式链表
				for(list<Expression>::iterator iter1=l->begin();iter1!=l->end();iter1++,k++)
				{
					scan_struct_array_exp(&(*iter1));//转换当前迭代器所指的下标表达式
					
					//计算尚未遍历的下标值的乘积temp
					int temp=1;
					for(int i=k+1;i<size;i++)
					{
						temp*=v[i];
					}
					
					Expression* exp2=NULL;//用于存储构造一维下标表达式的的临时多项式
					if(1==temp)
					{
						//如果算出的temp值为1，则直接将转换后的下标表达式作为多项式
						exp2=new Expression(*iter1);
					}
					else
					{
						//如果temp值不为1，则将转换后的下标表达式作为右操作数，temp值作为左操作数构造
						//乘法表达式，将此表达式作为多项式
						string str=int2str(temp);
						exp2=new Expression(new Expression(str),"*",&(*iter1));
					}
					//判断当前是否在处理第一个下标表达式
					if(k==0)
					{
						//如果是，则将当前生成的多项式拷贝给最终生成的一维数组下标表达式
						exp=new Expression(*exp2);
						delete exp2;
					}
					else
					{
						//如果不是，则将已经生成的下标表达式作为左操作数，当前生成的多项式作为右操作数构造
						//加法表达式，并将此表达式拷贝给最终的下标表达式，参与下一个下标表达式的遍历
						Expression* exp4=new Expression(new Expression(*exp),"+",exp2);
						delete exp;
						exp=new Expression(*exp4);
						delete exp4;
					}
				}
				stamp=temp_stamp;//恢复全局标签信息	
				
				break;
			}
			else
			{
				//如果不一致，则继续遍历
			}
		}
		//判断迭代器是否到了链表的末端
		if(arrays->end()==iter)
		{
			//如果是，则说名没有找到该数组，抛出错误信息
			string error_info="array "+array_name+" has not been declared!";
			throw error_info;
		}
		else
		{
			//如果不是，则忽略
		}
		return exp;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan expression object : "+str;
		throw error_info;
	}
}

// 功能：查找结构体成员信息，根据传入的结构体变量名，成员名等信息，查找该成员在结构体中的相对位置
//			以及该成员的类型名
// 输入参数：struct_name-结构体名称
// 			      mem_name-成员名称
//			      stamp-标签信息
// 输出参数：loc-记录成员在结构体中位置的指针
// 返回值：成功则返回该成员的类型名称，失败则抛出错误信息
// 作者：李刚
static string find_struct_member(string struct_name,string mem_name,string stamp,int *loc)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(struct_name,"struct's name");
		check_str_arg(mem_name,"struct member's name");
		check_str_arg(stamp,"stamp information");
		
		string struct_type_name="";//存储将要返回的类型名称
		list<struct_var>::iterator iter;//结构体变量链表迭代器
		//遍历存储所有结构体变量的全局链表
		for(iter=struct_vars->begin();iter!=struct_vars->end();iter++)
		{
			//判断当前迭代器所指的结构体变量名是否同传入的结构体变量名一致
			//并且判断当前所指的结构体变量的标签信息是否与当前全局的标签信息一致
			if(iter->var_name_==struct_name && iter->stamp_==stamp)
			{
				//如果均一致，说明在结构体变量表中找到了该结构体变量
				list<mem_loc>::iterator iter1;//结构体成员链表迭代器
				//遍历结构体成员链表
				for(iter1=iter->struct_info_->get_members()->begin();
					iter1!=iter->struct_info_->get_members()->end();iter1++)
				{
					//判断当前迭代器所指的成员名称是否同要查找的成员名一致
					if(iter1->name==mem_name)
					{
						//如果一致，则取出该成员的相对位置，并跳出
						*loc=iter1->loc;
						struct_type_name=iter->struct_info_->get_struct_name();
						break;
					}
				}
				//判断迭代器是否指向了链表的末端
				if(iter->struct_info_->get_members()->end()==iter1)
				{
					//如果是，则说明没有找到该成员，则抛出错误信息
					string error_info="struct variable "+struct_name+" has no member named "+mem_name;
					throw error_info;
				}
				break;
			}
		}
		//判断外层迭代器是否指向了链表的末端
		if(iter==struct_vars->end())
		{
			//如果是，则说明该结构体变量未声明过，抛出错误信息
			string error_info="struct variable "+struct_name+" has not been declared! ";
			throw error_info;
		}
			
		return struct_type_name;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="find struct member's information : "+str;
		throw error_info;
	}
}

// 功能：查找结构体的大小，在对结构体数组进行转换时，根据数组名，标签信息，查找该结构体类型的大小
// 输入参数：array_name-数组的名称
//			      stamp-标签信息
// 返回值：成功则返回结构体的大小，失败则抛出错误信息
// 作者：李刚
static int find_struct_size(string array_name,string stamp)
{
	try
	{
		//检查输入参数的有效性
		check_str_arg(array_name,"struct array's name");
		check_str_arg(stamp,"stamp information");
		
		int num=0;//记录将要返回的结构体的大小
		list<struct_var>::iterator iter;
		//遍历全局的结构体变量链表
		for(iter=struct_vars->begin();iter!=struct_vars->end();iter++)
		{
			//如果当前迭代器所指的对象的名称与传入的数组名一致，并且标签信息一致，
			//说明找到了该结构体数组变量
			if(iter->var_name_==array_name && iter->stamp_==stamp)
			{
				//取出该结构体的大小
				num=iter->struct_info_->get_num();
				break;
			}
			else
			{
				//如果均不一致，则继续查找
			}
		}
		//遍历结束后，判断迭代器是否指向了链表末端
		if(iter==struct_vars->end())
		{
			//如果是，则说明该结构体变量未声明过，抛出错误信息
			string error_info="struct  variable"+array_name+" has not been declared! ";
			throw error_info;
		}
		return num;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="find struct 's size : "+str;
		throw error_info;
	}
}

// 功能：属于结构体变量转换的子模块，对结构体变量进行转换，将其传换为一维数组，通过参数将
//			转换所得的下标表达式输出
// 输入参数：s-结构体成员变量对象
// 输出参数：exp_sub-下标表达式对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_var_I(Var* s,Expression* exp_sub)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"struct member variable's pointer");
		check_pt_arg(exp_sub,"subscript expression's pointer");
		
		//匹配变量的类型
		switch(s->get_stamp())
		{
			case 0://成员为简单变量，忽略
			case 1://成员为一维数组，忽略
			break;
			case 2:
			{
				//如果为多维数组，则将其转换为一维数组类型
				string array_name=s->get_name();//取出数组名
				//将下标表达式换算成一维的下标表达式
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				//尝试对转换出的下标表达式进行计算
				int num=0;
				string result=compute_exp(exp_one,&num);
				//判断是否计算成功，并且计算的值为0
				if("right"==result && 0==num)
				{
					//如果是，则忽略
					;
				}
				else
				{
					//如果不能计算，或者计算所得的值不为0，则生成最终的一维数组下标表达式
					Expression* exp_1=new Expression(*exp_sub);
					exp_sub->set_stamp(0);
					exp_sub->set_op("+");
					exp_sub->set_cel(exp_1);
					exp_sub->set_cer(exp_one);
				}
			}
			break;
			case 3:
			{
				//如果为结构体变量，如struct_var.abc，则将其转换为一维数组变量
				string var_name=s->get_name();//取出结构体变量的名字
				int loc=0;
				//查找结构体成员变量在结构体中的相对位置
				string struct_type=find_struct_member(var_name,s->get_var()->get_name(),stamp,&loc);
				//判断相对位置是否为0
				if(0!=loc)
				{
					//如果不为0，则加上此偏移量
					Expression* exp_1=new Expression(*exp_sub);
					exp_sub->set_stamp(0);
					exp_sub->set_cel(exp_1);
					exp_sub->set_cer(new Expression(int2str(loc)));
					exp_sub->set_op("+");
				}
				else
				{
					//如果为0，则不加
				}
				string temp_stamp=stamp;
				stamp=struct_type;
				//继续分析下一层结构体变量
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;
			}
			break;
			case 4:
			{
				//如果为结构体数组变量，则先转换其下标链表，再转换其成员变量
				string array_name=s->get_name();//取出结构体数组名
				//查找结构体的大小
				int size=find_struct_size(array_name,stamp);
				//将数组的下标链表转换为一维下标
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				//查找下层结构体成员的位置
				int loc=0;
				string struct_type=find_struct_member(array_name,s->get_var()->get_name(),stamp,&loc);
				//计算本层结构体数组的下标表达式
				int sub=0;
				string result=compute_exp(exp_one,&sub);
				//判断计算是否成功
				if("right"==result )
				{
					
					int temp=size*sub+loc;
					if(0!=temp)
					{
						Expression* exp_1=new Expression(*exp_sub);
						exp_sub->set_stamp(0);
						exp_sub->set_cel(exp_1);
						exp_sub->set_cer(new Expression(int2str(temp)));
						exp_sub->set_op("+");
					}
				}
				else
				{
					if(0==loc)
					{
						Expression* exp_1=new Expression(*exp_sub);
						Expression* exp_2=new Expression(new Expression(int2str(size)),"*",exp_one);
						exp_sub->set_stamp(0);
						exp_sub->set_cel(exp_1);
						exp_sub->set_cer(exp_2);
						exp_sub->set_op("+");
					}
					else
					{
						Expression* exp_1=new Expression(*exp_sub);
						Expression* exp_2=new Expression(new Expression(int2str(size)),"*",exp_one);
						Expression* exp_3=new Expression(exp_1,"+",exp_2);
		
						exp_sub->set_stamp(0);
						exp_sub->set_cel(exp_3);
						exp_sub->set_op("+");
						exp_sub->set_cer(new Expression(int2str(loc)));
					}
					
				}
				
				string temp_stamp=stamp;
				stamp=struct_type;
				//继续遍历下层结构体成员
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;
			}
			break;
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="transform struct member : "+str;
		throw error_info;
	}
	
}

// 功能：遍历变量对象，根据变量的类型，采用不同的算法根据需要将其转换为一维数组
// 输入参数：s-变量对象指针
// 输出参数：s-变量对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_var(Var* s)
{
	try
	{
		//检查参数：变量对象指针的有效性
		check_pt_arg(s,"variable object's pointer");
		//匹配变量的类型
		switch(s->get_stamp())
		{
			case -1://空变量，不做处理
			case 0://简单变量，不做处理
			case 1: //一维数组，不作处理
			break;
			case 2:
			{
				//如果为多维数组，则将其转换为一维数组类型
				string array_name=s->get_name();//取出数组名
				//将下标表达式换算成一维的下标表达式
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				s->set_stamp(1);//修改变量的标签
				s->set_cexp(exp_one);//修改数组的下标表达式
			}
			break;
			case 3:
			{	
				string var_name=s->get_name();//取出结构体变量的名字
				int loc=0;
				//查找结构体成员变量在结构体中的相对位置
				string struct_type=find_struct_member(var_name,s->get_var()->get_name(),stamp,&loc);
				Expression* exp_sub=new Expression(int2str(loc));
				
				string temp_stamp=stamp;
				stamp=struct_type;
				//遍历成员
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;
				//将结构体变量转换为一维数组变量
				s->set_stamp(1);//修改变量的标签
				s->set_cexp(exp_sub);//修改下标表达式

			}
			break;
			case 4:
			{
				//如果为结构体数组变量，则先转换其下标链表，再转换其成员变量
				string array_name=s->get_name();//取出结构体数组名
				int size=0;
				Expression* exp_sub=NULL;
				list<struct_var>::iterator iter;
				for(iter=struct_vars->begin();iter!=struct_vars->end();iter++)
				{
					if(iter->var_name_==array_name&&iter->stamp_==stamp)
					{
						size=iter->struct_info_->get_num();
						break;
					}
				}
				if(iter==struct_vars->end())
				{
					string error_info="struct variable "+array_name+" has not been declared! ";
					throw error_info;
				}
				
				Expression* exp_one=array_high2low(array_name,s->get_exp_list());
				
				int loc=0;
				string struct_type=find_struct_member(array_name,s->get_var()->get_name(),stamp,&loc);
				
				int sub=0;
				string result=compute_exp(exp_one,&sub);
				if("right"==result )
				{
					int temp=size*sub+loc;
					exp_sub=new Expression(int2str(temp));
				}
				else
				{
					if(0==loc)
					{
						exp_sub=new Expression(new Expression(int2str(size)),"*",exp_one);
					}
					else
					{
						Expression* exp_1=new Expression(new Expression(int2str(size)),"*",exp_one);
						exp_sub=new Expression(exp_1,"+",new Expression(int2str(loc)));
					}
				}
				
				string temp_stamp=stamp;
				stamp=struct_type;
				//遍历结构体成员
				scan_struct_array_var_I(s->get_var(),exp_sub);
				stamp=temp_stamp;

				s->set_stamp(1);//修改此变量的标签
				s->set_cexp(exp_sub);//修改数组的下标表达式
			}
			break;
			default :
			{
				//变量标签为其他值，则抛出错误信息
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan variable  : "+str;
		throw error_info;
	}
}

// 功能：遍历表达式对象，根据表达式类型的不同进行不同的遍历，将需要转换的数组变量和结构体变量
//			转换为一维数组变量
// 输入参数：s-表达式对象指针
// 输出参数：s-表达式对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_exp(Expression* s)
{
	try
	{
		//检查参数：分支语句对象指针的有效性
		check_pt_arg(s,"expression object's pointer");
		//匹配表达式类型
		switch(s->get_stamp())
		{
			case -1://空表达式不处理
			case 3://常数表达式不处理
			break;
			
			case 0:
			{
				//如果为带两个操作数的表达式，则遍历左右表达式
				scan_struct_array_exp(s->get_cel());//遍历左表达式
				scan_struct_array_exp(s->get_cer());//遍历右表达式
			}
			break;
			case 1:
			{
				//如果为只含一个操作数的表达式，则遍历右表达式
				scan_struct_array_exp(s->get_cer());
			}
			break;
			case 2:
			{
				//如果为括号表达式，则遍历括号中的表达式
				scan_struct_array_exp(s->get_exp());
			}  
			break;
			case 4:
			{
				//如果为变量表达式，则遍历变量
				scan_struct_array_var(s->get_cvar());
			}	
			break;	
			case 5:
			{
				//如果为函数调用表达式，则遍历函数实参表达式链表
				for(list<Expression>::iterator iter=s->get_args()->begin();iter!=s->get_args()->end();iter++)
				{
					scan_struct_array_exp(&(*iter));//遍历当前迭代器所指的实参表达式
				}
			}
			break;
			default:
			{
				//如果标签为其他值，非法，抛出错误信息
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan expression object : "+str;
		throw error_info;
	}
}

// 功能：遍历语句，包括语句中的变量和表达式，将语句中用到的高维数组变量或者结构体变量转换为
//			一维数组变量
// 输入参数：s-语句对象指针
// 输出参数：s-语句对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void scan_struct_array_stmt(Statement* s)
{
	try
	{
		//检查参数：语句对象指针的有效性
		check_pt_arg(s,"statement object's pointer");
		//匹配语句的类型
		switch(s->get_stamp())
		{
			case 0:
			{
				//如果为复合语句，则遍历复合语句中的语句链表
				scan_struct_array_stmtlist(s->get_ccstmt()->get_csl());
			}
			break;
			case 1:
			{
				//如果为分支语句，则遍历分支语句
				scan_struct_array_exp(s->get_csstmt()->get_ce());//遍历分支入口表达式
				scan_struct_array_stmt(s->get_csstmt()->get_cs_if());//遍历if分支中的语句
				//判断该分支语句是否含有else分支
				if(1==s->get_csstmt()->get_stamp())
				{
					//如果含有else分支，则遍历else分支中的语句
					scan_struct_array_stmt(s->get_csstmt()->get_cs_else());
				}
				{
					//如果不含else分支，忽略
				}
			}
			break;
			case 2:
			{
				//如果为循环语句，则遍历循环语句
				scan_struct_array_exp(s->get_cistmt()->get_ce());//遍历循环语句的入口表达式
				scan_struct_array_stmt(s->get_cistmt()->get_cs());//遍历循环体中的语句
			}
			break;
			case 3:
			{
				//如果为返回语句，则遍历返回语句
				//判断该返回语句是否有返回值
				if(1==s->get_crs()->get_stamp())
				{
					//如果有，则遍历返回语句的返回表达式
					scan_struct_array_exp(s->get_crs()->get_ce());
				}
				else
				{
					//无返回值则忽略
				}
			}
			break;
			case 4:
			{
				//如果为赋值语句，则遍历左值变量和赋值表达式
				scan_struct_array_var(s->get_cvar());//遍历左值变量
				scan_struct_array_exp(s->get_ce());//遍历赋值表达式
			}
			break;
			case 5:
			{
				//如果为表达式语句，则遍历表达式
				scan_struct_array_exp(s->get_ce());
			}
			break;
			case -1://空语句忽略
			case 6://分号语句忽略
			break;
			default :
			{
				//语句标签为其他值，非法，抛出错误信息
				string error_info="stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan statement  : "+str;
		throw error_info;
	}
}

// 功能：遍历语句链表，对每条语句进行遍历，转换语句中用到的数组和结构体变量
// 输入参数：s-语句链表指针
// 输出参数：s-语句链表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void scan_struct_array_stmtlist(list<Statement>* s)
{
	try
	{
		//检查参数：语句链表指针的有效性
		check_pt_arg(s,"statement list's pointer");
		//遍历语句链表
		for(list<Statement>::iterator iter=s->begin();iter!=s->end();iter++)
		{
			line_num=iter->get_line_begin();//设置当前处理所位于的行号
			//对当前迭代器所指的语句对象进行遍历
			scan_struct_array_stmt(&(*iter));
		}	
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="scan statement list : "+str;
		throw error_info;
	}
}



