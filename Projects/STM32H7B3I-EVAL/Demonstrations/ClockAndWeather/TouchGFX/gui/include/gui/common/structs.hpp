#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <touchgfx/Unicode.hpp>

struct weatherData
{
    int temperature[6];
    int wind[6];
    char icon[6][10];
    char dates[6][30];
    char city[20];
    char country[20];
    int windDegree;
    int pressure;
    int humidity;
    double rain;
    int timezone;
    bool isDay;
    bool isLoaded;
};

struct wifiData
{
    touchgfx::Unicode::UnicodeChar ssid[32];
    touchgfx::Unicode::UnicodeChar encryption[30];
    int     signalStrength;
    int     id;
    bool    encrypted;
};

struct selectedWifi
{
    wifiData wifi;
    touchgfx::Unicode::UnicodeChar password[64];
};

#endif // STRUCTS_HPP

