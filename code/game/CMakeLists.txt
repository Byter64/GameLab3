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
        include/Components/EnemyBehaviour.h
        src/Systems/EnemyBehaviourSystem.cpp
        include/Systems/EnemyBehaviourSystem.h
        include/Components/Health.h
        include/Components/Dungeon.h
        src/Systems/DungeonSystem.cpp
        include/Systems/DungeonSystem.h
        src/Systems/DungeonEnemySystem.cpp
        include/Systems/DungeonEnemySystem.h
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
        include/Helpers/EnemyExtra.h
        include/Helpers/KindredSpiritExtra.h
        ../engine/include/InputSystem/GamepadInputID.h
        ../engine/src/InputSystem/GamepadInputID.cpp
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
)

set(EXE_FILE Game)
add_executable(${EXE_FILE} ${SOURCE_FILES})

include_directories(../engine/include)
include_directories(include)
add_custom_command(
        TARGET ${EXE_FILE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game/Assets
        $<TARGET_FILE_DIR:${EXE_FILE}>/Assets)

target_compile_definitions(${EXE_FILE} PRIVATE _ASSET_ROOT=Assets/)
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)
target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm) # Hallo ich bin ein Kommentar :)