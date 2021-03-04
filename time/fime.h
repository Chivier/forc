/*
 * @Author       : Chivier Humber
 * @Date         : 2021-03-04 22:26:02
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-03-04 23:32:46
 * @Description  : file content
 * @FilePath     : /forc/time/fime.h
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <sys/time.h>

#define FIME_TIME_FORMAT_MAX_LENGTH 100

struct fime_t {
    timeval fime_time;

    fime_t() {
        // Creator of fime_t
        // Initial the time with current time
        gettimeofday(&fime_time, NULL);
    }

    void measure() {
        // Update current time
        gettimeofday(&fime_time, NULL);
    }

    std::string print_time_to_string() {
        // Return a string of current time
        // Format example: 08/19/12 02:50:06
        char buffer[FIME_TIME_FORMAT_MAX_LENGTH];
        strftime(buffer, FIME_TIME_FORMAT_MAX_LENGTH, "%x %X", localtime(&fime_time.tv_sec));
        return std::string(buffer);
    }

    // Convert time into a double precision value
    double get_double_precision() const {
        return fime_time.tv_sec + fime_time.tv_usec / 1000.0;
    }

    // Time elapse
    double operator - (const fime_t &other) const {
        double time1 = this->get_double_precision();
        double time2 = other.get_double_precision();
        return time1 - time2;
    }

    // Time compare
    bool operator == (const fime_t &other) const {
        return timercmp(&fime_time, &(other.fime_time), ==);
    }
    bool operator > (const fime_t &other) const {
        return timercmp(&fime_time, &(other.fime_time), >);
    }
    bool operator < (const fime_t &other) const {
        return timercmp(&fime_time, &(other.fime_time), <);
    }
};
