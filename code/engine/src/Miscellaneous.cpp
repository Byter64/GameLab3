#include "Miscellaneous.h"
#include "Engine.h"

extern GLFWwindow* window;
namespace Engine
{
    bool isGamePaused;
    bool areAnimationsPaused;

    /*Tells the game to terminate. The current iterationuu of the game loop will still be regularly finished and
     * Engine::OnEndGame() will be called before actually closing the application.
     */
    void EndGame()
    {
        glfwSetWindowShouldClose(window, true);
    }

    /// Tells the game to pause
    /// \param pauseAnimations
    void PauseGame(bool pauseAnimations)
    {
        isGamePaused = true;
        areAnimationsPaused = pauseAnimations;
        Systems::timeManager->OnPause();
    }

    /// Tells the game to continue after a pause
    void ContinueGame()
    {
        isGamePaused = false;
        areAnimationsPaused = false;
        Systems::timeManager->OnContinue();
    }

    bool GetIsGamePaused()
    {
        return isGamePaused;
    }
} // Engine