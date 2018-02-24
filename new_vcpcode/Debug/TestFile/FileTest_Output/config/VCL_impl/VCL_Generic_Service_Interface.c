#include "VCL_Generic_Service_Interface.h"
#include <stdio.h>

/*****************************************************************************
 *    Global Var Define
 *****************************************************************************/

/******************************************************************************
 *    Coded Operation Interface Implement
 ******************************************************************************/

/**
 * Encoding algorithm library's implement
 */
static T_CODE Array_EXTA =
{ 0,
{ 32, 32 }, 32 };

static int RK[2];
static int Prime[2];

static long long i_temp_P1 = 8224836ll;
static long long i_temp_P2 = 11711720ll;

int g_GS_TL1;
int g_GS_TL2;
int dT;

void init_code()
{
	Prime[0] = P1;
	Prime[1] = P2;
	int i = 0;
	for (i = 0; i < 2; i++)
	{
		RK[i] = ((1ll << 32) % Prime[i] * (1ll << 32) % Prime[i]) % Prime[i];
	}
	g_GS_TL1 = 0;
	g_GS_TL2 = 0;

	dT = 1;
}

int AC_64bit_Add(int a1, int a2, int P)
{
	long long sum = (long long) a1 + (long long) a2;
	return sum % P;
}

int AC_64bit_Sub(int a1, int a2, int P)
{
	long long diff = (long long) a1 - (long long) a2;
	return diff % P;
}

UINT32 AC_64bit_UMul(UINT32 a1, UINT32 a2, int P)
{
	unsigned long long final = (long long) a1 * (long long) a2;
	return final % P;
}

int AC_64bit_Mul(int a1, int a2, int P)
{
	long long final = (long long) a1 * (long long) a2;
	return final % P;
}

T_CHECK F_VCL_CalIfSig_GreatThan(T_DATA inX, INT32 inK)
{
	INT32 S[2] =
	{ 0, 0 };
	INT32 Prime[2] =
	{ P1, P2 };
	INT32 inX_C[2] =
	{ inX.C.C1, inX.C.C2 };
	UINT32 TwoP64Mod[2] =
	{ 1106004, 799556 };
	INT32 TL[2] =
	{ g_GS_TL1, g_GS_TL2 };
	INT32 i = 0;
	INT32 inXL_NoTL = 0;
	UINT32 inXHShift_Mod = 0;
	INT32 R = 0;
	INT32 Tmp;
	while (i < 2)
	{
		Tmp = AC_64bit_Sub(inX_C[i], TL[i], Prime[i]);
		inXL_NoTL = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		inXHShift_Mod = AC_64bit_UMul(TwoP64Mod[i], (UINT32) inX.F, Prime[i]);
		Tmp = AC_64bit_Add((INT32) inXHShift_Mod, inXL_NoTL, Prime[i]);
		R = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		S[i] = AC_64bit_Mul(R, inK, Prime[i]);

		i = i + 1;
	}
	T_CHECK Ret;
	Ret.C1 = S[0];
	Ret.C2 = S[1];

	return Ret;
}

T_CHECK F_VCL_CalExitSig(T_DATA inX1, T_DATA inX2, INT32 inK)
{
	INT32 S[2] =
	{ 0, 0 };
	INT32 Prime[2] =
	{ P1, P2 };
	INT32 inX1_C[2] =
	{ inX1.C.C1, inX1.C.C2 };
	INT32 inX2_C[2] =
	{ inX2.C.C1, inX2.C.C2 };
	UINT32 TwoP64Mod[2] =
	{ 1106004, 799556 };
	INT32 TL[2] =
	{ g_GS_TL1, g_GS_TL2 };
	INT32 i = 0;
	INT32 inXL_NoTL1 = 0, inXL_NoTL2 = 0;
	UINT32 inXHShift_Mod1 = 0, inXHShift_Mod2 = 0;
	INT32 R = 0, R1 = 0, R2 = 0;
	INT32 Tmp;
	while (i < 2)
	{
		Tmp = AC_64bit_Sub(inX1_C[i], TL[i], Prime[i]);
		inXL_NoTL1 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		inXHShift_Mod1 = AC_64bit_UMul(TwoP64Mod[i], (UINT32) inX1.F, Prime[i]);
		Tmp = AC_64bit_Add((INT32) inXHShift_Mod1, inXL_NoTL1, Prime[i]);
		R1 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		Tmp = AC_64bit_Sub(inX2_C[i], TL[i], Prime[i]);
		inXL_NoTL2 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		inXHShift_Mod2 = AC_64bit_UMul(TwoP64Mod[i], (UINT32) inX2.F, Prime[i]);
		Tmp = AC_64bit_Add((INT32) inXHShift_Mod2, inXL_NoTL2, Prime[i]);
		R2 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		R = AC_64bit_Add(R1, R2, Prime[i]);

		S[i] = AC_64bit_Mul(R, inK, Prime[i]);

		i = i + 1;
	}
	T_CHECK Ret;
	Ret.C1 = S[0];
	Ret.C2 = S[1];

	return Ret;
}

