#define _CRT_SECURE_NO_WARNINGS
#include "sessions.h"
#include "groups.h"
#include <sstream>
#include <locale>
#include <iomanip>
#include <iostream>

uint64_t global_id_counter_s = 100000;

Session::Session()
{
	id_session = global_id_counter_s++;
}

void Session::setSession()
{
	
}

void Session::showInfo()
{
	std::stringstream t;
	while (true)
	{
		system("cls");
		std::cout << "--------StUdY-MAtE--------\n" << std::endl;
		int temp;

		while (true)
		{
			system("cls");
			std::cout << "--------StUdY-MAtE--------\n" << std::endl;
			std::cout << "SID: " << id_session << std::endl;

			std::cout << "\nTopic: " << topic << std::endl;
			std::cout << "Description: " << description << std::endl;
			std::cout << "Location: " << location << std::endl;
			std::cout << "Time: "; t = timeToString(startTime);
			std::cout << t.str();

			std::cout << "\n[0] Quit\n";
			std::cout << "\nEnter:";
			std::cin >> temp;

			if (temp == 0) {
				return;
			}
		}
	}	
}


void Session::reSetInfo()
{
}

void Session::setTime(const std::string& time)
{
	this->startTime = stringToTimeConverstion(time);
}

void Session::setLocation(const std::string& loc)
{
	this->location = loc;
}

void Session::setTopic(const std::string& t)
{
	this->topic = t;
}

const std::string& Session::getTopic() const
{
	return topic;
}

const std::string& Session::getLocation() const
{
	return location;
}

void Session::setDescription(const std::string& desc)
{
	description = desc;
}

const std::string& Session::getDescription() const
{
	return description;
}

void Session::setMode(const Mode md)
{

}

 Mode Session::getMode() const
{
	return mode;
}

 std::chrono::system_clock::time_point& Session::getTime()
 {
	 return startTime;
 }

 std::chrono::system_clock::time_point Session::stringToTimeConverstion(const std::string& s)
 {
	 //time type
	 std::tm time = {};

	 std::stringstream stream(s);

	 stream >> std::get_time(&time, "%Y-%m-%d %H:%M");

	 if (stream.fail()) {
		 return std::chrono::system_clock::now();
	 }

	 //time in seconds 
	 time_t r_time = std::mktime(&time);

	 return std::chrono::system_clock::from_time_t(r_time);
 }

 std::stringstream Session::timeToString(std::chrono::system_clock::time_point& t)
 {
	 std::time_t time = std::chrono::system_clock::to_time_t(t);

	 std::tm* tm_ptr = std::localtime(&time);

	 std::stringstream ss;
	 if (tm_ptr != nullptr) {
		 ss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
	 }
	 return ss;
 }

 void Session::setParentId(uint64_t id)
 {
	 this->id_parent_group = id;
 }

 uint64_t Session::getParentId()
 {
	 return id_parent_group;
 }

 void Session::setId(const uint64_t& n)
 {
	 this->id_session = n;
 }

 uint64_t Session::getId()
 {
	 return this->id_session;
 }
