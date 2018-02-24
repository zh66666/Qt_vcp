typedef struct TagPosition
{
  empty int  m_x;
  empty int  m_y;
}  Position;

typedef struct TagPlayer
{
  empty int  m_id;
  empty int  m_bag[5][5];
  empty int  m_age;
  empty Position  m_Pos;
}  Player;

empty Player  aPlayerList[20];

empty int  move_box(int _Action,int _PlayerId,int _Label)
{
  empty int  flag = 2;

  if(_Action==0)
  {
    if(_Label==1||(aPlayerList[_PlayerId].m_Pos.m_y<10&&_PlayerId<10))
    {
      if(aPlayerList[_PlayerId].m_Pos.m_y>0)
      {
        aPlayerList[_PlayerId].m_Pos.m_y = aPlayerList[_PlayerId].m_Pos.m_y-1;
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
    if(_Label==1||(aPlayerList[_PlayerId].m_Pos.m_y>0&&_PlayerId>4))
    {
      if(aPlayerList[_PlayerId].m_Pos.m_y<10)
      {
        aPlayerList[_PlayerId].m_Pos.m_y = aPlayerList[_PlayerId].m_Pos.m_y+1;
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
    if(_Label==1||(aPlayerList[_PlayerId].m_Pos.m_x<10&&_PlayerId>=2))
    {
      if(aPlayerList[_PlayerId].m_Pos.m_x>0)
      {
        aPlayerList[_PlayerId].m_Pos.m_x = aPlayerList[_PlayerId].m_Pos.m_x-1;
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
    if(_Label==1||(aPlayerList[_PlayerId].m_Pos.m_x>0&&_PlayerId<=19))
    {
      if(aPlayerList[_PlayerId].m_Pos.m_x<10)
      {
        aPlayerList[_PlayerId].m_Pos.m_x = aPlayerList[_PlayerId].m_Pos.m_x+1;
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
          aPlayerList[iPlayerId].m_bag[i][j] = 2;
        }
        else
        {
          if(iPutId<=3)
          {
            aPlayerList[iPlayerId].m_bag[i][j] = 1;
          }
          else
          {
            iPutId = iPutId+1;
          }
        }
        aPlayerList[iPlayerId].m_Pos.m_x = iInput;
        aPlayerList[iPlayerId].m_Pos.m_y = iInput;
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
