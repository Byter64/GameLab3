cmake_minimum_required(VERSION 3.18)

set(SOURCE_FILES ../engine/src/main.cpp
        ../engine/src/InputSystem.cpp
        ../engine/include/InputSystem.h
        ../engine/src/InputActions/InputAction.cpp
        ../engine/include/InputAction.h
        ../engine/src/InputActions/InputActionButton.cpp
        ../engine/include/InputActionButton.h
        ../engine/include/InputActionVec2.h
        ../engine/src/InputActions/InputActionVec2.cpp
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
        ../engine/src/ECS/Transform.cpp
        ../engine/include/ECS/Components/Transform.h
        ../engine/include/MeshRenderer.h
        ../engine/src/ECS/RenderSystem.cpp
        ../engine/include/RenderSystem.h
        ../engine/include/Material.h
        ../engine/include/Engine.h
        ../engine/src/FileSystem.cpp
        ../engine/include/FileSystem.h
        ../engine/include/Name.h
        ../engine/src/ECS/EntityUtility.cpp
        ../engine/include/EntityUtility.h
        ../engine/src/ECS/Shader.cpp
        ../engine/include/Shader.h
        src/GameObject.cpp
        include/GameObject.h
        ../engine/include/GameEvents.h
        ../engine/src/ECS/GameEventSystem.cpp
        ../engine/include/GameEventSystem.h
        src/GameObjectManager.cpp
        include/GameObjectManager.h
        src/Miscellaneous.cpp
        include/Miscellaneous.h
        src/Dungeon.cpp
        include/Dungeon.h
        ../engine/src/CollisionSystem.cpp
        ../engine/include/CollisionSystem.h
        ../engine/include/BoxCollider.h
        ../engine/src/ECS/BoxCollider.cpp
        ../engine/include/GlobalGameEvents.h
        src/GlobalGameEvents.cpp
        src/Player.cpp
        include/Player.h
        ../engine/include/ECS/Systems/TransformParentSystem.h
        ../engine/src/ECS/Systems/TransformParentSystem.cpp
        ../engine/src/ECS/Systems/TransformParentSystem.h
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