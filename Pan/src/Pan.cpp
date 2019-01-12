#include "Pan.h"

#if USE_JUMPER == 1
Pan::Pan() : output(""), hasJumper(false)
#else
Pan::Pan() : output("")
#endif
{
}

void Pan::begin() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);

  #if USE_JUMPER == 1
    pinMode(JUMPER_IN_PIN, INPUT);
    pinMode(JUMPER_OUT_PIN, OUTPUT);
    digitalWrite(JUMPER_OUT_PIN, HIGH);
    delay(100);
    hasJumper = digitalRead(JUMPER_IN_PIN) == HIGH;
    digitalWrite(JUMPER_OUT_PIN, LOW);

    if (hasJumper) {
      sCmd.addCommand(PSTR("PING"), ping);
      sCmd.addCommand(PSTR("ID"), sendID);
      sCmd.addCommand(PSTR("GET"), get);
      #if USE_SD == 1
        sCmd.addCommand(PSTR("HARVEST"), harvest);
        sCmd.addCommand(PSTR("CLEAR"), clear);
      #endif
      #if USE_RTC == 1
        sCmd.addCommand(PSTR("TIMESTAMP"), getTimestamp);
        sCmd.addCommand(PSTR("DATE"), setDate);
        sCmd.addCommand(PSTR("TIME"), setTime);
      #endif
      sCmd.setDefaultHandler(unknownCommand);
      DEBUG_OUTPUT(F("Serial control activated"));
      return;
    }
  #endif

  #if USE_SD == 1
    if (!beginSD()) {
      DEBUG_OUTPUT(F("SD initialisation failed"));
      blink(4);
    } else {
      DEBUG_OUTPUT(F("SD init OK"));
      openFile(FILE_WRITE);
      if (!dataFile) {
        DEBUG_OUTPUT(F("File opening failed"));
        blink(8);
      }
      DEBUG_OUTPUT(F("SD File data.csv opened OK"));
    }
  #endif
  
  #if USE_RTC == 1
    openRTC();
    #if RTC_SET == 1
      // day, weekday (0 sunday, 1 monday... 6 saturday), month, century (1=1900, 0=2000), year (0-99)
      rtc.setDate(10, 1, 9, 0, 18);
      // hours, minutes, seconds
      rtc.setTime(13, 37, 42);
    #endif
    if(!checkRTC()) {
      DEBUG_OUTPUT(F("RTC reports bad status bytes:"));
      DEBUG_OUTPUT(rtc.getStatus1());
      DEBUG_OUTPUT(rtc.getStatus2());
      blink(2);
    }
  #endif

  digitalWrite(LED_PIN, LOW);
  delay(2000);
}

void Pan::loop() {
  #if USE_JUMPER == 1
    if (hasJumper) {
      sCmd.readSerial();
      return;
    }
  #endif

  buildOutput();
  DEBUG_OUTPUT(output);
  
  #if USE_SD == 1
    if (dataFile) {
      dataFile.println(output);
      dataFile.flush();
    }
  #endif

  output = "";
  delay(FREQUENCY);
}

String Pan::buildOutput() {
  output = "";
  #if USE_RTC == 1
    // ASIA=YYYY-MM-DD; US=MM/DD/YYYY; WORLD=DD-MM-YYYY
    output.concat(rtc.formatDate(RTCC_DATE_ASIA));
    output.concat(',');
    output.concat(rtc.formatTime());
    output.concat(',');
  #endif
  return output;
}

void Pan::blink(byte nb) { 
  while (true) {
    for (byte i = 0; i < nb; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(250);
      digitalWrite(LED_PIN, HIGH);
      delay(250);
    }

    delay(1500);
  }
}

#if USE_SD == 1
  bool Pan::beginSD() {
    return SD.begin(SD_CS_PIN);
  }

  void Pan::openFile(uint8_t mode) {
    dataFile = SD.open(PSTR("data.csv"), mode);
  }
#endif


#if USE_RTC == 1
  void Pan::openRTC() {
    rtc.initClock();
  }

  bool Pan::checkRTC() {
    rtc.getTime();
    // This byte is set at 0 when everything is fine and 255 when the clock is unplugged.
    // Let's assume the clock only works at 0
    return rtc.getStatus1() != 0;
  }
#endif

#if USE_JUMPER == 1
  void Pan::ping() {
    Serial.println(F("PONG"));
  }

  void Pan::sendID() {
    Serial.println(F(STATION_ID));
  }

  // Get one reading
  void Pan::get() {
    buildOutput();
    Serial.println(output);
    output = "";
  }

  #if USE_SD == 1
    // Get everything from the SD card
    void Pan::harvest() {
      if(!dataFile) {
        if(!beginSD()) {
          Serial.println(F("SD card initialization failed"));
          return;
        }
        openFile(FILE_READ);
        if(!dataFile) {
          Serial.println(F("Data file opening failed"));
          return;
        }
        while(dataFile.available()) {
          Serial.write(dataFile.read());
        }
        dataFile.close();
      }
    }

    // Erase data from the SD card
    void Pan::clear() {
      if(!beginSD()) {
        Serial.println(F("SD card initialization failed"));
        return;
      }
      Serial.println(SD.remove(PSTR("data.csv")) ? F("OK") : F("Removal failed"));
    }
  #endif

  #if USE_RTC == 1
    void Pan::getTimestamp() {
      output.concat(rtc.formatDate(RTCC_DATE_ASIA));
      output.concat('T');
      output.concat(rtc.formatTime());
      Serial.println(output);
      output = "";
    }

    void Pan::setDate() {
      int year = atoi(sCmd.next());
      byte month = atoi(sCmd.next());
      byte day = atoi(sCmd.next());
      if(year < 1900 || year >= 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
        Serial.println(F("Bad date format"));
        return;
      }

      // day, weekday (0 sunday, 1 monday... 6 saturday), month, century (1=1900, 0=2000), year (0-99)
      // Weekday is not used anywhere so we will not care about it
      rtc.setDate(day, 0, month, (year < 2000) ? 1 : 0, year % 100);

      // Output the set date and time
      getTimestamp();
    }

    void Pan::setTime() {
      byte h = atoi(sCmd.next());
      byte m = atoi(sCmd.next());
      byte s = atoi(sCmd.next());
      if(h < 0 || h >= 24 || m < 0 || m >= 60 || s < 0 || s > 60) {
        Serial.println(F("Bad time format"));
        return;
      }
      rtc.setTime(h, m, s);
      // Output the set date and time
      getTimestamp();
    }
  #endif

  void Pan::unknownCommand(const char *command) {
    Serial.println(F("Unknown command"));
  }
#endif
