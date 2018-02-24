typedef int  Position;

typedef int  Player;

empty int  aPlayerList[580];

empty int  move_box(int _Action,int _PlayerId,int _Label)
{
  empty int  flag = 2;

  if(_Action==0)
  {
    if(_Label==1||(aPlayerList[29*_PlayerId+27+1]<10&&_PlayerId<10))
    {
      if(aPlayerList[29*_PlayerId+27+1]>0)
      {
        aPlayerList[29*_PlayerId+27+1] = aPlayerList[29*_PlayerId+27+1]-1;
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
  else  if(_Action==1)
  {
    if(_Label==1||(aPlayerList[29*_PlayerId+27+1]>0&&_PlayerId>4))
    {
      if(aPlayerList[29*_PlayerId+27+1]<10)
      {
        aPlayerList[29*_PlayerId+27+1] = aPlayerList[29*_PlayerId+27+1]+1;
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
  else  if(_Action==2)
  {
    if(_Label==1||(aPlayerList[29*_PlayerId+27]<10&&_PlayerId>=2))
    {
      if(aPlayerList[29*_PlayerId+27]>0)
      {
        aPlayerList[29*_PlayerId+27] = aPlayerList[29*_PlayerId+27]-1;
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
    if(_Label==1||(aPlayerList[29*_PlayerId+27]>0&&_PlayerId<=19))
    {
      if(aPlayerList[29*_PlayerId+27]<10)
      {
        aPlayerList[29*_PlayerId+27] = aPlayerList[29*_PlayerId+27]+1;
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


  return flag;
}
empty void  output()
{
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

  while(i<MaxBagKind)
  {
    while(j<MaxBagId)
    {
      if(iPutId<2)
      {
        iPutId = 9;
      }
      iInput = 0;
      iPlayerId = 0;
      while(iPlayerId<MaxPlayer)
      {
        if(iPlayerId<10)
        {
          aPlayerList[29*iPlayerId+1+5*i+j] = 2;
        }
        else
        {
          if(iPutId<=3)
          {
            aPlayerList[29*iPlayerId+1+5*i+j] = 1;
          }
          else
          {
            iPutId = iPutId+1;
          }
        }
        aPlayerList[29*iPlayerId+27] = iInput;
        aPlayerList[29*iPlayerId+27+1] = iInput;
        iPlayerId = iPlayerId+1;
        iInput = iInput+1;
      }
      j = j+1;
    }
    i = i+1;
  }
  iPlayerId = 0;
  while(iPlayerId<MaxPlayer)
  {
    iRet = move_box(iActionId,iPlayerId,iLabel);
    if(iRet==0)
    {
      output();
    }
    iActionId = iActionId+1;
    if(iActionId>3)
    {
      iActionId = 0;
    }
    iPlayerId = iPlayerId+1;
    if(iLabel==1)
    {
      iLabel = 0;
    }
    else
    {
      iLabel = 1;
    }
  }
  return 0;
}
