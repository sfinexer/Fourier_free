
#define int extern int
//#define float extern float


#define float extern float
#define double extern double
#undef __int64  // ������������ ���������� ������ ����� �������� ���� �����
#define __int64 extern long long // ��������� ������ ����� ������
#define string extern string

#include "GLOBAL.H"


#undef int
#undef float
#undef double
#undef __int64 // ������������� ������ ����� ������
#undef string
//===========
#define __int64 long long   // ���������� ���������� ������ ������
