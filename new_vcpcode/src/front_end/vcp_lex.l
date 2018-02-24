D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS          ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U))

%{

#include "program_struct.h"
#include "vcp_yacc.h"

static string str_tmp="";//store the content of the comment
static int line_begin=0;//store the beginning line of the comment
static int line_end=0;//store the ending line of the comment

//2010年3月10日追加
static bool line=false;//一个开关，标记当前是否位于#line后面

//defined in file : ./parser_utils.c
extern bool enter_noparse;
extern list<string>* type_names;
extern void store_cmnts(Cmnt*);				
extern int check_type(string,list<string>*);	
extern void yyerror(string err_info);

//defined in file : src/main_utils.c
extern int line_num;
extern list<Cmnt>* cmnts;
extern int str2int(string);
extern string int2str(int);

//2010年3月10日追加
extern string file_info;
extern int fake_line_num;

#define TRANSFER  char* dup_str = strdup(yytext); yylval.text = dup_str;
%}


%x CMNT
%x NOPARSE


%%
<NOPARSE>"/*KWNOPARSE*/"	{ 	BEGIN INITIAL;
								enter_noparse=false;
								str_tmp+="/*KWNOPARSE*/";
								str_tmp="/*KWNOPARSE*/"+str_tmp;
								line_begin=cmnts->back().get_line_begin();
								line_end=fake_line_num;
								Cmnt* cmnt=new Cmnt(str_tmp,line_begin,line_end);
								line_begin=0;
								line_end=0;
								cmnts->pop_back();
								cmnts->push_back(*cmnt);str_tmp="";
							}
<NOPARSE>"#line "[^\n]* {}
<NOPARSE>\n					{line_num++;fake_line_num++;str_tmp+="\n";}

<NOPARSE>.					{str_tmp+=(string)yytext;}




"/*"			{line_begin=fake_line_num;str_tmp+="/*";BEGIN CMNT;}

<CMNT>.			{str_tmp+=(string)yytext;}

<CMNT>\n		{line_num++;fake_line_num++;str_tmp+="\n";}

<CMNT>"*/"		{	line_end=fake_line_num;
					BEGIN INITIAL;
					str_tmp+="*/";
					Cmnt* cmnt=new Cmnt(str_tmp,line_begin,line_end);

					line_begin=0;line_end=0;
					store_cmnts(cmnt);
					str_tmp="";
					if(enter_noparse==true)
					{
						BEGIN NOPARSE;
					}
				}



"//".*      {yyerror("\"//\" is not supported!"); }



"#include"[ \t]*"<"[^ \t\n]*".h>"  	{TRANSFER   return(INCLUDE);}
"#include"[ \t]*"\""[^ \t\n]*".h\"" {TRANSFER   return(INCLUDE);}


"#line"			{line=true;}


"auto"			{ return(AUTO); }
"extern"		{ return(EXTERN); }
"static"		{ cout<<"warning : using \"static\" keyword! on line "<<line_num<<" of file : "<<file_info<<endl; return(STATIC);}
"typedef"		{ return(TYPEDEF); }
"register"		{ cout<<"warning : using \"register\" keyword! on line "<<line_num<<" of file : "<<file_info<<endl; return(REGISTER); }

"volatile"		{cout<<"warning : using \"volatile\" keyword! on line "<<line_num<<" of file : "<<file_info<<endl; return(VOLATILE);  }
"const"		{ cout<<"warning : using \"const\" keyword! on line "<<line_num<<" of file : "<<file_info<<endl; return(CONST);  }
"bool"			{ return(BOOL); }
"char"			{ yyerror("keyword char is not supported!"); }
"enum"			{ return(ENUM); }
"int"			{ return(INT); }
"long"			{ return(LONG); }
"short"			{yyerror("keyword short is not supported!");  }
"unsigned"		{ return(UNSIGNED); }
"signed"		{ return(SIGNED); }
"struct"		{ return(STRUCT); }
"void"			{ return(VOID); }

"if"			{ return(IF); }
"else"			{ return(ELSE); }

"while"			{ return(WHILE); }

"return"		{ return(RETURN); }



