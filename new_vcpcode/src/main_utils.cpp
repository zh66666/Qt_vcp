#include "CodedVarBuild_Util.h"
#include "bool_var.h"
#include "var_info.h"
#include "cmnt.h"
#include "fun_info.h"
#include "global_var.h"
#include "TmpVarIndexRange.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_utils.h"
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
class Program;
//此结构用于保存编码工程中dD的名字和对应的值
struct dD_value
{
	string name;//记录dD的名字
	int value;//记录dD的值
};

//以下数据为系统中的全局数据

int A = 0;												//存储素数A
int line_num=1;									//记录每行代码在原始文件中的行号
int indent_num=0;								//输出缩进的个数
int project_tempvar_num=0;				//记录整个工程中生成的临时变量tmp_Array_n 的个数
int project_tempcondvar_num=0;  	//记录整个工程中生成的临时变量tmp_CondArray_n 的个数
int macro_label=0;								//记录汇编宏调用时的LABEL标号，每个文件从0开始，每输出一个宏汇编调用，就加1
bool debug_flag=false;						//debug模式标记，默认为假值
string red_path="";							//冗余代码存放的路径
string str_A="";									//存储素数A的字符串
string enter_fun_name="";				//存储待编码工程中的入口函数名称
string current_file_name="";				//存储当前被处理的文件的名称
ifstream fin;											//关于签名文件的输入文件流

//2010年3月10日追加
string  file_info="";	 //用于记录当前所分析的代码隶属的文件的名字
int fake_line_num=1;		//记录每行代码在展开后的文件中的行号

string bool_config_file="/config/bool_values.txt";	//布尔配置文件名
string sig_config_file="/config/pre_sig.txt";				//预配置签名文件名
string prime_config_file="/config/prime_value.txt";	//素数A配置文件名
string sig_list_file="/config/randNumSeqFile.txt";//签名文件名
string asm_macro_file="/config/asm.txt";//汇编宏代码源文件名

//存储整个待编码工程中所有遇到过的全局变量的名称
//该链表中不会有重复的名称
list<string>* global_var_names = new list<string>;

//存储整个待编码工程中所有遇到过的函数名称
//该链表中不会有重复的名称
list<string>* function_names=new list<string>;

//保存整个工程中的需要在red_global_para_init()中调整的全局变量
list<Global_var>* global_vars=new list<Global_var>;

//动态签名表，存有整个工程中所有变量的名称，签名等信息
//对函数编码完成后，将签名表中，该函数里的局部变量清除掉
//对一个文件编码完毕后，将里面的临时变量清除掉
list<Var_info>* var_infos =new list<Var_info>;

//存储整个工程中的所遇到的函数信息,包括函数名，参数，返回值等
list<Fun_info>* fun_infos =new list<Fun_info>;

//存储当前文件中的注释信息，包括每条注释的内容，起止行号等
//当前文件编码结束后，需要将链表置空
list<Cmnt>* cmnts=new list<Cmnt>;

//存储工程中用到的所有布尔变量的真假值信息
list<Bool_var>* bool_vars=new list<Bool_var>;

//存储工程中用到的所有布尔变量的真假值的签名信息
list<Bool_var>* bool_sigs=new list<Bool_var>;

//存储从预配置签名文件中读取到的签名
map<string, int>* pre_sigs=new map<string, int>;

//存储在对循环结构进行编码时，dD的名称及其对应值
list<dD_value>* dD_values=new list<dD_value>;

//2014-12-15增加 每个函数中各类临时变量索引的数据范围
map<string,TFuncTmpRange> g_MapFuncTmpVar;
//2014-12-29增加 立即数的签名 李晨欢
 map<int,int> constantMap;
 extern int get_sig_index(int *);
 extern ofstream gen_out;
extern void add_new_sig(list<Var_info>*, string, int, string);

extern void add_new_vars(list<Var_info>*,string,int,string,string,int);

