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
 * Replace this file with your own config.
 * Date:   2017/09/27
 */

#ifndef CONFIG_H
#define CONFIG_H

// Set up to connect to your wifi
#define SSID     "REPLACE_WITH_YOUR_WIFI_SSID"
#define PASSWORD "REPLACE_WITH_YOUR_WIFI_PASSWORD"

// Get the device_id after registered your device at farmy.net
#define DEVICE_ID "REPLACE_WITH_YOUR_DEVICE_ID"

// API key from farmy.net
#define API_KEY "REPLACE_WITH_YOUR_API_KEY"

#define FLASH_DELAY 1000       // Set the short delay to trigger the pump
#define LONG_FLASH_DELAY 15000 // Set the long delay to trigger the pump

#define PORT_NUMBER_SIZE 8

int port_numbers[PORT_NUMBER_SIZE] = { 1, 2, 3 };

#endif
