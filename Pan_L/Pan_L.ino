/*
 MiniVP Pan-L v0.2
 PINS TODO
 LED => D9
 SD 3V3  => 3.3V
 SD GND  => GND
 SD CS   => D4
 SD MOSI => D11
 SD MISO => D12
 SD CLK  => D13
 RTC VCC => 5V or 3.3V
 RTC GND => GND
 RTC SDA => SDA
 RTC SCL => SCL

 Jumper pins: D2 -> D3
 */

#define USE_SD
#define USE_RTC
#define USE_JUMPER
#define DEBUG
// How often the station should fetch data, in milliseconds
#define FREQUENCY 300000
// Station ID : model name, model type, revision, serial number, frequency
#define STATION_ID "PAN-L-A-1-300"

#ifdef DEBUG
 #define DEBUG_OUTPUT(x) Serial.println(x)
#else
 #define DEBUG_OUTPUT(x)
#endif

#ifdef USE_SD
  #include <SPI.h>
  #include <SD.h>
  
  #define SD_CS_PIN 4
  
  File dataFile;
#endif

#ifdef USE_RTC
  #include <Rtc_Pcf8563.h>
  //#define RTC_SET

  Rtc_Pcf8563 rtc;
#endif

#ifdef USE_JUMPER
  #include <SerialCommand.h>
  #define JUMPER_OUT_PIN 3
  #define JUMPER_IN_PIN 2

  SerialCommand sCmd;
  bool hasJumper = false;
#endif

#define LED_PIN 9

String output = "";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(JUMPER_IN_PIN, INPUT);
  pinMode(JUMPER_OUT_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);

  #ifdef USE_JUMPER
    digitalWrite(JUMPER_OUT_PIN, HIGH);
    delay(100);
    hasJumper = digitalRead(JUMPER_IN_PIN) == HIGH;
    digitalWrite(JUMPER_OUT_PIN, LOW);

    if (hasJumper) {
      sCmd.addCommand("PING", ping);
      sCmd.addCommand("ID", sendID);
      sCmd.addCommand("GET", get);
      #ifdef USE_SD
        sCmd.addCommand("HARVEST", harvest);
        sCmd.addCommand("CLEAR", clear);
      #endif
      #ifdef USE_RTC
        sCmd.addCommand("TIMESTAMP", getTimestamp);
        sCmd.addCommand("DATE", setDate);
        sCmd.addCommand("TIME", setTime);
      #endif
      sCmd.setDefaultHandler(unknownCommand);
      DEBUG_OUTPUT(F("Serial control activated"));
      return;
    }
  #endif

  #ifdef USE_SD
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
  
  #ifdef USE_RTC
    openRTC();
    #ifdef RTC_SET
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

void loop() {
  if (hasJumper) {
    sCmd.readSerial();
    return;
  }

  buildOutput();
  DEBUG_OUTPUT(output);
  
  #ifdef USE_SD
    if (dataFile) {
      dataFile.println(output);
      dataFile.flush();
    }
  #endif

  output = "";
  delay(FREQUENCY);
}

void blink(byte nb) { 
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

void buildOutput() {
  #ifdef USE_RTC
    // ASIA=YYYY-MM-DD; US=MM/DD/YYYY; WORLD=DD-MM-YYYY
    output.concat(rtc.formatDate(RTCC_DATE_ASIA));
    output.concat(',');
    output.concat(rtc.formatTime());
    output.concat(',');
  #endif
}


#ifdef USE_SD
  bool beginSD() {
    return SD.begin(SD_CS_PIN);
  }

  void openFile(uint8_t mode) {
    dataFile = SD.open(F("data.csv"), mode);
  }
#endif


#ifdef USE_RTC
  void openRTC() {
    rtc.initClock();
  }

  bool checkRTC() {
    rtc.getTime();
    // This byte is set at 0 when everything is fine and 255 when then clock is unplugged.
    // Let's assume the clock only works at 0
    return rtc.getStatus1() != 0;
  }
#endif


#ifdef USE_JUMPER
  void ping() {
    Serial.println(F("PONG"));
  }

  void sendID() {
    Serial.println(F(STATION_ID));
  }

  // Get one reading
  void get() {
    buildOutput();
    Serial.println(output);
    output = "";
  }

  #ifdef USE_SD
    // Get everything from the SD card
    void harvest() {
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
    void clear() {
      if(!beginSD()) {
        Serial.println(F("SD card initialization failed"));
        return;
      }
      Serial.println(SD.remove(F("data.csv")) ? F("OK") : F("Removal failed"));
    }
  #endif

  #ifdef USE_RTC
    void getTimestamp() {
      output.concat(rtc.formatDate(RTCC_DATE_ASIA));
      output.concat('T');
      output.concat(rtc.formatTime());
      Serial.println(output);
      output = "";
    }

    void setDate() {
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

    void setTime() {
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

  void unknownCommand(const char *command) {
    Serial.println(F("Unknown command"));
  }
#endif
