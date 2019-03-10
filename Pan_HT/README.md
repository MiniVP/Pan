# MiniVP Pan-HT

See the [global setup instructions](../README.md) for more information on how to use this station.

## Components

Required components :

* Arduino (tested with Uno R3 and Pro Mini)
* DHT-11, DHT-21 (AM2301) or DHT-22 (AM2302, AM2320, AM2321, RHT03) sensor

Optional components :

* PCF8563 real-time clock  
  *Enable or disable RTC using the `rtc` build option*
* SD or microSD card reader  
  *Enable or disable SD support using the `sd` build option*

## Pinout

The DHT data, SD CS and LED pins can be defined using the `dht_pin`, `sd_cs_pin` and `led_pin` build options, respectively.

Arduino Uno | Components
----------- | ----------
3.3V        | SD 3V3, RTC VCC, DHT pin 1 (left)
GND         | SD GND, RTC GND, LED GND, DHT11/22 pin 4 (right) or AM2320 pin 3 (middle right)
SDA         | RTC SDA
SCL         | RTC SCL
D4          | SD CS
D9          | LED
D10         | DHT pin 2 (middle left)
D11         | SD MOSI
D12         | SD MISO
D13         | SD CLK

## Build options

The `platformio.ini` file lets you customize the firmware. In addition to the [common options](../README.md#common-build-options), the station adds the following settings:

Name      | Default value | Description
--------- | ------------- | -----------
`dht_pin` | 10            | The DHT data pin.
