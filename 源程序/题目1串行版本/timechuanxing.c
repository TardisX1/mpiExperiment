#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

double MPI_Wtime(void);

int main(int argc,char **argv)
{
  char *filename=argv[1];//测速程序名
  char *dimension=argv[2];//测试参数（可随意指定）
  char *row_number=argv[3];//测试参数
  char *rt=malloc(sizeof(char)*(4+strlen(filename)+strlen(dimension)+strlen(row_number)));//形成的命令行
  strcpy(rt,"./");
  strcat(rt,filename);
  strcat(rt," ");
  strcat(rt,dimension);
  strcat(rt," ");
  strcat(rt,row_number);
  //system("./main 200 7");
  //printf("%s\n",rt);

  MPI_Init (&argc, &argv);//MPI初始化
  double start,finish,pass;
  start = MPI_Wtime();//开始时间
  system(rt);//运行待测试程序
  finish= MPI_Wtime();//结束时间
  pass=finish-start;//间隔
  printf("程序运行时间为：%f\n",pass);
  MPI_Finalize();//MPI结束

  return 0;
}
