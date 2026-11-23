#include "user.h"
#include "groups.h"
#include <algorithm>

uint64_t global_id_counter = 100000;

User::User()
{
	id = global_id_counter++;
	interests.fill(false);
}

bool User::login(const std::string& login, const std::string& password, Database* db)
{
	
	return false;
}


bool User::registerAccount(const std::string& login, const std::string& password,
	std::array<bool, static_cast<size_t>(Intersets::COUNT)>& interests)
{
	setLogin(login);
	setPassword(password);
	this->interests = interests;
	return true;
}


std::string User::getEmail() const
{
	return email;
}

uint64_t User::getId() const
{
	return id;
}


void User::changePassword(const std::string& oldPass, const std::string& newPass)
{

}


bool User::getCheckPassword(const std::string& pswd)
{
	if (pswd == password) {
		return true;
	}
	else return false;
}

void User::showProfile()
{
	int temp;

	auto intr_print = [this]() {
		const auto& ints = getInterests();

		std::cout << "{ ";
		int skip = 0;

		for (size_t i = 0; i < ints.size(); i++)
		{
			if (ints[i] == true) {
				std::cout << toString((Intersets)i) << ", ";
				skip++;
			}

			if (skip == 3) {
				std::cout << "\n  ";
				skip = 0;
			}
		}

		std::cout << " }\n";
		};

	system("cls");
	while (true) {
		std::cout << "--------StUdY-MAtE--------" << std::endl << std::endl
			<< "Login: " << this->getLogin() << std::endl
			<< "Email: " << this->getEmail() << std::endl
			<< "UID: " << this->getId() << std::endl
			<< "Group number: " << this->getGroupNumber() << std::endl
			<< "Interests: \n"; intr_print();

		std::cout << "\n[0] Quit\n";
		std::cout << "\n\nEnter: ";  std::cin >> temp;

		if (temp == 0) {
			return;
		}
	}
}

int User::getGroupNumber()
{
	return groups.size();
}

void User::listAllGroups(int& place)
{
	for (int i = 0; i < groups.size(); i++)
	{
		std::cout << "   N-" << i+1  << "     " << groups[i]->getTopic() << "     ";
	
		if (i == place) {
			std::cout << "<--";
		}
		std::cout << std::endl;
	}
}

std::array<bool, size_t(Intersets::COUNT)>& User::getInterests() 
{
	return interests;
}

void User::addInterest(Intersets i)
{
	interests[(size_t)i] = true;
}

void User::removeInterest(Intersets i)
{
	interests[(size_t)i] = false;
}
void User::setLogin(const std::string& login)
{
	this->login_user = login;
}

void User::setId(uint64_t id)
{
	this->id = id;
}

void User::setPassword(const std::string& pswd)
{
	this->password = pswd;

}

void User::setEmail(const std::string& email)
{
	this->email = email;
}

void User::setInterestsFromDB(const std::string& interestsStr)
{
	this->interests.fill(false);

	for (int i = 0; i < interests.size() && i < interestsStr.size(); ++i)
	{
		if (interestsStr[i] == '1') {
			this->interests[i] = true;
		}
	}
}

void User::setInteresetsManually(std::array<bool, static_cast<size_t>(Intersets::COUNT)>& it)
{
	this->interests = it;
}

std::string User::getPassword()
{
	return this->password;
}

void User::setUID()
{
	id = global_id_counter;
}

void User::joinGroup(const std::shared_ptr<Group>& group)
{
	if (!group) return;

	if (std::find(groups.begin(), groups.end(), group) == groups.end()) {
		groups.push_back(group);
	}
}

void User::leaveGroup(const uint64_t& GID)
{
	groups.erase(
		std::remove_if(groups.begin(), groups.end(), [GID](const std::shared_ptr<Group>& grp) {
			return grp->getGID() == GID;
			}),
		//iterator in the end, so we wont delete anything
		groups.end()
	);
}


const std::vector<std::shared_ptr<Group>>& User::getUserGroups() const
{
	return groups;

}

std::string User::getLogin() const
{
	return login_user;
}

