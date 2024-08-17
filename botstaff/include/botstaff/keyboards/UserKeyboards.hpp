#ifndef UserKeyboards_hpp
#define UserKeyboards_hpp

#include <tgbot/tgbot.h>
#include <string>
#include "botstaff/database/DB.hpp"
#include "botstaff/database/Quiries.hpp"

using namespace TgBot;

namespace UserKeyboards
{
    InlineKeyboardMarkup::Ptr create_user_start_kb(long chat_id);
    InlineKeyboardMarkup::Ptr choose_role_kb();
    InlineKeyboardMarkup::Ptr create_list_teachers_kb();
    InlineKeyboardMarkup::Ptr create_parent_start_kb(long);
    InlineKeyboardMarkup::Ptr parent_comments(long);
}

#endif