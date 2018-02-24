#include "program_struct.h"
#include "var_info.h"
#include "fun_info.h"
#include "global_var.h"
#include "bool_var.h"

/*------------------------Toke_num--begin-------------------*/
Toke_num::Toke_num()
{
	toke_="";
}

Toke_num::Toke_num(string toke,int line)
{
	toke_=toke;
	num_=line;
}

int Toke_num::get_fake_line()
{
	return fake_line_;
}

void Toke_num::set_fake_line(int i)
{
	fake_line_=i;
}


string Toke_num::get_toke()
{
	return toke_;
}

int Toke_num::get_num()
{
	return num_;
}
/*------------------------Toke_num--end-------------------*/


/*------------------------Struct_specifier--begin-------------------*/
Struct_specifier::Struct_specifier():stamp_(-1)
{
}

Struct_specifier::Struct_specifier(string str,list<Var_declaration>* v):stamp_(0)
{
	name_=str;
	var_dec_list_=v;
}

Struct_specifier::Struct_specifier(list<Var_declaration>* v):stamp_(1)
{
	var_dec_list_=v;
}

Struct_specifier::Struct_specifier(string str):stamp_(2)
{
	name_=str;
}


string Struct_specifier::get_name()
{
	return name_;
}

list<Var_declaration>* Struct_specifier:: get_var_dec_list()  
{
	return var_dec_list_;
}

int Struct_specifier::get_stamp()
{
	return stamp_;
}

int Struct_specifier::get_line_begin()
{
	return line_begin_;
}

int Struct_specifier::get_line_end()
{
	return line_end_;
}

int Struct_specifier::get_fake_line()
{
	return fake_line_;
}

void Struct_specifier::set_line_begin(int i)
{
	line_begin_=i;
}

void Struct_specifier::set_line_end(int i)
{
	line_end_=i;
}

void Struct_specifier::set_fake_line(int i)
{
	fake_line_=i;
}

/*------------------------Struct_specifier--end-------------------*/



/*-----------------------Declarator-------begin------------------------*/

Declarator::Declarator():stamp_(-1)
{
}

Declarator::Declarator(string name):stamp_(0)
{
	name_=name;
	array_subs_=new list<Expression>;
}

Declarator::Declarator(string name,list<Expression>* l):stamp_(1)
{
	name_=name;
	array_subs_=new list<Expression>;
	array_subs_=l;
}

string Declarator::get_name()
{
	return name_;
}

list<Expression>* Declarator::get_array_subs()
{
	return array_subs_;
}

int Declarator::get_stamp()
{
	return stamp_;
}

int Declarator::get_line_begin()
{
	return line_begin_;
}

int Declarator::get_line_end()
{
	return line_end_;
}

void Declarator::set_stamp(int i)
{
	stamp_=i;
}

void Declarator::set_array_subs(list<Expression>* l)
{
	array_subs_->clear();
	array_subs_=l;
}

void Declarator::set_line_begin(int i)
{
	line_begin_=i;
}

void Declarator::set_line_end(int i)
{
	line_end_=i;
}
/*-----------------------Declarator-------end--------------------------*/




/*-----------------------Enumerator-------begin------------------------*/
Enumerator::Enumerator():stamp_(-1)
{
}

Enumerator::Enumerator(string ele):stamp_(0)
{
	element_=ele;
}

Enumerator::Enumerator(string ele,Expression* ce):stamp_(1)
{
	element_=ele;
	exp_=ce;
}

string 	Enumerator::get_element()
{
	return element_;
}

Expression* Enumerator::get_exp()
{
	return exp_;
}

int Enumerator::get_stamp()
{
	return stamp_;
}

int Enumerator::get_line_begin()
{
	return line_begin_;
}

int Enumerator::get_line_end()
{
	return line_end_;
}

void Enumerator::set_line_begin(int i)
{
	line_begin_=i;
}

void Enumerator::set_line_end(int i)
{
	line_end_=i;
}
/*-----------------------Enumerator-------end------------------------*/


/*------------------------Enum_specifier-------------begin-----------*/
Enum_specifier::Enum_specifier():stamp_(-1)
{
}

Enum_specifier::Enum_specifier(list<Enumerator>* enumers):stamp_(0)
{
	enumerator_list_=enumers;
}

Enum_specifier::Enum_specifier(string name,list<Enumerator>* enumers):stamp_(1)
{
	name_=name;
	
	enumerator_list_=enumers;
}