//2015-04-13添加 存储函数附加变量用于输出到VCL中
map<string, int> g_ExFun_Hash_map;
//2015-04-13添加 存储函数返回值全局变量到VCL中
map<string, string>  AC_RetVal_map;
//2015-04-28添加 存储函数返回值全局变量到VCL中
map<string, string>  AC_ParmVar_map;
//2015-06-09李晨欢添加变量进datetime_var_list中，用于输出时间标签
list<string>* datetime_var_list=new list<string>;
//2015-06-10邹宇添加，判断是否扫描过声明链表的标识
bool flag_var_dec=false;

//以下函数为系统中频繁使用的库函数

// 功能：检查string类型的参数有效性，根据是否为空来判断是否有效，如果为空，则向上抛出错误信息
// 输入参数：arg_name-上层传入的要检查的string参数
// 			      info-上层传入的附加报错信息
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void check_str_arg(string arg_name,string info)
{
	//判断传入的参数名字符串是否为空
	if(arg_name.empty())
	{
		//如果为空，则抛出错误信息
		string error_info="invalid argument : "+info+" is empty!";
		throw error_info;
	}
	else
	{
		//如果不为空，则忽略
		;
	}
}

// 功能：检查指针类型的参数有效性，根据是否为NULL来判断是否有效，如果为NULL，则向上抛出错误信息
// 输入参数：pt-上层传入的要检查的指针参数
// 			      info-上层传入的附加报错信息
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void check_pt_arg(void *pt,string info)
{
	//判断传入的参数名字符串是否为空
	if(NULL==pt)
	{
		//如果为空，则抛出错误信息
		string error_info="invalid argument : "+info+" is NULL!";
		throw error_info;
	}
	else
	{
		//如果不为空，则忽略
		;
	}
}

//功能：为了代码格式的需要，输出缩进
//输入参数：num-缩进的制表符个数，out-输出文件流
//输出参数：out-输出文件流
//返回值：成功返回void，失败则抛出错误
void out_indent(int num, ofstream& out)
{
	//检查输出制表符的个数是否为负
	if(num<0)
	{
		//如果为负，报错
		string error_info="output indent ：indent number is invalid!";
		throw error_info;
	}
	//检查文件流是否打开
	else if(!out.is_open())
	{
		//如果输出文件没有打开，报错
		string error_info="output indent ：ofstream is not open!";
		throw error_info;
	}
	else
	{
		//输出缩进个数不为负，且输出文件已经打开
		for(int i=0;i<num;i++)
		{
			out<<"  ";	//输出两个空格
		}
	}
}

// 功能：将整形转换成字符串
// 输入参数：num-int类型的待转换的数字
// 返回值：返回转换出的字符串
// 作者：李刚
string  int2str( int   num)
{
   stringstream stream("");	//字符串流
   string str="";					//存放转换的字符串
   stream << num;				//将num值输入流
   stream >> str;				//从stream中抽取前面插入的num值
   return str;
}

//将整型数转为十六进制字符串，用于输出立即数
//例如：输入是16，输出是 0x00000010
string int2HexStr(const int num)
{
	char *num_str=new char[100];
	sprintf(num_str,"%d",num);
//	sprintf(num_str,"%08X",num);
//	char HexSymbol[16]={'0','1','2','3'
//						,'4','5','6','7'
//						,'8','9','A','B'
//						,'C','D','E','F'};
//	string HexStr(8,'0');
//	int NumMod=0;
//	int CurNum=num>=0? num:-num;
//	for(int i=7;i>=0;i--)
//	{
//		if(CurNum>0)
//		{
//			NumMod=CurNum%16;
//			HexStr[i]=HexSymbol[NumMod];
//			CurNum>>=4;
//		}
//		else
//			break;
//	}
//	HexStr=num>=0? "(0x"+HexStr+")":"(-0x"+HexStr+")";
	string HexStr=num_str;
	delete num_str;
	return HexStr;
}

//整数转16进制，用于输出立即数编码绑定变量
string int2HexStr_data(const int num)
{
	char *num_str=new char[100];
	sprintf(num_str,"%08X",num);
	string hex_data="0x"+string(num_str);
	delete num_str;
	return hex_data;
}

