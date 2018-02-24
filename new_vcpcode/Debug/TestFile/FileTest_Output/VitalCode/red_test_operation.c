#include "Interface.h" 
#include "Interface.c" 
long long RS=3197;
int S_Fun_entry=3197;
int S_Fun_out;
int main(void)
{
  long long a_1=0*A1;
  long long a_2=0*A2;
  long long b_1=0*A1;
  long long b_2=0*A2;
  long long c_1=0*A1;
  long long c_2=0*A2;
  long long d_1=0*A1;
  long long d_2=0*A2;

  int S_Fun_out_this=S_Fun_out;
  P_pre_idx=1;
  RS=RS+18881-S_Fun_entry;

  P_pre_idx=2;
  a_1=2*A1+3*A1;
  a_2=2*A2+3*A2+(-12124);
  RS=RS+a_2+a_1;
  a_2=a_2-(-12124);

  P_pre_idx=3;
  b_1=a_1+1*A1;
  b_2=a_2+1*A2+(1463);
  RS=RS+b_2+b_1;
  b_2=b_2-(1463);

  P_pre_idx=4;
  c_1=2*A1+b_1;
  c_2=2*A2+b_2+(5414);
  RS=RS+c_2+c_1;
  c_2=c_2-(5414);

  P_pre_idx=5;
  d_1=b_1+c_1;
  d_2=b_2+c_2+(-6629);
  RS=RS+d_2+d_1;
  d_2=d_2-(-6629);

  P_pre_idx=6;
  a_1=2*A1-3*A1;
  a_2=2*A2-3*A2+(21430);
  RS=RS+a_2+a_1;
  a_2=a_2-(21430);

  P_pre_idx=7;
  b_1=a_1-1*A1;
  b_2=a_2-1*A2+(-12611);
  RS=RS+b_2+b_1;
  b_2=b_2-(-12611);

  P_pre_idx=8;
  c_1=2*A1-b_1;
  c_2=2*A2-b_2+(-7584);
  RS=RS+c_2+c_1;
  c_2=c_2-(-7584);

  P_pre_idx=9;
  d_1=b_1-c_1;
  d_2=b_2-c_2+(10374);
  RS=RS+d_2+d_1;
  d_2=d_2-(10374);

  P_pre_idx=10;
  mul_temp_A_1=2*A1*3*A1;
  mul_temp_A_2=2*A1*3*A2+(9511);
  mul_temp_B_1=2*A2*3*A1;
  mul_temp_B_2=2*A2*3*A2;
  a_1=(mul_temp_A_1+mul_temp_A_2-(9511))>>14;
  a_2=((mul_temp_B_1-mul_temp_B_2)>>1)+(9511);
  RS=RS+a_2+a_1;
  a_2=a_2-(9511);

  P_pre_idx=11;
  mul_temp_A_1=a_1*1*A1;
  mul_temp_A_2=a_1*1*A2+(-13487);
  mul_temp_B_1=a_2*1*A1;
  mul_temp_B_2=a_2*1*A2;
  b_1=(mul_temp_A_1+mul_temp_A_2-(-13487))>>14;
  b_2=((mul_temp_B_1-mul_temp_B_2)>>1)+(-13487);
  RS=RS+b_2+b_1;
  b_2=b_2-(-13487);

  P_pre_idx=12;
  mul_temp_A_1=2*A1*b_1;
  mul_temp_A_2=2*A1*b_2+(-5904);
  mul_temp_B_1=2*A2*b_1;
  mul_temp_B_2=2*A2*b_2;
  c_1=(mul_temp_A_1+mul_temp_A_2-(-5904))>>14;
  c_2=((mul_temp_B_1-mul_temp_B_2)>>1)+(-5904);
  RS=RS+c_2+c_1;
  c_2=c_2-(-5904);

  P_pre_idx=13;
  mul_temp_A_1=b_1*c_1;
  mul_temp_A_2=b_1*c_2+(13052);
  mul_temp_B_1=b_2*c_1;
  mul_temp_B_2=b_2*c_2;
  d_1=(mul_temp_A_1+mul_temp_A_2-(13052))>>14;
  d_2=((mul_temp_B_1-mul_temp_B_2)>>1)+(13052);
  RS=RS+d_2+d_1;
  d_2=d_2-(13052);

  P_pre_idx=14;
  a_1=(3*A1)/(2*A1)*A1;
  a_2=(3*A2)/(2*A2)*A2+(-3501);
  div_temp_var_decode=check_decode(a_1,a_2-(-3501))+(-3501);
  div_temp_l=check_decode(3*A1,3*A2);
  div_temp_r=check_decode(2*A1,2*A2);
  div_temp_var=div_temp_l/div_temp_r;
  RS=RS+div_temp_var-div_temp_var_decode;
  a_2=a_2-(-3501);
  
  P_pre_idx=15;
  b_1=(a_1)/(1*A1)*A1;
  b_2=(a_2)/(1*A2)*A2+(-14047);
  div_temp_var_decode=check_decode(b_1,b_2-(-14047))+(-14047);
  div_temp_l=check_decode(a_1,a_2);
  div_temp_r=check_decode(1*A1,1*A2);
  div_temp_var=div_temp_l/div_temp_r;
  RS=RS+div_temp_var-div_temp_var_decode;
  b_2=b_2-(-14047);
  
  P_pre_idx=16;
  c_1=(2*A1)/(b_1)*A1;
  c_2=(2*A2)/(b_2)*A2+(16003);
  div_temp_var_decode=check_decode(c_1,c_2-(16003))+(16003);
  div_temp_l=check_decode(2*A1,2*A2);
  div_temp_r=check_decode(b_1,b_2);
  div_temp_var=div_temp_l/div_temp_r;
  RS=RS+div_temp_var-div_temp_var_decode;
  c_2=c_2-(16003);
  
  P_pre_idx=17;
  d_1=(b_1)/(c_1)*A1;
  d_2=(b_2)/(c_2)*A2+(-14284);
  div_temp_var_decode=check_decode(d_1,d_2-(-14284))+(-14284);
  div_temp_l=check_decode(b_1,b_2);
  div_temp_r=check_decode(c_1,c_2);
  div_temp_var=div_temp_l/div_temp_r;
  RS=RS+div_temp_var-div_temp_var_decode;
  d_2=d_2-(-14284);
  
  S_Fun_out=S_Fun_out_this;
  RS=RS+S_Fun_out_this-5957;

long long c=check_decode (c_1,c_2);
return c;
  }
