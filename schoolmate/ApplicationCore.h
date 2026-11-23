#pragma once
#include "matching_alg.h"
#include "database.h"
#include "user.h"
#include "calendar.h"
#include "groups.h"
#include "sessions.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "UI.h"


class AppCore {
public:
    AppCore();
    void run();    

private:
    std::shared_ptr<User> currentUser = nullptr;
    std::vector<std::shared_ptr<Group>> allGroups;
    std::vector<std::shared_ptr<User>> allUsers;
    std::unique_ptr<Calendar> calendar;
    std::unique_ptr<MatchEng> matcher;
    std::unique_ptr<Database> database;
    bool isRunning = false;
    bool isLoggedIn = false;

 
    void mainMenu();
    void loginMenu();
    void groupMenu();
    void calendarMenu();
    std::shared_ptr<User> findUser(const std::string& login, const std::string& password);
    std::shared_ptr<Group> findGroup(const std::shared_ptr<Group> allgrp);
    void leavingProcces(int& p_);
    bool isAppRunning();
    void setRunning(bool st);

    std::shared_ptr<User> getCurrentUser();
    std::vector<std::shared_ptr<Group>> getAllGroups();
    std::vector<std::shared_ptr<User>> getAllUsers();
    std::unique_ptr<Calendar>& getCalendar();
    std::unique_ptr<MatchEng>& getMatcher();

    void createGroup();
    void createSession(uint64_t groupId);
    void editInterests(std::array<bool, static_cast<size_t>(Intersets::COUNT)>& currentInterests);
    
    void loadDatabase();
    void saveInDatabase();
    void saveGroups();
    void saveUsers();
    friend class UI;
};
