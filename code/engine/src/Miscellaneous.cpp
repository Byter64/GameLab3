#include "Miscellaneous.h"
#include "Engine.h"

extern GLFWwindow* window;
namespace Engine
{
    /*Tells the game to terminate. The current iterationuu of the game loop will still be regularly finished and
     * Engine::OnEndGame() will be called before actually closing the application.
     */
    void EndGame()
    {
        glfwSetWindowShouldClose(window, true);
    }
} // Engine