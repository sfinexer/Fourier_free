#include <all>
#include "gl.h"   // �������� ���������� ���������� (����� ����� ��������)
void GOU();
extern "C" __declspec(dllexport) void ini();
string replaser(string str1, string str2, string str)  // ������ ������� ����, �� ������� ������, ��������������
{
  int pos;
begin:
  pos=-1;
  pos = str.find(str1, 0);
  if (pos>=0)
    {
      str.replace( pos, str1.size(), str2 );
      goto begin;
    }
  return str;
}
void GOU_HELP()  // ������ ����� ���, � ������ ����� �����
{
  // ������ ���� ����� ����
  //cout<<(put_prog+"\\###go").c_str()<<endl;
  ofstream file((put_prog+"###go").c_str(), ios::trunc); // ������ ����� ���� ��� ����
#include "GO.h"
  file.close();  // �������� �����
}
void GOU()              // ������ ���� ������������
{
  int str=0,cou=0,slovo=1,flag=0,exitt=0;
  int num=0;
  int r;
  int i,j;
  r=0;
  string fpd;  // ���� ������� �������
  string repl[400][10];  // ����� ���� ����� ���
  string in,fil;
begin:
  ifstream fgo((put_prog+"###go").c_str());
  if ( ! fgo)
    {
      //cerr<<"###GO   ERROR FILE INPUT. \n";
      GOU_HELP();
      system("pause");
      return;
    }
  while (getline(fgo, in))
    {
      ++str;
      cou=0;
      slovo=1;
      exitt=0;
      fil="";
      flag=0;
      while ( in[cou] && (exitt==0))
        {
          // ������ ��������
          if ((in[cou]) != 32)
            {
              fil=fil+in[cou];
              flag=1;
            }
          else
            {
              if (flag==1)
                {
                  exitt=1;
                }
            }
          ++cou;
        }
      num=atoi(fil.c_str());
      flag=0;
      if (num>0)
        {
          while ( in[cou])
            {
              if ((in[cou]) != 32)
                {
                  GO[num][slovo]=GO[num][slovo]+in[cou];
                  flag=1;
                }
              else
                {
                  if (flag==1)
                    {
                      ++slovo;
                      if (slovo>10)
                        {
                          goto ex;
                        }
                    }
                  flag=0;
                }
              ++cou;
            }
ex:
          {
          }
        }
      else
        {
          if (fil[0]==37)
            {
              r++;
              repl[r][1]=fil;
              flag=0;
              exitt=0;
              while ( in[cou]&& (exitt==0))
                {
                  if ((in[cou]) != 32)
                    {
                      repl[r][2]=repl[r][2]+in[cou];
                      flag=1;
                    }
                  else
                    {
                      if (flag==1)
                        {
                          exitt=1;
                        }
                    }
                  ++cou;
                }
            }
        }
    }
  // ��� ��� ��������
  for( i=1; i<=r; i++)
    {
      for (j=1; j<399; j++)
        {
          if (GO[j][1][0]>0)
            {
              GO[j][1]=replaser(repl[i][1].c_str(), repl[i][2].c_str(),  GO[j][1].c_str());
            }
          if (GO[j][2][0]>0)
            {
              GO[j][2]=replaser(repl[i][2].c_str(), repl[i][2].c_str(),  GO[j][2].c_str());
            }
        }
    }
  if (GO[1][1][0]==37)
    {
      system("pause");
      goto begin;
    }
  fgo.close ();
}

