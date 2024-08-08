#include "botstaff/database/DB.hpp"
#include "botstaff/database/Quiries.hpp"

static std::mutex bdmtx;
using namespace TgBot;


pqxx::result sql_transaction(const std::string& query, bool read_only)
{
    std::lock_guard lk(bdmtx);
    pqxx::result R;
    try
    {
        pqxx::connection c{URI};
        if (c.is_open())
        {
            std::cout << "db is open";
        }
        else
        {
            std::cout << "closed";
        }
        std::cout << std::endl;

        
        if (read_only)
        {
            pqxx::nontransaction N(c);
            R = N.exec(query);
        }
        else
        {
            pqxx::work wrk(c);  
            R = wrk.exec(query);
            wrk.commit();
        }
        std::cout << "Work is done successfully" << std::endl;
        c.close();
        return R;

    }   
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return R;
    }
}   


std::shared_ptr<BotUser> BotUser::construct(const pqxx::row& res)
{
    std::shared_ptr<BotUser> user(new BotUser());
    if (res.empty())
        return user;
    if (res.at(9).as<std::string>() == "pupil")
    {
        user->teacher = res.at(1).as<long>();
        user->cls = res.at(7).as<std::string>();
    }
    user->chat_id = res.at(0).as<long>();
    user->tgusername = res.at(2).as<std::string>();
    user->first_name = res.at(3).as<std::string>();
    user->last_name = res.at(4).as<std::string>();
    user->phone = res.at(5).as<std::string>();
    user->email = res.at(6).as<std::string>();
    user->comment = res.at(8).as<std::string>();
    user->role = (bot_roles) res.at(9).as<int>();
    user->is_active = res.at(10).as<bool>();
    return user;
}


std::shared_ptr<UserLesson>  UserLesson::construct(const pqxx::row& res)
{
    std::shared_ptr<UserLesson> user_lesson(new UserLesson());
    if (res.empty())
        return user_lesson;
    user_lesson->id = res.at(0).as<int>();
    user_lesson->date = res.at(1).as<std::string>();
    user_lesson->time = res.at(2).as<std::string>();
    user_lesson->teacher = res.at(3).as<long>();
    user_lesson->pupil = res.at(4).as<long>();
    user_lesson->objectives = res.at(5).as<std::string>();
    user_lesson->comment_for_pupil = res.at(6).as<std::string>();
    user_lesson->comment_for_teacher = res.at(7).as<std::string>();
    user_lesson->comment_for_teacher = res.at(8).as<std::string>();
    user_lesson->is_paid = res.at(9).as<bool>();
    return user_lesson;
}

const std::string BotUser::_get = "SELECT * FROM bot_user WHERE chat_id={};";
const std::string BotUser::_get_all = "SELECT * FROM bot_user;";
const std::string BotUser::_create_teacher = "INSERT INTO bot_user VALUES \
({}, null, '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', {})\
RETURNING *;";
const std::string BotUser::_create_pupil = "INSERT INTO bot_user VALUES \
({}, {}, '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', {})\
RETURNING *;";
const std::string BotUser::_update = "UPDATE bot_user SET teacher = {}, \
tgusername = '{}', first_name = '{}', last_name = '{}', phone = '{}', \
email = '{}', class = '{}', comment = '{}', user_role = '{}',\
is_active = {} WHERE chat_id = {} RETURNING *;";
const std::string BotUser::_destroy = "DELETE FROM bot_user WHERE chat_id={} \
RETURNING *";
const std::string BotUser::_activate = "UPDATE bot_user SET is_active=true \
WHERE chat_id = {} RETURNING *";

std::shared_ptr<BotUser> BotUser::get(int pk)
{
    return sql::get<BotUser>(BotUser::_get, pk);
}

std::vector<std::shared_ptr<BotUser>> BotUser::get_all()
{
    return sql::get_all<BotUser>(BotUser::_get_all);
}

std::vector<std::shared_ptr<BotUser>> BotUser::get_all(const std::string& q)
{
    return sql::get_all<BotUser>(q);
}

std::shared_ptr<BotUser> BotUser::destroy(int pk)
{
    return sql::destroy<BotUser>(BotUser::_destroy, pk);
}

std::shared_ptr<BotUser> BotUser::destroy()
{
    return sql::destroy<BotUser>(BotUser::_destroy, chat_id);
}

std::shared_ptr<BotUser> BotUser::activate(int pk)
{
    return sql::create_update<BotUser>(BotUser::_activate, pk);
}

