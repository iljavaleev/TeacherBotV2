#include "botstaff/FSM/UserRegistration.hpp"

using namespace TgBot;


void UserRegistration::get_first_name(const std::string& name)
{   
    filter_sender.send(
        msg::user_registration::check_first_name(
            name,
            messanger
        )
    );

    messanger.
    wait().
    handle<msg::user_registration::first_name_ok>(
        [&](const msg::user_registration::first_name_ok& msg)
    {
        state = &UserRegistration::get_last_name;
        user->first_name = name;
        try
        {
            bot.getApi().sendMessage(
            user->chat_id, 
            "Enter your surname"
        );
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
    }).
    handle<msg::user_registration::first_name_fail>(
        [&](const msg::user_registration::first_name_fail& msg)
    {
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "You entered incorrect information"
            ); 
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
    });
}

void UserRegistration::get_last_name(const std::string& name)
{   
    filter_sender.send(
        msg::user_registration::check_last_name(
            name,
            messanger
        )
    );

    messanger.
    wait().
    handle<msg::user_registration::last_name_ok>(
        [&](const msg::user_registration::last_name_ok& msg)
    {
        state = &UserRegistration::get_class;
        user->last_name = name;
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "Enter your class"
            );
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }).
    handle<msg::user_registration::last_name_fail>(
        [&](const msg::user_registration::last_name_fail& msg)
    {
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "You entered incorrect information"
            ); 
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
    });
}

void UserRegistration::get_class(const std::string& cls)
{
    filter_sender.send(
        msg::user_registration::check_class(
            cls,
            messanger
        )
    );

    messanger.
    wait().
    handle<msg::user_registration::class_ok>(
        [&](const msg::user_registration::class_ok& msg)
    {
        
        state = &UserRegistration::get_phone;
        user->cls = cls;
        
        try
        {
            bot.getApi().sendMessage(
            user->chat_id, 
            "Enter your phone number"
            );
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
    }).
    handle<msg::user_registration::class_fail>(
        [&](const msg::user_registration::class_fail& msg)
    {
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "You entered incorrect information"
            ); 
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }  
    });
};

void UserRegistration::get_phone(const std::string& phone)
{
    filter_sender.send(
        msg::user_registration::check_phone(
            phone,
            messanger
        )
    );

    messanger.
    wait().
    handle<msg::user_registration::phone_ok>(
        [&](const msg::user_registration::phone_ok& msg)
    {
        state = &UserRegistration::get_email;
        user->phone = phone;
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "Enter your email"
            );
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
        
    }).
    handle<msg::user_registration::phone_fail>(
        [&](const msg::user_registration::phone_fail& msg)
    {
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "You entered incorrect information"
            ); 
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
    });
}

void UserRegistration::get_email(const std::string& email)
{
    filter_sender.send(
        msg::user_registration::check_email(
            email,
            messanger
        )
    );

    messanger.
    wait().
    handle<msg::user_registration::email_ok>(
        [&](const msg::user_registration::email_ok& msg)
    {
        state = &UserRegistration::agreement;
        user->email = email;
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "You consent to the processing of personal data?",
                nullptr, 
                nullptr, 
                Keyboards::agreement_kb()
            );
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
        
    }).
    handle<msg::user_registration::email_fail>(
        [&](const msg::user_registration::email_fail& msg)
    {
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "You entered incorrect information"
            ); 
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }    
    });
}

void UserRegistration::agreement(const std::string& email)
{
    long chat_id = user->chat_id;
    try
    {
        if (user->role == bot_roles::teacher)
            user = user->create_teacher();
        else
            user = user->create_pupil();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    std::string message{};
    if (user)
    {
        message = "Registration completed successfully";
    }
    else
    {
        message = "Something is wrong, try again later";
    }
        
    try
    {
        bot.getApi().sendMessage(
            chat_id, 
            std::move(message)
        );
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    
}

void UserRegistration::done()
{
    get_sender().send(messaging::close_queue());
}

void UserRegistration::run(const std::string& message) 
{
    try
    {
        (this->*state)(message);
    }
    catch(const messaging::close_queue& )
    {}
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }       
}