#include "botstaff/States.hpp"

class CreateLesson;
class ParentRegistration;
class UpdateLesson;
class UpdateUser;
class UserRegistration;

std::unordered_map<long, std::shared_ptr<CreateLesson>> 
    State::create_lesson_state = 
        std::unordered_map<long, std::shared_ptr<CreateLesson>>();

std::unordered_map<long, std::shared_ptr<UpdateLesson>>  
    State::update_lesson_state = 
        std::unordered_map<long, std::shared_ptr<UpdateLesson>>();  

std::unordered_map<long, std::shared_ptr<UserRegistration>> 
    State::user_registration_state = 
        std::unordered_map<long, std::shared_ptr<UserRegistration>>();

std::unordered_map<long, std::shared_ptr<UpdateUser>> 
    State::user_update_state = 
        std::unordered_map<long, std::shared_ptr<UpdateUser>>();

std::unordered_map<long, std::shared_ptr<ParentRegistration>> 
    State::parent_registration_state = 
        std::unordered_map<long, std::shared_ptr<ParentRegistration>>();

void State::clear_lesson_state(long user_id)
{
    if (create_lesson_state.contains(user_id))
    {
        create_lesson_state.erase(user_id);
    }
}

void State::clear_lesson_update_state(long user_id)
{
    if (update_lesson_state.contains(user_id))
    {
        update_lesson_state.erase(user_id);
    }
}


void State::clear_user_state(long user_id)
{
    if (user_registration_state.contains(user_id))
    {
        user_registration_state.erase(user_id);
    }
}

void State::clear_update_user_state(long user_id)
{
    if (user_update_state.contains(user_id))
    {
        user_update_state.erase(user_id);
    }
}

void State::clear_parent_registration_state(long user_id)
{
     if (parent_registration_state.contains(user_id))
    {
        parent_registration_state.erase(user_id);
    }
}

void State::clear_all_states(long user_id)
{
    clear_lesson_state(user_id);
    clear_lesson_update_state(user_id);
    clear_user_state(user_id);
    clear_update_user_state(user_id);
    clear_parent_registration_state(user_id);
}

