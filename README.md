# C++ class LibCpp::cTime representing Unix time

Published under MIT License  
  
Content is:  
*src/cTime.h*  
*src/cTimeStd.cpp*   
  
This code provides the class 'cTime' within the namespace 'LibCpp'. This class simplifies the
usage of date and time compared to the functions defined in 'time.h' and is based on those
functions, only.  
Each cTime instance stores the time as unix time, which are the seconds after 1.1.1970 00:00:00. This integer value is the only member variable of the object.  
Basically you have access to the value itself or to a calendar representation. The term 'calendar' within this documentation always means calendar and time information in the human understandable form of year, hour, minute and so on.

## Code

    #include "cTime.h"
    using namespace LibCpp;

    int main()
    {
        cTime  timeNow = cTime::now();
        time_t value = timeNow.time();          // Unix time representation
        stCalendar cal = timeNow.calendar();    // Calendar time representation
        printf("Unix time is: %d\n", (int)value);
        printf("Calendar  is: %s\n", cTime::toString(cal).c_str());

        // Unix time is: 1695223058
        // Calendar  is: 2023-09-20#17:17:38#DST#+01:00
    }

The calendar is denoted in the 'Geographic Zone Calendar (GZC) string'.  
The term 'DST' means 'daylight saving time (DST)'. Possibly the term 'STD' for standard time appears. In both cases '+01:00' stands for the geographic time zone the calendar data is given for. It is important to distinguish between the geographic time zone and the relative Deviation of the given time to the Coordinated Universal Time (UTC) (same as Greenwich Mean Time (GMT)).  
It is a deliberate decision not to use the ISO8601 time format for the calendar string. The
reason is the lack of ability to express the DST, which is of importance for a meaningful understanding of the date and time imformation. (C++ decided the same within the 'struct tm' within 'time.h'.)  
To receive the relative Deviation from UTC (or UTC time offset), which is indicated by the DST value of -1 within the stCalendar struct, you can call:

## Code

    cal = timeNow.calendar(cTime::asUTC);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());

    // Calendar  is: 2023-09-20#17:17:38#UTC#+02:00

It is also possible to "translate" date and time to another UTC relative time. (It is not possible
to translate to other geographic time zones as this would require DST-infomation to correctly indicate the time. This is due to DST-information not being dependent on time zones but on specific countries.  
For example to UTC calendar.

## Code

    cal = timeNow.calendar(cTime::UTC);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
    // Calendar  is: 2023-09-20#15:17:38#UTC#+00:00

    int8_t utcOffset = 5;
    cal = timeNow.calendar(&utcOffset);
    printf("Calendar  is: %s\n", cTime::toString(cal).c_str());
    // Calendar  is: 2023-09-20#20:17:38#UTC#+05:00

It is also possible to calculate with cTime instances. First, there
are quite a few static cTime::set() methods to create a cTime instance
from different input values. Please refer to cTimeStd.cpp for complete
information. We will set christmas day and continue to calculate the
time duration we will have to wait till christmas day. As it doesn't make
sence to interprete a time difference as a calendar date, we can
represent a cTime instance as a time duration stored in a stDuration
struct containing days, hours, minutes and so on. Using years or months
doesn't make sence in a scientific calculation as these values have
not constant durations. Take notice of how to access the year of our
current time 'timeNow'.

## Code
    cTime timeChristmas = cTime::set(timeNow.calendar().year, 12, 24, 18, 0, 0);
    cTime timeWait = timeChristmas - timeNow;
    stDuration duration = timeWait.duration();
    printf("The wait time is: %s\n", cTime::toString(duration).c_str());
    // The wait time is: D95#00:42:22

The 'D' indicates a duration, the time to wait is 95 days, 42 minutes and 22 seconds.
There are two more static methods to point out concerning the time zone and UTC time offset calculation. Further more string conversion options.

## Code

    printf("local time zone is: %+03d:00\n", (int)cTime::localTimeZone());
    printf("UTC deviation is  : %+03d:00\n", (int)cTime::UTCdeviation(timeNow.calendar()));
    cTime  time = cTime::set("2023-09-20#17:17:38#DST#+01:00");
    printf("Calendar is       : %s\n", time.toString().c_str());
    printf("Weekday is        : %s\n", weekdays[time.calendar().dayInWeek]);
    printf("Wait time is      : %s\n", timeWait.toDurationString().c_str());

    // local time zone is: +01:00
    // UTC deviation is  : +02:00
    // Calendar is       : 2023-09-20#20:17:38#DST#+01:00
    // Weekday is        : Wednesday
    // Wait time is      : D95#00:42:22

For further available methods refer to \ref cTime.h.
