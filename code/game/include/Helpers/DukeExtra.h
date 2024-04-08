#pragma once
#include <list>

struct DukeExtra
{
    enum class BigPhase
    {
        Waiting,
        Preparing,
        Fighting
    };

    enum class Phase
    {
        Chasing,
        Duelling,
        Retreating
    };

    inline static const int preparingThreshold = 7;
    inline static const int fightingThreshold = 17;
    inline static std::list<std::pair<int, int>> preparationPositions{};
    BigPhase bigPhase = BigPhase::Waiting;
    Phase phase = Phase::Chasing;

    int targetPathNode = 0;

    ~DukeExtra() = default;
};
