/*----------------------------------
FILE:	imread6.C 
AUTHOR:	ABHISHEK GUPTA
DATE:	09/27/2007
DESC:	Video Surveillance
-----------------------------------*/

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <math.h>
#include <curses.h>
#include "Background.h"

#include <sys/time.h>
#define ROWS 240
#define COLS 320
#define MAX_OBJECTS 320
/*-------PROTOTYPES-------*/
long getImageInfo(FILE*, long, int);
void copyImageInfo(FILE* inputFile, FILE* outputFile);
void createColorTable(FILE* inputFile, FILE* outputFile);






/*------STRUCTURES--------*/
typedef struct {int rows; int cols;int** data;} sImage;
typedef struct { int x,y;} point; 
typedef struct  { point min,max; int label;} Object; 
//struct blobs

void bmp2gray(char img_name[13],sImage img);
void imsubstract(sImage img1,sImage img2,sImage img);

void writegraybmp(sImage img,char []);
void backup_writegraybmp(sImage img,char  str[14]);
int blobidentify(sImage,Object *);
void label(sImage,int c,int r,int currentlabel,int *,int *,int *,int *);
int match(Object * objG, Object *objC, int ng, int nc);           
float overlap(Object rect1,Object rect2);

typedef struct {int roffset; int coffset;} se;

void imerode(sImage img, se [],int n);
void imdilate(sImage img, se [],int n);
void imreconstruct(sImage img,sImage mask, se[],int n);
int imcmp(sImage img1,sImage img2);
void  imand(sImage img,sImage mask);

char find_threshold(sImage img);