std::shared_ptr<BotUser> BotUser::update(
    long teacher,
    const std::string& tgusername,
    const std::string& first_name,
    const std::string& last_name,
    const std::string& phone,
    const std::string& email,
    const std::string& cls,
    const std::string& comment,
    const bot_roles& role,
    bool is_active,
    int chat_id
)
{
    return sql::create_update<BotUser>(
        BotUser::_update, 
        teacher,
        tgusername,
        first_name,
        last_name,
        phone,
        email,
        cls,
        comment,
        static_cast<int>(role),
        is_active,
        chat_id
    );
}
std::shared_ptr<BotUser> BotUser::create_teacher(
    int chat_id,
    const std::string& tgusername,
    const std::string& first_name,
    const std::string& last_name,
    const std::string& phone,
    const std::string& email,
    const std::string& cls,
    const std::string& comment,
    const bot_roles& role,
    bool is_active
)
{
    return sql::create_update<BotUser>(
        BotUser::_create_teacher, 
        chat_id,
        tgusername,
        first_name,
        last_name,
        phone,
        email,
        cls,
        comment,
        static_cast<int>(role),
        is_active
    );
}
std::shared_ptr<BotUser> BotUser::create_pupil(
    int chat_id,
    long teacher,
    const std::string& tgusername,
    const std::string& first_name,
    const std::string& last_name,
    const std::string& phone,
    const std::string& email,
    const std::string& cls,
    const std::string& comment,
    const bot_roles& role,
    bool is_active
)
{
    std::lock_guard lk(bdmtx);
    return sql::create_update<BotUser>(
        BotUser::_create_pupil, 
        chat_id,
        teacher,
        tgusername,
        first_name,
        last_name,
        phone,
        email,
        cls,
        comment,
        static_cast<int>(role),
        is_active
    );
}

std::shared_ptr<BotUser> BotUser::update()
{
    return sql::create_update<BotUser>(
        BotUser::_update, 
        teacher,
        tgusername,
        first_name,
        last_name,
        phone,
        email,
        cls,
        comment,
        static_cast<int>(role),
        is_active,
        chat_id
    );
}
    
std::shared_ptr<BotUser> BotUser::create_teacher()
{
    return sql::create_update<BotUser>(
        BotUser::_create_teacher, 
        chat_id,
        tgusername,
        first_name,
        last_name,
        phone,
        email,
        cls,
        comment,
        static_cast<int>(role),
        is_active
    );
}

std::shared_ptr<BotUser> BotUser::create_pupil()
{
    return sql::create_update<BotUser>(
        BotUser::_create_pupil, 
        chat_id,
        teacher,
        tgusername,
        first_name,
        last_name,
        phone,
        email,
        cls,
        comment,
        static_cast<int>(role),
        is_active
    );
}

const std::string UserLesson::_get = "SELECT * FROM user_lesson WHERE id={};";
const std::string UserLesson::_get_all = "SELECT * FROM user_lesson;";
const std::string UserLesson::_update = "UPDATE user_lesson SET \
id=DEFAULT, date='{}', time='{}', teacher={}, pupil = {}, objectives = '{}', \
comment_for_pupil = '{}', comment_for_teacher = '{}', \
comment_for_parent = '{}', is_paid = {} WHERE id = {} RETURNING *";
const std::string UserLesson::_create = "INSERT INTO user_lesson VALUES \
(DEFAULT, '{}', '{}', {}, {}, '{}', '{}', '{}', '{}', {}) RETURNING *;";
const std::string UserLesson::_destroy = "DELETE FROM user_lesson \
WHERE id={} RETURNING *;";


std::shared_ptr<UserLesson> UserLesson::get(int pk)
{
    return sql::get<UserLesson>(UserLesson::_get, pk);
}

std::vector<std::shared_ptr<UserLesson>> UserLesson::get_all()
{
    return sql::get_all<UserLesson>(UserLesson::_get_all);
}

std::vector<std::shared_ptr<UserLesson>> UserLesson::get_all(
    const std::string& q
)
{
    return sql::get_all<UserLesson>(q);
}

std::shared_ptr<UserLesson> UserLesson::destroy(int pk)
{
    return sql::destroy<UserLesson>(UserLesson::_destroy, pk);
}

std::shared_ptr<UserLesson> UserLesson::destroy()
{
    return sql::destroy<UserLesson>(UserLesson::_destroy, id);
}

std::shared_ptr<UserLesson> UserLesson::update(
    const std::string& date,
    const std::string& time,
    long teacher,
    long pupil,
    const std::string& objectives,
    const std::string& comment_for_pupil,
    const std::string& comment_for_teacher,
    const std::string& comment_for_parent,
    bool is_paid,
    int id
)
{
    return sql::create_update<UserLesson>(
        UserLesson::_update, 
        date,
        time,
        teacher,
        pupil,
        objectives,
        comment_for_pupil,
        comment_for_teacher,
        comment_for_parent,
        is_paid,
        id
    );
}

