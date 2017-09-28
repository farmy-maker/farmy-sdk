/**
 * Farmy
 */
#include "Arduino.h"
#include "Farmy.h"
#include "dht11.h"

extern "C" {
  #include "spi.h"
  #include "spi_register.h"
}


#define FLASH_DELAY 1000
#define LONG_FLASH_DELAY 15000
#define SEND_INTERVAL = 2000 // milliseconds

// Todo: try to config this in config file.
/* setup Wifi */
const char* ssid     = "HWD15_E0191D438EFA";
const char* password = "2i769r4mirj8y5i";

const char* device_id = "bx7eWzca";
String api_key = "CyL3K5AkTUM6cmuPRvXL2T";

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
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  delay(SEND_INTERVAL);

  Serial.print("connecting to ");
  Serial.println(host);

  Farmy farmy;
  farmy.send(device_id, input_nums, api_key, client);
  delay(SEND_INTERVAL);
  char* json = farmy.get(device_id, api_key, client);
  if (json) {
    executeActions(json);
    free(json);
  }
}
