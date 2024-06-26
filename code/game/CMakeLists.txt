cmake_minimum_required(VERSION 3.18)

set(SOURCE_FILES ../engine/src/main.cpp
        ../engine/src/InputSystem/InputSystem.cpp
        ../engine/include/InputSystem/InputSystem.h
        ../engine/src/InputSystem/InputAction.cpp
        ../engine/include/InputSystem/InputAction.h
        ../engine/src/InputSystem/InputActionButton.cpp
        ../engine/include/InputSystem/InputActionButton.h
        ../engine/include/InputSystem/InputActionVec2.h
        ../engine/src/InputSystem/InputActionVec2.cpp
        ../engine/include/ECS/Entity.h
        ../engine/src/ECS/EntityManager.cpp
        ../engine/include/ECS/EntityManager.h
        ../engine/include/ECS/ComponentType.h
        ../engine/include/ECS/IComponentArray.h
        ../engine/include/ECS/ComponentArray.h
        ../engine/include/ECS/ComponentManager.h
        ../engine/include/ECS/System.h
        ../engine/src/ECS/SystemManager.cpp
        ../engine/include/ECS/SystemManager.h
        ../engine/src/ECS/ECSSystem.cpp
        ../engine/include/ECSSystem.h
        ../engine/src/ECS/Components/Transform.cpp
        ../engine/include/ECS/Components/Transform.h
        ../engine/include/ECS/Components/MeshRenderer.h
        ../engine/src/ECS/Systems/RenderSystem.cpp
        ../engine/include/ECS/Systems/RenderSystem.h
        ../engine/include/ECS/Helpers/Material.h
        ../engine/include/Engine.h
        ../engine/src/FileSystem.cpp
        ../engine/include/FileSystem.h
        ../engine/include/ECS/Components/Name.h
        ../engine/src/ECS/EntityUtility.cpp
        ../engine/include/EntityUtility.h
        ../engine/src/ECS/Helpers/Shader.cpp
        ../engine/include/ECS/Helpers/Shader.h
        ../engine/src/ECS/Systems/CollisionSystem.cpp
        ../engine/include/ECS/Systems/CollisionSystem.h
        ../engine/include/ECS/Components/BoxCollider.h
        ../engine/include/GlobalGameEvents.h
        src/GlobalGameEvents.cpp
        ../engine/include/ECS/Systems/TransformParentSystem.h
        ../engine/src/ECS/Systems/TransformParentSystem.cpp
        ../engine/include/ECS/Helpers/Collision.h
        ../engine/src/ECS/Helpers/Collision.cpp
        src/Components/PlayerController.cpp
        include/Components/PlayerController.h
        src/Systems/PlayerControllerSystem.cpp
        include/Systems/PlayerControllerSystem.h
        ../engine/src/ECS/Entity.cpp
        include/Components/Bullet.h
        src/Systems/BulletSystem.cpp
        include/Systems/BulletSystem.h
        include/Components/Enemies/EnemyBehaviour.h
        src/Systems/EnemyBehaviourSystem.cpp
        include/Systems/EnemyBehaviourSystem.h
        include/Components/Health.h
        include/Components/Dungeon.h
        src/Systems/DungeonSystem.cpp
        include/Systems/DungeonSystem.h
        src/Components/Dungeon.cpp
        ../engine/include/ECS/Helpers/RenderLayer.h
        ../extern/glText/gltext.h
        ../engine/src/ECS/Systems/TextRenderSystem.cpp
        ../engine/include/ECS/Systems/TextRenderSystem.h
        ../engine/include/ECS/Components/Text.h
        ../engine/src/ECS/Components/Text.cpp
        include/Components/Loot.h
        include/CollisionLayer.h
        src/ECSHelper.cpp
        include/ECSHelper.h
        include/ECSExtension.h
        ../engine/include/Systems.h
        ../engine/src/Systems.cpp
        ../engine/src/Miscellaneous.cpp
        ../engine/include/Miscellaneous.h
        ../engine/include/InputSystem/GamepadButton.h
        ../engine/src/InputSystem/GamepadButton.cpp
        ../engine/src/TimeManager.cpp
        ../engine/include/TimeManager.h
        src/GameDefines.cpp
        include/GameDefines.h
        ../engine/include/ECS/Helpers/Animation.h
        ../engine/src/ECS/Systems/AnimationSystem.cpp
        ../engine/include/ECS/Systems/AnimationSystem.h
        ../engine/src/ECS/Helpers/Animation.cpp
        ../engine/include/ECS/Components/Animator.h
        include/Components/Elevator.h
        src/Systems/ElevatorSystem.cpp
        include/Systems/ElevatorSystem.h
        src/Miscellaneous.cpp
        include/Miscellaneous.h
        ../engine/include/ECS/Components/TilemapCollider.h
        include/Components/BreakableWall.h
        src/Systems/BreakableWallSystem.cpp
        include/Systems/BreakableWallSystem.h
        ../extern/a-star-master/include/AStar.hpp
        ../extern/a-star-master/source/AStar.cpp
        ../engine/src/CrashHandler.cpp
        ../engine/include/CrashHandler.h
        ../engine/src/EngineException.cpp
        ../engine/include/EngineException.h
        ../extern/StackWalker-master/Main/StackWalker/StackWalker.cpp
        ../engine/src/ECSException.cpp
        ../engine/include/ECSException.h
        ../engine/src/ECS/ComponentManager.cpp
        include/Helpers/EnemySpawnerData.h
        include/Components/Enemies/Hubertus.h
        src/Systems/HubertusSystem.cpp
        include/Systems/HubertusSystem.h
        include/Systems.h
        include/Helpers/Movement.h
        include/Components/Enemies/KindredSpirit.h
        include/Systems/KindredSpiritSystem.h
        src/Systems/KindredSpiritSystem.cpp
        include/Components/Enemies/Assi.h
        src/Systems/AssiSystem.cpp
        include/Systems/AssiSystem.h
        include/Components/Enemies/Cuball.h
        src/Systems/CuballSystem.cpp
        include/Systems/CuballSystem.h
        include/Components/Enemies/Duke.h
        src/Systems/DukeSystem.cpp
        include/Systems/DukeSystem.h
        src/Systems.cpp
        src/HighScoreManager.cpp
        include/HighScoreManager.h
        src/Scenes/01_TitleScreen.cpp
        include/Scenes/01_TitleScreen.h
        src/Scenes/02_Game.cpp
        include/Scenes/02_Game.h
        src/Scenes/03_GameWin.cpp
        include/Scenes/03_GameWin.h
        src/Scenes/04_GameLost.cpp
        include/Scenes/04_GameLost.h
        ../engine/src/Scene.cpp
        ../engine/include/Scene.h
        ../engine/src/SceneManager.cpp
        ../engine/include/SceneManager.h
        ../engine/src/ECS/Components/MeshRenderer.h.cpp
        include/HighScore.h
        src/HighScore.cpp
        src/Keyboard.cpp
        include/Keyboard.h
        ../engine/include/InputSystem/GamepadAxis.h
        ../engine/src/InputSystem/GamepadAxis.cpp
)

set(EXE_FILE Game)
add_executable(${EXE_FILE} ${SOURCE_FILES})

include_directories(../engine/include)
include_directories(../extern/a-star-master/include)
include_directories(include)
include_directories(../extern/StackWalker-master/Main/StackWalker)
add_custom_command(
        TARGET ${EXE_FILE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game/Assets
        $<TARGET_FILE_DIR:${EXE_FILE}>/Assets)

target_compile_definitions(${EXE_FILE} PRIVATE _ASSET_ROOT=Assets/)
target_compile_definitions(${EXE_FILE} PRIVATE _LOGS_ROOT=Logs/)
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)
target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm ${TARGET_StackWalker}) # Hallo ich bin ein Kommentar :)

if (MSVC)
    # warning level 4
    add_compile_options(/MP)
endif ()