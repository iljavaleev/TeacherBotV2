#ifndef LessonCreation_hpp
#define LessonCreation_hpp

#include "botstaff/FSM/Headers.hpp"
#include "botstaff/utils/Utils.hpp"
#include "botstaff/Vocabular.hpp"
#include <functional>
#include <unordered_map>

using namespace TgBot;



inline std::unordered_map<std::string, std::string> lesson_creation_messages = 
{
    {"date", lesson_messges::_date},
    {"pupil", lesson_messges::_pupil},
    {"time", lesson_messges::_time},
    {"objectives", lesson_messges::_objectives},
    {"comment_for_pupil", lesson_messges::_comments_for_pupil},
    {"comment_for_teacher", lesson_messges::_comments_for_teacher},
    {"comment_for_parent", lesson_messges::_comments_for_parent},
    {"is_paid", lesson_messges::_paid},
    {"finish_lesson_update", lesson_messges::_finish}
};


class CreateLesson
{
    messaging::receiver messanger;
    messaging::sender filter_sender;
    std::shared_ptr<UserLesson> lesson;
    void (CreateLesson::* state)(const std::string&);
    const TgBot::Bot& bot;
    
    void choose_date(const std::string& date);
    void choose_pupil(const std::string& pupil);
    void set_time(const std::string& time);
    void set_objectives(const std::string& objectives);
    void comments_for_pupil(const std::string& comments);
    void comments_for_teacher(const std::string& comments);
    void comments_for_parent(const std::string& comments);

    CreateLesson(const CreateLesson&) = delete;
    CreateLesson& operator=(const CreateLesson&) = delete;
public:
    CreateLesson(
        const messaging::sender& _fs,
        const TgBot::Bot& _bot,
        long chat_id):
            filter_sender(_fs),
            bot(_bot)
    {    
        lesson = std::make_shared<UserLesson>();
        lesson->teacher = chat_id;
        state = &CreateLesson::choose_date;
    }
    void run(const std::string& message);
    messaging::sender get_sender(){ return messanger; }
};



class UpdateLesson
{
    messaging::receiver messanger;
    messaging::sender filter_sender;
    const TgBot::Bot& bot;
    std::shared_ptr<LessonInfo> lesson_info;
    void (UpdateLesson::* state)(const std::string&);
    
    
    typedef void (UpdateLesson::*callable)(const std::string&);
    static std::unordered_map<std::string, callable> states;
    
    void choose_date(const std::string& date);
    void choose_pupil(const std::string& pupil);
    void set_time(const std::string& time);
    void set_objectives(const std::string& objectives);
    void comments_for_pupil(const std::string& comments);
    void comments_for_teacher(const std::string& comments);
    void comments_for_parent(const std::string& comments);
    void is_paid(const std::string& is_paid);

    void send_update_kb();
    
    UpdateLesson(const UpdateLesson&) = delete;
    UpdateLesson& operator=(const UpdateLesson&) = delete;
public:
    UpdateLesson(
        const messaging::sender& _fs,
        const TgBot::Bot& _bot,
        std::shared_ptr<LessonInfo> _lesson
        ):
            filter_sender(_fs),
            bot(_bot),
            lesson_info(_lesson)
    {}
    std::shared_ptr<UserLesson> get_instance()
    { 
        return lesson_info->lesson; 
    }
    void change_state(const std::string& message);
    void run(const std::string& message);
    messaging::sender get_sender(){ return messanger; }
};

#endif