/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>
#include "HmiWebServer.hpp"

#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

HmiWebServer hmi;

void taskComm();

void setup() {
  // put your setup code here, to run once:
  hmi.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  hmi.task();
}