Enum_specifier::Enum_specifier(string name):stamp_(2)
{
	name_=name;
	
}

string Enum_specifier::get_name()
{
	return name_;
}

list<Enumerator>* Enum_specifier::get_enum_list()
{
	return enumerator_list_;
}

int Enum_specifier::get_stamp()
{
	return stamp_;
}

int Enum_specifier::get_line_begin()
{
	return line_begin_;
}

int Enum_specifier::get_line_end()
{
	return line_end_;
}

int Enum_specifier::get_fake_line()
{
	return fake_line_;
}

void Enum_specifier::set_line_begin(int i)
{
	line_begin_=i;
}

void Enum_specifier::set_line_end(int i)
{
	line_end_=i;
}

void Enum_specifier::set_fake_line(int i)
{
	fake_line_=i;
}
/*------------------------Enum_specifier-------------end-----------*/


/*------------------------Init_declarator-----------begin----------*/
Init_declarator::Init_declarator():stamp_(-1)
{
}

Init_declarator::Init_declarator(Declarator* dec):stamp_(0)
{
	dec_=dec;
}

Init_declarator::Init_declarator(Declarator* dec,Initializer* init):stamp_(1)
{
	dec_=dec;
	init_=init;
}

Declarator* Init_declarator::get_dec()
{
	return dec_;
}

Initializer* Init_declarator::get_init()
{
	return init_;
}

int Init_declarator::get_stamp()
{
	return stamp_;
}

int Init_declarator::get_line_begin()
{
	return line_begin_;
}

int Init_declarator::get_line_end()
{
	return line_end_;
}

void Init_declarator::set_line_begin(int i)
{
	line_begin_=i;
}

void Init_declarator::set_line_end(int i)
{
	line_end_=i;
}
/*------------------------Init_declarator-----------end----------*/
 
 
 
/*----------------Initializer--------------begin-------------------*/

Initializer::Initializer():stamp_(-1)
{
	
}

Initializer::Initializer(Expression* exp):stamp_(0)
{
	exp_=exp;
}

Initializer::Initializer(list<Initializer>* init):stamp_(1)
{
	init_list_=init;
}

Expression* Initializer::get_exp()
{
	return exp_;
}

list<Initializer>* Initializer::get_init_list()
{
	return init_list_;
}

int Initializer::get_stamp()
{
	return stamp_;
}

int Initializer::get_line_begin()
{
	return line_begin_;
}

int Initializer::get_line_end()
{
	return line_end_;
}

void Initializer::set_line_begin(int i)
{
	line_begin_=i;
}

void Initializer::set_line_end(int i)
{
	line_end_=i;
}
/*----------------Initializer--------------end-----------------------*/


/*----------Var_declaration------------begin------------------------*/				
Var_declaration::Var_declaration():stamp_(-1)
{
	nodt_stamp_=-1;
	bool_stamp_=-1;
	noadj_stamp_=-1;
}

Var_declaration::Var_declaration(Declaration_specifiers* decl_spec):stamp_(0)
{
	decl_spec_=decl_spec;
	nodt_stamp_=-1;
	bool_stamp_=-1;
	noadj_stamp_=-1;
	fake_line_=decl_spec->get_fake_line();
}

Var_declaration::Var_declaration(Declaration_specifiers* decl_spec,list<Init_declarator>* inits):stamp_(1)
{
	decl_spec_=decl_spec;
	inits_=inits;
	nodt_stamp_=-1;
	bool_stamp_=-1;
	noadj_stamp_=-1;
	fake_line_=decl_spec->get_fake_line();
}

Declaration_specifiers* Var_declaration::get_decl_spec()
{
	return decl_spec_;
}

list<Init_declarator>* Var_declaration::get_inits()
{
	return inits_;
}

string Var_declaration::get_file_info()
{
	return file_info_;
}

int Var_declaration::get_stamp()
{
	return stamp_;
}

int Var_declaration::get_noadj_stamp()
{
	return noadj_stamp_;
}

int Var_declaration::get_bool_stamp()
{
	return bool_stamp_;
}

int Var_declaration::get_nodt_stamp()
{
	return nodt_stamp_;
}

int Var_declaration::get_line_begin()
{
	return line_begin_;
}

int Var_declaration::get_line_end()
{
	return line_end_;
}

int Var_declaration::get_fake_line()
{
	return fake_line_;
}

