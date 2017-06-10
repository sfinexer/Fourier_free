#include <all>
#include "gl.h"   // описание глобальных переменных (перед нашим конфигом)
int load(string file); // загрузка данных для внутренних целей
extern "C" __declspec(dllexport) int Renco(double *input, double *input1, __int64 *Time,
    double *output, __int64 *Renco_time, int kolinp);
void interp(double *x, double *y, double *rezult, int N, int K);
extern "C" __declspec(dllexport) double fourier_V1(double *input, double *output, int kollinp, int prognoz);
void del_mem();
extern "C" __declspec(dllexport) void ini();
extern "C" __declspec(dllexport) double prognoz_chen_mql()
{
  //  return (18.5);
  put_prog="MQL5/Libraries/";
  ini();
  vrem_kol=0;
  // прогноз дальнейшего поведения цены
  //cout<<"Чтение БД"<<endl;
  load((put_prog+"../Files/data.db").c_str());
  int progn=0;
  double prognoz, delta_prognoz;
  int RENKO;
  double kol_iter=0.0; // количество итераций
  // создаем ренко бары
  RENKO=Renco(Open, Close, Time, Renco_out, Renco_time, kol_blokov);
  inite++;
  //cout << "Renco=  "<<RENKO << " ("<<RENKO/60<<")"<< endl;
  tm* timeinfo;
  time_t seconds;
  int Hous;
  int stepen=1;  // степень коррекционной кривой
  int i, j;
  double *x, *y, *rezult, *output;
  double old_line, test_2;
  // выделим память на 100 часов
  x = new double[100*60+100*100]; // начальные данные
  y = new double[100*60+100*100]; // начальные данные
// output= new double[Hous*60+Hous*100];
  rezult = new double[100];
  output= new double[100*60+100*100];
  // обнуление данных
  for (i=0; i<100*60+100*100; i++)
    {
      x[i]=0.0;
      y[i]=0.0;
    }
  int kol_mas=0;
  delta_prognoz=0;
  for (Hous=atoi(GO[4][2].c_str()); Hous<=atoi(GO[4][3].c_str()); Hous=Hous+atoi(GO[4][4].c_str()))
    {
      vrem_kol+=Renco_kol[RENKO]-Renco_kol[RENKO-Hous*60];
      kol_iter++;
      kol_mas++;
      for (i=0; i<=Hous*60; i++)
        {
          x[i]=i;
          y[i]=(Renco_out[RENKO-Hous*60+i]+Renco_out[RENKO-Hous*60+i])/2.0;
        }
      //cout<<"Старт интерполятора"<<endl;
      interp(x, y, rezult, Hous*60, stepen);
      // cout<<"OK"<<endl;
      //return (9);
      // cout<<"Старт Фурье"<<endl;
      fourier_V1(y, output, 60*Hous, 30*Hous);
      //cout<<"OK"<<endl;
      //return (00);
      // Считаем базовое значение апроксимации
      old_line=0.0;
      for(i=0; i<=stepen; i++)
        {
          old_line+=rezult[i]*pow(60*Hous,i);  // текущая точка апроксимации
        }
      j=60*Hous+Hous*10;  // прогнозная точка
      prognoz=0.0;
      for(i=0; i<=stepen; i++)
        {
          prognoz+=rezult[i]*pow(j,i); // прогноз по 2 порядку
        }
      test_2=output[j];  // прогноз по фурье
      // коррекционный прогноз по фурье
      test_2+=(prognoz-old_line)*0.6; // коррекционный прогноз по фурье
      delta_prognoz+=(test_2-Close[kol_blokov-1])*(atof(GO[kol_mas+100][2].c_str())+1.0);
      test_2=test_2+(atof(GO[kol_mas+100][1].c_str())*(test_2-Close[kol_blokov-1]));  // корректировка
      //fprognoz->rezult_mass[fprognoz->kol_mas]=test_2;   // цель прогноза и корректировка
      //fprognoz->rezult_time_mass[fprognoz->kol_mas]=Renco_time[RENKO-Hous*60]*60;  // массив начала счета
    }
  delete [] x;
  delete [] y;
  delete [] rezult;
  delete [] output;
  delta_prognoz/=kol_iter;
  //fprognoz->time=Time[kol_blokov-1]*60;  // последнее время
  //fprognoz->rezult=progn/2;
  //fprognoz->End_chen=Close[kol_blokov-1];
  return (delta_prognoz);
//
}
