#ifndef Vocabular_hpp
#define Vocabular_hpp

#include <vector>
#include <string>

#define EN

#ifdef RU
const static std::vector<std::string> MONTHS_RU = {
    "Январь", 
    "Февраль", 
    "Март", 
    "Апрель", 
    "Май", 
    "Июнь", 
    "Июль", 
    "Август", 
    "Сентябрь", 
    "Октябрь", 
    "Ноябрь", 
    "Декабрь"
};
#endif
#ifdef EN
const static std::vector<std::string> MONTHS_EN = {
    "January", 
    "February", 
    "March", 
    "April", 
    "May", 
    "June", 
    "July", 
    "August", 
    "September", 
    "October", 
    "November", 
    "December"
};
#endif

#endif
