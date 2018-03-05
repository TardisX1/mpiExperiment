
class Block 
{
public:
  int row_begin;
  int row_end;
  int col_begin;
  int col_end;
  int size;
  int* block;

  Block(int row,int col,int length);
  Block();
  ~Block();
  int one(int i,int j,int **M,int S[26][26],char *X,char *Y);
  int two(int i,int j,int **M,int d);
  int three(int i,int j,int **M,int d);
  int max(int i,int j,int **M,int S[26][26],int d,char *X,char *Y);
  
  int* calculate(int **M,int S[26][26],int d,char *X,char *Y);
};