"inline"			{ yyerror("keyword inline is not supported!"); }
"restrict"		{ yyerror("keyword restrict is not supported!"); }
"sizeof"			{ yyerror("keyword sizeof is not supported!"); }
"switch"			{ yyerror("keyword switch is not supported!"); }
"union"			{ yyerror("keyword union is not supported!"); }
"break"			{ yyerror("keyword break is not supported!"); }
"case"			{ yyerror("keyword case is not supported!"); }
"float"			{ yyerror("keyword float is not supported!"); }
"continue"		{ yyerror("keyword continue is not supported!"); }
"default"		{ yyerror("keyword default is not supported!"); }
"do"				{ yyerror("keyword do is not supported!"); }
"double"			{ yyerror("keyword double is not supported!"); }
"goto"			{ yyerror("keyword goto is not supported!"); }
"for"				{ yyerror("keyword for is not supported!"); }
"label:"             { yyerror("keyword label is not supported!"); }



{L}({L}|{D})*						{ TRANSFER; return(check_type(yytext,type_names)); }

0[xX]{H}+{IS}?					{ string temp=yytext; int temp1=str2int(temp);yylval.text=(char*)(int2str(temp1).c_str()); return(CONSTANT); }
0{D}+{IS}?							{ string temp=yytext; int temp1=str2int(temp);yylval.text=(char*)(int2str(temp1).c_str()); return(CONSTANT);}


{D}+{IS}?							{if(true==line){string temp=yytext; line_num=str2int(temp);line_num--;}else{TRANSFER; return(CONSTANT);} }

L?'(\\.|[^\\'\n])+'			{ yyerror("character is not supported!");  }
{D}+{E}{FS}?						{ TRANSFER; return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ yyerror("decimal is not supported!"); }
{D}+"."{D}*({E})?{FS}?	{ yyerror("decimal is not supported!"); }


"-"											{  return(SUB); }
"+"											{  return(ADD); }
"*"											{  return(MUL); }
"/"											{  return(DIV); }
"<="										{  return(LE); }
">="										{  return(GE); }
"=="										{  return(EQ); }
"!="											{  return(NE); }
"<"											{  return(LT); }
">"											{  return(GT); }
">>"										{  return(RIGHT); }
"<<"										{  return(LEFT); }
"!"											{  return(NOT); }
"&&"										{  return(AND); }
"||"											{  return(OR); }
"="											{  return('='); }
";"											{  return(';'); }
("{"|"<%")							{  return('{'); }
("}"|"%>")							{  return('}'); }
","											{  return(','); }
"("											{  return('('); }
")"											{  return(')'); }
("["|"<:")								{  return('['); }
("]"|":>")								{  return(']'); }
"."											{  return('.'); }
"~"										{ return(BitNot); }


"^"										{  yyerror("\'^\' is not supported!"); }
"|"										{  yyerror("\'|\' is not supported!"); }
"?"										{  yyerror("\'\?\' is not supported!"); }
"%"									{  yyerror("\'%\' is not supported!"); }
":"										{  yyerror("位域\':\' is not supported!"); }
"..."										{  yyerror("\'...\' is not supported!"); }
">>="									{  yyerror("\'>>=\' is not supported!"); }
"<<="									{  yyerror("\'<<=\' is not supported!"); }
"+="									{  yyerror("\'+=\' is not supported!"); }
"-="									{  yyerror("\'-=\' is not supported!"); }
"*="									{  yyerror("\'*=\' is not supported!"); }
"/="									{  yyerror("\'/=\' is not supported!"); }
"%="									{  yyerror("\'%=\' is not supported!"); }
"&="									{  yyerror("\'&=\' is not supported!"); }
"^="									{  yyerror("\'^=\' is not supported!"); }
"|="									{  yyerror("\'|=\' is not supported!"); }
"++"									{  yyerror("\'++\' is not supported!"); }
"--"										{  yyerror("\'--\' is not supported!"); }
"->"									{  yyerror("\'->\' is not supported!"); } 
L?\"(\\.|[^\\"\n])*\"	{  if(true==line){file_info=yytext;line=false;}else{yyerror("string is not supported!");} }

[ \t\v\f]		{;}
[\r]			|
[\n]			|
[\r]+[\n]							{ line_num++;fake_line_num++;}
.											{  yyerror("invalid characters!"); }

%%

int yywrap(void)
{
	return 1;
}