T_CHECK F_VCL_CalBodySig(T_DATA inX1, T_DATA inX2)
{
	INT32 S[2] =
	{ 0, 0 };
	INT32 Prime[2] =
	{ P1, P2 };
	INT32 inX1_C[2] =
	{ inX1.C.C1, inX1.C.C2 };
	INT32 inX2_C[2] =
	{ inX2.C.C1, inX2.C.C2 };
	UINT32 TwoP64Mod[2] =
	{ 1106004, 799556 };
	INT32 TL[2] =
	{ g_GS_TL1, g_GS_TL2 };
	INT32 i = 0;
	INT32 inXL_NoTL1 = 0, inXL_NoTL2 = 0;
	UINT32 inXHShift_Mod1 = 0, inXHShift_Mod2 = 0;
	INT32 R = 0, R1 = 0, R2 = 0;
	INT32 Tmp;
	while (i < 2)
	{
		Tmp = AC_64bit_Sub(inX1_C[i], TL[i], Prime[i]);
		inXL_NoTL1 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		inXHShift_Mod1 = AC_64bit_UMul(TwoP64Mod[i], (UINT32) inX1.F, Prime[i]);
		Tmp = AC_64bit_Add((INT32) inXHShift_Mod1, inXL_NoTL1, Prime[i]);
		R1 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		Tmp = AC_64bit_Sub(inX2_C[i], TL[i], Prime[i]);
		inXL_NoTL2 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		inXHShift_Mod2 = AC_64bit_UMul(TwoP64Mod[i], (UINT32) inX2.F, Prime[i]);
		Tmp = AC_64bit_Add((INT32) inXHShift_Mod2, inXL_NoTL2, Prime[i]);
		R2 = AC_64bit_Add(Tmp, Prime[i], Prime[i]);

		R = AC_64bit_Add(R1, R2, Prime[i]);

		S[i] = R;

		i = i + 1;
	}
	T_CHECK Ret;
	Ret.C1 = S[0];
	Ret.C2 = S[1];

	return Ret;
}

T_DATA F_VCL_Div(const T_DATA inX, const INT32 inDiv, const INT32 inRecip1,
		const INT32 inRecip2, const INT32 inComp1, const INT32 inComp2)
{
	T_DATA outZ;
	INT32 F_Z;
	UINT32 UF_Z;
//	if(inX.SymbolFlag==1)//the number with symbol
//	{
	F_Z = (INT32) inX.F / (INT32) inDiv;
	outZ.F = (UINT32) F_Z;
//	}
//	else
//	{
//		UF_Z=(UINT32)inX.F/(UINT32)inDiv// the number without symbol
//		outZ.F=(UINT32)F_Z;
//	}
//	outZ.SymbolFlag=inX.SymbolFlag;
	INT32 AC_tmp_1;
	AC_tmp_1 = AC_64bit_Mul(inX.F % inDiv, RK[0], P1);
	AC_tmp_1 = AC_64bit_Add(inX.C.C1, AC_tmp_1, P1);
	AC_tmp_1 = AC_64bit_Sub(AC_tmp_1, g_GS_TL1, P1);
	AC_tmp_1 = AC_64bit_Mul(AC_tmp_1, inRecip1, P1);
	AC_tmp_1 = AC_64bit_Add(AC_tmp_1, inComp1, P1);
	outZ.C.C1 = AC_64bit_Add(AC_tmp_1, g_GS_TL1, P1);

	AC_tmp_1 = AC_64bit_Mul(inX.F % inDiv, RK[1], P2);
	AC_tmp_1 = AC_64bit_Add(inX.C.C2, AC_tmp_1, P2);
	AC_tmp_1 = AC_64bit_Sub(AC_tmp_1, g_GS_TL2, P2);
	AC_tmp_1 = AC_64bit_Mul(AC_tmp_1, inRecip2, P2);
	AC_tmp_1 = AC_64bit_Add(AC_tmp_1, inComp2, P2);
	outZ.C.C2 = AC_64bit_Add(AC_tmp_1, g_GS_TL2, P2);

	return outZ;
}

