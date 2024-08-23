add_library(bot
src/handlers/Handlers.cpp
src/database/DB.cpp
src/keyboards/Keyboards.cpp
src/keyboards/TeacherKeyboards.cpp
src/keyboards/UserKeyboards.cpp
src/utils/CalendarUtils.cpp 
src/utils/Utils.cpp 
src/FSM/Filter.cpp
src/FSM/LessonCreation.cpp
src/FSM/UserRegistration.cpp
src/handlers/TeacherHandlers.cpp
src/handlers/UserHandlers.cpp
src/States.cpp
)

target_include_directories(bot PUBLIC include)
