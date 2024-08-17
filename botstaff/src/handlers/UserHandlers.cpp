#include "botstaff/handlers/UserHandlers.hpp"

#include <tgbot/tgbot.h>

#include "botstaff/utils/Utils.hpp"
#include "botstaff/handlers/Handlers.hpp"
#include "botstaff/keyboards/UserKeyboards.hpp"
#include "botstaff/FSM/Filter.hpp"
#include "botstaff/FSM/UserRegistration.hpp"

using namespace TgBot;
using namespace std;

std::unordered_map<long, std::shared_ptr<UserRegistration>> 
    user_registration_state;
std::unordered_map<long, std::shared_ptr<UpdateUser>> 
    user_update_state;
std::unordered_map<long, std::shared_ptr<ParentRegistration>> 
    parent_registration_state;
extern RegistrationFilter rf;


void clear_user_state(long user_id)
{
    if (user_registration_state.contains(user_id))
    {
        user_registration_state.erase(user_id);
    }
}

void clear_update_user_state(long user_id)
{
    if (user_update_state.contains(user_id))
    {
        user_update_state.erase(user_id);
    }
}

void clear_parent_registration_state(long user_id)
{
     if (parent_registration_state.contains(user_id))
    {
        parent_registration_state.erase(user_id);
    }
}

namespace user_handlers
{
    Message::Ptr user_start_handler::operator()(const CallbackQuery::Ptr& query)
    {
        if (query->data == "start_register") 
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
    
    Message::Ptr list_of_student_debts_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(query->data != "parent_debts")
            return Message::Ptr(nullptr);
        
        std::string debts = 
            ParentBotUser::get_debts_message(query->message->chat->id);

        std::thread send(
            send_message,
            std::ref(bot),
            query->message->chat->id,
            debts,
            "HTML" 
        );
        send.detach();
        
        return Message::Ptr(nullptr);
    }

    Message::Ptr list_of_student_rescheduling_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(query->data != "parent_reschedule")
            return Message::Ptr(nullptr);
        
        std::string mess = 
            ParentBotUser::get_rescedule_list(query->message->chat->id);

        std::thread send(
            send_message,
            std::ref(bot),
            query->message->chat->id,
            mess,
            "HTML" 
        );


        send.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr list_of_for_parent_comments_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(query->data != "parent_comments")
            return Message::Ptr(nullptr);
        
        std::thread send(
            send_message_with_kb,
            std::ref(bot),
            query->message->chat->id,
            "<b><u>Select comment by date</u></b>",
            UserKeyboards::parent_comments(query->message->chat->id),
            "HTML" 
        );

        send.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr content_of_for_parent_comment_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(!StringTools::startsWith(query->data, "for_parent_comment"))
            return Message::Ptr(nullptr);
        std::size_t pos = query->data.find(' ');
        std::string mess = query->data.substr(pos+1);

        std::thread send(
            send_message,
            std::ref(bot),
            query->message->chat->id,
            mess,
            "HTML" 
        );

        send.detach();
        return Message::Ptr(nullptr);
    }

    Message::Ptr lesson_delete_request::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::split(query->data, ' ').at(0) != "change_lesson_date")
            return Message::Ptr(nullptr);
    
        long lesson_id = 
            stol(StringTools::split(query->data, ' ').at(1));
        
        long teacher_id{};
        std::string mess = 
            lesson_delete_request_message(lesson_id, teacher_id);
        
        std::thread send(
            send_message,
            std::ref(bot),
            teacher_id,
            mess,
            "HTML" 
        );

        send.detach();

        return bot.getApi().sendMessage(
            query->message->chat->id, 
            "We have sent your request to the teacher"
        );
    }
}

namespace user_register_handlers
{
    Message::Ptr user_registration_handler::operator()(
        const Message::Ptr& message
    )
    {
        if(!user_registration_state.contains(message->chat->id))
            return Message::Ptr(nullptr);
        std::shared_ptr<UserRegistration> ur = 
            user_registration_state.at(message->chat->id);

        std::thread t{&UserRegistration::run, ur, message->text};
        t.detach();
        
        return Message::Ptr(nullptr);
    }

