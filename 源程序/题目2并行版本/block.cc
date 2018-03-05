#include <stdlib.h>
#include <stdio.h>
#include "block.h"

Block::Block(int row,int col,int length)
{
  row_begin=row;
  row_end=row+length;
  col_begin=col;
  col_end=col+length;
  size=length;

  block=(int*)malloc(sizeof(int)*size*size);


}

Block::Block() {

}

Block::~Block() {

}

//M(i-1,j-1)+S(Xi,Yj)
int Block::one(int i,int j,int **M,int S[26][26],char *X,char *Y)
{
  
  int pre=M[i-1][j-1];//M(i-1,j-1)
  int xi=X[i-1]-65;
  int yj=Y[j-1]-65;
  int Sij=S[xi][yj];
  int o=pre+Sij;
  return o;
}

//M(k,j)+(i-k)^2
int Block::two(int i,int j,int **M,int d)
{
  int t=M[0][j]+i*i;
  for(int index=0;index<i;index++)
  {
    d=(i-index)*(i-index);
    if(M[index][j]+d>=t)
      t=M[index][j]+d;
    //printf("%d\n",t);
  }
 
  //printf("%d\n",t);
  return t;
}

//M(i,k)+(j-k)^2
int Block::three(int i,int j,int **M,int d)
{
  int th=M[i][0]+j*j;
  for(int index=0;index<j;index++)
  {
    d=(j-index)*(j-index);
    if(M[i][index]+d>=th)
      th=M[i][index]+d;
    //printf("%d\n",t);
  }
  //printf("%d\n",th);
  return th;
}

//计算 M(i,j)
int Block::max(int i,int j,int **M,int S[26][26],int d,char *X,char *Y)
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

int* Block::calculate(int **M,int S[26][26],int d,char *X,char *Y)
{ 
    int index=0;
    for(int i=row_begin;i<row_end;i++)
      for(int j=col_begin;j<col_end;j++){
        M[i][j]=max(i,j,M,S,d,X,Y);
        block[index++]=M[i][j];
        //printf("%d\n",block[index++]);
      }
  
  return block;
}
