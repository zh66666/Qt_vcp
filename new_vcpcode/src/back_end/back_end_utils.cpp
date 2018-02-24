#include "expression.h"
#include "var.h"
#include "program.h"
#include <fstream>

// 存储在拆分当前结构时用掉的一般临时变量的最大值
int tempvar_num_max=0;

// 存储在拆分当前结构时用掉的条件临时变量的最大值
int tempcondvar_num_max=0;

//记录while语句的goto语句标号，每遇到一个while结构，就加1
int goto_label=0;									
// 定义在文件 : src/main_utils.c
extern int str2int(string str_num);
extern void check_pt_arg(void *pt,string info);

// 定义在文件 : ./ transformer/trans_struct.cpp
extern void trans_struct(Program *s);

// 定义在文件 : ./generator/gen_pro.cpp
extern void gen_pro(Program *s); 


// 功能：分析表达式对象，计算它的值，若能计算则返回“right”
//           否则，返回“wrong”
// 输入参数：s-表达式对象指针
// 输出参数：result-计算结果指针
// 返回值：函数运行成功则返回string，失败则抛出错误信息
// 作者：李刚
string compute_exp(Expression* s,int* result)
{
	try
	{
		//检查参数：表达式对象指针的有效性
		check_pt_arg(s,"expression object's pointer");
		
		//匹配表达式对象的标签
		switch(s->get_stamp())
		{
			case 0:
			{
				// 带有两个操作数的表达式
				int left=0;	//存储左表达式计算的值
				string str_l=compute_exp(s->get_cel(),&left);	//计算左表达式
				
				int right=0;	//存储右表达式计算的值
				string str_r=compute_exp(s->get_cer(),&right);	//计算右表达式
				
				//判断左表达式和右表达式能否计算出来
				if(str_l=="right" && str_r=="right")
				{
					//如果左右表达式均能计算出，则计算该表达式的值
					//匹配操作符
					if(s->get_op()=="+")
					{
						//如果是加号，则进行加法，并返回计算结果
						*result=left+right;
						return "right";
					}
					else if(s->get_op()=="-")
					{
						//如果是减号，则进行减法，并返回计算结果
						*result=left-right;
						return "right";
					}
					else if(s->get_op()=="*")
					{
						//如果是乘号，则进行乘法，并返回计算结果
						*result=left*right;
						return "right";
					}
					else if(s->get_op()=="/")
					{
						//如果是除号，则进行除法，并返回计算结果
						*result=left/right;
						return "right";
					}
					else if(s->get_op()=="<<")
					{
						//如果是左移符号，则进行左移，并返回计算结果
						*result=left<<right;
						return "right";
					}
					else if(s->get_op()==">>")
					{
						//如果是右移符号，则进行右移，并返回计算结果
						*result=left>>right;
						return "right";
					}
					else if(s->get_op()=="&&")
					{
						//如果是与运算符号，则进行与运算，并返回计算结果
						*result=left&&right;
						return "right";
					}
					else if(s->get_op()=="||")
					{
						//如果是或运算符号，则进行或运算，并返回计算结果
						*result=left||right;
						return "right";
					}
					else
					{
						//出现未知的操作符，抛出错误信息
						string error_info="operation is invalid!";
						throw error_info;
					}
				}
				else
				{
					//左右表达式只要有一个计算失败，则表明该表达式也无法计算
					return "wrong";
				}
			}
			break;
			
			case 1:
			{
				//只含有一个操作数的表达式
				int right=0;	//存储右表达式计算的值
				string str_r=compute_exp(s->get_cer(),&right);	//计算右表达式
				
				//判断右表达式是否能计算出来
				if(str_r=="right")
				{
					//匹配操作符是否是取非运算
					if(s->get_op()=="!")
					{
						//如果是取非运算符，则执行取非运算，并返回计算结果
						*result=!right;
						return "right";
					}
					else
					{
						//未知操作符，抛出错误信息
						string error_info="operation is invalid!";
						throw error_info;
					}
				}
				else
				{
					//右表达式计算失败，则表明该表达式也无法计算
					return "wrong";
				}
			}
			break;
			
			case 2:
			{
				//括号表达式
				int tmp=0;		//存储括号里面表达式计算的值
				string str_r=compute_exp(s->get_exp(),&tmp);		//计算括号里面的表达式
				
				//判断括号里面的表达式是否计算成功
				if(str_r=="right")
				{
					//成功，则将其值作为本表达式的值
					*result=tmp;
					return "right";
				}
				else
				{
					//不成功，则说明本表达式也不可计算
					return "wrong";
				}
			}
			break;
			
			case 3:
			{
				//常数表达式，返回常数的值作为本表达式的值
				*result=str2int(s->get_c());
				return "right";
			}
			break;
			
			case 4:
			case 5:
			{
				//函数调用表达式和变量表达式不可被计算
				return "wrong";
			}
			break;
			
			default:
			{
				//表达式对象的标签非法，则抛出错误信息
				string error_info="expression's stamp is invalid!";
				throw error_info;
			}
		}
	}
	catch (string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="compute expression's value : "+str;
		throw error_info;
	}
}

//根据运算符计算表达式的值
int compute_exp(int left,int right,string op)
{
	try
	{
		int result=0;
		if(op=="+")
		{
			result=left+right;
		}
		else if(op=="-")
		{
			result=left-right;
		}
		else if(op=="*")
		{
			result=left*right;
		}
		else if(op=="/")
		{
			result=left/right;
		}
		else
		{
			//出现未知的操作符，抛出错误信息
			string error_info="operation is invalid!";
			throw error_info;
		}
		return result;
	}
	catch(string str)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="compute expression's value : "+str;
		throw error_info;
	}
}

// 功能：对前端处理所得到的中间结构进行转换，然后生成冗余代码
// 输入参数：s-中间结构指针的引用
// 输出参数：s-中间结构指针的引用
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void back_end(Program*& s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s,"program struct 's pointer");

		trans_struct(s);	//转换中间结构

		gen_pro(s);		//生成冗余代码
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="back end : "+str;
		throw error_info;
	}
}