int main(int argc, char* argv[])
{
    sImage img,blob_image,back_img,next_frame;
    int *newobjlabel =malloc(50*sizeof(int));
  
    struct timeval tval1,tval2,tval3,tval4;struct timezone tz;
    gettimeofday(&tval1,&tz);

    blob_image.data=malloc(ROWS * sizeof(int *));
    int row,col;
    for(row=0;row<ROWS;row++)
          blob_image.data[row]=(int *)(malloc(COLS * sizeof(int)));
            
    img.data=malloc(ROWS * sizeof(int *));
    for(row=0;row<ROWS;row++)
          img.data[row]=(int *)(malloc(COLS * sizeof(int)));
   
     int newobj;
    char str[14];
    Object * prev_obj,*cur_obj;
    int n1=0,n2=0; 
    prev_obj=(Object *)(malloc(200*sizeof(Object)));//do ir dynamically 
    cur_obj=(Object *)(malloc(200*sizeof(Object)));//do ir dynamically 
    int image_num=100;
  //  back_img=bmp2gray("img_00000.bmp");
    //next_frame=bmp2gray("img_00002.bmp");
 //   back_img=bmp2gray("img_00002.bmp");
    int img_num;int i=0;
    char img_name[13];//printf("here");
    img_num=100+i;
  
    se str_el[5]={{-1,0},{0,-1},{0,0},{1,0},{0,1}};
    se str_el2[4]={{-1,-1},{-1,0},{0,0},{0,-1}};
    se str_el3[6]={{-1,-1},{-1,0},{0,0},{0,-1},{-1,1},{0,1},};
    se str_el4[6]={{-1,-1},{-1,0},{0,0},{0,-1},{1,0},{1,-1}};
        
     se str_rc[9]={{-1,-1},{-1,0},{0,0},{0,-1},{1,0},{1,-1},{1,1},{0,1},{-1,1}};
   	Object *globalObjStorage = (Object *)malloc( MAX_OBJECTS*sizeof(Object) );
	int cur=0, max=MAX_OBJECTS;
	int matchResult;
  /////////////////////////////////////////bcg
    
    //clock_t start1=clock();
     sImage Mean,Var;
     Mean.data=malloc((ROWS) * sizeof(int *)); 
     for(row=0;row<ROWS;row++)
          Mean.data[row]=(int *)(malloc((COLS) * sizeof(int)));
    // sImage marker; 

     Var.data=malloc((ROWS) * sizeof(int *)); 
     for(row=0;row<ROWS;row++)
           Var.data[row]=(int *)(malloc((COLS) * sizeof(int))); 
     unsigned  int M[ROWS][COLS];unsigned int V[ROWS][COLS];    
 
    // sprintf(str,"men_00%d.bmp",500);          
    // writegraybmp(Mean,str);    
   //  sprintf(str,"Var_00%d.bmp",500); 
   //  writegraybmp(Var,str); ;      
     Mean.rows=Var.rows=ROWS;
     Mean.cols=Var.cols=COLS;
    // clock_t start2=clock();
     sImage mark;
     mark.data=malloc((ROWS) * sizeof(int *)); 
     for(row=0;row<ROWS;row++)
          mark.data[row]=(int *)(malloc((COLS) * sizeof(int))); 
          sImage marker;
     marker.data=malloc((ROWS) * sizeof(int *)); 
     for(row=0;row<ROWS;row++)
          marker.data[row]=(int *)(malloc((COLS) * sizeof(int))); 
       
    img.rows=mark.rows=ROWS;img.cols=mark.cols=COLS;
    double time_r=0,time_w=0; 
    for(image_num=100;image_num<134;image_num+=2)  
      {  
         // printf("yes \n");
      if((image_num-100)%16==0)
    {
if(image_num==100)  find_bkg( M,V,image_num); 
else
     find_bkg( M,V,image_num-16); 
     //  printf("here \n");
     // getch();
      for(row=0;row<ROWS;row++)
       for(col=0;col<COLS;col++)
       {Mean.data[row][col]=M[row][col];Var.data[row][col]=V[row][col];
    
       }
        }
          sprintf(str,"img_00%d.bmp",image_num);
         // free((void*)(next_frame.data));
          gettimeofday(&tval3,&tz);
          bmp2gray(str,img);
          gettimeofday(&tval4,&tz);
          time_r+=((tval4.tv_usec-tval3.tv_usec)*1.0/1000.0);
       //  time/=1000;
          time_r+=((tval4.tv_sec-tval3.tv_sec)*1000);
          imsubstract(Mean,img,img);
          //imcpy(img,mark);
        // sprintf(str,"Mea_00%d.bmp",image_num); 
         //writegraybmp(Mean,str);   
       //   printf("here %d \t",img.data[61][87]);
         int ii,jj;
         float val;
         for(ii=0;ii<ROWS;ii++)
            { // printf("%d y %d no %d \t",Mean.data[ii][jj],img.data[ii][jj],Var.data[ii][jj] ); 
             for(jj=0;jj<COLS;jj++)
                {
                 mark.data[ii][jj]=img.data[ii][jj]; 
             // printf("here %d,%d \t",img.data[ii][jj],Var.data[ii][jj]);
                    val=img.data[ii][jj]*img.data[ii][jj];
                //  val/=Var.data[ii][jj];     
                   if(val>=4*Var.data[ii][jj]) 
                       {//printf("yes %d,%d \n",Var.data[ii][jj],img.data[ii][jj]); 
                        img.data[ii][jj]=255;}
                    else    
                         img.data[ii][jj]=0; 
                    // 
                }
                }
            //    sprintf(str,"bin_00%d.bmp",image_num); 
              //  writegraybmp(img,str); 
               for(row=0;row<ROWS;row++)
                   for(col=0;col<COLS;col++)  
                      marker.data[row][col]=img.data[row][col];
                      
    
        imerode(img,&(str_el4[0]),6); // sprintf(str,"bn1_00%d.bmp",image_num); 
               // writegraybmp(img,str); 
        imreconstruct(img,marker,&(str_rc[0]),9); 
        imdilate(img,&(str_el[0]),5);
        imerode(img,&(str_el[0]),5);
      //  sprintf(str,"bn1_00%d.bmp",image_num); 
       // writegraybmp(img,str);    
        n2=blobidentify(img,cur_obj); 
      //  printf("val= %d \n",n2);   
        int k;    int rr,cc;
        for( k=0;k<n2;k++)
         {
                 for( rr=cur_obj[k].min.x;rr<=cur_obj[k].max.x;rr++)
                           mark.data[rr][cur_obj[k].min.y]= mark.data[rr][cur_obj[k].max.y]=255;                
                 for( cc=cur_obj[k].min.y;cc<=cur_obj[k].max.y;cc++)
                           mark.data[cur_obj[k].min.x][cc]= mark.data[cur_obj[k].max.x][cc]=255;
                         
         } 
   
   
    sprintf(str,"blb_00%d.bmp",image_num); 
          gettimeofday(&tval3,&tz);
 //printf("here \n");
         writegraybmp(mark,str);  
// printf("here \n");  
          gettimeofday(&tval4,&tz);
          time_w+=((tval4.tv_usec-tval3.tv_usec)/1000.0);
       //  time/=1000;
          time_w+=((tval4.tv_sec-tval3.tv_sec)*1000);
        //  printf("here1");getch();     
  // writegraybmp(Mean,str);   printf("here2");getch();        
         
        matchResult=match(prev_obj,cur_obj,n1,n2);
               
           //Global update
         
   	//int k;
			//printf("PPU says %d blobs in SPU %d 's image\n",num_of_objects,i);
    if(matchResult==1)
			{
            for(k=0;k<n1;k++)
            {
                    

                    if( prev_obj[k].min.x==250) continue;
                    if( cur==max)   ////// SCOPE FOR IMPROVEMENT
			        {
				     globalObjStorage =(Object *)realloc((void *)globalObjStorage, (max+MAX_OBJECTS)*sizeof(Object) );
                     max += MAX_OBJECTS;
                    }
                    globalObjStorage[cur++] =prev_obj[k];                   
            }
            }
              
          n1=n2;
          for(k=0;k<n1;k++)
           prev_obj[k]=cur_obj[k];
               
               
            
    
    
               
           
            
         }
         
         //last one
     int k;
        for(k=0;k<n1;k++)
            {
                    

                    if( prev_obj[k].min.x==250) continue;
                    if( cur==max)   ////// SCOPE FOR IMPROVEMENT
			        {
				     globalObjStorage =(Object *)realloc((void *)globalObjStorage, (max+MAX_OBJECTS)*sizeof(Object) );
                     max += MAX_OBJECTS;
                    }
                    globalObjStorage[cur++] =prev_obj[k];                   
            }  
               
      
        // sprintf(str,"rsb_00%d.bmp",image_num);          
        // writegraybmp(img,str);
       
       
       
      
       
     //    printf("%s processed \n",str);
        
   // printf("done  ");
    
  //  clock_t stop=clock();
   /*  double time1=start2-start1;
    time1/=CLOCKS_PER_SEC;
    double time2=stop-start2;
    time2/=CLOCKS_PER_SEC;

   */ 
    gettimeofday(&tval2,&tz);  
    double time=(tval2.tv_usec-tval1.tv_usec);
    time/=1000;
    time+=(tval2.tv_sec-tval1.tv_sec)*1000;
       
       //time/=CLOCKS_PER_SEC;
    printf("Total Time by gettimeofday= %f \n Total Read time in main %f \n Total time for write in main %f \n",time,time_r,time_w);
  //  printf("bkg Time=%f frame time=%f",time1,time2);
  //  getch();
   
    return 0;
    
}   

