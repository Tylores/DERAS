#ifndef _TSU_H_INCLUDED_
#define _TSU_H_INCLUDED_

#include <iostream>     // cin, cout
#include <fstream>
#include <sstream>
#include <string>       // getline, stoi, stod
#include <vector>
#include <regex>
#include <ctime>        // strftime, time_t, tm
#include <map>

namespace tsu {

// map<section, map<property, value>>
// this is only for the INI file methods because it is huge
typedef std::map<std::string, std::map<std::string, std::string>> config_map;

// vector <vector <string>>
// this is used to create a matrix of strings
typedef std::vector <std::vector <std::string>> string_matrix;

template <typename T>
static std::string ToString (T t_value) {
    std::ostringstream ss;
    ss << t_value;
    return ss.str();
} // end ToString

// Get Time
// - the current time for viewing
static std::string GetTime () {
    time_t now = time(0);
    struct tm ts = *localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%T", &ts);
    return tsu::ToString(buf);
} // end Get Time

// Get Date
// - the current date for viewing
static std::string GetDate () {
    time_t now = time(0);
    struct tm ts = *localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%F", &ts);
    return ToString(buf);
} // end Get Date

// Get Date Time
// - the current date and time for viewing
static std::string GetDateTime () {
    time_t now = time(0);
    struct tm ts = *localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%F %T", &ts);
    return tsu::ToString(buf);
} // end Get Date Time


// Get Seconds
// - the current seconds to use in an operation
static unsigned int GetSeconds () {
    time_t now = time(0);   // store current time
    struct tm time = *localtime(&now);
    return time.tm_sec;
}  // end Get Seconds

// Get Minutes
// - the current minutes to use in an operation
static unsigned int GetMinutes () {
    time_t now = time(0);   // store current time
    struct tm time = *localtime(&now);
    return time.tm_min;
}  // end Get Minutes

// Get Hours
// - the current minutes to use in an operation
static unsigned int GetHours () {
    time_t now = time(0);   // store current time
    struct tm time = *localtime(&now);
    return time.tm_hour;
}  // end Get Hours

// Get Hours
// - the current seconds since epoch to use in an operation
static unsigned int GetUTC () {
    return time(0);   // current time since epoch
}  // end Get Hours

// FileToString
// - this method reads the entire file into a single string
// - it is more efficient to pull the file into memory than parse lines in file
static std::string FileToString (const std::string& kFilename) {
    if (std::ifstream file{kFilename, std::ios::binary | std::ios::ate}) {
        auto buffer = file.tellg();
        std::string str(buffer, '\0');  // construct string to stream size
        file.seekg(0);
        if (file.read(&str[0], buffer)) {
            return str;
        } else {
            printf("[ERROR]...reading file!");
            return NULL;
        }
    } else {
        printf("[ERROR]...file not found!");
        return NULL;
    }
} // end FileToString

// MapConfigFile
// - this method uses regular expressions to create a config map to
//   initialize class members or define program settings.
// - https://en.wikipedia.org/wiki/Regular_expression
static config_map MapConfigFile (const std::string& kFilename) {
    // read file into a string
    config_map file_map;
    std::string file_string, line, section, property, value;
    file_string = FileToString(kFilename);

    // create section and property regular expressions
    std::regex section_regex("^\\[(.*)\\]");
    std::regex property_regex("^(\\w+)=([^\\+]+(?!\\+{3}))");
    std::smatch match_regex;

    // split long string by each new line
    std::stringstream ss(file_string);
    while (std::getline(ss, line)) {
        // first look for section then property
        if (std::regex_search(line, match_regex, section_regex)) {
            section = match_regex[1];
        } else if (std::regex_search(line, match_regex, property_regex)){
            property = match_regex[1];
            value = match_regex[2];
            file_map[section][property] = value;
        }
    }
    return file_map;
} // end MapConfigFile

// CountDelimiter
// - count number of delimiters within string to make creating vectors and
// - matrices more efficient
static double CountDelimiter (const std::string& kString,
                              const char kDelimiter) {
    std::string line, item;
    double ctr = 0;
    std::stringstream ss(kString);
    while (std::getline(ss, line)) {
        std::stringstream ss2(line);
        while (std::getline(ss2, item, kDelimiter)) {
            ctr++;
        }
    }
    return ctr;
} // end CountDelimiter

// SplitString
// - split string given delimiter
// - operates on vector passed as a reference to reduce memory
static std::vector<std::string> SplitString (const std::string& kString,
                                             const char& kDelimiter) {
    std::vector<std::string> split_string;
    std::string line, item;
    double items = tsu::CountDelimiter(kString, kDelimiter);
    split_string.reserve(items);
    std::stringstream ss(kString);
    while (std::getline(ss, line)) {
        std::stringstream ss2(line);
        while (std::getline(ss2, item, kDelimiter)) {
            split_string.emplace_back(item);
        }
    }
    return split_string;
} // end SplitString

// FileToVector
// - open file and move to end for buffer size
// - read file into allocated string
// - parse string for delimiter
static std::vector<std::string> FileToVector (const std::string& kFilename,
                                              const char& kDelimiter) {
    if (std::ifstream file{kFilename, std::ios::binary | std::ios::ate}) {
        auto size = file.tellg();
        std::string str(size, '\0');  // construct string to stream size
        file.seekg(0);
        if (file.read(&str[0], size)) {
            return tsu::SplitString(str, kDelimiter);
        } else {
            std::printf("[ERROR]...reading file!");
            throw std::runtime_error("Error");
        }
    } else {
        std::printf("[ERROR]...file not found!");
        throw std::runtime_error("Error");
    }
    printf("[ERROR]...what did you even do!");
    throw std::runtime_error("Error");
} // end FileToVector

// File To Matrix
// - convert file to a vector of rows and then convert rows to a
// - vector of vectors of columns.
static string_matrix FileToMatrix (
    const std::string &kFilename,
    char kDelimiter,
    unsigned int columns) {
    std::vector <std::string> file_vector;
    file_vector = FileToVector (kFilename, kDelimiter);

    // determine rows in matrix
    unsigned int count = file_vector.size();
    unsigned int rows = count/columns;

    // initialize matrix for rows/cols
    string_matrix matrix (rows, std::vector <std::string> (columns, ""));

    // iterate through file_vector to populate matrix
    unsigned int cnt = 0;
    for (auto &row : matrix) {
        for (auto &col : row) {
            col = file_vector[cnt];
            cnt++;
        }
    }
    return matrix;
}

};

#endif  // TSU_H_INCLUDED
