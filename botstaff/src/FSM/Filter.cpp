#include "botstaff/FSM/Filter.hpp"
#include <regex>

using namespace TgBot;


bool RegistrationFilter::name_is_ok(std::string name)
{ 
    std::erase(name, ' ');
    
    if (name.empty())
        return false; 
    
    if(!std::regex_match(name, std::regex("^[A-Za-z\\s]*$")))
        return false;
    
    return true;
}

int RegistrationFilter::count_numbers(const std::string& str)
{
    const std::unordered_set<char> nums = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    int c{};
    for (auto i{str.begin()}; i != str.end(); ++i)
    {
        if (nums.contains(*i)) ++c;
    }
    printf("%d\n", c);
    return c;
}

bool RegistrationFilter::phone_is_valid(const std::string& phone)
{
    if (!std::regex_match(phone, std::regex("^[0-9\\s()-]*$")))
    {
        return false;
    }
    return count_numbers(phone) == 11; // for russian tnumbers
} 

void RegistrationFilter::done()
{ 
    get_sender().send(messaging::close_queue()); 
}

void RegistrationFilter::run()
{   
    try
    {
        while(1)
        {
            messanger
            .wait()
            .handle<msg::user_registration::check_first_name>(
                [&](const msg::user_registration::check_first_name& msg)
                {
                    if (name_is_ok(msg.content))
                    {
                        msg.queue.send(
                            msg::user_registration::first_name_ok()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::user_registration::first_name_fail()
                        );
                    }
                }
            ).
            handle<msg::user_registration::check_last_name>(
                [&](const msg::user_registration::check_last_name& msg)
                {
                    if (name_is_ok(msg.content))
                    {
                        msg.queue.send(
                            msg::user_registration::last_name_ok()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::user_registration::last_name_fail()
                        );
                    }
                }
            ).
            handle<msg::user_registration::check_class>(
                [&](const msg::user_registration::check_class& msg)
                {
                    if(std::regex_match(
                        msg.content, std::regex("^[A-Za-z0-9\\s]*$"))
                    )
                    {
                        msg.queue.send(
                            msg::user_registration::class_ok()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::user_registration::class_fail()
                        );
                    }

                }
            ).
            handle<msg::user_registration::check_phone>(
                [&](const msg::user_registration::check_phone& msg)
                {
                    if(phone_is_valid(msg.content))
                    {
                        printf("phone is valid\n");
                        msg.queue.send(
                            msg::user_registration::phone_ok()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::user_registration::phone_fail()
                        );
                    }

                }
            ).
            handle<msg::user_registration::check_email>(
                [&](const msg::user_registration::check_email& msg)
                {
                    const std::regex pattern{
                        "(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"
                    };
                    
                    if(std::regex_match(msg.content, pattern))
                    {
                        msg.queue.send(
                            msg::user_registration::email_ok()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::user_registration::email_fail()
                        );
                    }

                }
            ).
            handle<msg::create_lesson::check_date>(
                [&](const msg::create_lesson::check_date& msg)
                {
                    std::chrono::year_month_day date = 
                        string_to_chrono_date(msg.content);
                    
                    const std::chrono::time_point now_t{
                        std::chrono::system_clock::now()
                    };
                    
                    const std::chrono::year_month_day now{
                        std::chrono::floor<std::chrono::days>(now_t)
                    };
                    
                    if(date >= now)
                    {
                        msg.queue.send(
                            msg::create_lesson::date_ok()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::create_lesson::date_fail()
                        );
                    }

                }
            ).
            handle<msg::create_lesson::check_time>(
                [&](const msg::create_lesson::check_time& msg)
                {
                    auto pattern = std::regex("^[0-9\\s.:/-]*$");
                    if(!std::regex_match(msg.content, pattern) || 
                        count_numbers(msg.content) > 4)
                    {
                        msg.queue.send(
                            msg::create_lesson::time_fail()
                        );
                    }
                    else
                    {
                        msg.queue.send(
                            msg::create_lesson::time_ok()
                        );
                    }

                }
            );
        }
    }
    catch(const messaging::close_queue&){}
}
