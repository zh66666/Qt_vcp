#include<math.h>
#include <stdlib.h>

int fun(int a)
{
int b;
b=a+1;
return b;
}

void fun1()
{
}
void fun2(int a,int b)
{
}
 int  main(void )
{
int a=1;
int c;
a=2;
c=a;
c=a+1;
c=fun(a);
fun1();
return c;
}
