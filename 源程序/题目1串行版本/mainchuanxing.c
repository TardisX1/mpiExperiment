#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int one(int i,int j,int **M,int S[26][26],char *X,char *Y);
int two(int i,int j,int **M,int d);
int three(int i,int j,int **M,int d);
int max(int i,int j,int **M,int S[26][26],int d,char *X,char *Y);


int main (int argc, char **argv)
{
  int rank, numprocs;//线程
  int namelen;//处理器
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int i,j,dimension=0,dimensionx,row_number=0,d=-8;//必要条件
  int LX,LY,p,q;//M初始化
  int **M;//目标矩阵
  char *X,*Y;

  MPI_Init (&argc, &argv);//MPI初始化
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);//获得当前进程号
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);//获得进程总数
  //printf("%d\n",numprocs);
  MPI_Get_processor_name (processor_name, &namelen);//获得处理器名
  
  //行列数
  dimension=atoi(argv[1]);
  //printf("%d\n",dimension);
  dimensionx=dimension+1;
  //M的行号
  row_number=atoi(argv[2]);
  //printf("%d\n",row_number);
  
  char residues[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
  X = (char*)malloc(sizeof(char)*(dimension)); 
  Y = (char*)malloc(sizeof(char)*(dimension)); 
  for (i = 0; i < dimension; i++) 
  {   
    X[i] = residues[i%26];  
    Y[i] = residues[i%26]; 
  } 

             /* A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z */ 
  int S[26][26] =
  {  
     /* A */ {  4,-2, 0,-2,-1,-2, 0,-2,-1, 0,-1,-1,-1,-2, 0,-1,-1,-1, 1, 0, 0, 0,-3, 0,-2,-1},     
     /* B */ { -2, 4,-3, 4, 1,-3,-1, 0,-3, 0, 0,-4,-3, 3, 0,-2, 0,-1, 0,-1, 0,-3,-4,-1,-3, 1},     
     /* C */ {  0,-3, 9,-3,-4,-2,-3,-3,-1, 0,-3,-1,-1,-3, 0,-3,-3,-3,-1,-1, 0,-1,-2,-2,-2,-3},     
     /* D */ { -2, 4,-3, 6, 2,-3,-1,-1,-3, 0,-1,-4,-3, 1, 0,-1, 0,-2, 0,-1, 0,-3,-4,-1,-3, 1},     
     /* E */ { -1, 1,-4, 2, 5,-3,-2, 0,-3, 0, 1,-3,-2, 0, 0,-1, 2, 0, 0,-1, 0,-2,-3,-1,-2, 4},     
     /* F */ { -2,-3,-2,-3,-3, 6,-3,-1, 0, 0,-3, 0, 0,-3, 0,-4,-3,-3,-2,-2, 0,-1, 1,-1, 3,-3},     
     /* G */ {  0,-1,-3,-1,-2,-3, 6,-2,-4, 0,-2,-4,-3, 0, 0,-2,-2,-2, 0,-2, 0,-3,-2,-1,-3,-2},     
     /* H */ { -2, 0,-3,-1, 0,-1,-2, 8,-3, 0,-1,-3,-2, 1, 0,-2, 0, 0,-1,-2, 0,-3,-2,-1, 2, 0},     
     /* I */ { -1,-3,-1,-3,-3, 0,-4,-3, 4, 0,-3, 2, 1,-3, 0,-3,-3,-3,-2,-1, 0, 3,-3,-1,-1,-3},     
     /* J */ {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     
     /* K */ { -1, 0,-3,-1, 1,-3,-2,-1,-3, 0, 5,-2,-1, 0, 0,-1, 1, 2, 0,-1, 0,-2,-3,-1,-2, 1},     
     /* L */ { -1,-4,-1,-4,-3, 0,-4,-3, 2, 0,-2, 4, 2,-3, 0,-3,-2,-2,-2,-1, 0, 1,-2,-1,-1,-3},     
     /* M */ { -1,-3,-1,-3,-2, 0,-3,-2, 1, 0,-1, 2, 5,-2, 0,-2, 0,-1,-1,-1, 0, 1,-1,-1,-1,-1},     
     /* N */ { -2, 3,-3, 1, 0,-3, 0, 1,-3, 0, 0,-3,-2, 6, 0,-2, 0, 0, 1, 0, 0,-3,-4,-1,-2, 0},     
     /* O */ {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},    
     /* P */ { -1,-2,-3,-1,-1,-4,-2,-2,-3, 0,-1,-3,-2,-2, 0, 7,-1,-2,-1,-1, 0,-2,-4,-2,-3,-1},    
     /* Q */ { -1, 0,-3, 0, 2,-3,-2, 0,-3, 0, 1,-2, 0, 0, 0,-1, 5, 1, 0,-1, 0,-2,-2,-1,-1, 3},    
     /* R */ { -1,-1,-3,-2, 0,-3,-2, 0,-3, 0, 2,-2,-1, 0, 0,-2, 1, 5,-1,-1, 0,-3,-3,-1,-2, 0},   
     /* S */ {  1, 0,-1, 0, 0,-2, 0,-1,-2, 0, 0,-2,-1, 1, 0,-1, 0,-1, 4, 1, 0,-2,-3, 0,-2, 0},    
     /* T */ {  0,-1,-1,-1,-1,-2,-2,-2,-1, 0,-1,-1,-1, 0, 0,-1,-1,-1, 1, 5, 0, 0,-2, 0,-2,-1},    
     /* U */ {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
     /* V */ {  0,-3,-1,-3,-2,-1,-3,-3, 3, 0,-2, 1, 1,-3, 0,-2,-2,-3,-2, 0, 0, 4,-3,-1,-1,-2},  
     /* W */ { -3,-4,-2,-4,-3, 1,-2,-2,-3, 0,-3,-2,-1,-4, 0,-4,-2,-3,-3,-2, 0,-3,11,-2, 2,-3}, 
     /* X */ {  0,-1,-2,-1,-1,-1,-1,-1,-1, 0,-1,-1,-1,-1, 0,-2,-1,-1, 0, 0, 0,-1,-2,-1,-1,-1}, 
     /* Y */ { -2,-3,-2,-3,-2, 3,-3, 2,-1, 0,-2,-1,-1,-2, 0,-3,-1,-2,-2,-2, 0,-1, 2,-1, 7,-2}, 
     /* Z */ { -1, 1,-3, 1, 4,-3,-2, 0,-3, 0, 1,-3,-1, 0, 0,-1, 3, 0, 0,-1, 0,-2,-3,-1,-2, 4}  
  }; 

  //动态数组M
  M=(int**)malloc(sizeof(int*)*dimensionx);  
  for(i=0;i<dimensionx;i++)  
    M[i]=(int*)malloc(sizeof(int)*dimensionx);  
  //初始化M
  LX=dimensionx;
  LY=dimensionx;
  for(p=0;p<LX;p++)
    M[0][p]=p*d;
  for(q=0;q<LY;q++)
    M[q][0]=q*d;

  //小矩阵
  for(i=1;i<dimensionx;i++){
    for(j=1;j<dimensionx;j++)
      M[i][j]=max(i,j,M,S,d,X,Y);
  }
    
  /*for(i=0;i<LX;i++){//输出整个矩阵
    for(j=0;j<LY;j++)
      printf("%d ",M[i][j]);
    printf("\n");
  }*/
  for(i=row_number,j=0;j<LY;j++)//输出第i行
    printf("%d ",M[i][j]);
  printf("\n");

  //fprintf (stderr, "Hello World! Process %d of %d on %s\n", rank, numprocs, processor_name);

  //int a=two(3,3,M,-1);

  //释放动态内存
  free(M);
  free(X);
  free(Y);
  MPI_Finalize ();//结束

  return 0;
}

//M(i-1,j-1)+S(Xi,Yj)
int one(int i,int j,int **M,int S[26][26],char *X,char *Y)
{
  
  int pre=M[i-1][j-1];//M(i-1,j-1)
  int xi=X[i-1]-65;
  int yj=Y[j-1]-65;
  int Sij=S[xi][yj];
  int o=pre+Sij;
  return o;
}

//M(k,j)+d
int two(int i,int j,int **M,int d)
{
  int t=M[0][j];
  for(int index=0;index<i;index++)
  {
    if(M[index][j]>=t)
      t=M[index][j];
    //printf("%d\n",t);
  }
  t+=d;
  //printf("%d\n",t);
  return t;
}

//M(i,k)+d
int three(int i,int j,int **M,int d)
{
  int th=M[i][0];
  for(int index=0;index<j;index++)
  {
    if(M[i][index]>=th)
      th=M[i][index];
    //printf("%d\n",t);
  }
  th+=d;
  //printf("%d\n",th);
  return th;
}

//计算 M(i,j)
int max(int i,int j,int **M,int S[26][26],int d,char *X,char *Y)
{
  int o=one(i,j,M,S,X,Y);
  int t=two(i,j,M,d);
  int th=three(i,j,M,d);
  int ret=o;
  if(t>ret)
    ret=t;
  if(th>ret)
    ret=th;

  return ret;
}
