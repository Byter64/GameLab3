cmake_minimum_required(VERSION 3.18)

set(SOURCE_FILES src/main.cpp
        ../engine/src/InputSystem.cpp
        ../engine/src/InputSystem.h
)

set(EXE_FILE Game)

add_executable(${EXE_FILE} ${SOURCE_FILES})

target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)
target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm) # Hallo ich bin ein Kommentar :)