// 功能：将字符串转换成整形
// 输入参数：str-待转换的字符串
// 返回值：成功则返回转换出的整形值，失败则抛出错误信息
// 作者：李刚
int   str2int( string   str)
{
	try
	{
		check_str_arg(str,"string ");//检查参数：待转换的字符串是否为空
		
		int num=0;//存储转换出的整型值
		//判断字符串中的整数是否为16进制
		if("0x"==str.substr(0,2) || "0X"==str.substr(0,2))
		{
			//如果是，则将其以16进制数读入，并转换为十进制整数
			sscanf((char*)(str.c_str()),"%x",&num);    
		}
		else
		{
			//判断字符串中的整数是否为8进制
			if("0"==str.substr(0,1))
			{
				//如果是，则将其以8进制数读入，并转换为十进制整数
				sscanf((char*)(str.c_str()),"%o",&num); 
			}
			else
			{
				//如果不是，则将其以10进制数读入，并转换为十进制整数
				sscanf((char*)(str.c_str()),"%d",&num); 
			}
		}
		return num;
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="convert string to int : "+s;
		throw error_info;
	}
} 


// 功能：在动态签名表中，根据变量的名称及其当前位于的作用域，查找该变量的当前签名
// 输入参数：s-动态签名表指针
// 		          str-变量名称
//                  scope-该变量当前所在的作用域
// 返回值：成功则返回查找到的签名值，失败则抛出错误信息
// 作者：李刚
int get_current_sig(list<Var_info>* s,string str,string scope)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"var_info's list pointer ");
		check_str_arg(str,"variable's name ");
		check_str_arg(scope,"varibale's scope ");
		
		//不为空，则查找动态签名表
		list<Var_info>::iterator iter;//动态签名表迭代器
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//判断当前迭代器所指对象的名字和作用域是否与要查找变量的一致
			if(scope==iter->get_scope() && str==iter->get_name())
			{
				//如果一致，则返回当前签名
				return iter->get_sigs()->back();
			}
			else
			{
			}
		}
		//如果没有找到，将作用域换成"global"再查找一次
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//判断当前迭代器所指对象的名字是否和要查找的变量名一致，且作用域为"global"
			if("global"==iter->get_scope() && str==iter->get_name())
			{
				//如果一致，则返回当前签名
				return iter->get_sigs()->back();
			}
			else
			{
			}
		}
		//查找中没有返回，说明没有找到该变量，抛出错误
		string error_info="can not find varibale : "+str+"in var_info lis!";
		throw error_info;
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="get  current signature : "+s;
		throw error_info;
	}
}

