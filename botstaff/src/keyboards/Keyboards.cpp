#include "botstaff/keyboards/Keyboards.hpp"

#include <tgbot/tgbot.h>
#include <vector>
#include <utility>
#include <format>
#include <unordered_set>

#include "botstaff/utils/Utils.hpp"
#include "botstaff/database/DB.hpp"
#include "botstaff/utils/CalendarUtils.hpp"
#include "botstaff/Vocabular.hpp"

using namespace std;
using namespace TgBot;


namespace Keyboards
{
    const std::string calendar_kb::cb_data = 
        "calendar_day {} {} {} {}";
    const std::string calendar_kb::update_lesson_cb_data = 
        "update_date_for_lesson {} {}-{}-{}";
    const std::vector<std::string> calendar_kb::days = 
        keyboards_voc::keyboards::DAYS;

    int calendar_kb::day_number(YMD date) 
    { 
        static std::vector<int> t = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 }; 
        date.year -= date.month < 3; 
        int num = ( 
            date.year + 
            date.year/4 - 
            date.year/100 + 
            date.year/400 + 
            t[date.month-1] + 1) % 7; 
        if (num == 0)
            return 6;
        return num - 1;  
    } 

    int calendar_kb::days_in_month(YMD date)
    {
        if (
            date.month == 1 || 
            date.month == 3 || 
            date.month == 5 || 
            date.month == 7 || 
            date.month == 8 || 
            date.month == 11 || 
            date.month == 12
        )
            return 31;
        if (
            date.month == 4 || 
            date.month == 6 || 
            date.month == 9 || 
            date.month == 10
        )
            return 30;
        return (!(date.year % 4) && (
            date.year % 100)) || !(date.year % 400) ? 29 : 28;
    }

    void calendar_kb::create_first_row()
    {
        InlineKeyboardButton::Ptr btn(new InlineKeyboardButton);
        std::vector<InlineKeyboardButton::Ptr> week_days(7, btn);
        for (int i{}; i < 7; ++i)
        {
            InlineKeyboardButton::Ptr btn(new InlineKeyboardButton);
            btn->text = days.at(i);
            btn->callbackData = days.at(i);
            week_days.at(i) = btn;
        }
        keyboard->inlineKeyboard.push_back(week_days);  
    }

    void calendar_kb::create_last_row(
        const YMD& date,
        const bot_roles& role, 
        bool update_lesson
        )
    {
        YMD ymd = get_curent_ymd();
        std::vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr prev_month_btn(new InlineKeyboardButton);
        prev_month_btn->text = "<<";
        prev_month_btn->callbackData = std::format(
            "change_month << {} {} {} {}", 
            date.year, 
            date.month, 
            update_lesson, 
            (int) role
        );
        row.push_back(prev_month_btn);

        InlineKeyboardButton::Ptr current_btn(new InlineKeyboardButton);
        current_btn->text = std::vformat(
            keyboards_voc::keyboards::_today,
                std::make_format_args(
                ymd.day, 
                ymd.month, 
                ymd.year
            )
        );
        current_btn->callbackData = " ";
        row.push_back(current_btn);

        InlineKeyboardButton::Ptr next_month_btn(new InlineKeyboardButton);
        next_month_btn->text = ">>";
        next_month_btn->callbackData = std::format(
            "change_month >> {} {} {} {}", 
            date.year, 
            date.month, 
            update_lesson, 
            (int) role
        );
        row.push_back(next_month_btn);

        keyboard->inlineKeyboard.push_back(row);
    }

    InlineKeyboardMarkup::Ptr calendar_kb::operator()(
        const YMD& ymd, 
        const bot_roles& role,
        long chat_id, 
        bool update_lesson)
    {
        std::unordered_set<int> lesson_days = get_lesson_days(
            ymd, 
            chat_id, 
            role
        );
        int first_day_number = day_number(ymd);
        int month_day_number = days_in_month(ymd);
        int count = 1;

        
        create_first_row();
        
        bool first_row{true};
        bool outer{true};

        while(outer)
        {   
            int start{};
            vector<InlineKeyboardButton::Ptr> row(7, empty_btn);
            if (first_row)
            {
                start = first_day_number;
                first_row = false;
            }
            for (int j{start}; j < 7; ++j)
            {
                InlineKeyboardButton::Ptr cal_btn(new InlineKeyboardButton);
                
                if (lesson_days.contains(count))
                    cal_btn->text = std::format("_{}_", count);
                else
                    cal_btn->text = std::format("{}", count);
                
                cal_btn->callbackData = std::vformat(
                    update_lesson ? update_lesson_cb_data : cb_data, 
                    std::make_format_args(
                        (int) role, 
                        ymd.year, 
                        ymd.month, 
                        count
                    )
                );
                
                row.at(j) = cal_btn;
                count++;
                
                if (count > month_day_number)
                {   
                    outer = false;
                    break;
                }
            }
            keyboard->inlineKeyboard.push_back(row);
        }
        create_last_row(ymd, role, update_lesson);
        return keyboard;
    }
            
    InlineKeyboardMarkup::Ptr create_day_kb()
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr admin_calendar_btn(new InlineKeyboardButton);
        admin_calendar_btn->text = keyboards_voc::keyboards::_day_cal;
        admin_calendar_btn->callbackData = "admin calendar";
        row.push_back(admin_calendar_btn);

        InlineKeyboardButton::Ptr list_pupils_btn(new InlineKeyboardButton);
        list_pupils_btn->text = keyboards_voc::keyboards::_pup;
        list_pupils_btn->callbackData = "admin pupils";
        row.push_back(list_pupils_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr agreement_kb()
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr yes_btn(new InlineKeyboardButton);
        yes_btn->text = keyboards_voc::keyboards::_yes;
        yes_btn->callbackData = "agreement yes";
        row.push_back(yes_btn);

        InlineKeyboardButton::Ptr no_btn(new InlineKeyboardButton);
        no_btn->text =  keyboards_voc::keyboards::_no;
        no_btn->callbackData = "agreement no";
        row.push_back(no_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr lessons_list_kb(
        long chat_id, 
        const bot_roles& role, 
        const YMD& ymd
    )
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::vector<std::vector<std::string>> info = get_lessons_by_day(
            ymd,
            chat_id, 
            role 
        );

        for (auto it{info.begin()}; it != info.end(); ++it)
        {
            vector<InlineKeyboardButton::Ptr> row;
            InlineKeyboardButton::Ptr user_btn(new InlineKeyboardButton);
            user_btn->text = std::format(
                "{} {} {}", 
                it->at(0), 
                it->at(1), 
                it->at(3)
            );
            user_btn->callbackData = std::format(
                "user_lesson {} {}", 
                it->at(2), 
                (int) role
            );
            row.push_back(user_btn);
            keyboard->inlineKeyboard.push_back(row);
        }
        if (role == bot_roles::teacher || role == bot_roles::admin)
        {
            vector<InlineKeyboardButton::Ptr> row;
            InlineKeyboardButton::Ptr create_lesson_btn(
                new InlineKeyboardButton
            );
            create_lesson_btn->text = keyboards_voc::keyboards::_create_lesson;
            create_lesson_btn->callbackData = std::format(
                "create_lesson {}-{}-{}", 
                ymd.year, 
                ymd.month, 
                ymd.day
            );
            row.push_back(create_lesson_btn);
            keyboard->inlineKeyboard.push_back(row);
        }

        return keyboard;
    }

    InlineKeyboardMarkup::Ptr day_info_kb(int lesson_id, const bot_roles& role)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        if (role == bot_roles::pupil)
        {
            InlineKeyboardButton::Ptr change_date_btn(new InlineKeyboardButton);
            change_date_btn->text = keyboards_voc::keyboards::_day_reschedule;
            change_date_btn->callbackData = std::format(
                "change_lesson_date {}", 
                lesson_id
            );
            row.push_back(change_date_btn);
        }
        else
        {
            InlineKeyboardButton::Ptr update_lesson_btn(
                new InlineKeyboardButton
            );
            update_lesson_btn->text = keyboards_voc::keyboards::_lesson_edit;
            update_lesson_btn->callbackData = std::format(
                "update_lesson {}", 
                lesson_id
            );
            row.push_back(update_lesson_btn);

            InlineKeyboardButton::Ptr delete_lesson_btn(
                new InlineKeyboardButton
            );
            delete_lesson_btn->text = keyboards_voc::keyboards::_lesson_delete;
            delete_lesson_btn->callbackData = std::format(
                "delete_lesson {}", 
                lesson_id
            );
            row.push_back(delete_lesson_btn);
        }
        
        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }
}
