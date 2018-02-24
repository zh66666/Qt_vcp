/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "vcp_yacc.y" /* yacc.c:339  */

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



#line 114 "vcp_yacc.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "vcp_yacc.hpp".  */
#ifndef YY_YY_VCP_YACC_HPP_INCLUDED
# define YY_YY_VCP_YACC_HPP_INCLUDED
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

union YYSTYPE
{
#line 51 "vcp_yacc.y" /* yacc.c:355  */

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

#line 248 "vcp_yacc.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_VCP_YACC_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 265 "vcp_yacc.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   567

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  200

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      53,    54,     2,     2,    47,     2,    55,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    46,
       2,    48,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    49,     2,    50,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,     2,    52,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   161,   161,   173,   181,   190,   199,   207,   218,   236,
     256,   269,   307,   318,   323,   328,   333,   339,   345,   354,
     363,   373,   383,   393,   403,   413,   423,   433,   442,   451,
     460,   468,   479,   488,   500,   514,   531,   539,   547,   555,
     564,   575,   584,   596,   608,   612,   625,   634,   645,   658,
     666,   677,   687,   697,   709,   720,   729,   740,   751,   767,
     779,   794,   801,   811,   820,   830,   862,   883,   894,   904,
     917,   930,   937,   945,   956,   966,   974,   986,   994,  1002,
    1010,  1023,  1033,  1047,  1060,  1073,  1086,  1094,  1105,  1117,
    1128,  1141,  1152,  1160,  1174,  1189,  1204,  1220,  1232,  1244,
    1256,  1268,  1280,  1292,  1304,  1316,  1328,  1340,  1348,  1356,
    1364,  1371,  1384,  1398,  1411,  1427,  1435,  1450,  1461,  1465,
    1474,  1483,  1494
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CONSTANT", "TYPE_NAME",
  "LEFT", "RIGHT", "LE", "GE", "EQ", "NE", "GT", "LT", "AND", "OR", "MUL",
  "DIV", "ADD", "SUB", "NOT", "BitNot", "EXTERN", "STATIC", "REGISTER",
  "AUTO", "BOOL", "INT", "VOID", "STRUCT", "ENUM", "TYPEDEF", "CHAR",
  "SIGNED", "UNSIGNED", "SHORT", "VOLATILE", "CONST", "LONG", "IF", "ELSE",
  "WHILE", "RETURN", "INCLUDE", "IFX", "UMINUS", "';'", "','", "'='",
  "'['", "']'", "'{'", "'}'", "'('", "')'", "'.'", "$accept", "program",
  "include_list", "include", "declaration_list", "declaration",
  "var_declaration", "declaration_specifiers", "storage_class_specifier",
  "type_specifier", "init_declarator_list", "init_declarator",
  "declarator", "initializer", "initializer_list", "struct_specifier",
  "struct_body", "struct_", "type_specifier_list", "enum_specifier",
  "enum", "enumerator_list", "enumerator", "fun_declaration", "params",
  "param_list", "param", "compound_stmt", "empty", "statement_list",
  "statement", "selection_stmt", "if", "iteration_stmt", "while",
  "return_stmt", "return", "expression", "var", "expression_list", "call",
  "args", "arg_list", "identifier", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    59,    44,    61,    91,
      93,   123,   125,    40,    41,    46
};
# endif

#define YYPACT_NINF -80

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-80)))

