#include <all>
#include "gl.h"   // описание глобальных переменных (перед нашим конфигом)
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void Freq(double *input, double *output, int kollinp);
void TrigFit(double *input, double *output, int kollinp);
double w,m,a,b; // параметры синусоиды
extern "C" __declspec(dllexport) double fourier_V1(double *input, double *output, int kollinp, int prognoz)
{
  // количество анализируемых чисел  kollinp
  //--- main cycle
  //--- prepare input data
  double av=0;
  for(int i=0; i<kollinp; i++) // присваивание масссива
    {
      av+=input[i];
    }
  av/=kollinp;  //Средняя линия
  for(int i=0; i<kollinp+prognoz; i++)
    {
      output[i]=av;
      //  xm_old[i+start]=0.0; // минусы по синусам
    }
  for(int harm=1; harm<=fourier_garm; harm++)  // цикл по количеству гармоник
    {
      Freq(input, output, kollinp);
      for(int i=0; i<kollinp+prognoz; i++)
        {
          output[i]+=m+a*cos(w*(i))+b*sin(w*(i));  // История
        }
    }
  return(0);
}
//+------------------------------------------------------------------+
//| Quinn and Fernandes algorithm for finding frequency |
//+------------------------------------------------------------------+
void Freq(double *input, double *output, int kollinp)
{
  double *z;
  z = new double[kollinp+1];
  double alpha=0.0;
  double beta=2.0;
  z[0]=input[0]-output[0];
  while(fabs(alpha-beta)>FreqTOL)
    {
      alpha=beta;
      z[1]=input[1]-output[1]+alpha*z[0];
      double num=z[0]*z[1];
      double den=z[0]*z[0];
      for(int i=2; i<kollinp; i++)
        {
          z[i]=input[i]-output[i]+alpha*z[i-1]-z[i-2];
          num+=z[i-1]*(z[i]+z[i-2]);
          den+=z[i-1]*z[i-1];
        }
      beta=num/den;
    }
  w=acos(beta/2.0);
  TrigFit(input, output, kollinp);
  delete [] z;
}
//+------------------------------------------------------------------+
//| Least-squares fitting of trigonometric series |
//+------------------------------------------------------------------+
void TrigFit(double *input, double *output, int kollinp)
{
  double Sc =0.0;
  double Ss =0.0;
  double Scc=0.0;
  double Sss=0.0;
  double Scs=0.0;
  double Sx =0.0;
  double Sxc=0.0;
  double Sxs=0.0;
  for(int i=0; i<kollinp; i++)
    {
      double c=cos(w*i);
      double s=sin(w*i);
      double dx=input[i]-output[i];
      Sc +=c;
      Ss +=s;
      Scc+=c*c;
      Sss+=s*s;
      Scs+=c*s;
      Sx +=dx;
      Sxc+=dx*c;
      Sxs+=dx*s;
    }
  Sc /=kollinp;
  Ss /=kollinp;
  Scc/=kollinp;
  Sss/=kollinp;
  Scs/=kollinp;
  Sx /=kollinp;
  Sxc/=kollinp;
  Sxs/=kollinp;
  if(w==0.0)
    {
      m=Sx;
      a=0.0;
      b=0.0;
    }
  else
    {
      // calculating a, b, and m
      double den=(Scs-Sc*Ss)*(Scs-Sc*Ss)-(Scc-Sc*Sc)*(Sss-Ss*Ss);
      a=((Sxs-Sx*Ss)*(Scs-Sc*Ss)-(Sxc-Sx*Sc)*(Sss-Ss*Ss))/den;
      b=((Sxc-Sx*Sc)*(Scs-Sc*Ss)-(Sxs-Sx*Ss)*(Scc-Sc*Sc))/den;
      m=Sx-a*Sc-b*Ss;
    }
}
