#ifndef Vocabular_hpp
#define Vocabular_hpp
#include "botstaff/database/DB.hpp"

#include <vector>
#include <string>

#define RU

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
        "<b><u>Комментарии</u></b>:\n{}";
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
        "<b>Телеграм</b>: @{}\n"
        "<b>Телефон</b>: {}\n"
        "<b>Адресс электронной почты</b>:{}\n"
        "<b>Комментарий:</b>{}";

    inline std::string _lesson_delete_request_message = 
        "Ученик {} {} сделал запрос на отмену занятия {} числа в {}";
  
}

namespace FSM_voc
{
    inline std::string _change_status_y = "д";
    inline std::string _change_status_n = "н";
    
    namespace filter_voc
    {
        inline std::string _name_is_ok = "^[\u0400-\u04FF\\s]+";
        inline std::string _check_class = "^[\u0400-\u04FF0-9\\s]+";
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
        inline std::string _get_info_for_pupil =
            "<b>Для вас запланировано занятие на {}</b>\n"
            "<b>Время начала занятия: {}</b>\n"
            "<b>Тема урока</b>: {}\n"
            "<b>Комментарий для ученика</b>:\n{}";
    }
    
    namespace user_reg_voc
    {
        inline std::string _cls = 
            "Введите класс обучения с литерой (8а, 5б и т.д)";
        inline std::string _first_name = "Введите ваше имя";
        inline std::string _last_name = "Введите вашу фамилию";
        inline std::string _phone = "Введите номер вашего телефона";
        inline std::string _email = "Введите адрес электронной почты";
        inline std::string _agreement = 
            "Вы даете согласие на хранение и обработку персональных данных?";
        inline std::string _finish_no = 
            "<b>Регистрация не завершена<\b>";          
        inline std::string _finish_yes = 
            "Вы успешно прошли регистрацию. Перейдите в меню"; 
        inline std::string _finish_error = 
            "Что-то пошло не так, попробуйте позднее"; 
        
        inline std::string _update_kb =  "Выберите поле для обновления\n{}";
        inline std::string _child_phone = "Введите номер телефона вашего ребенка";
        inline std::string _child_not_found = 
            "Извините, ученика с таким телефоном и адресом почты нет в нашей базе. " 
            "Уточните информацию и повторите попытку позднее";
        
    }

}
namespace handlers_voc
{
    namespace handlers
    {
        inline std::string _menu_adm = "Меню администратора";
        inline std::string _menu_tchr = "Меню учителя";
        inline std::string _menu_par = "Меню родителя";
        inline std::string _menu_tchr_not = 
            "Пожалуйста, подождите, когда администратор активирует ваш аккаунт, вам придет уведомление";
        inline std::string _menu_pup_not = 
            "Пожалуйста, подождите, когда ваш учитель активирует аккаунт, вам придет уведомление";
        inline std::string _menu_anon = 
            "Меню для незарегестрированных пользователей";

        inline std::string _cancel = "Кнопка отмены";
        inline std::string _next_month = "<b><i>Расписание на {}</i></b>";
        inline std::string _calendar_day = "Занятия на сегодня";
        inline std::string _calendar_day_not = "Сегодня нет занятий";
    };
    namespace teacher
    {
        inline std::string _comments = "Ваши последние комментарии";
        inline std::string _debts = "Список задолжностей студентов";
        inline std::string _payment = "Изменить статус опланы?";
        inline std::string _change_debt_status = "Статус был изменен";
        inline std::string _change_debt_status_not = "Ok";
        inline std::string _error = "Что-то пошло не так, попробуйте позднее";
        inline std::string _category = "Выберите категорию";
        inline std::string _teacher_active = "Список активных учителей";
        inline std::string _teacher_not_active = "Список кандидатов";
        inline std::string _empty = "Пока здесь пусто";
        inline std::string _student_active = "Список активных учеников";
        inline std::string _student_not_active = "Список кандидатов";
        inline std::string _reg_to_user = "Вы завершили регистрацию";
        inline std::string _reg_user =  "Пользователь добавлен";
        inline std::string _delete_user =  "Пользователь был удален";
        inline std::string _update_user =  "Выберите поле для исправления\n{}";
        inline std::string _delete_lesson_reason = "Выберите причину отмены урока";
        inline std::string _delete_lesson = "Урок отменен";
        inline std::string _cancel_lesson_for_pupil = "Ваш запрос одобрен";
    };