char find_threshold(sImage img)
{
     int oldT=255,presentT=125;
     double temp;
     int i,j;
     long val1=0,val2=0,cnt1=0,cnt2=0;
     while(presentT-oldT >1 || oldT-presentT >1)
     {
      val1=0;val2=0;cnt1=0;cnt2=0;
       for(i=0;i<ROWS;i++)
              for(j=0;j<COLS;j++)
                  if(img.data[i][j]<presentT)
                     { val1+=img.data[i][j];cnt1++;}
                      else 
                      {val2+=img.data[i][j];cnt2++;}
     
       oldT=presentT;
       temp=val1;temp/=cnt1;
       temp+=(double)(val2)/cnt2;
       temp/=2;
       presentT=(int)(temp);
        
      // printf("val= %d \n" ,presentT);
       }
       
       return presentT; 
   }
        
                 
                      
                                   
                     
     

              

int blobidentify(sImage img3,Object * obj)
    {   //Incorporate Noise Removal
        sImage img4;
        //write("done inside",stdout);
        //source of problem
         //obj=(Object *)(malloc(200*sizeof(Object)));//do ir dynamically 
         img4.data=malloc((ROWS+2) * sizeof(int *));
         int row,i,j;
         int xmin=340,xmax=-1,ymin=340,ymax=-1;
         point mn,mx;
         for(row=0;row<=ROWS+1;row++)
          img4.data[row]=(int *)(malloc((COLS+2) * sizeof(int))); 
         for(i=0;i<=ROWS+1;i++) img4.data[i][0]=img4.data[i][COLS+1]=0; 
         for(j=0;j<=COLS+1;j++) img4.data[0][j]=img4.data[ROWS+1][j]=0; 
         char T=find_threshold(img3);
         //convert to binary array
          for(i=1;i<=ROWS;i++)
              for(j=1;j<=COLS;j++)
                {
                 
                           img4.data[i][j]=img3.data[i-1][j-1]/255;                   
                }   
           //labeling
          int currentlabel=2; 
          for(i=1;i<=ROWS;i++)
              for(j=1;j<=COLS;j++)
              {
               if(img4.data[i][j]==1)
                  {
                 // printf("\n%d %d %d \t",i,j,img4.data[i][j]);
                   xmin=i;xmax=i;ymin=j;ymax=j;
                  
                  label(img4,j,i,currentlabel,&xmin,&ymin,&xmax,&ymax);
                  //printf("no%d  %d %d %d %d  \t",currentlabel,xmin,ymin,xmax,ymax); 
                   //single point noise
              if(xmax-xmin<=2 && ymax-ymin<=2)
                 {
                                 int k,l;
                                 for(k=xmin;k<xmax;k++)
                                 for(l=ymin;l<ymax;l++)
                                 img4.data[k][l]=0;
                                 // printf(" \n yes %d  %d %d %d %d  \t",currentlabel,xmin,ymin,xmax,ymax); 
                 }  
                 //  if(xmin==i && xmax==i && ymin==j && ymax==j)
                   //img4.data[i][j]=0; 
                
             else  
                   {   
                      mn.x=xmin-1;mn.y=ymin-1;
                      mx.x=xmax-1;mx.y=ymax-1;
                      obj[currentlabel-2].min=mn;
                      obj[currentlabel-2].max=mx;
                      //obj[currentlabel-2].
                      currentlabel++;
                    // printf(" \n yes %d  %d %d %d %d  \t",currentlabel,xmin,ymin,xmax,ymax);   
                    }
               }    
                                
               }
    
      // {  printf("( %d ,%d ),( %d ,%d) \n",obj[s].min.x,obj[s].min.y,obj[s].max.x,obj[s].max.y);}
 for(row=0;row<=ROWS+1;row++)
   free(img4.data[row]);
   free( img4.data);
         return currentlabel-2;         
          //deallocate img4         
      }         
                             
