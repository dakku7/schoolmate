#pragma once
#include <vector>
#include <memory>
#include "groups.h"
#include "user.h"
#include "database.h"

class MatchEng {
private:
    float scoreCompatibility(std::shared_ptr<User>& u, std::shared_ptr<Group>& group);

public:
    MatchEng();
    std::vector<std::shared_ptr<Group>> findMatches(std::shared_ptr<User>& u, std::vector<std::shared_ptr<Group>>& grp, bool onlinePrefer);
    std::vector<std::shared_ptr<User>> findUsers(User& user);
    std::vector<std::shared_ptr<Group>> searchGroups(const std::string& keyword);
    void showRecomendedGroups(std::shared_ptr<User>& u, std::vector<std::shared_ptr<Group>>& grp_, Database* db);
    std::string stringifyInterests3(const std::array<bool, (size_t)Intersets::COUNT>& arr);
};