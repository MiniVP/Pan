# MiniVP Pan-L

See the [global setup instructions](../README.md) for more information on how to use this station.

## Components

Required components :

* Arduino (tested with Uno R3 and Pro Mini)
* TSL2561 luxmeter

Optional components :

* PCF8563 real-time clock  
  *Enable or disable RTC using the `rtc` build option*
* SD or microSD card reader  
  *Enable or disable SD support using the `sd` build option*

## Pinout

The SD CS and LED pins can be defined using the `sd_cs_pin` and `led_pin` build options, respectively.

Arduino Uno | Components
----------- | ----------
3.3V        | SD 3V3, RTC VCC
GND         | SD GND, RTC GND, LED GND
SDA         | RTC SDA
SCL         | RTC SCL
D4          | SD CS
D9          | LED
D11         | SD MOSI
D12         | SD MISO
D13         | SD CLK
TODO        | TODO

## Build options

The `platformio.ini` file lets you customize the firmware. In addition to the [common options](../README.md#common-build-options), the station adds the following settings:

Name      | Default value | Description
--------- | ------------- | -----------
`TODO`    | TODO          | TODO
