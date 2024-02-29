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
        ../engine/src/ECS/Components/PlayerController.cpp
        ../engine/include/ECS/Components/PlayerController.h
        ../engine/src/ECS/Systems/PlayerControllerSystem.cpp
        ../engine/include/ECS/Systems/PlayerControllerSystem.h
        ../engine/src/ECS/Entity.cpp
        ../engine/include/ECS/Components/Bullet.h
        ../engine/src/ECS/Systems/BulletSystem.cpp
        ../engine/include/ECS/Systems/BulletSystem.h
        ../engine/include/ECS/Components/EnemyBehaviour.h
        ../engine/src/ECS/Systems/EnemyBehaviourSystem.cpp
        ../engine/include/ECS/Systems/EnemyBehaviourSystem.h
        ../engine/include/ECS/Components/Health.h
        ../engine/include/ECS/Components/Dungeon.h
        ../engine/src/ECS/Systems/DungeonSystem.cpp
        ../engine/include/ECS/Systems/DungeonSystem.h
        ../engine/src/ECS/Systems/DungeonEnemySystem.cpp
        ../engine/include/ECS/Systems/DungeonEnemySystem.h
        ../engine/src/ECS/Components/Dungeon.cpp
        ../engine/include/ECS/Helpers/RenderLayer.h
        ../extern/glText/gltext.h
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