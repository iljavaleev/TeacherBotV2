#include "botstaff/handlers/Handlers.hpp"

#include <unordered_map>
#include <string>
#include <exception>
#include <iostream>
#include <memory>
#include <future>
#include <thread>

#include "botstaff/FSM/LessonCreation.hpp"
#include "botstaff/FSM/UserRegistration.hpp"
#include "botstaff/FSM/Filter.hpp"
#include "botstaff/keyboards/Keyboards.hpp"
#include "botstaff/keyboards/TeacherKeyboards.hpp"
#include "botstaff/keyboards/UserKeyboards.hpp"
#include "botstaff/utils/Utils.hpp"
#include "botstaff/Vocabular.hpp"

#include <thread>

#include <tgbot/tgbot.h>
#include "botstaff/handlers/TeacherHandlers.hpp"
#include "botstaff/handlers/UserHandlers.hpp"

using namespace TgBot;


namespace command_handlers
{
    Message::Ptr start_command::operator()(const Message::Ptr& message)
    {
        long chat_id(message->chat->id);
        clear_user_state(chat_id);
        clear_update_user_state(chat_id);
        clear_lesson_state(chat_id);
        clear_lesson_update_state(chat_id);
        clear_parent_registration_state(chat_id);
        bot_roles role = get_role(chat_id);
        
        std::string mes{};
        InlineKeyboardMarkup::Ptr kb(nullptr);
        if (role == bot_roles::admin)
        {
            mes = "Admin menu";
            kb = teacherKeyboards::create_teacher_start_kb(true);
        }
        else if(role == bot_roles::teacher)
        {
            mes = "Teacher menu";
            kb = teacherKeyboards::create_teacher_start_kb(false);
        }
        else if(role == bot_roles::pupil)
        {
            send_current_calendar(bot, chat_id, role);
            return Message::Ptr(nullptr);
        }
        else if(role == bot_roles::parent)
        {
            mes = "Parent menu";
            kb = UserKeyboards::create_parent_start_kb(chat_id);
        }
        else if(role == bot_roles::teacher_not_active)
        {
            mes = "Please wait for administrator to activate your account";
        }
        else if(role == bot_roles::pupil_not_active)
        {
            mes ="Please wait for your teacher to activate your account";
        }
        else
        {
            mes = "Unregistred users menu";
            kb = UserKeyboards::create_user_start_kb(chat_id);
        } 
  
        std::thread send(
            send_message_with_kb,
            std::ref(bot), 
            chat_id, 
            mes,
            kb,
            "HTML"
        ); 
        send.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr cancel_command::operator()(const Message::Ptr& message)
    {
        long chat_id(message->chat->id);
        clear_user_state(chat_id);
        clear_update_user_state(chat_id);
        clear_lesson_state(chat_id);
        clear_lesson_update_state(chat_id);
        clear_parent_registration_state(chat_id);
        std::thread send(
            send_message,
            std::ref(bot), 
            chat_id,
            "Cancel command",
            "HTML"
        ); 
        send.detach();
        return Message::Ptr(nullptr);
    }
}


namespace handlers
{
    Message::Ptr calendar_handler::operator()(const CallbackQuery::Ptr& query)
    {
       if (StringTools::split(query->data, ' ').at(0) == "calendar") 
        {
            bot_roles role = 
                StringTools::split(query->data, ' ').at(1) == 
                    "teacher" ? bot_roles::teacher : bot_roles::pupil;
            send_current_calendar(bot, query->message->chat->id, role);
        }
        return Message::Ptr(nullptr); 
    }
 
    Message::Ptr next_month_handler::operator()(const CallbackQuery::Ptr& query)
    {
        if (StringTools::split(query->data, ' ').at(0) == "change_month")
        {
            auto info = StringTools::split(query->data, ' ');
            YMD ymd{ stoi(info.at(2)), stoi(info.at(3)), 1};
            bool update = true ? info.at(4) == "true" : false;
            bot_roles role = 
                info.at(5) == 
                    "1" ? bot_roles::teacher : bot_roles::pupil;
            
            if (info.at(1) == "<<")
            {   
                ymd.month--;
            }
            else
            {
                ymd.month++;
            }
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                std::format(
                    "<b><i>Schedule for {}</i></b>", 
                    MONTHS.at(ymd.month-1)
                ),
                Keyboards::calendar_kb()(
                    ymd,
                    role, 
                    query->message->chat->id, 
                    update
                ),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr); 
    }

    Message::Ptr calendar_day_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::split(query->data, ' ').at(0) == "calendar_day") 
        {
            auto info = StringTools::split(query->data, ' ');
            
            YMD ymd{ stoi(info.at(2)), stoi(info.at(3)), stoi(info.at(4))};
            
            auto kb = Keyboards::lessons_list_kb(
                query->message->chat->id, 
                static_cast<bot_roles>(std::stoi(info.at(1))), 
                ymd
            );
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                kb->inlineKeyboard.empty() ? "No classes today" : 
                "Today's classes",
                kb,
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr); 
    };


    Message::Ptr day_info_handler::operator()(const CallbackQuery::Ptr& query)
    {
        if (StringTools::split(query->data, ' ').at(0) == 
            "user_lesson")
        {
            auto info = StringTools::split(query->data, ' ');
            int user_lesson_id = stoi(info.at(1));
            bot_roles role = static_cast<bot_roles>(std::stoi(info.at(2)));
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                get_user_lesson_info(
                    query->message->chat->id, 
                    user_lesson_id, 
                    role
                ),
                Keyboards::day_info_kb(user_lesson_id, role),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr); 
    }

}

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl)
{
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgWebhookTcpServer webhookServer(8080, bot);
        printf("Server starting\n");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }
}
