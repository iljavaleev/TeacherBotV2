#include "botstaff/utils/Utils.hpp"

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
    TgBot::InlineKeyboardMarkup::Ptr kb,
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
    return std::format(
        "<b>{} {}</b>\n<b>Username</b>: {}\n"
        "<b>Phone number</b>: {}\n"
        "<b>Email address</b>: {}\n"
        "<b>Comments:</b> {}", 
        u->first_name, 
        u->last_name, 
        u->tgusername, 
        u->phone, 
        u->email, 
        u->comment
    );
}


std::string get_pupil_info(const std::shared_ptr<BotUser>& u)
{
    return std::format(
        "<b>{} {}. Class: {}</b>\n<b>Username</b>: @{}\n"
        "<b>Phone number</b>: {}\n<b>Email address</b>: {}\n"
        "<b>Comments:</b> {}", 
        u->first_name, 
        u->last_name, 
        u->cls, 
        u->tgusername, 
        u->phone, 
        u->email, 
        u->comment
    );
}
