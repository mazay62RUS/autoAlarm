#include <LiquidCrystal_I2C.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>

 // - 5 s 1.5 s 5 
 // 9 1
 // - 5 s 5

LiquidCrystal_I2C disp(0x27,16,2);

const byte RELAY_PIN = 5;

bool isShorten  = false;
bool isAuto     = false;

void setup()
{
  //disp.init();
  disp.begin();
  disp.backlight();
  setSyncProvider(RTC.get);
  //Часы, минуты, секунды, день, месяц, год
  //setTime(22,11,16,5,11,2018);
  RTC.set(now());
  pinMode(RELAY_PIN, OUTPUT);
  currMode();
}

void loop() {
  if ((hour() == 20) && minute() == 0 && (second() == 0)) { disp.noBacklight(); }
  if ((hour() == 7) && minute() == 0 && (second() == 0)) { disp.backlight(); }
  if ( (hour() == 0) && (minute() == 0) && (second() == 0) && isAuto) { isAuto = true; }
  if ( (hour() == 0) && (minute() == 0) && (second() == 0) && !isAuto){ isShorten = false; isAuto = false; }
  if ( second() == 0 ) { disp.clear(); }
  tmElements_t tm;
  if (RTC.read(tm)) {
    modeSettings(checkButton());
    if ( !isAuto ) {
      mode(weekday());
    }
    
    if ( isAuto )
    {
      disp.setCursor(0,0);
      disp.print("Auto");
    }
    
    if ( weekday() == 1 )
    {
      isAuto = true;
      disp.setCursor(0,0);
      disp.print("Auto");
    }
    
    Serial.println();
    currMode();
    disp.setCursor(10,0);
    disp.print(weekDay(weekday()));
    disp.setCursor(14,0);
    disp.print(day());
    disp.setCursor(0,1);
    disp.print(month());
    disp.print("/");
    disp.print(year());
    disp.setCursor(8,1);
    disp.print(hour());
    disp.print(":");
    disp.print(minute());
    disp.print(":");
    disp.print(second());
  }
  else
  {
    disp.print("time error");
  }
  delay(300);
}

bool isSaturday(int weekDay)
{
  if (weekDay == 7) { return true; }
  else { return false; }
}

void mode(int weekDay)
{  
   if (isShorten && isSaturday(weekDay))
   {
      shortSaturday(); 
   }
   if (!isShorten && isSaturday(weekDay))
   {
      typicalSaturday();
   }
   if (isShorten && !isSaturday(weekDay))
   {
      shortDay(); 
   }
   if (!isShorten && !isSaturday(weekDay))
   {
      typicalDay(); 
   }
}

void currMode()
{
  if (isShorten  && !isAuto) { disp.setCursor(0,0); disp.print("Short");   }
  if (!isShorten && !isAuto) { disp.setCursor(0,0); disp.print("Typical"); } 
  //else                { disp.setCursor(0,0); disp.print("Auto");    }
}

void modeSettings(int btn)
{
    if( btn == 4 )
    {
      alarm();
    }
    if( btn == 2 )
    {
      isShorten = false;
      isAuto    = false;
      disp.clear();
      disp.setCursor(0,0);
      disp.print("Typical");
    }
    if( btn == 3 )
    {
      isShorten = true;
      isAuto    = false;
      disp.clear();
      disp.setCursor(0,0);
      disp.print("Short");
    }
    if( btn == 1 )
    {
      isAuto = true;
      disp.clear();
      disp.setCursor(0,0);
      disp.print("Auto");
    }
}

String weekDay(int numOfWeekDay)
{
  if (numOfWeekDay == 2) { return "Mon"; }
  if (numOfWeekDay == 3) { return "Tue"; }
  if (numOfWeekDay == 4) { return "Wed"; }
  if (numOfWeekDay == 5) { return "Thu"; }
  if (numOfWeekDay == 6) { return "Fri"; }
  if (numOfWeekDay == 7) { return "Sat"; }
  if (numOfWeekDay == 1) { return "Sun"; }
}

void alarm()
{
  disp.clear();
  digitalWrite(RELAY_PIN, HIGH);
  disp.setCursor(5, 0);
  disp.print("ALARM!");
  delay(4500);
  disp.clear();
  digitalWrite(RELAY_PIN, LOW);
}

