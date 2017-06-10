
#define int extern int
//#define float extern float


#define float extern float
#define double extern double
#undef __int64  // приостановим глабольную замену чтобы заменить наши файлы
#define __int64 extern long long // запускаем замену наших файлов
#define string extern string

#include "GLOBAL.H"


#undef int
#undef float
#undef double
#undef __int64 // останавливаем замену наших файлов
#undef string
//===========
#define __int64 long long   // продолжаем глобальную замену файлов
