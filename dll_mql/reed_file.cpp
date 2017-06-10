#include <all>
#include "gl.h"   // �������� ���������� ���������� (����� ����� ��������)
extern "C" __declspec(dllexport) void ini();
fstream fs;
int load(string file)
{
  ini();
  int i, j;
  int chi;
  int kol_grup;  // ���������� ����� ������ ������
  __int64 kol1, temp;
  fs.open(file, ios :: in |  ios::binary );
  struct stat fi;
  stat(file.c_str(),&fi);
  sizefile=fi.st_size;
  // ������� � �����
  if ((smesh>0)&&(smesh<sizefile/16008))
    {
      kol_grup=(sizefile)/16008;  // ���������� ����� �����
      fs.seekg(kol_grup*16008-16008*smesh, ios_base::beg);  // �������� �� ��������� ����� �����
      cout<<"����� ������ ������ ����=  "<<sizefile-16008*smesh<<"   ����"<<endl;
      sizefile-=kol_grup*16008-16008*smesh;
    }
  // ����� ������� � �����
  kol_grup=(sizefile)/16008;  // ������ ������ � �������
  kol_blokov=(sizefile-kol_grup*8-8)/160;  // ����� � �������
  fs.read((char *)&kol1, 8);
  //cout<<"Statr data= "<<kol<<endl;
  double *mass, *vrem_mass;
  __int64 *mass__int64, *vrem_mass__int64;
  mass = new double [sizefile/8];
  vrem_mass=mass;
  mass__int64 = new __int64 [sizefile/8];
  vrem_mass__int64=mass__int64;
  for (i=0; i<=kol_grup; i++) // ���� �� ���������� ����� �����
    {
      if (kol_grup==i) // ��������� ����
        {
          chi=kol_blokov-kol_grup*100;
          for (j=1; j<=chi; j++) // ���� �� ���������� ����� �����
            {
              fs.read((char *)&*vrem_mass, 10*8);
              fs.read((char *)&*vrem_mass__int64, 10*8);
              vrem_mass+=10;
              vrem_mass__int64+=10;
            }
        }
      else
        {
          for (j=1; j<=100; j++) // ���� �� ���������� ����� �����
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
  // �������� ������
  // ����������� ������ �� �������
  int er=0;
  for (i=0; i<kol_blokov; i++) // ���� �� ���������� ����� �����
    {
      Open[i]=mass[i*10+0];
      High[i]=mass[i*10+1];
      Low[i]=mass[i*10+2];
      Close[i]=mass[i*10+3];
      Time[i]=mass__int64[i*10+0];
    }
  // ����� ������� ������
  delete [] mass;
  delete [] mass__int64;
  return kol_blokov;
}
