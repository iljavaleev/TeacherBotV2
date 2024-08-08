#ifndef Quiries_hpp
#define Quiries_hpp
#include <string>

namespace other_quiries
{
    inline std::string _get_lesson_days = "SELECT EXTRACT('day' from date) \
    FROM user_lesson WHERE EXTRACT('year' from date)={} AND \
    EXTRACT('month' from date)={} AND {}={};";

    inline std::string _get_lessons_by_day =  "SELECT u.first_name, \
    u.last_name, l.id, l.time FROM bot_user as u JOIN (SELECT {}, id, time \
    FROM user_lesson WHERE {}={} AND date='{}') as l ON u.chat_id=l.{};"; 

    inline std::string _get_comment_text =  "SELECT u.first_name, u.last_name, \
    TO_CHAR(l.date, 'dd/mm/yyyy'), l.comment_for_teacher FROM user_lesson as l \
    JOIN bot_user as u ON l.pupil=u.chat_id WHERE l.id={};";
    
    inline std::string _get_comments_for_kb = "SELECT l.id, TO_CHAR(l.date, \
    'dd/mm/yyyy'), u.first_name, u.last_name FROM user_lesson as l JOIN \
    bot_user as u ON l.pupil=u.chat_id WHERE l.teacher={} \
    ORDER BY date DESC LIMIT 10;";

    inline std::string _create_teachers_list_kb = 
    "SELECT * FROM bot_user WHERE user_role=1 AND is_active={} \
    ORDER BY last_name;";

    inline std::string _create_all_active_teachers_list_kb = 
    "SELECT * FROM bot_user WHERE (user_role=1 OR user_role=3) AND \
    is_active=true ORDER BY last_name;";

    inline std::string _create_pupils_list_kb = "SELECT * FROM bot_user WHERE \
    teacher={} AND is_active={} ORDER BY last_name";

    inline std::string _create_pupil_for_lesson_kb = "SELECT * FROM bot_user \
    WHERE teacher={} AND is_active=true ORDER BY last_name";
    
     inline std::string _lesson_info = "SELECT first_name, last_name FROM \
     bot_user WHERE chat_id={} OR chat_id={};";
}

#endif