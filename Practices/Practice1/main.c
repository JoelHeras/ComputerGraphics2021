#include <stdio.h>
int main()
{
	int i,j;
	int x1=-1,x2=-1;
	int x,y;
	int y1=101,y2=101;
	int Dx,Dy;
	int IncrYi,IncrXi;
	int IncrYr,IncrXr;
	char image[50][100];
	for(i=0;i<50;i++){
		for(j=0;j<100;j++)
			image[i][j]='*';
	}
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
	if(x1>x2){
		i=x2;
		x2=x1;
		x1=i;
	}
	if(y1>y2){
		i=y2;
		y2=y1;
		y1=i;
	}
	Dx=x2-x1;
	Dy=y2-y1;
	//Vertical increments
	if(Dy>=0)
		IncrYi=1;
	else {
		Dy=Dy*-1;
		IncrYi=-1;
	}
	if(Dx>=0)
		IncrXi=1;
	else{
		Dx=Dx*-1;
		IncrXi=-1;
	}
	//Horizontal increments
	if(Dx>=Dy){
		IncrYr=0;
		IncrXr=IncrXi;
	}
	else{
		IncrXr=0;
		IncrYr=IncrYi;
		i=Dx;
		Dx=Dy;
		Dy=i;
	}
	//Initialize values
	x=x1;
	y=y1;
	int avR=(2*Dy);
	int av=(avR-Dx);
	int avI=(av-Dx);

	//Draw line
	do{
		//Draw Pixel
		image[x][y]='_';
		printf("%d ",av);
		if(av>=0){
			x=x+IncrXi;
			y=x+IncrYi;
			av=av+avI;
		}
		else{
			x=x+IncrXr;
			y=y+IncrYr;
			av=av+avR;
		}
	}while(x<x2);
	printf("\n");
	for(i=0;i<50;i++){
		for(j=0;j<100;j++)
			printf("%c",image[i][j]);
		printf("\n");
	}

return 0;
}
