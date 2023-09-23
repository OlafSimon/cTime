// utf-8 (ü)

// MIT License
// Copyright © 2023 Olaf Simon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the “Software”), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/**
 * @file   cTimeStd.cpp
 * @author Olaf Simon
 * @date   20.09.2023
 * @brief  Class LibCpp::cTime
 *
 * \addtogroup LibCpp_time
 * @{
 *
 * \class LibCpp::cTime
 *
 * Published under MIT License (see cTimeStd.cpp)
 *
 * This code provides the class 'cTime' within the namespace 'LibCpp'. This class simplifies the
 * usage of date and time compared to the functions defined in 'time.h' and is based on those
 * functions, only.
 *
 * Each cTime instance stores the time as unix time, which are the seconds after 1.1.1970 00:00:00.
 * This integer value is the only member variable of the object.\n
 * Basically you have access to the value itself or to a calendar representation. The term 'calendar'
 * within this documentation always means calendar and time information in the human understandable form of
 * year, hour, minute and so on.
 *
 * \code
 * #include "cTime.h"
 *
 * using namespace LibCpp;
 *
 * int main()
 * {
 *     cTime  timeNow = cTime::now();
 *     time_t value = timeNow.time();          // Unix time representation
 *     stCalendar cal = timeNow.calendar();    // Calendar time representation
 *     printf("Unix time is: %d\n", (int)value);
 *     printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
 *
 *     // Unix time is: 1695223058
 *     // Calendar  is: 2023-09-20#17:17:38#DST#+01:00
 * }
 * \endcode
 * The calendar is denoted in the 'Geographic Zone Calendar (GZC) string'.\n
 * The term 'DST' means daylight saving time (dst). Possibly the term 'STD' for standard time appears.
 * In both cases '+01:00' stands for the geographic time zone the calendar data is given for.
 * It is important to distinguish between the geographic time zone and the relative deviation
 * of the given time to the Coordinated Universal Time (UTC) (same as Greenwich Mean Time (GMT)).\n
 * It is a deliberate decision not to use the ISO8601 time format for the calendar string. The
 * reason is the lack of ability to express the dst which is of importance for a meaningful understanding
 * of the date and time imformation. (C++ decided the same within the 'struct tm' within 'time.h'.)\n
 * To receive the relative deviation (or UTC time offset), which is indicated by the dst value of -1
 * within the stCalendar struct, you can call:
 * \code
 *     cal = timeNow.calendar(cTime::asUTC);
 *     printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
 *     // Calendar  is: 2023-09-20#17:17:38#UTC#+02:00
 * \endcode
 * It is also possible to "translate" date and time to another UTC relative time. (It is not possible
 * to translate to other geographic time zones as this would require dst-infomation to correctly
 * show the time. This is due to dst-information not being dependent on time zones but on specific countries.)
 * For example to UTC calendar.
 * \code
 *     cal = timeNow.calendar(cTime::UTC);
 *     printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
 *     // Calendar  is: 2023-09-20#15:17:38#UTC#+00:00
 *     int8_t utcOffset = 5;
 *     cal = timeNow.calendar(&utcOffset);
 *     printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
 *     // Calendar  is: 2023-09-20#20:17:38#UTC#+05:00
 * \endcode
 * It is also possible to calculate with cTime instances. First, there
 * are quite a few static cTime::set() methods to create a cTime instance
 * from different input values. Please refer to \ref cTime.h for complete
 * information. We will set christmas day and continue to calculate the
 * time duration we will have to wait till christmas day. As it doesn't make
 * sence to interprete a time difference as a calendar date, we can
 * represent a cTime instance as a time duration stored in a stDuration
 * struct containing days, hours, minutes and so on. Using years or months
 * doen't make sence in a scientific calculation as these values have
 * not constant durations. Take notice of how to access the year of our
 * current time 'timeNow'.
 * \code
 *     cTime timeChristmas = cTime::set(timeNow.calendar().year, 12, 24, 18, 0, 0);
 *     cTime timeWait = timeChristmas - timeNow;
 *     stDuration duration = timeWait.duration();
 *     printf("The wait time is: %s\n", cTime::toString(duration).c_str());
 *     // The wait time is: D95#00:42:22
 * \endcode
 * The 'D' indicates a duration, the time to wait is 95 days, 42 minutes and 22 seconds.\n
 * There are two more static methods to point out concerning the time zone and UTC time offset
 * calculation. Further more string conversion options.
 * \code
 *     printf("local time zone is: %+03d:00\n", (int)cTime::localTimeZone());
 *     printf("UTC deviation is  : %+03d:00\n", (int)cTime::UTCdeviation(timeNow.calendar()));
 *     cTime  time = cTime::set("2023-09-20#17:17:38#DST#+01:00");
 *     printf("Calendar is       : %s\n", time.toString().c_str());
 *     printf("Weekday is        : %s\n", weekdays[time.calendar().dayInWeek]);
 *     printf("Wait time is      : %s\n", timeWait.toDurationString().c_str());
 *     // local time zone is: +01:00
 *     // UTC deviation is  : +02:00
 *     // Calendar is       : 2023-09-20#20:17:38#DST#+01:00
 *     // Weekday is        : Wednesday
 *     // Wait time is      : D95#00:42:22
 * \endcode
 * For further available methods refer to \ref cTime.h.
**/

#if defined(LibCpp_Y2038) || defined(LibCpp_Y2038TEST)
    #include "CalendarToTime.h"
#else
    #include "cTime.h"
#endif

#define LibCpp_SECONDSPERHOUR 3600  ///< Konstante

//! @cond Doxygen_Suppress
#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
//! @endcond

#include <cstring>

using namespace LibCpp;

int8_t LibCpp::int8Zero = 0;        ///< Value of zero to let cTime::UTC point to.
int8_t LibCpp::int8_0x80 = 0x80;    ///< Value of 0x80 to let cTime::UTCdeviation point to.
const tm_t LibCpp::tm_Ini = {0, 0, 0, 0, 0, 0, 0, 0, 0};  ///< Initializer for struct tm variables. The order of elements is undefined, thus tm_isdst cannot be initialized to -1
const stCalendar LibCpp::stCalendar_Ini = {0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0}; ///< Initializer for stCalendar variables.
const stDuration LibCpp::stDuration_Ini = {0, 0, 0, 0, 1};          ///< Initializer for stDuration
const char LibCpp::dstString[3][4] = {"UTC", "STD", "DST"};         ///< String representation for dst
const char* LibCpp::weekdays[8] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};  ///< String representation for weekdays                                          ///< String representations for weekdays

