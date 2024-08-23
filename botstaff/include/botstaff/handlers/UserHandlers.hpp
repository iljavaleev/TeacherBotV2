#ifndef UserHandlers_hpp
#define UserHandlers_hpp

#include <tgbot/tgbot.h>
#include "botstaff/States.hpp"

using namespace TgBot;
using namespace std;
 

namespace user_handlers
{   
    class user_start_handler{
        const TgBot::Bot& bot;
    public:
        user_start_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class list_of_student_debts_handler{
        const TgBot::Bot& bot;
    public:
        list_of_student_debts_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

     class list_of_student_rescheduling_handler{
        const TgBot::Bot& bot;
    public:
        list_of_student_rescheduling_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

     class list_of_for_parent_comments_handler{
        const TgBot::Bot& bot;
    public:
        list_of_for_parent_comments_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class content_of_for_parent_comment_handler{
        const TgBot::Bot& bot;
    public:
        content_of_for_parent_comment_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class lesson_delete_request{
        const TgBot::Bot& bot;
    public:
        lesson_delete_request(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
}

namespace user_register_handlers
{   
    class user_registration_handler
    {
    public:
        Message::Ptr operator()(const Message::Ptr&);
    };

    class parent_registration_handler
    {
    public:
        Message::Ptr operator()(const Message::Ptr&);
    };

    class start_register_handler
    {
        const TgBot::Bot& bot;
    public:
        start_register_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

    class choose_teacher_for_register_handler
    {
    public:
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

    class agree_handler
    {
    public:
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

    class start_user_update_handler{
        const TgBot::Bot& bot;
    public:
        start_user_update_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

     class user_update_state_handler{
        const TgBot::Bot& bot;
    public:
        user_update_state_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class get_message_data_for_user_update_handler
    {
    public:
        Message::Ptr operator()(const Message::Ptr&);
    };
}

#endif