void Var_declaration::set_line_begin(int i)
{
	line_begin_=i;
}

void Var_declaration::set_line_end(int i)
{
	line_end_=i;
}

void Var_declaration::set_noadj_stamp(int i)
{
	noadj_stamp_=i;
}

void Var_declaration::set_nodt_stamp(int i)
{
	nodt_stamp_=i;
}

void Var_declaration::set_bool_stamp(int i)
{
	bool_stamp_=i;
}

void Var_declaration::set_stamp(int i)
{
	stamp_=i;
}

void Var_declaration::set_file_info(string s)
{
	file_info_=s;
}

void Var_declaration::set_fake_line(int i)
{
	fake_line_=i;
}
/*----------Var_declaration------------end------------------------*/




/*---------Declaration_specifiers----------begin------------------*/
Declaration_specifiers::Declaration_specifiers()
{
}

Declaration_specifiers::Declaration_specifiers(string str,list<Type_specifier>* l)
{
	stor_spec_=str;
	type_spec_list_=l;
	fake_line_=l->front().get_fake_line();
}



string Declaration_specifiers::get_stor_spec()
{
	return stor_spec_;
}

list<Type_specifier>* Declaration_specifiers::get_type_spec_list()
{
	return type_spec_list_;
}

int Declaration_specifiers::get_line_begin()
{
	return line_begin_;
}

int Declaration_specifiers::get_line_end()
{
	return line_end_;
}

int Declaration_specifiers::get_fake_line()
{
	return fake_line_;
}

void Declaration_specifiers::set_line_begin(int i)
{
	line_begin_=i;
}

void Declaration_specifiers::set_line_end(int i)
{
	line_end_=i;
}

void Declaration_specifiers::set_fake_line(int i)
{
	fake_line_=i;
}
/*---------Declaration_specifiers----------end------------------*/



/*--------------Type_specifier--------------begin---------------*/
Type_specifier::Type_specifier():stamp_(-1)
{
}

Type_specifier::Type_specifier(string type_name,int stamp):stamp_(stamp)
{
	type_name_=type_name;
}

Type_specifier::Type_specifier(Struct_specifier* struct_spec):stamp_(2)
{
	struct_spec_=struct_spec;
	fake_line_=struct_spec->get_fake_line();
}

Type_specifier::Type_specifier(Enum_specifier* enum_spec):stamp_(3)
{
	enum_spec_=enum_spec;
	fake_line_=enum_spec->get_fake_line();
}
		
string Type_specifier::get_type_name()
{
	return type_name_;
}

Struct_specifier* Type_specifier::get_struct_spec()
{
	return struct_spec_;
}

Enum_specifier* Type_specifier::get_enum_spec()
{
	return enum_spec_;
}

int Type_specifier::get_stamp()
{
	return stamp_;
}

int Type_specifier::get_line_begin()
{
	return line_begin_;
}

int Type_specifier::get_line_end()
{
	return line_end_;
}

int Type_specifier::get_fake_line()
{
	return fake_line_;
}

void Type_specifier::set_stamp(int i)
{
	stamp_=i;
}

void Type_specifier::set_type_name(string str)
{
	type_name_=str;
}

void Type_specifier::set_line_begin(int i)
{
	line_begin_=i;
}

void Type_specifier::set_line_end(int i)
{
	line_end_=i;
}

void Type_specifier::set_fake_line(int i)
{
	fake_line_=i;
}

/*--------------Type_specifier--------------end---------------*/





/*-------------Param----------begin--------------------------*/	
Param::Param()
{
}

Param::Param(list<Type_specifier>* l,Declarator* d):stamp_(0)
{
	type_spec_list_=l;
	dec_=d;
}

Param::Param(list<Type_specifier>* l):stamp_(1)
{
	type_spec_list_=l;
}

list<Type_specifier>* Param::get_type_spec_list()
{
	return type_spec_list_;
}

Declarator* Param::get_dec()
{
	return dec_;
}	

int Param::get_stamp()
{
	return stamp_;
}

int Param::get_line_begin()
{
	return line_begin_;
}

int Param::get_line_end()
{
	return line_end_;
}

void Param::set_line_begin(int i)
{
	line_begin_=i;
}

void Param::set_line_end(int i)
{
	line_end_=i;
}
/*-------------Param----------end--------------------------*/
 
 
 





/*---------------Fun_declaration----------begin---------*/		
Fun_declaration::Fun_declaration():stamp_(-1)
{
}

