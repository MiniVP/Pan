/*
 MiniVP Pan-HT v0.1
 DHT 1 => 5V or 3.3V
 DHT 2 => D8
 DHT 4 => GND
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
 */

#include <DHT.h>

#define USE_SD
#define USE_RTC
#define DEBUG

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

#define LED_PIN 9
#define DHT_PIN 8
// DHT11, DHT21, DHT22
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
String output = "";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);
  dht.begin();

  #ifdef USE_SD
    if (!SD.begin(SD_CS_PIN)) {
      DEBUG_OUTPUT(F("SD initialisation failed"));
      blink(4);
    } else {
      DEBUG_OUTPUT(F("SD init OK"));
      dataFile = SD.open(F("data.csv"), FILE_WRITE);
      if (!dataFile) {
        DEBUG_OUTPUT(F("File opening failed"));
        blink(8);
      }
      DEBUG_OUTPUT(F("SD File data.csv opened OK"));
    }
  #endif
  
  #ifdef USE_RTC
    rtc.initClock();
    #ifdef RTC_SET
    // day, weekday (0 sunday, 1 monday... 6 saturday), month, century (1=1900, 0=2000), year (0-99)
    rtc.setDate(10, 1, 9, 0, 18);
    // hours, minutes, seconds
    rtc.setTime(13, 37, 42);
    #endif
    rtc.getTime();
    if(rtc.getStatus1() != 0) {
      // This byte is set at 0 when everything is fine and 255 when then clock is unplugged.
      // Let's assume the clock only works at 0
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
  #ifdef USE_RTC
    // ASIA=YYYY-MM-DD; US=MM/DD/YYYY; WORLD=DD-MM-YYYY
    output.concat(rtc.formatDate(RTCC_DATE_ASIA));
    output.concat(',');
    output.concat(rtc.formatTime());
    output.concat(',');
  #endif
  output.concat(dht.readTemperature());
  output.concat(',');
  output.concat(dht.readHumidity());
  
  DEBUG_OUTPUT(output);
  
  #ifdef USE_SD
    if (dataFile) {
      dataFile.println(output);
      dataFile.flush();
    }
  #endif

  output = "";
  delay(300000);
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
