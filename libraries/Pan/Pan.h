#pragma once
#include <Arduino.h>

// How often the station should fetch data, in milliseconds
#ifndef FREQUENCY
  #define FREQUENCY 300000
#endif
// Station ID : model name, model type, revision, serial number, frequency
#ifndef STATION_ID
  #define STATION_ID "PAN--A-1-300"
#endif

#ifdef DEBUG
 #define DEBUG_OUTPUT(x) Serial.println(x)
#else
 #define DEBUG_OUTPUT(x)
#endif

#ifdef USE_SD
  #include <SPI.h>
  #include <SD.h>
  
  #ifndef SD_CS_PIN
    #define SD_CS_PIN 4
  #endif
#endif

#ifdef USE_RTC
  #include <Rtc_Pcf8563.h>
#endif

#ifdef USE_JUMPER
  #include <SerialCommand.h>
  #ifndef JUMPER_OUT_PIN
    #define JUMPER_OUT_PIN 3
  #endif
  #ifndef JUMPER_IN_PIN
    #define JUMPER_IN_PIN 2
  #endif
#endif

#ifndef LED_PIN
  #define LED_PIN 9
#endif

class Pan
{
    public:
        Pan();
        virtual void begin();
        virtual void loop();
        virtual String buildOutput();
    protected:
        String output;
        void blink(byte);
        #ifdef USE_SD
            File dataFile;
            bool beginSD();
            void openFile(uint8_t);
        #endif
        #ifdef USE_RTC
            Rtc_Pcf8563 rtc;
            void openRTC();
            void checkRTC();
        #endif
        #ifdef USE_JUMPER
            SerialCommand sCmd;
            bool hasJumper;
            void ping();
            void sendID();
            void get();
            void unknownCommand();
            #ifdef USE_SD
                void harvest();
                void clear();
            #endif
            #ifdef USE_RTC
                void getTimestamp();
                void setDate();
                void setTime();
            #endif
        #endif
};
