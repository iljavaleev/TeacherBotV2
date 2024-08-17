#ifndef Vocabular_hpp
#define Vocabular_hpp
#include "botstaff/database/DB.hpp"

#include <vector>
#include <string>

#define EN

#ifdef RU
const static std::vector<std::string> MONTHS = {
    "Январь", 
    "Февраль", 
    "Март", 
    "Апрель", 
    "Май", 
    "Июнь", 
    "Июль", 
    "Август", 
    "Сентябрь", 
    "Октябрь", 
    "Ноябрь", 
    "Декабрь"
};

namespace DB_voc
{
    inline std::string _error = "Ошибка, попробуйте позднее";
    inline std::string _active = "активен";
    inline std::string _not_active = "не активен";
    inline std::string _comment = "Комментарий: ";
    inline std::string _yes = "да";
    inline std::string _no = "нет";

    inline std::string _db_open_failed = "Ошибка установки связи с BD\n";
    inline std::string _db_work_failed =  "Ошибка трансакции";
    inline std::string _get_full_info_pupil =   
        "<b><u>{} {}</u></b>\n"
        "<b><u>Класс</u></b>: {}\n"
        "<b><u>Телефон</u></b>: {}\n"
        "<b><u>Адресс электронной почты</u></b>: {}\n"
        "<b><u>Статус</u></b>: {}\n"
        "<b><u>Комментарии</u></b>:\n{}";
    inline std::string _get_full_info_teacher =   
       "<b><u>{} {}</u></b>\n"
        "<b><u>Телефон</u></b>: {}\n"
        "<b><u>Адресс электронной почты</u></b>: {}\n"
        "<b><u>Статус</u></b>: {}\n"
        "<b><u>Комментарии</u></b>:\n{}"
    inline std::string _get_full_info_parent =   
        "<b><u>{} {}</u></b>\n"
        "<b><u>Телеграм</u></b>: @{}\n"
        "<b><u>Телефон</u></b>: {}\n";
    inline std::string _get_debts_message =  "<b><u>Ваша задолжность:</u></b>\n";
    inline std::string _get_rescedule_list = 
        "<b><u>Последние переносы занятий:</u></b>\n"; 
    
    inline std::string _get_info_for_teacher =
        "<b>Ученик: {}</b>\n"
        "<b>Время начала занятия: {}</b>\n"
        "<b>Тема урока</b>: {}\n"
        "<b>Комментарий для учителя</b>:\n{}\n"
        "<b>Статус оплаты</b>: {}";
    
    inline std::string _get_info_for_pupil =
        "<b>Учитель: {}</b>\n"
        "<b>Время начала занятия: {}</b>\n"
        "<b>Тема урока</b>: {}\n"
        "<b>Комментарий для ученика</b>:\n{}";

    inline std::string _get_info_for_parent =
        "<b>Учитель: {}</b>\n"
        "<b>Дата</b>: {}\n"
        "<b>Время начала занятия: {}</b>\n"
        "<b>Тема урока</b>: {}\n"
        "<b>Комментарий для родителя</b>:\n{}\n"
        "<b>Статус оплаты</b>: {}";

    inline std::string _get_full_info_lesson =
        "<b>ID: {}</b>\n"
        "<b><u>Учитель</u>: {}</b>\n"
        "<b><u>Ученик</u>: {}</b>\n"
        "<b><u>Дата</u></b>: {}\n"
        "<b><u>Время начала занятия</u>: {}</b>\n"
        "<b><u>Тема урока</u></b>: {}\n"
        "<b><u>Комментарий для родителя</u></b>:\n{}\n"
        "<b><u>Комментарий для ученика</u></b>:\n{}\n"
        "<b><u>Комментарий для учителя</u></b>:\n{}\n" 
        "<b><u>Статус оплаты</u></b>: {}";
 

    inline std::string _get_comment_text =  
        "<b>Ученик: {} {}</b>\n<b>Дата: {}</b>\n\n<b>Ваши комментарии</b>: {}\n";

    inline std::string _get_pupil_info = 
        "<b>{} {} {}</b>\n"
        "<b>Телеграм</b>: {}\n"
        "<b>Телефон</b>: {}\n"
        "<b>Адресс электронной почты</b>:{}\n"
        "<b>Комментарий:</b>{}";

    inline std::string _lesson_delete_request_message = 
        "Ученик {} {} сделал запрос на отмену занятия {} числа в {}";
  
}

