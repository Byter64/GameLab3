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
        Waiting,
        Duelling,
        Retreating
    };

    inline static const int preparingThreshold = 7;
    inline static const int fightingThreshold = 17;
    inline static float detectionRadius;
    inline static float minPlayerDistance;
    inline static float prefPlayerDistance;
    inline static std::list<std::pair<int, int>> preparationPositions{};
    BigPhase bigPhase = BigPhase::Waiting;
    Phase phase = Phase::Waiting;
    Engine::Entity target;
    int targetPathNode = 0;

    ~DukeExtra() = default;
};
