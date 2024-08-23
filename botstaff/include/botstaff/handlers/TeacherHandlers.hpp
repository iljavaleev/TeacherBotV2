#ifndef TeacherHandlers_hpp
#define TeacherHandlers_hpp

#include <tgbot/tgbot.h>
#include "botstaff/States.hpp"

using namespace TgBot;
using namespace std;


namespace teacher_handlers
{
    class list_all_teachers_handler{
        const TgBot::Bot& bot;
    public:
        list_all_teachers_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
    
    class list_teachers_handler{
        const TgBot::Bot& bot;
    public:
        list_teachers_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class list_user_handler{
        const TgBot::Bot& bot;
    public:
        list_user_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class list_active_not_handler{
        const TgBot::Bot& bot;
    public:
        list_active_not_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class user_info_handler{
        const TgBot::Bot& bot;
    public:
        user_info_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class activate_user{
        const TgBot::Bot& bot;
    public:
        activate_user(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class delete_user{
        const TgBot::Bot& bot;
    public:
        delete_user(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class comment_text_handler{
        const TgBot::Bot& bot;
    public:
        comment_text_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class list_comments_handler{
        const TgBot::Bot& bot;
    public:
        list_comments_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class list_debts_handler{
        const TgBot::Bot& bot;
    public:
        list_debts_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class change_payment_status_request_handler{
        const TgBot::Bot& bot;
    public:
        change_payment_status_request_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

     class change_payment_status_handler{
        const TgBot::Bot& bot;
    public:
        change_payment_status_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
    
}

namespace lesson
{
    class lesson_creation_handler
    {
        const TgBot::Bot& bot;
    public:
        lesson_creation_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };
    
     class start_lesson_creation_handler
    {
        const TgBot::Bot& bot;
    public:
        start_lesson_creation_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };


    class choose_pupil_for_lesson_creation_handler
    {
        const TgBot::Bot& bot;
    public:
        choose_pupil_for_lesson_creation_handler(
            const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

    class lesson_update_state_handler
    {
        const TgBot::Bot& bot;
    public:
        lesson_update_state_handler(
            const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

    class start_lesson_update_handler
    {
        const TgBot::Bot& bot;
    public:
        start_lesson_update_handler(
            const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

     class get_message_data_for_lesson_update_handler{
        const TgBot::Bot& bot;
    public:
        get_message_data_for_lesson_update_handler(
            const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };

     class get_query_data_for_lesson_update_handler{
        const TgBot::Bot& bot;
    public:
        get_query_data_for_lesson_update_handler(
            const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
    
    class delete_lesson_handler{
        const TgBot::Bot& bot;
    public:
        delete_lesson_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class delete_lesson_reason_handler{
        const TgBot::Bot& bot;
    public:
        delete_lesson_reason_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class one_time_delete_lesson_handler{
        const TgBot::Bot& bot;
    public:
        one_time_delete_lesson_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
}


#endif