Fun_declaration::Fun_declaration(Declaration_specifiers* decl_spec,string fun_name,list<Param>* params,Compound_stmt* ccstmt):stamp_(0)
{
	decl_spec_=decl_spec;
	fun_name_=fun_name;
	params_=params;
	ccstmt_=ccstmt;
	fake_line_=decl_spec->get_fake_line();
}

Fun_declaration::Fun_declaration(Declaration_specifiers* decl_spec,string fun_name,list<Param>* params):stamp_(1)
{
	decl_spec_=decl_spec;
	fun_name_=fun_name;
	params_=params;
	fake_line_=decl_spec->get_fake_line();
}


Declaration_specifiers* Fun_declaration::get_decl_spec()
{
	return decl_spec_;
}

string Fun_declaration::get_fun_name()
{
	return fun_name_;
}

list<Param>* Fun_declaration::get_params()
{
	return params_;
}

Compound_stmt* Fun_declaration:: get_ccstmt()
{
	return ccstmt_;
}

string Fun_declaration::get_file_info()
{
	return file_info_;
}

int Fun_declaration::get_stamp()
{
	return stamp_;
}

int Fun_declaration::get_line_begin()
{
	return line_begin_;
}

int Fun_declaration::get_line_end()
{
	return line_end_;
}

int Fun_declaration::get_fake_line()
{
	return fake_line_;
}

void Fun_declaration::set_line_begin(int i)
{
	line_begin_=i;
}

void Fun_declaration::set_line_end(int i)
{
	line_end_=i;
}

void Fun_declaration::set_file_info(string s)
{
	file_info_=s;
}

void Fun_declaration::set_fake_line(int i)
{
	fake_line_=i;
}
/*---------------Fun_declaration----------end---------*/



/*----------------Declaration---------begin-----------*/		
Declaration::Declaration():stamp_(-1)
{
}

Declaration::Declaration(Var_declaration *cvd):cvd_(cvd),stamp_(0)
{
	file_info_=cvd->get_file_info();
	fake_line_=cvd->get_fake_line();
}

Declaration::Declaration(Fun_declaration *cfd):cfd_(cfd),stamp_(1)
{
	file_info_=cfd->get_file_info();
	fake_line_=cfd->get_fake_line();
}


Var_declaration* Declaration::get_cvd()
{
	return cvd_;
}

Fun_declaration* Declaration::get_cfd()
{
	return cfd_;
}		

string Declaration::get_file_info()
{
	return file_info_;
}


int Declaration::get_stamp()
{
	return stamp_;
}

int Declaration::get_line_begin()
{
	return line_begin_;
}

int Declaration::get_line_end()
{
	return line_end_;
}

int Declaration::get_fake_line()
{
	return fake_line_;
}

void Declaration::set_line_begin(int i)
{
	line_begin_=i;
}

void Declaration::set_line_end(int i)
{
	line_end_=i;
}

void Declaration::set_file_info(string s)
{
	file_info_=s;
}

void Declaration::set_fake_line(int i)
{
	fake_line_=i;
}
/*----------------Declaration---------end-----------*/	




/*---------------Statement------------begin-----------*/		
Statement::Statement():stamp_(-1)
{
}

Statement::Statement(Compound_stmt* ccstmt):ccstmt_(ccstmt),stamp_(0)
{
}

Statement::Statement(Selection_stmt* csstmt):csstmt_(csstmt),stamp_(1)
{
	fake_line_=csstmt->get_fake_line();
}

Statement::Statement(Iteration_stmt* cistmt):cistmt_(cistmt),stamp_(2)
{
	fake_line_=cistmt->get_fake_line();
}

Statement::Statement(Return_stmt* crs):crs_(crs),stamp_(3)
{	
	fake_line_=crs->get_fake_line();
}

Statement::Statement(Var* cvar,Expression* ce):cvar_(cvar),ce_(ce),stamp_(4)
{
	fake_line_=cvar->get_fake_line();
}

Statement::Statement(Expression* e):ce_(e),stamp_(5)
{
	fake_line_=e->get_fake_line();
}

Statement::Statement(string semi):semi_(semi),stamp_(6)
{
}

string Statement::get_file_info()
{
	return file_info_;
}

Compound_stmt* Statement:: get_ccstmt()
{
	return ccstmt_;
}

Selection_stmt* Statement:: get_csstmt()
{
	return csstmt_;
}

