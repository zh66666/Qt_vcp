// ���ܣ���ָ�����������У���ĳ����������Ԫ�����������Ϊ1����0�����������Ϣ��
//			���������Ԫ������Ϊ0������Ԫ�����������Ϣ
// ���������num-������Ԫ������
//			      prime-������
// ����ֵ���ɹ��򷵻ؼ������õ���Ԫ
// ���ߣ�����
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



