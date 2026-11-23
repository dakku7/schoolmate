#include "pch.h"
#include "../schoolmate/ApplicationCore.h"

TEST(UserGroup, JoinGroup) {
	std::cout << "\n===============UNIT TEST 1(JoinGroup)=================\n";
	auto u = std::make_shared<User>();
	auto g = std::make_shared<Group>();

	g->addMember(u);
	u->joinGroup(g);
  ASSERT_EQ(g->getMembersCount(), 1 && "There must be 1 user in group's vector");
  ASSERT_EQ(u->getGroupNumber(), 1 && "There must be 1 group in user's vector");
}


TEST(UserGroup, LeaveGroup) {
	std::cout << "\n===============UNIT TEST 2(LeaveGroup)=================\n";
	auto u = std::make_shared<User>();
	auto g = std::make_shared<Group>();

	g->addMember(u);
	u->joinGroup(g);

	u->leaveGroup(g->getGID());
	g->removeMember(u->getId());
	EXPECT_EQ(g->getMembersCount(), 0);
}

TEST(UserGroup, LeaveGroupAdmin) {
	
	std::cout << "\n===============UNIT TEST 3(AdminCheck)=================\n\n";
	
	auto u = std::make_shared<User>();
	auto g = std::make_shared<Group>();

	g->addMember(u);
	g->addAdmin(u);
	u->joinGroup(g);
	u->leaveGroup(g->getGID());
	g->removeMember(u->getId());
	

	EXPECT_FALSE(g->isAdmin(u)) << "User should not be admin after leaving the group\n";
}

TEST(Integration_MatchEngine, UserDoesNotSeeJoinedGroup)
{
	std::cout << "\n===============INTEGRATION TEST 1(UserDoesNotSeeJoinedGroup)=================\n\n";


	auto user = std::make_shared<User>();
	auto g1 = std::make_shared<Group>();
	auto g2 = std::make_shared<Group>();

	g1->addInterests(Intersets::Programming);
	g2->addInterests(Intersets::Programming);

	user->addInterest(Intersets::Programming);

	// user already joined g1
	g1->addMember(user);
	user->joinGroup(g1);

	std::vector<std::shared_ptr<Group>> allGroups = { g1, g2 };

	MatchEng matcher;
	auto result = matcher.findMatches(user, allGroups, true);

	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], g2);
}
	

TEST(Integration_MatchEngine, JoinedGroupIsIgnoredEvenWithPerfectMatch)
{

	std::cout << "\n===============INTEGRATION TEST 2(JoinedGroupIsIgnoredEvenWithPerfectMatch)=================\n\n";

	auto user = std::make_shared<User>();
	auto group = std::make_shared<Group>();

	user->addInterest(Intersets::Programming);
	group->addInterests(Intersets::Programming);

	group->addMember(user);
	user->joinGroup(group);

	std::vector<std::shared_ptr<Group>> allGroups = { group };

	MatchEng matcher;
	auto result = matcher.findMatches(user, allGroups, true);

	EXPECT_TRUE(result.empty());
}


TEST(Functional_UserScenario, UserJoinsGroup)
{
	std::cout << "\n===============FUNCTIONAL TEST 1(UserJoinsGroup)=================\n\n";

	auto user = std::make_shared<User>();
	auto group = std::make_shared<Group>();

	group->setGroup("Math Group", "Study math");

	group->addMember(user);
	user->joinGroup(group);

	EXPECT_EQ(user->getGroupNumber(), 1);
	EXPECT_EQ(group->getMembersCount(), 1);
}

TEST(Functional_UserScenario, AdminCreatesSession)
{
	std::cout << "\n===============FUNCTIONAL TEST 2(AdminCreatesSession)=================\n\n";

	auto user = std::make_shared<User>();
	auto group = std::make_shared<Group>();

	group->setGroup("Physics", "Study physics");

	group->addMember(user);
	group->addAdmin(user);

	ASSERT_TRUE(group->isAdmin(user));

	group->makeSession("Lesson", "Gravity", "Room 101", "2026-01-12 12:00", group->getGID());

	EXPECT_EQ(group->getSessions().size(), 1);
}