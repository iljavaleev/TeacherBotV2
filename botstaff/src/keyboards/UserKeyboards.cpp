#include "botstaff/keyboards/UserKeyboards.hpp"

#include <tgbot/tgbot.h>
#include <vector>
#include <utility>
#include <format>
#include <unordered_set>

#include "botstaff/database/DB.hpp"

using namespace std;
using namespace TgBot;

namespace UserKeyboards
{
    InlineKeyboardMarkup::Ptr create_user_start_kb(long chat_id, bool exists)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        if (exists)
        {
            InlineKeyboardButton::Ptr user_calendar_btn(
                new InlineKeyboardButton
            );
            user_calendar_btn->text = "Calendar";
            user_calendar_btn->callbackData = "calendar pupil";
            row.push_back(user_calendar_btn);
            keyboard->inlineKeyboard.push_back(row);
            return keyboard;
        }
        
        InlineKeyboardButton::Ptr register_btn(new InlineKeyboardButton);
        register_btn->text = "Registration";
        register_btn->callbackData = "start_register";
        row.push_back(register_btn);
        keyboard->inlineKeyboard.push_back(row);
        
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr choose_role_kb()
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

       
        InlineKeyboardButton::Ptr pupil_btn(new InlineKeyboardButton);
        pupil_btn->text = "Student";
        pupil_btn->callbackData = "register_as 0";
        row.push_back(pupil_btn);
  
        
        InlineKeyboardButton::Ptr teacher_btn(new InlineKeyboardButton);
        teacher_btn->text = "Teacher";
        teacher_btn->callbackData = "register_as 1";
        row.push_back(teacher_btn);

        InlineKeyboardButton::Ptr parent_btn(new InlineKeyboardButton);
        parent_btn->text = "Parent";
        parent_btn->callbackData = "register_as 2";
        row.push_back(parent_btn);   
        
        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_list_teachers_kb()
    {
        
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);

        std::vector<std::shared_ptr<BotUser>> teacher_list = BotUser::get_all(
            other_quiries::_create_all_active_teachers_list_kb
        );
        for (auto it{teacher_list.begin()}; it!=teacher_list.end(); ++it)
        {
            std::vector<InlineKeyboardButton::Ptr> row;
            InlineKeyboardButton::Ptr teacher_btn(new InlineKeyboardButton);
            teacher_btn->text = std::format(
                "{} {}", 
                (*it)->first_name, 
                (*it)->last_name
            );
            teacher_btn->callbackData = std::format(
                "register_pupil {}", 
                (*it)->chat_id
            );
            row.push_back(teacher_btn);
            keyboard->inlineKeyboard.push_back(row);
        }

        return keyboard;
    }
}
