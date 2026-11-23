#pragma once
#include <string>

enum class Intersets {
    Physics,
    Math,
    Chemistry,
    ComputerScience,
    Programming,
    Languages,
    SocialSciences,
    Statistics,
    Law,
    Engineering,
    Economics,
    Biology,
    Finance,
    TeamProjects,
    DataAnalysis,
    Presentations,
    LateNightStudy,
    LiteratureReview,
    Beginner,
    TheoryBuilding,
    PracticalApplication,
    OpenSourceProject,
    COUNT
};

static const char* InterestNames[] = {
    "Physics",
    "Math",
    "Chemistry",
    "ComputerScience",
    "Programming",
    "Languages",
    "SocialSciences",
    "Statistics",
    "Law",
    "Engineering",
    "Economics",
    "Biology",
    "Finance",
    "TeamProjects",
    "DataAnalysis",
    "Presentations",
    "LateNightStudy",
    "LiteratureReview",
    "Beginner",
    "TheoryBuilding",
    "PracticalApplication",
    "OpenSourceProject"
};


inline const char* toString(Intersets interest) {
    return InterestNames[(int)interest];
}