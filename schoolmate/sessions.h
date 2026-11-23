#pragma once
#include <string>
#include <chrono>

enum class Mode { ONLINE, OFFLINE };
extern uint64_t global_id_counter_s;

class Session {
public:
    Session();
    void setSession();


    void showInfo();
    void reSetInfo();
    void setTime(const std::string& time);

    void setLocation(const std::string& loc); 
    const std::string& getLocation() const;

    void setDescription(const std::string& desc);
    const std::string& getDescription() const;

    void setTopic(const std::string& t);
    const std::string& getTopic() const;


    void setMode(const Mode md);
    Mode getMode() const;
   
    std::chrono::system_clock::time_point& getTime();
    std::stringstream timeToString(std::chrono::system_clock::time_point& t);
    void setParentId(uint64_t id);
    uint64_t getParentId();

    void setId(const uint64_t& n);
    uint64_t getId();

private:
    std::string topic;
    std::string description;
    uint64_t id_session;
    uint64_t id_parent_group;
    std::string location;
    Mode mode;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::chrono::system_clock::time_point stringToTimeConverstion(const std::string& s);

};