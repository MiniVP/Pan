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
D2          | Jumper output
D3          | Jumper input
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
4. Edit the `UV_PIN`, `UV_EN_PIN`, `REF_PIN`, `SD_CS_PIN`, `LED_PIN`, `JUMPER_IN_PIN` and `JUMPER_OUT_PIN` constants if you need to move those pins.
5. (Un)comment the `USE_SD`, `USE_RTC`, `USE_JUMPER`, `DEBUG` constants if you need to enable or disable specific features.
6. Load the firmware onto the Arduino.
7. Have fun

# Troubleshooting

* **The LED turns on, then blinks twice in a loop** : The real-time clock reports an incorrect status. Check the RTC is properly connected and its date/time information is properly set.
* **The LED turns on, then blinks 4 times in a loop** : The SD card initialization failed. Check the pinout and connection to the SD card reader and check the SD card is properly plugged in and works on another device.
* **The LED turns on, then blinks 8 times in a loop** : The SD card was properly initialized but the `DATA.CSV` file could not be opened. Check that the card or file is not read-only, that there is still some disk space on the card, and that the card is properly formatted in FAT16 or FAT32 format as per the [Arduino recommendations](https://www.arduino.cc/en/Reference/SDCardNotes).
* **The LED turns on and does not turn off** : The Arduino is stuck while trying to initialize the real-time clock. Check its pinout and connections and try again.

By enabling the `DEBUG` preprocessor directive, you will be able to get logs sent back to you, particularly during the initialization process, that may be useful for troubleshooting the board.

# Controlling via serial

By enabling the `USE_JUMPER` preprocessor directive, you can let the station enter a special "remote control" mode over serial when booting it with a wire between D2 and D3 (`JUMPER_IN_PIN` and `JUMPER_OUT_PIN`).

Once done, open a serial connection to the Arduino (using the Arduino IDE's serial monitor, or `screen`, `minicom`, `picocom`, Visual Studio Code's Arduino extension, `echo` and `cat`, HyperTerminal, PuTTY, etc.). You can then send commands to the station:

* `PING` : A simple test command that should send `PONG` back.
* `ID` : Will return the station's ID, formatted as `PAN-UV-X-Y-Z`:
  * `X` is the revision number (currently, revision A) ;
  * `Y` is the unique serial number of the station ;
  * `Z` is the frequency, in seconds, of data gathering. For Pan-UV, it defaults to 300.
  Example : `PAN-UV-A-42-600` is a Pan-UV weather station revision A with serial number 42 and collecting data every 10 minutes.
* `GET` : Get a single line of CSV output, just like what would be saved by the station when running normally.

If the firmware was compiled with the `USE_RTC` directive, the following commands are also available:

* `TIMESTAMP` : Get a UNIX timestamp or a ISO 8601 date/time without timezones (`yyyy-mm-ddThh:mm:ss`) for the currently set date/time on the station ;
* `DATE <YYYY> <MM> <DD>` : Allows to set the current date on the station's real-time clock. For example, `DATE 2001 09 11` will set the date to September 11th, 2001 ;
* `TIME <HH> <MM> <SS>` : Allows to set the current time on the station's real-time clock, in 24-hour format. For example, `TIME 06 09 42` will set the time to 6:09:42 AM.

If the firmware was compiled with the `USE_SD` directive, the following commands are also available:

* `HARVEST` : Get all the records that were stored on the SD card, without altering it ;
* `CLEAR` : Remove all the stored records from the SD card.
