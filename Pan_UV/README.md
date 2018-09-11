# MiniVP Pan-UV

## Components

Required components :

* Arduino (tested with Uno R3 and Pro Mini)
* ML8511 UV sensor

Optional components :

* PCF8563 real-time clock  
  *Enable or disable RTC using the `#define USE_RTC` statement*
* SD or microSD card reader  
  *Enable or disable SD support using the `#define USE_SD` statement*

## Pinout

The 3.3V reference pin, UV output pin, SD CS and LED pins can be defined using the `REF_PIN`, `UV_PIN`, `SD_CS_PIN` and `LED_PIN` preprocessor directives, respectively.

Arduino Uno | Components
----------- | ----------
3.3V        | SD 3V3, RTC VCC, UV 3V3, Arduino A1
GND         | SD GND, RTC GND, UV GND, LED GND
SDA         | RTC SDA
SCL         | RTC SCL
D4          | SD CS
D9          | LED
D10         | UV EN
D11         | SD MOSI
D12         | SD MISO
D13         | SD CLK
A0          | UV OUT
A1          | Arduino 3.3V

## Setup

1. Clone or download this repository.
2. Move the contents of the `libraries/` folder into your Arduino IDE libraries folder.
3. Open `Pan_UV.ino` in the Arduino IDE.
4. Edit the `UV_PIN`, `UV_EN_PIN`, `REF_PIN`, `SD_CS_PIN` and `LED_PIN` constants if you need to move those pins.
5. (Un)comment the `USE_SD`, `USE_RTC`, `DEBUG` constants if you need to enable or disable specific features.
6. Load the firmware onto the Arduino.
7. Have fun

# Troubleshooting

* **The LED turns on, then blinks twice in a loop** : The real-time clock reports an incorrect status. Check the RTC is properly connected and its date/time information is properly set.
* **The LED turns on, then blinks 4 times in a loop** : The SD card initialization failed. Check the pinout and connection to the SD card reader and check the SD card is properly plugged in and works on another device.
* **The LED turns on, then blinks 8 times in a loop** : The SD card was properly initialized but the `DATA.CSV` file could not be opened. Check that the card or file is not read-only, that there is still some disk space on the card, and that the card is properly formatted in FAT16 or FAT32 format as per the [Arduino recommendations](https://www.arduino.cc/en/Reference/SDCardNotes).
* **The LED turns on and does not turn off** : The Arduino is stuck while trying to initialize the real-time clock. Check its pinout and connections and try again.

By enabling the `DEBUG` preprocessor directive, you will be able to get logs sent back to you, particularly during the initialization process, that may be useful for troubleshooting the board.