    Message::Ptr choose_teacher_for_register_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(user_registration_state.contains(query->message->chat->id) && 
            StringTools::startsWith(query->data, "register_pupil"))
        { 
            std::string teacher_id = 
                StringTools::split(query->data, ' ').at(1);
            
            std::shared_ptr<UserRegistration> ur = 
                user_registration_state.at(query->message->chat->id);

            std::thread t{&UserRegistration::run, ur, teacher_id};
            t.detach();
        }
        return Message::Ptr(nullptr);
    }


    Message::Ptr start_register_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (!StringTools::startsWith(query->data, "register_as")) 
            return Message::Ptr(nullptr);
        InlineKeyboardMarkup::Ptr kb{nullptr};
        std::string mess;

        bot_roles role = static_cast<bot_roles>(
            stoi(StringTools::split(query->data, ' ').at(1))
        );    
        
        if (role == bot_roles::parent)
        {
            std::shared_ptr<ParentRegistration> pr = 
                std::make_shared<ParentRegistration>(
                    rf.get_sender(),
                    std::ref(bot), 
                    query->message->chat->id, 
                    query->message->chat->username
                );
            parent_registration_state.emplace(query->message->chat->id, pr);
            mess = "Enter your child's email ";
        }
        else
        {
            std::shared_ptr<UserRegistration> ur = 
                std::make_shared<UserRegistration>(
                    rf.get_sender(),
                    std::ref(bot), 
                    query->message->chat->id, 
                    query->message->chat->username,
                    role
                );
            user_registration_state.emplace(query->message->chat->id, ur);

            if (role == bot_roles::pupil)
            {
                kb = UserKeyboards::create_list_teachers_kb();
                mess = "<b>Choose your teacher</b>"; 
            }
            else
                mess = "<b>Enter your name</b>";
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
        return Message::Ptr(nullptr);
    }

    
    Message::Ptr agree_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(!StringTools::startsWith(query->data, "agreement"))
            return Message::Ptr(nullptr);
       
        std::string answ = StringTools::split(query->data, ' ').at(1);   
        
        if (user_registration_state.contains(query->message->chat->id))
        {
            std::shared_ptr<UserRegistration> reg = 
                user_registration_state.at(query->message->chat->id);
            std::thread t{&UserRegistration::run, reg, answ};
            t.join();
            user_registration_state.erase(query->message->chat->id);
        }
        else
        {
            std::shared_ptr<ParentRegistration> reg = 
                parent_registration_state.at(query->message->chat->id);
            std::thread t{&ParentRegistration::run, reg, answ};
            t.join();
            parent_registration_state.erase(query->message->chat->id);
        } 
        return Message::Ptr(nullptr); 
    }
    

    Message::Ptr start_user_update_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(StringTools::split(query->data, ' ').at(0) == "update_user" && 
            !user_update_state.contains(query->message->chat->id)
        )
        {
            long user_id = std::stol(
                StringTools::split(query->data, ' ').at(1)
            );
            std::shared_ptr<BotUser> user = BotUser::get(user_id);
            std::shared_ptr<UpdateUser> uu = std::make_shared<UpdateUser>(
                rf.get_sender(),
                std::ref(bot), 
                user,
                query->message->chat->id
            );
            user_update_state.emplace(query->message->chat->id, uu);
            
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                std::format(
                    "<b><i>Choose field to update</i></b>\n{}", 
                    user->get_full_info()
                ),
                teacherKeyboards::update_user_info_kb(user->role),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr); 
    }
    
    Message::Ptr user_update_state_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::startsWith(query->data, "update_user_field") 
            && user_update_state.contains(query->message->chat->id)) 
        {
            std::string field = StringTools::split(query->data, ' ').at(1);
            long teacher_chat_id{query->message->chat->id};
            
            std::shared_ptr<UpdateUser> uu = 
                user_update_state.at(query->message->chat->id);

            InlineKeyboardMarkup::Ptr kb{nullptr};
            std::string mess = user_update_messages.at(field);
            if (field == "finish")
            {   
                try
                {   
                    uu->get_instance()->update();
                    clear_update_user_state(teacher_chat_id);
                    uu->done();
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            uu->change_state(field);
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

    Message::Ptr get_message_data_for_user_update_handler::operator()(
        const Message::Ptr& message
    )
    {
        if(!user_update_state.contains(message->chat->id))
        {
            return Message::Ptr(nullptr);
        }
           
        std::shared_ptr<UpdateUser> uu = 
            user_update_state.at(message->chat->id);
        std::thread t{&UpdateUser::run, uu, message->text};
        t.detach();
        return Message::Ptr(nullptr);
    }


    Message::Ptr parent_registration_handler::operator()(
        const Message::Ptr& message
    )
    {
        if(!parent_registration_state.contains(message->chat->id))
            return Message::Ptr(nullptr);
        std::shared_ptr<ParentRegistration> ur = 
            parent_registration_state.at(message->chat->id);

        std::thread t{&ParentRegistration::run, ur, message->text};
        t.detach();
        
        return Message::Ptr(nullptr);
    }

    
}