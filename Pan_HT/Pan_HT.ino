/*
 MiniVP Pan-HT v0.1
 DHT 1 => 5V or 3.3V
 DHT 2 => D8
 DHT 4 => GND
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
#define DEBUG

#ifdef DEBUG
 #define DEBUG_OUTPUT(x)  Serial.println(x)
#else
 #define DEBUG_OUTPUT(x)
#endif

#ifdef USE_SD
  #include <SPI.h>
  #include <SD.h>
  
  #define SD_CS_PIN 4
  
  File dataFile;
#endif

#define DHT_PIN 8
// DHT11, DHT21, DHT22
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
String output = "";

void setup() {
  Serial.begin(115200);
  dht.begin();

  #ifdef USE_SD
    if (!SD.begin(SD_CS_PIN)) {
      DEBUG_OUTPUT(F("SD initialisation failed"));
    } else {
      DEBUG_OUTPUT(F("SD init OK"));
      dataFile = SD.open(F("data.csv"), FILE_WRITE);
      if (!dataFile) {
        DEBUG_OUTPUT(F("File opening failed"));
      }
      DEBUG_OUTPUT(F("SD File data.csv opened OK"));
    }
  #endif
  
  delay(2000);
}

void loop() {
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
  delay(60000);
}
