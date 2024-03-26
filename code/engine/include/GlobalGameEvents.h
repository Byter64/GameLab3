#pragma once
#include "Engine.h"

void OnStartGame(int screenWidth, int screenHeight);
void Update(float deltaTime);

//use Engine::Systems::timeManager.GetTimeSinceStartupWithoutPauses() if you need time data here
void UpdateWithoutPause();
void OnEndGame();
