#ifndef UserRegistration_hpp
#define UserRegistration_hpp

#include "Headers.hpp"

using namespace TgBot;

class UserRegistration
{
    messaging::receiver messanger;
    messaging::sender filter_sender;

    std::shared_ptr<BotUser> user;
    const Bot& bot;

    void (UserRegistration::* state)(const std::string&);


    void get_first_name(const std::string& name);
    void get_last_name(const std::string& name);
    void get_class(const std::string& cls);
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
        const std::string& username):
            filter_sender(_fs), 
            bot(_bot)      
    {    
        user = std::make_shared<BotUser>(chat_id);
        user->tgusername = username;
        state = &UserRegistration::get_first_name;
    }

    void done();
    void run(const std::string& message);
    messaging::sender get_sender(){ return messanger; }
};

#endif