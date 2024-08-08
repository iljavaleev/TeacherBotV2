#ifndef Messages_hpp
#define Messages_hpp
#include <string>
#include "FSM.hpp"

struct check_message
{
    std::string content;
    mutable messaging::sender queue;
    check_message(
        const std::string& _msg,
        messaging::sender _queue
    ):
    content(_msg), queue(_queue){}
    virtual ~check_message() {}
};


namespace msg
{
    
    namespace user_registration
    {        
        
        struct check_first_name: public check_message
        {
            check_first_name(
                const std::string& _name,
                messaging::sender _queue
            ):
            check_message(_name, _queue){}
        };
        
        struct check_last_name: public check_message
        {
            check_last_name(
                const std::string& _name,
                messaging::sender _queue
            ):
            check_message(_name, _queue){}
        };

        struct check_class: public check_message
        {
            check_class(
                const std::string& _cls, 
                messaging::sender _queue
            ):
            check_message(_cls, _queue){}
        };

        struct check_phone: public check_message
        {
            check_phone(
                const std::string& _phone,
                messaging::sender _queue
            ):
            check_message(_phone, _queue){}
        };

        struct check_email: public check_message
        {
            check_email(
                const std::string& _email,
                messaging::sender _queue
            ):
            check_message(_email, _queue){}
        };
        
        struct check_agreement: public check_message
        {
            check_agreement(
                const std::string& agr,
                messaging::sender _queue
            ):
            check_message(agr, _queue){}
        };

        ///////////////////////////////////////////////////////////////////
        struct first_name_ok
        {};
        
        struct last_name_ok
        {
        };
        
        struct class_ok
        {
        };
        struct phone_ok
        {
        };
        struct email_ok
        {
        };
    
        struct first_name_fail
        {
        };
        struct last_name_fail
        {
        };
        struct class_fail
        {
        };
        struct phone_fail
        {
        };
        struct email_fail
        {
        };
    };
    
    namespace create_lesson
    {
        struct check_date: public check_message
        {
            check_date(
                const std::string& _date,
                messaging::sender _queue
            ):
            check_message(_date, _queue){}
        };

        struct date_ok
        {
        };

        struct date_fail
        {
        };

        struct check_time: public check_message
        {
            check_time(
                const std::string& _time,
                messaging::sender _queue
            ):
            check_message(_time, _queue){}
        };

        struct time_ok
        {
        };

        struct time_fail
        {
        };
    };
}


#endif
