/**
 * @file WifiSettings.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "WString.h"
#include "JsonData.h"
#include "ArduinoJson.h"

#define WIFI_SETTINGS_FILE "/wifi.json"

enum WifiMode 
{
    MODE_ACCESS_POINT,
    MODE_STATION
};

class WifiSettings : JsonData
{
private:
public:
    WifiMode mode;
    String ssid;
    String pass;

    WifiSettings() {}
    ~WifiSettings() {}

    void fromJson(String jsonStr) override {
        StaticJsonDocument<256> doc;
        deserializeJson(doc, jsonStr);

        mode = doc["Mode"];
        ssid = doc["Ssid"].as<String>();
        pass = doc["Pass"].as<String>();
    }

    String toJson() override {
        StaticJsonDocument<256> doc;
        String jsonStr;

        doc["Mode"] = mode;
        doc["Ssid"] = ssid;
        doc["Pass"] = pass;

        serializeJson(doc, jsonStr);
        return jsonStr;
    }
};

class WifiSettingsFile : public JsonDataFile
{
private:
public:
    WifiSettings data;

    WifiSettingsFile() {
        setFileName(WIFI_SETTINGS_FILE);
    }

    ~WifiSettingsFile() {}

    void fromJson(String jsonStr) override {
        data.fromJson(jsonStr);
    }

    String toJson() override {
        return data.toJson();
    }
};