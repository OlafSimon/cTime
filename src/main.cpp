#include "LibCpp/Time/cTime.h"

using namespace LibCpp;
using namespace std;

int main()
{
    printf("------- Usage of strptime / strftime ---------\n");
    char timeStr[64];
    struct tm tp;

    time_t now = time(0);
    localtime_s(&tp,&now);
    stTimeZone localZone = LibOb_localTimeZone(0);
    LibOb_strftime(timeStr, 64, 0, &tp, &localZone, 0);
    printf("Current time is: %s\n", timeStr);
    LibOb_strftime(timeStr, 64, "%A %Y-%m-%d %H:%M:%S %Z", &tp, &localZone, 0);
    printf("or formatted: %s\n", timeStr);

    //string dateString = "2022-9-15";
    //string dateString = "15:32:10";
    //string dateString = "15:32:10DST";
    //string dateString = "15:32:10 DST";
    //string dateString = "15:32:10 DST +02:00";
    //string dateString = "2007-08-31T16:47+01:45";
    //string dateString = "2007-08-31#16:47:36#UTC#+01:00";
    //string dateString = "03/15/2023";
    //string dateString = "1.2.2024";
    //string dateString = "8:23 Uhr am 1. 2. 2024 CEST";
    //string dateString = "1. Februar 2024 um 10:11Uhr";
    //string dateString = "May 15th 2019";

    const char* dateString = "May 15, 2019";
    printf("Test input: %s\n", dateString);
    LibOb_strptime(dateString, 0, &tp, &localZone);
    printf("Numerical calendar data: year %d, month %d, day %d\n", (int)tp.tm_year+1900, (int)tp.tm_mon+1, (int)tp.tm_mday);
    fflush(stdout);

    printf("------- Usage of cTime: Representations ---------\n");

    cTime  timeNow = cTime::now();          // cTime::set("20.09.2023 15:02:36 CEST");
    time_t value = timeNow.time();          // Unix time representation
    stCalendar cal = timeNow.calendar();    // Calendar time representation
    printf("Unix time is: %d\n", (int)value);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
    // Unix time is: 1695214956
    // Calendar  is: 2023-09-20#15:02:36#DST#+01:00

    printf("------- Usage of cTime: UTC relative calendar representation ---------\n");

    cal = timeNow.calendar(cTime::asUTC);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
    // Calendar  is: 2023-09-20#15:39:14#UTC#+02:00

    printf("------- Usage of cTime: Calendar representation within other time zones ---------\n");

    cal = timeNow.calendar(cTime::UTC);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
    // Calendar  is: 2023-09-20#15:17:38#UTC#+00:00
    int8_t utcOffset = 5;
    cal = timeNow.calendar(&utcOffset);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
    // Calendar  is: 2023-09-20#20:17:38#UTC#+05:00

    printf("------- Usage of cTime: Mathematical operations ---------\n");

    cTime timeChristmas = cTime::set(timeNow.calendar().year, 12, 24, 18, 0, 0);
    cTime timeWait = timeChristmas - timeNow;
    stDuration duration = timeWait.duration();
    printf("The wait time is: %s\n", cTime::toString(duration).c_str());
    // The wait time is: D95#00:42:22

    printf("------- Usage of cTime: Time zones and string conversions ---------\n");

    printf("local time zone is: %+03d:00\n", (int)cTime::localTimeZone().hours);
    printf("UTC deviation is  : %+03d:00\n", (int)cTime::UTCdeviation(timeNow.calendar().timeZone, timeNow.calendar().dst).hours);
    cTime  time = cTime::set("2023-09-20#17:17:38#DST#+01:00");
    printf("Calendar is       : %s\n", time.toString().c_str());
    printf("Wait time is      : %s\n", timeWait.toDurationString().c_str());
    // local time zone is: +01:00
    // UTC deviation is  : +02:00
    // Calendar is       : 2023-09-20#20:17:38#DST#+01:00
    // Weekday is        : Wednesday
    // Wait time is      : D95#00:42:22

    // UTC dates
    // 	1996604133: 2033-04-08 20:15:33 -> {2033, 4, 8, 20, 15, 33, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //  2180908800: 2039-02-10 00:00:00 -> {2039, 2, 10, 0, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //   950144523: 2000-02-10 01:02:03 -> {2000, 2, 10, 1, 2, 3, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //  1078880523: 2004-03-10 01:02:03 -> {2004, 3, 10, 1, 2, 3, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    // 	1072915199: 2003-12-31 23:59:59 -> {2003, 12, 31, 23, 59, 59, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //   978307200: 2001-01-01 00:00:00 -> {2001, 1, 1, 0, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //   978393600: 2001-01-02 00:00:00 -> {2001, 1, 2, 0, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //   978825600: 2001-01-07 00:00:00 -> {2001, 1, 7, 0, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //	1694790000: 2023-09-15 15:00:00 -> {2023, 9, 15, 15, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0}
    //	1009843200: 2002-01-01 00:00:00 -> {2002, 1, 1, 0, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0}

    cal = {2023, 9, 15, 15, 0, 0, -1, {0, 0}, 0, 0, 0, 0, 0, 0, 0};
    time = cTime::set(cal);
    printf("unix : %d : %s\n", (int)time.time(), time.toString().c_str());

    fflush(stdout);

    return 0;
}
