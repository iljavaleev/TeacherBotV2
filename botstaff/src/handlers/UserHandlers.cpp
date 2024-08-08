#include "botstaff/handlers/UserHandlers.hpp"

#include <tgbot/tgbot.h>

#include "botstaff/utils/Utils.hpp"
#include "botstaff/handlers/Handlers.hpp"
#include "botstaff/keyboards/UserKeyboards.hpp"

using namespace TgBot;
using namespace std;


namespace UserHandlers
{
    Message::Ptr user_start_handler::operator()(const CallbackQuery::Ptr& query)
    {
        if (query->data == "register") 
        {
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id, 
                "Choose role",
                UserKeyboards::choose_role_kb(),
                "HTML"  
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }
}