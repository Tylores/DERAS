// Author: Tylor Slay
// Description: These are functions I have build using lots of other resources to
// help with importing, export, and manipulating data. I have tried to follow
// Google's coding standard to make things easier to read and understand.
//
// Google C++ coding standard
// --------------------------
// https://google.github.io/styleguide/cppguide.html
//
// General rules:
// - Indents are two spaces. No tabs should be used anywhere.
// - Each line must be at most 80 characters long.
// - Comments can be // or /* but // is most commonly used.
// - Avoid Abbreviations
// - Type/Function Names: CamelCase
// - Variable Names: lowercase_underscores
// - Class Data Members: trailing_underscores_
// - Constant Names: kCamelCase
// - Enumerator/Macro Names: ALL_CAPS
//
// Note: Please reference http://en.cppreference.com/w/cpp for additional help

#ifndef TS_UTILITY_HPP_INCLUDED
#define TS_UTILITY_HPP_INCLUDED

// INCLUDE
#include <iostream>     // cin, cout
#include <cstdio>       // printf
#include <fstream>      // ofstream, ifstream
#include <sstream>      // stringstream, istringstream
#include <vector>       // reserve, emplace_back
#include <string>       // getline, stoi, stod
#include <algorithm>    // sort
#include <typeinfo>     // typeid
#include <ctime>        // strftime, time_t, tm
#include <regex>        // regex_search
#include <map>          // find
#include <ctime>        // strftime, time_t, tm

// tylor slay utilities (tsu)
namespace tsu {

// map<section, map<property, value>>
// this is only for the INI file methods because it is huge
typedef std::map<std::string, std::map<std::string, std::string>> config_map;

// Print
// - simple function to write to terminal
// - a template is used to abstract the data type
template <typename T>
static void Print (const T& kOutput) {
    std::cout << kOutput << std::endl;
} // end Print

// ToString
// - converts any value to a string
template <typename T>
static std::string ToString (const T& kValue) {
    std::ostringstream ss;
    ss << kValue;
    return ss.str();
} // end ToString

// ToNumber
// - converts string to desired number type else return zero
template <typename T>
static T StringToNumber (const std::string kString, T& number) {

    std::string type = typeid(number).name();
    // this try-catch block will strip non number values
    try {
        number = std::stod(kString);
    } catch (...) {
        return 0;
    }

    // compare data type and set string to number
    if (!kString.empty()) {
        if (type == "j"){
            number = std::stoul(kString);
        } else if (type == "d") {
            number = std::stod(kString);
        } else if (type == "i") {
            number = std::stoi(kString);
        }
    }
    return number;
} // end ToNumber

// EpochToReadable
// * convert UTC time in seconds to date/time readable
static std::string EpochToReadable (const unsigned int kEpoch) {
    time_t time_utc = kEpoch;
    struct tm ts;
    char buf[80];
    ts = *localtime(&time_utc);
    strftime(buf, sizeof(buf), "%D %T", &ts);
    std::string date_time = buf;
    return date_time;
} // end EpochToReadable

// EpochToReadable
// * convert UTC time in seconds to date/time readable
static unsigned int EpochToMinutes(const unsigned int kEpoch) {
    unsigned int minutes;
    time_t time_utc = kEpoch;
    struct tm ts;
    char buf[80];
    ts = *localtime(&time_utc);
    strftime(buf, sizeof(buf), "%H", &ts);
    minutes = std::stoul(buf)*60;
    strftime(buf, sizeof(buf), "%M", &ts);
    minutes = std::stoul(buf) + minutes;
    return minutes;
} // end EpochToReadable

static std::string MinutesToTime (const unsigned int minutes) {
    unsigned int H, M;
    M = minutes % 60;
    H = (minutes-M) / 60;
    std::stringstream ss;
    if (H < 10) {
        ss << "0" << H << ":";
    } else {
        ss << H << ":";
    }
    if (M < 10) {
        ss << "0" << M << ":00";
    } else {
        ss << M << ":00";
    }
    return ss.str();
} // end MinutesToTime

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

// GetSectionProperty
// - this is a method that makes accessing the config map more readable
static std::string GetSectionProperty (const config_map& kMap,
                                       const std::string& kSection,
                                       const std::string& kProperty) {
    auto section_it = kMap.find(kSection);
    if (section_it != kMap.end() ) {
       auto& property_map = section_it->second;
       auto property_it = property_map.find(kProperty);
       if (property_it != property_map.end()) {
            return property_it->second;
       }
    }
    printf("[ERROR] at <%s,%s>\n", kSection.c_str(), kProperty.c_str());
    return NULL;
} // end GetSectionProperty

// AverageVector
// - return the average of a vector
template <typename T>
static int AverageVector (const std::vector<T>& kVector) {
    float sum = 0;
    unsigned int items = kVector.size();
    for (unsigned int i = 0; i < items; i++) {
        sum = sum + kVector[i];
    }
    return sum/items;
} // end AverageVector

// PrintVector
template <typename T>
static void PrintVector (const std::vector<T>& kVector) {
    unsigned int kSize = kVector.size();
    for (unsigned int i = 0; i < kSize; i++){
        std::cout << i << "\t" << kVector[i] << '\n';
    }
    std::cout << std::endl;  // inserts newline and flushes stdin
} // end PrintVector

// PrintMatrix
template <typename T>
static void PrintMatrix (const std::vector<std::vector<T>>& kMatrix) {
    unsigned int rows = kMatrix.size();
    unsigned int columns = kMatrix[0].size();
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < columns; j++) {
            std::cout << ToString(kMatrix[i][j]) << "\t";
        }
        std::cout << '\n';
    }
    std::cout << std::endl;  // inserts newline and flushes stdin
} // end PrintMatrix

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