std::shared_ptr<UserLesson> UserLesson::create(
    const std::string& date,
    const std::string& time,
    long teacher,
    long pupil,
    const std::string& objectives,
    const std::string& comment_for_pupil,
    const std::string& comment_for_teacher,
    const std::string& comment_for_parent,
    bool is_paid
)
{
    return sql::create_update<UserLesson>(
        UserLesson::_create, 
        date,
        time,
        teacher,
        pupil,
        objectives,
        comment_for_pupil,
        comment_for_teacher,
        comment_for_parent,
        is_paid
    );
}

std::shared_ptr<UserLesson> UserLesson::update()
{
    return sql::create_update<UserLesson>(
        UserLesson::_update, 
        date,
        time,
        teacher,
        pupil,
        objectives,
        comment_for_pupil,
        comment_for_teacher,
        comment_for_parent,
        is_paid,
        id
    );
}
    

std::shared_ptr<UserLesson> UserLesson::create()
{
    return sql::create_update<UserLesson>(
        UserLesson::_create, 
        date,
        time,
        teacher,
        pupil,
        objectives,
        comment_for_pupil,
        comment_for_teacher,
        comment_for_parent,
        is_paid
    );
}

std::string LessonInfo::get_info_for_teacher()
{
    return std::format(
        "<b>Pupil: {}</b>\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for teacher</b>: {}", 
        pupil,
        time, 
        objectives, 
        comment_for_teacher
    );
};

std::string LessonInfo::get_info_for_pupil()
{
    return std::format(
        "<b>Teacher {}</b>\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for student</b>: {}", 
        teacher,
        time, 
        objectives, 
        comment_for_pupil
    );
}

std::string LessonInfo::get_info_for_parent()
{
    return std::format(
        "<b>Teacher {}</b>\n"
        "<b>Date</b>: {}\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for parent</b>: {}\n"
        "<b>Payment status</b>: {}", 
        teacher,
        date,
        time, 
        objectives, 
        comment_for_parent,
        is_paid
    );
}

std::string LessonInfo::get_full_info()
{
    return std::format(
        "<b>ID: {}</b>\n"
        "<b>Teacher {}</b>\n"
        "<b>Pupil {}</b>\n"
        "<b>Date</b>: {}\n"
        "<b>Class start time: {}</b>\n"
        "<b>Objectives</b>: {}\n"
        "<b>Comments for parent</b>: {}\n"
        "<b>Comments for student</b>: {}\n"
        "<b>Comments for teacher</b>: {}\n" 
        "<b>Статус оплаты</b>: {}",
        id,
        teacher,
        pupil, 
        date, 
        time,
        objectives,
        comment_for_parent,
        comment_for_teacher,
        comment_for_pupil,
        is_paid
    );
}

void LessonInfo::set_pupil(long chat_id)
    {
        std::shared_ptr<BotUser> user = BotUser::get(chat_id);
        pupil = std::format("{} {}", user->first_name, user->last_name);
        lesson->pupil = chat_id;
    }

void LessonInfo::set_is_paid(bool paid)
{
    lesson->is_paid = paid;
    is_paid = paid ? "yes" : "no";
}

void LessonInfo::set_date(const std::string& _date)
{
    date = _date;
    lesson->date = _date;
}

void LessonInfo::set_time(const std::string& _time)
{
    time = _time;
    lesson->time = _time;
}

void LessonInfo::set_objectives(const std::string& _objectives)
{
    objectives = _objectives;
    lesson->objectives = _objectives;
}
void LessonInfo::set_comment_for_pupil(const std::string& _comment_for_pupil)
{
    comment_for_pupil = _comment_for_pupil;
    lesson->comment_for_pupil = _comment_for_pupil;
}
void LessonInfo::set_comment_for_parent(const std::string& _comment_for_parent)
{
    comment_for_parent = _comment_for_parent;
    lesson->comment_for_parent = _comment_for_parent;
}
void LessonInfo::set_comment_for_teacher(const std::string& _comment_for_teacher)
{
    comment_for_teacher = _comment_for_teacher;
    lesson->comment_for_teacher = _comment_for_teacher;
}


std::unordered_set<int> get_lesson_days(
    const YMD& ymd,
    long chat_id, 
    const bot_roles& brole
)
{
    std::unordered_set<int> dates;
    std::string query;
    std::string role{brole == bot_roles::teacher ? "teacher" : "pupil"};
    
    
    query = create_query(
        other_quiries::_get_lesson_days, ymd.year, ymd.month, role, chat_id
    );
   
    pqxx::result res = sql_transaction(query);
    for (auto it{res.begin()}; it != res.end(); ++it)
    {
        dates.insert(it->at(0).as<int>());
    }

    return dates;
}

