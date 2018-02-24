/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef VCL_GENERIC_SERVICE_INTERFACE_H_
#define VCL_GENERIC_SERVICE_INTERFACE_H_

/******************************************************************************
*    Include File Section
******************************************************************************/
#include "VCL_Generic_Service_Root.h"
#include "CFG_Constants.h"

/******************************************************************************
*    Coded Operation Interface
******************************************************************************/

T_DATA F_VCL_Add(const T_DATA inX,const T_DATA inY,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_Sub(const T_DATA inX,const T_DATA inY,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_Mul(const T_DATA inX,const T_DATA inY,const INT32 inSx,const INT32 inSy,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_Div(const T_DATA inX,const INT32 inDiv,const INT32 inRecip1,const INT32 inRecip2,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_Assign(const T_DATA inX,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_BitShiftR(const T_DATA inX,const INT32 inShiftNum,const INT32 inRecip1,const INT32 inRecip2,const INT32inComp1,const INT32inComp2);

T_DATA F_VCL_BitShiftL(const T_DATA inX,const INT32 inShiftNum,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_BitInverse(const T_DATA inX,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_LogicAnd (const T_DATA inX,const T_DATA inY,const INT32 inSx, const INT32 inSy,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_LogicOr(const T_DATA inX,const T_DATA inY,const INT32 inSx, const INT32 inSy,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_LogicNot(const T_DATA inX,const INT32 inSx, const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_ArrayRef(const T_DATA inX,const T_DATA inIndex,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_ArrayAssign(const T_DATA inX,const T_DATA inIndex,const int inComp1,const int inComp2);

void F_VCL_ArrayAssignExt(const T_CHECK inArr, const T_CHECK inExp, const T_DATA inIndex, const INT32 inComp1, const INT32 inComp2);

INT32 F_VCL_RegisterCodeId(const INT32 id);

void F_VCL_ArrayExtUpdate(const INT32 sig);

void F_VCL_ArrayCheckUpdate(const INT32 Comp1,const INT32 Comp2);

T_DATA F_VCL_GreaterThanOrEqual(const T_DATA inX,const T_DATA inY,const INT32 inSubComp1,const INT32 inSubComp2,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_LessThanOrEqual(const T_DATA inX,const T_DATA inY,const INT32 inSubComp1,const INT32 inSubComp2,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_GreaterThan(const T_DATA inX,const T_DATA inY,const INT32 inSubComp1,const INT32 inSubComp2,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_LessThan(const T_DATA inX,const T_DATA inY,const INT32 inSubComp1,const INT32 inSubComp2,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_Equal(const T_DATA inX,const T_DATA inY,const INT32 inSubComp1,const INT32 inSubComp2,const INT32 inComp1,const INT32 inComp2);

T_DATA F_VCL_NotEqual(const T_DATA inX,const T_DATA inY,const INT32 inSubComp1,const INT32 inSubComp2,const INT32 inComp1,const INT32 inComp2);

INT32 AC_64bit_Add(const INT32 inX,const INT32 inY,const INT32 inP);

INT32 AC_64bit_Sub(const INT32 inX,const INT32 inY,const INT32 inP);

INT32 AC_64bit_Mul(const INT32 inX,const INT32 inY,const INT32 inP);

UINT32 AC_64bit_UAdd(const UINT32 inX,const UINT32 inY,const INT32 inP);

UINT32 AC_64bit_UMul(const UINT32 inX,const UINT32 inY,const INT32 inP);

INT32 AC_64bit_LShift(const INT32 inX,const INT32 inY,const INT32 inP);

INT32 AC_64bit_RShift(const INT32 inX,const INT32 inY,const INT32 inP);

T_CHECK F_VCL_CalIfSig_GreatThan(T_DATA inX,INT32 inK);

T_CHECK F_VCL_CalExitSig(T_DATA inX1,T_DATA inX2,INT32 inK);

T_CHECK F_VCL_CalBodySig(T_DATA inX1,T_DATA inX2);

void F_VCL_CheckCodeVar(T_CODE inVar,INT32 inErrNo);

void F_VCL_CheckCodeArr(T_CODE arr[], INT32 index);
void F_VCL_CheckExtrVar();
#endif