/**
 * @brief Constructor
 */
cTime::cTime()
{
    _time = 0;
}

/**
 * @brief Deliveres a cTime instance holding the current time.
 * @return Created instance
 */
cTime cTime::now()
{
    cTime result;
    result._time = ::time(nullptr);
    return result;
}

/**
 * @brief Deliveres a cTime instance initialized with unixTime.
 * @param unixTime
 * @return Created instance
 */
cTime cTime::set(time_t unixTime)
{
    cTime t;
    t._time = unixTime;
    return t;
}

/**
 * @brief Deliveres a cTime instance initialized with the calendar data input.
 * The calendar data is to be provided as /ref stCalendar struct.
 * @param calendar
 * @return Created instance
 */
cTime cTime::set(stCalendar calendar)
{
    struct tm tmCalendar = tm_Ini;
    tmCalendar.tm_year  = calendar.year - 1900;
    tmCalendar.tm_mon   = calendar.month - 1;
    tmCalendar.tm_mday  = calendar.day;
    tmCalendar.tm_hour  = calendar.hour;
    tmCalendar.tm_min   = calendar.minute;
    tmCalendar.tm_sec   = calendar.second;
    tmCalendar.tm_isdst = calendar.dst;
    time_t timeValue = mktime(&tmCalendar);
    int8_t localZone = localTimeZone();
    int dst = 0;
    if (tmCalendar.tm_isdst) dst=1;
    timeValue += (localZone + dst + calendar.timeZone) * LibCpp_SECONDSPERHOUR;
    return set(timeValue);
}

/**
 * @brief Deliveres a cTime instance initialized with the given time duration information.
 * The duration data is to be provided as /ref stDuration struct.
 * @param duration
 * @return Created instance
 */
cTime cTime::set(stDuration duration)
{
    time_t value = duration.days * 86400 + duration.hours * 3600 + duration.minutes * 60 + duration.seconds;
    if (duration.sign < 0) value = -value;
    return cTime::set(value);
}

/**
 * @brief Deliveres a cTime instance representing the given calendar data according to the local clock configuration.
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @param second
 * @return Created instance
 */
cTime cTime::set(int32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    stCalendar calendar = getCalendar(year, month, day, hour, minute, second);
    return cTime::set(calendar);
}

/**
 * @brief ///< Deliveres a cTime instance representing the given duration data.
 * @param days
 * @param hours
 * @param minutes
 * @param seconds
 * @param sign
 * @return Created instance
 */
cTime cTime::set(uint64_t days, uint64_t hours, uint64_t minutes, uint64_t seconds, int8_t sign)
{
    stDuration duration = stDuration_Ini;
    duration.days = days;
    duration.hours = hours;
    duration.minutes = minutes;
    duration.seconds = seconds;
    duration.sign = sign;
    return set(duration);
}

