/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_VCP_YACC_H_INCLUDED
# define YY_YY_VCP_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    CONSTANT = 259,
    TYPE_NAME = 260,
    LEFT = 261,
    RIGHT = 262,
    LE = 263,
    GE = 264,
    EQ = 265,
    NE = 266,
    GT = 267,
    LT = 268,
    AND = 269,
    OR = 270,
    MUL = 271,
    DIV = 272,
    ADD = 273,
    SUB = 274,
    NOT = 275,
    BitNot = 276,
    EXTERN = 277,
    STATIC = 278,
    REGISTER = 279,
    AUTO = 280,
    BOOL = 281,
    INT = 282,
    VOID = 283,
    STRUCT = 284,
    ENUM = 285,
    TYPEDEF = 286,
    CHAR = 287,
    SIGNED = 288,
    UNSIGNED = 289,
    SHORT = 290,
    VOLATILE = 291,
    CONST = 292,
    LONG = 293,
    IF = 294,
    ELSE = 295,
    WHILE = 296,
    RETURN = 297,
    INCLUDE = 298,
    IFX = 299,
    UMINUS = 300
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 51 "vcp_yacc.y" /* yacc.c:1909  */

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

#line 148 "vcp_yacc.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_VCP_YACC_TAB_H_INCLUDED  */
