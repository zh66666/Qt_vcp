#ifndef APP_UTILS_H_
#define APP_UTILS_H_
#include <cstdarg>
#include <cstdio>
#include <string>
using std::string;

//ͨ�����
#define log_error(format, ...) do \
					{ \
						printf("error occur. FileName=%s, LineNo=%d, Function=%s. description:",\
								__FILE__, __LINE__, __FUNCTION__); \
						printf((format), ##__VA_ARGS__); \
					} while(false);

template<typename T, size_t Size>
char (*get_count(T (&array)[Size]))[Size];

#define __count_of(Array) get_count((Array))

#define __countof(Array) sizeof(*__count_of((Array)))
//�����ļ�����
const string str_IO_FILE_NAME = "IO_interface.c";
const string str_IO_FILE_PATH = "/config/VCL_impl/";

//IO��������ݴ��ļ�
const string str_IO_TMP_SRC = "./IO_Tmp.c";
const string str_IO_TMP_H = "./IO_Tmp.h";




#endif


