// ChristmasTimer.ino
// last edit 28.Dec.2016  /klatoo  - created
//
// application that uses the RtcLib to switch the LEDs of a Christmas tree on 
// at a certain time
// 

#include "RtcLibHelper.h"
#include "DS3231.h"
#include "Relais.h"

TimerClock24h timerClock;
Relais relais;


void setup () 
{
     // Initialize INT0 for accepting interrupts, check if pin is available on your board 
     PORTD |= 0x04; 
     DDRD &=~ 0x04;

     for(int i=4; i<20; i++) 
     {
      pinMode(i,OUTPUT); // saves some power
     }
  
     Serial.begin(57600);
        
     rtc.begin();
     attachInterrupt(digitalPinToInterrupt(2), timerISR, FALLING);   // check if pin is available on your board 
     
     // Enable periodic interrupt (possible values are: DS3231::Periodicity::EverySecond, EveryMinute, EveryHour)
     rtc.enableInterrupts(DS3231::Periodicity::EveryMinute); 
	 
	 timerClock.setup(17,45,0,23,58,0); // switching on at 17:45:00 and off at 23:58:00
	 relais.setup(3);
}


void loop () 
{
  DateTime now = rtc.now();   // get the current date-time

  
	Serial.print(now.day(), DEC);
	Serial.print('.');
	Serial.print(now.month(), DEC);
	Serial.print('.');
	Serial.print(now.year(), DEC);
		
	Serial.print("    ");
	
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.println(now.second(), DEC);
			
	Serial.print("unix-time: "); 
	Serial.println(now.unixtime());
  Serial.println();
  
 
	if(timerClock.IsOn(now))
	{
	   relais.Off(); // On(); // changed for MOSFET
	}
    else
	{
	   relais.On(); // Off(); // changede for MOSFET
	}

  

   rtc.clearINTStatus();
   delay(10); // wait for serial output to finish
    
   // put arduino to sleep
   SMCR |= (1<<2); // power down mode
   SMCR |= 1; // enable sleep
   __asm__ __volatile__("sleep"); 

 //  Serial.println("in loop()");
}


//Interrupt service routine for RTC interrupt
void timerISR()
{
  // don't do anything currently ;)
  // all the work is done in the main loop
}
