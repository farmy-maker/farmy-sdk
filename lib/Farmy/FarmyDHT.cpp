#include <ArduinoJson.h>
#include <dht11.h>
#include "FarmyDHT.h"

FarmyDHT::FarmyDHT(int port_number) {
    port_number = port_number;
}

float FarmyDHT::fetch_temperature() {
  dht11 dht;
  int status = dht.read(port_number);

  switch (status)
  {
    case DHTLIB_OK:
                Serial.println("OK");
                break;
    case DHTLIB_ERROR_CHECKSUM:
                Serial.println("Checksum error");
                return ERROR_TEMPERATURE;
    case DHTLIB_ERROR_TIMEOUT:
                Serial.print("Time out error,\t");
                return ERROR_TEMPERATURE;
    default:
                Serial.print("Unknown error,\t");
                return ERROR_TEMPERATURE;
  }

  Serial.print("Humidity and temperature are: ");
  Serial.print(dht.humidity, 1);
  Serial.print(",\t");
  Serial.println(dht.temperature, 1);

  return dht.temperature;
}
