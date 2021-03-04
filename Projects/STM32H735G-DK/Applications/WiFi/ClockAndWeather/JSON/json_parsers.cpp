#include "jsmn.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "json_parsers.h"
#include "FreeRTOS.h"

static jsmntok_t* t = NULL;

int extractWeatherData(const char *json_string, struct weatherData* data);
int jsoneq(const char *json, jsmntok_t *tok, const char *s);
void getString(char *destination, const char *json_string, jsmntok_t *tok);
void getValDouble(double *destination, const char *json_string, jsmntok_t *tok);
void getValInt(int *destination, const char *json_string, jsmntok_t *tok);

int extractWeatherData(const char *json_string, struct weatherData* data)
{
  int i;
  int r;
  jsmn_parser p;
  int token_size = 0;
  int timezone;

  jsmn_init(&p);

  /* Passing NULL instead of the tokens array would not store parsing results,
  but instead the function will return the value of tokens needed to parse the given string.
  This can be useful if you don’t know yet how many tokens to allocate. */
 token_size = jsmn_parse(&p, json_string, strlen(json_string), NULL,0);

  t = (jsmntok_t*) pvPortMalloc(token_size * sizeof(jsmntok_t));

  jsmn_init(&p);

  r = jsmn_parse(&p, json_string, strlen(json_string), t, token_size);

  if (r < 0) {
    return 1;
  }

  /* Assume the top-level element is an object */
  if (r < 1 || t[0].type != JSMN_OBJECT) {
    return 1;
  }
  for (i = 1; i < r; i++) {
    if (jsoneq(json_string, &t[i], "city") == 0) {
      int j;
      if (t[i + 1].type != JSMN_OBJECT) {
        continue;
      }
      for (j = 0; j < t[i + 1].size; j++) {
        if (jsoneq(json_string, &t[i + j], "name") == 0) {
          getString(data->city, json_string, &t[i + j + 1]);
        }
      }
      i += t[i + 1].size + 1;
    }

    else if(jsoneq(json_string, &t[i], "country") == 0) {
      getString(data->country, json_string, &t[i+1]);
      i++;
    }
    else if (jsoneq(json_string, &t[i], "timezone") == 0) {
      getValInt(&timezone, json_string, &t[i + 1]);
      data->timezone = timezone;
      i++;
    }
  }

  char tempMaxClock;
  char tempMinClock;

  if (timezone >= 0)
  {
    if (timezone <= 3600) // Time Zones: UTC: 0, +1
    {
      tempMaxClock = '5'; //15:00
      tempMinClock = '3'; //03:00
    }
    else if (timezone <= 3600 * 3) // Time Zones: UTC: +2, +3
    {
      tempMaxClock = '2'; //12:00
      tempMinClock = '0'; //00:00
    }
    else if (timezone <= 3600 * 6) // Time Zones: UTC: +4, +5, +6
    {
      tempMaxClock = '9'; //09::00
      tempMinClock = '1'; //21::00
    }
    else if (timezone <= 3600 * 9)// Time Zones: UTC: +7 +8, +9
    {
      tempMaxClock = '6'; //06:00
      tempMinClock = '8'; //18:00
    }
    else// Time Zones: UTC: +10 +11 +12
    {
      tempMaxClock = '3'; //03:00
      tempMinClock = '5'; //15:00
    }
  }
  else
  {
    if (timezone >= -3600 * 2) // Time Zones: UTC: -1, -2
    {
      tempMaxClock = '5'; //15:00
      tempMinClock = '3'; //03:00
    }
    else if (timezone >= -3600 * 5) // Time Zones: UTC: -3, -4, -5
    {
      tempMaxClock = '8'; //18:00
      tempMinClock = '6'; //06:00
    }
    else if (timezone >= -3600 * 8) // Time Zones: UTC: -6, -7, -8
    {
      tempMaxClock = '1'; //21:00
      tempMinClock = '9'; //09:00
    }
    else if (timezone >= -3600 * 9) // Time Zones: UTC: -9, -10, -11
    {
      tempMaxClock = '0'; //00:00
      tempMinClock = '2'; //12:00
    }
    else // Time Zones: UTC: -12
    {
      tempMaxClock = '3'; //03:00
      tempMinClock = '5'; //15:00
    }
  }

  bool firstDataSet = true;
  bool rainPresent = false;

  bool dataDay = false;

  int clockCnt = 0;
  double temporaryWindeSpeed;
  double temporaryTempMin;
  double temporaryTempMax;
  char temporaryDate[20];
  char temporaryIcon[10];

  for (i = 1; i < r; i++)
  {
    if (jsoneq(json_string, &t[i], "temp_min") == 0) {
      getValDouble(&temporaryTempMin, json_string, &t[i + 1]);
      i++;
    }

    else if (jsoneq(json_string, &t[i], "temp_max") == 0) {
      getValDouble(&temporaryTempMax, json_string, &t[i + 1]);
      i++;
    }

    else if (jsoneq(json_string, &t[i], "speed") == 0) {
      getValDouble(&temporaryWindeSpeed, json_string, &t[i + 1]);
      i++;
    }

    else if (jsoneq(json_string, &t[i], "icon") == 0) {
      getString(temporaryIcon, json_string, &t[i + 1]);
      if (firstDataSet)
      {
        dataDay = (temporaryIcon[2] == 'd') ? true : false;
        data->isDay = dataDay;
      }
      i++;
    }

    else if((jsoneq(json_string, &t[i], "deg") == 0) && firstDataSet) {
      getValInt(&data->windDegree, json_string, &t[i + 1]);
      i++;
    }

    else if ((jsoneq(json_string, &t[i], "pressure") == 0) && firstDataSet) {
      getValInt(&data->pressure, json_string, &t[i + 1]);
      i++;
    }

    else if ((jsoneq(json_string, &t[i], "humidity") == 0) && firstDataSet) {
      getValInt(&data->humidity, json_string, &t[i + 1]);
      i++;
    }

    else if ((jsoneq(json_string, &t[i], "3h") == 0) && firstDataSet) {
      getValDouble(&data->rain, json_string, &t[i + 1]);
      rainPresent = true;
      i++;
    }

    else if (jsoneq(json_string, &t[i], "dt_txt") == 0) {
      getString(temporaryDate, json_string, &t[i + 1]);

      if (dataDay)
      {
        if (temporaryDate[12] == tempMaxClock || firstDataSet)
        {
          do
          {
            if (temporaryTempMax >= 0)
            {
              if (temporaryTempMax >= (int)temporaryTempMax + 0.5)
              {
                data->temperature[clockCnt] = (int)temporaryTempMax + 1;
              }
              else
              {
                data->temperature[clockCnt] = (int)temporaryTempMax;
              }
            }
            else
            {
              if (temporaryTempMax < (int)temporaryTempMax - 0.5)
              {
                data->temperature[clockCnt] = (int)temporaryTempMax - 1;
              }
              else
              {
                data->temperature[clockCnt] = (int)temporaryTempMax;
              }
            }

            if (temporaryWindeSpeed >= (int)temporaryWindeSpeed + 0.5)
            {
              data->wind[clockCnt] = (int)temporaryWindeSpeed + 1;
            }
            else
            {
              data->wind[clockCnt] = (int)temporaryWindeSpeed;
            }

            memcpy(data->dates[clockCnt], &temporaryDate, sizeof(temporaryDate));
            memcpy(data->icon[clockCnt], &temporaryIcon, sizeof(temporaryIcon));

            clockCnt++;
          } while ((temporaryDate[12] == tempMaxClock) && (firstDataSet) && clockCnt < 2);
          firstDataSet = false;
        }
      }
      else
      {
        if (temporaryDate[12] == tempMinClock || firstDataSet)
        {
          do
          {
            if (temporaryTempMax >= 0)
            {
              if (temporaryTempMax >= (int)temporaryTempMax + 0.5)
              {
                data->temperature[clockCnt] = (int)temporaryTempMax + 1;
              }
              else
              {
                data->temperature[clockCnt] = (int)temporaryTempMax;
              }
            }
            else
            {
              if (temporaryTempMax < (int)temporaryTempMax - 0.5)
              {
                data->temperature[clockCnt] = (int)temporaryTempMax - 1;
              }
              else
              {
                data->temperature[clockCnt] = (int)temporaryTempMax;
              }
            }

            if (temporaryWindeSpeed >= (int)temporaryWindeSpeed + 0.5)
            {
              data->wind[clockCnt] = (int)temporaryWindeSpeed + 1;
            }
            else
            {
              data->wind[clockCnt] = (int)temporaryWindeSpeed;
            }

            memcpy(data->dates[clockCnt], &temporaryDate, sizeof(temporaryDate));
            memcpy(data->icon[clockCnt], &temporaryIcon, sizeof(temporaryIcon));

            clockCnt++;
          } while ((temporaryDate[12] == tempMinClock) && (firstDataSet) && clockCnt < 2);
          firstDataSet = false;
        }
      }
    }
  }
  if (!rainPresent)
  {
    data->rain = 0;
  }
  /* deallocating the memory*/
  vPortFree(t);
  return EXIT_SUCCESS;
}

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
      }
  return -1;
}

