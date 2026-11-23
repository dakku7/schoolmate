#pragma once
#include <string>
#include "sessions.h"
#include "user.h"
#include "sqlite3.h"
#include <sstream>
#include <vector>
class User;
class Group;

class Database {
private:
    sqlite3* db;
    //for erros
    char* zErrMsg = 0;
    //completed operations
    int rc = 0;

    std::string interestsToString(const std::array<bool, 22>& interests);
    bool execQuery(const std::string& path);
public:
    Database();
    void loadAllData(std::vector<std::shared_ptr<User>>& allusers, std::vector<std::shared_ptr<Group>>& allgroups);

    bool open(const std::string& path);
    void close();

    void saveUser(std::shared_ptr<User>& user);
    void saveGroup(std::shared_ptr<Group>& group);
    void saveSession(std::shared_ptr<Session>& session);
    void saveUserGroupConnection(uint64_t userId, uint64_t groupId);
    void deleteUserGroupConnection(uint64_t userId, uint64_t groupId);

     ~Database();
};