#define YYTABLE_NINF -3

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -80,    10,    70,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   138,   -80,   -80,    18,   319,   -80,   -80,   -80,
     -80,   -80,    37,   -80,   -28,    49,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,    29,   319,   -80,    34,   -80,    19,    23,   234,   319,
      -3,   369,    15,   -80,    19,    30,   -80,    42,   -80,    93,
      95,   274,   274,    23,   274,   -80,   506,   -80,   -80,   -44,
     -80,   353,   437,    54,    69,   -80,   -80,   274,   -80,    18,
     183,   369,   -24,   -80,    72,    19,   -80,   -80,   -80,   -80,
     -80,   -23,   290,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,    19,
     -42,   -80,   -80,    -6,   319,   371,   -80,   -80,   459,    19,
     -80,   274,    35,    23,   -80,   -80,   107,   107,   240,   240,
     548,   548,   240,   240,   534,   520,   -80,   -80,    94,    94,
     -80,   506,    63,    71,   -80,   -30,    19,   -80,   126,   -80,
     -80,   -80,   -80,   -80,   506,   -80,   -80,   -80,   274,   -80,
     -80,   -80,   -80,   -80,   -80,   170,   -80,   178,   -80,   -80,
      68,   -80,    74,   -80,    86,   398,    83,   506,   -80,   222,
     -80,   -80,   274,   274,   -80,   416,   -80,   274,   -80,   304,
     322,   -80,   439,   230,   230,   -80,   100,   -80,   230,   -80
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      71,     0,    71,     4,     1,    14,    15,    17,    16,    13,
       5,     3,    71,     7,     8,     0,     0,     9,    18,     6,
     122,    10,     0,    32,    34,    36,    29,    20,    19,    28,
      48,    54,    21,    24,    25,    22,    27,    26,    23,    49,
      30,     0,    12,    31,     0,    11,     0,     0,     0,    71,
      37,    71,    45,    50,     0,    53,    33,    36,   108,     0,
       0,     0,     0,     0,     0,    35,    39,   109,   110,   111,
      38,     0,    66,     0,    61,    64,    62,     0,    47,     0,
      71,    71,     0,    55,    57,     0,    92,    91,    89,    90,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
     112,   116,    65,     0,     0,     0,    44,    46,    71,     0,
      51,     0,     0,     0,    40,   107,   105,   106,   100,    99,
     102,   101,    98,    97,   103,   104,    95,    96,    93,    94,
     119,   121,     0,   118,   113,   111,     0,    60,    71,    59,
      63,   115,    43,    56,    58,    52,    42,   117,     0,   114,
      83,    85,    88,    74,    69,    71,    77,     0,    73,    78,
       0,    79,     0,    80,     0,     0,   109,   120,    70,     0,
      68,    72,     0,     0,    86,     0,    75,     0,    67,     0,
       0,    87,     0,     0,     0,    76,    81,    84,     0,    82
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -80,   -80,   -80,   -80,   -14,   -12,   -47,   -45,   -80,   -34,
     -80,    89,    80,   -51,   -80,   -80,    61,   -80,   139,   -80,
     -80,    73,    40,   -80,   -80,   -80,    50,    41,    14,    -9,
     301,   -80,   -80,   -80,   -80,   -80,   -80,   -46,   -79,   -66,
     -80,   -80,   -80,    24
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    11,    12,    13,    14,    15,    16,    39,
      22,    23,    24,    65,    91,    40,    80,    41,    72,    43,
      44,    82,    83,    17,    73,    74,    75,   166,    18,   167,
     168,   169,   170,   171,   172,   173,   174,   175,    67,    50,
      68,   142,   143,    69
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      19,    66,    71,   110,    78,   107,    79,    77,    53,   108,
       4,   109,    90,   146,     3,    88,    89,    66,    92,   107,
      47,    20,    20,   119,   123,   109,    20,    58,   120,   124,
     144,   115,    20,   117,    78,    79,    79,    20,    53,    25,
     147,    59,    60,    61,    62,   148,    77,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    71,   141,    76,    21,    52,    81,   159,    55,   176,
      57,   117,   156,    79,    63,   154,    64,    66,    84,   110,
      51,    85,   119,    45,    46,    54,   176,   155,   176,    20,
      58,    48,     5,     6,     7,     8,    57,    86,    48,    87,
     176,     9,    49,    57,    59,    60,    61,    62,   113,    84,
     103,   104,   177,    10,   176,   176,   114,   157,   158,   176,
     121,   182,   140,   103,   104,   105,   106,   183,   185,    20,
      58,   187,   184,   145,   165,    56,   189,   190,    -2,    64,
     198,   192,   118,    84,    59,    60,    61,    62,     5,     6,
       7,     8,   112,    19,   149,    42,   179,     9,   122,   153,
       5,     6,     7,     8,   150,   160,     0,   161,   162,     9,
     145,     0,   163,    20,    58,     0,     0,   148,   164,    64,
       0,    20,    58,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     5,     6,     7,     8,    59,    60,    61,    62,
       0,     9,     0,     0,     0,     5,     6,     7,     8,   160,
       0,   161,   162,     0,     9,     0,   163,   160,     0,   161,
     162,   148,   178,    64,   163,    20,    58,     0,     0,   148,
     180,    64,     0,    20,    58,   116,     0,    20,    58,     0,
      59,    60,    61,    62,     0,     0,    93,    94,    59,    60,
      61,    62,    59,    60,    61,    62,   103,   104,   105,   106,
       0,   160,     0,   161,   162,     0,     0,     0,   163,   160,
       0,   161,   162,   148,   188,    64,   163,    20,    58,     0,
       0,   148,     0,    64,    70,     0,     0,    64,     0,     0,
       0,     0,    59,    60,    61,    62,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,    26,     0,     0,    64,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,     0,   125,    27,    28,    29,    30,    31,
       0,    32,    33,    34,    35,    36,    37,    38,   193,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,     0,     0,   194,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     5,     6,     7,     8,     0,     0,     0,     0,     0,
       9,     0,     0,   111,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,     0,
       0,   151,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,     0,     0,     0,
      20,     0,    26,     0,   186,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     0,
       0,     0,   191,    27,    28,    29,    30,    31,   181,    32,
      33,    34,    35,    36,    37,    38,     0,     0,     0,     0,
     181,     5,     6,     7,     8,   195,     0,     0,     0,     0,
       9,     0,     0,     0,   196,   197,     0,     0,     0,   199,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   152,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    93,    94,    95,    96,
      97,    98,    99,   100,   101,     0,   103,   104,   105,   106,
      93,    94,    95,    96,    97,    98,    99,   100,     0,     0,
     103,   104,   105,   106,    93,    94,    95,    96,     0,     0,
      99,   100,     0,     0,   103,   104,   105,   106
};

