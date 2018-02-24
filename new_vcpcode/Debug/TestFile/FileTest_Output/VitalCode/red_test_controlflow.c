#include "Interface.h" 
#include "Interface.c" 
long long RS=5957;
int S_Fun_entry=5957;
int S_Fun_out;
int main(void)
{
  long long i_1=10*A1;
  long long i_2=10*A2;
  long long a_1=1*A1;
  long long a_2=1*A2;
  long long b_1=2*A1;
  long long b_2=2*A2;
  long long c_1=0*A1;
  long long c_2=0*A2;

  int S_Fun_out_this=S_Fun_out;
  P_pre_idx=18;
  RS=RS+32539-S_Fun_entry;

  P_pre_idx=19;
  c_1=a_1+b_1;
  c_2=a_2+b_2+(-7145);
  RS=RS+c_2+c_1;
  c_2=c_2-(-7145);

  P_pre_idx=20;
  AC_Tmp_1_1=a_1-2*A1;
  AC_Tmp_1_2=a_2-2*A2+(-21981);
  RS=RS+AC_Tmp_1_2+AC_Tmp_1_1;
  AC_Tmp_1_2=AC_Tmp_1_2-(-21981);

  Select_decode=check_decode(AC_Tmp_1_1,AC_Tmp_1_2);
  Select_mark=(long long) Select_decode>>32;
  
  if(Select_decode>=0)
  {
    P_pre_idx=21;
    c_1=a_1+b_1;
    c_2=a_2+b_2+(17107);
    RS=RS+c_2+c_1;
    c_2=c_2-(17107);

    P_pre_idx=22;
    a_1=b_1+c_1;
    a_2=b_2+c_2+(-6527);
    RS=RS+a_2+a_1;
    a_2=a_2-(-6527);

  }
  else
  {
    P_pre_idx=23;
    c_1=a_1-b_1;
    c_2=a_2-b_2+(20692);
    RS=RS+c_2+c_1;
    c_2=c_2-(20692);

  }
  S_select=(13993&(~Select_mark))^(24105&Select_mark);
  
  P_pre_idx=24;
  RS=RS+(24269)-S_select;
  
  P_pre_idx=25;
  c_1=a_1+b_1;
  c_2=a_2+b_2+(-16356);
  RS=RS+c_2+c_1;
  c_2=c_2-(-16356);

  P_pre_idx=26;
  AC_Tmp_2_1=i_1-0*A1;
  AC_Tmp_2_2=i_2-0*A2+(17224);
  RS=RS+AC_Tmp_2_2+AC_Tmp_2_1;
  AC_Tmp_2_2=AC_Tmp_2_2-(17224);

  Iterat_decode=check_decode(AC_Tmp_2_1,AC_Tmp_2_2);
  
  while(Iterat_decode>=0)
  {
    P_pre_idx=27;
    a_1=a_1+1*A1;
    a_2=a_2+1*A2+(3651);
    RS=RS+a_2+a_1;
    a_2=a_2-(3651);

    P_pre_idx=28;
    i_1=i_1-1*A1;
    i_2=i_2-1*A2+(-7149);
    RS=RS+i_2+i_1;
    i_2=i_2-(-7149);

    P_pre_idx=29;
    AC_Tmp_2_1=i_1-0*A1;
    AC_Tmp_2_2=i_2-0*A2+(8611);
    RS=RS+AC_Tmp_2_2+AC_Tmp_2_1;
    AC_Tmp_2_2=AC_Tmp_2_2-(8611);

    RS=RS-(30250)+(25137&(~Iterat_mark))^(30250&Iterat_mark);
    
    Iterat_decode=check_decode(AC_Tmp_2_1,AC_Tmp_2_2);
    Iterat_mark=(long long) Iterat_decode>>32;
    
  }
RS=RS-(25137)+(25137&(~Iterat_mark))^(30250&Iterat_mark);
  
  P_pre_idx=30;
  c_1=a_1+b_1;
  c_2=a_2+b_2+(561);
  RS=RS+c_2+c_1;
  c_2=c_2-(561);

  S_Fun_out=S_Fun_out_this;
  RS=RS+S_Fun_out_this-30811;

long long c=check_decode (c_1,c_2);
return c;
  }
