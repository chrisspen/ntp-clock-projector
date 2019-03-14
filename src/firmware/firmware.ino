/*
  NTP-TZ-DST
  NetWork Time Protocol - Time Zone - Daylight Saving Time

  This example shows how to read and set time,
  and how to use NTP (set NTP0_OR_LOCAL1 to 0 below)
  or an external RTC (set NTP0_OR_LOCAL1 to 1 below)

  TZ and DST below have to be manually set
  according to your local settings.

  This example code is in the public domain.
*/

#include <ESP8266WiFi.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

#include "EEPROMAnything.h"

#include "credentials.h"

////////////////////////////////////////////////////////

#define CLOCK_UPDATE_T 500

// Persistent variables.
struct config_t
{
    char ssid[255];
    char ssidpwd[225];
    
} configuration;

unsigned long last_clock_update = 0;

#define COMMAND_SET_LOGIN 'l'
#define COMMAND_GET_LOGIN 'k'
#define COMMAND_GET_TIME 't'
#define COMMAND_END '\n'
#define COMMAND_SEP ':'

#define TZ              -5 // -5=EST      // (utc+) TZ in hours
#define DST_MN          60      // use 60mn for summer time in some countries

#define NTP0_OR_LOCAL1  0       // 0:use NTP  1:fake external RTC
#define RTC_TEST        1510592825 // 1510592825 = Monday 13 November 2017 17:07:05 UTC

////////////////////////////////////////////////////////

#define TZ_MN           ((TZ)*60)
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)

#define DISCONNECTED_T 100
#define CONNECTED_T 1000

unsigned long last_blink_time = 0;

timeval cbtime;         // time set in callback
bool cbtime_set = false;

Adafruit_7segment matrix = Adafruit_7segment();

unsigned long last_colon_blink = 0;
bool colon = true;

//void write_time(int hour, int minute, bool colon=true){
void write_time(const tm* tm){
//    if(millis() - last_colon_blink >= 1000){
//      last_colon_blink = millis();
//      colon = !colon;
//    }

    if (WiFi.status() != WL_CONNECTED){
        if (millis() - last_blink_time >= DISCONNECTED_T){
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            colon = !colon;
            last_blink_time = millis();
        }
        matrix.writeDigitNumMirrored(0, 8, false);
        matrix.writeDigitNumMirrored(1, 8, false);
        matrix.drawColon(colon);
        matrix.writeDigitNumMirrored(3, 8, false);
        matrix.writeDigitNumMirrored(4, 8, false);
    }else{
        if (millis() - last_blink_time >= CONNECTED_T){
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            colon = !colon;
            last_blink_time = millis();
        }
        int minute1 = tm->tm_min/1%10;
        int minute2 = tm->tm_min/10%10;
        int hour = tm->tm_hour;
        //if(!tm->tm_isdst){
        //    hour -= 1; // Unapply DST hour?
        //}
        hour = hour % 12;
        if(hour == 0 || hour == 12 || hour == 24){
          hour = 12;
        }
        int hour1 = hour/1%10;
        int hour2 = hour/10%10;
        matrix.writeDigitNumMirrored(0, minute1, false); // minute, low digit
        matrix.writeDigitNumMirrored(1, minute2, false); // minute, high digit
        matrix.drawColon(colon);
        matrix.writeDigitNumMirrored(3, hour1, false); // hour, low digit
        if(hour2){
          matrix.writeDigitNumMirrored(4, hour2, false); // hour, high digit
        }else{
          matrix.turnOffDigit(4);
        }
    }
    matrix.writeDisplay();
}

void time_is_set(void) {
  gettimeofday(&cbtime, NULL);
  cbtime_set = true;
  //Serial.println("------------------ settimeofday() was called ------------------");
}

//void blink_wifi_status(){
//  
//    if (WiFi.status() != WL_CONNECTED){
//        if (millis() - last_blink_time >= DISCONNECTED_T){
//            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//            last_blink_time = millis();
//        }
//    }else{
//        if (millis() - last_blink_time >= CONNECTED_T){
//            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//            last_blink_time = millis();
//        }
//    }
//}

void save_configuration(){
    EEPROM_writeAnything(0, configuration);
}

