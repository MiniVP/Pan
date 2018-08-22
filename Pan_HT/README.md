# MiniVP Pan-HT

## Components

Required components :

* Arduino (tested with Uno R3 and Pro Mini)
* DHT-11, DHT-21 (AM2301) or DHT-22 (AM2302, AM2320, AM2321) sensor
  *Set the sensor type in the `#define DHT_TYPE` statement before compiling*

Optional components :

* DS3231 real-time clock
  *Enable or disable RTC using the `#define USE_RTC` statement*
* SD or microSD card reader
  *Enable or disable SD support using the `#define USE_SD` statement*

## Pinout

Arduino Uno | Components
----------- | ----------
3.3V        | SD 3V3, RTC VCC, DHT pin 1 (left)
GND         | SD GND, RTC GND, DHT pin 4 (right)
SDA         | RTC SDA
SCL         | RTC SCL
D4          | SD CS
D8          | DHT pin 2 (middle left)
D11         | SD MOSI
D12         | SD MISO
D13         | SD CLK

## Setup

1. Clone or download this repository.
2. Move the contents of the `libraries/` folder into your Arduino IDE libraries folder.
3. Open `Pan_HT.ino` in the Arduino IDE.
4. Edit the `DHT_PIN` and `SD_CS_PIN` constants if you need to move those pins.
5. Edit the `DHT_TYPE` constant to match your DHT sensor type.
6. (Un)comment the `USE_SD`, `USE_RTC`, `DEBUG` constants if you need to enable or disable specific features.
6. Load the firmware onto the Arduino.
7. Once the RTC clock has been set once, you can comment the previously edited `rtc.set` instructions to prevent a clock reset at each Arduino reboot.
8. Resend the firmware to the Arduino.
