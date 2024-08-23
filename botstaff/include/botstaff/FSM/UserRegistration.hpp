#ifndef UserRegistration_hpp
#define UserRegistration_hpp

#include "Headers.hpp"
#include "botstaff/Vocabular.hpp"

using namespace TgBot;

inline std::unordered_map<std::string, std::string> user_update_messages = 
{
    {"first_name", user_reg_messages::_first_name},
    {"last_name", user_reg_messages::_last_name},
    {"class", user_reg_messages::_class},
    {"phone", user_reg_messages::_phone},
    {"email", user_reg_messages::_email},
    {"comments", user_reg_messages::_comments},
    {"status", user_reg_messages::_status},
    {"finish", user_reg_messages::_finish}
};


class UserRegistration
{
    messaging::receiver messanger;
    messaging::sender filter_sender;

    std::shared_ptr<BotUser> user;
    const Bot& bot;

    void (UserRegistration::* state)(const std::string&);

    void choose_teacher(const std::string& chat_id);
    void get_class(const std::string& cls);
    void get_first_name(const std::string& name);
    void get_last_name(const std::string& name);
    void get_phone(const std::string& phone);
    void get_email(const std::string& email);
    void agreement(const std::string& email);

    UserRegistration(const UserRegistration&) = delete;
    UserRegistration& operator=(const UserRegistration&) = delete;
public:
    
    UserRegistration(
        const messaging::sender& _fs,
        const TgBot::Bot& _bot, 
        long chat_id, 
        const std::string& username,
        const bot_roles& _role):
            filter_sender(_fs), 
            bot(_bot)
    {    
        user = std::make_shared<BotUser>(chat_id);
        user->tgusername = username;
        user->role = _role;
        if (_role == bot_roles::pupil)
            state = &UserRegistration::choose_teacher;
        else if(_role == bot_roles::teacher)
            state = &UserRegistration::get_first_name;
    }

    void run(const std::string& message);
    messaging::sender get_sender(){ return messanger; }
};


class UpdateUser
{
    messaging::receiver messanger;
    messaging::sender filter_sender;
    const TgBot::Bot& bot;
    std::shared_ptr<BotUser> user;
    long teacher_id;
    void (UpdateUser::* state)(const std::string&);
    
    
    typedef void (UpdateUser::*callable)(const std::string&);
    static std::unordered_map<std::string, callable> states;
    
    void change_first_name(const std::string& name);
    void change_last_name(const std::string& name);
    void change_class(const std::string& cls);
    void change_phone(const std::string& phone);
    void change_email(const std::string& email);
    void change_comment(const std::string& comment);
    void change_active_status(const std::string& status);

    void send_update_kb();
    
    UpdateUser(const UpdateUser&) = delete;
    UpdateUser& operator=(const UpdateUser&) = delete;
public:
    UpdateUser(
        const messaging::sender& _fs,
        const TgBot::Bot& _bot,
        std::shared_ptr<BotUser> _user,
        long _teacher_id
        ):
            filter_sender(_fs),
            bot(_bot),
            user(_user),
            teacher_id(_teacher_id)
    {}
    std::shared_ptr<BotUser> get_instance()
    { 
        return user; 
    }
    
    void change_state(const std::string& message);
    void run(const std::string& message);
    messaging::sender get_sender(){ return messanger; }
  
};


class ParentRegistration
{
    messaging::receiver messanger;
    messaging::sender filter_sender;

    std::shared_ptr<ParentBotUser> user;
    const Bot& bot;
    std::string child_email;
    std::string child_phone;

    void (ParentRegistration::* state)(const std::string&);

    void get_child_email(const std::string& email);
    void get_child_phone(const std::string& phone);

    void get_first_name(const std::string& name);
    void get_last_name(const std::string& name);
    void get_phone(const std::string& phone);
    void agreement(const std::string& email);

    ParentRegistration(const ParentRegistration&) = delete;
    ParentRegistration& operator=(const ParentRegistration&) = delete;
public:
    
    ParentRegistration(
        const messaging::sender& _fs,
        const TgBot::Bot& _bot, 
        long chat_id, 
        const std::string& username
        ):
            filter_sender(_fs), 
            bot(_bot)
    {    
        user = std::make_shared<ParentBotUser>(chat_id);
        user->tgusername = username;
        state = &ParentRegistration::get_child_email;
    }

    void run(const std::string& message);
    messaging::sender get_sender(){ return messanger; }
};
#endif