T_DATA F_VCL_Assign(const T_DATA inX, const INT32 inComp1, const INT32 inComp2)
{

	T_DATA outZ;
	AC_BIND_CONST_NUM_0x00000000 = CONST_NUM_0x00000000;
	AC_BIND_CONST_NUM_0x00000000.Data.C.C1 += g_GS_TL1;
	AC_BIND_CONST_NUM_0x00000000.Data.C.C2 += g_GS_TL2;
	outZ = F_VCL_Add(inX, AC_BIND_CONST_NUM_0x00000000.Data, inComp1, inComp2);

	return outZ;
}

T_DATA F_VCL_Add(const T_DATA inX, const T_DATA inY, const INT32 inComp1,
		const INT32 inComp2)
{
	T_DATA outZ;
	INT32 F_Z;
	UINT32 UF_Z;
	/*if(inX.SymbolFlag==1)
	 {*/
	F_Z = (INT32) inX.F + (INT32) inY.F;
	outZ.F = (INT32) F_Z;
	/*}
	 else
	 {
	 UF_Z=(UINT32)inX.F+(UINT32)inY.F
	 outZ.F=(UINT32)F_Z;
	 }
	 outZ.SymbolFlag=inX.SymbolFlag;*/
	INT32 temp1 = AC_64bit_Add(inX.C.C1, inY.C.C1, P1);
	temp1 = AC_64bit_Add(temp1, inComp1, P1);
	outZ.C.C1 = AC_64bit_Sub(temp1, g_GS_TL1, P1);
	INT32 temp2 = AC_64bit_Add(inX.C.C2, inY.C.C2, P2);
	temp2 = AC_64bit_Add(temp2, inComp2, P2);
	outZ.C.C2 = AC_64bit_Sub(temp2, g_GS_TL2, P2);
	return outZ;
}
T_DATA F_VCL_Sub(const T_DATA inX, const T_DATA inY, const INT32 inComp1,
		const INT32 inComp2)
{
	T_DATA outZ;
	INT32 F_Z;
	UINT32 UF_Z;
//	if(inX.SymbolFlag==1)
//	{
	F_Z = (INT32) inX.F - (INT32) inY.F;
	outZ.F = (UINT32) F_Z;
//	}
//	else
//	{
//		UF_Z=(UINT32)inX.F-(UINT32)inY.F
//		outZ.F=(UINT32)F_Z;
//	}
	//outZ.SymbolFlag=inX.SymbolFlag;
	INT32 temp1 = AC_64bit_Sub(inX.C.C1, inY.C.C1, P1);
	temp1 = AC_64bit_Add(temp1, inComp1, P1);
	outZ.C.C1 = AC_64bit_Add(temp1, g_GS_TL1, P1);
	INT32 temp2 = AC_64bit_Sub(inX.C.C2, inY.C.C2, P2);
	temp2 = AC_64bit_Add(temp2, inComp2, P2);
	outZ.C.C2 = AC_64bit_Add(temp2, g_GS_TL2, P2);
	return outZ;
}
//
T_DATA F_VCL_Mul(const T_DATA inX, const T_DATA inY, const INT32 inSx,
		const INT32 inSy, const INT32 inComp1, const INT32 inComp2)
{
	T_DATA outZ;
	INT32 F_Z;
	UINT32 UF_Z;
//	if(inX.SymbolFlag==1)
//	{
	F_Z = (INT32) inX.F * (INT32) inY.F;
	outZ.F = (UINT32) F_Z;
//	}
//	else
//	{
//		UF_Z=(UINT32)inX.F-(UINT32)inY.F
//		outZ.F=(UINT32)F_Z;
//	}
//	outZ.SymbolFlag=inX.SymbolFlag;
	INT32 AC_tmp_2, AC_tmp_3, AC_tmp_4, AC_tmp_7, AC_tmp_8;
	//W11=(T_L-L_1_x)(L_1_y-2*S_y-T_L);
	AC_tmp_3 = AC_64bit_Sub(g_GS_TL1, inX.C.C1, P1);
	AC_tmp_7 = AC_64bit_Sub(inY.C.C1, inSy * 2, P1);	//
	AC_tmp_7 = AC_64bit_Sub(AC_tmp_7, g_GS_TL1, P1);
	AC_tmp_7 = AC_64bit_Mul(AC_tmp_3, AC_tmp_7, P1);

	//W12=(T_L-L_1_y)(L_1_x-2*S_x-T_L);
	AC_tmp_4 = AC_64bit_Sub(g_GS_TL1, inY.C.C1, P1);
	AC_tmp_8 = AC_64bit_Sub(inX.C.C1, inSx * 2, P1);
	AC_tmp_8 = AC_64bit_Sub(AC_tmp_8, g_GS_TL1, P1);
	AC_tmp_8 = AC_64bit_Mul(AC_tmp_4, AC_tmp_8, P1);

	//Get Check1
	AC_tmp_2 = AC_64bit_Add(AC_tmp_7, AC_tmp_8, P1);
	AC_tmp_2 = AC_64bit_Mul(i_temp_P1, AC_tmp_2, P1);
	AC_tmp_2 = AC_64bit_Add(AC_tmp_2, inComp1, P1);
	outZ.C.C1 = AC_64bit_Add(AC_tmp_2, g_GS_TL1, P1);

	//W21=(T_L-L_2_x)(L_2_y-2*S_y-T_L);
	AC_tmp_3 = AC_64bit_Sub(g_GS_TL2, inX.C.C2, P2);
	AC_tmp_7 = AC_64bit_Sub(inY.C.C2, inSy * 2, P2);
	AC_tmp_7 = AC_64bit_Sub(AC_tmp_7, g_GS_TL2, P2);
	AC_tmp_7 = AC_64bit_Mul(AC_tmp_3, AC_tmp_7, P2);

	//W22=(T_L-L_2_y)(L_2_x-2*S_x-T_L);
	AC_tmp_4 = AC_64bit_Sub(g_GS_TL2, inY.C.C2, P2);
	AC_tmp_8 = AC_64bit_Sub(inX.C.C2, inSx * 2, P2);
	AC_tmp_8 = AC_64bit_Sub(AC_tmp_8, g_GS_TL2, P2);
	AC_tmp_8 = AC_64bit_Mul(AC_tmp_4, AC_tmp_8, P2);

	//Get Check2
	AC_tmp_2 = AC_64bit_Add(AC_tmp_7, AC_tmp_8, P2);
	AC_tmp_2 = AC_64bit_Mul(i_temp_P2, AC_tmp_2, P2);
	AC_tmp_2 = AC_64bit_Add(AC_tmp_2, inComp2, P2);
	outZ.C.C2 = AC_64bit_Add(AC_tmp_2, g_GS_TL2, P2);

	return outZ;
}
T_DATA F_VCL_BitShiftL(const T_DATA inX, const INT32 inShiftNum,
		const INT32 inComp1, const INT32 inComp2)
{
	T_DATA outZ;
	INT32 F_Z;
	UINT32 UF_Z;

	F_Z = (INT32) inX.F << (INT32) inShiftNum;
	outZ.F = (UINT32) F_Z;

	long long AC_temp1 = ((long long) inX.C.C1 - g_GS_TL1) << inShiftNum;
	long long AC_temp2 = (long long) inX.F >> (32 - inShiftNum);
	long long AC_temp3 = ((long long) inX.F >> (32 - inShiftNum - 1)) & 1;
	outZ.C.C1 = (AC_temp1 + (AC_temp2 + AC_temp3) * (RK[0]) * (RK[0]) + g_GS_TL1
			+ inComp1) % P1;

	long long AC_temp11 = ((long long) inX.C.C2 - g_GS_TL2) << inShiftNum;
	long long AC_temp12 = (long long) inX.F >> (32 - inShiftNum);
	long long AC_temp13 = ((long long) inX.F >> (32 - inShiftNum - 1)) & 1;
	outZ.C.C2 = (AC_temp11 + (AC_temp12 + AC_temp13) * (RK[1]) * (RK[1])
			+ g_GS_TL2 + inComp2) % P2;

	return outZ;
}

