/*
 * Farmy
 */

#include <Arduino.h>
#include <dht11.h>
#include "Farmy.h"
#include "config.h"

extern "C" {
  #include "spi.h"
  #include "spi_register.h"
}


const int nums_size = 8;
int input_nums[nums_size] = { 1, 2, 3 };

// Todo: try to support other device, such as wire cable network.
WiFiClient client;

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
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);

  Serial.print("connecting to ");
  Serial.println(host);

  Farmy farmy;
  farmy.send(DEVICE_ID, input_nums, API_KEY, client);
  delay(2000);
  char* json = farmy.receive(DEVICE_ID, API_KEY, client);
  if (json) {
    executeActions(json);
    free(json);
  }
}
