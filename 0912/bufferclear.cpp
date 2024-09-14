#include <stdio.h>

int main(){
	char string[20];
	char c;
	
	scanf("%s", string);
	//getchar(); // scanf를 곧바로 이어 받으면 위의 %s에 입력한 문자가 그대로 남아서 알아서 %c에 들어가버린다. 
	scanf(" %c", &c); // %c 앞에 공백을 한칸 넣어주면 문제가 잡힌다. 
	
	printf("%s\n", string);
	printf("!!%c!!\n", c);
	
	return 0;
}