T_DATA F_VCL_BitShiftR(const T_DATA inX, const INT32 inShiftNum,
		const INT32 inRecip1, const INT32 inRecip2, const INT32 inComp1,
		const INT32 inComp2)
{
	T_DATA outZ;
	INT32 F_Z;
	UINT32 UF_Z;

	F_Z = (INT32) inX.F >> (INT32) inShiftNum;
	outZ.F = (UINT32) F_Z;

	long long AC_temp1 = (long long) (1 << inShiftNum) - 1;
	long long AC_temp2 = ((long long) inX.F & AC_temp1) * RK[0];	//Global_Rk1
	outZ.C.C1 = (((long long) inX.C.C1 + AC_temp2 - g_GS_TL1) * inRecip1
			+ g_GS_TL1 + inComp1) % P1;

	long long AC_temp22 = ((long long) inX.F & AC_temp1) * RK[1];	//Global_Rk2
	outZ.C.C2 = (((long long) inX.C.C2 + AC_temp22 - g_GS_TL2) * inRecip2
			+ g_GS_TL2 + inComp2) % P2;
	return outZ;
}

T_DATA F_VCL_ArrayRef(const T_DATA inX, const T_DATA inIndex,
		const INT32 inComp1, const INT32 inComp2)
{

	T_DATA result;
	result.F = (INT32) inX.F;

	INT32 AC_tmp_0 = AC_64bit_Add(inX.C.C1, inIndex.C.C1, P1);
	result.C.C1 = AC_64bit_Add(AC_tmp_0, inComp1, P1);

	INT32 AC_tmp_1 = AC_64bit_Add(inX.C.C2, inIndex.C.C2, P2);
	result.C.C2 = AC_64bit_Add(AC_tmp_1, inComp2, P2);

	return result;

}

