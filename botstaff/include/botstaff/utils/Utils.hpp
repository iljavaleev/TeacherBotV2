#ifndef Utils_hpp
#define Utils_hpp

#include <chrono>                            
#include <future>                              
#include <memory>                              
#include <string>                              
#include <thread>                              
#include <type_traits>                         
#include <unordered_map>

#include "botstaff/Vocabular.hpp"

#include "tgbot/types/InlineKeyboardMarkup.h"  
#include "tgbot/types/Message.h"               

enum class bot_roles;
struct BotUser;

namespace TgBot { class Bot; }
using namespace TgBot;

inline std::shared_ptr<std::unordered_map<long, bot_roles>> roles(
    new std::unordered_map<long, bot_roles>()
);

template<typename F, typename A>
std::future<std::invoke_result_t<F, A&&>> spwn_task(F&& f, A&& a)
{
    using result_type = std::invoke_result_t<F, A&&>;
    std::packaged_task<result_type(A&&)> task(std::move(f));
    std::future<result_type> res(task.get_future());
    std::thread t(std::move(task), std::move(a));
    t.detach();
    return res;
}

std::chrono::year_month_day string_to_chrono_date(
    const std::string &s, 
    char delim='-'
);
std::string chrono_to_string_date(const std::chrono::year_month_day& ymd);



void send_message_with_kb(
    const TgBot::Bot& bot, 
    long chat_id, 
    const std::string& mess, 
    TgBot::InlineKeyboardMarkup::Ptr kb, 
    const std::string& pm="HTML"
);

void send_message(
    const TgBot::Bot& bot, 
    long chat_id, 
    const std::string& mess,
    const std::string& pm="HTML"
);

std::string get_pupil_info(const std::shared_ptr<BotUser>&);
std::string get_pupil_info(const TgBot::Message::Ptr& message);
std::string get_teacher_info(const std::shared_ptr<BotUser>&);
std::string get_comment_text(int);
void send_current_calendar(
    const TgBot::Bot& bot,
    long chat_id, 
    const bot_roles& role);
void send_error_message(
    const TgBot::Bot& bot, 
    long chat_id, 
    std::string mess=utils_voc::_error);


#endif