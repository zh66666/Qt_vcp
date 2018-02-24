#ifndef TYPE_SPECIFIER_H_
#define TYPE_SPECIFIER_H_

#include "common.h"

/* a class used to store the type specifier */
class Type_specifier
{
	private:
		string type_name_;//store the type name
		Struct_specifier* struct_spec_;//store the struct specifier
		Enum_specifier* enum_spec_;//store the enum specifer
		int stamp_;//a stamp used to mark the kind of the type specifer
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//该类型标记的伪行号，2010年3月10日追加
		
	public:
		Type_specifier();//default,stamp_=-1
		explicit Type_specifier(string,int);//common type,stamp_=0,userdefine type,stamp_=1
		explicit Type_specifier(Struct_specifier*);//struct type,stamp_=2
		explicit Type_specifier(Enum_specifier*);//enume type,stamp_=3
		
		string get_type_name();//get the type name
		Struct_specifier* get_struct_spec();//get the struct specifier
		Enum_specifier* get_enum_spec();//get the enum specifer
		int get_stamp();//get the stamp
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//获取该类型标记的伪行号，2010年3月10日追加
		void set_stamp(int);//set the stamp
		void set_type_name(string);//set the type name
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_fake_line(int);//设置该类型标记的伪行号，2010年3月10日追加
};

#endif

