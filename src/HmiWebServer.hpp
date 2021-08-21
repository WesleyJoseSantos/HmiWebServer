/**
 * @file HmiWebServer.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "WebServer.hpp"
#include "ArduinoJson.h"
#include "WifiSettings.hpp"

#define BUF_SIZ 150

class HmiWebServer : public WebServer
{
private:
    WifiSettingsFile wifiFile;
    AsyncWebServer server;

    void wifiBegin(){
        if(wifiFile.exists()){
            WiFi.softAPdisconnect(true);
            wifiFile.load();
            if(wifiFile.data.mode == MODE_ACCESS_POINT){
                WiFi.softAP(wifiFile.data.ssid.c_str(), wifiFile.data.pass.c_str());
                Serial.println("Access Point Started.");
                Serial.print("ssid: ");
                Serial.println(wifiFile.data.ssid);
                Serial.println(WiFi.softAPIP());
                server.addHandler(this);
                server.begin();
            }else{
                WiFi.begin(wifiFile.data.ssid.c_str(), wifiFile.data.pass.c_str());
                Serial.print("Connecting to: ");
                Serial.print(wifiFile.data.ssid);
                Serial.print(", pass: ");
                Serial.println(wifiFile.data.pass);
                int cont = 0;
                while(!WiFi.isConnected()){
                    Serial.print('.');
                    delay(500);
                    if(cont++ > 30){
                        Serial.println("Connection failed.");
                        return;
                    }
                }
                Serial.println("connected!");
                Serial.println(WiFi.localIP());
                server.addHandler(this);
                server.begin();
            }
        }else{
            Serial.println("Wifi is not configured.");
        }
    }

    void setTag(StaticJsonDocument<512> json){
        String tagsData;
        String name = json["Name"];

        if(dataSended) tags.clear();
        dataSended = false;

        HmiWebServer::tags[name] = json["Value"];
        serializeJson(tags, tagsData);
        setData(tagsData);
    }

public:
    static DynamicJsonDocument tags;

    HmiWebServer():server(80){}
    ~HmiWebServer(){}

    void begin(){
        #ifdef  ESP8266
        Serial.begin(74880);
        #else
        Serial.begin(115200);
        #endif  //ESP8266
        Serial.println("\n\nDEVICE STARTED.");
        if(!LittleFS.begin()){
            Serial.println("SPIFFS Mount Failed");
            return;
        }
        wifiBegin();
    }

    void serialReadFile(String fileName, long lenght){
        File file = LittleFS.open(fileName.c_str(), "w");
        uint8_t bytes[BUF_SIZ];
        
        long receivedLenght = 0;

        Serial.printf("Started file %s\n", fileName.c_str());

        while(receivedLenght <= lenght){
            if(Serial.available()){
                int siz = lenght - receivedLenght > BUF_SIZ ? BUF_SIZ : lenght - receivedLenght;
                receivedLenght += Serial.readBytes(bytes, siz);

                file.write(bytes, siz);

                int progress = map(receivedLenght, 0, lenght, 0, 100);
                Serial.printf("Reading file: %s (%d%%) [%ld/%ld]\n", fileName.c_str(), progress, receivedLenght, lenght);
            }
            if(receivedLenght == lenght){
                file.close();
                Serial.printf("File %s Saved!\n", fileName.c_str());
                break;
            }
        }
    }

    void task(){
        if(Serial.available()){
            String data = Serial.readStringUntil('\n');
            StaticJsonDocument<512> json;
            deserializeJson(json, data);
            bool unrecognized = true;

            if(json.containsKey("Test")){
                Serial.println("{\"Test\":true}");
                unrecognized = false;
            }

            if(json.containsKey("StartWifi")){
                wifiBegin();
                unrecognized = false;
            }

            if(json.containsKey("Format")){
                Serial.println("Formatting filesystem...");
                LittleFS.format();
                if(!LittleFS.begin()){
                    Serial.println("SPIFFS Mount Failed");
                }
                Serial.println("Filesystem formated");
                unrecognized = false;
            }

            if(json.containsKey("File")){
                String fileName = json["File"];
                long lenght = json["Length"];
                serialReadFile(fileName, lenght);
                unrecognized = false;
            }

            if(json.containsKey("Ssid")){
                wifiFile.fromJson(data);
                wifiFile.save();
                unrecognized = false;
            }

            if(json.containsKey("Name")){
                setTag(json);
                unrecognized = false;
            }

            if(unrecognized){
                Serial.println("Unrecognized data received:");
                Serial.println(data);
            }
        }
    }
};

DynamicJsonDocument HmiWebServer::tags(2048);