// 功能：在动态签名表中，根据变量的名称及其当前位于的作用域，查找该变量的当前签名索引
// 输入参数：s-动态签名表指针
// 		          str-变量名称
//                  scope-该变量当前所在的作用域
// 返回值：成功则返回查找到的签名值，失败则抛出错误信息
// 作者：李晨欢
int get_current_sigIndex(list<Var_info>* s,string str,string scope)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"var_info's list pointer ");
		check_str_arg(str,"variable's name ");
		check_str_arg(scope,"varibale's scope ");

		//不为空，则查找动态签名表
		list<Var_info>::iterator iter;//动态签名表迭代器
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//判断当前迭代器所指对象的名字和作用域是否与要查找变量的一致
			if(scope==iter->get_scope() && str==iter->get_name())
			{
				//如果一致，则返回当前签名
				return iter->getSigIndex();
			}
			else
			{
			}
		}
		//如果没有找到，将作用域换成"global"再查找一次
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//判断当前迭代器所指对象的名字是否和要查找的变量名一致，且作用域为"global"
			if("global"==iter->get_scope() && str==iter->get_name())
			{
				//如果一致，则返回当前签名
				return iter->getSigIndex();
			}
			else
			{
			}
		}
		//查找中没有返回，说明没有找到该变量，抛出错误
		string error_info="can not find varibale : "+str+"in var_info lis!";
		throw error_info;
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="get  current signature : "+s;
		throw error_info;
	}
}
// 功能：在动态签名表中，根据变量的名称及其当前位于的作用域，查找该变量的标签信息
// 输入参数：s-动态签名表指针
// 		          var_name-变量名称
//                  var_scope-该变量当前所在的作用域
// 返回值：成功则返回查找到的标签信息，失败则抛出错误信息
// 作者：李刚
string get_var_stamp(list<Var_info>* s,string var_name,string var_scope)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"var_info list's pointer");
		check_str_arg(var_name,"variable's name");
		check_str_arg(var_scope,"variable's scope");
		
		list<Var_info>::iterator iter;//动态签名表迭代器
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//判断当前迭代器所指对象的名字和作用域是否与要查找变量的一致
			if(var_scope==iter->get_scope() && var_name==iter->get_name())
			{
				//如果一致，则返回当前对象的标签信息
				return iter->get_stamp();
			}
			else
			{
			}
		}
		//如果没有找到，将作用域换成"global"再查找一次
		for(iter=s->begin();iter!=s->end();++iter)
		{
			//判断当前迭代器所指对象的名字是否和要查找的变量名一致，且作用域为"global"
			if("global"==iter->get_scope() && var_name==iter->get_name())
			{
				//如果一致，则返回当前对象的标签信息
				return iter->get_stamp();
			}
			else
			{
			}
		}
		
		//查找中没有返回，说明没有找到该变量，抛出错误
		string error_info="can not find varibale : "+var_name+"in var_info lis!";
		throw error_info;
	}
	catch (string str)
	{
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="get variable's stamp : "+str;
		throw error_info;
	}
}

// 功能：根据行号，查找此行号之前是否有注释，如果有，则打印注释，并且保证输出到正确的位置。
// 输入参数：cmnt_list-存放有所有注释信息的链表指针
//                  line-当前的行号
//			      out-文件输出流
//输出参数：out-文件输出流
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void out_cmnt(list<Cmnt>* cmnt_list,int line,ofstream& out)
{
	try
	{
		//检查参数：注释链表指针是否为空
		check_pt_arg(cmnt_list,"comment list's pointer");
		
		//遍历注释链表
		for(list<Cmnt>::iterator iter=cmnt_list->begin();iter!=cmnt_list->end();iter++)
		{
			//判断当前迭代器所指的对象的起始行号是否小于传入的行号，且输出标志是否为false
			if(iter->get_line_begin()<=line && iter->get_output()==false)
			{
				//说明该对象所含的注释信息尚未打印，且需要打印
				out_indent(indent_num, out);//输出缩进
				out<<iter->output_cmnt(indent_num)<<endl;//输出注释
				
				//注释打印之后，将该对象的输出标志置为true，表明该注释已被打印
				iter->set_output(true);
			}
			else
			{
				//不满足判断条件，则忽略，继续查找
			}
		}
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print comment : "+s;
		throw error_info;
	}
}

// 功能：输出动态签名表中的签名到文件中，维护时使用
// 输入参数：s-动态签名表
//                  file_name-所要输出的文件
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void out_var_sigs(list<Var_info>* s, string file_name)
{
	try
	{
		//检查输入参数的有效性
		check_pt_arg(s,"var_info list's pointer");
		check_str_arg(file_name,"file's name");
		
		//创建输出文件流
		ofstream out;
		out.open(file_name.c_str());
		
		//判断文件是否创建成功
		if(!out.is_open())
		{
			//为true，说明文件创建失败，报错
			string error_info="out put file creating failed!";
			throw error_info;
		}
		else
		{
			//为false，文件创建成功，开始遍历动态签名表，输出所有签名
			for(list<Var_info>::iterator iter=s->begin();iter!=s->end();++iter)
			{
				out<<iter->get_name()<<"'s signature history : ";
				unsigned int k=0;//对已经输出的签名进行计数
				//遍历签名表
				for(vector<int>::iterator iter1=iter->get_sigs()->begin();iter1!=iter->get_sigs()->end();iter1++,k++)
				{
					//先判断已输出的签名个数是否>=10，且为10的倍数
					if(k>=10 && k%10==0 )
					{
						//如果是则输出回车符，使其换行
						out<<"\n";
					}
					out<<*iter1<<" ";
				}
				out<<endl;
			}
		}
		out.close();
	}
	catch( string s)
	{
		//捕获抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info="print signatures : "+s;
		throw error_info;
	}
}