void label(sImage img4,int j,int i,int currentlabel,int *xmin,int *ymin,int *xmax,int *ymax)
     {         
              
              img4.data[i][j]=currentlabel; 
               if(i<(*xmin)) (*xmin)=i;
                else if(i>(*xmax)) (*xmax)=i; 
                if(j<(*ymin)) (*ymin)=j;
                else if(j>(*ymax)) (*ymax)=j; 
                int nbr,r,c;
                 for(nbr=0;nbr<=7;nbr++)
                     { 
                        
                       switch(nbr)
                          {case 0:c=j+1;r=i;break; 
                          case 1: c=j;r=i-1;break;
                          case 2:c=j-1;r=i;break;
                          case 3:c=j;r=i+1;break; 
                       
                          case 4:c=j+1;r=i-1;break; 
                          case 5:c=j-1;r=i-1;break;
                          case 6:c=j-1;r=i+1;break; 
                          case 7:c=j+1;r=i+1;
                                 
                          }  
                        if(img4.data[r][c]==1)                         
                             {  // printf("hmmm( %d ,%d ),( %d ,%d) \n",*xmin,*ymin,*xmax,*ymax);
                                 label(img4,c,r,currentlabel,xmin,ymin,xmax,ymax);}
                   }      
                      
    }                                 
        
void imsubstract(sImage img1,sImage img2,sImage img3)
    {
        // sImage img3;
         img3.cols =COLS;
         img3.rows =ROWS;
    
         int i,j;   
          for(i=0;i<img1.rows;i++){//printf("Next");
           for(j=0;j<img2.cols;j++)
                 {
                     img3.data[i][j]= img1.data[i][j]- img2.data[i][j];
                     if( img3.data[i][j]<0) (img3.data[i][j])*=-1;
                    // if( img3.data[i][j]<20)   img3.data[i][j]=0;
                    // printf("%d  %d %d \t",img1.data[i][j],img2.data[i][j],img3.data[i][j]);
                 } }
            // printf("here  %d %d %d \t",img1.data[61][87],img2.data[61][87],img3.data[61][87]);        
       // return img3;
    }      

