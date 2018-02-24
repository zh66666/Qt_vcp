/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef VCL_GENERIC_SERVICE_ROOT_H_
#define VCL_GENERIC_SERVICE_ROOT_H_

/******************************************************************************
*    Debug switch Section
******************************************************************************/

/* Intel 环境下打开此开关 */
/*#define COMPILE_ENVIROMENT_INTEL */




/******************************************************************************
*    Include File Section
******************************************************************************/



/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
/*自定义基础数据类型*/
typedef unsigned char  UINT8    ;
typedef signed   char  INT8     ;
typedef char           CHARACTER;
typedef unsigned short UINT16   ;
typedef signed   short INT16    ;
typedef int            INTEGER32;
typedef unsigned int  UINT32   ;
typedef int  INT32    ;
typedef float          REAL32   ;
typedef double         REAL64   ;

/*定义空指针*/
#define NULL          ((void*)0)




/******************************************************************************
*    Global Struct    Define Section
******************************************************************************/
/*定义校验码*/
typedef struct TAG_Chk
{
  INT32 C1; /*根据PrimeA计算确定的校验码*/
  INT32 C2; /*根据PrimeB计算确定的校验码*/
}T_CHECK;


/* 定义编码后数据类型 */
typedef struct TAG_DATA 
{
  INT32 F; /*功能码*/
  T_CHECK C; /*校验码*/
}T_DATA;


/*定义被保护的编码数据*/
typedef struct TAG_CODE
{
  T_DATA Data; /*将变量编码后的数据                          */
  UINT32 Id  ; /*签名的标识(记录变量的签名在"签名表"中的位置)*/
}T_CODE;
/*暂定大写*/


/******************************************************************************
*    Global Variable Declare Section
******************************************************************************/
extern INT32 g_GS_TL0;/*时间标签的初始值，用于数组编码运算*/
extern INT32 g_GS_TL1;
extern INT32 g_GS_TL2;
extern INT32 dT;

typedef enum TAG_BOOL 
{
  e_TRUE = 0x00A2E527,
  e_FALSE = 0xFFBED234
} E_BOOL;

#define P1 12970357
#define P2 12239417


/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/



/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/

#endif
