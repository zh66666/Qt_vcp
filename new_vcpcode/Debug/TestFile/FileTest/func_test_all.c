
int g = 0;
int func1(int a);
int func2();
void func3(int a);
void func4();
/*with param & with return*/
int func1(int a)
{
	int b = 1;
	b = b + g;
	return b;
}
/*no param & with return*/
int func2()
{
	int b = 1;
	return b;
}
/*with param & no return*/
void func3(int a)
{
	int c = 1; 
	int f = 1;
	f = c;
	g = f;
}
/*no param & no return*/
void func4()
{
	int f = 1;
	f = g;
}


/*call function*/
void func()
{
	int a = 1, b = 2;
	func1(a);
	a = func1(1);
	func2();
	a = func2();
	func3(a);
	func3(2);
	func4();
	if(a > 1)
	{
		func1(a);
		a = func1(1);
		func2();
		if(b > 1)
		{
			a = func2();
			func3(a);
			while(a > 1)
			{
				func3(2);
				func4();
				a = func2();
			}
		}
		else
		{
			func3(2);
			func4();
		}		
	}
	else
	{
		while(b > 1)
		{
			a = func2();
			func3(a);
			func3(2);
			b = b - 1;
		}
	}
}

int main()
{
	func();
	return 0;
}

