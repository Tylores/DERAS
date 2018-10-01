#ifndef _TSU_H_INCLUDED_
#define _TSU_H_INCLUDED_

#include <iostream>     // cin, cout
#include <fstream>
#include <sstream>
#include <string>       // getline, stoi, stod
#include <ctime>        // strftime, time_t, tm

namespace tsu {

template <typename T>
static std::string ToString (T t_value) {
    std::ostringstream ss;
    ss << t_value;
    return ss.str();
} // end ToString

// Get Time
static std::string GetTime () {
    time_t now = time(0);
    struct tm ts = *localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%T", &ts);
    return tsu::ToString(buf);
} // end Get Time

// Get Date
static std::string GetDate () {
    time_t now = time(0);
    struct tm ts = *localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%F", &ts);
    return ToString(buf);
} // end Get Date

// Get Date Time
static std::string GetDateTime () {
    time_t now = time(0);
    struct tm ts = *localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%F %T", &ts);
    return tsu::ToString(buf);
} // end Get Date Time


// Get Seconds
static unsigned int GetSeconds () {
    time_t now = time(0); 	// store current time
    struct tm time = *localtime(&now);
    return time.tm_sec;
}  // end Get Seconds

// Get Minutes
static unsigned int GetMinutes () {
    time_t now = time(0); 	// store current time
    struct tm time = *localtime(&now);
    return time.tm_min;
}  // end Get Minutes

// Get Hours
static unsigned int GetHours () {
    time_t now = time(0); 	// store current time
    struct tm time = *localtime(&now);
    return time.tm_hour;
}  // end Get Hours
};

#endif  // TSU_H_INCLUDED