std::vector<std::vector<std::string>> get_lessons_by_day(
    const YMD& ymd,
    long chat_id, 
    const bot_roles& role
)
{
    std::string date = std::format("{}-{}-{}", ymd.year, ymd.month, ymd.day);
    std::string arole, brole;
    if (role == bot_roles::teacher)
    {
        arole = "teacher";
        brole = "pupil";
    }
    else
    {
        brole = "teacher";
        arole = "pupil";
    }

    std::string query = create_query(
        other_quiries::_get_lessons_by_day, 
        brole, 
        arole, 
        chat_id, 
        date, 
        brole
    ); 
    
    pqxx::result R = sql_transaction(query);
    std::vector<std::vector<std::string>> result;
    
    for (auto it{R.begin()}; it != R.end(); ++it)
    {
        std::vector<std::string> v = {
            it->at(0).as<std::string>(),
            it->at(1).as<std::string>(),
            it->at(2).as<std::string>(),
            it->at(3).as<std::string>()
        };
        result.push_back(v);
    }

    return result;
}



std::string get_comment_text(int id)
{
    std::string query = create_query(other_quiries::_get_comment_text, id); 
    pqxx::result R = sql_transaction(query);
    auto it{R.begin()};
    std::string date{it->at(0).as<std::string>()};
    std::string first_name{it->at(1).as<std::string>()};
    std::string last_name{it->at(2).as<std::string>()};
    std::string comment_for_teacher{it->at(3).as<std::string>()};
    
    return std::format(
        "<b>Ученик: {} {}</b>\n<b>Дата: {}</b>\n\n<b>Ваш комментарий</b>: {}\n", 
        date, first_name, last_name, comment_for_teacher);
}

std::string get_pupil_info(const TgBot::Message::Ptr& message)
{
    auto u = BotUser::get(message->chat->id);
    return std::format(
        "<b>{} {} {}</b>\n\
        <b>Юзернэйм</b>: {}\n\
        <b>Указан телефон</b>: {}\n\
        <b>Адрес электронной почты</b>:{}\n\
        <b>Комментарий:</b>{}", 
        u->first_name, 
        u->last_name, 
        u->cls, 
        u->tgusername, 
        u->phone, 
        u->email, 
        u->comment
    );
}

void get_comments_for_kb(InlineKeyboardMarkup::Ptr kb, long chat_id)
{
    std::vector<InlineKeyboardButton::Ptr> row;
    
    std::string query = create_query(
        other_quiries::_get_comments_for_kb, chat_id
    ); 
    
    pqxx::result R = sql_transaction(query); 

    for (auto it{R.begin()}; it!=R.end(); ++it)
    {
        long id{it->at(0).as<long>()};
        std::string date{it->at(1).as<std::string>()};
        std::string first_name{it->at(2).as<std::string>()};
        std::string last_name{it->at(3).as<std::string>()};
        
        std::vector<InlineKeyboardButton::Ptr> row;
        InlineKeyboardButton::Ptr comment_btn(new InlineKeyboardButton);
        comment_btn->text = std::format(
            "{} {} {}", 
            date, 
            first_name, 
            last_name
        );
        comment_btn->callbackData = std::format(
            "comment_for_teacher {}", 
            id
        );
        row.push_back(comment_btn);
        kb->inlineKeyboard.push_back(row);
    }
}

void get_users_for_kb(
    InlineKeyboardMarkup::Ptr kb,
    const std::string& query,
    const std::string& callback
)
{
    std::vector<std::shared_ptr<BotUser>> user_list = BotUser::get_all(query);
    for (auto it{user_list.begin()}; it!=user_list.end(); ++it)
    {
        std::vector<InlineKeyboardButton::Ptr> row;
        InlineKeyboardButton::Ptr user_btn(new InlineKeyboardButton);
        user_btn->text = std::format(
            "{} {}", 
            (*it)->first_name, 
            (*it)->last_name
        );
        user_btn->callbackData = std::vformat(
                callback, 
                std::make_format_args((*it)->chat_id)
            );
        row.push_back(user_btn);
        kb->inlineKeyboard.push_back(row);
    }
}

std::string get_user_lesson_info(
    long chat_id, 
    int user_lesson_id, 
    const bot_roles& role
)
{   
    std::shared_ptr<UserLesson> user_lesson = UserLesson::get(user_lesson_id);
    if (!user_lesson->id)
        return "Произошла ошибка, повторите позднее";
    LessonInfo info(user_lesson);
    if (role == bot_roles::teacher or role == bot_roles::admin)
        return info.get_info_for_teacher();
    if (role == bot_roles::pupil)
        return info.get_info_for_pupil();
    if (role == bot_roles::parent)
        return info.get_info_for_parent();
    return std::string{};
}
