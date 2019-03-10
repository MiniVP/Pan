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

#include "Pan.h"

class PanL: public Pan {
  public:
    PanL(): Pan() {
    }
    void begin() {
      Pan::begin();
      // TODO
    }
    String buildOutput() {
      String out = Pan::buildOutput();
      // TODO
      return out;
    }
};

PanL panL;

void setup() {
  panL.begin();
}

void loop() {
  panL.loop();
}
