#include "botstaff/keyboards/UserKeyboards.hpp"

#include <chrono>          
#include <format>                              
#include <memory>                              
#include <string>                              
#include <vector>                              

#include "botstaff/Vocabular.hpp"              
#include "botstaff/database/DB.hpp"            
#include "botstaff/database/Quiries.hpp"       
#include "botstaff/utils/Utils.hpp" 
           
#include "tgbot/types/InlineKeyboardButton.h"  

using namespace std;
using namespace TgBot;

namespace UserKeyboards
{
    InlineKeyboardMarkup::Ptr create_user_start_kb(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;
 
        InlineKeyboardButton::Ptr register_btn(new InlineKeyboardButton);
        register_btn->text = keyboards_voc::user::_reg;
        register_btn->callbackData = "start_register";
        row.push_back(register_btn);
        keyboard->inlineKeyboard.push_back(row);
        
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_parent_start_kb(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr reschedule_class_btn(
            new InlineKeyboardButton
        );
        reschedule_class_btn->text = keyboards_voc::user::_parent_resc;
        reschedule_class_btn->callbackData = "parent_reschedule";
        row.push_back(reschedule_class_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr comments_btn(new InlineKeyboardButton);
        comments_btn->text = keyboards_voc::user::_parent_comm;
        comments_btn->callbackData = "parent_comments";
        row.push_back(comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr debt_btn(new InlineKeyboardButton);
        debt_btn->text = keyboards_voc::user::_parent_debts;
        debt_btn->callbackData = "parent_debts";
        row.push_back(debt_btn);
        keyboard->inlineKeyboard.push_back(row);
        
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr choose_role_kb()
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr pupil_btn(new InlineKeyboardButton);
        pupil_btn->text = keyboards_voc::user::_role_std;
        pupil_btn->callbackData = "register_as 0";
        row.push_back(pupil_btn);
  
        InlineKeyboardButton::Ptr teacher_btn(new InlineKeyboardButton);
        teacher_btn->text = keyboards_voc::user::_role_teach;
        teacher_btn->callbackData = "register_as 1";
        row.push_back(teacher_btn);

        InlineKeyboardButton::Ptr parent_btn(new InlineKeyboardButton);
        parent_btn->text = keyboards_voc::user::_role_par;
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

    InlineKeyboardMarkup::Ptr parent_comments(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::vector<std::shared_ptr<UserLesson>> comments = 
            get_parent_comments(chat_id);
        vector<InlineKeyboardButton::Ptr> row;
        
        for (const auto& comment: comments)
        {
            InlineKeyboardButton::Ptr btn(new InlineKeyboardButton);
            std::chrono::year_month_day ymd = 
                string_to_chrono_date(comment->date);
            
            btn->text = std::format(
                "{} {} {}", ymd.day(), ymd.month(), ymd.year());
            btn->callbackData = std::format(
                "for_parent_comment {}", 
                comment->comment_for_parent
            );
            row.push_back(btn);
            keyboard->inlineKeyboard.push_back(row);
            row.clear();
        }
        return keyboard;
    }
    
    InlineKeyboardMarkup::Ptr request_to_delete_kb(long lesson_id, long pupil_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr delete_btn(new InlineKeyboardButton);
        delete_btn->text = keyboards_voc::user::_delete;
        delete_btn->callbackData = std::format(
            "cancel_lesson {} {}", lesson_id, pupil_id);
        row.push_back(delete_btn);
        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }
}