//T_DATA F_VCL_ArrayAssign(const T_DATA inX, const T_CHECK inArr,
//		const T_CHECK inExp, const T_DATA inIndex, const INT32 inComp1,
//		const INT32 inComp2, const INT32 inExtComp1, const INT32 inExtComp2)
//{
//	T_DATA result;
//	result.F = (INT32) inX.F;
//
//	result.C.C1 = inX.C.C1;
//	result.C.C2 = inX.C.C2;
//
//	INT32 AC_extr_tmp_0 = AC_64bit_Add(Array_EXTA.Data.C.C1, inIndex.C.C1, P1);
//	INT32 AC_extr_tmp_01 = AC_64bit_Add(AC_extr_tmp_0, result.C.C1, P1);
//	INT32 AC_extr_tmp_02 = AC_64bit_Sub(AC_extr_tmp_01, inExp.C1, P1);
//	Array_EXTA.Data.C.C1 = AC_64bit_Add(AC_extr_tmp_02, inExtComp1, P1)
//			- g_GS_TL1;
//
//	INT32 AC_extr_tmp_1 = AC_64bit_Add(Array_EXTA.Data.C.C2, inIndex.C.C2, P2);
//	INT32 AC_extr_tmp_11 = AC_64bit_Add(AC_extr_tmp_1, result.C.C2, P2);
//	INT32 AC_extr_tmp_12 = AC_64bit_Sub(AC_extr_tmp_11, inExp.C2, P2);
//	Array_EXTA.Data.C.C2 = AC_64bit_Add(AC_extr_tmp_12, inExtComp2, P2)
//			- g_GS_TL2;
//
//	INT32 AC_tmp_0 = AC_64bit_Sub(result.C.C1, inIndex.C.C1, P1);
//	result.C.C1 = AC_64bit_Add(AC_tmp_0, inComp1, P1);
//
//	INT32 AC_tmp_1 = AC_64bit_Sub(result.C.C2, inIndex.C.C2, P2);
//	result.C.C2 = AC_64bit_Add(AC_tmp_1, inComp2, P2);
//
//	return result;
//
//}

T_DATA F_VCL_ArrayAssign(const T_DATA inX, const T_DATA inIndex,
		const int inComp1, const int inComp2)
{

	T_DATA result;
	result.F = (INT32) inX.F;

	INT32 AC_tmp_0 = AC_64bit_Sub(inX.C.C1, inIndex.C.C1, P1);
	result.C.C1 = AC_64bit_Add(AC_tmp_0, inComp1, P1);

	INT32 AC_tmp_1 = AC_64bit_Sub(inX.C.C2, inIndex.C.C2, P2);
	result.C.C2 = AC_64bit_Add(AC_tmp_1, inComp2, P2);
	return result;

}

void F_VCL_ArrayAssignExt(const T_CHECK inArr, const T_CHECK inExp,
		const T_DATA inIndex, const INT32 inComp1, const INT32 inComp2)
{

	INT32 temp = Array_EXTA.Data.F + inIndex.F;
	Array_EXTA.Data.F = temp;

	INT32 AC_extr_tmp_0 = AC_64bit_Add(Array_EXTA.Data.C.C1, inIndex.C.C1, P1);
	INT32 AC_extr_tmp_01 = AC_64bit_Add(AC_extr_tmp_0, inArr.C1, P1);
	INT32 AC_extr_tmp_02 = AC_64bit_Sub(AC_extr_tmp_01, inExp.C1, P1);
	INT32 AC_extr_tmp_03 = AC_64bit_Sub(AC_extr_tmp_02, RK[0] * inIndex.F, P1);	//
	Array_EXTA.Data.C.C1 = AC_64bit_Add(AC_extr_tmp_03, inComp1, P1) - g_GS_TL1;

	INT32 AC_extr_tmp_1 = AC_64bit_Add(Array_EXTA.Data.C.C2, inIndex.C.C2, P2);
	INT32 AC_extr_tmp_11 = AC_64bit_Add(AC_extr_tmp_1, inArr.C2, P2);
	INT32 AC_extr_tmp_12 = AC_64bit_Sub(AC_extr_tmp_11, inExp.C2, P2);
	INT32 AC_extr_tmp_13 = AC_64bit_Sub(AC_extr_tmp_12, RK[1] * inIndex.F, P2);	//
	Array_EXTA.Data.C.C2 = AC_64bit_Add(AC_extr_tmp_13, inComp2, P2) - g_GS_TL2;
}

