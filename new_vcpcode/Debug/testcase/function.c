int fun(int a, int b);

void main(){
	int a = 10, b = 20, c;
	a = a + b;
	c = fun(a, b);
	c = c * a;
}

int fun(int a, int b){
	int c = a + b;
	return c;
}