    namespace user
    {
        inline std::string _parent_comments = "<b><u>Выбор коментариев по дате</u></b>";
        inline std::string _lesson_request = "Мы отправили ваш запрос учителю";
        inline std::string _child_email = "Ввведите электронный адрес ребенка";
        inline std::string _choose_teacher = "<b>Выберите вашего преподавателя</b>";
        inline std::string _name = "<b>Введите имя</b>";
        inline std::string _update_user =  "Выберите поле для исправления\n{}";
        inline std::string _role = "<b>Выберите роль</b>";
    };
}

namespace keyboards_voc
{
    namespace keyboards
    {
        inline std::vector<std::string> DAYS = { 
            "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс" 
        };

        inline std::string _yes = "Да";
        inline std::string _no = "Нет";
        inline std::string _today = "Сегодня {}/{}/{}";
        inline std::string _day_cal = "Календарь";
        inline std::string _pup = "Список учеников";
        inline std::string _create_lesson = "Создать урок";
        inline std::string _day_reschedule = "Запрос на перенос занятия";
        inline std::string _lesson_edit = "Редактировать";
        inline std::string _lesson_delete = "Отменить";
    };

    namespace teacher
    {
        inline std::string _yes = "Да";
        inline std::string _no = "Нет";
        inline std::string _today = "Сегодня {}/{}/{}";
        inline std::string _teach = "Список учителей";
        inline std::string _comments = "Комментарии";
        inline std::string _debts = "Задолжность";

        inline std::string _list_stud_active = "Список активных учеников";
        inline std::string _list_not_active = "Список кандидатов";

        inline std::string _list_teach_active = "Список активных учителей";
        inline std::string _activate = "Активировать";
        inline std::string _user_edit = "Редактировать";
        inline std::string _user_delete = "Удалить";
        
        inline std::string _kb_name = "Изменить имя";
        inline std::string _kb_sname = "Изменить фимилию";
        inline std::string _kb_class = "Изменить класс обучения";
        inline std::string _kb_phone = "Изменить номер телефона";
        inline std::string _kb_email = "Изменить адресс эл.почты";
        inline std::string _kb_comment = "Изменить комментарий";
        inline std::string _kb_status = "Изменить статус";
        inline std::string _kb_finish = "Закончить редактирование";

        inline std::string _lkb_date = "Изменить дату";
        inline std::string _lkb_time = "Изменить время";
        inline std::string _lkb_std = "Изменить ученика";
        inline std::string _lkb_obj = "Изменить тему урока";
        inline std::string _lkb_comment_pup = "Изменить комментарий для ученика";
        inline std::string _lkb_comment_par = "Изменить комментарий для родителя";
        inline std::string _lkb_comment_teach = "Изменить комментарий для учителя";
        inline std::string _lkb_status = "Изменить статус оплаты";
        inline std::string _lkb_finish = "Закончить редактирование";

        inline std::string _cancel_reason_s = "Запрос ученика";
        inline std::string _cancel_reason_o = "Другие причины";
    };

    namespace user
    {
        inline std::string _reg = "Регистрация";
        inline std::string _parent_resc = "Список переносов занятий";
        inline std::string _parent_comm = "Комментарии преподователя";
        inline std::string _parent_debts = "Задолжности по оплате";

        inline std::string _role_std = "Ученик";
        inline std::string _role_teach = "Учитель";
        inline std::string _role_par = "Родитель";
        inline std::string _delete = "Отменить занятие";
    };
}
namespace utils_voc
{
    inline std::string _techer_info =  
        "<b>{} {}</b>\n"
        "<b>Телеграм</b>: @{}\n"
        "<b>Телефонный номер</b>: {}\n"
        "<b>Имейл</b>: {}\n"
        "<b>Комментарий:</b> {}";
    
    inline std::string _pupil_info = 
        "<b><u>{} {}. Класс: {}</u></b>\n<b><u>Телеграм</u></b>: @{}\n"
        "<b><u>Телефонr</u></b>: {}\n<b><u>Имейл</u></b>: {}\n"
        "<b><u>Комментарий:</u></b> {}";

    inline std::string _cal = "<b><i>Расписание на {}</i></b>";

    inline std::string _error = "Вы ввели некорректные данные. Попробуйте снова";
}

