int fun(int a, int b);

void main(){
	int a = 10, b = 20, c;
	c = a + b;
	while(c >= 0){
		if(c >= 10){
			a = a + 10;
		}else{
			b = b - 2;
		}
		c = c - 1;
	}
	c = fun(a, b);
	c = c * a;
}

int fun(int a, int b){
	int c = a + b;
	return c;
}
