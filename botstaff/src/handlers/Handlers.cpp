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


using namespace TgBot;

std::unordered_map<long, std::shared_ptr<UserRegistration>> 
    user_registration_state;
std::unordered_map<long, std::shared_ptr<CreateLesson>> 
    create_lesson_state;
std::unordered_map<long, std::shared_ptr<UpdateLesson>> 
    update_lesson_state;
extern RegistrationFilter rf;

void clear_user_state(long user_id)
{
    if (user_registration_state.contains(user_id))
    {
        user_registration_state.erase(user_id);
    }
}

void clear_lesson_state(long user_id)
{
    if (create_lesson_state.contains(user_id))
    {
        create_lesson_state.erase(user_id);
    }
}

void clear_lesson_update_state(long user_id)
{
    if (update_lesson_state.contains(user_id))
    {
        update_lesson_state.erase(user_id);
    }
}

namespace CommandHandlers
{
    Message::Ptr start_command::operator()(const Message::Ptr& message)
    {
        long chat_id(message->chat->id);
        clear_user_state(chat_id);
        clear_lesson_state(chat_id);
        bot_roles role = get_role(chat_id);
        
        std::string mes;
        InlineKeyboardMarkup::Ptr kb;
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
            mes = "Pupil menu";
            kb = UserKeyboards::create_user_start_kb(chat_id);
        }
        else
        {
            mes = "Unregistred users menu";
            kb = UserKeyboards::create_user_start_kb(chat_id, false);
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
        clear_lesson_state(chat_id);
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


namespace Handlers
{
    Message::Ptr user_registration_handler::operator()(
        const Message::Ptr& message
    )
    {
        if(!user_registration_state.contains(message->chat->id))
            return Message::Ptr(nullptr);
        std::shared_ptr<UserRegistration> ur = 
            user_registration_state.at(message->chat->id);

        std::thread t{&UserRegistration::run, ur, std::ref(message->text)};
        t.detach();
        
        return Message::Ptr(nullptr);
    }


    Message::Ptr start_register_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(query->data != "register")
            return Message::Ptr(nullptr);
        std::shared_ptr<UserRegistration> ur = 
        std::make_shared<UserRegistration>(
            rf.get_sender(),
            std::ref(bot), 
            query->message->chat->id, 
            query->message->chat->username
        );
        user_registration_state.emplace(query->message->chat->id, ur);        
        std::thread send(
                send_message,
                std::ref(bot),
                query->message->chat->id,
                "Enter your name",
                "HTML"
            );
        send.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr lesson_creation_handler::operator()(
        const Message::Ptr& message
    )
    {
       if(!create_lesson_state.contains(message->chat->id))
            return Message::Ptr(nullptr);
        std::shared_ptr<CreateLesson> ul = 
            create_lesson_state.at(message->chat->id);

        std::thread t{&CreateLesson::run, ul, std::ref(message->text)};
        t.detach();
        
        return Message::Ptr(nullptr);
    }

    Message::Ptr start_lesson_creation_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(!StringTools::startsWith(query->data, "create_lesson"))
            return Message::Ptr(nullptr);
        auto data = StringTools::split(query->data, ' ');
        
        std::shared_ptr<CreateLesson> cl = 
        std::make_shared<CreateLesson>(
            rf.get_sender(), 
            std::ref(bot), 
            query->message->chat->id
        );
        create_lesson_state.emplace(query->message->chat->id, cl);        
        std::thread t{&CreateLesson::run, cl, data.at(1)};
        t.detach();
        return Message::Ptr(nullptr);
    }


     Message::Ptr choose_pupil_for_lesson_creation_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(!StringTools::startsWith(query->data, "choose_pupil_for_lesson") || 
            !create_lesson_state.contains(query->message->chat->id))
            return Message::Ptr(nullptr);
        
        
        std::shared_ptr<CreateLesson> cl = 
            create_lesson_state.at(query->message->chat->id);

        auto data = StringTools::split(query->data, ' ');
        std::thread t{&CreateLesson::run, cl, data.at(1)};
        t.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr agree_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        auto res = Message::Ptr(nullptr);
        
        if(!StringTools::startsWith(query->data, "agreement"))
            return res;

        if(query->data == "agreement_yes")
        {
            std::shared_ptr<UserRegistration> ur = 
            user_registration_state.at(query->message->chat->id);
   
            std::thread t{&UserRegistration::run, ur, std::ref(query->data)};
            t.detach();
       }
       else
       {
            std::thread send(
                send_message,
                std::ref(bot),
                query->message->chat->id,
                "Registration is not completed", 
                "HTML"
            );
            send.detach();
       }     
        user_registration_state.erase(query->message->chat->id);
        return Message::Ptr(nullptr); 
    }

