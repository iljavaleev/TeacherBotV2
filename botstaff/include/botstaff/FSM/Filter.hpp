#ifndef Filter_hpp
#define Filter_hpp

#include <regex>

#include "Headers.hpp"

using namespace TgBot;


class RegistrationFilter
{
    messaging::receiver messanger;
    bool name_is_ok(std::string name);
    int count_numbers(const std::string& str);
    bool phone_is_valid(const std::string& phone);
    bool user_exist(const std::string& phone, const std::string& email);
    
public:

    void done();
    
    void run();
    messaging::sender get_sender() { return messanger; }
};

#endif