void bmp2gray(char img_name[13],sImage newImage)
{
 static double time_rr=0,time_rrr=0;
  struct timeval tval1,tval2,tval3,tval4;struct timezone tz;
  gettimeofday(&tval3,&tz);
        
            
 FILE			*bmpInput; //*rasterOutput, *bmpOutput, *grayBmpInput;
  int r,c;
//  unsigned char			redValue, greenValue, blueValue, grayValue;
  unsigned char		pChar[ROWS*COLS*3];
  /*sImage newImage;
  newImage.cols=COLS;
  newImage.rows=ROWS;
  newImage.data=malloc(newImage.rows * sizeof(int *));
  int row;
  for(row=0;row<newImage.rows;row++)
            
                newImage.data[row]=(int *)(malloc(newImage.cols * sizeof(int)));
   */             

  /*----DECLARE INPUT FILES----*/
  bmpInput = fopen(img_name, "rb");
  fseek(bmpInput, 0L, SEEK_END); 

  
  // printf("Reading file\n");

  fseek(bmpInput, 54, SEEK_SET);
      //uint32_t finish=spu_read_decrementer();
  fread(pChar, ROWS*COLS*3*sizeof(unsigned char), 1, bmpInput);
  
  gettimeofday(&tval4,&tz);
          time_rr+=((tval4.tv_usec-tval3.tv_usec)*1.0/1000.0);
       //  time/=1000;
          time_rr+=((tval4.tv_sec-tval3.tv_sec)*1000); 
  printf("Inside read %f \n",time_rr);

  int i=0;
  for(r=0; r<=ROWS-1; r++)
  {
    for(c=0; c<=COLS-1; c++)
    {
          
      newImage.data[r][c]= (unsigned char)(0.299*pChar[i+2] + 0.587*pChar[i+1] + 0.114*pChar[i]);
      i=i+3;
    
                     
    
    }
  }
gettimeofday(&tval4,&tz);
          time_rrr+=((tval4.tv_usec-tval3.tv_usec)*1.0/1000.0);
       //  time/=1000;
          time_rrr+=((tval4.tv_sec-tval3.tv_sec)*1000); 
  printf("Inside  %f \n",time_rrr);
  fclose(bmpInput);
 //return newImage;
}
sImage Bmp2gray(char img_name[13])
{
  FILE			*bmpInput, *rasterOutput, *bmpOutput, *grayBmpInput;
  sImage		originalImage;
  sImage		newImage;
  unsigned char		someChar;
  unsigned char		*pChar;
  unsigned long		someLong;
  unsigned long		*pLong;
  long			fileSize;
  int			vectorSize, nColors;
  int			r, c, i;
  int			redValue, greenValue, blueValue, grayValue;
  
  

  /*--------INITIALIZE POINTER----------*/
  someChar = '0';
  pChar = &someChar;

  someLong = '0';
  pLong = &someLong;



  /*----DECLARE INPUT FILES----*/
  bmpInput = fopen(img_name, "rb");
  fseek(bmpInput, 0L, SEEK_END);
  grayBmpInput = fopen("p1.bmp", "rb");
  fseek(grayBmpInput, 0L, SEEK_END);
  

  /*----DECLARE OUTPUT FILES----*/
  rasterOutput = fopen("data24.txt", "w");
  bmpOutput = fopen("gray.bmp", "wb");

  /*-----GET INPUT BMP DATA-----*/
  newImage.cols = originalImage.cols = (int)getImageInfo(bmpInput, 18, 4);
  newImage.rows = originalImage.rows = (int)getImageInfo(bmpInput, 22, 4);
  
  // can use char here
  newImage.data=malloc(newImage.rows * sizeof(int *));
  int row;
  for(row=0;row<newImage.rows;row++)
            
                newImage.data[row]=(int *)(malloc(newImage.cols * sizeof(int)));
                
  
  fileSize = getImageInfo(bmpInput, 2, 4);
  nColors = getImageInfo(bmpInput, 46, 4);


  copyImageInfo(bmpInput, bmpOutput);

  /*----EDIT FILE SIZE TO CONFORM TO 8 BIT BMP---*/
  fseek(bmpOutput, 2, SEEK_SET);
  *pLong = (unsigned long)(54 + 256*4+(originalImage.cols * originalImage.rows));
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);
  //printf("File size: %ld\n", (long)getImageInfo(bmpOutput, 2, 4));

  /*----CHANGE BIT DEPTH FROM 24 TO 8----*/
  fseek(bmpOutput, 28, SEEK_SET);
  *pLong = (unsigned long)(8);
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);
 // printf("Bits/pixel: %d\n", (int)getImageInfo(bmpOutput, 28, 2));

  /*---CHANGE #COLORS FROM 16M TO 256---*/
  fseek(bmpOutput, 46, SEEK_SET);
  *pLong = (unsigned long)(256);
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);
  //printf("No. colors: %d\n", (int)getImageInfo(bmpOutput, 46, 4));
  
   fseek(bmpOutput, 10, SEEK_SET);
  
  fread(pLong, sizeof(unsigned long), 1, bmpOutput);
  //printf("yahoo %ld",(*pLong));
   *pLong = (unsigned long)(1078);
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);


  
  createColorTable(grayBmpInput, bmpOutput);


  fseek(bmpOutput, (54 + 4*256), SEEK_SET);

  /*-----------READ RASTER DATA-----------*/
  for(r=0; r<=originalImage.rows-1; r++)
  {
    for(c=0; c<=originalImage.cols-1; c++)
    {
      /*-----READ FIRST BYTE TO GET BLUE VALUE-----*/
      fread(pChar, sizeof(char), 1, bmpInput);
      blueValue = *pChar;

      /*-----READ NEXT BYTE TO GET GREEN VALUE-----*/
      fread(pChar, sizeof(char), 1, bmpInput);
      greenValue = *pChar;

      /*-----READ NEXT BYTE TO GET RED VALUE-----*/
      fread(pChar, sizeof(char), 1, bmpInput);
      redValue = *pChar;

      /*-----USE FORMULA TO CONVERT RGB VALUE TO GRAYSCALE-----*/
      grayValue = (int)(0.299*redValue + 0.587*greenValue + 0.114*blueValue);
    
    
      newImage.data[r][c]=grayValue;
      /*-----WRITE TO NEW BMP FILE------*/
   
    }
  }

  fclose(bmpInput);
  fclose(rasterOutput);
  fclose(bmpOutput);
  fclose(grayBmpInput);

  return newImage;
}

/*----GET IMAGE INFO SUBPROGRAM------*/

long getImageInfo(FILE* inputFile, long offset, int numberOfChars)
{
  unsigned char		*ptrC;
  long			value=0L;
  int			i;
  unsigned char		dummy;


  dummy = '0';
  ptrC = &dummy;

  fseek(inputFile, offset, SEEK_SET);

  for(i=1; i<=numberOfChars; i++)
  {
    fread(ptrC, sizeof(char), 1, inputFile);
    /* calculate value based on adding bytes */
    value = (long)(value + (*ptrC)*(pow(256, (i-1))));
  }

  return(value);
}

/*--------COPIES HEADER AND INFO HEADER------*/
void copyImageInfo(FILE* inputFile, FILE* outputFile)
{
  unsigned char			*ptrC;
  unsigned char			dummy;
  int				i;
int dummy2;


  dummy = '0';
  ptrC = &dummy;

  fseek(inputFile, 0L, SEEK_SET);
  fseek(outputFile, 0L, SEEK_SET);

  for(i=0; i<=50; i++)
  {
    fread(ptrC, sizeof(char), 1, inputFile);
    fwrite(ptrC, sizeof(char), 1, outputFile);
     dummy2=*ptrC;
    //printf(" yo %d  ",dummy2);
  }

}