/**
 * @brief Deliveres a cTime instance initialized by a string representing either a calendar or a duration string.
 * @param text
 * @return Created instance
 */
cTime cTime::set(std::string text)
{
  if (text[0] == 'D')
  {
      stDuration duration = cTime::fromDurationString(text);
      return cTime::set(duration);
  }
  else
  {
      stCalendar calendar = cTime::fromString(text);
      return cTime::set(calendar);
  }
}

/**
 * @brief Returns the unix time stamp (seconds till 1.1.1970 00:00:00 GMT).
 * @return unix time
 */
time_t cTime::time()
{
    return _time;
}

/**
 * @brief Returns the calendar data representation of the instance.
 * Returns the memorized unix time as calendar data based on the local system clock configuration.
 * In case 'pRequestedTimeZone' is set and points to a int8_t variable containing the requested
 * UTC time deviation, the corresponding date and time is returned.\n
 * Use 'cTime::UTC' as parameter to receive the calendar data valid at UTC deviation zero.\n
 * Use 'cTime::asUTC' to keep the local date and time string but using UTC time deviation
 * instead of geographic time zone. This is useful for creating ISO8601 conform strings.
 * @param pRequestedTimeZone Pointer to a variable containing the desired UTC time deviation.
 * @return calendar struct
 */
stCalendar cTime::calendar(int8_t* pRequestedTimeZone)
{
    struct tm lt = tm_Ini;
    int8_t zone = localTimeZone();
    localtime_s(&lt, &_time);

    stCalendar calendar = stCalendar_Ini;

    if (pRequestedTimeZone)
    {
        int8_t relZone = zone;
        if (lt.tm_isdst>0)
            relZone += 1;

        int8_t relDestZone;
        if (*pRequestedTimeZone == (int8_t)0x80)
        {
            relDestZone = zone;
            if (lt.tm_isdst > 0) relDestZone++;
        }
        else
            relDestZone = *pRequestedTimeZone;
        lt.tm_hour += relDestZone - relZone;
        mktime(&lt);
        lt.tm_isdst = -1;
        calendar.timeZone = relDestZone;
    }
    else
        calendar.timeZone = zone;

    calendar.second    = lt.tm_sec;         // seconds after the minute	0-60*
    calendar.minute    = lt.tm_min;         // minutes after the hour	0-59
    calendar.hour      = lt.tm_hour;        // hours since midnight	0-23
    calendar.day       = lt.tm_mday;        // day of the month	1-31
    calendar.month     = lt.tm_mon	+ 1;    // months since January	0-11
    calendar.year      = lt.tm_year	+ 1900; // years since 1900
    calendar.dst       = lt.tm_isdst;       // Daylight Saving Time flag
    calendar.dayInWeek = lt.tm_wday;
    if (!calendar.dayInWeek) calendar.dayInWeek = 7;
    calendar.dayInYear = lt.tm_yday + 1;

    return calendar;
}

/**
 * @brief Returns the internal unix time value as duration information.
 * @return Struct \ref _stDuration
 */
stDuration cTime::duration()
{
    stDuration duration;
    time_t value = _time;
    duration.sign = 1;
    if (value<0)
    {
        duration.sign = -1;
        value = -value;
    }
    duration.days = value / 86400;
    value = value % 86400;
    duration.hours = value / 3600;
    value = value % 3600;
    duration.minutes = value / 60;
    duration.seconds = value % 60;
    return duration;
}

/**
 * @brief Deliveres a \ref _stCalendar struct representing the given calendar data using the local geographic time zone
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @param second
 * @return Created calendar struct
 */
stCalendar cTime::getCalendar(int32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    stCalendar calendar = stCalendar_Ini;

    struct tm tmCalendar = tm_Ini;
    tmCalendar.tm_year  = year - 1900;
    tmCalendar.tm_mon   = month - 1;
    tmCalendar.tm_mday  = day;
    tmCalendar.tm_hour  = hour;
    tmCalendar.tm_min   = minute;
    tmCalendar.tm_sec   = second;
    tmCalendar.tm_isdst = -1;
    mktime(&tmCalendar);    // generates tm_isdst
    int8_t localZone = localTimeZone();
    calendar.dst      = tmCalendar.tm_isdst;
    calendar.timeZone = localZone;
    calendar.year     = year;
    calendar.month    = month;
    calendar.day      = day;
    calendar.hour     = hour;
    calendar.minute   = minute;
    calendar.second   = second;
    calendar.dayInWeek = tmCalendar.tm_wday;
    if (calendar.dayInWeek == 0) calendar.dayInWeek = 7;
    calendar.dayInYear = tmCalendar.tm_yday + 1;
    return calendar;
}

