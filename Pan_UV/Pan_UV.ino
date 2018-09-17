/*
 MiniVP Pan-UV v0.2-beta
 LED     => D9
 UV 3V3  => 3.3V
 UV EN   => D10
 UV OUT  => A0
 UV GND  => GND
 A 3.3V  => A1
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
#define STATION_ID "PAN-UV-A-1-300"

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
#define UV_PIN A0
#define UV_EN_PIN 10
#define REF_PIN A1

String output = "";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(UV_EN_PIN, OUTPUT);
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

// Average of multiple readings on an analog pin to try to enhance precision
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return runningValue;
}

// The Arduino Map function but for floats
// From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void buildOutput() {
  #ifdef USE_RTC
    // ASIA=YYYY-MM-DD; US=MM/DD/YYYY; WORLD=DD-MM-YYYY
    output.concat(rtc.formatDate(RTCC_DATE_ASIA));
    output.concat(',');
    output.concat(rtc.formatTime());
    output.concat(',');
  #endif
  digitalWrite(UV_EN_PIN, HIGH);
  delay(10);
  output.concat(mapfloat(3.3 / averageAnalogRead(REF_PIN) * averageAnalogRead(UV_PIN), 0.976, 2.8, 0.0, 15.0));
  digitalWrite(UV_EN_PIN, LOW);
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
      // TODO
    }

    void setTime() {
      // TODO
    }
  #endif

  void unknownCommand(const char *command) {
    Serial.println(F("Unknown command"));
  }
#endif
