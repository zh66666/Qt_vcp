T_DATA F_VCL_GreaterThanOrEqual(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)
{
  T_CODE Diff,IfOutVar;
  INT32 AdjTmpVar=0;
  Diff.Data=F_VCL_Sub(inX,inY,inSubComp1,inSubComp2);//Diff's sig
  T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 6);
  if(Diff.Data.F >= 0)
  {
    AC_BIND_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;
    AC_BIND_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 1, 1);
  }
  else
  {
    AC_BIND_BOOL_FALSE.Data.F = CONST_BOOL_FALSE.Data.F;
    AC_BIND_BOOL_FALSE.Data.C.C1 = CONST_BOOL_FALSE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_FALSE.Data.C.C2 = CONST_BOOL_FALSE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 1, 1);

    IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 2538295, P1);
    IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 10404809, P2);
  }
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);
  IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);
  IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);
  return IfOutVar.Data;
}
T_DATA F_VCL_LessThan(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)
{
  T_CODE Diff,IfOutVar;
  INT32 AdjTmpVar=0;
  Diff.Data=F_VCL_Sub(inX,inY,inSubComp1,inSubComp2);//Diff's sig
  T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 10);
  if(Diff.Data.F >= 0)
  {
    AC_BIND_BOOL_FALSE.Data.F = CONST_BOOL_FALSE.Data.F;
    AC_BIND_BOOL_FALSE.Data.C.C1 = CONST_BOOL_FALSE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_FALSE.Data.C.C2 = CONST_BOOL_FALSE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 4, 4);
  }
  else
  {
    AC_BIND_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;
    AC_BIND_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 6, 6);

    IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 12877397, P1);
    IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 5101932, P2);
  }
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);
  IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);
  IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);
  return IfOutVar.Data;
}
T_DATA F_VCL_LessThanOrEqual(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)
{
  T_CODE Diff,IfOutVar;
  INT32 AdjTmpVar=0;
  Diff.Data=F_VCL_Sub(inY,inX,inSubComp1,inSubComp2);//Diff's sig
  T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 14);
  if(Diff.Data.F >= 0)
  {
    AC_BIND_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;
    AC_BIND_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 9, 9);
  }
  else
  {
    AC_BIND_BOOL_FALSE.Data.F = CONST_BOOL_FALSE.Data.F;
    AC_BIND_BOOL_FALSE.Data.C.C1 = CONST_BOOL_FALSE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_FALSE.Data.C.C2 = CONST_BOOL_FALSE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 9, 9);

    IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 10246142, P1);
    IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 12038472, P2);
  }
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);
  IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);
  IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);
  return IfOutVar.Data;
}
T_DATA F_VCL_GreaterThan(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)
{
  T_CODE Diff,IfOutVar;
  INT32 AdjTmpVar=0;
  Diff.Data=F_VCL_Sub(inY,inX,inSubComp1,inSubComp2);//Diff's sig
  T_CHECK OutSig = F_VCL_CalIfSig_GreatThan(Diff.Data, 18);
  if(Diff.Data.F >= 0)
  {
    AC_BIND_BOOL_FALSE.Data.F = CONST_BOOL_FALSE.Data.F;
    AC_BIND_BOOL_FALSE.Data.C.C1 = CONST_BOOL_FALSE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_FALSE.Data.C.C2 = CONST_BOOL_FALSE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 12, 12);
  }
  else
  {
    AC_BIND_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;
    AC_BIND_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 14, 14);

    IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 7614887, P1);
    IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 6735595, P2);
  }
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);
  IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);
  IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);
  return IfOutVar.Data;
}
T_DATA F_VCL_Equal(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)
{
  T_CODE Diff,IfOutVar;
  INT32 AdjTmpVar=0;
  Diff.Data=F_VCL_Sub(inX,inY,inSubComp1,inSubComp2);//Diff's sig
  AC_BIND_CONST_NUM_0x00000000.Data.F = CONST_NUM_0x00000000.Data.F;
  AC_BIND_CONST_NUM_0x00000000.Data.C.C1 = CONST_NUM_0x00000000.Data.C.C1 + g_GS_TL1;
  AC_BIND_CONST_NUM_0x00000000.Data.C.C2 = CONST_NUM_0x00000000.Data.C.C2 + g_GS_TL2;
  T_CODE Diff2;
  Diff2.Data = F_VCL_Sub(AC_BIND_CONST_NUM_0x00000000.Data, Diff.Data, 39, 39);
  T_CHECK OutSig = F_VCL_CalExitSig(Diff.Data, Diff2.Data, 23);
  if(Diff.Data.F == 0)
  {
    AC_BIND_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;
    AC_BIND_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 18, 18);
  }
  else
  {
    AC_BIND_BOOL_FALSE.Data.F = CONST_BOOL_FALSE.Data.F;
    AC_BIND_BOOL_FALSE.Data.C.C1 = CONST_BOOL_FALSE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_FALSE.Data.C.C2 = CONST_BOOL_FALSE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 18, 18);

    IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 1083229, P1);
    IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 3166853, P2);
  }
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);
  IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);
  IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);
  return IfOutVar.Data;
}
T_DATA F_VCL_NotEqual(T_DATA inX,T_DATA inY,int inSubComp1,int inSubComp2,int inComp1,int inComp2)
{
  T_CODE Diff,IfOutVar;
  INT32 AdjTmpVar=0;
  Diff.Data=F_VCL_Sub(inX,inY,inSubComp1,inSubComp2);//Diff's sig
  AC_BIND_CONST_NUM_0x00000000.Data.F = CONST_NUM_0x00000000.Data.F;
  AC_BIND_CONST_NUM_0x00000000.Data.C.C1 = CONST_NUM_0x00000000.Data.C.C1 + g_GS_TL1;
  AC_BIND_CONST_NUM_0x00000000.Data.C.C2 = CONST_NUM_0x00000000.Data.C.C2 + g_GS_TL2;
  T_CODE Diff2;
  Diff2.Data = F_VCL_Sub(AC_BIND_CONST_NUM_0x00000000.Data, Diff.Data, 49, 49);
  T_CHECK OutSig = F_VCL_CalExitSig(Diff.Data, Diff2.Data, 28);
  if(Diff.Data.F == 0)
  {
    AC_BIND_BOOL_FALSE.Data.F = CONST_BOOL_FALSE.Data.F;
    AC_BIND_BOOL_FALSE.Data.C.C1 = CONST_BOOL_FALSE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_FALSE.Data.C.C2 = CONST_BOOL_FALSE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_FALSE.Data, 22, 22);
  }
  else
  {
    AC_BIND_BOOL_TRUE.Data.F = CONST_BOOL_TRUE.Data.F;
    AC_BIND_BOOL_TRUE.Data.C.C1 = CONST_BOOL_TRUE.Data.C.C1 + g_GS_TL1;
    AC_BIND_BOOL_TRUE.Data.C.C2 = CONST_BOOL_TRUE.Data.C.C2 + g_GS_TL2;
    IfOutVar.Data = F_VCL_Assign(AC_BIND_BOOL_TRUE.Data, 24, 24);

    IfOutVar.Data.C.C1 = AC_64bit_Add(IfOutVar.Data.C.C1, 7521928, P1);
    IfOutVar.Data.C.C2 = AC_64bit_Add(IfOutVar.Data.C.C2, 11837528, P2);
  }
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C1,OutSig.C1,P1);
  IfOutVar.Data.C.C1=AC_64bit_Add(AdjTmpVar,inComp1,P1);
  AdjTmpVar=AC_64bit_Add(IfOutVar.Data.C.C2,OutSig.C2,P1);
  IfOutVar.Data.C.C2=AC_64bit_Add(AdjTmpVar,inComp2,P2);
  return IfOutVar.Data;
}
