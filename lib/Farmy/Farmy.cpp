extern "C" {
  #include "spi.h"
  #include "spi_register.h"
}

#include "Farmy.h"
#include "dht11.h"

void Farmy::send( const char* device_id, int input_pins[], String api_key, WiFiClient client)
{
  if(!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  String data = collectData(input_pins);
  sendData(device_id, api_key, client, data);
}

char* Farmy::get(const char* device_id, String api_key, WiFiClient client) {
  char* json = (char *)malloc(JSON_BUFFER);

  if(!client.connect(host, 80)) {
    Serial.println("connection failed");
    return json;
  }

  String url = String("/api/v0/user_devices/") + device_id + "/triggered_actions/";

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print(String("X-Farmy-Api-Key: ") + api_key + "\r\n");
  client.print("Connection: close\r\n\r\n");
  client.print("\r\n\r\n");

  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return json;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  bool begin = false;
  int index = 0;
  while(client.available()) {
    char c = client.read();
    Serial.print(c);
    if (c == '[') begin = true;
    if (begin) json[index++] = c;
    if (c == ']') break;
  }
  Serial.println("\n\nGot json data ---------------");
  Serial.println(json);

  delay(500);
  if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
  }

  return json;
}

String Farmy::collectData(int input_pins[])
{
  StaticJsonBuffer<256> jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();

  int i = 0;
  while(input_pins[i]) {
    JsonObject& object = array.createNestedObject();
    object["pin"] = input_pins[i];
    object["value"] = check(input_pins[i]);
    ++i;
  }

  // Get dht data
  int DHT11_PIN = 4;
  dht11 DHT;
  int chk = DHT.read(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
                Serial.print("OK,\t");
                break;
    case DHTLIB_ERROR_CHECKSUM:
                Serial.print("Checksum error,\t");
                break;
    case DHTLIB_ERROR_TIMEOUT:
                Serial.print("Time out error,\t");
                break;
    default:
                Serial.print("Unknown error,\t");
                break;
  }

  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);

  delay(1000);

  JsonObject& dht_object = array.createNestedObject();
  dht_object["pin"] = 4;
  dht_object["value"] = DHT.temperature;

  char data[JSON_BUFFER];
  array.printTo(data, sizeof(data));
  Serial.println("\n\nCollected json data:  ----------------------");
  Serial.println(data);
  Serial.println();

  String str = data;

  return data;
}

void Farmy::sendData(const char* device_id, String api_key, WiFiClient client, String data)
{
  // Todo: use retry to connect internet.
  Serial.println("Connected to Famry.");
  Serial.println("Posted:" + data);

  // Create HTTP POST Data
  String url = String("/api/v0/user_devices/") + device_id + "/sensor_datas/";
  client.print(String("POST ") + url + " HTTP/1.1\n"+ "Host: " + host + "\n");
  client.print(String("Host: ") + host + "\n");
  client.print("Content-Type: application/json\n");
  client.print(String("X-Farmy-Api-Key: ") + api_key + "\n");
  client.print("Content-Length: ");
  client.print(data.length());
  client.print("\n\n");

  client.print(data);
  Serial.println("Posted finished -----------");

  delay(500);
  if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
  }
}

uint32 Farmy::check(int channel) {
  uint8 cmd = (0b11 << 3) | channel;

  const uint32 COMMAND_LENGTH = 5;
  const uint32 RESPONSE_LENGTH = 12;

  uint32 retval = spi_transaction(HSPI, 0, 0, 0, 0, COMMAND_LENGTH, cmd, RESPONSE_LENGTH, 0);

  retval = retval & 0x3FF; // mask to 10-bit value
  return retval;
}
