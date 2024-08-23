#include "botstaff/FSM/LessonCreation.hpp"

#ifdef EN 
#define SIZE 1
#endif
#ifdef RU
#define SIZE 2
#endif


using namespace TgBot;


void CreateLesson::choose_date(const std::string& date)
{   
    filter_sender.send(msg::create_lesson::check_date(date, messanger));

    messanger.
    wait().
    handle<msg::create_lesson::date_ok>(
        [&](const msg::create_lesson::date_ok& msg)
    {
        state = &CreateLesson::choose_pupil;
        lesson->date = date;
        send_message_with_kb(
            bot, 
            lesson->teacher, 
            lesson_messges::_pupil, 
            teacherKeyboards::create_pupil_for_lesson_kb(lesson->teacher));
        
    }).
    handle<msg::create_lesson::date_fail>(
        [&](const msg::create_lesson::date_fail& msg)
    {
        send_error_message(
            bot, 
            lesson->teacher, 
            FSM_voc::lesson_voc::_date_fail
        );
    });  
}

void CreateLesson::choose_pupil(const std::string& pupil)
{   
    state = &CreateLesson::set_time;
    lesson->pupil = std::stol(pupil);
    send_message(bot, lesson->teacher, lesson_messges::_time);
}

void CreateLesson::set_time(const std::string& time)
{   
    filter_sender.send(msg::create_lesson::check_time(time, messanger));

    messanger.
    wait().
    handle<msg::create_lesson::time_ok>(
        [&](const msg::create_lesson::time_ok& msg)
    {
        state = &CreateLesson::set_objectives;
        lesson->time = time;
        send_message(bot, lesson->teacher,  lesson_messges::_objectives);
    }).
    handle<msg::create_lesson::time_fail>(
        [&](const msg::create_lesson::time_fail& msg)
    {
        send_error_message(
            bot, 
            lesson->teacher
        );

    });  
}

void CreateLesson::set_objectives(const std::string& objectives)
{   
    state = &CreateLesson::comments_for_pupil;
    if (objectives.size() > SIZE)
        lesson->objectives = objectives;
    send_message(bot, lesson->teacher,  lesson_messges::_comments_for_pupil);
}

void CreateLesson::comments_for_pupil(const std::string& comments)
{   
    state = &CreateLesson::comments_for_teacher;
    if(comments.size() > SIZE)
        lesson->comment_for_pupil = comments;
    send_message(bot, lesson->teacher,  lesson_messges::_comments_for_teacher);
}

void CreateLesson::comments_for_teacher(const std::string& comments)
{   
    state = &CreateLesson::comments_for_parent;
    if(comments.size() > SIZE)
    {   
        lesson->comment_for_teacher = comments;
    }
        
    send_message(bot, lesson->teacher, lesson_messges::_comments_for_parent);
}

void CreateLesson::comments_for_parent(const std::string& comments)
{   
    if(comments.size() > SIZE)
        lesson->comment_for_parent = comments;
    long chat_id = lesson->teacher;
    try
    {
        lesson = lesson->create();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    std::string message{};
    if (lesson)
    {
        message = FSM_voc::lesson_voc::_comments_for_parent_ok;
    }
    else
    {
        message = FSM_voc::lesson_voc::_comments_for_parent_fail;
    }
    send_message(bot, chat_id, std::move(message));
    send_message(
        bot, 
        lesson->pupil, 
        std::vformat(
            FSM_voc::lesson_voc::_get_info_for_pupil, 
            std::make_format_args(
                lesson->date, 
                lesson->time, 
                lesson->objectives, 
                lesson->comment_for_pupil)
        )
    );
}


void CreateLesson::run(const std::string& message) 
{
    try
    {
        (this->*state)(message);
    }
    catch(const messaging::close_queue& )
    {}
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }       
}

void UpdateLesson::send_update_kb()
{
    send_message_with_kb(
        bot,
        lesson_info->lesson->teacher, 
        std::vformat(FSM_voc::lesson_voc::_send_update_kb, 
            std::make_format_args(
                lesson_info->get_full_info()
            )
        ),
        teacherKeyboards::update_lesson_info_kb(lesson_info->lesson->id)
    );
}

void UpdateLesson::choose_date(const std::string& date)
{   
    filter_sender.send(msg::create_lesson::check_date(date, messanger));

    messanger.
    wait().
    handle<msg::create_lesson::date_ok>(
        [&](const msg::create_lesson::date_ok& msg)
    {
        lesson_info->set_date(date);
        send_update_kb();
    }).
    handle<msg::create_lesson::date_fail>(
        [&](const msg::create_lesson::date_fail& msg)
    {
        send_error_message(
            bot, 
            lesson_info->lesson->teacher, 
            FSM_voc::lesson_voc::_date_fail
        );
    });  
}

void UpdateLesson::choose_pupil(const std::string& pupil)
{   
    lesson_info->set_pupil(std::stol(pupil));
    send_update_kb();
}

void UpdateLesson::set_time(const std::string& time)
{   
    filter_sender.send(msg::create_lesson::check_time(time, messanger));

    messanger.
    wait().
    handle<msg::create_lesson::time_ok>(
        [&](const msg::create_lesson::time_ok& msg)
    {
        lesson_info->set_time(time);
        send_update_kb();
    }).
    handle<msg::create_lesson::time_fail>(
        [&](const msg::create_lesson::time_fail& msg)
    {
        send_error_message(bot, lesson_info->lesson->teacher);
    });  
}

void UpdateLesson::set_objectives(const std::string& objectives)
{   
    if (objectives.size() > SIZE)
        lesson_info->set_objectives(objectives);
    send_update_kb();
}

void UpdateLesson::comments_for_pupil(const std::string& comments)
{   
    if(comments.size() > SIZE)
        lesson_info->set_comment_for_pupil(comments);
    send_update_kb();
}

void UpdateLesson::comments_for_teacher(const std::string& comments)
{   
    if(comments.size() > SIZE)
        lesson_info->set_comment_for_teacher(comments);
    send_update_kb();
}

void UpdateLesson::comments_for_parent(const std::string& comments)
{   
    if(comments.size() > SIZE)
        lesson_info->set_comment_for_parent(comments);
    send_update_kb();
}

void UpdateLesson::is_paid(const std::string& is_paid)
{   
    if(is_paid == FSM_voc::_change_status_y)
        lesson_info->set_is_paid(true);
    send_update_kb();
}


void UpdateLesson::run(const std::string& message) 
{
    try
    {
        (this->*state)(message);
    }
    catch(const messaging::close_queue& )
    {}
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }       
}

void UpdateLesson::change_state(const std::string& message)
{
    if(states.contains(message))
        state = states.at(message);
}

typedef void (UpdateLesson::*callable)(const std::string&);
std::unordered_map<std::string, callable> UpdateLesson::states = { 
    {"date", &UpdateLesson::choose_date},
    { "pupil", &UpdateLesson::choose_pupil},
    { "objectives", &UpdateLesson::set_objectives },
    { "time",  &UpdateLesson::set_time },
    { "comment_for_pupil", &UpdateLesson::comments_for_pupil },
    {"comment_for_teacher",  &UpdateLesson::comments_for_teacher },
    {"comment_for_parent", &UpdateLesson::comments_for_parent },
    {"is_paid", &UpdateLesson::is_paid }
};