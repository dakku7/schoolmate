#pragma once
#include <vector>
#include <string>
#include "sessions.h"
#include <memory>
#include <map>

class User;

class Calendar {
public:
    Calendar();

//   void addEvent(std::shared_ptr<Session>);
// void removeEvent(const std::string& sessionId);
    
    void showCalendarWeek(std::map<int, std::vector<Session>> sessions);
    std::map<int, std::vector<Session>> getCurrentWeekSessions(std::shared_ptr<User> u);
private:
    
    std::time_t getEndtOfTheWeek();
    std::time_t getStartOfTheWeek();
    int getDayOfTheWeek(std::time_t& t);
    void showDaySession(int arrow, std::map<int, std::vector<Session>>& sessions);
    std::vector<std::shared_ptr<Session>> events;
};