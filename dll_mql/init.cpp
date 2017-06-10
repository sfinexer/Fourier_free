#include <all>
#include "GLOBAL.H"  // в мейне описание глобальных переменных ПОСЛЕ КОНФИГУРАЦИИ
void GOU();
extern "C" __declspec(dllexport) void ini()
{
  if (inite==0)
    {
      // путь до папки где лежит программа!
      kol_blokov=0;
      cout<<"Выделяем память  "<<endl;
      GOU();
      // выделяем память
      Open = new double [1'000'000+1];
      High = new double [1'000'000+1];
      Low = new double [1'000'000+1];
      Close = new double [1'000'000+1];
      Time = new __int64 [1'000'000+1];
      // для прогноза
      Renco_out = new double [1'000'000];
      Renco_time = new __int64 [1'000'000];
      Renco_kol = new int [1'000'000];
      //
      if (atoi(GO[3][1].c_str())>0)
        {
          smesh=atoi(GO[3][1].c_str());
        }
      if (atof(GO[11][1].c_str())>0.0)
        {
          FreqTOL=atof(GO[11][1].c_str());
        }
      else
        {
          FreqTOL=0.000005;
        }
      if (atoi(GO[10][1].c_str())>0)   // количество гармоник для фурье
        {
          fourier_garm=atoi(GO[10][1].c_str());
        }
      else
        {
          fourier_garm=5;
        }
      if (atoi(GO[1][1].c_str())==1)   // шаг по ренко
        {
          shag_renco=atof(GO[2][1].c_str());
        }
      else
        {
          shag_renco=0.00016;
        }
      if (atoi(GO[20][1].c_str())>0)   // количество гармоник для фурье
        {
          time_poias=atoi(GO[20][1].c_str());
        }
      else
        {
          time_poias=0;
        }
      cout<< "          Дельта построения ренко =  "<<shag_renco<<endl;
      inite=1;
    }
}
