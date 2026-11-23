#pragma once
#include <string>
#include <iostream>
#include <memory>


class AppCore;

class UI {
private:
	AppCore* app;

public:
	UI(AppCore* p_app);

	void mainMenu();
	void loginMenu();
	void groupMenu();
	void calendarMenu();
	void interestsMenu();

	~UI();
};