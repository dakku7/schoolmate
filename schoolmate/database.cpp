#include "database.h"
#include "groups.h"

    
std::string Database::interestsToString(const std::array<bool, 22>& interests)
{
    std::string res = "";
    for (bool val : interests) res += (val ? '1' : '0');
    return res;
}

bool Database::execQuery(const std::string& path)
{
    char* errMsg = nullptr;
    if (sqlite3_exec(db, path.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error exec: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

Database::Database()
    :db(nullptr)
{
}
// database initialization part
void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}



bool Database::open(const std::string& path)
{
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        return false;
    }
    //for multiple tables
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    std::string sql =
        "CREATE TABLE IF NOT EXISTS Users ("
        "   id_user INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   email TEXT,"
        "   password TEXT,"
        "   login TEXT NOT NULL UNIQUE,"
        "   interests TEXT);"

        "CREATE TABLE IF NOT EXISTS Groups ("
        "   id_group INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   topic TEXT,"
        "   description TEXT,"
        "   interests TEXT);"

        "CREATE TABLE IF NOT EXISTS UserGroup ("
        "   id_user INTEGER,"
        "   id_group INTEGER,"
        "   PRIMARY KEY (id_user, id_group)," 
        "   FOREIGN KEY (id_user) REFERENCES Users(id_user) ON DELETE CASCADE,"
        "   FOREIGN KEY (id_group) REFERENCES Groups(id_group) ON DELETE CASCADE);"

        "CREATE TABLE IF NOT EXISTS Sessions ("
        "   id_session INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   id_group INTEGER,"
        "   topic TEXT,"
        "   description TEXT,"
        "   location TEXT,"
        "   mode INTEGER,"
        "   startTime INTEGER,"
        "   FOREIGN KEY(id_group) REFERENCES Groups(id_group) ON DELETE CASCADE);"
        ;

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Table creation problem " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

//users part loadup-callback
//argc - number of columns
//argv - line in specific column??
// colname - column which we are reffering to
static int loadUserCallback(void* data, int argc, char** argv, char** colName) {
    auto list = static_cast<std::vector<std::shared_ptr<User>>*>(data);
    auto u = std::make_shared<User>();

    // argv[0] -> ID
    // argv[1] -> Email
    // argv[2] -> Password
    // argv[3] -> Login
    // argv[4] -> Interests ("10100...")

    if (argv[0]) {
        u->setId(std::stoll(argv[0]));
    }


    u->setEmail(argv[1] ? argv[1] : "NULL");
    u->setPassword(argv[2] ? argv[2] : "NULL");
    u->setLogin(argv[3] ? argv[3] : "NULL");

    if (argv[4]) {
        u->setInterestsFromDB(argv[4]);
    }

    list->push_back(u);

    return 0;
}

static int loadGroupCallback(void* data, int argc, char** argv, char** colName) {
    auto list = static_cast<std::vector<std::shared_ptr<Group>>*>(data);
    auto g = std::make_shared<Group>();

    // argv[0] -> id_group
    // argv[1] -> topic
    // argv[2] -> description
    // argv[3] -> interests ("1010...")


    if (argv[0]) {
        g->setId(std::stoll(argv[0]));
    }


    std::string t = argv[1] ? argv[1] : "No Topic";
    std::string d = argv[2] ? argv[2] : "No Desc";
    g->setGroup(t, d);

    if (argv[3]) {
        g->setInterestsFromDB(argv[3]);
    }

    list->push_back(g);

    return 0;
}

struct UserGroupConnection {
    std::vector<std::shared_ptr<User>>* users;
    std::vector<std::shared_ptr<Group>>* groups;
};

static int loadConnectionUserGroup(void* data, int argc, char** argv, char** colName) {
    auto list_con = static_cast<UserGroupConnection*>(data);


    //if in those columns is nullptr
    if (!argv[0] || !argv[1]) {
        return 0;
    }
    // argv[0] = id_user
    // argv[1] = id_group

    uint64_t uid = std::stoll(argv[0]);
    uint64_t gid = std::stoll(argv[1]);
    std::shared_ptr<User> foundUser = nullptr;
    std::shared_ptr<Group> foundGroup = nullptr;

    for (const auto& var : *list_con->users)
    {
        if (var->getId() == uid) {
            foundUser = var;
            break;
        }
    }

    for (const auto& var : *list_con->groups)
    {
        if (var->getGID() == gid) {
            foundGroup = var;
            break;
        }
    }

    if (foundGroup && foundUser) {
        foundUser->joinGroup(foundGroup);
        foundGroup->addMember(foundUser);
    }

    return 0;

}

struct SessionGroupConnect {
    std::vector<std::shared_ptr<Group>>* groups;
};

static int loadSessionCallBack(void* data, int argc, char** argv, char** colName) {
    auto list_conection = static_cast<SessionGroupConnect*>(data);

    // argv[0] = id_session
    // argv[1] = id_group 
    // argv[2] = topic
    // argv[3] = description
    // argv[4] = location
    // argv[5] = mode 0/1
    // argv[6] = start_time 

    std::string sql = "INSERT INTO Sessions (id_session, id_group, topic, description, location, mode, startTime) VALUES (";
        

    if (!argv[1]) return 0; //group check

    uint64_t gid = std::stoll(argv[1]);

    std::shared_ptr<Group> pgroup = nullptr;

    for (const auto& var : * list_conection->groups)
    {
        if (var->getGID() == gid) {
            pgroup = var;
            break;
        }
    }

    if (pgroup) {
        auto s = std::make_shared<Session>();
        if (argv[0]) {
            s->setId(std::stoll(argv[0]));
        }

        s->setTopic(argv[2] ? argv[2] : "");
        s->setDescription(argv[3] ? argv[3] : "");
        s->setLocation(argv[4] ? argv[4] : "");
        s->setMode(Mode::OFFLINE);
        if (argv[6]) {
            s->setTime(argv[6]);
        }

        pgroup->addSession(s);
    }

    return 0;
}
//main loader
void Database::loadAllData(std::vector<std::shared_ptr<User>>& allusers, std::vector<std::shared_ptr<Group>>& allgroups)
{
    char* errMsg = nullptr;
    // users
    sqlite3_exec(db, "SELECT * FROM Users;", loadUserCallback, &allusers, &errMsg);
   
    // groups
    sqlite3_exec(db, "SELECT * FROM Groups;", loadGroupCallback, &allgroups, nullptr);

    // connections
    UserGroupConnection ctx = { &allusers, &allgroups };
    sqlite3_exec(db, "SELECT * FROM UserGroup;", loadConnectionUserGroup, &ctx, nullptr);

    // Session
    SessionGroupConnect sctx = { &allgroups };
    sqlite3_exec(db, "SELECT * FROM Sessions;", loadSessionCallBack, &sctx, nullptr);

    // else old code
    /*auto u1 = std::make_shared<User>();
    u1->registerAccount("test1", "1111");

    allusers.push_back(u1);

    u1 = std::make_shared<User>();
    u1->registerAccount("dakku", "vany6744");

    allusers.push_back(u1);*/

}

//save section
void Database::saveUser(std::shared_ptr<User>& user)
{
    const char* sql =
        "INSERT INTO Users (id_user, email, password, login, interests) "
        "VALUES (?, ?, ?, ?, ?) "
        "ON CONFLICT(id_user) DO UPDATE SET "
        "email = excluded.email, password = excluded.password, "
        "login = excluded.login, interests = excluded.interests;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string intr = interestsToString(user->getInterests());
    sqlite3_bind_int64(stmt, 1, static_cast<sqlite3_int64>(user->getId()));
    sqlite3_bind_text(stmt, 2, user->getEmail().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user->getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user->getLogin().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, intr.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to save user: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}

void Database::saveGroup(std::shared_ptr<Group>& g)
{
    std::string sql = "INSERT INTO Groups (id_group, topic, description, interests) VALUES (" +
        std::to_string(g->getGID()) + ", '" + g->getTopic() + "', '" +
        g->getDescription() + "', '" + interestsToString(g->getInterests()) + "');";

    if (!execQuery(sql)) {
        std::cerr << "Failed to save group: " << g->getGID() << std::endl;
    }
}

void Database::saveUserGroupConnection(uint64_t userId, uint64_t groupId)
{
    std::string sql = "INSERT OR IGNORE INTO UserGroup (id_user, id_group) VALUES (" +
        std::to_string(userId) + ", " +
        std::to_string(groupId) + ");";

    execQuery(sql);
}

void Database::deleteUserGroupConnection(uint64_t userId, uint64_t groupId)
{
    std::string sql = "DELETE FROM UserGroup WHERE id_user=" + std::to_string(userId) +
        " AND id_group=" + std::to_string(groupId) + ";";
    execQuery(sql);
}

void Database::saveSession(std::shared_ptr<Session>& s)
{
    auto time = s->timeToString(s->getTime()); 
   
    std::string sql = "INSERT INTO Sessions (id_session, id_group, topic, description, location, mode, startTime) VALUES (" +
        std::to_string(s->getId()) + ", '" + std::to_string(s->getParentId()) + "', '" +
        s->getTopic() + "', '" + s->getDescription() + "', '" +
        s->getLocation() + "', '" + std::to_string(0) + "', '" + time.str() + "');";

    if (!execQuery(sql)) {
        std::cerr << "Failed to save session: " << s->getId() << std::endl;
    }
}



Database::~Database()
{
    close();
}
