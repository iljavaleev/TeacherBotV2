#include "botstaff/utils/CalendarUtils.hpp"

using namespace TgBot;


YMD chrono_to_ymd(const std::chrono::year_month_day& ymd)
{
    return YMD(
        static_cast<int>(ymd.year()), 
        (int) static_cast<unsigned>(ymd.month()),
        (int) static_cast<unsigned>(ymd.day())
    );
}

YMD get_curent_ymd()
{
    const std::chrono::time_point now{std::chrono::system_clock::now()};
    const std::chrono::year_month_day ymd{
        std::chrono::floor<std::chrono::days>(now)
    };
    
    return chrono_to_ymd(ymd);

}



