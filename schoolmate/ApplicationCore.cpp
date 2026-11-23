#include "ApplicationCore.h"
#include <iomanip>

AppCore::AppCore()
{
	/*auto newGroup = std::make_shared<Group>();

	newGroup->setGroup("WoW team", "wow playing team");
	newGroup->addInterests(Intersets::Programming);
	newGroup->addInterests(Intersets::Chemistry);
	newGroup->addInterests(Intersets::Biology);
	allGroups.push_back(newGroup);

	newGroup = std::make_shared<Group>();
	newGroup->setGroup("Ivan1234", "test");
	newGroup->addInterests(Intersets::Programming);
	newGroup->addInterests(Intersets::Beginner);
	newGroup->addInterests(Intersets::Languages);
	allGroups.push_back(newGroup);
	*/
	calendar = std::make_unique<Calendar>();
	database = std::make_unique<Database>();
	matcher = std::make_unique<MatchEng>();

	loadDatabase();
	
}

void AppCore::run()
{
	UI userInterface(this);

	setRunning(true);

	while (isRunning)
	{
		system("cls");
		if (!isLoggedIn) {
			userInterface.loginMenu();
			continue;
		}
		
		userInterface.mainMenu();
	}

	if (!isRunning) {
		//saveInDatabase();
	}
}

void AppCore::loadDatabase()
{
//	allGroups[1]->makeSession("Testing", "Im testing loadup", "Nevermind", "2025-12-16 12:00", allGroups[1]->getGID());
	
	if (!database->open("studymate.db")) {

		std::cout << "Error opening database!\n";
		return;
	}

	database->loadAllData(allUsers, allGroups);
//id sync with users	
	uint64_t maxId = 0;
	for (const auto& user : allUsers) {
		if (user->getId() > maxId) {
			maxId = user->getId();
		}
	}
	global_id_counter = maxId + 1;

//id sync with grps
	uint64_t maxGrpId = 0;
	for (const auto& grp : allGroups) {
		if (grp->getGID() > maxGrpId) { 
			maxGrpId = grp->getGID();
		}
	}
	global_id_counter_grp = maxGrpId + 1;

//id sync sessions
	uint64_t maxSessionId = 0;
	for (const auto& grp : allGroups) {
		for (const auto& session : grp->getSessions()) {
			if(session->getId() > maxSessionId){
				maxSessionId = session->getId();
			}
		}
	}
	global_id_counter_s = maxSessionId + 1;
}

void AppCore::saveInDatabase()
{
	saveGroups();
	saveUsers();
}

void AppCore::saveGroups()
{
	for (auto& it : allGroups)
	{
		for (auto& ij : it->getSessions())
		{
			database->saveSession(ij);
		}
		database->saveGroup(it);
	}
}

void AppCore::saveUsers()
{
	for (auto& it : allUsers)
	{
		database->saveUser(it);
	}
}


std::shared_ptr<User> AppCore::findUser(const std::string& login, const std::string& password)
{
	for (auto& u : allUsers)
	{
		if (u->getLogin() == login && u->getCheckPassword(password))
		{
			return u; 
		}
	}
	return nullptr;
}

std::shared_ptr<Group> AppCore::findGroup(const std::shared_ptr<Group> allgrp)
{
	return std::shared_ptr<Group>();
}

void AppCore::leavingProcces(int& p_)
{
	//firstly we take group
	auto grp_tmp = currentUser->getUserGroups().at(p_);

	//delete user from taken group
	grp_tmp->removeMember(currentUser->getId());
	if (grp_tmp->isAdmin(currentUser)) {
		grp_tmp->deleteAdmin(currentUser->getId());
	}

	//delete in users group list association
	currentUser->leaveGroup(grp_tmp->getGID());

	if (database) {
		database->deleteUserGroupConnection(currentUser->getId(), grp_tmp->getGID());
	}
}

bool AppCore::isAppRunning()
{
	return isRunning;
}

void AppCore::setRunning(bool st)
{
	this->isRunning = st;
}

std::shared_ptr<User> AppCore::getCurrentUser()
{
	return currentUser;
}

std::vector<std::shared_ptr<Group>> AppCore::getAllGroups()
{
	return allGroups;
}

std::vector<std::shared_ptr<User>> AppCore::getAllUsers()
{
	return allUsers;
}

std::unique_ptr<Calendar>& AppCore::getCalendar()
{
	return calendar;
}

std::unique_ptr<MatchEng>& AppCore::getMatcher()
{
	return matcher;
}

void AppCore::createGroup() 
{
	std::string topic, desc;
	std::cout << "Enter Topic: ";
	std::cin.ignore();
	std::getline(std::cin, topic);

	std::cout << "Enter Description: ";
	std::getline(std::cin, desc);

	auto newGroup = std::make_shared<Group>();
	newGroup->setId(global_id_counter_grp++);

	newGroup->setGroup(topic, desc);

	newGroup->addMember(currentUser);
	newGroup->addAdmin(currentUser);
	currentUser->joinGroup(newGroup);

	editInterests(newGroup->getInterests());

	allGroups.push_back(newGroup);

	if (database) {
		database->saveGroup(newGroup);
		database->saveUserGroupConnection(currentUser->getId(), newGroup->getGID());
	}

	std::cout << "Group created successfully!\n";
}

void AppCore::createSession(uint64_t groupId)
{
	std::shared_ptr<Group> currentGroup = nullptr;
	for (auto& g : allGroups) {
		if (g->getGID() == groupId) {
			currentGroup = g;
			break;
		}
	}
	if (!currentGroup) return;

	int temp; std::string t_string;
	std::string t_startTime;

	auto s = std::make_shared<Session>();
	s->setParentId(currentGroup->getGID());
	std::cin.ignore();
	std::cout << "Enter Topic: ";
	std::getline(std::cin, t_string);
	s->setTopic(t_string);

	std::cout << "Enter Description: ";
	std::getline(std::cin, t_string);
	s->setDescription(t_string);

	std::cout << "Location: ";
	std::getline(std::cin, t_string);
	s->setLocation(t_string);

	std::cout << "Time: (YYYY-MM-DD HH:MM)";
	std::getline(std::cin, t_startTime);
	s->setTime(t_startTime);

	s->showInfo();

	currentGroup->addSession(s);

	if (database) {
		database->saveSession(s);
	}
}

void AppCore::editInterests(std::array<bool, static_cast<size_t>(Intersets::COUNT)>& currentInterests)
{
	int choice = -1;
	size_t count = static_cast<size_t>(Intersets::COUNT);

	while (true) {
		system("cls"); 
		std::cout << "-------- SELECT INTERESTS --------\n";
		std::cout << "Enter the ID to toggle selection.\n";
		std::cout << "Enter [0] to Save and Finish.\n\n";

		for (size_t i = 0; i < count; ++i) {

			bool isSelected = currentInterests[i];

			// [1] [X] Physics
			// [2] [ ] Math
			std::cout << "[" << std::setw(2) << i + 1 << "] "
				<< (isSelected ? "[X]" : "[ ]") << " "
				<< InterestNames[i] << std::endl;
		}

		std::cout << "\nChoice: ";
		std::cin >> choice;

		if (choice == 0) {
			break;
		}

		if (choice > 0 && choice <= static_cast<int>(count)) {
			int index = choice - 1;
			currentInterests[index] = !currentInterests[index];
		}
		else {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
		}
	}
}