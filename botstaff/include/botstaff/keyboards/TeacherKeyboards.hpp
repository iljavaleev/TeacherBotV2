#ifndef TeacherKeyboards_hpp
#define TeacherKeyboards_hpp

#include <tgbot/tgbot.h>
#include <string>

#include "botstaff/database/DB.hpp"
#include "botstaff/database/Quiries.hpp"

using namespace TgBot;

namespace teacherKeyboards
{
    InlineKeyboardMarkup::Ptr create_teacher_start_kb(bool);
    InlineKeyboardMarkup::Ptr create_teachers_kb();
    InlineKeyboardMarkup::Ptr create_pupils_kb(long);
    
    
    InlineKeyboardMarkup::Ptr create_comments_kb(long);
    InlineKeyboardMarkup::Ptr create_debts_kb(long);
    InlineKeyboardMarkup::Ptr change_payment_status(long);
    

    InlineKeyboardMarkup::Ptr create_teachers_list_kb(bool is_active);
    InlineKeyboardMarkup::Ptr create_pupils_list_kb(
        long chat_id, 
        bool is_active
    );
    InlineKeyboardMarkup::Ptr create_user_info_kb(std::shared_ptr<BotUser>&);
    InlineKeyboardMarkup::Ptr update_user_info_kb(const bot_roles& role);
    InlineKeyboardMarkup::Ptr create_list_pupils_kb(
        long chat_id, 
        bool is_active=true
    );
    InlineKeyboardMarkup::Ptr create_pupil_for_lesson_kb(
        long chat_id, bool update_lesson=false
    );
    InlineKeyboardMarkup::Ptr update_lesson_info_kb(int lesson_id);
}

#endif