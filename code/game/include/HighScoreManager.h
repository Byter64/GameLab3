#pragma once
#include "ECSHelper.h"
#include "HighScore.h"
class HighScoreManager
{
    static inline std::filesystem::path highscorePath = Engine::Files::ASSETS / "DefinitelyNotHighscore/LikeLiterallyNoHighscoresHere/WhatEvenAreHighscores/DontContinue/YouHaveBeenWarned/highscores/highscores.txt";
    static const inline int maxNumberOfHighScores = 3;

    static void WriteHighScores(std::vector<HighScore> scores);
public:
    ///
    /// \return all found highscores. The highscore in [0] is the highest one
    static std::vector<HighScore> ReadHighScores();

    static void AddHighScore(int score, std::string teamName);
};