namespace lesson_messges
{
    inline std::string _date = "Выберите новую дату урока";
    inline std::string _pupil = "Выберите ученика";
    inline std::string _time = 
        "Установите время начала занятия (например 12 00, 12-00, и т.д)";
    inline std::string _objectives = 
        "Установите тему занятия или отправьте любой символ, чтобы пропустить";
    inline std::string _comments_for_pupil = 
        "Введите информацию для ученика или отправьте любой символ, чтобы пропустить";
    inline std::string _comments_for_parent = 
        "Введите информацию для родителя или отправьте любой символ, чтобы пропустить";
    inline std::string _comments_for_teacher = 
        "Введите информацию для учителя или отправьте любой символ, чтобы пропустить";
    inline std::string _paid = "Урок оплачен: д/н";
    inline std::string _finish = "Редактирование завершено";
}

namespace user_reg_messages
{
    inline std::string _first_name = "Введите имя";
    inline std::string _last_name = "Введите фамилию";
    inline std::string _class = "Установите класс обучения ученика";
    inline std::string _phone = "Введите номер телефона";
    inline std::string _email= "Введите номер имейл";
    inline std::string _comments = "Введите комментарий о пользователе";
    inline std::string _status = "Пользователь активен? д/н";
    inline std::string _finish = "Редактирование завершено";
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
    inline std::string _change_status_y = "y";
    inline std::string _change_status_n = "n";
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
        inline std::string _get_info_for_pupil =
            "<b>You have a class scheduled for {}</b>\n"
            "<b>Class start time: {}</b>\n"
            "<b>Lesson topic</b>: {}\n"
            "<b>Comment for student</b>:\n{}"
    };

    namespace user_reg_voc
    {
        inline std::string _cls = "Enter your class";
        inline std::string _first_name = "Enter your first name";
        inline std::string _last_name = "Enter your last name";
        inline std::string _phone = "Enter your phone number";
        inline std::string _email = "Enter your email";
        inline std::string _agreement = 
            "You consent to the processing of personal data?";
        inline std::string _finish_no = 
            "<b>Registration is not completed<\b>";          
        inline std::string _finish_yes = 
            "Registration completed successfully. Go to menu"; 
        inline std::string _finish_error = 
            "Something is wrong, try again later"; 
        
        inline std::string _update_kb =  "Choose field to update\n{}";
        inline std::string _child_phone = "Enter your child phone number";
        inline std::string _child_not_found = 
            "Sorry, there is no student with this " 
            "phone number and email address. Check the information";
    }
}

namespace handlers_voc
{
    namespace handlers
    {
        inline std::string _menu_adm = "Admin menu";
        inline std::string _menu_tchr = "Teacher menu";
        inline std::string _menu_par = "Parent menu";
        inline std::string _menu_tchr_not = "Please wait for administrator to activate your account";
        inline std::string _menu_pup_not = "Please wait for your teacher to activate your account";
        inline std::string _menu_anon = "Unregistred users menu";

        inline std::string _cancel = "Cancel command";
        inline std::string _next_month = "<b><i>Schedule for {}</i></b>";
        inline std::string _calendar_day = "Today's classes";
        inline std::string _calendar_day_not = "No classes today";
    };
    namespace teacher
    {
        inline std::string _comments = "Your latest comments";
        inline std::string _debts = "List of student debts";
        inline std::string _payment = "Change payment status?";
        inline std::string _change_debt_status = "Satus was changed successfully";
        inline std::string _change_debt_status_not = "Ok";
        inline std::string _error = "Somthing went wrong, try later";
        inline std::string _category = "Choose category";
        inline std::string _teacher_active = "List of active teachers";
        inline std::string _teacher_not_active = "List of candidats";
        inline std::string _empty = "It's empty for now";
        inline std::string _student_active = "List of students";
        inline std::string _student_not_active = "List of candidats";
        inline std::string _reg_to_user =  "You are registered";
        inline std::string _reg_user =  "User added";
        inline std::string _delete_user =  "User was removed";
        inline std::string _update_user =  "Choose field to update\n{}";
        inline std::string _delete_lesson_reason = "Select the reason for canceling the lesson";
        inline std::string _delete_lesson = "You canceled your lesson";
        inline std::string _cancel_lesson_for_pupil = "Your request has been approved";
    };

    namespace user
    {
        inline std::string _parent_comments = "<b><u>Select comment by date</u></b>";
        inline std::string _lesson_request = "We have sent your request to the teacher";
        inline std::string _child_email = "Enter your child's email";
        inline std::string _choose_teacher = "<b>Choose your teacher</b>";
        inline std::string _name = "<b>Enter your name</b>";
        inline std::string _update_user =  "Choose field to update\n{}";
        inline std::string _role =  "<b>Choose role</b>";
    };
}

