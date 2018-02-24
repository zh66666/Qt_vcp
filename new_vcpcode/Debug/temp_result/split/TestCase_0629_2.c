typedef int  Position;

typedef int  Player;

empty int  aPlayerList[580];

empty int  move_box(int _Action,int _PlayerId,int _Label)
{
  empty int  flag = 2;

  AC_Tmp_1 = _Action==0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = _Label==1;
    AC_Tmp_3 = _PlayerId*29;
    AC_Tmp_4 = AC_Tmp_3+28;
    AC_Tmp_5 = aPlayerList[AC_Tmp_4];
    AC_Tmp_6 = AC_Tmp_5<10;
    AC_Tmp_7 = _PlayerId<10;
    AC_Tmp_8 = AC_Tmp_6&&AC_Tmp_7;
    AC_Tmp_9 = AC_Tmp_2||AC_Tmp_8;
    AC_IfSub_3 = AC_Tmp_9-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_9==e_TRUE)
    {
      AC_Tmp_10 = _PlayerId*29;
      AC_Tmp_11 = AC_Tmp_10+28;
      AC_Tmp_12 = aPlayerList[AC_Tmp_11];
      AC_Tmp_13 = AC_Tmp_12>0;
      AC_IfSub_5 = AC_Tmp_13-e_TRUE;
      AC_IfSub_6 = 0-AC_IfSub_5;
      if(AC_Tmp_13==e_TRUE)
      {
        AC_Tmp_14 = _PlayerId*29;
        AC_Tmp_15 = AC_Tmp_14+28;
        AC_Tmp_16 = _PlayerId*29;
        AC_Tmp_17 = AC_Tmp_16+28;
        AC_Tmp_18 = aPlayerList[AC_Tmp_17];
        AC_Tmp_19 = AC_Tmp_18-1;
        aPlayerList[AC_Tmp_15] = AC_Tmp_19;
      }
      else
      {
        flag = 0;
      }
    }
    else
    {
      flag = 1;
    }
  }
  else
  {
    AC_Tmp_20 = _Action==1;
    AC_IfSub_7 = AC_Tmp_20-e_TRUE;
    AC_IfSub_8 = 0-AC_IfSub_7;
    if(AC_Tmp_20==e_TRUE)
    {
      AC_Tmp_21 = _Label==1;
      AC_Tmp_22 = _PlayerId*29;
      AC_Tmp_23 = AC_Tmp_22+28;
      AC_Tmp_24 = aPlayerList[AC_Tmp_23];
      AC_Tmp_25 = AC_Tmp_24>0;
      AC_Tmp_26 = _PlayerId>4;
      AC_Tmp_27 = AC_Tmp_25&&AC_Tmp_26;
      AC_Tmp_28 = AC_Tmp_21||AC_Tmp_27;
      AC_IfSub_9 = AC_Tmp_28-e_TRUE;
      AC_IfSub_10 = 0-AC_IfSub_9;
      if(AC_Tmp_28==e_TRUE)
      {
        AC_Tmp_29 = _PlayerId*29;
        AC_Tmp_30 = AC_Tmp_29+28;
        AC_Tmp_31 = aPlayerList[AC_Tmp_30];
        AC_Tmp_32 = AC_Tmp_31<10;
        AC_IfSub_11 = AC_Tmp_32-e_TRUE;
        AC_IfSub_12 = 0-AC_IfSub_11;
        if(AC_Tmp_32==e_TRUE)
        {
          AC_Tmp_33 = _PlayerId*29;
          AC_Tmp_34 = AC_Tmp_33+28;
          AC_Tmp_35 = _PlayerId*29;
          AC_Tmp_36 = AC_Tmp_35+28;
          AC_Tmp_37 = aPlayerList[AC_Tmp_36];
          AC_Tmp_38 = AC_Tmp_37+1;
          aPlayerList[AC_Tmp_34] = AC_Tmp_38;
        }
        else
        {
          flag = 0;
        }
      }
      else
      {
        flag = 1;
      }
    }
    else
    {
      AC_Tmp_39 = _Action==2;
      AC_IfSub_13 = AC_Tmp_39-e_TRUE;
      AC_IfSub_14 = 0-AC_IfSub_13;
      if(AC_Tmp_39==e_TRUE)
      {
        AC_Tmp_40 = _Label==1;
        AC_Tmp_41 = _PlayerId*29;
        AC_Tmp_42 = AC_Tmp_41+27;
        AC_Tmp_43 = aPlayerList[AC_Tmp_42];
        AC_Tmp_44 = AC_Tmp_43<10;
        AC_Tmp_45 = _PlayerId>=2;
        AC_Tmp_46 = AC_Tmp_44&&AC_Tmp_45;
        AC_Tmp_47 = AC_Tmp_40||AC_Tmp_46;
        AC_IfSub_15 = AC_Tmp_47-e_TRUE;
        AC_IfSub_16 = 0-AC_IfSub_15;
        if(AC_Tmp_47==e_TRUE)
        {
          AC_Tmp_48 = _PlayerId*29;
          AC_Tmp_49 = AC_Tmp_48+27;
          AC_Tmp_50 = aPlayerList[AC_Tmp_49];
          AC_Tmp_51 = AC_Tmp_50>0;
          AC_IfSub_17 = AC_Tmp_51-e_TRUE;
          AC_IfSub_18 = 0-AC_IfSub_17;
          if(AC_Tmp_51==e_TRUE)
          {
            AC_Tmp_52 = _PlayerId*29;
            AC_Tmp_53 = AC_Tmp_52+27;
            AC_Tmp_54 = _PlayerId*29;
            AC_Tmp_55 = AC_Tmp_54+27;
            AC_Tmp_56 = aPlayerList[AC_Tmp_55];
            AC_Tmp_57 = AC_Tmp_56-1;
            aPlayerList[AC_Tmp_53] = AC_Tmp_57;
          }
          else
          {
            flag = 0;
          }
        }
        else
        {
          flag = 1;
        }
      }
      else
      {
        AC_Tmp_58 = _Label==1;
        AC_Tmp_59 = _PlayerId*29;
        AC_Tmp_60 = AC_Tmp_59+27;
        AC_Tmp_61 = aPlayerList[AC_Tmp_60];
        AC_Tmp_62 = AC_Tmp_61>0;
        AC_Tmp_63 = _PlayerId<=19;
        AC_Tmp_64 = AC_Tmp_62&&AC_Tmp_63;
        AC_Tmp_65 = AC_Tmp_58||AC_Tmp_64;
        AC_IfSub_19 = AC_Tmp_65-e_TRUE;
        AC_IfSub_20 = 0-AC_IfSub_19;
        if(AC_Tmp_65==e_TRUE)
        {
          AC_Tmp_66 = _PlayerId*29;
          AC_Tmp_67 = AC_Tmp_66+27;
          AC_Tmp_68 = aPlayerList[AC_Tmp_67];
          AC_Tmp_69 = AC_Tmp_68<10;
          AC_IfSub_21 = AC_Tmp_69-e_TRUE;
          AC_IfSub_22 = 0-AC_IfSub_21;
          if(AC_Tmp_69==e_TRUE)
          {
            AC_Tmp_70 = _PlayerId*29;
            AC_Tmp_71 = AC_Tmp_70+27;
            AC_Tmp_72 = _PlayerId*29;
            AC_Tmp_73 = AC_Tmp_72+27;
            AC_Tmp_74 = aPlayerList[AC_Tmp_73];
            AC_Tmp_75 = AC_Tmp_74+1;
            aPlayerList[AC_Tmp_71] = AC_Tmp_75;
          }
          else
          {
            flag = 0;
          }
        }
        else
        {
          flag = 1;
        }
      }
    }
  }
  return flag;
}
empty int  output()
{
  return 0;
}
empty int  main()
{
  empty int  i = 0,j = 0;

  empty int  MaxBagKind = 5,MaxBagId = 5;

  empty int  iPlayerId = 0;

  empty int  MaxPlayer = 20;

  empty int  iActionId = 0;

  empty int  iLabel = 0;

  empty int  iPutId = 0;

  empty int  iInput = 0;

  empty int  iRet = 0;

  AC_Tmp_1 = i<MaxBagKind;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = j<MaxBagId;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      AC_Tmp_3 = iPutId<2;
      AC_IfSub_1 = AC_Tmp_3-e_TRUE;
      AC_IfSub_2 = 0-AC_IfSub_1;
      if(AC_Tmp_3==e_TRUE)
      {
        iPutId = 9;
      }
      iInput = 0;
      iPlayerId = 0;
      AC_Tmp_4 = iPlayerId<MaxPlayer;
      AC_WhileSub_5 = AC_Tmp_4-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
      while(AC_Tmp_4==e_TRUE)
      {
        AC_Tmp_5 = iPlayerId<10;
        AC_IfSub_3 = AC_Tmp_5-e_TRUE;
        AC_IfSub_4 = 0-AC_IfSub_3;
        if(AC_Tmp_5==e_TRUE)
        {
          AC_Tmp_6 = iPlayerId*29;
          AC_Tmp_7 = i*5;
          AC_Tmp_8 = AC_Tmp_6+AC_Tmp_7;
          AC_Tmp_9 = AC_Tmp_8+j;
          AC_Tmp_10 = AC_Tmp_9+1;
          aPlayerList[AC_Tmp_10] = 2;
        }
        else
        {
          AC_Tmp_11 = iPutId<=3;
          AC_IfSub_5 = AC_Tmp_11-e_TRUE;
          AC_IfSub_6 = 0-AC_IfSub_5;
          if(AC_Tmp_11==e_TRUE)
          {
            AC_Tmp_12 = iPlayerId*29;
            AC_Tmp_13 = i*5;
            AC_Tmp_14 = AC_Tmp_12+AC_Tmp_13;
            AC_Tmp_15 = AC_Tmp_14+j;
            AC_Tmp_16 = AC_Tmp_15+1;
            aPlayerList[AC_Tmp_16] = 1;
          }
          else
          {
            iPutId = iPutId+1;
          }
        }
        AC_Tmp_17 = iPlayerId*29;
        AC_Tmp_18 = AC_Tmp_17+27;
        aPlayerList[AC_Tmp_18] = iInput;
        AC_Tmp_19 = iPlayerId*29;
        AC_Tmp_20 = AC_Tmp_19+28;
        aPlayerList[AC_Tmp_20] = iInput;
        iPlayerId = iPlayerId+1;
        iInput = iInput+1;
        AC_Tmp_4 = iPlayerId<MaxPlayer;
        AC_WhileSub_5 = AC_Tmp_4-e_TRUE;
        AC_WhileSub_6 = 0-AC_WhileSub_5;
      }
      j = j+1;
      AC_Tmp_2 = j<MaxBagId;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    i = i+1;
    AC_Tmp_1 = i<MaxBagKind;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  iPlayerId = 0;
  AC_Tmp_21 = iPlayerId<MaxPlayer;
  AC_WhileSub_7 = AC_Tmp_21-e_TRUE;
  AC_WhileSub_8 = 0-AC_WhileSub_7;
  while(AC_Tmp_21==e_TRUE)
  {
    iRet = move_box(iActionId,iPlayerId,iLabel);
    AC_Tmp_22 = iRet==0;
    AC_IfSub_7 = AC_Tmp_22-e_TRUE;
    AC_IfSub_8 = 0-AC_IfSub_7;
    if(AC_Tmp_22==e_TRUE)
    {
      AC_Void_Func_RetVal = output();
    }
    iActionId = iActionId+1;
    AC_Tmp_23 = iActionId>3;
    AC_IfSub_9 = AC_Tmp_23-e_TRUE;
    AC_IfSub_10 = 0-AC_IfSub_9;
    if(AC_Tmp_23==e_TRUE)
    {
      iActionId = 0;
    }
    iPlayerId = iPlayerId+1;
    AC_Tmp_24 = iLabel==1;
    AC_IfSub_11 = AC_Tmp_24-e_TRUE;
    AC_IfSub_12 = 0-AC_IfSub_11;
    if(AC_Tmp_24==e_TRUE)
    {
      iLabel = 0;
    }
    else
    {
      iLabel = 1;
    }
    AC_Tmp_21 = iPlayerId<MaxPlayer;
    AC_WhileSub_7 = AC_Tmp_21-e_TRUE;
    AC_WhileSub_8 = 0-AC_WhileSub_7;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
