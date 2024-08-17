DROP TABLE IF EXISTS bot_user cascade;
DROP TABLE IF EXISTS parent_bot_user cascade;
DROP TABLE IF EXISTS user_lesson;
DROP TABLE IF EXISTS rescedule_table;


CREATE TABLE bot_user(
    chat_id bigint PRIMARY KEY,
    teacher bigint,
    tgusername varchar(128),
    first_name varchar(255),
    last_name varchar(255),
    phone varchar(128) UNIQUE,
    email varchar(255) UNIQUE,
    class varchar(32),
    comment text,
    user_role int,
    is_active boolean DEFAULT FALSE,
    FOREIGN KEY (teacher) REFERENCES bot_user (chat_id) ON DELETE CASCADE 
);

CREATE TABLE parent_bot_user(
    chat_id bigint PRIMARY KEY,
    child bigint,
    tgusername varchar(128),
    first_name varchar(255),
    last_name varchar(255),
    phone varchar(128) UNIQUE,
    FOREIGN KEY (child) REFERENCES bot_user (chat_id) ON DELETE CASCADE 
);

CREATE TABLE rescedule_table(
    id SERIAL PRIMARY KEY,
    chat_id bigint,
    date date,
    comment text,
    UNIQUE(chat_id, date),
    FOREIGN KEY (chat_id) REFERENCES bot_user (chat_id) ON DELETE CASCADE 
);

CREATE TABLE user_lesson(
    id SERIAL PRIMARY KEY,
    date date UNIQUE,
    time varchar(32) NOT NULL,
    teacher bigint,
    pupil bigint,
    objectives varchar(255) NOT NULL,
    comment_for_pupil text,
    comment_for_teacher text,
    comment_for_parent text,
    is_paid boolean DEFAULT FALSE,
    FOREIGN KEY (pupil) REFERENCES bot_user (chat_id) ON DELETE CASCADE,
    FOREIGN KEY (teacher) REFERENCES bot_user (chat_id) ON DELETE CASCADE
);

-- SET YOUR QUERY TO CREATE ADMIN
-- INSERT INTO bot_user VALUES (1111111, null, 'admin', 'admin admin', 'admin', '+() 12 23 34', 'mail@mail.com', null, 'no comments', 'teacher', TRUE);