namespace keyboards_voc
{
    namespace keyboards
    {
        inline std::vector<std::string> DAYS = { 
            "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" 
        };

        inline std::string _yes = "Yes";
        inline std::string _no = "No";
        inline std::string _today = "Today {}/{}/{}";
        inline std::string _day_cal = "Calendar";
        inline std::string _pup = "List of students";
        inline std::string _create_lesson = "Create new lesson";
        inline std::string _day_reschedule = "Request to reschedule a class";
        inline std::string _lesson_edit = "Edit";
        inline std::string _lesson_delete = "Delete";
    };

    namespace teacher
    {
        inline std::string _yes = "Yes";
        inline std::string _no = "No";
        inline std::string _today = "Today {}/{}/{}";
        inline std::string _teach = "List of teachers";
        inline std::string _comments = "Comments";
        inline std::string _debts = "Debt list";

        inline std::string _list_stud_active = "List of active students";
        inline std::string _list_not_active = "Request list";

        inline std::string _list_teach_active = "List of active teachers";
        inline std::string _activate = "Activate";
        inline std::string _user_edit = "Edit";
        inline std::string _user_delete = "Delete";
        
        inline std::string _kb_name = "Change name";
        inline std::string _kb_sname = "Change surname";
        inline std::string _kb_class = "Change class";
        inline std::string _kb_phone = "Change phone";
        inline std::string _kb_email = "Change email";
        inline std::string _kb_comment = "Change comment";
        inline std::string _kb_status = "Change status";
        inline std::string _kb_finish = "Finish editing";

        inline std::string _lkb_date = "Change date";
        inline std::string _lkb_time = "Change time";
        inline std::string _lkb_std = "Change student";
        inline std::string _lkb_obj = "Change objectives";
        inline std::string _lkb_comment_pup = "Change comments for pupil";
        inline std::string _lkb_comment_par = "Change comments for parent";
        inline std::string _lkb_comment_teach = "Change comments for teacher";
        inline std::string _lkb_status = "Change payment status";
        inline std::string _lkb_finish = "Finish editing";

        inline std::string _cancel_reason_s = "Student request";
        inline std::string _cancel_reason_o = "Other reasons";
    };

    namespace user
    {
        inline std::string _reg = "Registration";
        inline std::string _parent_resc = "List of class rescheduling";
        inline std::string _parent_comm = "List of teacher comments";
        inline std::string _parent_debts = "Student debts";

        inline std::string _role_std = "Student";
        inline std::string _role_teach = "Teacher";
        inline std::string _role_par = "Parent";
        inline std::string _delete = "Cancel lesson";
    };

}

namespace utils_voc
{
    inline std::string _techer_info =  
        "<b>{} {}</b>\n"
        "<b>Username</b>: {}\n"
        "<b>Phone number</b>: {}\n"
        "<b>Email address</b>: {}\n"
        "<b>Comments:</b> {}";
    
    inline std::string _pupil_info = 
        "<b><u>{} {}. Class: {}</u></b>\n<b><u>Username</u></b>: @{}\n"
        "<b><u>Phone number</u></b>: {}\n<b><u>Email address</u></b>: {}\n"
        "<b><u>Comments:</u></b> {}";

    inline std::string _cal = "<b><i>Schedule for {}</i></b>";

    inline std::string _error = "You entered incorrect information. Try again";
}

namespace lesson_messges
{
    inline std::string _date = "Choose new date for class";
    inline std::string _pupil = "Choose student for this lesson";
    inline std::string _time = 
        "Set lesson start time (like 12 00, 12-00, etc)";
    inline std::string _objectives = 
        "Set objectives for this lesson or send random character";
    inline std::string _comments_for_pupil = 
        "Enter information for pupil or send random character to skip";
    inline std::string _comments_for_parent = 
        "Enter information for parent or send random character to skip";
    inline std::string _comments_for_teacher = 
        "Enter information for teacher or send random character to skip";
    inline std::string _paid = "Service paid: y/n";
    inline std::string _finish = "Editing was comleted";
}

namespace user_reg_messages
{
    inline std::string _first_name = "Enter name";
    inline std::string _last_name = "Enter surname";
    inline std::string _class = "Set student's class";
    inline std::string _phone = "Set phone number";
    inline std::string _email= "Set email address";
    inline std::string _comments = "Enter comments about student";
    inline std::string _status = "Activate user? y/n";
    inline std::string _finish = "Editing was comleted";
}

#endif
#endif