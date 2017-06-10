#include <all>
#include "gl.h"   // описание глобальных переменных (перед нашим конфигом)
extern "C" __declspec(dllexport) void ini();
fstream fs;
int load(string file)
{
  ini();
  int i, j;
  int chi;
  int kol_grup;  // количество групп блоков данных
  __int64 kol1, temp;
  fs.open(file, ios :: in |  ios::binary );
  struct stat fi;
  stat(file.c_str(),&fi);
  sizefile=fi.st_size;
  // позиция в файле
  if ((smesh>0)&&(smesh<sizefile/16008))
    {
      kol_grup=(sizefile)/16008;  // Количество целых групп
      fs.seekg(kol_grup*16008-16008*smesh, ios_base::beg);  // отнимаем от последних целых групп
      cout<<"Точка начала чтения базы=  "<<sizefile-16008*smesh<<"   байт"<<endl;
      sizefile-=kol_grup*16008-16008*smesh;
    }
  // КОНЕЦ позиция в файле
  kol_grup=(sizefile)/16008;  // группы блоков с данными
  kol_blokov=(sizefile-kol_grup*8-8)/160;  // блоки с данными
  fs.read((char *)&kol1, 8);
  //cout<<"Statr data= "<<kol<<endl;
  double *mass, *vrem_mass;
  __int64 *mass__int64, *vrem_mass__int64;
  mass = new double [sizefile/8];
  vrem_mass=mass;
  mass__int64 = new __int64 [sizefile/8];
  vrem_mass__int64=mass__int64;
  for (i=0; i<=kol_grup; i++) // цикл по количеству целых групп
    {
      if (kol_grup==i) // последний блок
        {
          chi=kol_blokov-kol_grup*100;
          for (j=1; j<=chi; j++) // цикл по количеству целых групп
            {
              fs.read((char *)&*vrem_mass, 10*8);
              fs.read((char *)&*vrem_mass__int64, 10*8);
              vrem_mass+=10;
              vrem_mass__int64+=10;
            }
        }
      else
        {
          for (j=1; j<=100; j++) // цикл по количеству целых групп
            {
              fs.read((char *)&*vrem_mass, 10*8);
              fs.read((char *)&*vrem_mass__int64, 10*8);
              vrem_mass+=10;
              vrem_mass__int64+=10;
            }
          fs.read((char *)&temp, 8);
        }
    }
  fs.close();
  // загрузка данных
  // распихиваем данные по группам
  int er=0;
  for (i=0; i<kol_blokov; i++) // цикл по количеству целых групп
    {
      Open[i]=mass[i*10+0];
      High[i]=mass[i*10+1];
      Low[i]=mass[i*10+2];
      Close[i]=mass[i*10+3];
      Time[i]=mass__int64[i*10+0];
    }
  // конец массива данных
  delete [] mass;
  delete [] mass__int64;
  return kol_blokov;
}