INT32 F_VCL_RegisterCodeId(const INT32 id)
{
	return id;
}

void F_VCL_ArrayExtUpdate(const INT32 sig)
{

	Array_EXTA.Id = sig;

}

void F_VCL_ArrayCheckUpdate(const INT32 Comp1, const INT32 Comp2)
{

	Array_EXTA.Data.C.C1 = (Array_EXTA.Data.C.C1 + Comp1) % P1;
	Array_EXTA.Data.C.C2 = (Array_EXTA.Data.C.C2 + Comp2) % P2;

}

/*void F_VCL_CheckCodeArr(T_CODE arr[], INT32 index)
 {

 INT32 AC_tmp_1 = 0, AC_tmp_11 = 0;
 AC_tmp_1 = AC_64bit_Mul(arr[index].Data.F, (long long) RK[0], P1);
 AC_tmp_1 = AC_64bit_Add(AC_tmp_1, arr[index].Data.C.C1, P1);
 AC_tmp_1 = AC_64bit_Sub(AC_tmp_1, g_GS_TL1, P1);
 AC_tmp_11 = AC_64bit_Mul(index, (long long) RK[0], P1);
 AC_tmp_1 = AC_64bit_Sub(AC_tmp_1, AC_tmp_11, P1);

 INT32 AC_tmp_2 = 0, AC_tmp_22 = 0;
 AC_tmp_2 = AC_64bit_Mul(arr[index].Data.F, (long long) RK[1], P2);
 AC_tmp_2 = AC_64bit_Add(AC_tmp_2, arr[index].Data.C.C2, P2);
 AC_tmp_2 = AC_64bit_Sub(AC_tmp_2, g_GS_TL2, P2);
 AC_tmp_22 = AC_64bit_Mul(index, (long long) RK[1], P2);
 AC_tmp_2 = AC_64bit_Sub(AC_tmp_2, AC_tmp_22, P2);

 printf("%d %d", AC_tmp_1, AC_tmp_2);
 printf("\n");

 }*/

void F_VCL_CheckCodeArr(T_CODE inData, INT32 inIndex, INT32 ErrorNo)
{
	INT32 AC_tmp_1 = 0, AC_tmp_11 = 0;
	AC_tmp_1 = AC_64bit_Mul(inData.Data.F, (long long) RK[0], P1);
	AC_tmp_1 = AC_64bit_Add(AC_tmp_1, inData.Data.C.C1, P1);
	AC_tmp_1 = AC_64bit_Sub(AC_tmp_1, g_GS_TL1, P1);
	AC_tmp_11 = AC_64bit_Mul(inIndex, (long long) RK[0], P1);
	AC_tmp_1 = AC_64bit_Sub(AC_tmp_1, AC_tmp_11, P1);

	INT32 AC_tmp_2 = 0, AC_tmp_22 = 0;
	AC_tmp_2 = AC_64bit_Mul(inData.Data.F, (long long) RK[1], P2);
	AC_tmp_2 = AC_64bit_Add(AC_tmp_2, inData.Data.C.C2, P2);
	AC_tmp_2 = AC_64bit_Sub(AC_tmp_2, g_GS_TL2, P2);
	AC_tmp_22 = AC_64bit_Mul(inIndex, (long long) RK[1], P2);
	AC_tmp_2 = AC_64bit_Sub(AC_tmp_2, AC_tmp_22, P2);

	printf("%d %d", AC_tmp_1, AC_tmp_2);
	printf("\n");
}

void F_VCL_CheckCodeVar(T_CODE inVar, INT32 inErrNo)
{
	int sig[2] =
	{ 0 };
	int i = 0;
	int Check[2] =
	{ inVar.Data.C.C1, inVar.Data.C.C2 };
	int TL[2] =
	{ g_GS_TL1, g_GS_TL2 };
	for (i = 0; i < 2; i++)
	{
		sig[i] = ((((long long) RK[i] * (long long) inVar.Data.F) % Prime[i]
				+ Check[i] % Prime[i] - TL[i] % Prime[i]) % Prime[i] + Prime[i])
				% Prime[i];
	}

	printf("%d %d", sig[0], sig[1]);
	printf("\n");

}

