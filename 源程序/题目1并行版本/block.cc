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

//M(k,j)+d
int Block::two(int i,int j,int **M,int d)
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
int Block::three(int i,int j,int **M,int d)
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
