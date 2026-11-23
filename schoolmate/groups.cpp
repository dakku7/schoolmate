#include "groups.h"
#include <algorithm>
#include <iostream>
#include "user.h"

uint64_t global_id_counter_grp = 100000;



Group::Group()
{
	id_group = global_id_counter_grp++;
    grp_interests.fill(false);
}

void Group::setId(uint64_t id) {
	this->id_group = id;
}

void Group::setInterestsFromDB(const std::string& interestsStr) {
	// 1. Сбрасываем (заполняем false)
	this->grp_interests.fill(false);

	// 2. Читаем строку
	for (size_t i = 0; i < interestsStr.size() && i < this->grp_interests.size(); ++i) {
		if (interestsStr[i] == '1') {
			this->grp_interests[i] = true;
		}
	}
}

void Group::setTopic(const std::string& topic) {
	this->topic = topic;
}


void Group::setDescription(const std::string& desc)
{
	this->description = desc;
}

void Group::setInterests(std::array<bool, static_cast<size_t>(Intersets::COUNT)>& intr)
{
	for (int i = 0; i < intr.size() && i < grp_interests.size(); i++)
	{
		grp_interests[i] = intr[i];
	}
}

void Group::showInfo(std::shared_ptr<User>& u) {
	
	uint64_t t_id;
	system("cls");
	while (true) {

		int temp;
		std::cout << "--------StUdY-MAtE--------" << std::endl << std::endl
			<< "Topic:" << topic << std::endl
			<< "Description: " << description << std::endl
			<< "GID: " << id_group << std::endl
			<< "Number of members: " << members.size() << std::endl;

		std::cout << "\nSessions:\n";
		for (const auto& s : sessions) {
			std::cout << " - " << s->getTopic() << " (" << s->getLocation() << ")\n";
		}

		if (isAdmin(u)) {
			std::cout << "[1] Add new admin\n[2] Remove admin\n";
		}

		std::cout << "\n [0] Quit\n";
		std::cout << "\n\nEnter: ";  std::cin >> temp;

		if (temp == 0) {
			return;
		}

		if (isAdmin(u)) {
			switch (temp)
			{
			case 1:
				std::cout << "Enter UID to promote admin: "; std::cin >> t_id;
				addAdmin(findUserByUID(t_id));
				break;
			case 2:
				std::cout << "Enter UID to remove admin: "; std::cin >> t_id;
				deleteAdmin(t_id);
				break; 

			default:
				break;
			}
		}
		switch (temp)
		{
		default:
			return;
			break;
		}

	}
}


void Group::showInfoJoin(std::shared_ptr<User>& u, std::vector<std::shared_ptr<Group>>& grp_, int& place)
{
	int temp;

	system("cls");
	while (true) {
		std::cout << "--------StUdY-MAtE--------" << std::endl << std::endl
			<< "Topic:" << topic << std::endl
			<< "Description: " << description << std::endl
			<< "GID: " << id_group << std::endl
			<< "Number of members: " << members.size() << std::endl;


		std::cout << "\n[1] Join   [0] Quit\n";
		std::cout << "\n\nEnter: ";  std::cin >> temp;

		if (temp == 0) {
			return;
		}

		if (temp == 1) {
			addMember(u);
			u->joinGroup(grp_[place]);
			system("cls");
			std::cout << "You've succecfully joined group! Chec your group in *My groups* section.\n";

		}

	}
}

void Group::addMember(const std::shared_ptr<User>& user)
{
	if (!user || hasMember(user->getId())) return;
	members.push_back(user);
}

void Group::removeMember(uint64_t uid)
{
	members.erase(
		std::remove_if(members.begin(), members.end(), [uid](const std::weak_ptr<User>& wp) {
			auto sp = wp.lock();
			return !sp || sp->getId() == uid;
			}),
		members.end()
	);
}

std::shared_ptr<User> Group::findUserByUID(uint64_t uid)
{
	for (const auto& wp : members) {
		auto sp = wp.lock();
		if (sp && sp->getId() == uid) {
			return sp;
		}
	}
	return nullptr;
}

void Group::addAdmin(const std::shared_ptr<User>& user)
{
	if (!user) return;

	for (auto it : admins_id)
	{
		if (it == user->getId()) return;
	}
	admins_id.push_back(user->getId());
}

bool Group::isAdmin(const std::shared_ptr<User>& user)
{
	if (!user) return false;

	for (auto it : admins_id)
	{
		if (it == user->getId()) return true;
	}
	return false;
}

void Group::deleteAdmin(uint64_t uid)
{
	admins_id.erase(
		std::remove(admins_id.begin(), admins_id.end(), uid),
		admins_id.end()
	);
}

void Group::makeSession()
{
	auto s = std::make_shared<Session>();
	s->setSession();
	s->setParentId(id_group);
	addSession(s);
}

void Group::makeSession(std::string topic, std::string desc, std::string location, std::string time, uint64_t gid)
{
	auto s = std::make_shared<Session>();
	s->setTopic(topic);
	s->setDescription(desc);
	s->setLocation(location);
	s->setTime(time);
	s->setParentId(gid);
	addSession(s);

}

void Group::addSession(const std::shared_ptr<Session>& s)
{
	if (!s) return;
	if (std::find(sessions.begin(), sessions.end(), s) == sessions.end()) {
		sessions.push_back(s);
	}
}

const std::vector<std::weak_ptr<User>>& Group::getMembers() const
{
	return members;
}

std::vector<std::shared_ptr<Session>>& Group::getSessions() 
{
	return sessions;
}

std::string Group::getTopic()
{
	return topic;
}

std::string Group::getDescription()
{
	return description;
}


void Group::setGroup(std::string topic, std::string desc)
{
	this->topic = topic;
	this->description = desc;
}

std::array<bool, static_cast<size_t>(Intersets::COUNT)>& Group::getInterests()
{
	return grp_interests;
}

void Group::addInterests(Intersets intr)
{
	grp_interests[(size_t)intr] = true;
}

void Group::deleteInterests(Intersets intr)
{
	grp_interests[(size_t)intr] = false;
}

void Group::getInfo()
{

}

uint64_t Group::getGID()
{
	return id_group;
}

int Group::getMembersCount()
{
	return static_cast<int>(members.size());

}

Group& Group::getGroupByGID(const uint64_t& GID)
{
	if (GID == id_group) {
		return *this;
	} 

}

bool Group::hasMember(uint64_t uid) const
{
	for (const auto& wp : members) {
		auto sp = wp.lock();
		if (sp && sp->getId() == uid) {
			return true;
		}
	}
	return false;
}