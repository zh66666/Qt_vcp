/*
 * gen_io.cpp
 *
 *  Created on: 2015年8月8日
 *      Author: halin
 */
#include "gen_utils.h"
#include "program.h"
#include "var_declaration.h"
#include "initializer.h"
#include "init_declarator.h"
#include "declarator.h"
#include "declaration.h"
#include "expression.h"
#include<string>
#include<map>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include<vector>
using namespace std;
extern void front_end(string, Program*&);
extern void trans_struct(Program *s);
extern void check_pt_arg(void *pt, string info);
extern ofstream extra_out;
extern const string ARRAY_EXTR_NAME;
extern string current_file_name;
extern string resolve_file_name(string& path_name);
extern int assign_var_sig(string name, string var_stamp);
extern int assign_extra_sig(string name, string var_stamp);
extern string compute_exp(Expression* s, int* result);
extern string compute_init_list(list<Initializer>* l, vector<int>* v_tmp);
extern int get_current_sig(list<Var_info>* s, string str, string scope);
extern long long K_NUM_P1;
extern long long K_NUM_P2;
extern const int P1;
extern const int P2;
extern list<Var_info>* var_infos;
extern string int2str(int num);
extern string var_scope;
//map管理IO文件中数组，<数组名，下标>如数组g_USR_BufRecvN[1024+1],对应map为<"g_USR_BufRecvN",1025>
map<string, int> ioArrayMap;
//存储数组名，初始化值
map<string, vector<string> > initMap;
//为数组分配签名、附加变量分配签名
void gen_array(string arrayName, int sub, vector<int>* init_values)
{
	string inits = "";
	ioArrayMap.insert(make_pair(arrayName, sub));
	assign_var_sig(arrayName, "array");	//为数组变量分配签名
	assign_extra_sig(ARRAY_EXTR_NAME, "common");	//为数组附加变量分配签名
	vector<string> val;
	unsigned int k = 0;
	for (vector<int>::iterator iter = init_values->begin();
			iter != init_values->end(); iter++)
	{
		int array_sig = get_current_sig(var_infos, arrayName, var_scope);
		int low1 = ((-((long long) (*iter) * K_NUM_P1) % P1
				+ ((long long) k * K_NUM_P1) % P1 + array_sig) % P1 + P1) % P1;
		int low2 = ((-((long long) (*iter) * K_NUM_P2) % P2
				+ ((long long) k * K_NUM_P2) % P2 + array_sig) % P2 + P2) % P2;
		k++;
		string temp = "";
		temp.append("{{");
		temp.append(int2str((*iter)));
		temp.append(", {");
		temp.append(int2str(low1));
		temp.append(", ");
		temp.append(int2str(low2));
		temp.append("}}, ");
		temp.append(int2str(array_sig));
		temp.append("}");
		val.push_back(temp);
	}
	initMap.insert(make_pair(arrayName, val));
}
void gen_io_array_dec(Var_declaration* v_d)
{
	try
	{
		//检查参数：变量声明对象指针的有效性
		check_pt_arg(v_d, "variable declaration object's pointer");
		for (list<Init_declarator>::iterator iter = v_d->get_inits()->begin();
				iter != v_d->get_inits()->end(); iter++)
		{
			//只处理数组变量
			if (iter->get_dec()->get_stamp() != 0)
			{
				string array_name = iter->get_dec()->get_name();	//取出数组变量名
				string subValue = array_name.substr(0, array_name.length() - 1);
				if (subValue == "g_USR_BufRecv" || subValue == "g_USR_BufSend"
						|| subValue == "g_USR_BufFetch"
						|| subValue == "g_USR_BufDeliver")
				{

				}
				else
				{
					string error_info = subValue + " not io array! ";
					throw error_info;
				}
				int sub = 0; //存储数组下标表达式的值
				//计算数组下表达式
				string sub_result = compute_exp(
						&(iter->get_dec()->get_array_subs()->front()), &sub);
				//判断是否计算成功
				if (sub_result == "wrong")
				{
					//如果计算失败，则抛出错误信息
					string error_info = "array's subscript is not constant! ";
					throw error_info;
				}
				else
				{
					string init_result;
					vector<int>* init_value = new vector<int>;	//用于存储数组元素初始化的值
					if (0 == iter->get_stamp())
					{		//无初始化
						for (int i = 0; i < sub; i++)
						{
							init_value->push_back(0);
						}
						gen_array(array_name, sub, init_value);
						delete init_value;

					}
					else
					{		//带初始化
						init_result = compute_init_list(
								iter->get_init()->get_init_list(), init_value);
						if (init_result == "wrong")
						{
							//如果计算失败，则抛出错误信息
							string error_info = "initializing is not constant! ";
							throw error_info;
						}
						else
						{
							//如果均计算成功，则对带有初始化的数组变量声明进行编码
							gen_array(array_name, sub, init_value);
							delete init_value;
						}
					}

				}
			}
		}
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding io array variable declaration : " + str;
		throw error_info;
	}
}
void gen_io_declist(list<Declaration>* s)
{
	for (list<Declaration>::iterator iter = s->begin(); iter != s->end();
			iter++)
	{
		if (0 == iter->get_stamp())
		{
			//遍历变量声明
			gen_io_array_dec(iter->get_cvd());
		}
		else
		{
			//遍历函数声明
		}
	}
}

