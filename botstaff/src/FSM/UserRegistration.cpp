#include "botstaff/FSM/UserRegistration.hpp"

using namespace TgBot;


void UserRegistration::choose_teacher(const std::string& chat_id)
{
    user->teacher = std::stol(chat_id);
    state = &UserRegistration::get_class;
    send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_cls);
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
        send_message(
            bot, user->chat_id, FSM_voc::user_reg_voc::_first_name);
    }).
    handle<msg::user_registration::class_fail>(
        [&](const msg::user_registration::class_fail& msg)
    {
        send_error_message(bot, user->chat_id);
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
        send_message(
            bot, user->chat_id,FSM_voc::user_reg_voc::_last_name);
    }).
    handle<msg::user_registration::first_name_fail>(
        [&](const msg::user_registration::first_name_fail& msg)
    {
        send_error_message(bot, user->chat_id);
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
        send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_phone);
    }).
    handle<msg::user_registration::last_name_fail>(
        [&](const msg::user_registration::last_name_fail& msg)
    {
        send_error_message(bot, user->chat_id);
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
        user->phone = msg.phone;
        send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_email);
    }).
    handle<msg::user_registration::phone_fail>(
        [&](const msg::user_registration::phone_fail& msg)
    {
        send_error_message(bot, user->chat_id);
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
        send_message_with_kb(
            bot, 
            user->chat_id, 
            FSM_voc::user_reg_voc::_agreement,
            Keyboards::agreement_kb()
        );
    }).
    handle<msg::user_registration::email_fail>(
        [&](const msg::user_registration::email_fail& msg)
    {
        send_error_message(bot, user->chat_id);
    });
}

void UserRegistration::agreement(const std::string& choice)
{
    std::string mess{};
    long chat_id = user->chat_id;
    if (choice == "no")
        mess = FSM_voc::user_reg_voc::_finish_no;
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
            mess = FSM_voc::user_reg_voc::_finish_yes;
        }
        else
        {
            mess = FSM_voc::user_reg_voc::_finish_error;
        }
    }
    send_message(bot, chat_id, mess);
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
    send_message_with_kb(
        bot, 
        teacher_id,
        vformat(FSM_voc::user_reg_voc::_update_kb, std::make_format_args(
            user->get_full_info())), 
        teacherKeyboards::update_user_info_kb(user->role)
    );
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
       send_error_message(bot, teacher_id);
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
        send_error_message(bot, teacher_id);
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
        send_error_message(bot, teacher_id);
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
        user->phone = msg.phone;
        send_update_kb();
    }).
    handle<msg::user_registration::phone_fail>(
        [&](const msg::user_registration::phone_fail& msg)
    {
        send_error_message(bot, teacher_id);
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
        send_error_message(bot, teacher_id);
    });
}

void UpdateUser::change_comment(const std::string& comment)
{
    user->comment = comment;
    send_update_kb();  
}

void UpdateUser::change_active_status(const std::string& status)
{
    if(status == FSM_voc::_change_status_y)
        user->is_active = true;
    else if(status == FSM_voc::_change_status_n)
        user->is_active = false;
    else
    {
        send_error_message(bot, teacher_id);
        return;
    }
    send_update_kb();
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
        child_email = email;
        state = &ParentRegistration::get_child_phone;
        send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_child_phone);
    }).
    handle<msg::user_registration::email_fail>(
        [&](const msg::user_registration::email_fail& msg)
    {
        send_error_message(bot, user->chat_id);
    });

    
}

void ParentRegistration::get_child_phone(const std::string& phone)
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
        child_phone = msg.phone;
    }).
    handle<msg::user_registration::phone_fail>(
        [&](const msg::user_registration::phone_fail& msg)
    {
        send_error_message(bot, user->chat_id);
        return;
    });

    filter_sender.send(
        msg::user_registration::check_pupil_exists(
            child_phone,
            child_email,
            messanger
        )
    );
    
    messanger.
    wait().
    handle<msg::user_registration::pupil_exists_ok>(
        [&](const msg::user_registration::pupil_exists_ok& msg)
    {
        state = &ParentRegistration::get_first_name;
        user->child = msg.child_id;
        send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_first_name);
    }).
    handle<msg::user_registration::pupil_exists_fail>(
        [&](msg::user_registration::pupil_exists_fail& msg)
    {
        send_message(
            bot, user->chat_id, FSM_voc::user_reg_voc::_child_not_found);
        return;    
    });
}

void ParentRegistration::get_first_name(const std::string& name)
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
        state = &ParentRegistration::get_last_name;
        user->first_name = name;
        send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_last_name);
    }).
    handle<msg::user_registration::first_name_fail>(
        [&](const msg::user_registration::first_name_fail& msg)
    {
        send_error_message(bot, user->chat_id);
    });
}

void ParentRegistration::get_last_name(const std::string& name)
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
        state = &ParentRegistration::get_phone;
        user->last_name = name;
        send_message(bot, user->chat_id, FSM_voc::user_reg_voc::_phone);
    }).
    handle<msg::user_registration::last_name_fail>(
        [&](const msg::user_registration::last_name_fail& msg)
    {
        send_error_message(bot, user->chat_id);
    });
}

void ParentRegistration::get_phone(const std::string& phone)
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
        state = &ParentRegistration::agreement;
        user->phone = msg.phone;
        
        send_message_with_kb(
            bot, 
            user->chat_id, 
            FSM_voc::user_reg_voc::_agreement, 
            Keyboards::agreement_kb()
        );
    }).
    handle<msg::user_registration::phone_fail>(
        [&](const msg::user_registration::phone_fail& msg)
    {
        send_error_message(bot, user->chat_id);
    });
}

void ParentRegistration::agreement(const std::string& choice)
{
    std::string mess{};
    long chat_id = user->chat_id;
    if (choice == "no")
        mess = FSM_voc::user_reg_voc::_finish_no;
    else
    {   
        try
        {
            user->create();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        mess = FSM_voc::user_reg_voc::_finish_yes;
    }

    send_message(bot, chat_id, mess);
}

void ParentRegistration::run(const std::string& message) 
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