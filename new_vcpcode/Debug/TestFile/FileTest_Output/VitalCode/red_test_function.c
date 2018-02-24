#include "Interface.h" 
#include "Interface.c" 
long long RS=30811;
int S_Fun_entry=30811;
int S_Fun_out;
int fun(long long a_1,long long a_2, long long b_1,long long b_2);
int main(void)
{
  long long a_1=10*A1;
  long long a_2=10*A2;
  long long b_1=20*A1;
  long long b_2=20*A2;
  long long c_1=0*A1;
  long long c_2=0*A2;

  int S_Fun_out_this=S_Fun_out;
  P_pre_idx=31;
  RS=RS+2664-S_Fun_entry;

  P_pre_idx=32;
  a_1=a_1+b_1;
  a_2=a_2+b_2+(22059);
  RS=RS+a_2+a_1;
  a_2=a_2-(22059);

  S_Fun_entry=24723;
  P_pre_idx=33;
  S_Fun_out=30350;

  FUN_RET=fun(a_1,a_2,b_1,b_2);
    
  P_pre_idx=34;
  RS=RS+(-955);
  
  P_pre_idx=35;
  c_1=FUN_RET*8193;
  c_2=FUN_RET*8191+(-1664);
  RS=RS+c_2+c_1;
  c_2=c_2-(-1664);

  P_pre_idx=36;
  mul_temp_A_1=c_1*a_1;
  mul_temp_A_2=c_1*a_2+(3523);
  mul_temp_B_1=c_2*a_1;
  mul_temp_B_2=c_2*a_2;
  c_1=(mul_temp_A_1+mul_temp_A_2-(3523))>>14;
  c_2=((mul_temp_B_1-mul_temp_B_2)>>1)+(3523);
  RS=RS+c_2+c_1;
  c_2=c_2-(3523);

  S_Fun_out=S_Fun_out_this;
  RS=RS+S_Fun_out_this-31254;

return 0;
}
int fun(long long a_1,long long a_2, long long b_1,long long b_2)
{
  long long c_1=0*A1;
  long long c_2=0*A2;


  int S_Fun_out_this=S_Fun_out;
  P_pre_idx=37;
  RS=RS+21113-S_Fun_entry;

  P_pre_idx=38;
  c_1=a_1+b_1;
  c_2=a_2+b_2+(-8062);
  RS=RS+c_2+c_1;
  c_2=c_2-(-8062);

  S_Fun_out=S_Fun_out_this;
  RS=RS+S_Fun_out_this-13051;

long long c=check_decode (c_1,c_2);
return c;
  }
