#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "Interests.h"
#include <array>
#include <vector>

class Database;
class Group;

extern uint64_t global_id_counter;



class User {
public:
    User();

    bool login(const std::string& login, const std::string& password, Database* db);
    bool registerAccount(const std::string& login, const std::string& password, std::array<bool, static_cast<size_t>(Intersets::COUNT)>&);

    std::string getEmail() const;
    uint64_t getId() const;

    void joinGroup(const std::shared_ptr<Group>& group);
    void leaveGroup(const uint64_t& GID);
    const std::vector<std::shared_ptr<Group>>& getUserGroups() const;

    std::string getLogin() const;

    void changePassword(const std::string& oldPass, const std::string& newPass);
    bool getCheckPassword(const std::string& pswd);


    void showProfile();
    int getGroupNumber();
    void listAllGroups(int& place);

    std::array<bool, static_cast<size_t>(Intersets::COUNT)>& getInterests();
    void addInterest(Intersets i);
    void removeInterest(Intersets i);

    void setLogin(const std::string& login);
    void setId(uint64_t id);
    void setPassword(const std::string& password);
    void setEmail(const std::string& email);
    void setInterestsFromDB(const std::string& interestsStr);
    void setInteresetsManually(std::array<bool, static_cast<size_t>(Intersets::COUNT)>& it);
    std::string getPassword();
private:
    std::string login_user;
    std::string email;
    std::string password;
    std::vector<std::shared_ptr<Group>> groups;
    uint64_t id;
    std::array<bool, static_cast<size_t>(Intersets::COUNT)> interests;
    //bool privilageMode = false;

    inline const char* toString(Intersets interest) {
        return InterestNames[(int)interest];
    }
    void setUID();
};