Iteration_stmt* Statement:: get_cistmt()
{
	return cistmt_;
}

Expression* Statement:: get_ce()
{
	return ce_;
}

Var* Statement:: get_cvar()
{
	return cvar_;
}

Return_stmt* Statement:: get_crs()
{
	return crs_;
}

string Statement:: get_semi()
{
	return semi_;
}	
	
int  Statement::get_stamp()
{
	return stamp_;
}		

int Statement::get_line_begin()
{
	return line_begin_;
}

int Statement::get_line_end()
{
	return line_end_;
}

int Statement::get_fake_line()
{
	return fake_line_;
}

void Statement::set_fake_line(int i)
{
	fake_line_=i;
}

void Statement::set_line_begin(int i)
{
	line_begin_=i;
}

void Statement::set_line_end(int i)
{
	line_end_=i;
}

void Statement::set_file_info(string s)
{
	file_info_=s;
}
/*---------------Statement------------end-----------*/	




/*----------------Compound_stmt-------begin----------*/		
Compound_stmt::Compound_stmt()
{
}

Compound_stmt::Compound_stmt(list<Declaration>* cdl,list<Statement >* csl)
{
	cdl_=new list<Declaration>;
	for(list<Declaration>::iterator iter=cdl->begin();iter!=cdl->end();iter++)
	{
		cdl_->push_back(*iter);
	}
	csl_=new list<Statement >;
	for(list<Statement>::iterator iter=csl->begin();iter!=csl->end();iter++)
	{
		csl_->push_back(*iter);
	}
}
	
list<Declaration>* Compound_stmt:: get_cdl()
{
	return cdl_;
}

list<Statement>* Compound_stmt:: get_csl()
{
	return csl_;
}	

int Compound_stmt::get_line_begin()
{
	return line_begin_;
}

int Compound_stmt::get_line_end()
{
	return line_end_;
}

int Compound_stmt::get_fake_line()
{
	return fake_line_;
}

void Compound_stmt::set_line_begin(int i)
{
	line_begin_=i;
}

void Compound_stmt::set_line_end(int i)
{
	line_end_=i;
}

void Compound_stmt::set_fake_line(int i)
{
	fake_line_=i;
}
/*----------------Compound_stmt-------end----------*/	




/*---------------Selection_stmt----------begin----------*/		
Selection_stmt::Selection_stmt():ce_(),cs_if_(),cs_else_(),stamp_(-1)
{
}
  
Selection_stmt::Selection_stmt(Expression* ce ,Statement* cs_if):ce_(ce),cs_if_(cs_if),stamp_(0)
{
}

Selection_stmt::Selection_stmt(Expression* ce  ,Statement* cs_if,Statement* cs_else):ce_(ce),cs_if_(cs_if),cs_else_(cs_else),stamp_(1)
{
}

Expression* Selection_stmt:: get_ce()
{
	return ce_;
}

Statement* Selection_stmt:: get_cs_if()
{
	return cs_if_;
}

Statement* Selection_stmt:: get_cs_else()
{
	return cs_else_;
}

int  Selection_stmt::get_stamp()
{
	return stamp_;
}

int  Selection_stmt::get_fake_line()
{
	return fake_line_;
}

void Selection_stmt::set_exp(Expression* exp1)
{
	ce_=exp1;
}

void Selection_stmt::set_cs_if(Statement* stmt)
{
	cs_if_=stmt;
}

void Selection_stmt::set_cs_else(Statement* stmt)
{
	cs_else_=stmt;
}

void Selection_stmt::set_fake_line(int i)
{
	fake_line_=i;
}

int Selection_stmt::get_line_begin()
{
	return line_begin_;
}

int Selection_stmt::get_line_end()
{
	return line_end_;
}

void Selection_stmt::set_line_begin(int i)
{
	line_begin_=i;
}

void Selection_stmt::set_line_end(int i)
{
	line_end_=i;
}
/*---------------Selection_stmt----------end----------*/




/*-----------------Iteration_stmt----------begin-----------*/		
Iteration_stmt::Iteration_stmt():ce_(),cs_()
{
}

Iteration_stmt::Iteration_stmt(Expression* ce,Statement* cs):ce_(ce),cs_(cs)
{
}

Expression* Iteration_stmt:: get_ce()
{
	return ce_;
}

Statement* Iteration_stmt:: get_cs()
{
	return cs_;
}

void Iteration_stmt::set_cs(Statement* stmt1)
{
	cs_=stmt1;
}