void load_configuration(){
    EEPROM_readAnything(0, configuration);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {

    load_configuration();
    
    // https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.cpp
    // On the esp8266, you can manually specify the i2c pins.
    Wire.begin(); // SDA=A5=D2, SCL=A4=D1
    
    last_blink_time = 0;

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    matrix.begin(0x70);
    
    Serial.begin(115200);
    settimeofday_cb(time_is_set);
  
    configTime(TZ_SEC, DST_SEC, "pool.ntp.org");
    WiFi.mode(WIFI_STA);
    //WiFi.begin(SSID, SSIDPWD);
    WiFi.begin(configuration.ssid, configuration.ssidpwd);
    // don't wait, observe time changing when ntp timestamp is received

}

// for testing purpose:
extern "C" int clock_gettime(clockid_t unused, struct timespec *tp);

#define PTM(w) \
  Serial.print(":" #w "="); \
  Serial.print(tm->tm_##w);

void printTm(const char* what, const tm* tm) {
  Serial.print(what);
  PTM(isdst); PTM(yday); PTM(wday);
  PTM(year);  PTM(mon);  PTM(mday);
  PTM(hour);  PTM(min);  PTM(sec);
}

timeval tv;
timespec tp;
time_t now;
uint32_t now_ms, now_us;

String read_buffer = "";
char read_byte;
int bytes_read = 0;
unsigned long last_buffer_read = 0;
char current_command;
bool current_command_done = false;

void print_serial_login(){
  Serial.println(String("SSID=")+String(configuration.ssid)+String(":SSIDPWD=")+String(configuration.ssidpwd));
  Serial.flush();
}

void print_serial_time(){
  printTm("LOCALTIME=", localtime(&now));
  Serial.println();
  Serial.flush();
}

void loop() {
  //blink_wifi_status();
  if(millis() - last_clock_update >= CLOCK_UPDATE_T){
    last_clock_update = millis();
    
    gettimeofday(&tv, nullptr);
    clock_gettime(0, &tp);
    now = time(nullptr);
    now_ms = millis();
    now_us = micros();
  
    /*/ localtime / gmtime every second change
    static time_t lastv = 0;
    if (lastv != tv.tv_sec) {
      lastv = tv.tv_sec;
      Serial.println();
      printTm("localtime", localtime(&now));
      Serial.println();
      printTm("gmtime   ", gmtime(&now));
      Serial.println();
      Serial.println();
    }
    */

  /*
    // time from boot
    Serial.print("clock:");
    Serial.print((uint32_t)tp.tv_sec);
    Serial.print("/");
    Serial.print((uint32_t)tp.tv_nsec);
    Serial.print("ns");
  
    // time from boot
    Serial.print(" millis:");
    Serial.print(now_ms);
    Serial.print(" micros:");
    Serial.print(now_us);
  
    // EPOCH+tz+dst
    Serial.print(" gtod:");
    Serial.print((uint32_t)tv.tv_sec);
    Serial.print("/");
    Serial.print((uint32_t)tv.tv_usec);
    Serial.print("us");
  
    // EPOCH+tz+dst
    Serial.print(" time:");
    Serial.print((uint32_t)now);
  
    // human readable
    Serial.print(" ctime:(UTC+");
    Serial.print((uint32_t)(TZ * 60 + DST_MN));
    Serial.print("mn)");
    Serial.print(ctime(&now));
  */
    // simple drifting loop
    //delay(100);
      
    //write_time(10, 28, colon);
    write_time(localtime(&now));
    //delay(10);
  
  }

  while (Serial.available()) {
    last_buffer_read = millis();

    // If we don't have a command, then the first byte specifies the command.
    if(bytes_read == 0){
      current_command = Serial.read();
      bytes_read += 1;
      continue;
    }

    // Otherwise, read argument data.
    read_byte = Serial.read();
    bytes_read += 1;

    // Collect arguments.
    if(read_byte == COMMAND_END){
      // Terminator received, so begin executing.
      current_command_done = true;
    }else if (current_command == COMMAND_SET_LOGIN){
      // Read arguments.
      read_buffer += read_byte;
    }else if (current_command == COMMAND_GET_LOGIN){
      // No arguments. Act immediately.
      current_command_done = true;
    }else if (current_command == COMMAND_GET_TIME){
      // No arguments. Act immediately.
      current_command_done = true;
    }else{
      // Unknown command received.
      current_command = '\0';
    }

    // Execute command.
    if(current_command_done){
      current_command_done = false;
      if (current_command == COMMAND_SET_LOGIN){
        strcpy(configuration.ssid, getValue(read_buffer, COMMAND_SEP, 0).c_str());
        strcpy(configuration.ssidpwd, getValue(read_buffer, COMMAND_SEP, 1).c_str());
        print_serial_login();
        save_configuration();
        WiFi.begin(configuration.ssid, configuration.ssidpwd);
      }else if (current_command == COMMAND_GET_LOGIN){
        print_serial_login();
      }else if (current_command == COMMAND_GET_TIME){
        print_serial_time();
      }
      break;
    }

  }

  // If we don't read anything after 5 seconds, then reset read buffer.
  if(millis() - last_buffer_read >= 5000 || current_command_done){
    read_buffer = "";
    bytes_read = 0;
    current_command = '\0';
    current_command_done = false;
  }
  
  
}

