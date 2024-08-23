#include <stdio.h>
#include <memory>
#include <functional>
#include <cstdlib>
#include <format>
#include <string>
#include <thread>
#include <sys/signal.h>                          
#include <vector>

#include "botstaff/handlers/Handlers.hpp"
#include "botstaff/handlers/TeacherHandlers.hpp"
#include "botstaff/handlers/UserHandlers.hpp"
#include "botstaff/FSM/Filter.hpp"
              
#include "tgbot/Api.h"                          
#include "tgbot/Bot.h"                           
#include "tgbot/EventBroadcaster.h"  
#include "tgbot/types/BotCommand.h" 

using namespace TgBot;
using namespace std;

RegistrationFilter rf;

std::vector<BotCommand::Ptr> create_commands()
{
    std::vector<BotCommand::Ptr> commands;

    BotCommand::Ptr start_command{new BotCommand};
    start_command->command = "start";
    start_command->description = "начать/выйти в главное меню";
    commands.push_back(start_command);

    BotCommand::Ptr cancel_command{new BotCommand};
    cancel_command->command = "cancel";
    cancel_command->description = "сброс/отмена";
    commands.push_back(cancel_command);

    return commands;
}

int main() {
    std::thread filter{&RegistrationFilter::run, std::ref(rf)};
    std::string bot_token = std::string(std::getenv("BOT_TOKEN"));
    std::string base_webhook_url = std::string(std::getenv("BASE_WEBHOOK_URL"));
    std::string TEL = std::format("https://api.telegram.org/bot{}/setWebhook?url=", bot_token);
    std::string WEBHOOK_URL = base_webhook_url + std::format("/{}", bot_token);    

    TgBot::Bot bot(std::getenv("BOT_TOKEN"));
    std::vector<BotCommand::Ptr> commands = create_commands();
   
    bot.getApi().setMyCommands(commands);
    bot.getEvents().onCommand("start", command_handlers::start_command(bot));
    bot.getEvents().onCommand("cancel", command_handlers::cancel_command(bot));
    bot.getEvents().onCallbackQuery(user_handlers::user_start_handler(bot));
    bot.getEvents().onCallbackQuery(user_register_handlers::start_register_handler(bot));
    bot.getEvents().onNonCommandMessage(user_register_handlers::user_registration_handler());
    bot.getEvents().onNonCommandMessage(user_register_handlers::parent_registration_handler());
    bot.getEvents().onCallbackQuery(user_register_handlers::agree_handler());
    bot.getEvents().onCallbackQuery(user_register_handlers::user_update_state_handler(bot));
    bot.getEvents().onCallbackQuery(user_register_handlers::start_user_update_handler(bot));
    bot.getEvents().onCallbackQuery(
        user_register_handlers::choose_teacher_for_register_handler()
    );

    bot.getEvents().onCallbackQuery(lesson::start_lesson_creation_handler(bot));
    bot.getEvents().onNonCommandMessage(lesson::lesson_creation_handler(bot));
    bot.getEvents().onNonCommandMessage(
        lesson::get_message_data_for_lesson_update_handler(bot)
    );
    bot.getEvents().onNonCommandMessage(user_register_handlers::get_message_data_for_user_update_handler());

    bot.getEvents().onCallbackQuery(teacher_handlers::list_active_not_handler(bot));
    bot.getEvents().onCallbackQuery(teacher_handlers::list_all_teachers_handler(bot));
    bot.getEvents().onCallbackQuery(teacher_handlers::list_comments_handler(bot));
    bot.getEvents().onCallbackQuery(teacher_handlers::list_teachers_handler(bot));
    bot.getEvents().onCallbackQuery(teacher_handlers::list_user_handler(bot));

    bot.getEvents().onCallbackQuery(teacher_handlers::activate_user(bot));
    bot.getEvents().onCallbackQuery(teacher_handlers::delete_user(bot));
     
    bot.getEvents().onCallbackQuery(teacher_handlers::user_info_handler(bot));
    bot.getEvents().onCallbackQuery(teacher_handlers::comment_text_handler(bot));
    
    bot.getEvents().onCallbackQuery(handlers::calendar_handler(bot));
    bot.getEvents().onCallbackQuery(handlers::next_month_handler(bot));
    bot.getEvents().onCallbackQuery(handlers::calendar_day_handler(bot));
    bot.getEvents().onCallbackQuery(handlers::day_info_handler(bot));
    bot.getEvents().onCallbackQuery(
        lesson::choose_pupil_for_lesson_creation_handler(bot)
    );
    bot.getEvents().onCallbackQuery(lesson::start_lesson_update_handler(bot));
    bot.getEvents().onCallbackQuery(lesson::lesson_update_state_handler(bot));
    bot.getEvents().onCallbackQuery(
        lesson::get_query_data_for_lesson_update_handler(bot)
    );
    bot.getEvents().onCallbackQuery(
        lesson::delete_lesson_handler(bot)
    );
    bot.getEvents().onCallbackQuery(teacher_handlers::list_debts_handler(bot));
    bot.getEvents().onCallbackQuery(
        teacher_handlers::change_payment_status_request_handler(bot)
    );
    bot.getEvents().onCallbackQuery(
        teacher_handlers::change_payment_status_handler(bot)
    );
    bot.getEvents().onCallbackQuery(
        user_handlers::list_of_student_debts_handler(bot)
    );

    bot.getEvents().onCallbackQuery(
        user_handlers::list_of_student_rescheduling_handler(bot)
    );

     bot.getEvents().onCallbackQuery(
        user_handlers::list_of_for_parent_comments_handler(bot)
    );
     bot.getEvents().onCallbackQuery(
        user_handlers::content_of_for_parent_comment_handler(bot)
    );
    bot.getEvents().onCallbackQuery(
        user_handlers::lesson_delete_request(bot)
    );
    bot.getEvents().onCallbackQuery(
        lesson::delete_lesson_reason_handler(bot)
    );
    bot.getEvents().onCallbackQuery(
        lesson::one_time_delete_lesson_handler(bot)
    );


    

    
   
           
    signal(SIGINT, [](int s) 
    { 
        printf("SIGINT got\n"); 
        rf.done();
        exit(0);
    });

    startWebhook(bot, WEBHOOK_URL);
    
    filter.join();
    return 0;
}
