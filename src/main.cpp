/* Config.h */
/* Copyright (C) 2016 farmy.net, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Author: feng.zhang
 * Date:   2017/09/28
 */

#include <Arduino.h>
#include <dht11.h>
#include "Farmy.h"
#include "Config.h"

extern "C" {
  #include "spi.h"
  #include "spi_register.h"
}

WiFiClient client;

// According to the user setting from Farmy.net, trigger the pump to give water in different ways.
void executeActions(char* json)
{
  StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
  JsonArray& array = jsonBuffer.parseArray(json);
  for(JsonArray::iterator it=array.begin(); it!=array.end(); ++it)
  {
    JsonObject& object = *it;
    String num_s = object["num"];
    String action_type = object["action_type"];
    Serial.println(num_s);
    Serial.println(action_type);

    int num = atof(num_s.c_str());
    pinMode(num, OUTPUT);

    if(action_type == "turn_on") {
        Serial.println("Start to turn on");
        digitalWrite(num, LOW);
    }
    else if(action_type == "turn_off") {
        Serial.println("Start to turn off");
        digitalWrite(num, HIGH);
    }
    else if(action_type == "flash") {
        Serial.println("Start to flash");
        digitalWrite(num, LOW);
        delay(FLASH_DELAY);
        digitalWrite(num, HIGH);
    }
    else if(action_type == "flash_long") {
        Serial.println("Start to long flash");
        digitalWrite(num, LOW);
        delay(LONG_FLASH_DELAY);
        digitalWrite(num, HIGH);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  spi_init(HSPI);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected with ip address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);

  Serial.print("connecting to ");
  Serial.println(host);

  Farmy farmy;
  // send sensor data to Farmy.net
  farmy.send(DEVICE_ID, port_numbers, API_KEY, client);
  delay(2000);
  // Get pump trigger setting from Farmy.net
  char* json = farmy.receive(DEVICE_ID, API_KEY, client);
  if (json) {
    executeActions(json);
    free(json);
  }
}
