#include "botstaff/FSM/UserRegistration.hpp"

using namespace TgBot;

void UserRegistration::choose_teacher(const std::string& chat_id)
{
    user->teacher = std::stol(chat_id);
    state = &UserRegistration::get_class;
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
        
        state = &UserRegistration::get_first_name;
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
        state = &UserRegistration::get_phone;
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

void UserRegistration::agreement(const std::string& choice)
{
    std::string mess{};
    long chat_id = user->chat_id;
    if (choice == "no")
        mess = "<b>Registration is not completed<\b>";
    else
    {   
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
        
        if (user)
        {
            mess = "Registration completed successfully";
        }
        else
        {
            mess = "Something is wrong, try again later";
        }
    }

    try
    {
        bot.getApi().sendMessage(
            chat_id, 
            std::move(mess)
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


void UpdateUser::send_update_kb()
{
    try
    {
        bot.getApi().sendMessage(
            teacher_id, 
            std::format(
                "Choose field to update\n{}", 
                user->get_full_info(user->role)
            ),
            nullptr,
            nullptr,
            teacherKeyboards::update_user_info_kb(user->role),
            "HTML"
        );
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }    
}

void UpdateUser::send_error_msg(const std::string msg)
{
    try
    {
        bot.getApi().sendMessage(
            teacher_id, 
            msg
        ); 
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }    
}

void UpdateUser::change_first_name(const std::string& name)
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
        user->first_name = name;
        send_update_kb();
    }).
    handle<msg::user_registration::first_name_fail>(
        [&](const msg::user_registration::first_name_fail& msg)
    {
       send_error_msg();
    });
}

void UpdateUser::change_last_name(const std::string& name)
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
        user->last_name = name;
        send_update_kb();
    }).
    handle<msg::user_registration::last_name_fail>(
        [&](const msg::user_registration::last_name_fail& msg)
    {
       send_error_msg();
    });
}

void UpdateUser::change_class(const std::string& cls)
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
        
        user->cls = cls;
        send_update_kb();
    }).
    handle<msg::user_registration::class_fail>(
        [&](const msg::user_registration::class_fail& msg)
    {
       send_error_msg();
    });
};

void UpdateUser::change_phone(const std::string& phone)
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
        user->phone = phone;
        send_update_kb();
    }).
    handle<msg::user_registration::phone_fail>(
        [&](const msg::user_registration::phone_fail& msg)
    {
        send_error_msg();
    });
}

void UpdateUser::change_email(const std::string& email)
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
        user->email = email;
        send_update_kb();
        
    }).
    handle<msg::user_registration::email_fail>(
        [&](const msg::user_registration::email_fail& msg)
    {
        send_error_msg();
    });
}

void UpdateUser::change_comment(const std::string& comment)
{
    user->comment = comment;
    send_update_kb();  
}

void UpdateUser::change_active_status(const std::string& status)
{
    if(status == "y")
        user->is_active = true;
    else if(status == "n")
        user->is_active = false;
    else
    {
        send_error_msg();
        return;
    }
    send_update_kb();
}

void UpdateUser::done()
{
    get_sender().send(messaging::close_queue());
}

void UpdateUser::run(const std::string& message) 
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

void UpdateUser::change_state(const std::string& message)
{
    if(states.contains(message))
        state = states.at(message);
}

typedef void (UpdateUser::*callable)(const std::string&);
std::unordered_map<std::string, callable> UpdateUser::states = { 
    {"first_name", &UpdateUser::change_first_name},
    { "last_name", &UpdateUser::change_last_name},
    { "class", &UpdateUser::change_class },
    { "phone",  &UpdateUser::change_phone },
    { "email", &UpdateUser::change_email },
    {"comments",  &UpdateUser::change_comment },
    {"status", &UpdateUser::change_active_status }
};



void ParentRegistration::get_child_email(const std::string& email)
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
        state = &ParentRegistration::get_child_phone;
        child_email = email;
        try
        {
            bot.getApi().sendMessage(
                user->chat_id, 
                "Enter your child phone number"
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
void ParentRegistration::get_child_phone(const std::string& phone)
{

}
void ParentRegistration::get_first_name(const std::string& name)
{

}
void ParentRegistration::get_last_name(const std::string& name)
{

}
void ParentRegistration::get_phone(const std::string& phone)
{

}
void ParentRegistration::agreement(const std::string& email)
{

}