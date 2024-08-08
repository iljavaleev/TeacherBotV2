#ifndef Handlers_hpp
#define Handlers_hpp

#include <tgbot/tgbot.h>

using namespace TgBot;
using namespace std;

void clear_user_state(long user_id);
void clear_lesson_state(long user_id);

namespace CommandHandlers
{
    class start_command{
        const TgBot::Bot& bot;
    public:
        start_command(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };

    class cancel_command{
        const TgBot::Bot& bot;
    public:
        cancel_command(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };
   
};

namespace Handlers
{   

    class any_message_handler
    {
        const TgBot::Bot& bot;
    public:
        any_message_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };

    class user_registration_handler
    {
        const TgBot::Bot& bot;
    public:
        user_registration_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };

    class lesson_creation_handler
    {
        const TgBot::Bot& bot;
    public:
        lesson_creation_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };

    class start_register_handler
    {
        const TgBot::Bot& bot;
    public:
        start_register_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
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

    class agree_handler
    {
        const TgBot::Bot& bot;
    public:
        agree_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

    class calendar_handler
    {
        const TgBot::Bot& bot;
    public:
        calendar_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr& query);
    };

     class next_month_handler{
        const TgBot::Bot& bot;
    public:
        next_month_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
    

    class calendar_day_handler{
        const TgBot::Bot& bot;
    public:
        calendar_day_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class day_info_handler{
        const TgBot::Bot& bot;
    public:
        day_info_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
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
};

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl);

#endif