/*---------COPIES COLOR TABLE--------*/
void createColorTable(FILE* inputFile, FILE* outputFile)
{
  unsigned char			*ptrC;
  unsigned char			dummy;
  int				i;

  dummy = '0';
  ptrC = &dummy;

  fseek(inputFile, 54L, SEEK_SET);
  fseek(outputFile, 54L, SEEK_SET);

  for(i=1; i<=4*256; i++)
  {
    fread(ptrC, sizeof(char), 1, inputFile);
    fwrite(ptrC, sizeof(char), 1, outputFile);
  }

}



void writegraybmp(sImage img,char  str[14])
{
  
  
  unsigned char			*ptrC,*pChar;
  unsigned char			some;

  pChar = &some;


  FILE			*bmpOutput, *grayBmpInput;

  grayBmpInput = fopen("p1.bmp", "rb");  
  bmpOutput = fopen(str,"wb");
  fseek(grayBmpInput, 0L, SEEK_SET);
  fseek(bmpOutput, 0L, SEEK_SET);
  
 // unsigned char			*ptrC;//,*pChar;
  unsigned char			dummy[256*4+54];
     int r=0,c=0;
 
    ptrC = &dummy; 
 
    fread(ptrC, (256*4+54)*sizeof(char), 1, grayBmpInput);
    fwrite(ptrC, (256*4+54)*sizeof(char), 1, bmpOutput);
   for(r=0; r<=ROWS-1; r++)
  
    for(c=0; c<=COLS-1; c++)
    {
    
    
      *pChar = img.data[r][c];
      fwrite(pChar, sizeof(char), 1, bmpOutput);
    } 
 
  fclose(bmpOutput);
  fclose(grayBmpInput);
}


float overlap(Object rect1,Object rect2)
   {
       int x1=rect1.min.x,y1=rect1.min.y,x2=rect1.max.x,y2=rect1.max.y;
       int x3=rect2.min.x,y3=rect2.min.y,x4=rect2.max.x,y4=rect2.max.y;   
       //char flag[8]={'n','n','n','n','n','n','n','n'};
        // for( s=0;s<(currentlabel-1);s++)
     
        
        
        int area=0;
        float overlap=0;
        int place[8];int count1=0,count2=0;
       
         if(x1>=x3 && x1<=x4)
          { if(y1>=y3 && y1<=y4)
              place[count1++]=0;//flag[0]='y';
           if(y2>=y3 && y2<=y4)
              place[count1++]=1;
            }      
        if(x2>=x3 && x2<=x4)
            { if(y2>=y3 && y2<=y4)
             place[count1++]=2;
            if(y1>=y3 && y1<=y4)
               place[count1++]=3;
           }    
        if(x3>=x1 && x3<=x2)
           {if(y3>=y1 && y3<=y2)
              {place[count1+count2]=4;count2++;}
           if(y4>=y1 && y4<=y2)
               {place[count1+count2]=5;count2++;}
           }    
       if(x4>=x1 && x4<=x2)
            { if(y4>=y1 && y4<=y2)
              {place[count1+count2]=6;count2++;}
             if(y3>=y1 && y3<=y2)
              {place[count1+count2]=7;count2++;}
          }    
        
     
        if(count1==0 && count2==0) return 0;
       // printf("( %d ,%d ),( %d ,%d) \t",x1,y1,x2,y2);
        //printf("( %d ,%d ),( %d ,%d) \n",x3,y3,x4,y4);
           if(count1==4 || count2==4) { return 1.0;}
          
        if(count1==1 && count2==1)
           {  //printf("yes1\n");    
             int a=place[0],b=place[1],xA=0,yA=0;
             switch (a)
                {
                  case 0: xA =x1;yA=y1;break;
                  case 1: xA =x1;yA=y2;break;
                  case 2: xA =x2;yA=y2;break;
                  case 3: xA =x2;yA=y1;break;
               }
            switch (b)
              {
                  case 4: xA -=x3;yA-=y3;break;
                  case 5: xA -=x3;yA-=y4;break;
                  case 6: xA -=x4;yA-=y4;break;
                  case 7: xA -=x4;yA-=y3;break;
               }
             area=xA*yA;
           
         }  
        if(count1==2)
           {  //printf("yes2\n");    
                 int a=place[0],b=place[1];
                 //printf("%d ,%d  \n",a,b);
                 if(a==0 &&  b==1)
                    area=(y2-y1)*(x4-x1);
                 if(a==2 && b==3)
                    area=(y2-y1)*(x2-x3); 
                 if(a==0 && b==3)         
                    area=(x2-x1)*(y1-y4);    
                 if(a==1 && b==2)         
                    area=(x2-x1)*(y2-y3);       
                    
                                    //printf("area= %d \n\n",area);               
           }  
        else if(count2==2)
          {        //printf("yes3 \n");                   
                  int a=place[count1+0],b=place[count1+1];
                  if(a==4 &&  b==5)
                    area=(y4-y3)*(x2-x3);
                  if(a==6 && b==7)
                    area=(y4-y3)*(x4-x1); 
                  if(a==4 && b==7)         
                    area=(x4-x3)*(y3-y2);    
                  if(a==5 && b==6)         
                    area=(x4-x3)*(y4-y1);
                    
                     // printf("area= %d \n\n",area);  
           }   
          if(area<0) area*=(-1); 
          int area1=(x2-x1)*(y2-y1);
          int area2=(x4-x3)*(y4-y3);
          if(area1!=0) overlap=area*1.0/area1;
          float overlap2=0;
          if(area2!=0)
            overlap2=area*1.0/area2;
          if(overlap2>overlap) overlap=overlap2;
          //printf("Overlap= %f \n",overlap);
          return overlap;
      }   
	// function 'match' will modify global_obj. and it'll be sent to be stored on the main memory
	// the entries to be deleted from global_obj will find their 'min' set to (250, 250)
	// 'match' will return 'false' if nothing needs to be sent back to the main memory
