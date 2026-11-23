#define _CRT_SECURE_NO_WARNINGS
#include "calendar.h"
#include "user.h"
#include "groups.h"
#include <time.h>
#include <algorithm>
#include <sstream>

Calendar::Calendar()
{
}

void Calendar::showCalendarWeek(std::map<int, std::vector<Session>> sessions)
{
	std::stringstream temp; 
	int choice, arrowControl = 0;
	
	std::string week[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturaday", "Sunday" };
	while (true)
	{
		system("cls");
		std::cout << "--------StUdY-MAtE--------" << std::endl;
		std::cout << "\n        Schedule        \n\n";

		std::cout << "Day:" << "        Session:             " << "Time: " << std::endl;
		for (int i = 0; i < sessions.size(); i++)
		{

			if (sessions[i].size() >= 1) {
				temp = sessions[i].at(0).timeToString(sessions[i].at(0).getTime());
				std::cout << week[i] << " -  " << sessions[i].at(0).getTopic(); if (sessions[i].size() > 1) std::cout << "(+" << sessions[i].size() << ")"; std::cout << "		" << temp.str();
			}
			else {
				std::cout << week[i] << " -  " << "					";

			}
			if (i == arrowControl) {
				std::cout << " <--\n";
			} else std::cout << std::endl;
		}
		std::cout << "[1] Up, [2] Down, [3] View, [0] Quit" << std::endl;

		std::cout << std::endl; 
		std::cout << "Enter: "; std::cin >> choice;

		if (choice == 0) {
			return;
		}

		switch (choice)
		{
		case 1:
			if (arrowControl <= 0) {
				arrowControl = 0;
			}
			else arrowControl--;
			break;
		case 2:
			if (arrowControl >= 6) {
				arrowControl = 6;
			}
			else arrowControl++;
			break;
		case 3:
			showDaySession(arrowControl, sessions);
			break;
		default:
			break;
		}
	}
	
}

std::map<int, std::vector<Session>> Calendar::getCurrentWeekSessions(std::shared_ptr<User> u)
{
	// Инициализируем пустые векторы для всех дней недели (0-6)
	std::map<int, std::vector<Session>> weeklySessions;
	for (int i = 0; i < 7; i++)
	{
		weeklySessions[i] = {};
	}
	//end and start of the week
	std::time_t s = getStartOfTheWeek();
	std::time_t e = getEndtOfTheWeek();

	for (auto& it : u->getUserGroups())
	{
		for (auto& ij : it->getSessions())
		{
			std::time_t now = std::chrono::system_clock::to_time_t(ij->getTime());
			if (now >= s && now <= e) {
				int day = getDayOfTheWeek(now);
				weeklySessions[day].push_back(*ij);
			}	
		}
	}

	for (int i = 0; i < weeklySessions.size(); i++)
	{
		std::sort(weeklySessions[i].begin(), weeklySessions[i].end(), [](Session& a, Session& b) {
			return a.getTime() < b.getTime();
			});
	}
	// 1. Пробегаем по всем группам пользователя
	// 2. Пробегаем по всем сессиям группы
	// 3. Проверяем, попадает ли сессия в текущую неделю	
	// 4. Сортируем сессии внутри каждого дня по времени

	return weeklySessions;
}

std::time_t Calendar::getStartOfTheWeek()
{
	std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
	std::time_t now = (std::chrono::system_clock::to_time_t(t));
	std::tm* ptr = std::localtime(&now);
	std::tm nowTime = *ptr;

	int currentDay = 0;
	//convertation to europe calendar 0 - monday 6-sunday
	if (nowTime.tm_wday == 0) {
		currentDay = 6;
	}
	else {
		currentDay = nowTime.tm_wday - 1;
	}

	nowTime.tm_hour = 0;
	nowTime.tm_min = 0;
	nowTime.tm_sec = 0;

	std::time_t startOfTheWeek = std::mktime(&nowTime);
	return startOfTheWeek - (currentDay * 24 * 60 * 60);
}

int Calendar::getDayOfTheWeek(std::time_t& t)
{


	std::tm* nowTime = std::localtime(&t);

	int day = 0;
	if (nowTime->tm_wday == 0) {
		day = 6;
	}
	else {
		day = nowTime->tm_wday - 1;
	}

	return day;
}

void Calendar::showDaySession(int arrow, std::map<int, std::vector<Session>>& sessions)
{
	int t;
	while (true)
	{
		system("cls");

		for (int i = 0; i < sessions[arrow].size(); i++)
		{
			std::cout << i+1 << " - ";  std::cout << sessions[arrow].at(i).getTopic() << std::endl;
			std::cout << "   Description: " << sessions[arrow].at(i).getDescription() << std::endl;
			std::cout << "   Time: " << sessions[arrow].at(i).timeToString(sessions[arrow].at(i).getTime()).str() << std::endl;
		}
		std::cout << std::endl;
		std::cout << "\nEnter: "; std::cin >> t;

		if (t == 0) {
			return;
		}
	}
}

std::time_t Calendar::getEndtOfTheWeek()
{
	return getStartOfTheWeek() + (7 * 24 * 60 * 60) - 1;
}

