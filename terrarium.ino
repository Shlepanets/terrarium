//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Relays' ports
#define RELAY1 3
#define RELAY2 4
#define RELAY3 5

// Relays' state inverse

#define ON LOW
#define OFF HIGH

// On and off time
#define ON_HOUR 8
#define OFF_HOUR 19

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

RTC_DS1307 rtc;

float temp1 = 0;
float temp2 = 0;
int nowHour = 0;
int nowMinute = 0;
char relay1state = '-';
char relay2state = '-';
char relay3state = '-';

DateTime now;

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

  // Start up the library
  sensors.begin();
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp1 = sensors.getTempCByIndex(0);
  temp2 = sensors.getTempCByIndex(1);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // line just for time setup
  now = rtc.now();
  
  nowHour = now.hour();
  nowMinute = now.minute();

  // Setting up relays default off
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  digitalWrite(RELAY1, OFF);
  digitalWrite(RELAY2, OFF);
  digitalWrite(RELAY3, OFF);

  printLcd(); 
}

void printLcd()
{  
  lcd.clear();
  lcd.print("T1:");
  lcd.print(temp1);
  lcd.setCursor(11, 0);
  if ((nowHour >= 0) && (nowHour <= 9)) lcd.print('0');
  lcd.print(nowHour);
  lcd.print(':');
  if ((nowMinute >= 0) && (nowMinute <= 9)) lcd.print('0');
  lcd.print(nowMinute);
  
  lcd.setCursor(0, 1);
  lcd.print("T2:");
  lcd.print(temp2);
  lcd.setCursor(12, 1);  
  lcd.print(relay1state);
  lcd.print(relay2state);
  lcd.print(relay3state);
}


void loop()
{
 sensors.requestTemperatures(); // Send the command to get temperatures
 now = rtc.now(); 
 if ((temp1 != (sensors.getTempCByIndex(0))) || (temp2 != (sensors.getTempCByIndex(1))) || (nowMinute != (now.minute())) )
 {
  temp1 = sensors.getTempCByIndex(0);
  temp2 = sensors.getTempCByIndex(1);
  nowHour = now.hour();
  nowMinute = now.minute();

  printLcd(); 
 }
 
 // RELAY1 ON & OFF
 if ( (digitalRead(RELAY1) == OFF) && (nowHour >= ON_HOUR) && (nowHour < OFF_HOUR) ) 
 {
  digitalWrite(RELAY1, ON);
  relay1state = '+';   
 }
 
 if ( (digitalRead(RELAY1) == ON) && ((nowHour >= OFF_HOUR) || (nowHour < ON_HOUR)) ) 
 {
  digitalWrite(RELAY1, OFF);
  relay1state = '-'; 
 }
 
 // RELAY2 ON & OFF 
 if ( (digitalRead(RELAY2) == OFF) && (nowHour >= ON_HOUR) && (nowHour < OFF_HOUR) ) 
 {
  digitalWrite(RELAY2, ON);
  relay2state = '+'; 
 }
 
 if ( (digitalRead(RELAY2) == ON) && ((nowHour >= OFF_HOUR) || (nowHour < ON_HOUR)) ) 
 {
  digitalWrite(RELAY2, OFF); 
  relay2state = '-';
 }

 // RELAY3 ON & OFF 
 if ( (digitalRead(RELAY3) == OFF) && (nowHour >= ON_HOUR) && (nowHour < OFF_HOUR) ) 
 {
  digitalWrite(RELAY3, ON);
  relay3state = '+'; 
 }
 
 if ( (digitalRead(RELAY3) == ON) && ((nowHour >= OFF_HOUR) || (nowHour < ON_HOUR)) ) 
 {
  digitalWrite(RELAY3, OFF); 
  relay3state = '-';
 }
 

 delay(250);
  
}
