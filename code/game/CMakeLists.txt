cmake_minimum_required(VERSION 3.18)

set(SOURCE_FILES src/main.cpp
        ../engine/src/InputSystem.cpp
        ../engine/include/InputSystem.h
        ../engine/src/InputActions/InputAction.cpp
        ../engine/include/InputAction.h
        ../engine/src/InputActions/InputActionButton.cpp
        ../engine/include/InputActionButton.h
        ../engine/include/InputActionVec2.h
        ../engine/src/InputActions/InputActionVec2.cpp
        ../engine/include/Entity.h
        ../engine/src/EntityComponentSystem/EntityManager.cpp
        ../engine/include/EntityManager.h
        ../engine/include/ComponentType.h
        ../engine/include/IComponentArray.h
        ../engine/include/ComponentArray.h
        ../engine/include/ComponentManager.h
        ../engine/include/System.h
        ../engine/src/EntityComponentSystem/SystemManager.cpp
        ../engine/include/SystemManager.h
        ../engine/src/EntityComponentSystem/ECSSystem.cpp
        ../engine/include/ECSSystem.h
        ../engine/src/EntityComponentSystem/Transform.cpp
        ../engine/include/Transform.h
        ../engine/src/EntityComponentSystem/MeshRenderer.cpp
        ../engine/include/MeshRenderer.h
        ../engine/src/EntityComponentSystem/RenderSystem.cpp
        ../engine/include/RenderSystem.h
        ../engine/include/Material.h
        ../engine/include/Engine.h
        ../engine/src/FileSystem.cpp
        ../engine/include/FileSystem.h
        ../engine/include/Name.h
        ../engine/src/EntityComponentSystem/EntityUtility.cpp
        ../engine/include/EntityUtility.h
        ../engine/src/EntityComponentSystem/Shader.cpp
        ../engine/include/Shader.h
        src/GameObject.cpp
        include/GameObject.h
        ../engine/include/GameEvents.h
        ../engine/src/EntityComponentSystem/GameEventSystem.cpp
        ../engine/include/GameEventSystem.h
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