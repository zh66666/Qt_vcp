%{
#include "program_struct.h"
#include <vector>
#include <list>
#include <map>
#include <string>
//defined in file : ./parser_utils.c
extern list<string>* type_names;
extern Program *front_end_pro;
extern int struct_body;//2010年3月4日追加
extern void get_type_name(list<string>*, Var_declaration*);
extern void set_var_dec(Var_declaration* v_d);	
extern void yyerror(string err_info);
extern void check_var_dec(Var_declaration* v_d);
extern void check_array_sub(Expression* s);//2010年3月5日追加
extern void check_assign_stmt(Statement* s);//2010年3月9日追加
extern void check_logic_exp(Expression* s);//2010年3月9日追加
extern void check_unlogic_exp(Expression* s);//2010年3月9日追加
extern void check_inverse_exp(Expression* s);//2015追加
extern void check_enum_dec(string str);//2015追加
extern void check_subunsig_exp(Expression* sl,Expression* sr,map<string,string> v);//追加
extern void check_mixsign_exp(Expression* sl, Expression* sr,map<string,string> v);//追加
extern list <string> get_type_var(string name, map<string, string> v);//2015年1月追加 
extern void check_else_if(Statement* s);
extern void check_if_while(Statement* s);
//defined in file : src/main_utils.c
extern int line_num;
//2010年3月10日追加
extern string file_info;
extern int fake_line_num;
struct lines 
{
	int real_line;
	int fake_line;
} ;

static vector<string>* scope= new vector<string>;
static map<string,string> scope_name_type;

 //list<Type_specifier> type_s;
 //list<Type_specifier>::iterator types_s_lerator;

int yylex(void);
extern FILE* yyin;


%}


%union
{
	//string texts;
	char* text;
	list<string>* strings;
	struct lines* line;
	Program* program;
	
	Declaration* declaration;
	list<Declaration>* declaration_list;
	
	Var_declaration* var_declaration;
	Declaration_specifiers* declaration_specifiers;
	Type_specifier* type_specifier;
	list<Type_specifier>* type_specifiers;
	Init_declarator* init_declarator;
	list<Init_declarator>* init_declarators;
	Declarator* declarator;	
	
	Initializer* initializer;
	list<Initializer>* initializer_list;
	
	Struct_specifier* struct_specifier;
	
	list<Var_declaration>* var_declarations;
	
	Enum_specifier* enum_specifier;
	Enumerator* enumerator;
	list<Enumerator>* enumerator_list;
	
	Fun_declaration* fun_declaration;
	Param* param;
	list<Param>* params;
	
	Statement* statement;
	list<Statement>* statement_list;
	Compound_stmt* compound_stmt;
	Selection_stmt* selection_stmt;
	Iteration_stmt* iteration_stmt;
	Return_stmt* return_stmt;
	
	Expression* expression;
	list<Expression> * exps;
	Var* var;
	
	Toke_num* toke_num;
	list<Toke_num>* toke_nums;
};



%token  <text> IDENTIFIER   CONSTANT  TYPE_NAME
%token  LEFT RIGHT LE GE EQ NE GT LT AND OR MUL DIV ADD SUB  NOT BitNot
%token  EXTERN STATIC   REGISTER AUTO 
%token  BOOL  INT  VOID STRUCT ENUM TYPEDEF CHAR SIGNED UNSIGNED SHORT VOLATILE CONST LONG

%token  IF ELSE  WHILE RETURN
%token	<text> INCLUDE 

%type <text>  storage_class_specifier
%type <program> program
%type <line> struct_ enum if while return
%type <toke_nums> include_list
%type <declaration_list> declaration_list  
%type <declaration> declaration 
%type <var_declaration> var_declaration 
%type <declaration_specifiers> declaration_specifiers
%type <type_specifier> type_specifier
%type <type_specifiers> type_specifier_list
%type <init_declarator> init_declarator
%type <init_declarators> init_declarator_list 
%type <declarator> declarator
%type <fun_declaration> fun_declaration
%type <initializer> initializer
%type <initializer_list> initializer_list

%type <param> param
%type <params>  params param_list
%type <compound_stmt> compound_stmt
%type <statement_list> statement_list
%type <statement> statement
%type <selection_stmt> selection_stmt
%type <iteration_stmt> iteration_stmt
%type <return_stmt> return_stmt
%type <expression> expression call
%type <exps> arg_list args expression_list
%type <var> var
%type <struct_specifier> struct_specifier
%type <var_declarations> struct_body
%type <enumerator> enumerator
%type <enumerator_list> enumerator_list
%type <enum_specifier> enum_specifier
%type <toke_num> identifier include

