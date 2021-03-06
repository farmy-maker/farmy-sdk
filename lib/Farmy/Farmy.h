/* Farmy.h */
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

/**
  * Author: feng.zhang
  * Date:   2016/08/02
**/

#ifndef FARMY_H
#define FARMY_H

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define JSON_BUFFER_SIZE 256 // set json buffer size to 256 Bytes
#define HTTP_CLIENT_DEFAULT_TIMEOUT 15000 // timeout after 15 seconds

#define WIFI_CONNECT_TIMEOUT 5000 // timout after 5 seconds

static const char* host = "api.farmy.net";

class Farmy
{
  public:
    void send(const char* device_id, int port_numberss[], String api_key, WiFiClient client);
    char* receive(const char* device_id, String api_key, WiFiClient client);

  private:
    String collectData(int port_numberss[]);
    void sendData(const char* device_id, String api_key, WiFiClient client, String data);
    uint32 fetch(int channel);
};

#endif
