#pragma once
#include "sessions.h"
#include <vector>
#include <memory>
#include <array>
#include <string>
#include "Interests.h"
class User;

extern uint64_t global_id_counter_grp;

class Group {
public:
    Group();
    void showInfo(std::shared_ptr<User>& u);
    void showInfoJoin(std::shared_ptr<User>& u, std::vector<std::shared_ptr<Group>>& grp_, int& place);
    void addMember(const std::shared_ptr<User>& user);
    void removeMember(uint64_t uid);
   
    void addAdmin(const std::shared_ptr<User>& user);
    bool isAdmin(const std::shared_ptr<User>& user);
    void deleteAdmin(uint64_t uid);

    // 1 - make ; 2 - add;
    void makeSession();
    void makeSession(std::string topic, std::string desc, std::string location, std::string time, uint64_t gid);
    void addSession(const std::shared_ptr<Session>& s);

    const std::vector<std::weak_ptr<User>>& getMembers() const;
    std::vector<std::shared_ptr<Session>>& getSessions();
    std::string getTopic();
    std::string getDescription();


    void setGroup(std::string topic, std::string desc);
    std::array<bool, static_cast<size_t>(Intersets::COUNT)>& getInterests();
    void addInterests(Intersets intr);
    void deleteInterests(Intersets intr);
    
    void getInfo();
    uint64_t getGID();
    int getMembersCount();
    Group& getGroupByGID(const uint64_t& GID);

    void setId(uint64_t id);
    void setInterestsFromDB(const std::string& interestsStr);
    void setTopic(const std::string& topic);
    void setDescription(const std::string& desc);
    void setInterests(std::array<bool, static_cast<size_t>(Intersets::COUNT)>& intr);
    bool hasMember(uint64_t uid) const;

private:
    uint64_t id_group;
    std::string topic;
    std::string description;
    std::vector<std::weak_ptr<User>> members;

    //admins
    std::vector<uint64_t> admins_id;

    std::vector<std::shared_ptr<Session>> sessions;
    std::array<bool, size_t(Intersets::COUNT)>grp_interests;
    std::shared_ptr<User> findUserByUID(uint64_t uid);

};