%nonassoc IFX
%nonassoc ELSE
%left OR
%left AND
%left EQ NE
%left GE LE GT LT
%left LEFT RIGHT
%left ADD SUB
%left MUL DIV
%nonassoc UMINUS
%right NOT BitNot
%start program



%%
program
	:include_list declaration_list
	{  
		Program * tmp=new Program($1,$2);
		front_end_pro=tmp;
		$$=tmp;
		
		delete $1;
		delete $2;
	}
	;
			
include_list
 	:include_list include
    { 
        list<Toke_num>* tmp = $1;
        tmp->push_back(*$2);
        delete $2;
		   
		$$ = tmp;
	}
	|empty
 	{
 		list<Toke_num>* tmp=new list<Toke_num>;
 		
 		$$=tmp;
 	}
  	;

include
 	:INCLUDE  
  	{ 
  		Toke_num* tmp=new Toke_num($1,line_num);
		
		$$=tmp;
  	}
	;
	
declaration_list
	:declaration_list declaration  
	{
		list<Declaration>* tmp=$1;
		tmp->push_back(*$2);
		delete $2;
		
		$$=tmp;
	}
	|declaration      
	{ 
		list<Declaration>* tmp=new list<Declaration>;
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
	}
	;

declaration	
	:var_declaration
	{
		if(scope->size()!=0&&scope->back()=="if")
		{
			yyerror("can not have variable declaration in selection structure!");
		}
		if(scope->size()!=0&&scope->back()=="while")
		{
			yyerror("can not have variable declaration in while structure!");
		}
		
		Declaration* tmp=new Declaration($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	
	|fun_declaration
	{
		if(scope->size()!=0&&scope->back()=="if")
		{
			yyerror("can not have function declaration in selection structure!");
		}
		if(scope->size()!=0&&scope->back()=="while")
		{
			yyerror("can not have function declaration in while structure!");
		}
		
		Declaration* tmp=new Declaration($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	;
	
var_declaration	
	: declaration_specifiers ';' 
	{   
		Var_declaration *tmp=new Var_declaration($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		check_var_dec(tmp);
		set_var_dec(tmp);
	 	$$=tmp;
	}
	| declaration_specifiers init_declarator_list ';'  
	{	
		Var_declaration *tmp=new Var_declaration($1,$2);
		
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		check_var_dec(tmp);
		set_var_dec(tmp);
		get_type_name(type_names,tmp);
	
//遍历变量declrator的类型链表，使用全局map来构建变量和变量类型的映射
	//闪闪 追加
	list<Type_specifier>* type_s;
	list<Type_specifier>::iterator types_s_lerator;
	list<Init_declarator>::iterator ini_dec_lerator;
	string tynam;

	    type_s=$1->get_type_spec_list();

		for(ini_dec_lerator=$2->begin();ini_dec_lerator!=$2->end();ini_dec_lerator++){
		//遍历同一类型各变量
		//获取变量名
		string nam=ini_dec_lerator->get_dec()->get_name();
//scope_name_type保存每个变量的类型
	 for(types_s_lerator=type_s->begin();types_s_lerator!=type_s->end();types_s_lerator++) {
		 tynam=types_s_lerator->get_type_name();
		 scope_name_type.insert(pair<string,string>(nam,tynam));}
		 }

	 	$$=tmp;
	}
	;

declaration_specifiers 
 	: storage_class_specifier type_specifier_list
 	{
 		Declaration_specifiers* tmp=new Declaration_specifiers($1,$2);
 		tmp->set_line_begin($2->front().get_line_begin());
		tmp->set_line_end(line_num);

 		$$=tmp;
 	} 
 	;  
 
storage_class_specifier
	: TYPEDEF
	{	 
	   char typ[]="typedef";
		$$=typ;
	}
	| EXTERN   
	{
	char ext[]="extern"; 
		$$=ext;
	}
	| STATIC
	{ 
	 char sta[]="static";
		$$=sta;
	}
	| AUTO
	{
	  
	  char aut[]="auto";
		$$=aut;
	}
	|REGISTER
	{
	  
	  char reg[]="register";
		$$=reg;
	}
	| empty
	{ 
	 char emp[]="empty";
		$$=emp;
	}
	;


type_specifier	
	: INT
	{
		Type_specifier* tmp=new Type_specifier("int",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		$$=tmp;
	}       
	| BOOL 
	{
		Type_specifier* tmp=new Type_specifier("bool",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}     
	| CHAR
	{
		Type_specifier* tmp=new Type_specifier("char",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}   
	| SHORT
	{
		Type_specifier* tmp=new Type_specifier("short",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}   
	| LONG
	{
		Type_specifier* tmp=new Type_specifier("long",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}   
	| SIGNED
	{
		Type_specifier* tmp=new Type_specifier("signed",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}   
	| UNSIGNED
	{
		Type_specifier* tmp=new Type_specifier("unsigned",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}   
	| CONST
	{
		Type_specifier* tmp=new Type_specifier("const",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}   
	| VOLATILE
	{
		Type_specifier* tmp=new Type_specifier("volatile",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		$$=tmp;
	}   
	| VOID
	{
		Type_specifier* tmp=new Type_specifier("void",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		$$=tmp;
	}   
	| TYPE_NAME
	{
		Type_specifier* tmp=new Type_specifier($1,1);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		$$=tmp;
	}   
	| struct_specifier
	{
		Type_specifier* tmp=new Type_specifier($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}   
	| enum_specifier
	{
		Type_specifier* tmp=new Type_specifier($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}   
	;
	
init_declarator_list 
 	: init_declarator 
 	{
 		list<Init_declarator>* tmp=new list<Init_declarator>;
 		tmp->push_back(*$1);
 		delete $1;
 		
 		$$=tmp;
 		
 	}
 	| init_declarator_list ',' init_declarator 
 	{
 		list<Init_declarator>* tmp=$1;
 		tmp->push_back(*$3);
 		delete $3;
 		
 		$$=tmp;
 		
 	}
	;  
 
init_declarator 
	: declarator 
	{
		if(scope->size()==0 && $1->get_name().size()>112)
		{
			yyerror("global variable's name is too long!");
		}
		
		Init_declarator* tmp=new Init_declarator($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
		
	}
 	| declarator '=' initializer 
 	{
		if(scope->size()==0 && $1->get_name().size()>112)
		{
			yyerror("global variable's name is too long!");
		}
		
		Init_declarator* tmp=new Init_declarator($1,$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
		
	}
	; 

declarator
	: identifier
	{
		Declarator* tmp=new Declarator($1->get_toke());
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	| identifier expression_list
	{
		Declarator* tmp=new Declarator($1->get_toke(),$2);
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	| identifier '[' ']'
	{
		yyerror("invalid array subscript!");
	}
	;
	

initializer
	: expression
	{
		Initializer* tmp=new Initializer($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
		
	}
	| '{' initializer_list '}'
	{
		Initializer* tmp=new Initializer($2);
		tmp->set_line_begin($2->front().get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	;

initializer_list
	: initializer
	{
		list<Initializer>* tmp=new list<Initializer>;
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
		
	}
	| initializer_list ',' initializer
	{
		list<Initializer>* tmp=$1;
		tmp->push_back(*$3);
		delete $3;
		
		$$=tmp;
	}
	;
	

struct_specifier
	: struct_ identifier  '{' struct_body '}'
	{
		Struct_specifier* tmp=new Struct_specifier($2->get_toke(),$4);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		
		struct_body--;//2010年3月4日追加
		
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
		
	}
	| struct_ '{' struct_body '}'
	{
		yyerror("struct specifier must have a name!");
	}
	| struct_ identifier
	{
		Struct_specifier* tmp=new Struct_specifier($2->get_toke());
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		
		struct_body--;//2010年3月4日追加
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
	}
	;

struct_body
	: struct_body var_declaration
	{
		list<Var_declaration>* tmp=$1;
		tmp->push_back(*$2);
		delete $2;
		
		$$=tmp;
		
	}
	| var_declaration
	{
		list<Var_declaration>* tmp=new list<Var_declaration>;
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
	}
	;

struct_
	: STRUCT
	{
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;

		struct_body++;//2010年3月4日追加
		$$=tmp;
	}
	;

type_specifier_list 
 	: type_specifier
 	{
 		list<Type_specifier>* tmp=new list<Type_specifier>;
 		tmp->push_back(*$1);
 		delete $1;
 		
 		$$=tmp;
 	} 
 	| type_specifier_list type_specifier 
 	{
 		list<Type_specifier>* tmp=$1;
 		tmp->push_back(*$2);
 		delete $2;
 		
 		$$=tmp;
 	} 
 	; 
 	
enum_specifier
	: enum '{' enumerator_list '}'
	{
		Enum_specifier* tmp=new Enum_specifier($3);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line($1->fake_line);
		$$=tmp;
		
	}
	| enum identifier '{' enumerator_list '}'
	{
		Enum_specifier* tmp=new Enum_specifier($2->get_toke(),$4);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line($1->fake_line);
		$$=tmp;
		
	}
	| enum identifier
	{
		Enum_specifier* tmp=new Enum_specifier($2->get_toke());
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line($1->fake_line);
		$$=tmp;
	}
	;

enum
	: ENUM
	{
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		$$=tmp;
	}
	;
enumerator_list
	: enumerator
	{
		list<Enumerator>* tmp=new list<Enumerator>;
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
	
	}
	| enumerator_list ',' enumerator
	{
		list<Enumerator>* tmp=$1;
		tmp->push_back(*$3);
		delete $3;
		
		$$=tmp;
	}
	;
	
enumerator
	: identifier
	{
		Enumerator* tmp=new Enumerator($1->get_toke());
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		//   2015/3/19 闪闪追加		
		check_enum_dec($1->get_toke());		   		 
		scope_name_type.insert(pair<string,string>($1->get_toke(),"ENUM")); 
		//追加结束		
		$$=tmp;
	}
	| identifier '=' expression
	{
		Enumerator* tmp=new Enumerator($1->get_toke(),$3);
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		
		//   2015/3/19 闪闪追加		 
		scope_name_type.insert(pair<string,string>($1->get_toke(),"ENUM"));
		//追加结束
		
		
		$$=tmp;
	}
	;
	
fun_declaration
	: declaration_specifiers identifier  '('params')'  compound_stmt  
	{	
		Fun_declaration* tmp=new Fun_declaration($1,$2->get_toke(),$4,$6);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		$$=tmp;
		scope->pop_back();
	}
	| declaration_specifiers identifier '('params')'  ';'    
	{	
		Fun_declaration* tmp=new Fun_declaration($1,$2->get_toke(),$4);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		$$=tmp;
		scope->pop_back();
	}
	;
	
params
	: param_list  
	{	
		list<Param>* tmp=$1;
		
		$$=tmp;
		scope->push_back("fun");
	}
	| empty      
	{ 
		list<Param>* tmp=new list<Param>;
		
		$$=tmp;
		scope->push_back("fun");
	}
	;

param_list
	: param_list ','  param
	{	
		list<Param>* tmp=$1;
		tmp->push_back(*$3);
		delete $3;
		
		$$=tmp;
	
	}
	| param                
	{ 
		list<Param>* tmp=new list<Param>;
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
	}
	;
param
	: type_specifier_list declarator   
	{	
		if($1->front().get_stamp()==2  )
		{
			yyerror("function 's parameter should not be struct type!");
		}
		if($1->front().get_stamp()==3)
		{
			yyerror("function 's parameter should not be enum type!");
		}
		
		if($2->get_stamp()==1)
		{
			yyerror("function 's parameter should not be array!");
		}
		
		Param* tmp=new Param($1,$2);
		tmp->set_line_begin($1->front().get_line_begin());
		tmp->set_line_end(line_num);
		
		//   2015/3/14 闪闪追加
		list<Type_specifier>* type_ss;
		list<Type_specifier>::iterator types_ss_lerator; 
		string tynams;
		string namss=$2->get_name(); 	      
	 	for(types_ss_lerator=$1->begin();types_ss_lerator!=$1->end();types_ss_lerator++) {
		 	tynams=types_ss_lerator->get_type_name();
		 	scope_name_type.insert(pair<string,string>(namss,tynams));
		 	}
		//追加结束				
		$$=tmp; 
	}               
	| type_specifier_list         
	{	
		if($1->front().get_stamp()==2  )
		{
			yyerror("function 's parameter should not be struct type!");
		}
		if($1->front().get_stamp()==3)
		{
			yyerror("function 's parameter should not be enum type!");
		}
		
		Param* tmp=new Param($1);
		tmp->set_line_begin($1->front().get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}       
	;


compound_stmt	
	: '{' declaration_list statement_list '}'  
	{	
		Compound_stmt *tmp=new Compound_stmt($2,$3);
		tmp->set_line_begin($2->front().get_line_begin());
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
		
	}
	|'{' statement_list '}'  
	{	
		list<Declaration>* tmp1=new list<Declaration>;
		Compound_stmt *tmp=new Compound_stmt(tmp1,$2);
		tmp->set_line_begin($2->front().get_line_begin());
		tmp->set_line_end(line_num);	
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		$$=tmp;
	}
	| '{' '}'
	{
		list<Declaration>* tmp1=new list<Declaration>;
		list<Statement>* tmp2=new list<Statement>;
		Compound_stmt *tmp=new Compound_stmt(tmp1,tmp2);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}
	
	| '{' declaration_list '}'
	{
		list<Statement>* tmp2=new list<Statement>;
		Compound_stmt *tmp=new Compound_stmt($2,tmp2);
		tmp->set_line_begin($2->front().get_line_begin());
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		$$=tmp;
	}
	;

empty
	:
	;



statement_list
	: statement_list statement 
	{  
		list<Statement>* tmp=$1; 
		tmp->push_back(*$2);
		delete $2;
		
		$$=tmp;
	}
	| statement
	{
		list<Statement>* tmp=new list<Statement>; 
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
	}
	;

statement
	: ';'                           
	{	 
		Statement *tmp=new Statement(";");
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
      	$$=tmp;
    }
    | expression ';'      
	{	
		Statement *tmp=new Statement($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
	 	$$=tmp;
	}
	| var '=' expression ';'     
	{	
		Statement *tmp=new Statement($1,$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_assign_stmt(tmp);
		
	 	$$=tmp;
	 		 	
	}
	| compound_stmt    
	{	
		Statement *tmp=new Statement($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	| selection_stmt     
	{	
		Statement *tmp=new Statement($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	| iteration_stmt  
	{ 	
		Statement *tmp=new Statement($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	| return_stmt      
	{	
		Statement *tmp=new Statement($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	;



selection_stmt
	: if '(' expression ')' statement %prec IFX   			
	{	
		check_if_while($5);
		Selection_stmt *tmp=new Selection_stmt($3,$5);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
		scope->pop_back();
	}
	| if '(' expression ')' statement ELSE statement  
	{	
		check_if_while($5);
		check_else_if($7);
		Selection_stmt *tmp=new Selection_stmt($3,$5,$7);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
		scope->pop_back();
	}
	;

if
	: IF
	{
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		$$=tmp;
		scope->push_back("if");
	}
	;
	
iteration_stmt
	: while '('expression')' statement  
	{	
		check_if_while($5);
		Iteration_stmt *tmp=new Iteration_stmt($3,$5);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
		scope->pop_back();
	}
	;

while
	: WHILE
	{
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		$$=tmp;
		scope->push_back("while");
	}
	;
	
return_stmt
	: return ';'                        
	{	
		Return_stmt *tmp=new Return_stmt();
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
	}
	| return expression ';'  
	{	
		Return_stmt *tmp=new Return_stmt($2);
		tmp->set_line_begin($1->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->fake_line);//2010年3月10日追加
		$$=tmp;
	}
	;
	
return
	: RETURN
	{
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		$$=tmp;
	}
	;
	
expression
	: NOT expression							
	{
		Expression *tmp=new Expression("!",$2);
		tmp->set_line_begin($2->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_logic_exp(tmp);
		
		$$=tmp;
	}	
	| BitNot expression
	{
	
		//2015年1月阮闪闪追加
		Expression *tmp=new Expression("~",$2);
		tmp->set_line_begin($2->get_line_begin());
		tmp->set_line_end(line_num);			
		//check_logic_exp(tmp);
		check_inverse_exp(tmp);//3月追加 				 	
		$$=tmp;	
		
	
	}
	| SUB CONSTANT   %prec UMINUS 
	{
		string str="-";
		str=str+$2;
		Expression *tmp=new Expression(str);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
		
		$$=tmp;
	}
	| ADD CONSTANT  
	{
		Expression *tmp=new Expression($2);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
		$$=tmp;
	}
	| expression  ADD  expression		
	{
		Expression *tmp=new Expression($1,"+",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);		
		//有无符号混合运算检查
		//闪闪追加
        check_mixsign_exp($1,$3,scope_name_type);
		$$=tmp;
	}
	| expression  SUB expression			
	{
		Expression *tmp=new Expression($1,"-",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		//减法中不允许无符号运算
		//闪闪追加
		check_subunsig_exp($1,$3,scope_name_type);
		
		$$=tmp;
	}
	| expression  MUL  expression		
	{
		Expression *tmp=new Expression($1,"*",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		//有无符号混合运算检查
		//闪闪追加
		check_mixsign_exp($1,$3,scope_name_type);
		
		$$=tmp;
	}
	| expression  DIV  expression		
	{
		Expression *tmp=new Expression($1,"/",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		//有无符号混合运算检查
		//闪闪追加
		check_mixsign_exp($1,$3,scope_name_type);
		
		$$=tmp;
	}					
	
	| expression LT expression				
	{
		Expression *tmp=new Expression($1,"<",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression GT expression			
	{
		Expression *tmp=new Expression($1,">",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression GE expression			
	{
		Expression *tmp=new Expression($1,">=",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression LE expression				
	{
		Expression *tmp=new Expression($1,"<=",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression NE expression 			
	{
		Expression *tmp=new Expression($1,"!=",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression EQ expression			
	{
		Expression *tmp=new Expression($1,"==",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression AND expression			
	{
		Expression *tmp=new Expression($1,"&&",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_logic_exp($1);
		check_logic_exp($3);
		
		$$=tmp;
	}
	| expression OR expression   	 		
	{
		Expression *tmp=new Expression($1,"||",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_logic_exp($1);
		check_logic_exp($3);
		
		$$=tmp;
	}
	| expression LEFT expression    		
	{
		Expression *tmp=new Expression($1,"<<",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}
	| expression RIGHT expression     
	{
		Expression *tmp=new Expression($1,">>",$3);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp($1);
		check_unlogic_exp($3);
		
		$$=tmp;
	}	
    | '(' expression ')'   							
    {
     	Expression *tmp=new Expression($2);
     	tmp->set_line_begin($2->get_line_begin());
		tmp->set_line_end(line_num);
		
     	$$=tmp;
    }
   	| CONSTANT   							
    {
     	Expression *tmp=new Expression($1);
     	tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
     	$$=tmp;
    }
	| var    											
	{
		Expression *tmp=new Expression($1);
		tmp->set_line_begin($1->get_line_begin());
		tmp->set_line_end(line_num);
		
		$$=tmp;
	}
	| call    	
	{
		$$=$1;
	}
	;

var
	: identifier   
	{	
		 
		list<string> l1=get_type_var($1->get_toke(),scope_name_type);
		Var *tmp =new Var($1->get_toke(),l1);//2015年1月
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->get_fake_line());//2010年3月10日追加
		 
		
		$$=tmp;
	}
				
	| identifier expression_list
	{
	 
		list<string> l2=get_type_var($1->get_toke(),scope_name_type);
		Var* tmp=new Var($1->get_toke(),l2,$2);//2015年1月
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->get_fake_line());//2010年3月10日追加
		
	 
		
		$$=tmp;
			
	}
	| identifier '.' var
	{ 
	
		list<string> l3=get_type_var($1->get_toke(),scope_name_type);
		Var* tmp=new Var($3,$1->get_toke(),l3);//2015年1月
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->get_fake_line());//2010年3月10日追加
		
		 
		
		$$=tmp;
	}
	| identifier expression_list '.' var  
	{ 
	
		list<string> l4=get_type_var($1->get_toke(),scope_name_type);
		Var* tmp=new Var($4,$1->get_toke(),l4,$2);//2015年1月
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->get_fake_line());//2010年3月10日追加
		
		 
		
		$$=tmp;
	}
	;

expression_list
	: expression_list '['expression']'
	{
		list<Expression> *tmp=$1;
		tmp->push_back(*$3);
		delete $3;
		
		$$=tmp;
	}
	| '['expression']'
	{
		list<Expression> *tmp=new list<Expression>;
		tmp->push_back(*$2);
		
		//2010年3月5日追加
		check_array_sub($2);
		
		delete $2;
		
		$$=tmp;
	}
	;

call	
	: identifier '('args')' 
	{
		Expression *tmp=new Expression($1->get_toke(),$3);
		tmp->set_line_begin($1->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line($1->get_fake_line());//2010年3月10日追加
		$$=tmp;
	}
	;

args
	: arg_list   
	{
		$$=$1;
	}
	| empty 
	{
		list<Expression> *tmp=new list<Expression>;
		
		$$=tmp;
	}
	;

arg_list
	: arg_list ',' expression
	{
		list<Expression> *tmp=$1;
		tmp->push_back(*$3);
		delete $3;
		
		$$=tmp;
	}
		
	| expression	
	{
		list<Expression> *tmp=new list<Expression>;
		tmp->push_back(*$1);
		delete $1;
		
		$$=tmp;
	}
	;

identifier
	: IDENTIFIER
	{
		Toke_num* tmp=new Toke_num($1,line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
		$$=tmp;
	}
	;
%%






