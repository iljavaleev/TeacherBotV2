#ifndef DB_hpp
#define DB_hpp

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include "botstaff/utils/Utils.hpp"
#include "botstaff/utils/CalendarUtils.hpp"
#include <format>
#include <mutex>
#include <tgbot/tgbot.h>
#include "Quiries.hpp"

enum class bot_roles;

static const std::string URI = std::format(
    "dbname={} user={} password={} host={} port={}", 
    std::getenv("POSTGRES_DB") ? std::getenv("POSTGRES_DB") : "postgres", 
    std::getenv("POSTGRES_USER") ? std::getenv("POSTGRES_USER") : "postgres", 
    std::getenv("POSTGRES_PASSWORD") ? std::getenv("POSTGRES_PASSWORD") : 
    "postgres",
    std::getenv("POSTGRES_HOST") ? std::getenv("POSTGRES_HOST") : "localhost",
    std::getenv("POSTGRES_PORT") ? std::getenv("POSTGRES_PORT") : "5432"
);

pqxx::result sql_transaction(const std::string& query, bool read_only = true);


template<typename ... Args> std::string create_query(
    const std::string& bluep, Args&&... args
)
{
    return std::vformat(bluep, std::make_format_args(args...)); 
}  

namespace sql
{
        
    template<typename T, typename PK> 
    std::shared_ptr<T> get(const std::string& query, PK pk)
    {
        std::string q = create_query(query, pk);
        pqxx::result res = sql_transaction(q);
        if(res.empty())
            return nullptr;
        return T::construct(*res.begin());
    }

    template<typename T>
    std::vector<std::shared_ptr<T>> get_all(const std::string& query)
    {
        std::vector<std::shared_ptr<T>> objects;
        pqxx::result res = sql_transaction(query);
        for (auto i{res.begin()}; i != res.end(); ++i)
        {
            objects.emplace_back(T::construct(*i));
        }
        
        return objects;
    }

    template<typename T, typename ... Args>
    std::shared_ptr<T> create_update(
        const std::string& query,
        Args&&... args
        )
    {
        std::string q = create_query(query, args...);
        pqxx::result res = sql_transaction(q, false);
        return T::construct(*res.begin());
    }

    template<typename T, typename PK>
    std::shared_ptr<T> destroy(
        const std::string& query, 
        PK pk
        )
    {
        if(!get<T>(T::_get, pk))
            return nullptr;
        std::string q = create_query(query, pk);
        pqxx::result res = sql_transaction(q, false);

        return T::construct(*res.begin());
    }

}

struct BotUser{
    static const std::string _get;
    static const std::string _get_all;
    static const std::string _create_teacher;
    static const std::string _create_pupil;
    static const std::string _update;
    static const std::string _destroy;
    static const std::string _activate;  
    
    BotUser() = default;
    BotUser(long chat_id):chat_id(chat_id){}
    
    long chat_id{};
    long teacher{};
    std::string tgusername{};
    std::string first_name{};
    std::string last_name{};
    std::string phone{};
    std::string email{};
    std::string cls{};
    std::string comment{};
    bot_roles role{};
    bool is_active{false};

    bool empty() const
    {
        return !static_cast<bool>(chat_id);
    }

    static std::shared_ptr<BotUser> construct(const pqxx::row& res);

    static std::shared_ptr<BotUser> get(int);
    static std::shared_ptr<BotUser> activate(int);
    static std::vector<std::shared_ptr<BotUser>> get_all(const std::string&);
    static std::vector<std::shared_ptr<BotUser>> get_all();
    static std::shared_ptr<BotUser> destroy(int);
    static std::shared_ptr<BotUser> update(
        long, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&,
        const std::string&,
        const bot_roles&, 
        bool,
        int
    );
    static std::shared_ptr<BotUser> create_teacher(
        int,
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&,
        const std::string&,
        const bot_roles&,
        bool
    );
    static std::shared_ptr<BotUser> create_pupil(
        int,
        long, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&, 
        const std::string&,
        const std::string&,
        const bot_roles&, 
        bool
    );
    
    std::shared_ptr<BotUser> destroy();
    std::shared_ptr<BotUser> update();
    std::shared_ptr<BotUser> create_teacher();
    std::shared_ptr<BotUser> create_pupil();
};


struct UserLesson{
    static const std::string _get;
    static const std::string _get_all;
    static const std::string _create_with_contact;
    static const std::string _create;
    static const std::string _update;
    static const std::string _destroy;
    
    int id{};
    std::string date{};
    std::string time{};
    long teacher{};
    long pupil{};
    std::string objectives{};
    std::string comment_for_pupil{};
    std::string comment_for_teacher{};
    std::string comment_for_parent{};
    bool is_paid{false};