void typicalDay()
{
  if (  (((hour() == 8)  && (minute() == 0 && second() == 0  )) 
      || ((hour() == 8)  && (minute() == 55 && second() == 0 )) 
      || ((hour() == 9)  && (minute() == 0 && second() == 0  ))    
      || ((hour() == 9)  && (minute() == 45 && second() == 0 )) 
      || ((hour() == 9)  && (minute() == 50 && second() == 0 )) 
      || ((hour() == 10) && (minute() == 35 && second() == 0 ))
      || ((hour() == 10) && (minute() == 50 && second() == 0 ))
      || ((hour() == 11) && (minute() == 35 && second() == 0 ))
      || ((hour() == 11) && (minute() == 40 && second() == 0 ))
      || ((hour() == 12) && (minute() == 25 && second() == 0 ))
      || ((hour() == 12) && (minute() == 45 && second() == 0 ))
      || ((hour() == 13) && (minute() == 30 && second() == 0 ))
      || ((hour() == 13) && (minute() == 35 && second() == 0 ))
      || ((hour() == 14) && (minute() == 20 && second() == 0 ))
      || ((hour() == 14) && (minute() == 30 && second() == 0 ))
      || ((hour() == 15) && (minute() == 15 && second() == 0 ))
      || ((hour() == 15) && (minute() == 20 && second() == 0 ))
      || ((hour() == 16) && (minute() == 5 &&  second() == 0 ))
      || ((hour() == 16) && (minute() == 15 && second() == 0 ))
      || ((hour() == 17) && (minute() == 0 &&  second() == 0 ))
      || ((hour() == 17) && (minute() == 5 &&  second() == 0 ))
      || ((hour() == 17) && (minute() == 50 &&  second() == 0 )))
      //|| ((hour() == 18) && (minute() == 10 &&  second() == 0 ))
      //|| ((hour() == 18) && (minute() == 11 &&  second() == 0 ))
      //|| ((hour() == 18) && (minute() == 12 &&  second() == 0 ))
      //|| ((hour() == 18) && (minute() == 13 &&  second() == 0 )))
      || ((hour() == 17) && (minute() == 50 && second() == 0 )))

     ) { alarm(); } 
}

void shortDay()
{
  if (   (((hour() == 8)  && (minute() == 0 && second() == 0 )) 
      || ((hour() == 8)  && (minute() == 55 && second() == 0 )) 
      || ((hour() == 9)  && (minute() == 0 &&  second() == 0 ))    
      || ((hour() == 10) && (minute() == 0 &&  second() == 0 )) 
      || ((hour() == 10) && (minute() == 10 && second() == 0 )) 
      || ((hour() == 11) && (minute() == 10 && second() == 0 ))
      || ((hour() == 11) && (minute() == 20 && second() == 0 ))
      || ((hour() == 12) && (minute() == 20 && second() == 0 ))
      || ((hour() == 12) && (minute() == 30 && second() == 0 ))
      || ((hour() == 13) && (minute() == 30 && second() == 0 ))
      || ((hour() == 13) && (minute() == 40 && second() == 0 ))
      || ((hour() == 14) && (minute() == 40 && second() == 0 )))

     ) { alarm(); }  
}

void typicalSaturday()
{
  if (   (((hour() == 8)  && (minute() == 0 && second() == 0 )) 
      || ((hour() == 8)  && (minute() == 45 && second() == 0 )) 
      || ((hour() == 8)  && (minute() == 50 && second() == 0 ))    
      || ((hour() == 9)  && (minute() == 35 && second() == 0 )) 
      || ((hour() == 9)  && (minute() == 45 && second() == 0 )) 
      || ((hour() == 10) && (minute() == 30 && second() == 0 ))
      || ((hour() == 10) && (minute() == 35 && second() == 0 ))
      || ((hour() == 11) && (minute() == 20 && second() == 0 ))
      || ((hour() == 11) && (minute() == 30 && second() == 0 ))
      || ((hour() == 12) && (minute() == 15 && second() == 0 ))
      || ((hour() == 12) && (minute() == 20 && second() == 0 ))
      || ((hour() == 13) && (minute() == 5 &&  second() == 0 ))
      || ((hour() == 13) && (minute() == 15 && second() == 0 ))
      || ((hour() == 14) && (minute() == 0 &&  second() == 0 ))
      || ((hour() == 14) && (minute() == 5 &&  second() == 0 ))
      || ((hour() == 14) && (minute() == 50 && second() == 0 ))
      || ((hour() == 15) && (minute() == 0 &&  second() == 0 ))
      || ((hour() == 16) && (minute() == 0 &&  second() == 0 )))

     ) { alarm(); } 
}

void shortSaturday()
{
  if (   ((hour() == 8)  && (minute() == 0 &&  second() == 0 )) 
      || ((hour() == 9)  && (minute() == 0 &&  second() == 0 )) 
      || ((hour() == 9)  && (minute() == 10 && second() == 0 ))    
      || ((hour() == 10) && (minute() == 10 && second() == 0 )) 
      || ((hour() == 10) && (minute() == 20 && second() == 0 )) 
      || ((hour() == 11) && (minute() == 20 && second() == 0 ))
      || ((hour() == 11) && (minute() == 30 && second() == 0 ))
      || ((hour() == 12) && (minute() == 30 && second() == 0 ))
      || ((hour() == 12) && (minute() == 40 && second() == 0 ))
      || ((hour() == 13) && (minute() == 40 && second() == 0 ))

     ) { alarm(); }   
}

int checkButton()
{
  int in = analogRead(A0);
  
    if(in >= -10 && in <= 10)
    {
        return 1;
    } 
    else if(in >= 70 && in <= 115)
    {
        return 2;
    } 
    else if(in >= 210 && in <= 245)
    {
        return 3;
    } 
    else if(in >= 320 && in <= 375)
    {
       return 4;
    } 
    else if(in >= 480 && in <= 520)
    {
       return 5; 
    }
  
  delay(100);  
}