static const yytype_int16 yycheck[] =
{
      12,    47,    48,    69,    51,    49,    51,    49,    42,    53,
       0,    55,    63,    55,     0,    61,    62,    63,    64,    49,
      48,     3,     3,    47,    47,    55,     3,     4,    52,    52,
     109,    77,     3,    80,    81,    80,    81,     3,    72,    15,
      46,    18,    19,    20,    21,    51,    49,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    49,    46,    41,    51,   146,    44,   148,
      46,   118,   123,   118,    51,   121,    53,   123,    54,   145,
      51,    51,    47,    46,    47,    51,   165,    52,   167,     3,
       4,    49,    22,    23,    24,    25,    72,     4,    49,     4,
     179,    31,    53,    79,    18,    19,    20,    21,    54,    85,
      16,    17,   158,    43,   193,   194,    47,    54,    47,   198,
      48,    53,   108,    16,    17,    18,    19,    53,   174,     3,
       4,    48,    46,   109,   148,    46,   182,   183,     0,    53,
      40,   187,    81,   119,    18,    19,    20,    21,    22,    23,
      24,    25,    72,   165,   113,    16,   165,    31,    85,   119,
      22,    23,    24,    25,   114,    39,    -1,    41,    42,    31,
     146,    -1,    46,     3,     4,    -1,    -1,    51,    52,    53,
      -1,     3,     4,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    18,    19,    20,    21,
      -1,    31,    -1,    -1,    -1,    22,    23,    24,    25,    39,
      -1,    41,    42,    -1,    31,    -1,    46,    39,    -1,    41,
      42,    51,    52,    53,    46,     3,     4,    -1,    -1,    51,
      52,    53,    -1,     3,     4,    52,    -1,     3,     4,    -1,
      18,    19,    20,    21,    -1,    -1,     6,     7,    18,    19,
      20,    21,    18,    19,    20,    21,    16,    17,    18,    19,
      -1,    39,    -1,    41,    42,    -1,    -1,    -1,    46,    39,
      -1,    41,    42,    51,    52,    53,    46,     3,     4,    -1,
      -1,    51,    -1,    53,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     5,    -1,    -1,    53,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    54,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    38,    54,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    54,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    50,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    50,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
       3,    -1,     5,    -1,    46,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    46,    26,    27,    28,    29,    30,   167,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,
     179,    22,    23,    24,    25,    46,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   193,   194,    -1,    -1,    -1,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
       6,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      16,    17,    18,    19,     6,     7,     8,     9,    -1,    -1,
      12,    13,    -1,    -1,    16,    17,    18,    19
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,    58,    84,     0,    22,    23,    24,    25,    31,
      43,    59,    60,    61,    62,    63,    64,    79,    84,    61,
       3,    46,    66,    67,    68,    99,     5,    26,    27,    28,
      29,    30,    32,    33,    34,    35,    36,    37,    38,    65,
      71,    73,    74,    75,    76,    46,    47,    48,    49,    53,
      95,    51,    99,    65,    51,    99,    67,    99,     4,    18,
      19,    20,    21,    51,    53,    69,    93,    94,    96,    99,
      50,    93,    74,    80,    81,    82,    84,    49,    62,    63,
      72,    51,    77,    78,    99,    51,     4,     4,    93,    93,
      69,    70,    93,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    49,    53,    55,
      95,    50,    68,    54,    47,    93,    52,    62,    72,    47,
      52,    48,    77,    47,    52,    54,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      84,    93,    97,    98,    94,    99,    55,    46,    51,    83,
      82,    50,    52,    78,    93,    52,    69,    54,    47,    94,
      39,    41,    42,    46,    52,    60,    83,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    93,    52,    85,
      52,    86,    53,    53,    46,    93,    46,    48,    52,    93,
      93,    46,    93,    54,    54,    46,    86,    86,    40,    86
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    60,    60,    61,    61,
      62,    62,    63,    64,    64,    64,    64,    64,    64,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    66,    66,    67,    67,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    71,    72,    72,    73,    74,
      74,    75,    75,    75,    76,    77,    77,    78,    78,    79,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    83,
      83,    84,    85,    85,    86,    86,    86,    86,    86,    86,
      86,    87,    87,    88,    89,    90,    91,    91,    92,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    94,    94,    94,    94,    95,    95,    96,    97,    97,
      98,    98,    99
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     2,     1,     1,     1,
       2,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     2,     3,     1,
       3,     1,     3,     5,     4,     2,     2,     1,     1,     1,
       2,     4,     5,     2,     1,     1,     3,     1,     3,     6,
       6,     1,     1,     3,     1,     2,     1,     4,     3,     2,
       3,     0,     2,     1,     1,     2,     4,     1,     1,     1,
       1,     5,     7,     1,     5,     1,     2,     3,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     2,     3,     4,     4,     3,     4,     1,     1,
       3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 162 "vcp_yacc.y" /* yacc.c:1646  */
    {  
		Program * tmp=new Program((yyvsp[-1].toke_nums),(yyvsp[0].declaration_list));
		front_end_pro=tmp;
		(yyval.program)=tmp;
		
		delete (yyvsp[-1].toke_nums);
		delete (yyvsp[0].declaration_list);
	}
#line 1575 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 174 "vcp_yacc.y" /* yacc.c:1646  */
    { 
        list<Toke_num>* tmp = (yyvsp[-1].toke_nums);
        tmp->push_back(*(yyvsp[0].toke_num));
        delete (yyvsp[0].toke_num);
		   
		(yyval.toke_nums) = tmp;
	}
#line 1587 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 182 "vcp_yacc.y" /* yacc.c:1646  */
    {
 		list<Toke_num>* tmp=new list<Toke_num>;
 		
 		(yyval.toke_nums)=tmp;
 	}
#line 1597 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 191 "vcp_yacc.y" /* yacc.c:1646  */
    { 
  		Toke_num* tmp=new Toke_num((yyvsp[0].text),line_num);
		
		(yyval.toke_num)=tmp;
  	}
#line 1607 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 200 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Declaration>* tmp=(yyvsp[-1].declaration_list);
		tmp->push_back(*(yyvsp[0].declaration));
		delete (yyvsp[0].declaration);
		
		(yyval.declaration_list)=tmp;
	}
#line 1619 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 208 "vcp_yacc.y" /* yacc.c:1646  */
    { 
		list<Declaration>* tmp=new list<Declaration>;
		tmp->push_back(*(yyvsp[0].declaration));
		delete (yyvsp[0].declaration);
		
		(yyval.declaration_list)=tmp;
	}
#line 1631 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 219 "vcp_yacc.y" /* yacc.c:1646  */
    {
		if(scope->size()!=0&&scope->back()=="if")
		{
			yyerror("can not have variable declaration in selection structure!");
		}
		if(scope->size()!=0&&scope->back()=="while")
		{
			yyerror("can not have variable declaration in while structure!");
		}
		
		Declaration* tmp=new Declaration((yyvsp[0].var_declaration));
		tmp->set_line_begin((yyvsp[0].var_declaration)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.declaration)=tmp;
	}
#line 1652 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 237 "vcp_yacc.y" /* yacc.c:1646  */
    {
		if(scope->size()!=0&&scope->back()=="if")
		{
			yyerror("can not have function declaration in selection structure!");
		}
		if(scope->size()!=0&&scope->back()=="while")
		{
			yyerror("can not have function declaration in while structure!");
		}
		
		Declaration* tmp=new Declaration((yyvsp[0].fun_declaration));
		tmp->set_line_begin((yyvsp[0].fun_declaration)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.declaration)=tmp;
	}
