
#include "struct_array.h"


Struct_info::Struct_info(string name,list<mem_loc>* vs,int i,string scope)
{
	struct_name_=name;
	members_=new list<mem_loc>;
	members_->insert(members_->end(),vs->begin(),vs->end());
	num_=i;
	scope_=scope;
}

string Struct_info::get_struct_name()
{
	return struct_name_;
}


list<mem_loc>* Struct_info::get_members()
{
	return members_;
}

int Struct_info::get_num()
{
	return num_;
}

string Struct_info::get_scope()
{
	return scope_;
}

void Struct_info::set_struct_name(string s)
{
	struct_name_=s;
}

void Struct_info::set_members(list<mem_loc>* l)
{
	members_=l;
}

void Struct_info::set_num(int i)
{
	num_=i;
}

void Struct_info::set_scope(string s)
{
	scope_=s;
}
/*-----------------struct_info---------end---------*/


/*----------------Array_info--------begin---------------*/
Array_info::Array_info()
{
}

Array_info::Array_info(string str,vector<int>* v)
{
	array_name_=str;
	subs_=new vector<int>;
	subs_->insert(subs_->end(),v->begin(),v->end());
}

string Array_info::get_name()
{
	return array_name_;
}

vector<int>* Array_info::get_subs()
{
	return subs_;
}

string Array_info::get_scope()
{
	return scope_;
}

string Array_info::get_stamp()
{
	return stamp_;
}

void Array_info::set_scope(string s)
{
	scope_=s;
}

void Array_info::set_stamp(string s)
{
	stamp_=s;
}
/*---------------------Array_info--------end---------------*/