int match(Object * objG, Object *objC, int ng, int nc)
{
	int i,j;
	int flag = 0, mark;
	for( i=0; i<ng; i++)
	{
		mark=1;
		for( j=0; j<nc; j++)
		{
			if( (objG[i].max.x) < (objC[j].min.x) )
				break;
			if( overlap(objG[i],objC[j]) > 0.4)
			{
				mark = 0;
				break;
			}
		}
		if( mark==0)
		{
			objG[i].min.x = 250;
			objG[i].min.y = 250;
			//flag = 1;
		}
		 else flag=1;
   }	return flag;
}



void backup_writegraybmp(sImage img,char  str[14])
{
  FILE			*bmpInput, *rasterOutput, *bmpOutput, *grayBmpInput;
  sImage		originalImage;
  sImage		newImage;
  unsigned char		someChar;
  unsigned char		*pChar;
  unsigned long		someLong;
  unsigned long		*pLong;
  long			fileSize;
  int			vectorSize, nColors;
  int			r, c, i;
  int			redValue, greenValue, blueValue, grayValue;
  
  //printf("YYYYYYYYYYY");
  //getch();

  /*--------INITIALIZE POINTER----------*/
  someChar = '0';
  pChar = &someChar;

  someLong = '0';
  pLong = &someLong;

  /*if(argc < 2)
  {
    printf("Usage: %s bmpInput.bmp\n", argv[0]);
    exit(0);
  }*/
  
  //printf("Reading file %s\n","img_00000.bmp");

  /*----DECLARE INPUT FILES----*/
  bmpInput = fopen("img_00002.bmp", "rb");
  fseek(bmpInput, 0L, SEEK_END);
  grayBmpInput = fopen("p1.bmp", "rb");
  fseek(grayBmpInput, 0L, SEEK_END);
  

  /*----DECLARE OUTPUT FILES----*/
  rasterOutput = fopen("data24.txt", "w");
  
  bmpOutput = fopen(str,"wb");

  /*-----GET INPUT BMP DATA-----*/
  /*newImage.cols = originalImage.cols = (int)getImageInfo(bmpInput, 18, 4);
  newImage.rows = originalImage.rows = (int)getImageInfo(bmpInput, 22, 4);
  */
  // can use char here
  fileSize = getImageInfo(bmpInput, 2, 4);
  nColors = getImageInfo(bmpInput, 46, 4);

  /*----PRINT DATA TO SCREEN-----*/
  /*printf("Width: %d\n", originalImage.cols);
  printf("Height: %d\n", originalImage.rows);
  printf("File size: %ld\n", fileSize);
  printf("Bits/pixel: %d\n", getImageInfo(bmpInput, 28, 2));
  printf("No. colors: %d\n\n", nColors);
*/
  /*------------------------------------------------------

		TRIED TO COPY COLOR TABLE OF ANOTHER
		GRAYSCALE IMAGE???????

  ------------------------------------------------------*/
  copyImageInfo(bmpInput, bmpOutput);

  /*----EDIT FILE SIZE TO CONFORM TO 8 BIT BMP---*/
  fseek(bmpOutput, 2, SEEK_SET);
  *pLong = (unsigned long)(54 + 4*256 + (originalImage.cols * originalImage.rows));
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);
  //printf("File size: %ld\n", (long)getImageInfo(bmpOutput, 2, 4));

  /*----CHANGE BIT DEPTH FROM 24 TO 8----*/
  fseek(bmpOutput, 28, SEEK_SET);
  *pLong = (unsigned long)(8);
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);
  //printf("Bits/pixel: %d\n", (int)getImageInfo(bmpOutput, 28, 2));

  /*---CHANGE #COLORS FROM 16M TO 256---*/
  fseek(bmpOutput, 46, SEEK_SET);
  *pLong = (unsigned long)(256);
  fwrite(pLong, sizeof(unsigned long), 1, bmpOutput);
  //printf("No. colors: %d\n", (int)getImageInfo(bmpOutput, 46, 4));

  //createColorTable(grayBmpInput, bmpOutput);

  /*----FOR 24-BIT BMP, THERE IS NO COLOR TABLE-----*/
   fseek(bmpOutput, 54, SEEK_SET);
  //fseek(bmpOutput, (54 + 4*256), SEEK_SET);

  /*-----------READ RASTER DATA-----------*/
  for(r=0; r<=ROWS-1; r++)
  {
    for(c=0; c<=COLS-1; c++)
    {
    
      /*-----WRITE TO NEW BMP FILE------*/
      *pChar = img.data[r][c];
      fwrite(pChar, sizeof(char), 1, bmpOutput);
    }
  }

  fclose(bmpInput);
  fclose(rasterOutput);
  fclose(bmpOutput);
  fclose(grayBmpInput);

}


