#include "HighScoreManager.h"
#include <string>
#include <fstream>

std::vector<HighScore> HighScoreManager::ReadHighScores()
{
    std::vector<std::string> file = Engine::Files::ParseFile(highscorePath);

    std::vector<HighScore> scores;

    int i = 0;
    while(i < file.size())
    {
        int score = std::stoi(file[i]);
        i++;
        i++;

        std::string name = "";
        std::string word = file[i];

        while(word != ":")
        {
            name += word + " ";
            i++;
            word = file[i];
        }
        name = name.substr(0, name.size() - 1);
        scores.push_back({score, name});
        i++;
    }

    return scores;
}

void HighScoreManager::AddHighScore(int score, std::string teamName)
{
    if(teamName.empty())
        teamName = "Anonymous Gangsters";
    std::vector<HighScore> scores = ReadHighScores();
    scores.push_back({score, teamName});
    std::sort(scores.begin(), scores.end(), [](HighScore const & h1, HighScore const & h2) -> bool{return h1.score > h2.score;});

    WriteHighScores(scores);
}

void HighScoreManager::WriteHighScores(std::vector<HighScore> scores)
{
    std::ofstream file;
    file.open(highscorePath, std::ios::out | std::ios::trunc);
    for(int i = 0; i < maxNumberOfHighScores && i < scores.size(); i++)
    {
        HighScore score = scores[i];
        file << score.score << " : " << score.teamName << " : " << std::endl;
    }
}