//遍历中间结构中的声明链表
void gen_io_pro(Program* pro)
{
	try
	{
		check_pt_arg(pro, "program struct's pointer");
		gen_io_declist(pro->get_declist());
	} catch (string str)
	{
		cerr << "error : when coding file IO_interface.c: generate io ," << str << endl;
		exit(-1);
	}
}

void back_end_io(Program*& s)
{
	try
	{
		//检查参数：中间结构对象指针的有效性
		check_pt_arg(s, "program struct 's pointer");
		trans_struct(s);	//转换中间结构
		gen_io_pro(s);		//编码
	} catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "back end io: " + str;
		throw error_info;
	}
}

//创建SrcFile文件
void create_SrcFile(string SrcFile, string HeaderFile)
{
	extra_out.open((char*) (SrcFile).c_str());	//打开文件
	//文件是否创建成功
	if (!extra_out.is_open())
	{
		//为true，说明文件创建失败，抛出错误信息
		string error_info = SrcFile + " creating failed!";
		throw error_info;
	}
	else
	{
		resolve_file_name(HeaderFile);
		//如果文件创建成功，则输出数组对应的定义
		map<string, int>::iterator iter;
		for (iter = ioArrayMap.begin(); iter != ioArrayMap.end(); iter++)
		{
			string valueN = iter->first;
			string subValue = valueN.substr(0, valueN.length() - 1);
			//string outName = outArray(valueN);
			string outName = "";
			if (subValue == "g_USR_BufRecv")
			{
				outName = "g_ITF_BufMsgRecv";
			}
			else if (subValue == "g_USR_BufFetch")
			{
				outName = "g_ITF_BufMsgFetch";
			}
			else if (subValue == "g_USR_BufSend")
			{
				outName = "g_ITF_BufMsgSend";
			}
			else if (subValue == "g_USR_BufDeliver")
			{
				outName = "g_ITF_BufMsgDeliver";
			}
			char last = valueN[valueN.length() - 1];
			int index = iter->second;
			extra_out << "T_CODE " << outName << last << "[" << index << "] = {";
			map<string, vector<string> >::iterator initIter;
			initIter = initMap.find(valueN);
			if (initIter != initMap.end())
			{
				vector<string> v = initMap[valueN];
				int i = 0;
				for (vector<string>::iterator it = v.begin(); it != v.end(); it++)
				{
					if (i < (v.size() - 1))
					{
						extra_out << *(it) << "," << endl;
					}
					else
					{
						extra_out << *(it) << "};" << endl;
					}
					i++;
				}	//for

			}	//if

		}
		extra_out << endl;
		extra_out.close();
	}
}
//创建HeaderFile文件
void create_HeaderFile(string HeaderFile)
{
	extra_out.open((char*) (HeaderFile).c_str());		//打开文件
	//文件是否创建成功
	if (!extra_out.is_open())
	{
		//为true，说明文件创建失败，抛出错误信息
		string error_info = HeaderFile + " creating failed!";
		throw error_info;
	}
	else
	{
		//如果文件创建成功，则输出数组对应声明
		map<string, int>::iterator iter;
		for (iter = ioArrayMap.begin(); iter != ioArrayMap.end(); iter++)
		{
			string valueN = iter->first;
			string subValue = valueN.substr(0, valueN.length() - 1);
			//string outName = outArray(valueN);
			string outName = "";
			if (subValue == "g_USR_BufRecv")
			{
				outName = "g_ITF_BufMsgRecv";
			}
			else if (subValue == "g_USR_BufFetch")
			{
				outName = "g_ITF_BufMsgFetch";
			}
			else if (subValue == "g_USR_BufSend")
			{
				outName = "g_ITF_BufMsgSend";
			}
			else if (subValue == "g_USR_BufDeliver")
			{
				outName = "g_ITF_BufMsgDeliver";
			}
			char last = valueN[valueN.length() - 1];
			int index = iter->second;
			extra_out << "extern T_CODE " << outName << last << "[" << index << "];" << endl;

		}
		extra_out << endl;
		extra_out.close();
	}

}

/*外部输入输出管理方案s
 *1 词语法扫描InputFile
 *2 对InputFile中数组分配签名和附加变量(附加变量只有一个)
 *3 将各个数组编码(应该只有初始化的操作，不涉及赋值和引用)输出到SrcFile文件
 *4 将各个数组对应的声明输出到HeaderFile文件中
 *5 InputFile SrcFile HeaderFile均为绝对路径
 * */
void gen_io(string InputFile, string SrcFile, string HeaderFile)
{

	FILE *pFile = fopen(InputFile.c_str(), "r");
	if (!pFile)
	{
		return;
	}
	else
	{
		fclose(pFile);
	}

	current_file_name = InputFile;
	Program *ioPro = NULL;
	front_end(InputFile, ioPro); //前端
	back_end_io(ioPro); //后端
	//输出
	create_HeaderFile(HeaderFile);
	create_SrcFile(SrcFile, HeaderFile);
}