#line 1673 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 257 "vcp_yacc.y" /* yacc.c:1646  */
    {   
		Var_declaration *tmp=new Var_declaration((yyvsp[-1].declaration_specifiers));
		tmp->set_line_begin((yyvsp[-1].declaration_specifiers)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		check_var_dec(tmp);
		set_var_dec(tmp);
	 	(yyval.var_declaration)=tmp;
	}
#line 1690 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 270 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Var_declaration *tmp=new Var_declaration((yyvsp[-2].declaration_specifiers),(yyvsp[-1].init_declarators));
		
		tmp->set_line_begin((yyvsp[-2].declaration_specifiers)->get_line_begin());
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

	    type_s=(yyvsp[-2].declaration_specifiers)->get_type_spec_list();

		for(ini_dec_lerator=(yyvsp[-1].init_declarators)->begin();ini_dec_lerator!=(yyvsp[-1].init_declarators)->end();ini_dec_lerator++){
		//遍历同一类型各变量
		//获取变量名
		string nam=ini_dec_lerator->get_dec()->get_name();
//scope_name_type保存每个变量的类型
	 for(types_s_lerator=type_s->begin();types_s_lerator!=type_s->end();types_s_lerator++) {
		 tynam=types_s_lerator->get_type_name();
		 scope_name_type.insert(pair<string,string>(nam,tynam));}
		 }

	 	(yyval.var_declaration)=tmp;
	}
#line 1729 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 308 "vcp_yacc.y" /* yacc.c:1646  */
    {
 		Declaration_specifiers* tmp=new Declaration_specifiers((yyvsp[-1].text),(yyvsp[0].type_specifiers));
 		tmp->set_line_begin((yyvsp[0].type_specifiers)->front().get_line_begin());
		tmp->set_line_end(line_num);

 		(yyval.declaration_specifiers)=tmp;
 	}
#line 1741 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 319 "vcp_yacc.y" /* yacc.c:1646  */
    {	 
	   char typ[]="typedef";
		(yyval.text)=typ;
	}
#line 1750 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 324 "vcp_yacc.y" /* yacc.c:1646  */
    {
	char ext[]="extern"; 
		(yyval.text)=ext;
	}
#line 1759 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 329 "vcp_yacc.y" /* yacc.c:1646  */
    { 
	 char sta[]="static";
		(yyval.text)=sta;
	}
#line 1768 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 334 "vcp_yacc.y" /* yacc.c:1646  */
    {
	  
	  char aut[]="auto";
		(yyval.text)=aut;
	}
#line 1778 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 340 "vcp_yacc.y" /* yacc.c:1646  */
    {
	  
	  char reg[]="register";
		(yyval.text)=reg;
	}
#line 1788 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 346 "vcp_yacc.y" /* yacc.c:1646  */
    { 
	 char emp[]="empty";
		(yyval.text)=emp;
	}
#line 1797 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 355 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("int",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		(yyval.type_specifier)=tmp;
	}
#line 1810 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 364 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("bool",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1824 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 374 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("char",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1838 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 384 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("short",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1852 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 394 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("long",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1866 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 404 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("signed",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1880 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 414 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("unsigned",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1894 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 424 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("const",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1908 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 434 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("volatile",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		(yyval.type_specifier)=tmp;
	}
#line 1921 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 443 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier("void",0);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		(yyval.type_specifier)=tmp;
	}
#line 1934 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 452 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier((yyvsp[0].text),1);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		(yyval.type_specifier)=tmp;
	}
#line 1947 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 461 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier((yyvsp[0].struct_specifier));
		tmp->set_line_begin((yyvsp[0].struct_specifier)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1959 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 469 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Type_specifier* tmp=new Type_specifier((yyvsp[0].enum_specifier));
		tmp->set_line_begin((yyvsp[0].enum_specifier)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.type_specifier)=tmp;
	}
#line 1971 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 480 "vcp_yacc.y" /* yacc.c:1646  */
    {
 		list<Init_declarator>* tmp=new list<Init_declarator>;
 		tmp->push_back(*(yyvsp[0].init_declarator));
 		delete (yyvsp[0].init_declarator);
 		
 		(yyval.init_declarators)=tmp;
 		
 	}
#line 1984 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 489 "vcp_yacc.y" /* yacc.c:1646  */
    {
 		list<Init_declarator>* tmp=(yyvsp[-2].init_declarators);
 		tmp->push_back(*(yyvsp[0].init_declarator));
 		delete (yyvsp[0].init_declarator);
 		
 		(yyval.init_declarators)=tmp;
 		
 	}
#line 1997 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 501 "vcp_yacc.y" /* yacc.c:1646  */
    {
		if(scope->size()==0 && (yyvsp[0].declarator)->get_name().size()>112)
		{
			yyerror("global variable's name is too long!");
		}
		
		Init_declarator* tmp=new Init_declarator((yyvsp[0].declarator));
		tmp->set_line_begin((yyvsp[0].declarator)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.init_declarator)=tmp;
		
	}
#line 2015 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 515 "vcp_yacc.y" /* yacc.c:1646  */
    {
		if(scope->size()==0 && (yyvsp[-2].declarator)->get_name().size()>112)
		{
			yyerror("global variable's name is too long!");
		}
		
		Init_declarator* tmp=new Init_declarator((yyvsp[-2].declarator),(yyvsp[0].initializer));
		tmp->set_line_begin((yyvsp[-2].declarator)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.init_declarator)=tmp;
		
	}
#line 2033 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 532 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Declarator* tmp=new Declarator((yyvsp[0].toke_num)->get_toke());
		tmp->set_line_begin((yyvsp[0].toke_num)->get_num());
		tmp->set_line_end(line_num);
		
		(yyval.declarator)=tmp;
	}
#line 2045 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 540 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Declarator* tmp=new Declarator((yyvsp[-1].toke_num)->get_toke(),(yyvsp[0].exps));
		tmp->set_line_begin((yyvsp[-1].toke_num)->get_num());
		tmp->set_line_end(line_num);
		
		(yyval.declarator)=tmp;
	}