    Message::Ptr calendar_handler::operator()(const CallbackQuery::Ptr& query)
    {
       if (StringTools::split(query->data, ' ').at(0) == "calendar") 
        {
            auto ymd = get_curent_ymd();
            ymd.day = 1;
            bot_roles role = 
                StringTools::split(query->data, ' ').at(1) == 
                    "teacher" ? bot_roles::teacher : bot_roles::pupil;
            
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                std::format(
                    "<b><i>Schedule for {}</i></b>", 
                    MONTHS_EN.at(ymd.month-1)
                ),
                Keyboards::calendar_kb()(
                    ymd,
                    role, 
                    query->message->chat->id
                ),
                "HTML" 
            );
            send.detach();
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
                    "teacher" ? bot_roles::teacher : bot_roles::pupil;
            
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
                    MONTHS_EN.at(ymd.month-1)
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

    
    Message::Ptr start_lesson_update_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(StringTools::split(query->data, ' ').at(0) == "update_lesson")
        {
            
            long lesson_id = std::stol(
                StringTools::split(query->data, ' ').at(1)
            );
            std::shared_ptr<LessonInfo> lesson = 
                std::make_shared<LessonInfo>(UserLesson::get(lesson_id));
            std::shared_ptr<UpdateLesson> ul = std::make_shared<UpdateLesson>(
                rf.get_sender(),
                std::ref(bot), 
                lesson
            );
            update_lesson_state.emplace(query->message->chat->id, ul);
            printf("Create\n");
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                std::format(
                    "Choose field to update\n{}", 
                    lesson->get_full_info()
                ),
                teacherKeyboards::update_lesson_info_kb(lesson_id),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr); 
    }
    
    Message::Ptr lesson_update_state_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::startsWith(query->data, "update_lesson_field")) 
        {
            std::string field = StringTools::split(query->data, ' ').at(1);
            long teacher_chat_id{query->message->chat->id};
            
            std::shared_ptr<UpdateLesson> cl = 
                update_lesson_state.at(query->message->chat->id);

            InlineKeyboardMarkup::Ptr kb{nullptr};
            std::string mess = lesson_creation_messages.at(field);
            if (field == "finish_lesson_update")
            {   
                try
                {
                    cl->get_instance()->update();
                    cl->done();
                    clear_lesson_update_state(teacher_chat_id);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            else if(field == "date")
            {
                cl->change_state(field);
                YMD ymd = get_curent_ymd();
                kb = Keyboards::calendar_kb()(
                    ymd, 
                    bot_roles::teacher, 
                    teacher_chat_id,
                    true
                );
            }
            else if(field == "pupil")
            {
                cl->change_state(field);
                kb = teacherKeyboards::create_pupil_for_lesson_kb(
                    teacher_chat_id, 
                    true
                );
            }
            
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                teacher_chat_id,
                mess,
                kb,
                "HTML" 
            );


            send.detach();
        }
        return  Message::Ptr(nullptr);
    }

    Message::Ptr get_message_data_for_lesson_update_handler::operator()(
        const Message::Ptr& message
    )
    {
        if(!update_lesson_state.contains(message->chat->id))
            return Message::Ptr(nullptr);
        std::shared_ptr<UpdateLesson> ul = 
            update_lesson_state.at(message->chat->id);

        std::thread t{&UpdateLesson::run, ul, message->text};
        t.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr get_query_data_for_lesson_update_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(!update_lesson_state.contains(query->message->chat->id) || 
            StringTools::endsWith(query->data, "for_lesson"))
            return Message::Ptr(nullptr);
        
        std::shared_ptr<UpdateLesson> ul = 
            update_lesson_state.at(query->message->chat->id);
        auto data = StringTools::split(query->data, ' ');
        
        if (data.at(0) == "update_pupil_for_lesson")
        {
            std::thread t{&UpdateLesson::run, ul, data.at(1)};
            t.detach();
        }
        else if (data.at(0) == "update_date_for_lesson")
        {
            std::thread t{&UpdateLesson::run, ul, data.at(2)};
            t.detach();
            
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
