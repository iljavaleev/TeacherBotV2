#ifndef Keyboards_hpp
#define Keyboards_hpp

#include <tgbot/tgbot.h>
#include <string>

#include "botstaff/utils/CalendarUtils.hpp"
#include "botstaff/utils/Utils.hpp"

using namespace TgBot;
enum class bot_roles;

namespace Keyboards
{

    class calendar_kb
    {
        static const std::string cb_data;
        static const std::string update_lesson_cb_data;
        static const std::vector<std::string> days;
        InlineKeyboardMarkup::Ptr keyboard;
        InlineKeyboardButton::Ptr empty_btn;

        int day_number(YMD date);
        int days_in_month(YMD date);
        void create_first_row();

        void create_last_row(
            const YMD& date,
            const bot_roles& role, 
            bool update
        );
        

    public:
        calendar_kb():
            keyboard(new InlineKeyboardMarkup), 
            empty_btn(new InlineKeyboardButton)
        {
            empty_btn->text = " ";
            empty_btn->callbackData = " ";
        } 
        
        InlineKeyboardMarkup::Ptr operator()(
            const YMD& ymd, 
            const bot_roles& role,
            long chat_id, 
            bool update_lesson=false
        );        
    };


    InlineKeyboardMarkup::Ptr create_day_kb();
    InlineKeyboardMarkup::Ptr agreement_kb();
    InlineKeyboardMarkup::Ptr lessons_list_kb(
        long, 
        const bot_roles&, 
        const YMD&
    );
    InlineKeyboardMarkup::Ptr day_info_kb(int, const bot_roles&);
    
} 

#endif