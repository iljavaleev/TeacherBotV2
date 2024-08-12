#ifndef UserHandlers_hpp
#define UserHandlers_hpp

#include <tgbot/tgbot.h>

using namespace TgBot;
using namespace std;
 
void clear_user_state(long user_id);
void clear_update_user_state(long user_id);

namespace user_handlers
{   
    class user_start_handler{
        const TgBot::Bot& bot;
    public:
        user_start_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
}

namespace user_register_handlers
{   
    class user_registration_handler
    {
        const TgBot::Bot& bot;
    public:
        user_registration_handler(const TgBot::Bot& _bot):bot(_bot){}
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
        const TgBot::Bot& bot;
    public:
        choose_teacher_for_register_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };


   
    class agree_handler
    {
        const TgBot::Bot& bot;
    public:
        agree_handler(const TgBot::Bot& _bot):bot(_bot){}
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
        const TgBot::Bot& bot;
    public:
        get_message_data_for_user_update_handler(
            const TgBot::Bot& _bot
        ):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };
}

#endif