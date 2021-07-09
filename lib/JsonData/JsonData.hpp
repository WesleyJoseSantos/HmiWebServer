/**
 * @file JsonData.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "WString.h"

class JsonData
{
private:
    
public:
    JsonData() {}
    ~JsonData() {}

    virtual void fromJson(String jsonStr) = 0;
    virtual String toJson() = 0;
};