#line 2057 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 548 "vcp_yacc.y" /* yacc.c:1646  */
    {
		yyerror("invalid array subscript!");
	}
#line 2065 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 556 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Initializer* tmp=new Initializer((yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[0].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.initializer)=tmp;
		
	}
#line 2078 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 565 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Initializer* tmp=new Initializer((yyvsp[-1].initializer_list));
		tmp->set_line_begin((yyvsp[-1].initializer_list)->front().get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.initializer)=tmp;
	}
#line 2090 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 576 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Initializer>* tmp=new list<Initializer>;
		tmp->push_back(*(yyvsp[0].initializer));
		delete (yyvsp[0].initializer);
		
		(yyval.initializer_list)=tmp;
		
	}
#line 2103 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 585 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Initializer>* tmp=(yyvsp[-2].initializer_list);
		tmp->push_back(*(yyvsp[0].initializer));
		delete (yyvsp[0].initializer);
		
		(yyval.initializer_list)=tmp;
	}
#line 2115 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 597 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Struct_specifier* tmp=new Struct_specifier((yyvsp[-3].toke_num)->get_toke(),(yyvsp[-1].var_declarations));
		tmp->set_line_begin((yyvsp[-4].line)->real_line);
		tmp->set_line_end(line_num);
		
		struct_body--;//2010年3月4日追加
		
		tmp->set_fake_line((yyvsp[-4].line)->fake_line);//2010年3月10日追加
		(yyval.struct_specifier)=tmp;
		
	}
#line 2131 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 609 "vcp_yacc.y" /* yacc.c:1646  */
    {
		yyerror("struct specifier must have a name!");
	}
#line 2139 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 613 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Struct_specifier* tmp=new Struct_specifier((yyvsp[0].toke_num)->get_toke());
		tmp->set_line_begin((yyvsp[-1].line)->real_line);
		tmp->set_line_end(line_num);
		
		struct_body--;//2010年3月4日追加
		tmp->set_fake_line((yyvsp[-1].line)->fake_line);//2010年3月10日追加
		(yyval.struct_specifier)=tmp;
	}
#line 2153 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 626 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Var_declaration>* tmp=(yyvsp[-1].var_declarations);
		tmp->push_back(*(yyvsp[0].var_declaration));
		delete (yyvsp[0].var_declaration);
		
		(yyval.var_declarations)=tmp;
		
	}
#line 2166 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 635 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Var_declaration>* tmp=new list<Var_declaration>;
		tmp->push_back(*(yyvsp[0].var_declaration));
		delete (yyvsp[0].var_declaration);
		
		(yyval.var_declarations)=tmp;
	}
#line 2178 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 646 "vcp_yacc.y" /* yacc.c:1646  */
    {
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;

		struct_body++;//2010年3月4日追加
		(yyval.line)=tmp;
	}
#line 2192 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 659 "vcp_yacc.y" /* yacc.c:1646  */
    {
 		list<Type_specifier>* tmp=new list<Type_specifier>;
 		tmp->push_back(*(yyvsp[0].type_specifier));
 		delete (yyvsp[0].type_specifier);
 		
 		(yyval.type_specifiers)=tmp;
 	}
#line 2204 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 667 "vcp_yacc.y" /* yacc.c:1646  */
    {
 		list<Type_specifier>* tmp=(yyvsp[-1].type_specifiers);
 		tmp->push_back(*(yyvsp[0].type_specifier));
 		delete (yyvsp[0].type_specifier);
 		
 		(yyval.type_specifiers)=tmp;
 	}
#line 2216 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 678 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Enum_specifier* tmp=new Enum_specifier((yyvsp[-1].enumerator_list));
		tmp->set_line_begin((yyvsp[-3].line)->real_line);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line((yyvsp[-3].line)->fake_line);
		(yyval.enum_specifier)=tmp;
		
	}
#line 2230 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 688 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Enum_specifier* tmp=new Enum_specifier((yyvsp[-3].toke_num)->get_toke(),(yyvsp[-1].enumerator_list));
		tmp->set_line_begin((yyvsp[-4].line)->real_line);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line((yyvsp[-4].line)->fake_line);
		(yyval.enum_specifier)=tmp;
		
	}
#line 2244 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 698 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Enum_specifier* tmp=new Enum_specifier((yyvsp[0].toke_num)->get_toke());
		tmp->set_line_begin((yyvsp[-1].line)->real_line);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line((yyvsp[-1].line)->fake_line);
		(yyval.enum_specifier)=tmp;
	}
#line 2257 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 710 "vcp_yacc.y" /* yacc.c:1646  */
    {
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		(yyval.line)=tmp;
	}
#line 2270 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 721 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Enumerator>* tmp=new list<Enumerator>;
		tmp->push_back(*(yyvsp[0].enumerator));
		delete (yyvsp[0].enumerator);
		
		(yyval.enumerator_list)=tmp;
	
	}
#line 2283 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 730 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Enumerator>* tmp=(yyvsp[-2].enumerator_list);
		tmp->push_back(*(yyvsp[0].enumerator));
		delete (yyvsp[0].enumerator);
		
		(yyval.enumerator_list)=tmp;
	}
#line 2295 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 741 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Enumerator* tmp=new Enumerator((yyvsp[0].toke_num)->get_toke());
		tmp->set_line_begin((yyvsp[0].toke_num)->get_num());
		tmp->set_line_end(line_num);
		//   2015/3/19 闪闪追加		
		check_enum_dec((yyvsp[0].toke_num)->get_toke());		   		 
		scope_name_type.insert(pair<string,string>((yyvsp[0].toke_num)->get_toke(),"ENUM")); 
		//追加结束		
		(yyval.enumerator)=tmp;
	}
