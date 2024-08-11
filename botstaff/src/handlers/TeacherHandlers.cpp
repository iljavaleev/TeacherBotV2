#include "botstaff/handlers/TeacherHandlers.hpp"

#include <functional>
#include <format>
#include <tgbot/tgbot.h>
#include <memory>

#include "botstaff/utils/Utils.hpp"
#include "botstaff/database/DB.hpp"
#include "botstaff/keyboards/TeacherKeyboards.hpp"
#include "botstaff/FSM/LessonCreation.hpp"
#include "botstaff/FSM/Filter.hpp"

using namespace TgBot;
using namespace std;

std::unordered_map<long, std::shared_ptr<CreateLesson>> 
    create_lesson_state;
std::unordered_map<long, std::shared_ptr<UpdateLesson>> 
    update_lesson_state;
extern RegistrationFilter rf;


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

namespace teacher_handlers
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

    Message::Ptr list_debts_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (query->data == "debts")
        {
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                "List of student debts",
                teacherKeyboards::create_debts_kb(
                        query->message->chat->id
                    ),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }


    Message::Ptr change_payment_status_request_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::startsWith(query->data, "debt_info"))
        {
            int lesson_id(std::stoi(
                StringTools::split(query->data, ' ').at(1))
            );
            printf("HERE\n");
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                "Change payment status?",
                teacherKeyboards::change_payment_status(lesson_id),
                "HTML" 
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }

     Message::Ptr change_payment_status_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if (StringTools::startsWith(query->data, "change_debt"))
        {
            auto data = StringTools::split(query->data, ' ');
            
            std::string answ = data.at(1);
            int lesson_id(std::stoi(data.at(2)));
            std::string mess;
            if(answ == "yes")
            {
                mess = "Satus was changed successfully";
            }
            else
            {
                mess = "Ok";
            };
            try
            {
                change_debt_status(lesson_id);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                mess = "Somthing went wrong, try later";
            }

            std::thread send(
                send_message,
                std::ref(bot),
                query->message->chat->id,
                mess,
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
}

namespace lesson
{
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

        std::string data = StringTools::split(query->data, ' ').at(1);
        std::thread t{&CreateLesson::run, cl, data};
        t.detach();
        return Message::Ptr(nullptr);
    }
    
    Message::Ptr start_lesson_update_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(StringTools::split(query->data, ' ').at(0) == "update_lesson" && 
            !update_lesson_state.contains(query->message->chat->id)
        )
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
            
            std::thread send(
                send_message_with_kb,
                std::ref(bot),
                query->message->chat->id,
                std::format(
                    "<b><i>Choose field to update</i></b>\n{}", 
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
        if (StringTools::startsWith(query->data, "update_lesson_field") 
            && update_lesson_state.contains(query->message->chat->id)) 
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
                    clear_lesson_update_state(teacher_chat_id);
                    cl->done();
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            else if(field == "date")
            {
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
                kb = teacherKeyboards::create_pupil_for_lesson_kb(
                    teacher_chat_id, 
                    true
                );
            }
            cl->change_state(field);
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
        {
            return Message::Ptr(nullptr);
        }
            
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
    
    Message::Ptr delete_lesson_handler::operator()(
        const CallbackQuery::Ptr& query
    )
    {
        if(StringTools::split(query->data, ' ').at(0) == "delete_lesson")
        {   
            
            int lesson_id = stoi(
                StringTools::split(query->data, ' ').at(1)
            );
            
            try
            {
                UserLesson::destroy(lesson_id);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            std::thread send(
                send_message,
                std::ref(bot),
                query->message->chat->id, 
                "You canceled your lesson",
                "HTML"
            );
            send.detach();
        }
        return Message::Ptr(nullptr);
    }
}