/**
 * @brief Returns the local time zone according to system clock settings
 * @return Geographic time zone.
 */
int8_t cTime::localTimeZone()
{
    time_t time = ::time(nullptr);
    struct tm gt = tm_Ini;
    struct tm lt = tm_Ini;
    gt.tm_isdst = 0;
    lt.tm_isdst = -1;
    gmtime_s(&gt, &time);
    localtime_s(&lt, &time);
    time_t gtm = mktime(&gt);
    time_t ltm = mktime(&lt);
    int8_t zone = (uint8_t)(((ltm-gtm)+LibCpp_SECONDSPERHOUR/2) / LibCpp_SECONDSPERHOUR);
    if (gt.tm_isdst>0) zone -= 1;
    if (lt.tm_isdst>0) zone += 1;
    return zone;
}

/**
 * @brief Calculates the relative deviation of UTC (GMT) time.
 * @param calendar
 * @return
 */
int8_t cTime::UTCdeviation(stCalendar calendar)
{
    int8_t result = calendar.timeZone;
    if (calendar.dst > 0 ) result++;
    return result;
}

/**
 * @brief Delivers a calendar struct from a GZC string
 * @param dateString string
 * @return
 */
stCalendar cTime::fromString(std::string dateString)
{
    stCalendar calendar = stCalendar_Ini;
    if (dateString[0] == 'D') return calendar;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    char dst[4];
    int zone = 0;
    sscanf_s(dateString.c_str(), "%d-%d-%d#%d:%d:%d#%3*s#%d:00", &year, &month, &day, &hour, &minute, &second, &zone);
    dst[0] = dateString.c_str()[20];
    dst[1] = dateString.c_str()[21];
    dst[2] = dateString.c_str()[22];
    dst[3] = 0;
    if (strcmp(dst, "STD") == 0)
        calendar.dst = 0;
    if (strcmp(dst, "DST") == 0)
        calendar.dst = 1;
    else
        calendar.dst = -1;
    calendar.year = (int32_t)year;
    calendar.month = (uint8_t)month;
    calendar.day = (uint8_t)day;
    calendar.hour = (uint8_t)hour;
    calendar.minute = (uint8_t)minute;
    calendar.second = (uint8_t)second;
    calendar.timeZone = (int8_t)zone;
    return calendar;
}

/**
 * @brief Delivers a duration struct from a GZC string
 * A sample string is: D95#00:42:22
 * @param durationString
 * @return
 */
stDuration cTime::fromDurationString(std::string durationString)
{
    stDuration duration = stDuration_Ini;
    if (durationString[0] != 'D') return duration;
    int days = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    sscanf_s(durationString.c_str(), "D%d#%d:%d:%d", &days, &hours, &minutes, &seconds);
    duration.sign = 1;
    if (days < 0)
    {
        duration.sign = (int8_t)-1;
        days = -days;
    }
    duration.days = (uint64_t)days;
    duration.hours = (uint64_t)hours;
    duration.minutes = (uint64_t)minutes;
    duration.seconds = (uint64_t)seconds;
    return duration;
}

/**
 * @brief Returns a string interpretation of the 'calendar' method result
 * See /ref calendar .
 * @param pRequestedTimeZone
 * @return
 */
std::string cTime::toString(int8_t* pRequestedTimeZone)
{
    stCalendar cal = calendar(pRequestedTimeZone);
    return cTime::toString(cal);
}

/**
 * @brief Returns a string representing a duration format
 * @return
 */
std::string cTime::toDurationString()
{
    stDuration dur = duration();
    return toString(dur);
}

/**
 * @brief Generates a GZC string from a calendar struct
 * @param calendar
 * @return
 */
std::string cTime::toString(stCalendar calendar)
{
    std::string str;
    char buffer[32];
    int index = calendar.dst + 1;
    if (index<0 || index>2) index = 0;

    sprintf_s(buffer, 32, "%04d-%02d-%02d#%02d:%02d:%02d#%s#%+03d:00", (int)calendar.year, (int)calendar.month, (int)calendar.day, (int)calendar.hour, (int)calendar.minute, (int)calendar.second, dstString[index], (int)calendar.timeZone);
    std::string result(buffer);
    return result;
}

/**
 * @brief Generates a GZC string from a duration struct
 * @param duration
 * @return
 */
std::string cTime::toString(stDuration duration)
{
    std::string str;
    char buffer[32];
    int days = (int)duration.days;
    if (duration.sign < 0) days = -days;
    sprintf_s(buffer, 32, "D%d#%02d:%02d:%02d", days, (int)duration.hours, (int)duration.minutes, (int)duration.seconds);
    std::string result(buffer);
    return result;
}

/** @} */