int Iteration_stmt::get_line_begin()
{
	return line_begin_;
}

int Iteration_stmt::get_line_end()
{
	return line_end_;
}

int Iteration_stmt::get_fake_line()
{
	return fake_line_;
}

void Iteration_stmt::set_fake_line(int i)
{
	fake_line_=i;
}

void Iteration_stmt::set_line_begin(int i)
{
	line_begin_=i;
}

void Iteration_stmt::set_line_end(int i)
{
	line_end_=i;
}
/*-----------------Iteration_stmt----------end-----------*/		




/*-------------------Return_stmt-----------begin----------*/		
Return_stmt::Return_stmt():stamp_(0)
{
}

Return_stmt::Return_stmt( Expression* exp):ce_(exp),stamp_(1)
{
}

Expression* Return_stmt::get_ce()
{
	return ce_;
}

int Return_stmt:: get_stamp()
{
	return stamp_;
}		

void Return_stmt::set_ce(Expression* exp1)
{
	ce_=exp1;
}

void Return_stmt::set_stamp(int n)
{
	stamp_=n;
}

int Return_stmt::get_line_begin()
{
	return line_begin_;
}

int Return_stmt::get_line_end()
{
	return line_end_;
}

int Return_stmt::get_fake_line()
{
	return fake_line_;
}

void Return_stmt::set_line_begin(int i)
{
	line_begin_=i;
}

void Return_stmt::set_line_end(int i)
{
	line_end_=i;
}

void Return_stmt::set_fake_line(int i)
{
	fake_line_=i;
}
/*-------------------Return_stmt-----------end----------*/




/*--------------------Expression------------begin-----------*/	
Expression::Expression():stamp_(-1)
{
}		

Expression::Expression(Expression* cel,string op,Expression *cer):cel_(cel),op_(op),cer_(cer),stamp_(0)
{
	fake_line_=cel->get_fake_line();
}

Expression::Expression(string op,Expression *cer):op_(op),cer_(cer),stamp_(1)
{
	fake_line_=cer->get_fake_line();
}

Expression::Expression(Expression* exp):exp_(exp),stamp_(2)
{
	fake_line_=exp->get_fake_line();
}

Expression::Expression(string c):c_(c),stamp_(3)
{
}

Expression::Expression(Var* cvar):cvar_(cvar),stamp_(4)
{
	fake_line_=cvar->get_fake_line();
}

Expression::Expression(string fun_name,list<Expression>* args):c_(fun_name),args_(args),stamp_(5)
{
}	


string Expression::get_op()
{
	return op_;
}

Expression* Expression::get_exp()
{
	return exp_;
}

Expression* Expression::get_cel()
{
	return cel_;
}

Expression* Expression::get_cer()
{
	return cer_;
}

string Expression::get_c()
{
	return c_;
}

Var* Expression::get_cvar()
{
	return cvar_;
}
		
list<Expression> * Expression::get_args()
{
	return args_;
}

int  Expression:: get_stamp()
{
	return stamp_;
}		

int Expression::get_line_begin()
{
	return line_begin_;
}

int Expression::get_line_end()
{
	return line_end_;
}

int Expression::get_fake_line()
{
	return fake_line_;
}

void Expression::set_fake_line(int i)
{
	fake_line_=i;
}

void Expression::set_line_begin(int i)
{
	line_begin_=i;
}

void Expression::set_line_end(int i)
{
	line_end_=i;
}

void Expression::set_cer(Expression* exp1)
{
	cer_=exp1;
}
		
void Expression::set_cel(Expression* exp1)
{
	cel_=exp1;
}

void Expression::set_cvar(Var* var1)
{
	cvar_=var1;
}

void Expression::set_c(string ch)
{
	c_=ch;
}

void Expression::set_stamp(int n )
{
	stamp_=n;
}

void Expression::set_op(string ch)
{
	op_=ch;
}

void Expression::set_exp(Expression* exp)
{
	exp_=exp;
}

void Expression::set_args(list<Expression>* exps)
{
	args_=exps;
}
/*--------------------Expression------------end-----------*/



/*--------------------Var--------------------begin----------*/
Var::Var():stamp_(-1)
{
}

Var::Var(string name,list<string> vartype):name_(name),store_var_type(vartype),stamp_(0)
{

}

Var::Var(string name,list<string> vartype,Expression* cexp):name_(name),stamp_(1)
{
	cexp_=cexp;
	store_var_type=vartype;
}