    static std::shared_ptr<UserLesson> construct(const pqxx::row& res);
    static std::shared_ptr<UserLesson> get(int);
    static std::vector<std::shared_ptr<UserLesson>> get_all();
    static std::vector<std::shared_ptr<UserLesson>> get_all(const std::string&);
    static std::shared_ptr<UserLesson> destroy(int);
    static std::shared_ptr<UserLesson> update(
        const std::string&,
        const std::string&, 
        long, 
        long, 
        const std::string&, 
        const std::string&, 
        const std::string&,
        const std::string&, 
        bool,
        int
    );
    static std::shared_ptr<UserLesson> create(
        const std::string&,
        const std::string&, 
        long, 
        long, 
        const std::string&, 
        const std::string&, 
        const std::string&,
        const std::string&,  
        bool
    );
    std::shared_ptr<UserLesson> destroy();
    std::shared_ptr<UserLesson> update();
    std::shared_ptr<UserLesson> create();

    void print()
    {
        std::cout << id << std::endl;
        std::cout << date << std::endl;
        std::cout << time << std::endl;
        std::cout << teacher << std::endl;
        std::cout << pupil << std::endl;
        std::cout << objectives << std::endl;
        std::cout << comment_for_pupil << std::endl;
        std::cout << comment_for_parent << std::endl;
        std::cout << comment_for_teacher << std::endl;
        std::cout << is_paid << std::endl;
    }
};

struct LessonInfo
{
    const std::shared_ptr<UserLesson> lesson;
    std::string id{};
    std::string date{};
    std::string time{};
    std::string teacher{};
    std::string pupil{};
    std::string objectives{};
    std::string comment_for_pupil{};
    std::string comment_for_teacher{};
    std::string comment_for_parent{};
    std::string is_paid{};

    LessonInfo() = default;
    LessonInfo(const std::shared_ptr<UserLesson> _lesson):lesson(_lesson)
    {
        id = std::to_string(_lesson->id);
        date = _lesson->date;
        time = _lesson->time;
        auto result = sql_transaction(
            create_query(
                other_quiries::_lesson_info, _lesson->teacher, _lesson->pupil));

        auto iter = result.begin();
        teacher = std::format(
            "{} {}", 
            iter.at(0).as<std::string>(), 
            iter.at(1).as<std::string>()
        );
        iter++;
        pupil = std::format(
            "{} {}", 
            iter.at(0).as<std::string>(),
            iter.at(1).as<std::string>()
        );
        objectives = _lesson->objectives;
        comment_for_pupil = _lesson->comment_for_pupil;
        comment_for_teacher = _lesson->comment_for_teacher;
        comment_for_parent = _lesson->comment_for_parent;
        is_paid = _lesson->is_paid ? "yes" : "no";
    }
    
    LessonInfo(int lesson_id): LessonInfo(UserLesson::get(lesson_id)){}

    std::string get_info_for_teacher();
    std::string get_info_for_pupil();
    std::string get_info_for_parent();
    std::string get_full_info();

    void set_pupil(long chat_id);
    void set_is_paid(bool paid);
    void set_date(const std::string& date);
    void set_time(const std::string& time);
    void set_objectives(const std::string& objectives);
    void set_comment_for_pupil(const std::string& comment_for_pupil);
    void set_comment_for_parent(const std::string& comment_for_parent);
    void set_comment_for_teacher(const std::string& comment_for_teacher);
    void print()
    {
        std::cout << id << std::endl;
        std::cout << date << std::endl;
        std::cout << time << std::endl;
        std::cout << teacher << std::endl;
        std::cout << pupil << std::endl;
        std::cout << objectives << std::endl;
        std::cout << comment_for_pupil << std::endl;
        std::cout << comment_for_parent << std::endl;
        std::cout << comment_for_teacher << std::endl;
        std::cout << is_paid << std::endl;
    }
};



std::unordered_set<int> get_lesson_days(
    const YMD& ymd,
    long chat_id, 
    const bot_roles& role
);

std::vector<std::vector<std::string>> get_lessons_by_day(
    const YMD& ymd,
    long chat_id, 
    const bot_roles& role
);


std::string get_comment_text(int id);
std::string get_pupil_info(const TgBot::Message::Ptr& message);

std::string get_user_lesson_info(
    long chat_id, 
    int user_lesson_id, 
    const bot_roles& role
);

void get_comments_for_kb(TgBot::InlineKeyboardMarkup::Ptr kb, long chat_id);
void get_users_for_kb(
    TgBot::InlineKeyboardMarkup::Ptr kb,
    const std::string&  query,
    const std::string& callback
);

// template<typename FT, typename RT, typename PK> 
// static std::vector<std::shared_ptr<RT>> get_all_related(PK pk)
// {
//     std::vector<std::shared_ptr<RT>> objects;
    
//     pqxx::result res = sql_transaction(FT::GetAllRelated(pk));
//     for (auto i{res.begin()}; i != res.end(); ++i)
//     {
//         objects.emplace_back(RT::construct(*i));
//     }
//     return objects;
// }



#endif 
