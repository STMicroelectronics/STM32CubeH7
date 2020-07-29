#ifndef JSON_PARSERS_H
#define JSON_PARSERS_H
#include <gui/common/structs.hpp>
#ifdef __cplusplus
 extern "C" {
#endif
int extractWeatherData(const char *jsn_string, struct weatherData* data);
int extractDateTime(char *json_string, char* date_time, char* day_of_week, int *unixtime);

#ifdef __cplusplus
 }
#endif
#endif // JSON_PARSERS_H
