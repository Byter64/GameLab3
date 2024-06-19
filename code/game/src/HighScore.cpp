#include "HighScore.h"

bool HighScore::operator<(const HighScore &other)
{
    return score < other.score;
}