void imerode(sImage img, se* str_el,int n)
{
   sImage out_img;
        //write("done inside",stdout);
        //source of problem
         //obj=(Object *)(malloc(200*sizeof(Object)));//do ir dynamically 
   out_img.data=malloc((ROWS) * sizeof(int *)); 
   int row,col,s,newr,newc,val; 
   for(row=0;row<ROWS;row++)
          out_img.data[row]=(int *)(malloc((COLS) * sizeof(int))); 
 
   for(row=0;row<ROWS;row++)
     for(col=0;col<COLS;col++)
       {
         val=0;
         //if(img.data[row][col] ==1)
         for(s=0;s<n;s++)
         {
               newr=row+str_el[s].roffset;
               newc=col+str_el[s].coffset;
               //printf("%d %d",str_el[s].roffset,str_el[s].coffset);
               if(newr>=0 && newc>=0 && newr<ROWS && newc <COLS ) 
               if(img.data[newr][newc]==0) {val=0;break;}
               val=255;
         }
         out_img.data[row][col] =val;
        }
        
   for(row=0;row<ROWS;row++)
       for(col=0;col<COLS;col++)
       img.data[row][col]=out_img.data[row][col];
       
for(row=0;row<ROWS;row++)
   free(out_img.data[row]);
   free( out_img.data);
               
  }             
void imdilate(sImage img, se* str_el,int n)
{
   sImage out_img;
        //write("done inside",stdout);
        //source of problem
         //obj=(Object *)(malloc(200*sizeof(Object)));//do ir dynamically 
   out_img.data=malloc((ROWS) * sizeof(int *)); 
   int row,col,s,newr,newc,val; 
   for(row=0;row<ROWS;row++)
          out_img.data[row]=(int *)(malloc((COLS) * sizeof(int))); 
 
   for(row=0;row<ROWS;row++)
     for(col=0;col<COLS;col++)
       {
         val=255;
         //if(img.data[row][col] ==1)
         for(s=0;s<n;s++)
         {
               newr=row+str_el[s].roffset;
               newc=col+str_el[s].coffset;
               //printf("%d %d",str_el[s].roffset,str_el[s].coffset);
               if(newr>=0 && newc>=0 && newr<ROWS && newc <COLS ) 
               if(img.data[newr][newc]==255) {val=255;break;}
               val=0;
         }
         out_img.data[row][col] =val;
        }
        
   for(row=0;row<ROWS;row++)
       for(col=0;col<COLS;col++)
       img.data[row][col]=out_img.data[row][col];
      for(row=0;row<ROWS;row++)
   free(out_img.data[row]);
   free( out_img.data); }
       
void imreconstruct(sImage img,sImage mask, se* str_el,int n)
{
     sImage out_img;
     out_img.data=malloc((ROWS) * sizeof(int *)); 
     int row,col,s,newr,newc,val; 
     for(row=0;row<ROWS;row++)
          out_img.data[row]=(int *)(malloc((COLS) * sizeof(int))); 
     for(row=0;row<ROWS;row++)
                   for(col=0;col<COLS;col++)  
                      out_img.data[row][col]=img.data[row][col];    
     int compare=0;
     while(compare==0)
     {
             // printf("here");
             imdilate(out_img,str_el,n);
             imand(out_img,mask);     
             compare=imcmp(img,out_img);
             for(row=0;row<ROWS;row++)
                   for(col=0;col<COLS;col++)  
                      img.data[row][col]=out_img.data[row][col];
     }
     for(row=0;row<ROWS;row++)
   free(out_img.data[row]);
   free( out_img.data);
}
                              
            
void  imand(sImage img,sImage mask)
{
    
    int row,col;
    for(row=0;row<ROWS;row++)
       for(col=0;col<COLS;col++)  
        img.data[row][col]&=mask.data[row][col];
    }
    
int imcmp(sImage img1,sImage img2)
{
      
       int count=0; //numb of diff pixels
        int row,col;
       for(row=0;row<ROWS;row++)
          for(col=0;col<COLS;col++)   
            if( img1.data[row][col]!=img2.data[row][col]) 
                 {count++;
                 // printf("h");
                  //img.data[row][col]!=out_img.data[row][col];
                  } 
                 // printf("her=%d",count);
       if(count==0) return 1;
       return 0;
}                                                







