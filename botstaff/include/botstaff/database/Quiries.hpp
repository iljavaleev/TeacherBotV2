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
    bot_user as u ON l.pupil=u.chat_id WHERE l.teacher={}  AND  \
    l.comment_for_teacher IS NOT NULL ORDER BY date DESC LIMIT 10;";

    inline std::string _get_debts_for_kb = "SELECT l.id, TO_CHAR(l.date, \
    'dd/mm/yyyy'), u.first_name, u.last_name FROM user_lesson as l JOIN \
    bot_user as u ON l.pupil=u.chat_id WHERE l.teacher={}  AND  \
    l.is_paid=false ORDER BY date;";

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

    inline std::string _change_debt_status =  "UPDATE user_lesson SET \
    is_paid = CASE WHEN is_paid=TRUE THEN FALSE ELSE TRUE END WHERE id = {} \
    RETURNING *;";

    inline std::string _student_exist =  
        "SELECT * FROM bot_user WHERE phone='{}' AND email='{}';";

    inline std::string _get_debts = 
        "SELECT lesson.date FROM user_lesson as lesson \
        JOIN parent_bot_user as u ON u.child=lesson.pupil \
        WHERE lesson.is_paid=FALSE AND u.chat_id={} ORDER BY \
        lesson.date DESC";
    
    inline std::string _get_reschedule = 
        "SELECT date, comment FROM rescedule_table WHERE \
        chat_id=(SELECT child FROM parent_bot_user WHERE chat_id={}) \
        ORDER BY date DESC LIMIT 5";
    
    inline std::string _get_parent_comments = 
        "SELECT * FROM user_lesson WHERE \
        pupil=(SELECT child FROM parent_bot_user WHERE chat_id={}) \
        ORDER BY date DESC LIMIT 5";

    inline std::string _lesson_delete_request_message = 
        "SELECT l.teacher, u.first_name, u.last_name, \
        TO_CHAR(l.date, 'dd/mm/yyyy'), l.time \
        FROM bot_user as u JOIN (SELECT pupil, teacher, date, time FROM \
        user_lesson WHERE id={}) as l ON u.chat_id=l.pupil;"; 

    inline std::string _create_reschedule= 
        "INSERT INTO rescedule_table VALUES \
        (DEFAULT, {}, '{}', '{}');";
}   

#endif