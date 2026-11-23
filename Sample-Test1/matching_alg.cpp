#include "matching_alg.h"
#include <algorithm>


float MatchEng::scoreCompatibility(std::shared_ptr<User>& u, std::shared_ptr<Group>& group)
{
    int score = 0;

    for (int i = 0; i < int(Intersets::COUNT); i++) {
        if (u->getInterests()[i] && group->getInterests()[i]) {
            score++;
        }
    }
    return score;
}

MatchEng::MatchEng()
{
}

std::vector<std::shared_ptr<Group>> MatchEng::findMatches(std::shared_ptr<User>& u, std::vector<std::shared_ptr<Group>>& grp, bool onlinePrefer)
{
    std::vector<std::pair<float, std::shared_ptr<Group>>> scored_arr;
    
    for (auto& g : grp)
    {
        if (std::find(g->getMembers().begin(), g->getMembers().end(), u) != g->getMembers().end()) {
            continue;
        }
        else {
            float score = scoreCompatibility(u, g);

            scored_arr.push_back({ score, g });
        }
    }

    auto sort_fnc = [](auto& a, auto& b) {
        return a.first > b.first; };

    std::sort(scored_arr.begin(), scored_arr.end(), sort_fnc);
       
    std::vector<std::shared_ptr<Group>> result;
    for (auto& p : scored_arr)
        result.push_back(p.second);

    return result; 
}

std::vector<std::shared_ptr<User>> MatchEng::findUsers(User& user)
{
    return std::vector<std::shared_ptr<User>>();
}

std::vector<std::shared_ptr<Group>> MatchEng::searchGroups(const std::string& keyword)
{
    return std::vector<std::shared_ptr<Group>>();
}

void MatchEng::showRecomendedGroups(std::shared_ptr<User>& u, std::vector<std::shared_ptr<Group>>& grp_)
{
    std::vector<std::shared_ptr<Group>> grp = findMatches(u, grp_, true);
    int temp, place = 0;

    while (true)
    {
        system("cls");
        std::cout << "--------StUdY-MAtE--------" << std::endl;
     
        if (grp.size() == 0) {
            std::cout << "\nNo new groups for you...\n\n\n";
            system("pause");
            return;
        }
        
        for (int i = 0; i < grp.size(); ++i)
        {

            

            std::cout << "   [" << i + 1 << "] " << "    " << grp[i]->getTopic();

            if (i == place) {
                std::cout << "         <--";
            }

            std::cout << std::endl;
            std::cout << "      {" << stringifyInterests3(grp[i]->getInterests()); std::cout << " }\n\n";

            if (i > 0 && i % 5 == 0 || i == grp.size()-1) {
                std::cout << "[1] - down,  [2] - up,  [3] View group  [4] Join  [5] More  [0] Quit\n";

                std::cout << "Enter: ";  std::cin >> temp;

                switch (temp)
                {
                case 1:    
                    if (place >= grp.size() - 1) {
                        place = grp.size() - 1;
                    }
                    else place++;
                    break;
                case 2:
                    if (place <= 0) {
                        place = 0;
                    }
                    else place--;
                    break;
                case 3:
                    grp[place]->showInfo(u);
                    break;
                case 4:
                    u->joinGroup(grp[place]);
                    grp[place]->addMember(u);    
                    break;
                case 5:
                    system("cls");
                    std::cout << "--------StUdY-MAtE--------" << std::endl;
                    break;
                case 0:
                    return;
                    break;
                default:
                    break;
                }
            }

            
        }
    }
}

std::string MatchEng::stringifyInterests3(const std::array<bool, (size_t)Intersets::COUNT>& arr)
{
    std::string out;
    int count = 0;

    for (int i = 0; i < (int)Intersets::COUNT; i++)
    {
        if (arr[i])
        {
            out += InterestNames[i];
            count++;

            if (count == 3)
                break;

            out += ", ";
        }
    }

    return out.empty() ? "No interests" : out;
}

