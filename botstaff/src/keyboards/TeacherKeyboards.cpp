#include "botstaff/keyboards/TeacherKeyboards.hpp"

#include <tgbot/tgbot.h>
#include <vector>
#include <utility>
#include <format>
#include <unordered_set>
#include <iostream>
#include <pqxx/pqxx>

#include "botstaff/database/DB.hpp"


using namespace std;
using namespace TgBot;

namespace teacherKeyboards
{
    InlineKeyboardMarkup::Ptr create_comments_kb(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        get_comments_for_kb(keyboard, chat_id);
        return keyboard;
    }
    
    InlineKeyboardMarkup::Ptr create_teacher_start_kb(bool is_admin)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr admin_calendar_btn(new InlineKeyboardButton);
        admin_calendar_btn->text = "Calendar";
        admin_calendar_btn->callbackData = "calendar teacher";
        row.push_back(admin_calendar_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr list_pupils_btn(new InlineKeyboardButton);
        list_pupils_btn->text = "List of students";
        list_pupils_btn->callbackData = "pupils";
        row.push_back(list_pupils_btn);

        if (is_admin)
        {
            InlineKeyboardButton::Ptr list_teacher_btn(
                new InlineKeyboardButton
            );
            list_teacher_btn->text = "List of teachers";
            list_teacher_btn->callbackData = "teachers";
            row.push_back(list_teacher_btn);
        }
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr list_comments_btn(new InlineKeyboardButton);
        list_comments_btn->text = "My comments";
        list_comments_btn->callbackData = "comments";
        row.push_back(list_comments_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_pupils_kb(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr teacher_active_pupils_btn(
            new InlineKeyboardButton
        );
        teacher_active_pupils_btn->text = "List of active students";
        teacher_active_pupils_btn->callbackData = "pupils_list_active";
        row.push_back(teacher_active_pupils_btn);

        InlineKeyboardButton::Ptr list_pupils_btn(new InlineKeyboardButton);
        list_pupils_btn->text = "Request list";
        list_pupils_btn->callbackData = "pupils_list";
        row.push_back(list_pupils_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_teachers_kb()
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr teacher_active_teachers_btn(
            new InlineKeyboardButton
        );
        teacher_active_teachers_btn->text = "List of active teachers";
        teacher_active_teachers_btn->callbackData = "teachers_list_active";
        row.push_back(teacher_active_teachers_btn);

        InlineKeyboardButton::Ptr list_teachers_btn(new InlineKeyboardButton);
        list_teachers_btn->text = "Request teachers list";
        list_teachers_btn->callbackData = "teachers_list";
        row.push_back(list_teachers_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_teachers_list_kb(bool is_active)
    {
        
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        
        get_users_for_kb(keyboard, create_query(
            other_quiries::_create_teachers_list_kb, 
            is_active),
            "user_info {}"
        );
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_pupils_list_kb(
        long chat_id, 
        bool is_active
    )
    {
        
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        get_users_for_kb(keyboard, create_query(
            other_quiries::_create_pupils_list_kb, 
            chat_id,
            is_active),
            "user_info {}"
        );
        
        return keyboard;
    }


    InlineKeyboardMarkup::Ptr create_user_info_kb(std::shared_ptr<BotUser>& u)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        if (!u->is_active)
        {
            
            InlineKeyboardButton::Ptr activate_btn(new InlineKeyboardButton);
            activate_btn->text = "Activate";
            activate_btn->callbackData = std::format(
                "activate_user {}", 
                u->chat_id
            );
            row.push_back(activate_btn);
        }
        else
        {
            InlineKeyboardButton::Ptr update_btn(new InlineKeyboardButton);
            update_btn->text = "Edit";
            update_btn->callbackData = std::format(
                "update_user {} {}", 
                (int) u->role, 
                u->chat_id
            );
            row.push_back(update_btn);
        }
        
        InlineKeyboardButton::Ptr delete_btn(new InlineKeyboardButton);
        delete_btn->text = "Delete";
        delete_btn->callbackData = std::format("delete_user {}", u->chat_id);
        row.push_back(delete_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr update_user_info_kb(
        const bot_roles& role, 
        long chat_id
    )
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;
        
        InlineKeyboardButton::Ptr update_first_name_btn(
            new InlineKeyboardButton
        );
        update_first_name_btn->text = "Change name";
        update_first_name_btn->callbackData = std::format(
            "update_user_field first_name {}", 
            chat_id
        );
        row.push_back(update_first_name_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
        
        InlineKeyboardButton::Ptr update_last_name_btn(
            new InlineKeyboardButton
        );
        update_last_name_btn->text = "Change surname";
        update_last_name_btn->callbackData = std::format(
            "update_user_field last_name {}", 
            chat_id
        );
        row.push_back(update_last_name_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
        
        if (role == bot_roles::pupil)
        {
            InlineKeyboardButton::Ptr update_class_btn(
                new InlineKeyboardButton
            );
            update_class_btn->text = "Change class";
            update_class_btn->callbackData = std::format(
                "update_user_field class {}", 
                chat_id
            );
            row.push_back(update_class_btn);
            keyboard->inlineKeyboard.push_back(row);
            row.clear();
        }
        
        InlineKeyboardButton::Ptr update_phone_btn(new InlineKeyboardButton);
        update_phone_btn->text = "Change phone";
        update_phone_btn->callbackData = std::format(
            "update_user_field phone {}", 
            chat_id
        );
        row.push_back(update_phone_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_email_btn(new InlineKeyboardButton);
        update_email_btn->text = "Change mail";
        update_email_btn->callbackData = std::format(
            "update_user_field email {}", 
            chat_id
        );
        row.push_back(update_email_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_comments_btn(new InlineKeyboardButton);
        update_comments_btn->text = "Change comment";
        update_comments_btn->callbackData = std::format(
            "update_user_field comments {}", 
            chat_id
        );
        row.push_back(update_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr finish_update_btn(new InlineKeyboardButton);
        finish_update_btn->text = "Finish editing";
        finish_update_btn->callbackData = std::format(
            "update_user_field finish {}", 
            chat_id
        );
        row.push_back(finish_update_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        return keyboard;
    }

    InlineKeyboardMarkup::Ptr create_list_pupils_kb(
        long chat_id, 
        bool is_active
    )
    {
        
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::string callback_data = "lesson_pupil {}";        
        get_users_for_kb(keyboard, 
            create_query(
                other_quiries::_create_pupils_list_kb, chat_id, is_active
            ),
            callback_data
        );
        
        return keyboard;
    }
    
    InlineKeyboardMarkup::Ptr create_pupil_for_lesson_kb(
        long chat_id, bool update_lesson
    )
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::string callback_data = update_lesson ? 
            "update_pupil_for_lesson {}" : "choose_pupil_for_lesson {}";        
        get_users_for_kb(keyboard, 
            create_query(
                other_quiries::_create_pupil_for_lesson_kb, chat_id
            ),
            callback_data
        );
        
        return keyboard;
    }
    
    InlineKeyboardMarkup::Ptr update_lesson_info_kb(int lesson_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;
        
        InlineKeyboardButton::Ptr update_lesson_date_btn(
            new InlineKeyboardButton
        );
        update_lesson_date_btn->text = "Change date";
        update_lesson_date_btn->callbackData = "update_lesson_field date";
        row.push_back(update_lesson_date_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();


        InlineKeyboardButton::Ptr update_lesson_time_btn(
            new InlineKeyboardButton
        );
        update_lesson_time_btn->text = "Change time";
        update_lesson_time_btn->callbackData = "update_lesson_field time";
        row.push_back(update_lesson_time_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        
        InlineKeyboardButton::Ptr update_lesson_pupil_btn(
            new InlineKeyboardButton
        );
        update_lesson_pupil_btn->text = "Change student";
        update_lesson_pupil_btn->callbackData = "update_lesson_field pupil"; 
        row.push_back(update_lesson_pupil_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
        
      
        InlineKeyboardButton::Ptr update_lesson_objectives_btn(
            new InlineKeyboardButton
        );
        update_lesson_objectives_btn->text = "Change objectives";
        update_lesson_objectives_btn->callbackData = 
            "update_lesson_field objectives";
        row.push_back(update_lesson_objectives_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
     
        
        InlineKeyboardButton::Ptr update_lesson_pupil_comments_btn(
            new InlineKeyboardButton
        );
        update_lesson_pupil_comments_btn->text = "Change comments for pupil";
        update_lesson_pupil_comments_btn->callbackData = "update_lesson_field" 
            " comment_for_pupil";
        row.push_back(update_lesson_pupil_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_lesson_teacher_comments_btn(
            new InlineKeyboardButton
        );
        update_lesson_teacher_comments_btn->text = "Change coments"
            " for teacher";
        update_lesson_teacher_comments_btn->callbackData = 
            "update_lesson_field comment_for_teacher";
        row.push_back(update_lesson_teacher_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_lesson_parent_comments_btn(
            new InlineKeyboardButton
        );
        update_lesson_parent_comments_btn->text = "Change coments"
            " for parent";
        update_lesson_parent_comments_btn->callbackData = 
            "update_lesson_field comment_for_parent";
        row.push_back(update_lesson_parent_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_lesson_is_paid_btn(
            new InlineKeyboardButton
        );
        update_lesson_is_paid_btn->text = "Change payment status";
        update_lesson_is_paid_btn->callbackData = "update_lesson_field is_paid";
        row.push_back(update_lesson_is_paid_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr finish_update_btn(new InlineKeyboardButton);
        finish_update_btn->text = "Finish editing";
        finish_update_btn->callbackData = std::format("update_lesson_field"
            " finish_lesson_update", 
            lesson_id
        );
        row.push_back(finish_update_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        return keyboard;
    }

}
