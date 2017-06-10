#include <all>
#include "gl.h"   // описание глобальных переменных (перед нашим конфигом)
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
extern "C" __declspec(dllexport) int Renco(double *input, double *input1, __int64 *input_time,
    double *output, __int64 *Renco_time, int kolinp)
{
  // количество анализируемых чисел  kollinp
  int i, j;
  j=0;
  output[0]=(input[0]+input1[0])/2.0;
  Renco_time[0]=input_time[0];
  for (i=1; i<kolinp; i++)
    {
      while ((input[i]+input1[i])/2.0>=(output[j]+shag_renco))
        {
          j++;
          output[j]=output[j-1]+shag_renco;
          Renco_time[j]=input_time[i];
          Renco_kol[j]=i;
        }
      while ((input[i]+input1[i])/2.0<=(output[j]-shag_renco))
        {
          j++;
          output[j]=output[j-1]-shag_renco;
          Renco_time[j]=input_time[i];
          Renco_kol[j]=i;
        }
    }
  return(j);
}

