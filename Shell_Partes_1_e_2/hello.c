#include <stdio.h>
#include <stdlib.h>

char findChar(int n){
	char v;
	if(n == 1){v = '1';}
	if(n == 2){v = '2';}
	if(n == 3){v = '3';}
	if(n == 4){v = '4';}
	if(n == 5){v = '5';}
	if(n == 6){v = '6';}
	if(n == 7){v = '7';}
	if(n == 8){v = '8';}
	if(n == 9){v = '9';}
	return v;
}


void find_int(char* v, int k){
	int n = 0;
	int tam =0 ;
	int h = k;
	printf("%d\n", h);
	while(h>0){
		tam++;
		h = (int)(h/10);
	}
	h = k;
	while(h>0){
		n = h%10;
		h = (int)(h/10);
		printf("%d\n", h);
		v[tam] = findChar(n);
		tam--;
	}

}

int main(int argc, char const *argv[])
{
	printf("hello world\n");
	int n=0;

	//conta(v,0);
	printf("Digite um numero pra sair\n");
	scanf("%d",&n);
	//char vet[30];
	//find_int(vet,n);
	printf("hello world - %d\n",n);
	return 0;
}