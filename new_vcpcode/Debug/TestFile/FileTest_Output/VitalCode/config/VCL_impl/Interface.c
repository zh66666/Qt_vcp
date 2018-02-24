
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

/*long long add_enc(long long x, long long y)
 {
	return x + y;
}
long long sub_enc(long long x, long long y)
 {
	return x - y;
}

long long mul_enc(long long x1, long long y1,long long x2, long long y2,long long A)
 {
if(A==A1)
{
long long M1,M2,M0;
M1=x1*y1;
M2=x1*y2;
M0=M1+M2;
return M0>>14;
}
if(A==A2)
{
long long N1,N2,N0;
N1=x2*y1;
N2=x2*y2;
N0=N1-N2;
return N0>>1;
}
}
long long div_enc(long long x1, long long y1,long long x2, long long y2,long long A)
{
if(A==A1)
{
long long M1,M2,M_a;
M1=(x1*A1)/(y1/A1);
M2=(x1*A2)/(y1/A2);
M_a=M1+M2;
return  M_a>>14;
}
if(A==A2)
{
long long N1,N2,N_a;
N1=(x2*A1)/(y1/A1);
N2=(x2*A2)/(y2/A2);
N_a=N1-N2;
return  N_a>>1;
}
}*/
void error()
{ printf("error\n");

}
int check_decode (long long x1, long long x2)
//k��ʶ�Ƿ���Ҫ���룬��k=0������Ҫ����
 {long long M1,M2,N1,N2;
M1=x1+x2;
M2=x1-x2;
N1=M1&0x3F;//��ȡ��14λ
N2=M2&0x01;//��ȡ��һλ
int x_a=M1>>14;
int x_b=M2>>1;
if(N1!=0||N2!=0||x_a!=x_b) //����У��
{error();//����������ֹ
}
return x_a;
}

