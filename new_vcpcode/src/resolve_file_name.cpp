#include <string>
#include <iostream>
using namespace std;

// 功能：分析文件的路径名，解析出文件名称，并返回；同时将文件路径名中的文件名删去
// 例如：传入的参数为 /src/testcase/1.c，则返回1.c，同时将参数修改为/src/testcase/
// 注意：参数中的路径分割符必须为“/”
// 输入参数：path_name-文件路径名
// 输出参数：path_name-文件路径名(路径名：../.../，最后一个字符是/)
// 返回值：成功则返回解析出来的文件名，失败则抛出错误信息
string resolve_file_name(string& path_name)
{
	//判断文件路径名字符串是否为空
	if(path_name.empty())
	{
		//如果为空，则抛出错误信息
		string error_info="resolving file name from path_name , path name is empty!";
		throw error_info;
	}
	else
	{
		//如果不为空，则开始解析
		string file_name="";		//存储解析出的文件名
		unsigned int pos=path_name.find_last_of("/");//记录路径名字符串中最后一个"/"的位置
		
		//判断“/”的位置是否为默认的位置
		if(pos==string::npos)
		{
			//为默认的位置，说明路径名中没有"/"，说明为当前目录下的文件
			file_name=path_name;	//路径名即是文件名
			path_name.replace(0,path_name.length(),"./");	//将路径名修改成"./"
		}
		else
		{
			//不为默认的位置，说明路径名中含有“/”
			file_name = path_name.substr(pos+1);	//最后一个“/”之后的字符串即为文件名
			path_name.erase(pos+1,file_name.length());	//将路径名中的文件名部分删去
		}
		return file_name;
	}
}
