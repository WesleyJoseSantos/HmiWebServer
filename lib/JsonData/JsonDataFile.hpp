/**
 * @file JsonDataFile.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "JsonData.hpp"

#ifdef  ESP8266
#include "LittleFS.h"
#else
#include "SPIFFS.h"
#endif  //ESP8266

class JsonDataFile : public JsonData
{
private:
    String fileName;

    void saveToFile(String fileName){
        String jsonStr = toJson();
        File file = LittleFS.open(fileName, "w");
        file.print(jsonStr);
        file.close();
    }

    bool loadFromFile(String fileName){
        File file = LittleFS.open(fileName, "r");
        if(file){
            String jsonStr = file.readString();
            fromJson(jsonStr);
            file.close();
            return true;
        }
        return false;
    }

protected:
    void setFileName(String fileName){
        this->fileName = fileName;
    }

    String getFileName(){
        return fileName;
    }
public:
    JsonDataFile() {}
    ~JsonDataFile() {}

    bool exists(){
        return LittleFS.exists(fileName);
    }

    void save(){
        saveToFile(fileName);
    }

    bool load(){
        return loadFromFile(fileName);
    }

    void remove(){
        LittleFS.remove(fileName);
    }
};