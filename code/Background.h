#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#define ROWS 240
#define COLS 320

#define MAX 50
#define N 8

//int Mean[HEIGHT][WIDTH],Var[HEIGHT][WIDTH],B[HEIGHT][WIDTH][N];
unsigned int B[ROWS/2][COLS][N];
void find_bkg( unsigned int Mean[ROWS][COLS], unsigned int Var[ROWS][COLS],unsigned int img_num )
{
     //clock_t start1=clock();
      struct timeval tval1,tval2,tval3,tval4;
   struct timezone tz;
   gettimeofday(&tval1,&tz);
  int imgno=img_num,val=imgno+N,flag=0,i,j=0,k=0;
  int l,mid;
  float temp=0,I1,n,nv,d,x1,x2,W[N]={0},arr[N],t=0,x;
   unsigned char b,g,r;
    unsigned char pChar[ROWS/2*COLS*3];
   int u; 
   double time_r=0,time_w=0; 
  char s[20];
  if(val>=1000) flag=0;
      else flag=1;
  if(flag==0)  
  {
 
   for(i=0;i<N;i++)
   {
    if(imgno+i*2<1000)  sprintf(s,"img_00%d.bmp",imgno+i*2);
      else sprintf(s,"img_0%d.bmp",imgno+i*2);

// load from bitmap .............
   printf("READING IMAGES");
   FILE *fp;
   fp=fopen(s,"rb");
   if(fp == NULL) {
                   printf("Not able to open file\n");
                   return ;
                  }
   //fskip(fp,46+2*sizeof(int));
    fseek(fp, 54, SEEK_SET);
   // for(k=0; k<46+2*sizeof(int); k++)
     //     fgetc(fp);
  
   gettimeofday(&tval3,&tz);    
   fread(pChar, ROWS/2*COLS*3*sizeof(unsigned char), 1, fp);    
   gettimeofday(&tval4,&tz);
          time_r+=(tval4.tv_usec-tval3.tv_usec)/1000.0;
       //  time/=1000;
          time_r+=(tval4.tv_sec-tval3.tv_sec)*1000;   
    u=0;
    for(k=0; k<ROWS/2; k++)
    {
     for(j=0; j<COLS; j++)
     {
   //   fread(&b,sizeof(char),1,fp);
     // fread(&g,sizeof(char),1,fp);
     // fread(&r,sizeof(char),1,fp);
      
      B[k][j][i]=(int)(0.2989*(double)pChar[u+2]+0.5870*(double)pChar[u+1]   
                                  +0.1140*(double)pChar[u]);
                                  u+=3;
    }
   }
   fclose(fp);
    } 
   }// if ends
   else 
   {
     for(i=0;i<N;i++) 
     {
       if(imgno+i*2<1000)  sprintf(s,"img_00%d.bmp",imgno+i*2);
        else sprintf(s,"img_0%d.bmp",imgno+i*2);
     
//load From Bitmap..............
     FILE *fp;
     fp=fopen(s,"rb");
     if(fp == NULL){
                     printf("Not able to open file\n");
                     return;
                   }
   //  fskip(fp,46+2*sizeof(int));
      fseek(fp, 54, SEEK_SET);
   // for(k=0; k<46+2*sizeof(int); k++)
     //     fgetc(fp);
   gettimeofday(&tval3,&tz); 
      // printf("READ IMAGES 1");   
   fread(pChar, ROWS/2*COLS*3*sizeof(unsigned char), 1, fp);    
   gettimeofday(&tval4,&tz);
          time_r+=(tval4.tv_usec-tval3.tv_usec)/1000.0;
       //  time/=1000;
          time_r+=(tval4.tv_sec-tval3.tv_sec)*1000; 
// printf(" \n Total Read time in bkg%f \n ",time_r);   
    
    u=0;
    for(k=0; k<ROWS/2; k++)
    {
     for(j=0; j<COLS; j++)
     {
   //   fread(&b,sizeof(char),1,fp);
     // fread(&g,sizeof(char),1,fp);
     // fread(&r,sizeof(char),1,fp);
      
      B[k][j][i]=(int)(0.2989*(double)pChar[u+2]+0.5870*(double)pChar[u+1]   
                                  +0.1140*(double)pChar[u]);
      u+=3;
    }
   }
      fclose(fp);
    }
   } // else ends
  //printf("read images");
  for(i=0;i<ROWS/2;i++)
  {
   for(j=0;j<COLS;j++) 
   {
      for(k=0;k<N;k++)
      arr[k]=B[i][j][k];
      for(k=0;k<N;k++)   //sort
      {
        for(l=0;l<k;l++)
          if(arr[l]>arr[k]) { 
                             temp=arr[l];
                             arr[l]=arr[k];
                             arr[k]=temp;
                            } 
      }
      mid=(N)/2;
      if(N%2) mid++;
      I1=arr[mid];
      n=0;
      nv=0;
      d=0;
                
     for(k=0;k<N;k++)
     {
       x1=B[i][j][k];
	   //x2=B[i][j][k+1];
       t = expf(((x1-I1)*(x1-I1))/(-2*25));
       n=n+(t*x1);       
       d=d+t;
       W[k]=t;
     }
     /*if(!d) 
     {
            Mean[i][j]=B[i][j][0];
            Var[i][j]=0;
     }
     else
     {*/
       float val=n;
       val/=d;
       Mean[i][j]=(unsigned char)val;
       x=Mean[i][j];
     
       for(k=0;k<N;k++)
        {
         x1=B[i][j][k];
         nv=nv+(1.0*W[k]*(x1-x)*(x1-x));
        // nv=nv+1.0*W(k)*(x1-x)*(x1-x);
        }
        val=5*nv;
        val/=4;
        val/=d;
       // printf("%f y %f y %f\t",nv,d,val);
        //Var[i][j]=(unsigned char)(sqrtf(val));
		Var[i][j]=(unsigned char)(val);
     //}
       
    }
   
    
     
   } 
   //Next half
   
     if(flag==0)  
  {
 
   for(i=0;i<N;i++)
   {
    if(imgno+i*2<1000)  sprintf(s,"img_00%d.bmp",imgno+i*2);
      else sprintf(s,"img_0%d.bmp",imgno+i*2);

// load from bitmap .............
   printf("READING IMAGES");
   FILE *fp;
   fp=fopen(s,"rb");
   if(fp == NULL) {
                   printf("Not able to open file\n");
                   return ;
                  }
   //fskip(fp,46+2*sizeof(int));
    fseek(fp, 54+ ROWS/2*COLS*3, SEEK_SET);
   // for(k=0; k<46+2*sizeof(int); k++)
     //     fgetc(fp);
   gettimeofday(&tval3,&tz);
   fread(pChar, ROWS/2*COLS*3*sizeof(unsigned char), 1, fp);    
   gettimeofday(&tval4,&tz);
   time_r+=(tval4.tv_usec-tval3.tv_usec)/1000.0;
       //  time/=1000;
    time_r+=(tval4.tv_sec-tval3.tv_sec)*1000;
// printf(" \n Total Read time in bkg%f \n ",time_r);  
    u=0;
    for(k=0; k<ROWS/2; k++)
    {
     for(j=0; j<COLS; j++)
     {
   //   fread(&b,sizeof(char),1,fp);
     // fread(&g,sizeof(char),1,fp);
     // fread(&r,sizeof(char),1,fp);
      
      B[k][j][i]=(int)(0.2989*(double)pChar[u+2]+0.5870*(double)pChar[u+1]   
                                  +0.1140*(double)pChar[u]);
                                  u+=3;
    }
   }
   fclose(fp);
    } 
   }// if ends
   else 
   {
     for(i=0;i<N;i++) 
     {
       if(imgno+i*2<1000)  sprintf(s,"img_00%d.bmp",imgno+i*2);
        else sprintf(s,"img_0%d.bmp",imgno+i*2);
     
//load From Bitmap..............
     FILE *fp;
     fp=fopen(s,"rb");
     if(fp == NULL){
                     printf("Not able to open file\n");
                     return;
                   }
   //  fskip(fp,46+2*sizeof(int));
      fseek(fp, 54+ ROWS/2*COLS*3, SEEK_SET);
   // for(k=0; k<46+2*sizeof(int); k++)
     //     fgetc(fp);
     gettimeofday(&tval3,&tz);// printf("READ IMAGES 2"); 
   fread(pChar, ROWS/2*COLS*3*sizeof(unsigned char), 1, fp);    
   gettimeofday(&tval4,&tz);
   time_r+=(tval4.tv_usec-tval3.tv_usec)/1000.0;
       //  time/=1000;
    time_r+=(tval4.tv_sec-tval3.tv_sec)*1000;
 //printf(" \n Total Read time in bkg%f \n ",time_r);  
    
    u=0;
    for(k=0; k<ROWS/2; k++)
    {
     for(j=0; j<COLS; j++)
     {
 
      
      B[k][j][i]=(int)(0.2989*(double)pChar[u+2]+0.5870*(double)pChar[u+1]   
                                  +0.1140*(double)pChar[u]);
      u+=3;
    }
   }
      fclose(fp);
    }
   } // else ends
  //printf("read images");
  for(i=0;i<ROWS/2;i++)
  {
   for(j=0;j<COLS;j++) 
   {
      for(k=0;k<N;k++)
      arr[k]=B[i][j][k];
      for(k=0;k<N;k++)   //sort
      {
        for(l=0;l<k;l++)
          if(arr[l]>arr[k]) { 
                             temp=arr[l];
                             arr[l]=arr[k];
                             arr[k]=temp;
                            } 
      }
      mid=(N)/2;
      if(N%2) mid++;
      I1=arr[mid];
      n=0;
      nv=0;
      d=0;
                
     for(k=0;k<N;k++)
     {
       x1=B[i][j][k];
	   //x2=B[i][j][k+1];
       t = expf( ((x1-I1)*(x1-I1))/(-2*25));
       n=n+(t*x1);       
       d=d+t;
       W[k]=t;
     }
     /*if(!d) 
     {
            Mean[i+ROWS/2][j]=B[i][j][0];
            Var[i+ROWS/2][j]=0;
     }
     else
     {*/
       float val=n;
       val/=d;
       Mean[i+ROWS/2][j]=(unsigned char)val;
       x=Mean[i+ROWS/2][j];
     
       for(k=0;k<N;k++)
        {
         x1=B[i][j][k];
         nv=nv+(1.0*W[k]*(x1-x)*(x1-x));
        }
        val=5*nv;
        val/=4;
        val/=d;
       // printf("%f y %f y %f\t",nv,d,val);
        Var[i+ROWS/2][j]=(unsigned char)val;
     //}
       // printf("%d %d\t",Mean[i+ROWS/2][j],Var[i+ROWS/2][j] ); 
    }
   
    
     
   } 
    gettimeofday(&tval2,&tz);  
    double time=(tval2.tv_usec-tval1.tv_usec);
    time/=1000;
    time+=(tval2.tv_sec-tval1.tv_sec)*1000;
  // char str[14];
    // sprintf(str,"Mea_00%d.bmp",500); 
      //  writegraybmp(Mean,str);   
     printf("Total Time by gettimeofday for BKG calc(at each pass)= %f \n Total Read time in bkg%f \n ",time,time_r);
    // printf("here");
 

} 