Var::Var(string  name,list<string> vartype,list<Expression>* cexp):name_(name),stamp_(2)
{
	exp_list=cexp;
	store_var_type=vartype;
}

Var::Var(Var* var ,string name,list<string> vartype):stamp_(3)
{
	var_=var;
	name_=name;
	store_var_type=vartype;
	
}

Var::Var(Var* var ,string name,list<string> vartype, list<Expression>* cexp):stamp_(4)
{
	var_=var;
	name_=name;
	exp_list=cexp;
	store_var_type=vartype;

}

Var* Var::get_var()
{
	return var_;
}

int Var:: get_stamp()
{
	return stamp_;
}

string Var:: get_name()
{
	return name_;
}

list<Expression>* Var::get_exp_list()
{
	return exp_list;
}

Expression* Var::get_cexp()
{
	return cexp_;
}

int Var::get_line_begin()
{
	return line_begin_;
}

int Var::get_line_end()
{
	return line_end_;
}

int Var::get_fake_line()
{
	return fake_line_;
}
list<string> Var::get_var_type()
{
	return store_var_type;
}
void Var::set_fake_line(int i)
{
	fake_line_=i;
}

void Var::set_line_begin(int i)
{
	line_begin_=i;
}

void Var::set_line_end(int i)
{
	line_end_=i;
}

void Var::set_cexp(Expression* exp1)
{
	cexp_=exp1;
}

void Var::set_stamp(int i)
{
	stamp_=i;
}
void Var::set_var_type(list<string> tlist)
{
	store_var_type=tlist;
}
/*--------------------Var------------------end----------*/	





/*-------------------Program--------------begin----------*/		

Program::Program()
{
}


Program::Program(list<Toke_num>* inclist,list<Declaration>* declist)
{
	list<Toke_num>* temp1 = new list<Toke_num>;
	temp1->insert(temp1->end(),inclist->begin(),inclist->end());
	inclist_=temp1;
	
	list<Declaration>* temp2 = new list<Declaration>;
	temp2->insert(temp2->end(),declist->begin(),declist->end());
	declist_=temp2;
}


list<Declaration>* Program:: get_declist()
{
	return declist_;
}

list<Toke_num>* Program::get_inclist()
{
	return inclist_;
}
/*-------------------Program-------------end----------*/		







/*---------------Cmnt-----------------begin*/
Cmnt::Cmnt()
{
}

Cmnt::Cmnt(string str_c,int l_b,int l_e)
{
	content_=str_c;
	line_begin_=l_b;
	line_end_=l_e;
	output_=false;
}

string Cmnt::get_content()
{
	return content_;
}

string Cmnt::output_cmnt(int indent_tab)
{
	char *new_str=new char[4*content_.size()];
	int str_i=0,put_i=0;
	int space_size=0,real_space=0;

	for(str_i=0;str_i<content_.size();)
	{
		if(is_next_line_start(str_i,content_) && (content_[str_i]==' ' || content_[str_i]=='\t'))
		{
			while(content_[str_i]==' ' ||content_[str_i]=='\t')
			{
				if(content_[str_i]==' ')
					space_size++;
				str_i++;
			}
			real_space=indent_tab*2+space_size%2;
			for(int space_i=0;space_i<real_space;space_i++)
				new_str[put_i++]=' ';
			space_size=0;
		}
		else
		{
			new_str[put_i++]=content_[str_i++];
		}
	}
	new_str[put_i]='\0';

	string ret_cmnt=string(new_str);
	delete new_str;
	return ret_cmnt;
}

bool Cmnt::is_next_line_start(int index,string &str)
{
	if(index)
		return str[index-1]=='\n';
	else
		return false;
}

int Cmnt::get_line_begin()
{
	return line_begin_;
}

int Cmnt::get_line_end()
{
	return line_end_;
}

bool Cmnt::get_output()
{
	return output_;
}

string Cmnt::get_file_info()
{
	return file_info_;
}

void Cmnt::set_file_info(string s)
{
	file_info_=s;
}

void Cmnt::set_output(bool b)
{
	output_=b;
}
/*---------------Cmnt-----------------end*/

/*-------------------Var_info-------------begin----------*/	
Var_info::Var_info()
{
}

Var_info::Var_info(string s,int i,string sp)
{
	stamp="common";
	name=s;
	vector<int>* v=new vector<int>;
	v->push_back(i);
	sigs=v;
	scope=sp;

}