string BuildOutputFunc_str(string LeftVar,string FuncName,...)
{
	va_list paraList;
	va_start(paraList,FuncName);
	string BuildFuncName=string(FuncName)+"(";
	const char *CurPara=va_arg(paraList,const char*);
	while(CurPara!=NULL)
	{
		BuildFuncName=BuildFuncName+string(CurPara);
		CurPara=va_arg(paraList,const char*);
		if(CurPara!=NULL)
			BuildFuncName=BuildFuncName+", ";
	}
	BuildFuncName=BuildFuncName+")";
	va_end(paraList);
	string FuncCallStmt=LeftVar+" = "+BuildFuncName+";";
	return FuncCallStmt;
}

string BuildOperationExp_str(string& LeftVar,const string& Op,string& Mod,...)
{
	va_list paralist;
	va_start(paralist,Mod);
	int Op_i=0;
	const char *cur_para=va_arg(paralist,const char *);
	string opExp=cur_para;
	while(Op_i<Op.size())
	{
		opExp=opExp+" "+Op[Op_i++];
		cur_para=va_arg(paralist,const char *);
		opExp=opExp+" "+cur_para;
	}
	opExp=LeftVar+" = ("+opExp+") % "+Mod+";";
	return opExp;
}

string BuildSigIdAdjust_str(string LeftVar,int sig_id)
{
	string SigIDAdjStr;
	if(LeftVar!=ARRAY_EXTR_NAME)
	{
		LeftVar=TmpVarJudge(LeftVar)? LeftVar+VAR_CHK_ID_NAME:CODED_VAR_PREFIX+LeftVar+VAR_CHK_ID_NAME;
		SigIDAdjStr=BuildOutputFunc_str(LeftVar,SIG_REG_ITF,int2HexStr(sig_id).c_str(),NULL);
	}
	else
	{
		SigIDAdjStr=BuildOutputFunc_str("",FUNC_ARRAYEXT_SIG_UPDATE,int2HexStr(sig_id).c_str(),NULL);
		SigIDAdjStr=SigIDAdjStr.substr(3);
	}
	return SigIDAdjStr;
}

//邹宇修改
void constant2Str(const int constant)
{
	string value=int2HexStr_data(constant);
	string bind_str="AC_BIND_CONST_NUM_"+value;
	string const_str="CONST_NUM_"+value;
//	if(constant<0)
//		value="NEG_"+int2str(-constant);
//	else
//		value=int2str(constant);
	out_indent(indent_num,gen_out);
	gen_out<<bind_str+".Data.F"<<" = "<<const_str+".Data.F"<<";"<<endl;

	out_indent(indent_num,gen_out);
	gen_out<<bind_str+".Data.C.C1"<<" = "<<const_str+".Data.C.C1 + g_GS_TL1;"<<endl;

	out_indent(indent_num,gen_out);
	gen_out<<bind_str+".Data.C.C2"<<" = "<<const_str+".Data.C.C2 + g_GS_TL2;"<<endl;
}

//邹宇修改
void bool2Str(ofstream &out,bool inbool)
{
	string bind_str=CONST_BOOL_BIND_PREFIX;
	string const_str=CONST_BOOL_PREFIX;
	if(inbool=true)
	{
		bind_str+="TRUE";
		const_str+="TRUE";
	}
	else
	{
		bind_str+="FALSE";
		const_str+="FALSE";
	}
	out_indent(indent_num,out);
	out<<bind_str+".Data.F"<<" = "<<const_str+".Data.F"<<";"<<endl;

	out_indent(indent_num,out);
	out<<bind_str+".Data.C.C1"<<" = "<<const_str+".Data.C.C1 + g_GS_TL1;"<<endl;

	out_indent(indent_num,out);
	out<<bind_str+".Data.C.C2"<<" = "<<const_str+".Data.C.C2 + g_GS_TL2;"<<endl;
}

