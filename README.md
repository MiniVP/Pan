![MiniVP Pan](img/banner.png)

A set of autonomous single-sensor stations designed to last long.

* Pan-HT : *Humidity-Temperature*, uses DHT-11, DHT-21 or DHT-22 (AM2301, AM2302, AM2320, AM2321) sensors to record relative humidity and temperature every few minutes
* Pan-L : *Light* : uses a photoresistor to record light levels every few minutes
* Pan-UV : *UltraViolet* : uses a UV sensor to record UV index every few minutes

See the README files inside those folders to get more information about each station.

> This repository is mirrored to GitHub from https://gitlab.com/MiniVP/Pan.

# Global instructions

## Software requirements

All stations require [PlatformIO](https://platformio.org/) to be installed. PlatformIO will then take care of installing everything else.

## Compiling

You should be able to compile all the firmwares directly using `pio run`.

## Preparing a board

* Set station-specific settings in the `platformio.ini` files before uploading to a board.
* After plugging in your Arduino, use `pio device list` to get the name of your Arduino board.
* Set the `PLATFORMIO_UPLOAD_PORT` environment variable to your board's name.
* Run `pio run -t upload -e uno` for an Arduino Uno board, or `pio run -t upload -e promini` for a Pro Mini ATMega328 3.3V 8MHz.

### Using an unsupported board

Although nothing other than a Uno and a Pro Mini ATMega328P have been tested, you can try compiling a firmware for another board. In the `platformio.ini` file of each station, add the following settings:

```
[env:myboard]
platform = ...
board = ...
framework = arduino
build_flags = ${common.build_flags}
lib_deps = ${common.lib_deps}
```

[This list](https://platformio.org:443/boards?filter%5Bframeworks%5D=arduino) provides all the Arduino-compatible boards supported by PlatformIO.

## Common build options

Build options can be set in the `platformio.ini` files for each station firmware. Stations will add their own settings depending on the components they use; see their corresponding README for their specific settings.

Name             | Default value    | Description
---------------- | ---------------- | -----------
`debug`          | 1                | Enables debug mode; outside of the serial control mode, the station will print debugging information to a serial link.
`sd`             | 1                | Enable SD card support.
`rtc`            | 1                | Enable real-time clock support.
`jumper`         | 1                | Enable serial control, activated at station startup using a jumper.
`frequency`      | 300000           | Duration, in milliseconds, between each measurement.
`station_id`     | `PAN-xx-A-1-300` | A unique station identifier. MiniVP's convention is `ModelName-ModelType-Revision-SerialNo-Frequency`, with the frequency in seconds.
`led_pin`        | 9                | The digital pin for the LED indicator. Must be an output pin.
`sd_cs_pin`      | 4                | The SPI Slave Select pin, or CS, for the SD card. Must be an output pin.
`jumper_in_pin`  | 2                | The digital input pin for the serial control jumper.
`jumper_out_pin` | 3                | The digital output pin for the serial control jumper.

## Troubleshooting

All three stations have a common way of handling errors.

* **The LED turns on, then blinks twice in a loop** : The real-time clock reports an incorrect status. Check the RTC is properly connected and its date/time information is properly set.
* **The LED turns on, then blinks 4 times in a loop** : The SD card initialization failed. Check the pinout and connection to the SD card reader and check the SD card is properly plugged in and works on another device.
* **The LED turns on, then blinks 8 times in a loop** : The SD card was properly initialized but the `DATA.CSV` file could not be opened. Check that the card or file is not read-only, that there is still some disk space on the card, and that the card is properly formatted in FAT16 or FAT32 format as per the [Arduino recommendations](https://www.arduino.cc/en/Reference/SDCardNotes).
* **The LED turns on and does not turn off** : The Arduino is stuck while trying to initialize either the real-time clock or the DHT. Check their pinouts and connections and try again.

By enabling the `DEBUG` preprocessor directive, you will be able to get logs sent back to you, particularly during the initialization process, that may be useful for troubleshooting the board.

## Controlling via serial

By enabling the `USE_JUMPER` preprocessor directive, you can let the station enter a special "remote control" mode over serial when booting it with a wire between D2 and D3 (`JUMPER_IN_PIN` and `JUMPER_OUT_PIN`).

Once done, open a serial connection to the Arduino (using the Arduino IDE's serial monitor, or `screen`, `minicom`, `picocom`, Visual Studio Code's Arduino extension, `echo` and `cat`, HyperTerminal, PuTTY, etc.). You can then send commands to the station:

* `PING` : A simple test command that should send `PONG` back.
* `ID` : Will return the station's ID, formatted as `PAN-HT-X-Y-Z`:
  * `X` is the revision number (currently, revision A) ;
  * `Y` is the unique serial number of the station ;
  * `Z` is the frequency, in seconds, of data gathering. For Pan-UV, it defaults to 300.
  Example : `PAN-HT-A-42-600` is a Pan-HT weather station revision A with serial number 42 and collecting data every 10 minutes.
* `GET` : Get a single line of CSV output, just like what would be saved by the station when running normally.

If the firmware was compiled with the `USE_RTC` directive, the following commands are also available:

* `TIMESTAMP` : Get a UNIX timestamp or a ISO 8601 date/time without timezones (`yyyy-mm-ddThh:mm:ss`) for the currently set date/time on the station ;
* `DATE <YYYY> <MM> <DD>` : Allows to set the current date on the station's real-time clock. For example, `DATE 2001 09 11` will set the date to September 11th, 2001 ;
* `TIME <HH> <MM> <SS>` : Allows to set the current time on the station's real-time clock, in 24-hour format. For example, `TIME 06 09 42` will set the time to 6:09:42 AM.

If the firmware was compiled with the `USE_SD` directive, the following commands are also available:

* `HARVEST` : Get all the records that were stored on the SD card, without altering it ;
* `CLEAR` : Remove all the stored records from the SD card.
