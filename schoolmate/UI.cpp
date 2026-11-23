#include "UI.h"
#include "ApplicationCore.h"
//sql lite

UI::UI(AppCore* p_app)
{
	app = p_app;
}

void UI::mainMenu()
{
	int temp;

	std::cout << "--------StUdY-MAtE--------" << std::endl
		<< "[1] My profile" << std::endl
		<< "[2] My groups" << std::endl
		<< "[3] Edit your interests" << std::endl
		<< "[4] Schedule" << std::endl
		<< "[5] See recomended groups" << std::endl
	//	<< "[6] Search for groups" << std::endl
		<< "\n[0] Log out(Quit)" << std::endl;

	std::cout << "Enter: "; std::cin >> temp;

	switch (temp)
	{
	case 1:
		app->currentUser->showProfile();
		break;
	case 2:
		groupMenu();
		break;
	case 3:
		interestsMenu();
		break;
	case 4:
		calendarMenu();
		break;
	case 5:
		app->matcher->showRecomendedGroups(app->currentUser, app->allGroups, app->database.get());
		break;
	case 0:
		app->isLoggedIn = false;
		break;
	default:
		break;
	}

	system("cls");
}

void UI::loginMenu()
{
	int temp;

	std::cout << "--------StUdY-MAtE--------" << std::endl
		<< "\nHello student, firstly you should to log in or sign up" << std::endl
		<< "[1] Log in" << std::endl
		<< "[2] Sign up" << std::endl
		<< "\n[0] Quit" << std::endl;

	std::cout << "Enter: "; std::cin >> temp;

	if (temp == 0) {
		app->isRunning = false;
		return;
	}

	std::string t_login, t_password;

	auto printTemp = [&t_login, &t_password]() {
		system("cls");
		std::cout << "--------StUdY-MAtE--------" << std::endl;
		std::cout << "Login: "; std::cin >> t_login;
		std::cout << "Password: "; std::cin >> t_password; std::cout << std::endl;


		while (t_password.size() < 4) {
			std::cout << "The password is too small (needs to be greater than 4 symbols)\n";
			std::cout << "Password: "; std::cin >> t_password;

			if (t_password == "0") {
				return;
			}
		}
		};


	if (temp == 1) {
		printTemp();
		app->currentUser = app->findUser(t_login, t_password);

		if (app->currentUser != nullptr) {
			if (app->currentUser) {
				std::cout << "Login successful!\n";
				app->isLoggedIn = true;
			}
		}
		else {
			std::cout << "User not found or wrong password.\n";
		}
	}
	else if (temp == 2) {
		printTemp();

		if (app->findUser(t_login, t_password) != nullptr) {
			std::cout << "User already exists!\n";
			return;
		}
		auto newUser = std::make_shared<User>();
		newUser->registerAccount(t_login, t_password, newUser->getInterests());
		app->allUsers.push_back(newUser);
		if (app->database) {
			app->database->saveUser(newUser);
		}
		std::cout << "Registration successful! Please log in.\n\n";
	}
	else return;

	system("pause");
	system("cls");
}

void UI::groupMenu()
{

	int place = 0;
	while (true)
	{
		int temp;
		int count = app->currentUser->getGroupNumber();
		system("cls");
		std::cout << "--------StUdY-MAtE--------" << std::endl
			<< "\nGroups count: " << app->currentUser->getGroupNumber() << std::endl;
		app->currentUser->listAllGroups(place);

		std::cout << "\n\n[1] - Up, [2] - Down, [3] - Enter, [4] Leave from this group, [5] Create a new group\n\n [6] Add session at current group\n[0] - quit" << std::endl;
		std::cout << "Enter: "; std::cin >> temp;

		switch (temp)
		{
		case 1:
			if (place <= 0) {
				place = 0;
			}
			else place--;
			break;
		case 2:
			if (count > 0 && place < count - 1) place++;
			break;
		case 3:
			if (count == 0) break;
			app->currentUser->getUserGroups().at(place)->showInfo(app->currentUser);
			break;
		case 4:
			if (count == 0) break;
			app->leavingProcces(place);
			if (place >= app->currentUser->getGroupNumber() && place > 0) place--;
			break;
		case 5:

			app->createGroup();
			break;
		case 6:
			if (count == 0) break;
			app->createSession(app->currentUser->getUserGroups().at(place)->getGID());
			break;
		case 0:
			return;
			break;
		default:
			return;
			break;
		}
	}

}

void UI::calendarMenu()
{
	app->calendar->showCalendarWeek(app->calendar->getCurrentWeekSessions(app->currentUser));
}

void UI::interestsMenu()
{
	auto temp = app->currentUser->getInterests();
	app->editInterests(temp);
	app->currentUser->setInteresetsManually(temp);
	if (app->database) {
		app->database->saveUser(app->currentUser);
	}
}

UI::~UI()
{
}