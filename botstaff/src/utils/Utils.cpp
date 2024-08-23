#include "botstaff/utils/Utils.hpp"

#include <sstream>                  
#include <exception>                         
#include <format>                           
#include <functional>                        
#include <iostream>                          
#include <sstream>                          
#include <vector>                            

#include "botstaff/Vocabular.hpp"            
#include "botstaff/database/DB.hpp"         
#include "botstaff/keyboards/Keyboards.hpp"  
#include "botstaff/utils/CalendarUtils.hpp"  

#include "tgbot/Api.h"                       
#include "tgbot/Bot.h"

using namespace TgBot;

std::chrono::year_month_day string_to_chrono_date(
    const std::string &s, 
    char delim
) 
{
    std::vector<int> date;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) 
    {
        date.push_back(stoi(item));
    }  

    return std::chrono::year_month_day(
        std::chrono::year(date.at(0)),
        std::chrono::month(date.at(1)),
        std::chrono::day(date.at(2))
    );
}


std::string chrono_to_string_date(const std::chrono::year_month_day& ymd)
{
    return std::format("{}-{}-{}", 
        static_cast<int>(ymd.year()), 
        (int) static_cast<unsigned>(ymd.month()), 
        (int) static_cast<unsigned>(ymd.day())
    );
}


void send_message_with_kb(
    const TgBot::Bot& bot, 
    long chat_id, 
    const std::string& mess, 
    InlineKeyboardMarkup::Ptr kb,
    const std::string& pm)
{

    try
    {
        bot.getApi().sendMessage( 
            chat_id, 
            mess,
            nullptr,
            nullptr,
            kb, 
            pm
        );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void send_message(
    const TgBot::Bot& bot, 
    long chat_id, 
    const std::string& mess,
    const std::string& pm
)
{
    try
    {
        bot.getApi().sendMessage( 
            chat_id, 
            mess,
            nullptr,
            nullptr,
            nullptr,
            pm
        );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    
}

std::string get_teacher_info(const std::shared_ptr<BotUser>& u)
{
    return std::vformat(
        utils_voc::_techer_info, 
        std::make_format_args(u->first_name, 
        u->last_name, 
        u->tgusername, 
        u->phone, 
        u->email, 
        u->comment)
    );
}


std::string get_pupil_info(const std::shared_ptr<BotUser>& u)
{
    return std::vformat(
        utils_voc::_pupil_info,
        std::make_format_args( 
        u->first_name, 
        u->last_name, 
        u->cls, 
        u->tgusername, 
        u->phone, 
        u->email, 
        u->comment)
    );
}


void send_current_calendar(
    const TgBot::Bot& bot,
    long chat_id, 
    const bot_roles& role)
{
    auto ymd = get_curent_ymd();
    ymd.day = 1;
    
    std::thread send(
        send_message_with_kb,
        std::ref(bot),
        chat_id,
        std::vformat(
            utils_voc::_cal,
            std::make_format_args(MONTHS.at(ymd.month-1))
        ),
        Keyboards::calendar_kb()(
            ymd,
            role, 
            chat_id
        ),
        "HTML" 
    );
    send.detach();
}

void send_error_message(const TgBot::Bot& bot, long chat_id, std::string mess)
{
    try
    {
        bot.getApi().sendMessage(
            chat_id,
            mess
        ); 
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }    
}
