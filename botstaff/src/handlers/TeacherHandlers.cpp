#include "botstaff/handlers/TeacherHandlers.hpp"

#include <functional>
#include <format>
#include <tgbot/tgbot.h>
#include <memory>

#include "botstaff/utils/Utils.hpp"
#include "botstaff/database/DB.hpp"
#include "botstaff/keyboards/TeacherKeyboards.hpp"

using namespace TgBot;
using namespace std;

namespace teacherHandlers
{   
    Message::Ptr list_comments_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
            
        if (query->data == "comments")
        {
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                "Your latest comments",
                teacherKeyboards::create_comments_kb(
                        query->message->chat->id
                    ),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }


    Message::Ptr comment_text_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::split(query->data, ' ').at(0) == 
            "comment_for_teacher")
        {
            int lesson_id(std::stoi(
                StringTools::split(query->data, ' ').at(1))
            );
            std::thread send(
                send_message,
                std::ref(bot),
                query->message->chat->id,
                get_comment_text(lesson_id),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }

    Message::Ptr list_all_teachers_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (query->data == "teachers")
        {
             std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                "Choose category",
                teacherKeyboards::create_teachers_kb(),
                "HTML" 
            );
            send.detach();

        }
        return Message::Ptr(nullptr);
    }

    Message::Ptr list_teachers_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::startsWith(query->data, "teachers_list"))
        {
            std::string mess;
            InlineKeyboardMarkup::Ptr kb;
            if (query->data == "teachers_list_active")
            {
                mess = "List of active teachers";
                kb = teacherKeyboards::create_teachers_list_kb(true);
            }
            else
            {
                mess = "List of candidats";
                kb = teacherKeyboards::create_teachers_list_kb(false);
            }
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                std::move(mess),
                kb,
                "HTML" 
            );
            send.detach();
                
        }
        return Message::Ptr(nullptr);
    }

    Message::Ptr list_active_not_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (query->data == "pupils")
        {
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                "Choose category",
                teacherKeyboards::create_pupils_kb(query->message->chat->id),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }
    
    Message::Ptr list_user_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::startsWith(query->data, "pupils_list"))
            {
                long teacher_id(query->message->chat->id);
                std::string mess;
                InlineKeyboardMarkup::Ptr kb;
                if (StringTools::startsWith(query->data, "pupils_list_active"))
                {
                    kb = teacherKeyboards::create_pupils_list_kb(
                        teacher_id, true
                    );
                    mess = kb->inlineKeyboard.empty() ? "It's empty for now" : 
                        "List of students";
                }
                else
                {   
                    kb = teacherKeyboards::create_pupils_list_kb(
                        teacher_id, false
                    );
                    mess = kb->inlineKeyboard.empty() ? "It's empty for now" : 
                        "List of candidats";
                }
                    
                std::thread send(
                    send_message_with_kb,
                    std::ref(bot),
                    query->message->chat->id,
                    mess,
                    kb,
                    "HTML" 
                );
                send.detach();
            }
        return Message::Ptr(nullptr);        
    }

    Message::Ptr user_info_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(StringTools::split(query->data, ' ').at(0) == "user_info")
        {   
            std::string pupil_id{
                StringTools::split(query->data, ' ').at(1)
            };
            std::shared_ptr<BotUser> u = BotUser::get(std::stol(pupil_id));
            std::string info;
            if (u->role == bot_roles::teacher)
            {
                info = get_teacher_info(u);
            }
            else
            {
                info = get_pupil_info(u);
            }
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                info,
                teacherKeyboards::create_user_info_kb(u),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }

    Message::Ptr activate_user::operator()(const CallbackQuery::Ptr& query)
    {
        if(StringTools::split(query->data, ' ').at(0) == "activate_user")
            {   
                int pupil_id{
                    std::stoi(StringTools::split(query->data, ' ').at(1))
                };
                BotUser::activate(pupil_id);
                std::thread send(
                    send_message,
                    std::ref(bot),
                    pupil_id,
                    "You are registered",
                    "HTML"  
                );
                send.detach();
                 
                return bot.getApi().sendMessage(
                    query->message->chat->id, 
                    "User was added"
                );

            }
        return Message::Ptr(nullptr);
    }

    Message::Ptr delete_user::operator()(const CallbackQuery::Ptr& query)
    {
        if(StringTools::split(query->data,' ').at(0) == "delete_user")
        {   
            std::string pupil_id{
                StringTools::split(query->data, ' ').at(1)
            };
            BotUser::destroy(stol(pupil_id));
            std::thread send(
                send_message,
                std::ref(bot),
                query->message->chat->id, 
                "User was removed",
                "HTML"  
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }

    Message::Ptr update_user::operator()(const CallbackQuery::Ptr& query)
    {
        if(StringTools::split(query->data, ' ').at(0) == "update_user")
        {   
            auto info = StringTools::split(query->data, ' ');
            long user_id{std::stol(info.at(2))};
            bot_roles role = static_cast<bot_roles>(std::stoi(info.at(1)));
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id, 
                "Choose what you want to update",
                teacherKeyboards::update_user_info_kb(role, user_id),
                "HTML"  
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }
}


