/*
 MiniVP Pan-HT v0.2
 DHT 1 => 5V or 3.3V
 DHT 2 => D10
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

 Jumper pins: D2 -> D3
*/

#ifndef DHT_PIN
#define DHT_PIN 10
#endif

#include "Pan.h"
#include "DHT.h"

class PanHT: public Pan {
  public:
    PanHT(): Pan() {
    }
    void begin() {
      Pan::begin();
      dht.setup(DHT_PIN);
    }
    String buildOutput() {
      float temp = dht.getTemperature();
      float humid = dht.getHumidity();
      String out = Pan::buildOutput();
      out.concat(dht.getStatusString());
      out.concat(',');
      out.concat(temp);
      out.concat(',');
      out.concat(humid);
      return out;
    }
  protected:
    DHT dht;
};

PanHT panHT;

void setup() {
  panHT.begin();
}

void loop() {
  panHT.loop();
}
