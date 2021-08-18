#include <stdio.h>
int main()
{
int i,j;
int x1=-1,x2=-1;
int y1=101,y2=101;
int Ax,Ay;
do{
	printf("Coordinate x1\n");
	scanf("%d",&x1);
}while(x1<0);
do{
	printf("Coordinate y1\n");
	scanf("%d",&y1);
}while(y1>100);
do{
	printf("Coordinate x2\n");
	scanf("%d",&x2);
}while(x2<0);
do{
	printf("Coordinate y2\n");
	scanf("%d",&y2);
}while(y2>100);
printf("\n");
if(x1>x2)
	Ax=-1;
else if(x1<x2)
	Ax=1;
else
	Ax=0;
if(y1>y2)
	Ay=-1;
else if(y1<y2)
	Ay=1;
else
	Ay=0;
for(i=0;i<100;i++){
	for(j=0;j<101;j++){
		if(i==x1 && j==y1){
			printf(" ");
			x1=x1+Ax;
			y1=y1+Ay;}
		else
			printf("*");
	}
	printf("\n");
}
return 0;
}