// VectorToFile
// * open file and write simple vector to file
template <typename T>
static void VectorToFile (const std::string kFilename,
                          const std::vector<T>& kVector) {
    unsigned int items = kVector.size();
    std::string item;
    // open file to write to
    std::ofstream output_file(kFilename);
    while (output_file.is_open()) {
        for (unsigned int i = 0; i < items; i++) {
            item = tsu::ToString(kVector[i]);
            output_file << item << '\n';
        }
        output_file.close();
    }
} // end VectorToFile

// MatrixToFile
// - open file and write matrix using delimiter
template <typename T>
static void MatrixToFile (const std::string kFilename,
                          const char kDelimiter,
                          const std::vector<std::vector<T>>& kMatrix) {
    std::printf("\n[Logging] to %s\n\n", kFilename.c_str());
    unsigned int rows = kMatrix.size();
    unsigned int columns = kMatrix[0].size();
    std::string item;
    // open file to write to
    std::ofstream output_file(kFilename);
    while (output_file.is_open()) {
        for (unsigned int i = 0; i < rows; i++) {
            for (unsigned int j = 0; j < columns; j++) {
                item = tsu::ToString(kMatrix[i][j]);
                char delimiter = (j == columns - 1) ? '\n' : kDelimiter;
                output_file << item << delimiter;
            }
        }
        output_file.close();
    }
} // end MatrixToFile

// VectorToMatrix
// - converts single vector string into a vector of vectors of given type
template <typename T>
static void VectorToMatrix (const std::vector<std::string>& kVector,
                            const unsigned int kColumns,
                            const bool kHeader,
                            std::vector<std::vector<T>>* matrix_ptr) {
    // determine number of rows
    unsigned int items = kVector.size();
    unsigned int rows = items/kColumns;

    // preallocate memory for vectors
    std::vector<std::vector<T>>& matrix_ref = *matrix_ptr;
    std::vector<T> temp_vector;
    temp_vector.reserve(kColumns);
    matrix_ref.reserve(rows);
    T item;  // initialize item and use for type conversion

    // check to see if first row should be ignored
    unsigned int skip = (kHeader) ? kColumns : 0;

    // loop through each item, convert to number, store in matrix
    unsigned int ctr = 1;
    for (unsigned int i = skip; i < items; i++){
        if (ctr < kColumns){
            item = tsu::StringToNumber(kVector[i], item);
            temp_vector.emplace_back(item);
            ctr++;
        } else {
            ctr = 1;
            item = tsu::StringToNumber(kVector[i], item);
            temp_vector.emplace_back(item);
            matrix_ref.emplace_back(temp_vector);
            temp_vector.clear();
        }
    }
} // end VectorToMatrix

template <typename T>
static void TimeSeriesMatrix (unsigned int rows,
                              unsigned int columns,
                              unsigned int increment,
                              std::vector<std::vector<T>>& op_matrix) {
    T value;
    std::vector<T> row;
    row.reserve(columns);
    op_matrix.reserve(rows);
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < columns; j++) {
            value = (j == 0) ? increment*i : 0;
            row.emplace_back(value);
        }
        op_matrix.emplace_back(row);
        row.clear();
    }
} // end TimeSeriesMatrix

// SortMatrix
// - sorting function takes the column number to sort
// - kGreater (false) will sort least to greatest
// - sorts using lambda expression for greatest->least or vice-versa
template <typename T>
static void SortMatrix (const unsigned int kColumn,
                   const bool kGreater,
                   std::vector<std::vector<T>>* matrix_ptr) {
    std::vector<std::vector<T>>& matrix_ref = *matrix_ptr; // deref pointer
    std::sort (matrix_ref.begin(), matrix_ref.end(),
              [&kGreater, &kColumn](const std::vector<T>& a,
                                    const std::vector<T>& b) {
                if (kGreater) {
                    return a[kColumn] > b[kColumn];
                } else {
                    return a[kColumn] < b[kColumn];
                }
              });
} // end SortMatrix

} // end namespace tsu

#endif // TS_UTILITY_HPP_INCLUDED
