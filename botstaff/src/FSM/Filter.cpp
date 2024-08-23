#include "botstaff/FSM/Filter.hpp"

#include <stdio.h>                        
#include <chrono>                         
#include <compare>                        
#include <regex>                         
#include <unordered_set>                  
#include <vector>             
           
#include "botstaff/FSM/Messages.hpp"      
#include "botstaff/Vocabular.hpp"         
#include "botstaff/database/Quiries.hpp"  
#include "botstaff/utils/Utils.hpp"       

using namespace TgBot;


bool RegistrationFilter::name_is_ok(std::string name)
{ 
    std::erase(name, ' ');
    
    if (name.empty())
        return false; 
    
    if(!std::regex_match(name, std::regex(FSM_voc::filter_voc::_name_is_ok)))
    {
       
        return false;
    }
        
    return true;
}

int RegistrationFilter::count_numbers(std::string& str, bool num_only)
{
    const std::unordered_set<char> nums = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    int count{};
    for (auto i{str.begin()}; i != str.end(); ++i)
    {
        if (nums.contains(*i)) 
            count++;
        else if (num_only)
            str.erase(i);

    }
    return count;
}

bool RegistrationFilter::phone_is_valid(std::string& phone)
{
    if (!std::regex_match(phone, std::regex("^[0-9\\s()-]*$")))
    {   
        return false;
    }
    return count_numbers(phone, true) == 11; // for russian tnumbers
} 

std::shared_ptr<BotUser> RegistrationFilter::user_exist(
    const std::string& phone, 
    const std::string& email
)
{
    auto users = BotUser::get_all(
        create_query(other_quiries::_student_exist, phone, email));
    if (!users.empty())
    {   
        return users.at(0);
    }
        
    return nullptr;
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
                        msg.content, std::regex(
                            FSM_voc::filter_voc::_check_class)
                        )
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
                    std::string phone{msg.content};
                    if(phone_is_valid(phone))
                    {
                        msg.queue.send(
                            msg::user_registration::phone_ok(phone)
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
                    std::string time{msg.content};
                    if(!std::regex_match(time, pattern) || 
                        count_numbers(time) > 4)
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
            ).
            handle<msg::user_registration::check_pupil_exists>(
                [&](const msg::user_registration::check_pupil_exists& msg)
                {
                    std::shared_ptr<BotUser> pu = 
                        user_exist(msg.phone, msg.email);
                    if(pu)
                    {
                        msg.queue.send(
                            msg::user_registration::pupil_exists_ok(pu->chat_id)
                        );
                    }
                    else
                    {
                        msg.queue.send(
                             msg::user_registration::pupil_exists_fail()
                        );
                    }

                }
            );
        }
    }
    catch(const messaging::close_queue&){}
}
