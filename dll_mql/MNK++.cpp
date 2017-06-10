#include <stdio.h>
#include <process.h>
#include <math.h>
double *MNK_a, *MNK_b, **MNK_sums;  // данные для МНК никуда не выносить!
int MNK_N, MNK_K;
//N - number of data points
//K - polinom power
//K<=N
void freematrix()
{
  //free memory for matrixes
  int i;
  for(i=0; i<MNK_K+1; i++)
    {
      delete MNK_sums[i];
    }
  delete MNK_b;
  delete MNK_sums;
}
void allocmatrix()
{
  //allocate memory for matrixes
  int i,j;
//  MNK_a = new double[MNK_K+1];
  MNK_b = new double[MNK_K+1];
  MNK_sums = new double*[MNK_K+1];
  if( MNK_a==NULL || MNK_sums==NULL)
    {
      printf("\nNot enough memory to allocate. N=%d, K=%d\n", MNK_N, MNK_K);
      exit(-1);
    }
  for(i=0; i<MNK_K+1; i++)
    {
      MNK_sums[i] = new double[MNK_K+1];
      if(MNK_sums[i]==NULL)
        {
          printf("\nNot enough memory to allocate for %d equations.\n", MNK_K+1);
        }
    }
  for(i=0; i<MNK_K+1; i++)
    {
      MNK_a[i]=0;
      MNK_b[i]=0;
      for(j=0; j<MNK_K+1; j++)
        {
          MNK_sums[i][j] = 0;
        }
    }
}
void readmatrix(double *x, double *y)
{
  int i=0,j=0, k=0;
  //init square sums matrix
  for(i=0; i<MNK_K+1; i++)
    {
      for(j=0; j<MNK_K+1; j++)
        {
          MNK_sums[i][j] = 0;
          for(k=0; k<MNK_N; k++)
            {
              MNK_sums[i][j] += pow(x[k], i+j);
            }
        }
    }
  //init free coefficients column
  for(i=0; i<MNK_K+1; i++)
    {
      for(k=0; k<MNK_N; k++)
        {
          MNK_b[i] += pow(x[k], i) * y[k];
        }
    }
}
void diagonal()
{
  int i, j, k;
  double temp=0;
  for(i=0; i<MNK_K+1; i++)
    {
      if(MNK_sums[i][i]==0)
        {
          for(j=0; j<MNK_K+1; j++)
            {
              if(j==i)
                {
                  continue;
                }
              if(MNK_sums[j][i] !=0 && MNK_sums[i][j]!=0)
                {
                  for(k=0; k<MNK_K+1; k++)
                    {
                      temp = MNK_sums[j][k];
                      MNK_sums[j][k] = MNK_sums[i][k];
                      MNK_sums[i][k] = temp;
                    }
                  temp = MNK_b[j];
                  MNK_b[j] = MNK_b[i];
                  MNK_b[i] = temp;
                  break;
                }
            }
        }
    }
}
void interp(double *x, double *y, double *rezult, int N, int K)
{
  int i=0,j=0, k=0;
  MNK_N=N;
  MNK_K=K;
  MNK_a=rezult;
  allocmatrix();
  //read data from file
  readmatrix(x, y);
  //check if there are 0 on main diagonal and exchange rows in that case
  diagonal();
  //process rows
  for(k=0; k<MNK_K+1; k++)
    {
      for(i=k+1; i<MNK_K+1; i++)
        {
          if(MNK_sums[k][k]==0)
            {
              printf("\nSolution is not exist.\n");
              return;
            }
          double M = MNK_sums[i][k] / MNK_sums[k][k];
          for(j=k; j<MNK_K+1; j++)
            {
              MNK_sums[i][j] -= M * MNK_sums[k][j];
            }
          MNK_b[i] -= M*MNK_b[k];
        }
    }
  for(i=(MNK_K+1)-1; i>=0; i--)
    {
      double s = 0;
      for(j = i; j<MNK_K+1; j++)
        {
          s = s + MNK_sums[i][j]*MNK_a[j];
        }
      MNK_a[i] = (MNK_b[i] - s) / MNK_sums[i][i];
    }
  rezult=MNK_a;
  freematrix();
}
