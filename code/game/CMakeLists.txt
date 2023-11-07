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
)

set(EXE_FILE Game)

add_executable(${EXE_FILE} ${SOURCE_FILES})

target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)
target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm) # Hallo ich bin ein Kommentar :)