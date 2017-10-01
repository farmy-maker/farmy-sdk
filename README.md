# farmy-sdk
C++ SDK for [FARMY](https://www.farmy.net/)

## Requirement
- [PlatformIO 2.0.0-rc2 or above](http://platformio.org/): open source ecosystem for IoT development.
- [Atom 1.19.1 or above](https://atom.io/)

## Devices
Name|Production Number|Addition
---|---|---
ESP8266 Wroom | ESP8266 |low-cost Wi-Fi chip with full TCP/IP stack and MCU (microcontroller unit) capability|
DHT11 | DHT11 | low-cost Wi-Fi chip with full TCP/IP stack and MCU (microcontroller unit) capability |
IlluminanceIn sensor|DFR0026||
Moisture Sensor|SEN0114|-|
Transistor array|TD62064||50V|
A/D converter|MCP3004|
pump|-|
bread board|-|-|

## Installation
1. Create your project using PlatformIO.

2. Copy the source

```
git clone git@github.com:farmy-maker/farmy-sdk.git
```

3. Copy lib/* to your project path

```
cp -rf farmy-sdk/lib/* YOU_PROJECT_ROOT/lib/
```

4. Install libraries

```
platformio lib install dht11 --interactive (choose ID of 436)
platformio lib install ArduinoJson
```

5. Connect devices

6. Build

## Run Demo Application
1. Copy the source

```
git clone git@github.com:farmy-maker/farmy-sdk.git
```

2. Install libraries

```
platformio lib install dht11 --interactive (choose ID of 436)
platformio lib install ArduinoJson
```

3. Connect devices

4. Build
