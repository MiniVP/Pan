/*
 MiniVP Pan-UV v0.2
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

#define LED_PIN 9
#define UV_PIN A0
#define UV_EN_PIN 10
#define REF_PIN A1

#include "Pan.h"

class PanUV: public Pan {
  public:
    PanUV(): Pan() {
    }
    void begin() {
      Pan::begin();
      pinMode(UV_EN_PIN, OUTPUT);
    }
    String buildOutput() {
      String out = Pan::buildOutput();
      digitalWrite(UV_EN_PIN, HIGH);
      delay(10);
      out.concat(mapfloat(3.3 / averageAnalogRead(REF_PIN) * averageAnalogRead(UV_PIN), 0.976, 2.8, 0.0, 15.0));
      digitalWrite(UV_EN_PIN, LOW);
      return out;
    }
  private:
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
};

PanUV panUV;

void setup() {
  panUV.begin();
}

void loop() {
  panUV.loop();
}
