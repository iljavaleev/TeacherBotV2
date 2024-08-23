#ifndef States_hpp
#define States_hpp

#include <memory>         
#include <unordered_map>

class CreateLesson;
class ParentRegistration;
class UpdateLesson;
class UpdateUser;
class UserRegistration;

struct State
{
    static std::unordered_map<long, std::shared_ptr<CreateLesson>> 
        create_lesson_state;
    static std::unordered_map<long, std::shared_ptr<UpdateLesson>> 
        update_lesson_state;
    static std::unordered_map<long, std::shared_ptr<UserRegistration>> 
        user_registration_state;
    static std::unordered_map<long, std::shared_ptr<UpdateUser>> 
        user_update_state;
    static std::unordered_map<long, std::shared_ptr<ParentRegistration>> 
        parent_registration_state;

    static void clear_lesson_state(long);
    static void clear_lesson_update_state(long);
    static void clear_user_state(long);
    static void clear_update_user_state(long);
    static void clear_parent_registration_state(long);
    static void clear_all_states(long);
};

#endif