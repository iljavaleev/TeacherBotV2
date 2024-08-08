#ifndef Utils_hpp
#define Utils_hpp

#include <future>
#include <type_traits>
#include <thread>
#include <chrono>
#include <format>
#include <unordered_map>
#include <tgbot/tgbot.h>
#include <string>

#include "botstaff/database/DB.hpp"

enum class bot_roles{ pupil, teacher, parent, admin, anon };
struct BotUser;

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

bot_roles get_role(long chat_id);
bool is_admin(long chat_id);
bool is_teacher(long chat_id);

void send_message_with_kb(
    const TgBot::Bot& bot, 
    long chat_id, 
    const std::string& mess, 
    TgBot::InlineKeyboardMarkup::Ptr kb, 
    const std::string& pm
);

void send_message(
    const TgBot::Bot& bot, 
    long chat_id, 
    const std::string& mess,
    const std::string& pm
);

std::string get_pupil_info(const std::shared_ptr<BotUser>&);
std::string get_pupil_info(const TgBot::Message::Ptr& message);
std::string get_teacher_info(const std::shared_ptr<BotUser>&);
std::string get_comment_text(int);

#endif