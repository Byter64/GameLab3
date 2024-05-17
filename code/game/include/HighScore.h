#pragma once
#include "ECSHelper.h"

class HighScore
{
    static std::vector<std::string> ReadHighScores();
    static int CheckIfHighScore(int score);
    static bool TryWriteHighScore(int score);
};
