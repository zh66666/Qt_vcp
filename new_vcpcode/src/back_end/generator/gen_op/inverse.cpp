// 功能：在指定的有限域中，求某个整数的逆元，如果有限域为1或者0，输出警告信息，
//			如果待求逆元的整数为0，无逆元，输出警告信息
// 输入参数：num-待求逆元的整数
//			      prime-有限域
// 返回值：成功则返回计算所得的逆元
// 作者：丁佳
long long inverse(long long num,const long long prime)
{
	long long d,x1,y1,x2,y2,q,r,x,y;
	long long a=prime;
	long long z;
	if (0==num)
	{
		return 0;
	}

	x2=1;
	x1=0;
	y2=0;
	y1=1;
	num=(num%a+a)%a;

	while(num>0)
	{
		q=a/num;
		r=a-q*num;
		x=x2-q*x1;
		y=y2-q*y1;
 		a=num;
		num=r;
		x2=x1;
		x1=x;
		y2=y1;
		y1=y;
	}

	d=a;
	x=x2;
	y=y2;

	y=((y%prime)+prime)%prime;
	z=y;
	return z;
} 



