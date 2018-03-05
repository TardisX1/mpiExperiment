#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "block.h"

int main (int argc, char **argv)
{
  int rank, numprocs=0;//线程
  int namelen;//处理器
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int i,j,dimension=0,dimensionx,row_number=0,d=-8;//必要条件
  int LX,LY;//M初始化
  int **M;//目标矩阵
  char *X,*Y;
  int length,block_count,block_line=1,block_x,block_y;
  Block local_block;
  int row_begin,col_begin;
  int* bloc;

  MPI_Init (&argc, &argv);//MPI初始化
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);//获得当前进程号
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);//获得进程总数
  //printf("%d\n",numprocs);
  MPI_Get_processor_name (processor_name, &namelen);//获得处理器名
  
  Block** blocks;
  
  blocks=new Block*[numprocs];
  for(i=0;i<numprocs;i++)
    blocks[i]=new Block[numprocs];

  //行列数
  dimension=atoi(argv[1]);
  //dimension=200;
  //printf("%d\n",dimension);
  dimensionx=dimension+1;
  //M的行号
  row_number=atoi(argv[2]);
  //row_number=7;
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
  int* M_storage=new int[dimensionx*dimensionx];
  M=(int**)malloc(sizeof(int*)*dimensionx);  
  for(i=0;i<dimensionx;i++)  
    M[i]=&M_storage[i*dimensionx];
  //初始化M
  LX=dimensionx;
  LY=dimensionx;
  for(int p=0;p<LX;p++)
    M[0][p]=p*d;
  for(int q=0;q<LY;q++)
    M[q][0]=q*d;

  length=(dimension/numprocs);//分块长度
  if(length==0){//核的数量大于dimension 用进程1来计算
    length=dimension;
    numprocs=1;
  }
  
  bloc=(int*)malloc(sizeof(int)*length*length);
  //printf("%d\n",length);
  for(i=0;i<numprocs;i++)//分块及设定前置个数
    for(j=0;j<numprocs;j++) 
      blocks[i][j]=Block(i*length+1,j*length+1,length);
  

  //1算第一小块
  local_block=blocks[0][0];
  local_block.block=blocks[0][0].calculate(M,S,d,X,Y);
  //printf("%d \n",local[0]);  
  
  //Block block=Block(1,1,3);
  //int* test=block.calculate(M,S,d,X,Y);

  //2计算左上分块
  for(block_line=1;block_line<numprocs;block_line++)
  {
    MPI_Bcast(M_storage,LX*LY,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank<=block_line)
    {
      block_x=block_line-rank;
      block_y=rank;
      local_block=blocks[block_x][block_y];
      row_begin=local_block.row_begin;
      //printf("%d \n",row_begin);
      col_begin=local_block.col_begin;
      bloc=local_block.calculate(M,S,d,X,Y);
        
      if(rank!=0)
      {
        MPI_Send(&row_begin,1,MPI_INT,0,1,MPI_COMM_WORLD);
        MPI_Send(&col_begin,1,MPI_INT,0,2,MPI_COMM_WORLD);
        MPI_Send(bloc,length*length,MPI_INT,0,3,MPI_COMM_WORLD);
        
      }
      else{
        
        for(int c=1;c<=block_line;c++)
        {
          MPI_Recv(&row_begin,1,MPI_INT,c,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          //printf("%d process\n",row_begin);
          MPI_Recv(&col_begin,1,MPI_INT,c,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          MPI_Recv(bloc,length*length,MPI_INT,c,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          block_count=0;
          for(i=row_begin;i<row_begin+length;i++)//放入M中
            for(j=col_begin;j<col_begin+length;j++)
              M[i][j]=bloc[block_count++];       
        }
        
      }
      
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  //3计算右下分块
  for(block_line=1;block_line<numprocs;block_line++)
  {
    MPI_Bcast(M_storage,LX*LY,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank<numprocs-block_line)
    {
      block_x=numprocs-rank-1;
      block_y=block_line+rank;
      local_block=blocks[block_x][block_y];
      row_begin=local_block.row_begin;
      //printf("%d \n",row_begin);
      col_begin=local_block.col_begin;
      bloc=local_block.calculate(M,S,d,X,Y);
        
      if(rank!=0)
      {
        MPI_Send(&row_begin,1,MPI_INT,0,1,MPI_COMM_WORLD);
        MPI_Send(&col_begin,1,MPI_INT,0,2,MPI_COMM_WORLD);
        MPI_Send(bloc,length*length,MPI_INT,0,3,MPI_COMM_WORLD);
        
      }
      else{
        
        for(int c=1;c<numprocs-block_line;c++)
        {
          MPI_Recv(&row_begin,1,MPI_INT,c,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          //printf("%d process\n",row_begin);
          MPI_Recv(&col_begin,1,MPI_INT,c,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          MPI_Recv(bloc,length*length,MPI_INT,c,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          block_count=0;
          for(i=row_begin;i<row_begin+length;i++)//放入M中
            for(j=col_begin;j<col_begin+length;j++)
              M[i][j]=bloc[block_count++];       
        }
        
      }
      
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  //4计算未分块的行
  if(rank==0)
  {
    int left=(dimension%numprocs);//分块的余数
    int start=dimension-left+1;//未分块行列的开始
    
    for(i=start;i<dimensionx;i++)//行
      for(j=1;j<start;j++)
        M[i][j]=local_block.max(i,j,M,S,d,X,Y);

    for(j=start;j<dimensionx;j++)//列
      for(i=1;i<start;i++)
        M[i][j]=local_block.max(i,j,M,S,d,X,Y);

    for(i=start;i<dimensionx;i++)
      for(j=start;j<dimensionx;j++)
        M[i][j]=local_block.max(i,j,M,S,d,X,Y);

  }
  

  //小矩阵
  /*for(i=1;i<dimensionx;i++){
    for(j=1;j<dimensionx;j++)
      M[i][j]=max(i,j,M,S,d,X,Y);
  }*/
    
  

  //最后进程0输出
  if(rank==0){
    /*for(i=0;i<LX;i++){//输出整个矩阵
      for(j=0;j<LY;j++)
        printf("%d ",M[i][j]);
    printf("\n");
    }*/
    for(i=row_number,j=0;j<LY;j++)//输出第i行
      printf("%d ",M[i][j]);
    printf("\n");
  }

  //fprintf (stderr, "Hello World! Process %d of %d on %s\n", rank, numprocs, processor_name);

  //int a=two(3,3,M,-1);

  //释放动态内存
  free(M);
  free(X);
  free(Y);
  MPI_Finalize ();//结束

  return 0;
}





