cmake_minimum_required(VERSION 3.18)

set(SOURCE_FILES src/main.cpp
        ../engine/src/InputSystem.cpp
        ../engine/src/InputSystem.h
        ../engine/src/InputActions/InputAction.cpp
        ../engine/src/InputActions/InputAction.h
        ../engine/src/InputActions/InputActionButton.cpp
        ../engine/src/InputActions/InputActionButton.h
        ../engine/src/InputActions/InputActionVec2.h
        ../engine/src/InputActions/InputActionVec2.cpp
        ../engine/src/EntityComponentSystem/Entity.cpp
        ../engine/src/EntityComponentSystem/Entity.h
        ../engine/src/EntityComponentSystem/EntityManager.cpp
        ../engine/src/EntityComponentSystem/EntityManager.h
        ../engine/src/EntityComponentSystem/ComponentType.cpp
        ../engine/src/EntityComponentSystem/ComponentType.h
        ../engine/src/EntityComponentSystem/IComponentArray.h
        ../engine/src/EntityComponentSystem/ComponentArray.h
        ../engine/src/EntityComponentSystem/ComponentManager.h
        ../engine/src/EntityComponentSystem/System.h
        ../engine/src/EntityComponentSystem/SystemManager.cpp
        ../engine/src/EntityComponentSystem/SystemManager.h
        ../engine/src/EntityComponentSystem/ECSSystem.cpp
        ../engine/src/EntityComponentSystem/ECSSystem.h
        ../engine/src/EntityComponentSystem/Transform.cpp
        ../engine/src/EntityComponentSystem/Transform.h
        ../engine/src/EntityComponentSystem/MeshRenderer.cpp
        ../engine/src/EntityComponentSystem/MeshRenderer.h
        ../engine/src/EntityComponentSystem/RenderSystem.cpp
        ../engine/src/EntityComponentSystem/RenderSystem.h
        ../engine/src/EntityComponentSystem/Material.h
)

set(EXE_FILE Game)

add_executable(${EXE_FILE} ${SOURCE_FILES})

target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)
target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm) # Hallo ich bin ein Kommentar :)