extern "C" {
  #include "spi.h"
  #include "spi_register.h"
}

#include <dht11.h>
#include "Farmy.h"
#include "FarmyDHT.h"

/**
 * Collect sensor data and send back to farmy.net
 *
 * @param device_id
 *            The device_id that you registered at farmy.net
 * @param port_numbers
 *           The port numbers of sensors
 * @param api_key
 *            The api_key from farmy.net
 * @param client
 *            The wifi client
 */
void Farmy::send( const char* device_id, int port_numbers[], String api_key, WiFiClient client)
{
  if(!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  String data = collectData(port_numbers);
  sendData(device_id, api_key, client, data);
}

/**
 * Get trigger setting from Farmy.net. Ex: trigger the pump to give water.
 *
 * @param device_id
 *            The device_id that you registered at farmy.net
 * @param api_key
 *            The api_key from farmy.net
 * @param client
 *            The wifi client
 */
char* Farmy::receive(const char* device_id, String api_key, WiFiClient client) {
  char* json = (char *)malloc(JSON_BUFFER_SIZE);

  if(!client.connect(host, 80)) {
    Serial.println("connection failed");
    return json;
  }

  String url = String("/v0/user_devices/") + device_id + "/triggered_actions/";

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print(String("X-Farmy-Api-Key: ") + api_key + "\r\n");
  client.print("Connection: close\r\n\r\n");
  client.print("\r\n\r\n");

  int timeout = millis() + WIFI_CONNECT_TIMEOUT;
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
  Serial.println("Got json data:");
  Serial.println(json);

  delay(500);
  if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
  }

  return json;
}

/**
 * Collect all sensors' data
 *
 * @param port_numbers
 *           The port numbers of sensors
 */
String Farmy::collectData(int port_numbers[])
{
  StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();

  int i = 0;
  while(port_numbers[i]) {
    JsonObject& object = array.createNestedObject();
    object["num"]   = port_numbers[i];
    object["value"] = fetch(port_numbers[i]);
    ++i;
  }

  // TODO: DHT11 is a basic, ultra low-cost digital temperature and humidity sensor
  // send both DHT11 sensor data
  int DHT11_NUM = 4;
  FarmyDHT dht = FarmyDHT(DHT11_NUM);
  float temperature = dht.fetch_temperature();

  delay(1000);

  JsonObject& dht_object = array.createNestedObject();
  dht_object["num"] = 4;
  dht_object["value"] = temperature;

  char data[JSON_BUFFER_SIZE];
  array.printTo(data, sizeof(data));

  Serial.print("Collected sensors' data are:");
  Serial.println(data);

  return data;
}

/**
 * Send sensor data back to farmy.net
 *
 * @param device_id
 *            The device_id that you registered at farmy.net
 * @param api_key
 *            The api_key from farmy.net
 * @param client
 *            The wifi client
 * @param data
 *            The sensor data
 */
void Farmy::sendData(const char* device_id, String api_key, WiFiClient client, String data)
{
  // Todo: use retry to connect internet.
  Serial.println("Connected to Famry.");
  Serial.println("Posted:" + data);

  // Create HTTP POST Data
  String url = String("/v0/user_devices/") + device_id + "/sensor_datas/";
  client.print(String("POST ") + url + " HTTP/1.1\n"+ "Host: " + host + "\n");
  client.print(String("Host: ") + host + "\n");
  client.print("Content-Type: application/json\n");
  client.print(String("X-Farmy-Api-Key: ") + api_key + "\n");
  client.print("Content-Length: ");
  client.print(data.length());
  client.print("\n\n");

  client.print(data);
  Serial.println("Posted finished.");

  delay(500);
  if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
  }
}

/**
 * fetch sensor data
 *
 * @param channel
 *            The channel of the sensor
 */
uint32 Farmy::fetch(int channel) {
  uint8 cmd = (0b11 << 3) | channel;

  const uint32 COMMAND_LENGTH = 5;
  const uint32 RESPONSE_LENGTH = 12;

  uint32 retval = spi_transaction(HSPI, 0, 0, 0, 0, COMMAND_LENGTH, cmd, RESPONSE_LENGTH, 0);

  retval = retval & 0x3FF; // mask to 10-bit value
  return retval;
}
