/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef VCL_GENERIC_SERVICE_ROOT_H_
#define VCL_GENERIC_SERVICE_ROOT_H_

/******************************************************************************
*    Debug switch Section
******************************************************************************/

/* Intel �����´򿪴˿��� */
/*#define COMPILE_ENVIROMENT_INTEL */




/******************************************************************************
*    Include File Section
******************************************************************************/



/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
/*�Զ��������������*/
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

/*�����ָ��*/
#define NULL          ((void*)0)




/******************************************************************************
*    Global Struct    Define Section
******************************************************************************/
/*����У����*/
typedef struct TAG_Chk
{
  INT32 C1; /*����PrimeA����ȷ����У����*/
  INT32 C2; /*����PrimeB����ȷ����У����*/
}T_CHECK;


/* ���������������� */
typedef struct TAG_DATA 
{
  INT32 F; /*������*/
  T_CHECK C; /*У����*/
}T_DATA;


/*���屻�����ı�������*/
typedef struct TAG_CODE
{
  T_DATA Data; /*����������������                          */
  UINT32 Id  ; /*ǩ���ı�ʶ(��¼������ǩ����"ǩ����"�е�λ��)*/
}T_CODE;
/*�ݶ���д*/


/******************************************************************************
*    Global Variable Declare Section
******************************************************************************/
extern INT32 g_GS_TL0;/*ʱ���ǩ�ĳ�ʼֵ�����������������*/
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