string constantBindedStr(const int constant){
	string valueStr=int2HexStr_data(constant);
	return valueStr;
}

int getConstantSig(int constant,int *sigIndex){
	map<int,int>::iterator it=constantMap.find(constant);
//	string headConstant=constant>0?"CONST_NUM_":"CONST_NUM__NEG_";
	string value=int2HexStr_data(constant);
	string headConstant="CONST_NUM_"+value;
 	if(it==constantMap.end())
	//没找到
 	{
 		int index=0;
 		constantMap[constant]=get_sig_index(&index);
 		if(sigIndex!=NULL)
 			*sigIndex=index;
 		add_new_vars(var_infos, headConstant+int2str(constant), constantMap[constant], "global", "constant",index);//立即数的stamp暂用constant
	 	return constantMap[constant];
	 }
	 else
	 {
		 if(sigIndex!=NULL)
		  	*sigIndex=get_current_sigIndex(var_infos,headConstant+int2str(constant),"global");
		 return constantMap[constant];
	 }
 }
int getConstantSig(int constant){
	map<int,int>::iterator it=constantMap.find(constant);
 	if(it==constantMap.end())
	//没找到
 	{
 		int sigIndex=-1;
 		constantMap[constant]=get_sig_index(&sigIndex);
// 		string headConstant=constant>0?"CONST_NUM_":"CONST_NUM__NEG_";
 		string value=int2HexStr_data(constant);
 		string headConstant="CONST_NUM_"+value;
 		add_new_vars(var_infos, headConstant+int2str(constant), constantMap[constant], "global", "constant",sigIndex);//立即数的stamp暂用constant
 		return constantMap[constant];
	 }
	 else
	 {
		 return constantMap[constant];
	 }
 }

//复制文件方法
void Tranfile(string strFileCopeIn, string strFileCopeOut)
{
	ofstream OutFile(strFileCopeOut.c_str());
	ifstream InFile(strFileCopeIn.c_str());

	string buf;
	string strInput;
	if(InFile.is_open())
	{
		while(getline(InFile, strInput))
		{


			OutFile << strInput << endl;
		}
		InFile.close();
	}
	else
	{
		log_error("Copy Config File failed! Reason:open Src File failed.\n")
		exit(1);
	}
}

class IOArrayMap
{
private:
	IOArrayMap()
	{
		m_NameMap["g_USR_BufRecv"] = "g_ITF_BufMsgRecv";
		m_NameMap["g_USR_BufFetch"] = "g_ITF_BufMsgFetch";
		m_NameMap["g_USR_BufSend"] = "g_ITF_BufMsgSend";
		m_NameMap["g_USR_BufDeliver"] = "g_ITF_BufMsgDeliver";
	}
	std::map<string, string> m_NameMap;
	static IOArrayMap *m_pInstance;
public:
	static IOArrayMap* GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new IOArrayMap();
		}
		return m_pInstance;
	}

	std::map<string, string>& GetMap()
	{
		return m_NameMap;
	}
};
IOArrayMap *IOArrayMap::m_pInstance = NULL;

//获取数组名在CSC中的名称
string GetArrayNameInCSC(string ArrayName)
{
	//判断是否IO数组
	std::map<string, string>& refMap = IOArrayMap::GetInstance()->GetMap();

	std::map<string, string>::iterator it = refMap.begin();

	for(; it != refMap.end(); ++it)
	{
		string SubStr = ArrayName.substr(0, it->first.size());
		if(SubStr == it->first)
		{
			string StrInCSC = it->second + ArrayName.substr(it->first.size());
			return StrInCSC;
		}
	}

	//若不是，直接加前缀"TC_"
	return "TC_"+ArrayName;
}
