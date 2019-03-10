# MiniVP Pan-UV

See the [global setup instructions](../README.md) for more information on how to use this station.

## Components

Required components :

* Arduino (tested with Uno R3 and Pro Mini)
* ML8511 UV sensor

Optional components :

* PCF8563 real-time clock  
  *Enable or disable RTC using the `rtc` build option*
* SD or microSD card reader  
  *Enable or disable SD support using the `sd` build option*

## Pinout

The UV enable, UV output, 3.3V reference, SD CS and LED pins can be defined using the `uv_en_pin`, `uv_pin`, `ref_pin`, `sd_cs_pin` and `led_pin` build options, respectively.


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
A1          | Arduino 3.3V (`ref_pin`)

## Build options

The `platformio.ini` file lets you customize the firmware. In addition to the [common options](../README.md#common-build-options), the station adds the following settings:

Name        | Default value | Description
----------- | ------------- | -----------
`uv_en_pin` | 10            | Digital output pin to the UV enable pin.
`uv_pin`    | A0            | Analog input pin for the UV output.
`ref_pin`   | A1            | Analog input pin for the 3.3V reference.
