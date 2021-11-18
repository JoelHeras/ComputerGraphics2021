#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

int main(){
    const  char* filename="model.obj";
    FILE *object;
    FILE *image;
    char type;
    int i,j;
    int r,t;
    int vertices=0,faces=0;
    float x,y,z;
    float disX,disY,disZ;
    float x1,x2,y1,y2;
    int objFrame;
    float centerX,centerY,centerZ;
    int imageFrame=100;
    float frameCenter;
    float datPoint[4][3];
    unsigned int p1,p2,p3;    
    int **Triangle=NULL;
    float **Point3d=NULL;
    int **frame=NULL;
    ///Reading data in the file 
    object=fopen(filename,"r");
    if(!object){
        printf("\tThe archive is empty or it doesn't exist\n");
        exit (0);
    }
    while(type!=EOF){
        type=fgetc(object);
        if(type=='v'){
            vertices++;
            fscanf(object,"%f",&x);
            fscanf(object,"%f",&y);
            fscanf(object,"%f",&z);
            //printf("Vertice: %c %f %f %f \n",type,x,y,z);
        }
        else if(type=='f'){
            faces++;
            fscanf(object,"%d",&p1);
            fscanf(object,"%d",&p2);
            fscanf(object,"%d",&p3);
        }
    }    
    printf("Vertices in the object %d\n",vertices);
    printf("Faces in the object: %d\n",faces);
    fclose(object);
    object=NULL;
    //Create matrix for data
    Point3d=(float**)malloc(vertices*sizeof(float*));//Create rows for matrix for vertices
    for(i=0;i<vertices;i++)
        Point3d[i]=(float*)malloc(3*sizeof(float));//Create rows for matrix for faces
    Triangle=(int**)malloc(faces*sizeof(int*));
    for(i=0;i<faces;i++)
        Triangle[i]=(int*)malloc(3*sizeof(int));
    /*End programm if isn't there any point*/
    /*if (vertices==0 || faces==0)
        exit (0);*/        
    i=0;
    j=0;
    type='c';
    object=fopen(filename,"r");        
    while(type!=EOF){
        type=fgetc(object);
        if(type=='v'){                        
            fscanf(object,"%f",&x);
            fscanf(object,"%f",&y);
            fscanf(object,"%f",&z);
            Point3d[i][0]=x;
            Point3d[i][1]=y;
            Point3d[i][2]=z;
            i++;
        }
        else if(type=='f'){            
            fscanf(object,"%d",&p1);
            fscanf(object,"%d",&p2);
            fscanf(object,"%d",&p3);
            Triangle[j][0]=p1;
            Triangle[j][1]=p2;
            Triangle[j][2]=p3;
            j++;
        }
    }
    printf("\tPoints saved: %d\n",i);
    printf("\tTriangles saved: %d\n",j);    
    fclose(object);

    
    /*printf("Vertices\n");
    for(i=0;i<vertices;i++)
        printf("v %f %f %f\n",Point3d[i][0],Point3d[i][1],Point3d[i][2]);
    */
    /*Next step resazing*/
    //We asume the first point has the highest and the lowest points
    //first row of the matrix is for Maximun values
    datPoint[0][0]=Point3d[0][0];
    datPoint[0][1]=Point3d[0][1];
    datPoint[0][2]=Point3d[0][2];
    //Second row of the matrix is for Minimum values
    datPoint[1][0]=Point3d[0][0];
    datPoint[1][1]=Point3d[0][1];
    datPoint[1][2]=Point3d[0][2];
    //Proving if the assume is correct
    for(i=1;i<vertices;i++){
        if(Point3d[i][0]>datPoint[0][0])
            datPoint[0][0]=Point3d[i][0];
        if(Point3d[i][1]>datPoint[0][1])    
            datPoint[0][1]=Point3d[i][1];
        if(Point3d[i][2]>datPoint[0][2])    
            datPoint[0][2]=Point3d[i][2];

        if(Point3d[i][0]<datPoint[1][0])
            datPoint[1][0]=Point3d[i][0];
        if(Point3d[i][1]<datPoint[1][1])    
            datPoint[1][1]=Point3d[i][1];
        if(Point3d[i][2]<datPoint[1][2])    
            datPoint[1][2]=Point3d[i][2];    
    }
    printf("Max points x:%f y:%f z:%f\n",datPoint[0][0],datPoint[0][1],datPoint[0][2]);
    printf("Min points x:%f y:%f z:%f\n",datPoint[1][0],datPoint[1][1],datPoint[1][2]);
    //Calculate the bounds of the object    
    //Calculate the distance between Maxs and Mins                
    //The tird row is used for the distance between max and mins, respectibly    
    for(i=0;i<3;i++){
        /*For max[i]=|min[i]|*/
        //if(Point3d[0][i]==Point3d[1])
         /*For    max[i] && min[i] >0 */
        if(datPoint[0][i]>0 && datPoint[1][i]>0)
            datPoint[2][i]=datPoint[0][i]-datPoint[1][i];            
        /*For max[i]>0 && min[i]<0*/    
        else if(datPoint[0][i]>0 && datPoint[1][i]<0)
            datPoint[2][i]=datPoint[0][i]+(datPoint[1][i]*-1);
        /*For max[i] && min[i] <0*/
        else if(datPoint[0][i]<0 && datPoint[1][i]<0)
            datPoint[2][i]=(datPoint[1][i]-datPoint[0][i])*-1;        
        /*For max[i]=0*/
        else if(datPoint[0][i]=0){
            if(Point3d[1][i]<0)
                datPoint[2][i]=Point3d[1][i]*-1;
            else
                datPoint[2][i]=Point3d[1][i];
        }
        /*For min[i]=0*/
        else if(datPoint[1][i]=0){
            if(Point3d[0][i]<0)
                datPoint[2][i]=Point3d[0][i]*-1;
            else
                datPoint[2][i]=Point3d[0][i];
        }
        /*For min[i] && max[i] =0*/
        else
            datPoint[2][i]=0;
    }
    printf("Distance between Maxs and Mins  x:%f y:%f z:%f\n",datPoint[2][0],datPoint[2][1],datPoint[2][2]);
    //Calculate the frame for the image
    ///We assume the distance between the min an max X is the greatest
    objFrame=(int)(datPoint[2][0]+0.5);
    //Check if the distance between min and max Y is the greatest
    if(objFrame<datPoint[2][1])
        objFrame=(int)(datPoint[2][1]+0.5);                
    printf("Object frame: %d\n",objFrame);
    
    printf("Image frame: %d\n",imageFrame);
    //Then we move the points from center 0,0 to center objframe/2,objframe/2   
    frameCenter=imageFrame/2;
    //Check the way to move the vertices
    /*X*/
    if(datPoint[0][0]>0 && datPoint[1][0]<0)//Max X>0 && Min X <0
        centerX=-1*(datPoint[1][0]+(datPoint[2][0]/2));///Distance between max/min X + Min()
    else if(datPoint[0][0]>0 && datPoint[1][0]>=0)//Max X>0 && Min X>=0
        centerX=-1*(datPoint[1][0]+(datPoint[2][0]/2));
    else if(datPoint[0][0]<=0 && datPoint[1][0]<0)//Max X<=0 && Min X<0
        centerX=-1*(datPoint[0][0]-(datPoint[2][0]/2));
    /*Y*/
    if(datPoint[0][1]>0 && datPoint[1][1]<0)//Max Y>0 && Min Y <0
        centerY=-1*(datPoint[1][1]+(datPoint[2][1]/2));
    else if(datPoint[0][1]>0 && datPoint[1][1]>=0)//Max Y>0 && Min Y>=0
        centerY=-1*(datPoint[1][1]+(datPoint[2][1]/2));
    else if(datPoint[0][1]<=0 && datPoint[1][1]<0)//Max Y<=0 && Min Y<0
        centerY=-1*(datPoint[0][1]-(datPoint[2][1]/2));
    /*Z*/
    if(datPoint[0][2]>0 && datPoint[1][2]<0)//Max Y>0 && Min Y <0
        centerZ=-1*(datPoint[1][2]+(datPoint[2][2]/2));
    else if(datPoint[0][2]>0 && datPoint[1][2]>=0)//Max Y>0 && Min Y>=0
        centerZ=-1*(datPoint[1][2]+(datPoint[2][2]/2));
    else if(datPoint[0][2]<=0 && datPoint[1][2]<0)//Max Y<=0 && Min Y<0
        centerZ=-1*(datPoint[0][2]-(datPoint[2][2]/2));
    printf("Move the vertices \n");    
    for( i = 0; i < vertices; i++){
        Point3d[i][0]=centerX+Point3d[i][0];
        Point3d[i][1]=centerY+Point3d[i][1];
        Point3d[i][2]=centerZ+Point3d[i][2];
    }        
    /***********************************************************/
    datPoint[0][0]=Point3d[0][0];
    datPoint[0][1]=Point3d[0][1];
    datPoint[0][2]=Point3d[0][2];
    //Second row of the matrix is for Minimum values
    datPoint[1][0]=Point3d[0][0];
    datPoint[1][1]=Point3d[0][1];
    datPoint[1][2]=Point3d[0][2];
    //Proving if the assume is correct
    for(i=1;i<vertices;i++){
        if(Point3d[i][0]>datPoint[0][0])
            datPoint[0][0]=Point3d[i][0];
        if(Point3d[i][1]>datPoint[0][1])    
            datPoint[0][1]=Point3d[i][1];
        if(Point3d[i][2]>datPoint[0][2])    
            datPoint[0][2]=Point3d[i][2];

        if(Point3d[i][0]<datPoint[1][0])
            datPoint[1][0]=Point3d[i][0];
        if(Point3d[i][1]<datPoint[1][1])    
            datPoint[1][1]=Point3d[i][1];
        if(Point3d[i][2]<datPoint[1][2])    
            datPoint[1][2]=Point3d[i][2];    
    }
    printf("Moved Max points x:%f y:%f z:%f\n",datPoint[0][0],datPoint[0][1],datPoint[0][2]);
    printf("MOved Min points x:%f y:%f z:%f\n",datPoint[1][0],datPoint[1][1],datPoint[1][2]);
    /*************************************************************/

    printf("Resize the object \n");    
    //Resize the object
    for (i=0;i<vertices;i++)
        for(j=0;j<3;j++)
        Point3d[i][j]=(Point3d[i][j]*(imageFrame*0.98))/objFrame;
    /*for( i = 0; i < vertices; i++)
        for (j=0;j<2;j++)
            Point3d[i][j]=frameCenter+(Point3d[i][j]);    
    for(i=0;i<vertices;i++)
        printf("v %f %f %f\n",Point3d[i][0],Point3d[i][1],Point3d[i][2]);*/
       
    imageFrame++;        
    //create the information matrix to make the frame
    frame=(int**)malloc(imageFrame*sizeof(int*));//Create rows for matrix for frame
    for(i=0;i<imageFrame;i++)
        frame[i]=(int*)malloc(imageFrame*sizeof(int));          
    //fill the information matrix with blank data
    for(i=0;i<imageFrame;i++)        
        for(j=0;j<imageFrame;j++)
            frame[i][j]=0;


    /***Comprobar los puntos movidos***/
    datPoint[0][0]=Point3d[0][0];
    datPoint[0][1]=Point3d[0][1];
    datPoint[0][2]=Point3d[0][2];
    //Second row of the matrix is for Minimum values
    datPoint[1][0]=Point3d[0][0];
    datPoint[1][1]=Point3d[0][1];
    datPoint[1][2]=Point3d[0][2];
    //Proving if the assume is correct
    for(i=1;i<vertices;i++){
        if(Point3d[i][0]>datPoint[0][0])
            datPoint[0][0]=Point3d[i][0];
        if(Point3d[i][1]>datPoint[0][1])    
            datPoint[0][1]=Point3d[i][1];
        if(Point3d[i][2]>datPoint[0][2])    
            datPoint[0][2]=Point3d[i][2];

        if(Point3d[i][0]<datPoint[1][0])
            datPoint[1][0]=Point3d[i][0];
        if(Point3d[i][1]<datPoint[1][1])    
            datPoint[1][1]=Point3d[i][1];
        if(Point3d[i][2]<datPoint[1][2])    
            datPoint[1][2]=Point3d[i][2];    
    }
    printf("New Max points x:%f y:%f z:%f\n",datPoint[0][0],datPoint[0][1],datPoint[0][2]);
    printf("New Min points x:%f y:%f z:%f\n",datPoint[1][0],datPoint[1][1],datPoint[1][2]);
    /******/
    //Place the points in the frame
    printf("Filling the points\n");
    for(i=0;i<vertices;i++){
        r=frameCenter+((int)Point3d[i][1]*-1);
        t=frameCenter+((int)Point3d[i][0]*-1);
       // printf("%d %d ",r,t);
        frame[r][t]=1;
    }
    /*printf("Data in the frame\n");
    for(i=0;i<objFrame;i++){
        for(j=0;j<objFrame;j++)
            printf("%d ",frame[i][j]);
            printf("\n");
    }*/
    //Connect the face points
    /*for (i = 0; i < faces; i++){       //Point3d [#][eje] [1][0]->eje x punto 1
        if(Point3d[Triangle[i][0]][0]>Point3d[Triangle[i][1]][0])//X1 X2
            x1=Point3d
    }
    */

    //Create the image
    image=fopen("render.ppm","w");
    fprintf(image,"P3\n%d %d\n255\n",imageFrame,imageFrame);
    printf("Image header created\n");
    for ( i = 0; i < imageFrame; i++){
        for (j = 0; j <imageFrame; j++){
            if(frame[i][j]==1)
                fprintf(image,"255 255 0 ");
            else
                fprintf(image,"0 0 255 ");
        }
        fprintf(image,"\n");
    }
    fclose(image);        
    free(Point3d);
    free(Triangle);   
    free(frame) ;
    exit (0);
}
