#pragma once


enum class CollisionLayer : unsigned char
{
    Player,
    Enemy,
    Dungeon,
    Bullet,
    Collectible,
    Ignore
};
