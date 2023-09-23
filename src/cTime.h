// utf-8 (ü)
/**
 * @file   cTime.h
 * @author Olaf Simon
 * @date   20.09.2023
 * @brief  Class LibCpp::cTime
 *
 * \addtogroup LibCpp_time
 * @{
**/

#ifndef cTime_H
#define cTime_H

#define LibCpp_isLeapYear(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0) ///< Checks a year being a leap year

#include <time.h>
#include <string>
#include <stdint.h>

namespace LibCpp
{

typedef struct tm tm_t;             ///< tm_t

extern int8_t int8Zero;             ///< int8Zero
extern const tm_t tm_Ini;           ///< tm_Ini
extern int8_t int8_0x80;            ///< int8_0x80
extern const char* weekdays[8];     ///< weekdays
extern const char dstString[3][4];  ///< dstString

/**
 * @brief Calendar data being bijective convertible to UTC time.
 * Initialize with LibCpp::stCalendar_Ini.\n
**/
#pragma pack(1)
typedef struct _stCalendar
{
    int32_t  year;          ///< year   (subject to be swaped for big endian (Motorola) to little endian (IBM) conversion
    uint8_t  month;         ///< month 1-12
    uint8_t  day;           ///< day 1-31
    uint8_t  hour;          ///< hour 0-23
    uint8_t  minute;        ///< minute 0-59
    uint8_t  second;        ///< second 0-59
    int8_t   dst;           ///< daylight saving time 1=active 0=inaktive(standard) -1 unspecified (0 will be assumed as in this case geographycal and relative time zones are equal, see \ref cTime)
    int8_t   timeZone;      ///< time zone -12 to +12 hours as geographical time zone (anytime the standard time time-zone, see \ref cTime). Relative UTC time zone if dst = -1.
    int8_t   leapSecond;    ///< If a leap second occurs the unix-time stands still for one second. To indicate the second of standstill this value is set to 1, 0 otherwise (not implemented yet)
    uint32_t subZone;       ///< time zone 0 to +86.399 seconds, 0 if timeZone is -12 or +12 (not implemented yet)
    uint32_t picoSeconds;   ///< pico seconds after the specified second
    uint8_t  dayInWeek;     ///< 1-7 as 1 for monday
    int8_t   calendarWeek;  ///< full calendar weeks of the year (Monday to Sunday) 1-53, 0 for last week of previous year.
    int16_t  dayInYear;     ///< 1-366 as 1 for the 1st of January (int16 for 64 bit padding purpose)
} stCalendar;
#pragma pack()

extern const stCalendar stCalendar_Ini;     ///< stCalendar_Ini

/**
 * @brief Time duration as "calendar" data.
 * Initialize with LibCpp::stDuration_Ini.\n
 * Each entry can be as large as wanted. The resulting duration is the sum of all values.
**/
typedef struct _stDuration
{
    uint64_t days;      ///< amount of days
    uint64_t hours;     ///< amount of hours
    uint64_t minutes;   ///< amount of minutes
    uint64_t seconds;   ///< amount of seconds
    int8_t   sign;      ///< sign of the duration
} stDuration;

extern const stDuration stDuration_Ini; ///< stDuration_Ini

/**
 * @brief C++ class for time representation and calculations
**/
class cTime
{
public:
    static constexpr int8_t* UTC = &int8Zero;   ///< Pointer to a value of zero to indicate UTC reference time zone (GMT).
    static constexpr int8_t* asUTC = &int8_0x80;///< Pointer to a value of 0x80 to indicate using the locally valid UTC relative time zone.

public:
    cTime();                                    ///< Constructor.

    static cTime now();                         ///< Deliveres a cTime instance holding the current time.
    static cTime set(time_t unixTime);          ///< Deliveres a cTime instance initialized with unixTime.
    static cTime set(stCalendar calendar);      ///< Deliveres a cTime instance initialized with the calendar data input.
    static cTime set(stDuration timeDuration);  ///< Deliveres a cTime instance initialized with the given time duration information.
    static cTime set(int32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second); ///< Deliveres a cTime instance representing the given calendar data according to the local clock configuration.
    static cTime set(uint64_t days, uint64_t hours, uint64_t minutes, uint64_t seconds, int8_t sign = 1); ///< Deliveres a cTime instance representing the given duration data.
    static cTime set(std::string text);         ///< Deliveres a cTime instance initialized by a string representing either a calendar or a duration string.

    time_t     time();                          ///< Returns the unix time stamp (seconds till 1.1.1970 00:00:00 GMT).
    stCalendar calendar(int8_t* pRequestedTimeZone = nullptr);  ///< Returns the calendar data representation of the instance. A UTC time deviation can be chosen.
    stDuration duration();                      ///< Returns the internal unix time value as duration information.

    std::string toString(int8_t* pRequestedTimeZone = nullptr);  ///< Returns a string interpretation of the 'calendar' method result
    std::string toDurationString();             ///< Returns a string representing a duration format

    inline bool operator==(const cTime& a) { return _time == a._time; }             ///< operator ==
    inline bool operator!=(const cTime& a) { return _time != a._time; }             ///< operator !=
    inline bool operator< (const cTime& a) { return _time < a._time; }              ///< operator <
    inline bool operator> (const cTime& a) { return _time > a._time; }              ///< operator >
    inline bool operator<=(const cTime& a) { return _time <= a._time; }             ///< operator <=
    inline bool operator>=(const cTime& a) { return _time >= a._time; }             ///< operator >=
    inline cTime operator+(const cTime& a) { return cTime::set(_time + a._time); }  ///< operator +
    inline cTime operator-(const cTime& a) { return cTime::set(_time - a._time); }  ///< operator -
    inline cTime operator+=(const cTime& a) { return cTime::set(_time + a._time); } ///< operator +=
    inline cTime operator-=(const cTime& a) { return cTime::set(_time - a._time); } ///< operator -=

    static int8_t localTimeZone();                                      ///< Retrieves the local geographic time zone.
    static int8_t UTCdeviation(stCalendar calendar);                    ///< Calculates the relative deviation of UTC (GMT) time.

    static stCalendar  getCalendar(int32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second); ///< Deliveres a stCalendar struct representing the given calendar data using the local geographic time zone
    static stDuration  getDuration(uint64_t day, uint64_t hour, uint64_t minute, uint64_t second, int8_t sign = 1);         ///< Deliveres a stCalendar struct representing the given calendar data using the local geographic time zone
    static stCalendar  fromString(std::string dateString);               ///< Delivers a calendar struct from a GZC string
    static stDuration  fromDurationString(std::string durationString);   ///< Delivers a duration struct from a GZC string
    static std::string toString(stCalendar calendar);                    ///< Generates a GZC string from a calendar struct
    static std::string toString(stDuration duration);                    ///< Generates a GZC string from a duration struct

private:
    time_t _time;           ///< System (original) Unix / UTC time in seconds since 1.1.1970 00:00:00 Greenwich mean time
};

}
#endif // cTime_H

/** @} */