namespace FSM_voc
{
    namespace filter_voc
    {
        inline std::string _name_is_ok = "^[А-Яа-я\\s]*$";
        inline std::string _check_class = "^[А-Яа-я0-9\\s]*$";
    };
    namespace lesson_voc
    {
        inline std::string _date_fail = 
            "Ошибка. Выберите сегодняшнюю дату или более позднюю";
        inline std::string _comments_for_parent_ok = 
            "Занятие успешно создано";
        inline std::string _comments_for_parent_fail = 
            "Что-то пошло не так, попробуйте позднее";
        inline std::string _send_update_kb = 
            "Выберите поле для обновления\n{}";
    }

}

#endif
#ifdef EN
const static std::vector<std::string> MONTHS = {
    "January", 
    "February", 
    "March", 
    "April", 
    "May", 
    "June", 
    "July", 
    "August", 
    "September", 
    "October", 
    "November", 
    "December"
};

namespace DB_voc
{   
    inline std::string _error = "Error, try again later";
    inline std::string _active = "active";
    inline std::string _not_active = "not active";
    inline std::string _comment = "Comment: ";
    inline std::string _yes = "yes";
    inline std::string _no = "no";

    inline std::string _db_open_failed = "DB open failed\n";
    inline std::string _db_work_failed =  "Work is failed";
    inline std::string _get_full_info_pupil =   
        "<b><u>{} {}</u></b>\n"
        "<b><u>Class</u></b>: {}\n"
        "<b><u>Phone</u></b>: {}\n"
        "<b><u>Email</u></b>: {}\n"
        "<b><u>Status</u></b>: {}\n"
        "<b><u>Comment</u></b>:\n{}";
    
    inline std::string _get_full_info_teacher =   
       "<b><u>{} {}</u></b>\n"
        "<b><u>Phone</u></b>: {}\n"
        "<b><u>Email</u></b>: {}\n"
        "<b><u>Status</u></b>: {}\n"
        "<b><u>Comment</u></b>:\n{}";
    
    inline std::string _get_full_info_parent =   
        "<b><u>{} {}</u></b>\n"
        "<b><u>Telegram</u></b>: @{}\n"
        "<b><u>Phone</u></b>: {}\n";
    
    inline std::string _get_debts_message =  "<b><u>Your debts:</u></b>\n"; 
    
    inline std::string _get_rescedule_list = 
        "<b><u>Latest reschedules:</u></b>\n"; 

    inline std::string _get_info_for_teacher =
        "<b>Student: {}</b>\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for teacher</b>:\n{}\n"
        "<b>Payment status</b>: {}";

    inline std::string _get_info_for_pupil =
        "<b>Teacher: {}</b>\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for student</b>:\n{}";
    
    inline std::string _get_info_for_parent =
        "<b>Teacher: {}</b>\n"
        "<b>Date</b>: {}\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for parent</b>:\n{}\n"
        "<b>Payment status</b>: {}";

    inline std::string _get_full_info_lesson =
        "<b>ID: {}</b>\n"
        "<b><u>Teacher</u>: {}</b>\n"
        "<b><u>Pupil</u>: {}</b>\n"
        "<b><u>Date</u></b>: {}\n"
        "<b><u>Class start time</u>: {}</b>\n"
        "<b><u>Objectives</u></b>: {}\n"
        "<b><u>Comments for parent</u></b>:\n{}\n"
        "<b><u>Comments for student</u></b>:\n{}\n"
        "<b><u>Comments for teacher</u></b>:\n{}\n" 
        "<b><u>Payment status</u></b>: {}";

    inline std::string _get_comment_text =  
        "<b>Student: {} {}</b>\n<b>Date: {}</b>\n\n<b>Your comments</b>: {}\n";
    
    inline std::string _get_pupil_info = 
        "<b>{} {} {}</b>\n"
        "<b>Username:</b> {}\n"
        "<b>Phone number:</b> {}\n"
        "<b>Email address:</b>{}\n"
        "<b>Comments:</b>{}";
    
     inline std::string _lesson_delete_request_message = 
        "Student {} {} requested to cancel class {} in {}";
       
    
}

namespace FSM_voc
{
    namespace filter_voc
    {
        inline std::string _name_is_ok = "^[A-Za-z\\s]*$";
        inline std::string _check_class = "^[A-Za-z0-9\\s]*$";
    };
    namespace lesson_voc
    {
        inline std::string _date_fail = 
            "Error. Select today's date or a later date";
        inline std::string _comments_for_parent_ok = 
            "Lesson created successfully";
        inline std::string _comments_for_parent_fail = 
            "Something went wrong, try again later";
        inline std::string _send_update_kb = 
            "Choose field to update\n{}";
    };

    namespace user_reg_voc
    {
        
    }
}
#endif
#endif