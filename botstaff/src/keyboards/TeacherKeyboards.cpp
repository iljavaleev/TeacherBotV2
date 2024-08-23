#include "botstaff/keyboards/TeacherKeyboards.hpp"

#include <format>                             
#include <string>                              
#include <vector>        
                      
#include "botstaff/Vocabular.hpp"              
#include "botstaff/database/DB.hpp"            
#include "botstaff/database/Quiries.hpp"

#include "tgbot/types/InlineKeyboardButton.h"  

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

    InlineKeyboardMarkup::Ptr create_debts_kb(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        get_debts_for_kb(keyboard, chat_id);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr change_payment_status(long chat_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr change_status_yes_btn(new InlineKeyboardButton);
        change_status_yes_btn->text = keyboards_voc::keyboards::_yes;
        change_status_yes_btn->callbackData = 
            std::format("change_debt yes {}", chat_id);
        row.push_back(change_status_yes_btn);

        InlineKeyboardButton::Ptr change_status_no_btn(new InlineKeyboardButton);
        change_status_no_btn->text = keyboards_voc::keyboards::_no;
        change_status_no_btn->callbackData = 
            std::format("change_debt no {}", chat_id);
        row.push_back(change_status_no_btn);
        
        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }
    
    InlineKeyboardMarkup::Ptr create_teacher_start_kb(bool is_admin)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr admin_calendar_btn(new InlineKeyboardButton);
        admin_calendar_btn->text = keyboards_voc::keyboards::_day_cal;
        admin_calendar_btn->callbackData = "calendar teacher";
        row.push_back(admin_calendar_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr list_pupils_btn(new InlineKeyboardButton);
        list_pupils_btn->text = keyboards_voc::keyboards::_pup;
        list_pupils_btn->callbackData = "pupils";
        row.push_back(list_pupils_btn);

        if (is_admin)
        {
            InlineKeyboardButton::Ptr list_teacher_btn(
                new InlineKeyboardButton
            );
            list_teacher_btn->text = keyboards_voc::teacher::_teach;
            list_teacher_btn->callbackData = "teachers";
            row.push_back(list_teacher_btn);
        }
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr list_comments_btn(new InlineKeyboardButton);
        list_comments_btn->text = keyboards_voc::teacher::_comments;
        list_comments_btn->callbackData = "comments";
        row.push_back(list_comments_btn);

        InlineKeyboardButton::Ptr list_debts_btn(new InlineKeyboardButton);
        list_debts_btn->text = keyboards_voc::teacher::_debts;
        list_debts_btn->callbackData = "debts";
        row.push_back(list_debts_btn);
    

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
        teacher_active_pupils_btn->text = keyboards_voc::teacher::_list_stud_active;
        teacher_active_pupils_btn->callbackData = "pupils_list_active";
        row.push_back(teacher_active_pupils_btn);

        InlineKeyboardButton::Ptr list_pupils_btn(new InlineKeyboardButton);
        list_pupils_btn->text = keyboards_voc::teacher::_list_not_active;
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
        teacher_active_teachers_btn->text = 
            keyboards_voc::teacher::_list_teach_active;
        teacher_active_teachers_btn->callbackData = "teachers_list_active";
        row.push_back(teacher_active_teachers_btn);

        InlineKeyboardButton::Ptr list_teachers_btn(new InlineKeyboardButton);
        list_teachers_btn->text = keyboards_voc::teacher::_list_not_active;
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
            activate_btn->text = keyboards_voc::teacher::_activate;
            activate_btn->callbackData = std::format(
                "activate_user {}", 
                u->chat_id
            );
            row.push_back(activate_btn);
        }
        else
        {
            InlineKeyboardButton::Ptr update_btn(new InlineKeyboardButton);
            update_btn->text = keyboards_voc::teacher::_user_edit;
            update_btn->callbackData = std::format(
                "update_user {}", 
                u->chat_id
            );
            row.push_back(update_btn);
        }
        
        InlineKeyboardButton::Ptr delete_btn(new InlineKeyboardButton);
        delete_btn->text = keyboards_voc::teacher::_user_delete;
        delete_btn->callbackData = std::format("delete_user {}", u->chat_id);
        row.push_back(delete_btn);

        keyboard->inlineKeyboard.push_back(row);
        return keyboard;
    }

    InlineKeyboardMarkup::Ptr update_user_info_kb(
        const bot_roles& role
    )
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;
        
        InlineKeyboardButton::Ptr update_first_name_btn(
            new InlineKeyboardButton
        );
        update_first_name_btn->text = keyboards_voc::teacher::_kb_name;
        update_first_name_btn->callbackData = 
            "update_user_field first_name";
        row.push_back(update_first_name_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
        
        InlineKeyboardButton::Ptr update_last_name_btn(
            new InlineKeyboardButton
        );
        update_last_name_btn->text = keyboards_voc::teacher::_kb_sname;
        update_last_name_btn->callbackData = 
            "update_user_field last_name";
        row.push_back(update_last_name_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
        
        if (role == bot_roles::pupil)
        {
            InlineKeyboardButton::Ptr update_class_btn(
                new InlineKeyboardButton
            );
            update_class_btn->text = keyboards_voc::teacher::_kb_class;
            update_class_btn->callbackData = "update_user_field class";
            row.push_back(update_class_btn);
            keyboard->inlineKeyboard.push_back(row);
            row.clear();
        }
        
        InlineKeyboardButton::Ptr update_phone_btn(new InlineKeyboardButton);
        update_phone_btn->text = keyboards_voc::teacher::_kb_phone;
        update_phone_btn->callbackData = "update_user_field phone";
     
        row.push_back(update_phone_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_email_btn(new InlineKeyboardButton);
        update_email_btn->text = keyboards_voc::teacher::_kb_email;
        update_email_btn->callbackData = "update_user_field email";
        row.push_back(update_email_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_comments_btn(new InlineKeyboardButton);
        update_comments_btn->text = keyboards_voc::teacher::_kb_comment;
        update_comments_btn->callbackData = "update_user_field comments";
        row.push_back(update_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_status_btn(new InlineKeyboardButton);
        update_status_btn->text = keyboards_voc::teacher::_kb_status;
        update_status_btn->callbackData = "update_user_field status";
        row.push_back(update_status_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr finish_update_btn(new InlineKeyboardButton);
        finish_update_btn->text = keyboards_voc::teacher::_kb_finish;
        finish_update_btn->callbackData = "update_user_field finish";
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
        update_lesson_date_btn->text = keyboards_voc::teacher::_lkb_date;
        update_lesson_date_btn->callbackData = "update_lesson_field date";
        row.push_back(update_lesson_date_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();


        InlineKeyboardButton::Ptr update_lesson_time_btn(
            new InlineKeyboardButton
        );
        update_lesson_time_btn->text = keyboards_voc::teacher::_lkb_time;
        update_lesson_time_btn->callbackData = "update_lesson_field time";
        row.push_back(update_lesson_time_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        
        InlineKeyboardButton::Ptr update_lesson_pupil_btn(
            new InlineKeyboardButton
        );
        update_lesson_pupil_btn->text = keyboards_voc::teacher::_lkb_std;
        update_lesson_pupil_btn->callbackData = "update_lesson_field pupil"; 
        row.push_back(update_lesson_pupil_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
        
      
        InlineKeyboardButton::Ptr update_lesson_objectives_btn(
            new InlineKeyboardButton
        );
        update_lesson_objectives_btn->text = keyboards_voc::teacher::_lkb_obj;
        update_lesson_objectives_btn->callbackData = 
            "update_lesson_field objectives";
        row.push_back(update_lesson_objectives_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();
     
        
        InlineKeyboardButton::Ptr update_lesson_pupil_comments_btn(
            new InlineKeyboardButton
        );
        update_lesson_pupil_comments_btn->text = 
            keyboards_voc::teacher::_lkb_comment_pup;
        update_lesson_pupil_comments_btn->callbackData = "update_lesson_field" 
            " comment_for_pupil";
        row.push_back(update_lesson_pupil_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_lesson_teacher_comments_btn(
            new InlineKeyboardButton
        );
        update_lesson_teacher_comments_btn->text = 
            keyboards_voc::teacher::_lkb_comment_teach;
        update_lesson_teacher_comments_btn->callbackData = 
            "update_lesson_field comment_for_teacher";
        row.push_back(update_lesson_teacher_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_lesson_parent_comments_btn(
            new InlineKeyboardButton
        );
        update_lesson_parent_comments_btn->text = 
            keyboards_voc::teacher::_lkb_comment_par;
        update_lesson_parent_comments_btn->callbackData = 
            "update_lesson_field comment_for_parent";
        row.push_back(update_lesson_parent_comments_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr update_lesson_is_paid_btn(
            new InlineKeyboardButton
        );
        update_lesson_is_paid_btn->text = keyboards_voc::teacher::_lkb_status;
        update_lesson_is_paid_btn->callbackData = "update_lesson_field is_paid";
        row.push_back(update_lesson_is_paid_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr finish_update_btn(new InlineKeyboardButton);
        finish_update_btn->text = keyboards_voc::teacher::_lkb_finish;
        finish_update_btn->callbackData = std::format("update_lesson_field"
            " finish_lesson_update", 
            lesson_id
        );
        row.push_back(finish_update_btn);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        return keyboard;
    }

    InlineKeyboardMarkup::Ptr cancel_reason_kb(int lesson_id)
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        vector<InlineKeyboardButton::Ptr> row;
        
        InlineKeyboardButton::Ptr btn1(new InlineKeyboardButton);
        btn1->text = keyboards_voc::teacher::_cancel_reason_s;
        btn1->callbackData = std::format("delete_for"
            " student_request {}", 
            lesson_id
        );
        row.push_back(btn1);
        keyboard->inlineKeyboard.push_back(row);
        row.clear();

        InlineKeyboardButton::Ptr btn2(new InlineKeyboardButton);
        btn2->text = keyboards_voc::teacher::_cancel_reason_o;
        btn2->callbackData = std::format("delete_for"
            " other_reasons {}", 
            lesson_id
        );
        row.push_back(btn2);
        keyboard->inlineKeyboard.push_back(row);

        return keyboard;
    }
}