void F_VCL_CheckExtrVar()
{
	int inErrNo = 0;
	//F_VCL_CheckCodeVar(Array_EXTA, inErrNo);
	int sig[2] =
	{ 0 };
	int i = 0;
	int Check[2] =
	{ Array_EXTA.Data.C.C1, Array_EXTA.Data.C.C2 };
	int TL[2] =
	{ g_GS_TL1, g_GS_TL2 };
	for (i = 0; i < 2; i++)
	{
		sig[i] = ((((long long) RK[i] * (long long) Array_EXTA.Data.F)
				% Prime[i] + Check[i] % Prime[i]) % Prime[i] + Prime[i])
				% Prime[i];
	}

	printf("%d %d", sig[0], sig[1]);
	printf("\n");
}

T_DATA F_VCL_GreaterThanOrEqual(T_DATA inX, T_DATA inY, int inSubComp1,
		int inSubComp2, int inComp1, int inComp2)
{
	T_CODE Diff, IfOutVar;
	INT32 AdjTmpVar = 0;
	Diff.Data = F_VCL_Sub(inX, inY, inSubComp1, inSubComp2);	//Diff's sig ok
	T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 6);  //ok
	if (Diff.Data.F >= 0)
	{
		AC_BIND_BOOL_TRUE = CONST_BOOL_TRUE;
		AC_BIND_BOOL_TRUE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_TRUE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 1, 1);  //7
	}
	else
	{
		AC_BIND_BOOL_FALSE = CONST_BOOL_FALSE;
		AC_BIND_BOOL_FALSE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_FALSE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 1, 1);

		IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 2538295, P1);
		IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 10404809, P2);
	}
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C1, OutSig.C1, P1);   // 7+30=37
	IfOutVar.Data.C.C1 = AC_64bit_Add(AdjTmpVar, inComp1, P1);
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C2, OutSig.C2, P1);
	IfOutVar.Data.C.C2 = AC_64bit_Add(AdjTmpVar, inComp2, P2);
	return IfOutVar.Data;
}
T_DATA F_VCL_LessThan(T_DATA inX, T_DATA inY, int inSubComp1, int inSubComp2,
		int inComp1, int inComp2)
{
	T_CODE Diff, IfOutVar;
	INT32 AdjTmpVar = 0;
	Diff.Data = F_VCL_Sub(inX, inY, inSubComp1, inSubComp2);   //Diff's sig
	T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 10);
	if (Diff.Data.F >= 0)
	{
		AC_BIND_BOOL_FALSE = CONST_BOOL_FALSE;
		AC_BIND_BOOL_FALSE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_FALSE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 4, 4);
	}
	else
	{
		AC_BIND_BOOL_TRUE = CONST_BOOL_TRUE;
		AC_BIND_BOOL_TRUE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_TRUE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 6, 6);

		IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 12877397, P1);
		IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 5101932, P2);
	}
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C1, OutSig.C1, P1);
	IfOutVar.Data.C.C1 = AC_64bit_Add(AdjTmpVar, inComp1, P1);
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C2, OutSig.C2, P1);
	IfOutVar.Data.C.C2 = AC_64bit_Add(AdjTmpVar, inComp2, P2);
	return IfOutVar.Data;
}
T_DATA F_VCL_LessThanOrEqual(T_DATA inX, T_DATA inY, int inSubComp1,
		int inSubComp2, int inComp1, int inComp2)
{
	T_CODE Diff, IfOutVar;
	INT32 AdjTmpVar = 0;
	Diff.Data = F_VCL_Sub(inY, inX, inSubComp1, inSubComp2);   //Diff's sig
	T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 14);
	if (Diff.Data.F >= 0)
	{
		AC_BIND_BOOL_TRUE = CONST_BOOL_TRUE;
		AC_BIND_BOOL_TRUE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_TRUE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 9, 9);
	}
	else
	{
		AC_BIND_BOOL_FALSE = CONST_BOOL_FALSE;
		AC_BIND_BOOL_FALSE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_FALSE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 9, 9);

		IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 10246142, P1);
		IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 12038472, P2);
	}
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C1, OutSig.C1, P1);
	IfOutVar.Data.C.C1 = AC_64bit_Add(AdjTmpVar, inComp1, P1);
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C2, OutSig.C2, P1);
	IfOutVar.Data.C.C2 = AC_64bit_Add(AdjTmpVar, inComp2, P2);
	return IfOutVar.Data;
}
T_DATA F_VCL_GreaterThan(T_DATA inX, T_DATA inY, int inSubComp1, int inSubComp2,
		int inComp1, int inComp2)
{
	T_CODE Diff, IfOutVar;
	INT32 AdjTmpVar = 0;
	Diff.Data = F_VCL_Sub(inY, inX, inSubComp1, inSubComp2);   //Diff's sig
	T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 18);
	if (Diff.Data.F >= 0)
	{
		AC_BIND_BOOL_FALSE = CONST_BOOL_FALSE;
		AC_BIND_BOOL_FALSE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_FALSE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 12, 12);
	}
	else
	{
		AC_BIND_BOOL_TRUE = CONST_BOOL_TRUE;
		AC_BIND_BOOL_TRUE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_TRUE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 14, 14);

		IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 7614887, P1);
		IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 6735595, P2);
	}
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C1, OutSig.C1, P1);
	IfOutVar.Data.C.C1 = AC_64bit_Add(AdjTmpVar, inComp1, P1);
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C2, OutSig.C2, P1);
	IfOutVar.Data.C.C2 = AC_64bit_Add(AdjTmpVar, inComp2, P2);
	return IfOutVar.Data;
}
T_DATA F_VCL_Equal(T_DATA inX, T_DATA inY, int inSubComp1, int inSubComp2,
		int inComp1, int inComp2)
{
	T_CODE Diff, IfOutVar;
	INT32 AdjTmpVar = 0;
	Diff.Data = F_VCL_Sub(inX, inY, inSubComp1, inSubComp2);   //Diff's sig
	AC_BIND_CONST_NUM_0x00000000 = CONST_NUM_0x00000000;
	AC_BIND_CONST_NUM_0x00000000.Data.C.C1 += g_GS_TL1;
	AC_BIND_CONST_NUM_0x00000000.Data.C.C2 += g_GS_TL2;
	T_CODE Diff2;
	Diff2.Data = F_VCL_Sub(AC_BIND_CONST_NUM_0x00000000.Data, Diff.Data, 39, 39);
	T_CHECK OutSig = F_VCL_CalExitSig(Diff.Data, Diff2.Data, 23);
	if (Diff.Data.F == 0)
	{
		AC_BIND_BOOL_TRUE = CONST_BOOL_TRUE;
		AC_BIND_BOOL_TRUE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_TRUE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 18, 18);
	}
	else
	{
		AC_BIND_BOOL_FALSE = CONST_BOOL_FALSE;
		AC_BIND_BOOL_FALSE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_FALSE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 18, 18);

		IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 1083229, P1);
		IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 3166853, P2);
	}
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C1, OutSig.C1, P1);
	IfOutVar.Data.C.C1 = AC_64bit_Add(AdjTmpVar, inComp1, P1);
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C2, OutSig.C2, P1);
	IfOutVar.Data.C.C2 = AC_64bit_Add(AdjTmpVar, inComp2, P2);
	return IfOutVar.Data;
}
T_DATA F_VCL_NotEqual(T_DATA inX, T_DATA inY, int inSubComp1, int inSubComp2,
		int inComp1, int inComp2)
{
	T_CODE Diff, IfOutVar;
	INT32 AdjTmpVar = 0;
	Diff.Data = F_VCL_Sub(inX, inY, inSubComp1, inSubComp2);   //Diff's sig
	AC_BIND_CONST_NUM_0x00000000 = CONST_NUM_0x00000000;
	AC_BIND_CONST_NUM_0x00000000.Data.C.C1 += g_GS_TL1;
	AC_BIND_CONST_NUM_0x00000000.Data.C.C2 += g_GS_TL2;
	T_CODE Diff2;
	Diff2.Data = F_VCL_Sub(AC_BIND_CONST_NUM_0x00000000.Data, Diff.Data, 49, 49);
	T_CHECK OutSig = F_VCL_CalExitSig(Diff.Data, Diff2.Data, 28);
	if (Diff.Data.F == 0)
	{
		AC_BIND_BOOL_FALSE = CONST_BOOL_FALSE;
		AC_BIND_BOOL_FALSE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_FALSE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 22, 22);
	}
	else
	{
		AC_BIND_BOOL_TRUE = CONST_BOOL_TRUE;
		AC_BIND_BOOL_TRUE.Data.C.C1 += g_GS_TL1;
		AC_BIND_BOOL_TRUE.Data.C.C2 += g_GS_TL2;
		IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 24, 24);

		IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 7521928, P1);
		IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 11837528, P2);
	}
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C1, OutSig.C1, P1);
	IfOutVar.Data.C.C1 = AC_64bit_Add(AdjTmpVar, inComp1, P1);
	AdjTmpVar = AC_64bit_Add(IfOutVar.Data.C.C2, OutSig.C2, P1);
	IfOutVar.Data.C.C2 = AC_64bit_Add(AdjTmpVar, inComp2, P2);
	return IfOutVar.Data;
}