#line 2310 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 752 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Enumerator* tmp=new Enumerator((yyvsp[-2].toke_num)->get_toke(),(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].toke_num)->get_num());
		tmp->set_line_end(line_num);
		
		//   2015/3/19 闪闪追加		 
		scope_name_type.insert(pair<string,string>((yyvsp[-2].toke_num)->get_toke(),"ENUM"));
		//追加结束
		
		
		(yyval.enumerator)=tmp;
	}
#line 2327 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 768 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Fun_declaration* tmp=new Fun_declaration((yyvsp[-5].declaration_specifiers),(yyvsp[-4].toke_num)->get_toke(),(yyvsp[-2].params),(yyvsp[0].compound_stmt));
		tmp->set_line_begin((yyvsp[-5].declaration_specifiers)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		(yyval.fun_declaration)=tmp;
		scope->pop_back();
	}
#line 2343 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 780 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Fun_declaration* tmp=new Fun_declaration((yyvsp[-5].declaration_specifiers),(yyvsp[-4].toke_num)->get_toke(),(yyvsp[-2].params));
		tmp->set_line_begin((yyvsp[-5].declaration_specifiers)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月10日追加
		tmp->set_file_info(file_info);
		
		(yyval.fun_declaration)=tmp;
		scope->pop_back();
	}
#line 2359 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 795 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		list<Param>* tmp=(yyvsp[0].params);
		
		(yyval.params)=tmp;
		scope->push_back("fun");
	}
#line 2370 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 802 "vcp_yacc.y" /* yacc.c:1646  */
    { 
		list<Param>* tmp=new list<Param>;
		
		(yyval.params)=tmp;
		scope->push_back("fun");
	}
#line 2381 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 812 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		list<Param>* tmp=(yyvsp[-2].params);
		tmp->push_back(*(yyvsp[0].param));
		delete (yyvsp[0].param);
		
		(yyval.params)=tmp;
	
	}
#line 2394 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 821 "vcp_yacc.y" /* yacc.c:1646  */
    { 
		list<Param>* tmp=new list<Param>;
		tmp->push_back(*(yyvsp[0].param));
		delete (yyvsp[0].param);
		
		(yyval.params)=tmp;
	}
#line 2406 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 831 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		if((yyvsp[-1].type_specifiers)->front().get_stamp()==2  )
		{
			yyerror("function 's parameter should not be struct type!");
		}
		if((yyvsp[-1].type_specifiers)->front().get_stamp()==3)
		{
			yyerror("function 's parameter should not be enum type!");
		}
		
		if((yyvsp[0].declarator)->get_stamp()==1)
		{
			yyerror("function 's parameter should not be array!");
		}
		
		Param* tmp=new Param((yyvsp[-1].type_specifiers),(yyvsp[0].declarator));
		tmp->set_line_begin((yyvsp[-1].type_specifiers)->front().get_line_begin());
		tmp->set_line_end(line_num);
		
		//   2015/3/14 闪闪追加
		list<Type_specifier>* type_ss;
		list<Type_specifier>::iterator types_ss_lerator; 
		string tynams;
		string namss=(yyvsp[0].declarator)->get_name(); 	      
	 	for(types_ss_lerator=(yyvsp[-1].type_specifiers)->begin();types_ss_lerator!=(yyvsp[-1].type_specifiers)->end();types_ss_lerator++) {
		 	tynams=types_ss_lerator->get_type_name();
		 	scope_name_type.insert(pair<string,string>(namss,tynams));
		 	}
		//追加结束				
		(yyval.param)=tmp; 
	}
#line 2442 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 863 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		if((yyvsp[0].type_specifiers)->front().get_stamp()==2  )
		{
			yyerror("function 's parameter should not be struct type!");
		}
		if((yyvsp[0].type_specifiers)->front().get_stamp()==3)
		{
			yyerror("function 's parameter should not be enum type!");
		}
		
		Param* tmp=new Param((yyvsp[0].type_specifiers));
		tmp->set_line_begin((yyvsp[0].type_specifiers)->front().get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.param)=tmp;
	}
#line 2463 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 884 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Compound_stmt *tmp=new Compound_stmt((yyvsp[-2].declaration_list),(yyvsp[-1].statement_list));
		tmp->set_line_begin((yyvsp[-2].declaration_list)->front().get_line_begin());
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.compound_stmt)=tmp;
		
	}
#line 2478 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 895 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		list<Declaration>* tmp1=new list<Declaration>;
		Compound_stmt *tmp=new Compound_stmt(tmp1,(yyvsp[-1].statement_list));
		tmp->set_line_begin((yyvsp[-1].statement_list)->front().get_line_begin());
		tmp->set_line_end(line_num);	
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		(yyval.compound_stmt)=tmp;
	}
#line 2492 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 905 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Declaration>* tmp1=new list<Declaration>;
		list<Statement>* tmp2=new list<Statement>;
		Compound_stmt *tmp=new Compound_stmt(tmp1,tmp2);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.compound_stmt)=tmp;
	}
#line 2508 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 918 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Statement>* tmp2=new list<Statement>;
		Compound_stmt *tmp=new Compound_stmt((yyvsp[-1].declaration_list),tmp2);
		tmp->set_line_begin((yyvsp[-1].declaration_list)->front().get_line_begin());
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
		(yyval.compound_stmt)=tmp;
	}
#line 2523 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 938 "vcp_yacc.y" /* yacc.c:1646  */
    {  
		list<Statement>* tmp=(yyvsp[-1].statement_list); 
		tmp->push_back(*(yyvsp[0].statement));
		delete (yyvsp[0].statement);
		
		(yyval.statement_list)=tmp;
	}
#line 2535 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 946 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Statement>* tmp=new list<Statement>; 
		tmp->push_back(*(yyvsp[0].statement));
		delete (yyvsp[0].statement);
		
		(yyval.statement_list)=tmp;
	}
#line 2547 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 957 "vcp_yacc.y" /* yacc.c:1646  */
    {	 
		Statement *tmp=new Statement(";");
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		//2010年3月10日追加
		tmp->set_fake_line(fake_line_num);
		
      	(yyval.statement)=tmp;
    }
