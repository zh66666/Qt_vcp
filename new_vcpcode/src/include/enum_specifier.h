#ifndef ENUM_SPECIFIER_H_
#define ENUM_SPECIFIER_H_

#include "common.h"

/* a class used to store the enum specifier */
class Enum_specifier
{
	private:
		list<Enumerator>* enumerator_list_;//store the enumerators in the enum
		string name_;//store the name of the enum type
		int stamp_;//a stamp used to mark the kind of the enum specifer
		int line_begin_;//store the beginning line number
		int line_end_;//store the ending line number
		int fake_line_;//enum���ͱ�ǵ�α�кţ�2010��3��10��׷��
	public:
		Enum_specifier();//default,stamp_=-1
		explicit Enum_specifier(list<Enumerator>*);//only has the enumerators,stamp_=0
		explicit Enum_specifier(string,list<Enumerator>*);//has both the name and enumerators,stamp_=1
		explicit Enum_specifier(string);//only has the name,stamp_=2
		
		string get_name();//get the name of the enum type
		list<Enumerator>* get_enum_list();//get the enumerators in the enum
		int get_stamp();//get the stamp
		
		int get_line_begin();//get the beginning line number
		int get_line_end();//get the ending line number
		int get_fake_line();//��ȡenum���ͱ�ǵ�α�кţ�2010��3��10��׷��
		void set_line_begin(int);//set the beginning line number
		void set_line_end(int);//set the ending line number
		void set_fake_line(int);//����enum���ͱ�ǵ�α�кţ�2010��3��10��׷��
};

#endif