Var_info::Var_info(const Var_info& p)
{
	name=p.name;
	vector<int>* v=new vector<int>;
	v=p.sigs;
	sigs=v;
	scope=p.scope;
	stamp=p.stamp;
	sigIndex=p.sigIndex;
}

string Var_info::get_name()
{
	return name;
}

vector<int>* Var_info::get_sigs()
{
	return sigs;
}

void Var_info::add_sigs(int i)
{
	sigs->push_back(i);
}

string Var_info::get_scope()
{
	return scope;
}

int Var_info::getSigIndex()
{
	return sigIndex;
}

void Var_info::set_sigIndex(int index)
{
	sigIndex=index;
}

string Var_info::get_stamp()
{
	return stamp;
}
		
void Var_info::set_stamp(string str)
{
	stamp=str;
	
}
/*-------------------Var_info-------------end----------*/	


/*-------------------Global_var-------------begin---------*/	

Global_var::Global_var()
{
}

Global_var::Global_var(string name,int sig)
{
	//使用此构造函数构造的肯定是数组附加变量
	var_name_=name;
	sig_=sig;
	stamp_=0;
	ex_=true;
	adj_stamp_="adj";
}

Global_var::Global_var(string name,int value,int sig,int stamp)
{
	var_name_=name;
	sig_=sig;
	if(stamp==1)
	{
		stamp_=1;
		var_value_=value;
	}
	if(stamp==2)
	{
		stamp_=2;
		sub_=value;
	}
	ex_=true;
	adj_stamp_="adj";
}

Global_var::Global_var(string name,int sub,int sig,vector<int>* v)
{
	var_name_=name;
	stamp_=3;
	sub_=sub;
	sub_value_=v;
	sig_=sig;
	ex_=false;//有初始化值，肯定不是外部变量
	adj_stamp_="adj";
}

int Global_var::get_stamp()
{
	return stamp_;
}

string Global_var::get_var_name()
{
	return var_name_;
}

int Global_var::get_var_value()
{
	return var_value_;
}

int Global_var::get_sub()
{
	return sub_;
}

int Global_var::get_sig()
{
	return sig_;	
}

bool Global_var::get_ex()
{
	return ex_;	
}

string Global_var::get_adj_stamp()
{
	return adj_stamp_;	
}

vector<int>* Global_var::get_sub_value()
{
	return sub_value_;
}

void Global_var::set_ex(bool flag)
{
	ex_=flag;	
}

void Global_var::set_adj_stamp(string stamp)
{
	adj_stamp_=stamp;
}

/*-------------------Global_var-------------end---------*/	

/*-------------------Fun_info-------------begin----------*/	
Fun_info::Fun_info()
{
}

Fun_info::Fun_info(string name)
{
	sig_=-1;
	name_=name;
	return_stamp_=1;
	para_stamp_=1;
	para_num_=-1;
	paras_=new list<string>;
}

string Fun_info::get_name()
{
	return name_;
}

int Fun_info::get_return_stamp()
{
	return return_stamp_;
}

int Fun_info::get_para_stamp()
{
	return para_stamp_;
}


int Fun_info::get_para_num()
{
	return para_num_;
}

list<string>* Fun_info::get_paras()
{
	return paras_;
}

int Fun_info::get_sig()
{
	return sig_;
}

void Fun_info::set_return_stamp(int i)
{
	return_stamp_=i;
}
	
void Fun_info::set_para_stamp(int i)
{
	para_stamp_=i;
}

void Fun_info::set_para_num(int i)
{
	para_num_=i;
}

void Fun_info::set_paras(list<string>* l)
{
	for(list<string>::iterator iter=l->begin();iter!=l->end();iter++)
	{
		paras_->push_back(*iter);
	}
	
}

void Fun_info::set_sig(int i)
{
	sig_=i;
}
/*-------------------Fun_info-------------end----------*/	

/*------------Bool_var--------------*/
Bool_var::Bool_var()
{
}

Bool_var::Bool_var(string name,string scope,int t,int f)
{
	name_=name;
	scope_=scope;
	t_=t;
	f_=f;
}

string Bool_var::get_name()
{
	return name_;
}

string Bool_var::get_scope()
{
	return scope_;
}

int Bool_var::get_t()
{
	return t_;
}

int Bool_var::get_f()
{
	return f_;
}
/*------------Bool_var--------------*/



