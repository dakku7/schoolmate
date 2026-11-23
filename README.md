# StudyMate

A console application written in C++17 and SQLite3 for organizing study groups, matching students by shared interests, and tracking weekly study sessions.

## Features

* **User Accounts & Profiles:** Registration, login, profile overview, and customizable interest tags across 22 study categories.
* **Interest-Based Group Matching:** Recommends groups the user hasn't joined yet, ranked by the number of overlapping interests.
* **Group Management:** Create groups, join or leave existing ones, manage group admins, and schedule study sessions.
* **Weekly Schedule:** Collects sessions from all joined groups for the current week (Mon–Sun) and sorts them chronologically.
* **SQLite3 Persistence:** Local relational database (`studymate.db`) storing users, groups, sessions, and many-to-many memberships with foreign key constraints and cascade deletion.

## Tech Stack & Implementation

* **Language & Database:** C++17, SQLite3 (C API with parameterized queries via `sqlite3_prepare_v2` / `sqlite3_bind_*` and RAII connection cleanup).
* **Memory Management:** Smart pointers (`std::unique_ptr` for core services, `std::shared_ptr` and `std::weak_ptr` for user–group relationships to prevent circular references).
* **Standard Library:** STL containers and algorithms (`std::array`, `std::map`, `std::sort`, erase-remove idiom), plus `std::chrono` and `<iomanip>` for date/time parsing and formatting.
* **Architecture:** Modular separation between application core (`AppCore`), console interface (`UI`), persistence layer (`Database`), recommendation engine (`MatchEng`), and domain models (`User`, `Group`, `Session`, `Calendar`).

## Database Schema

* `Users` (`id_user`, `email`, `password`, `login`, `interests`)
* `Groups` (`id_group`, `topic`, `description`, `interests`)
* `UserGroup` (`id_user`, `id_group` — many-to-many junction table with `ON DELETE CASCADE`)
* `Sessions` (`id_session`, `id_group`, `topic`, `description`, `location`, `mode`, `startTime`)

## Build & Run (Windows / Visual Studio)

1. Clone the repository:
   ```bash
   git clone https://github.com/dakku7/studymate.git
   ```
2. Open the `.sln` or `.vcxproj` file in Visual Studio.
3. Make sure the project is configured to use **C++17** (`/std:c++17`) and that SQLite3 (`sqlite3.h` and `sqlite3.c` or prebuilt library paths) is linked.
4. Build and run under the **x64** configuration — `studymate.db` and its tables will be created automatically on first launch.
