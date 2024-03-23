#pragma once

namespace Engine
{
    void EndGame();
    void PauseGame(bool pauseAnimations = true);
    void ContinueGame();
    bool GetIsGamePaused();

} // Engine