#line 2561 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 967 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Statement *tmp=new Statement((yyvsp[-1].expression));
		tmp->set_line_begin((yyvsp[-1].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
	 	(yyval.statement)=tmp;
	}
#line 2573 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 975 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Statement *tmp=new Statement((yyvsp[-3].var),(yyvsp[-1].expression));
		tmp->set_line_begin((yyvsp[-3].var)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_assign_stmt(tmp);
		
	 	(yyval.statement)=tmp;
	 		 	
	}
#line 2589 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 987 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Statement *tmp=new Statement((yyvsp[0].compound_stmt));
		tmp->set_line_begin((yyvsp[0].compound_stmt)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.statement)=tmp;
	}
#line 2601 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 995 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Statement *tmp=new Statement((yyvsp[0].selection_stmt));
		tmp->set_line_begin((yyvsp[0].selection_stmt)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.statement)=tmp;
	}
#line 2613 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1003 "vcp_yacc.y" /* yacc.c:1646  */
    { 	
		Statement *tmp=new Statement((yyvsp[0].iteration_stmt));
		tmp->set_line_begin((yyvsp[0].iteration_stmt)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.statement)=tmp;
	}
#line 2625 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 1011 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Statement *tmp=new Statement((yyvsp[0].return_stmt));
		tmp->set_line_begin((yyvsp[0].return_stmt)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.statement)=tmp;
	}
#line 2637 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1024 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		check_if_while((yyvsp[0].statement));
		Selection_stmt *tmp=new Selection_stmt((yyvsp[-2].expression),(yyvsp[0].statement));
		tmp->set_line_begin((yyvsp[-4].line)->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-4].line)->fake_line);//2010年3月10日追加
		(yyval.selection_stmt)=tmp;
		scope->pop_back();
	}
#line 2651 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1034 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		check_if_while((yyvsp[-2].statement));
		check_else_if((yyvsp[0].statement));
		Selection_stmt *tmp=new Selection_stmt((yyvsp[-4].expression),(yyvsp[-2].statement),(yyvsp[0].statement));
		tmp->set_line_begin((yyvsp[-6].line)->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-6].line)->fake_line);//2010年3月10日追加
		(yyval.selection_stmt)=tmp;
		scope->pop_back();
	}
#line 2666 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1048 "vcp_yacc.y" /* yacc.c:1646  */
    {
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		(yyval.line)=tmp;
		scope->push_back("if");
	}
#line 2680 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1061 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		check_if_while((yyvsp[0].statement));
		Iteration_stmt *tmp=new Iteration_stmt((yyvsp[-2].expression),(yyvsp[0].statement));
		tmp->set_line_begin((yyvsp[-4].line)->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-4].line)->fake_line);//2010年3月10日追加
		(yyval.iteration_stmt)=tmp;
		scope->pop_back();
	}
#line 2694 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1074 "vcp_yacc.y" /* yacc.c:1646  */
    {
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		(yyval.line)=tmp;
		scope->push_back("while");
	}
#line 2708 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1087 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Return_stmt *tmp=new Return_stmt();
		tmp->set_line_begin((yyvsp[-1].line)->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-1].line)->fake_line);//2010年3月10日追加
		(yyval.return_stmt)=tmp;
	}
#line 2720 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1095 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		Return_stmt *tmp=new Return_stmt((yyvsp[-1].expression));
		tmp->set_line_begin((yyvsp[-2].line)->real_line);
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-2].line)->fake_line);//2010年3月10日追加
		(yyval.return_stmt)=tmp;
	}
#line 2732 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1106 "vcp_yacc.y" /* yacc.c:1646  */
    {
		//2010年3月10日修改
		struct lines* tmp=new struct lines;
		tmp->real_line=line_num;
		tmp->fake_line=fake_line_num;
		
		(yyval.line)=tmp;
	}
#line 2745 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1118 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression("!",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[0].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_logic_exp(tmp);
		
		(yyval.expression)=tmp;
	}
#line 2760 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1129 "vcp_yacc.y" /* yacc.c:1646  */
    {
	
		//2015年1月阮闪闪追加
		Expression *tmp=new Expression("~",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[0].expression)->get_line_begin());
		tmp->set_line_end(line_num);			
		//check_logic_exp(tmp);
		check_inverse_exp(tmp);//3月追加 				 	
		(yyval.expression)=tmp;	
		
	
	}
#line 2777 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1142 "vcp_yacc.y" /* yacc.c:1646  */
    {
		string str="-";
		str=str+(yyvsp[0].text);
		Expression *tmp=new Expression(str);
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
		
		(yyval.expression)=tmp;
	}
#line 2792 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1153 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[0].text));
		tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
		(yyval.expression)=tmp;
	}
#line 2804 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1161 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"+",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));		
		//有无符号混合运算检查
		//闪闪追加
        check_mixsign_exp((yyvsp[-2].expression),(yyvsp[0].expression),scope_name_type);
		(yyval.expression)=tmp;
	}
#line 2822 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1175 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"-",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		//减法中不允许无符号运算
		//闪闪追加
		check_subunsig_exp((yyvsp[-2].expression),(yyvsp[0].expression),scope_name_type);
		
		(yyval.expression)=tmp;
	}
#line 2841 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1190 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"*",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		//有无符号混合运算检查
		//闪闪追加
		check_mixsign_exp((yyvsp[-2].expression),(yyvsp[0].expression),scope_name_type);
		
		(yyval.expression)=tmp;
	}
#line 2860 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1205 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"/",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		//有无符号混合运算检查
		//闪闪追加
		check_mixsign_exp((yyvsp[-2].expression),(yyvsp[0].expression),scope_name_type);
		
		(yyval.expression)=tmp;
	}
