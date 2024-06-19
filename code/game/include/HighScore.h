#pragma once
#include <string>

struct HighScore
{
    int score;
    std::string teamName;

    bool operator<(HighScore const& other);
};