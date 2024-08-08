#ifndef CalendarUtils_hpp
#define CalendarUtils_hpp

#include <tgbot/tgbot.h>
#include <unordered_set>
#include <chrono>

struct YMD
{
    YMD() = default;
    YMD(int _year, int _month, int _day): 
        year(_year), 
        month(_month), 
        day(_day){}

    int year;
    int month;
    int day;
};

YMD get_curent_ymd();
YMD chrono_to_ymd(const std::chrono::year_month_day& ymd);
#endif