void getString(char *destination, const char *json_string, jsmntok_t *tok)
{
  int length = tok->end - tok->start;
  memcpy(destination, &json_string[tok->start], length);
  destination[length] = '\0';
}

void getValDouble(double *destination, const char *json_string, jsmntok_t *tok)
{
  char tmp[20];
  int length = tok->end - tok->start;
  memcpy(tmp, &json_string[tok->start], length);
  tmp[length] = '\0';

  *destination = atof(tmp);
}

void getValInt(int *destination, const char *json_string, jsmntok_t *tok)
{
  char tmp[20];
  int length = tok->end - tok->start;
  memcpy(tmp, &json_string[tok->start], length);
  tmp[length] = '\0';

  *destination = atoi(tmp);
}

int extractDateTime(char *json_string, char* date_time, char* day_of_week, int *unixtime)
{
  int i;
  int r;
  int token_size = 0;
  jsmn_parser p;

  jsmn_init(&p);

  /* Passing NULL instead of the tokens array would not store parsing results,
  but instead the function will return the value of tokens needed to parse the given string.
  This can be useful if you don’t know yet how many tokens to allocate. */
  token_size = jsmn_parse(&p, json_string, strlen(json_string), NULL, 0);

  t = (jsmntok_t*) pvPortMalloc(token_size * sizeof(jsmntok_t));

  jsmn_init(&p);

  r = jsmn_parse(&p, json_string, strlen(json_string), t, token_size);

  if (r < 0)
  {
    return 1;
  }

  /* Assume the top-level element is an object */
  if (r < 1 || t[0].type != JSMN_OBJECT)
  {
    return 1;
  }

  /* Loop over all keys of the root object */
  for (i = 1; i < r; i++) {
    if (jsoneq(json_string, &t[i], "utc_datetime") == 0) {
      /* We may use strndup() to fetch string value */
      sprintf(date_time ,"%.*s", t[i + 1].end - t[i + 1].start,
              json_string + t[i + 1].start);
      i++;
    }

    if (jsoneq(json_string, &t[i], "day_of_week") == 0) {
      /* We may use strndup() to fetch string value */
      sprintf(day_of_week ,"%.*s", t[i + 1].end - t[i + 1].start,
              json_string + t[i + 1].start);
      i++;
    }

    if (jsoneq(json_string, &t[i], "unixtime") == 0){
      getValInt(unixtime, json_string, &t[i + 1]);
      i++;
    }
  }
  /* deallocating the memory*/
  vPortFree(t);
  return 0;
}