#line 2879 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1221 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"<",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2895 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1233 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),">",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2911 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1245 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),">=",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2927 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1257 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"<=",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2943 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1269 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"!=",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2959 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1281 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"==",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2975 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1293 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"&&",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_logic_exp((yyvsp[-2].expression));
		check_logic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 2991 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1305 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"||",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_logic_exp((yyvsp[-2].expression));
		check_logic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 3007 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1317 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),"<<",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 3023 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1329 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-2].expression),">>",(yyvsp[0].expression));
		tmp->set_line_begin((yyvsp[-2].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
		//2010年3月9日追加
		check_unlogic_exp((yyvsp[-2].expression));
		check_unlogic_exp((yyvsp[0].expression));
		
		(yyval.expression)=tmp;
	}
#line 3039 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1341 "vcp_yacc.y" /* yacc.c:1646  */
    {
     	Expression *tmp=new Expression((yyvsp[-1].expression));
     	tmp->set_line_begin((yyvsp[-1].expression)->get_line_begin());
		tmp->set_line_end(line_num);
		
     	(yyval.expression)=tmp;
    }
#line 3051 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1349 "vcp_yacc.y" /* yacc.c:1646  */
    {
     	Expression *tmp=new Expression((yyvsp[0].text));
     	tmp->set_line_begin(line_num);
		tmp->set_line_end(line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
     	(yyval.expression)=tmp;
    }
#line 3063 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1357 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[0].var));
		tmp->set_line_begin((yyvsp[0].var)->get_line_begin());
		tmp->set_line_end(line_num);
		
		(yyval.expression)=tmp;
	}
#line 3075 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1365 "vcp_yacc.y" /* yacc.c:1646  */
    {
		(yyval.expression)=(yyvsp[0].expression);
	}
#line 3083 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1372 "vcp_yacc.y" /* yacc.c:1646  */
    {	
		 
		list<string> l1=get_type_var((yyvsp[0].toke_num)->get_toke(),scope_name_type);
		Var *tmp =new Var((yyvsp[0].toke_num)->get_toke(),l1);//2015年1月
		tmp->set_line_begin((yyvsp[0].toke_num)->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[0].toke_num)->get_fake_line());//2010年3月10日追加
		 
		
		(yyval.var)=tmp;
	}
#line 3099 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1385 "vcp_yacc.y" /* yacc.c:1646  */
    {
	 
		list<string> l2=get_type_var((yyvsp[-1].toke_num)->get_toke(),scope_name_type);
		Var* tmp=new Var((yyvsp[-1].toke_num)->get_toke(),l2,(yyvsp[0].exps));//2015年1月
		tmp->set_line_begin((yyvsp[-1].toke_num)->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-1].toke_num)->get_fake_line());//2010年3月10日追加
		
	 
		
		(yyval.var)=tmp;
			
	}
#line 3117 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1399 "vcp_yacc.y" /* yacc.c:1646  */
    { 
	
		list<string> l3=get_type_var((yyvsp[-2].toke_num)->get_toke(),scope_name_type);
		Var* tmp=new Var((yyvsp[0].var),(yyvsp[-2].toke_num)->get_toke(),l3);//2015年1月
		tmp->set_line_begin((yyvsp[-2].toke_num)->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-2].toke_num)->get_fake_line());//2010年3月10日追加
		
		 
		
		(yyval.var)=tmp;
	}
#line 3134 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1412 "vcp_yacc.y" /* yacc.c:1646  */
    { 
	
		list<string> l4=get_type_var((yyvsp[-3].toke_num)->get_toke(),scope_name_type);
		Var* tmp=new Var((yyvsp[0].var),(yyvsp[-3].toke_num)->get_toke(),l4,(yyvsp[-2].exps));//2015年1月
		tmp->set_line_begin((yyvsp[-3].toke_num)->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-3].toke_num)->get_fake_line());//2010年3月10日追加
		
		 
		
		(yyval.var)=tmp;
	}
#line 3151 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1428 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Expression> *tmp=(yyvsp[-3].exps);
		tmp->push_back(*(yyvsp[-1].expression));
		delete (yyvsp[-1].expression);
		
		(yyval.exps)=tmp;
	}
#line 3163 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1436 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Expression> *tmp=new list<Expression>;
		tmp->push_back(*(yyvsp[-1].expression));
		
		//2010年3月5日追加
		check_array_sub((yyvsp[-1].expression));
		
		delete (yyvsp[-1].expression);
		
		(yyval.exps)=tmp;
	}
#line 3179 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1451 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Expression *tmp=new Expression((yyvsp[-3].toke_num)->get_toke(),(yyvsp[-1].exps));
		tmp->set_line_begin((yyvsp[-3].toke_num)->get_num());
		tmp->set_line_end(line_num);
		tmp->set_fake_line((yyvsp[-3].toke_num)->get_fake_line());//2010年3月10日追加
		(yyval.expression)=tmp;
	}
#line 3191 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1462 "vcp_yacc.y" /* yacc.c:1646  */
    {
		(yyval.exps)=(yyvsp[0].exps);
	}
#line 3199 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1466 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Expression> *tmp=new list<Expression>;
		
		(yyval.exps)=tmp;
	}
#line 3209 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1475 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Expression> *tmp=(yyvsp[-2].exps);
		tmp->push_back(*(yyvsp[0].expression));
		delete (yyvsp[0].expression);
		
		(yyval.exps)=tmp;
	}
#line 3221 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1484 "vcp_yacc.y" /* yacc.c:1646  */
    {
		list<Expression> *tmp=new list<Expression>;
		tmp->push_back(*(yyvsp[0].expression));
		delete (yyvsp[0].expression);
		
		(yyval.exps)=tmp;
	}
#line 3233 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1495 "vcp_yacc.y" /* yacc.c:1646  */
    {
		Toke_num* tmp=new Toke_num((yyvsp[0].text),line_num);
		tmp->set_fake_line(fake_line_num);//2010年3月10日追加
		(yyval.toke_num)=tmp;
	}
#line 3243 "vcp_yacc.cpp" /* yacc.c:1646  */
    break;


#line 3247 "vcp_yacc.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1501 "vcp_yacc.y" /* yacc.c:1906  */







