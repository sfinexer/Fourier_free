//+------------------------------------------------------------------+
//|                                                         test.mq5 |
//|                        Copyright 2015, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2015, MetaQuotes Software Corp."
#property link      "https://www.mql5.com"
#property version   "1.00"
extern int  time=5;  // шаг в минутах выставления ордеров
extern double delta=0.0016;  // дельта между прогнозом и ценой
#import "forex.dll" 
double prognoz_chen_mql();
#import 
long tim;
int rates_total_old;
double progn_1=0.0, progn=0.0;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---
   tim=0;
   rates_total_old=0;
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick()
  {

   if(TimeCurrent()/(60*time)!=tim)
     {
      tim=TimeCurrent()/(60*time);
      int handle;
      long t,t_sect;
      string file;
      int size;
      long pozition;
      int section;
      int nehvat_section;
      int nehvat_bar;  // количество недостающих баров
      int i,zapis;
      file="data.db";
      MqlRates rates[];
      if(Period()==PERIOD_M1)
        {
         if(FileIsExist(file))
           {
            handle=FileOpen(file,FILE_READ|FILE_WRITE|FILE_BIN);
            size=FileSize(handle); // РАЗМЕР ФАЙЛА В БАЙТАХ!!!
                                   // узнаем количество ПОЛНЫХ секций
            section=(size)/(8*100*20+1*8);  // в секции 20 параметров по 8 байт всего 100 секций и 1 параметр время секции
                                            // Позиционируемся в конец последней секции
            pozition=section*(8*100*20+1*8);
            FileSeek(handle,pozition,SEEK_SET);  // позиция времени последней известной секции
                                                 //Print("позиция времени последней известной секции= ",pozition);
            t_sect=FileReadLong(handle);  // время последней известной секции
                                          // Print("Время последней известной секции= ",t_sect);
            t=TimeCurrent()/60;  // время в минутах!!!
                                 // вычисляем количество недостающих секций
            nehvat_section=(t-t_sect+1)/100; // Сколько секций нехватает......
            nehvat_bar=t-t_sect+1;   // количество недостающих баров
            Print("Всего баров= ",section*100+nehvat_bar,"     Недостает баров= ",nehvat_bar);
            //Print("t= ",t);
            // return;
            int copied=CopyRates(NULL,0,1,nehvat_bar,rates);  // скопируем недостающие бары
                                                              //  Print("rates[0].time= ",R_T);
            if(copied<=0)
              {
               Print("Ошибка копирования ценовых данных ",GetLastError());
              }
            zapis=0;
            for(i=0; i<copied; i++) // дописываем недостающие секции
              {
               if(rates[i].time/60>=t_sect)
                 {
                  // Print("111111 ");
                  zapis++;
                  FileWriteDouble(handle,rates[i].open);
                  FileWriteDouble(handle,rates[i].high);
                  FileWriteDouble(handle,rates[i].low);
                  FileWriteDouble(handle,rates[i].close);
                  FileWriteDouble(handle,0);
                  FileWriteDouble(handle,0);
                  FileWriteDouble(handle,0);
                  FileWriteDouble(handle,0);
                  FileWriteDouble(handle,0);
                  FileWriteDouble(handle,0);
                  // тип интежер
                  FileWriteLong(handle,rates[i].time/60);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);
                  FileWriteLong(handle,0);

                 }
               if(zapis==100)
                 {
                  FileWriteLong(handle,rates[i].time/60+1);  // записали время секции
                  zapis=0;
                  Print("!!!!!!!!!!!!!");
                  //return;
                 }
              }
            Print("copied=  ",copied,"   zapis=  ",zapis);
            FileClose(handle);
            progn_1=progn;
            progn=prognoz_chen_mql();
            Print("Результат прогноза = ",progn);
            if((progn_1<-delta) && (progn<-delta)) // ставка на сел
              {
               Print("   Ставка на сел ", progn_1,"  ", progn);
               OrderSend(Symbol(),OP_SELL,0.01,Bid,3,Bid+0.0015,Bid-0.0010,"My order",0001,0,clrGreen);
              }
            if((progn_1>delta) && (progn>delta)) // ставка на бай
              {
               Print("   Ставка на бай", progn_1,"  ", progn);
               OrderSend(Symbol(),OP_BUY,0.01,Ask,3,Ask-0.0015,Ask+0.0010,"My order",0001,0,clrGreen);
              }
           }
         else
           {
            PrintFormat("Файл %s отсутствует!",file);
            handle=FileOpen(file,FILE_READ|FILE_WRITE|FILE_BIN);
            FileSeek(handle,0,SEEK_SET);
            // далее пишим временную метку
            t=TimeCurrent()/60-20000;  // время в минутах!!!
            FileWriteLong(handle,t);
            // так как новых ЗАВЕРШОННЫХ баров нет - закрываем файл
            FileClose(handle);
           }
        }
      else
        {
         Print("Перейдите на МИНУТНЫЙ таймфрейм");
        }
     }
  }
//+------------------------------------------------------------------+
