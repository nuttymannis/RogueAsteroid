cmake_minimum_required(VERSION 4.4.2)
project(RogueAsteroid VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(RogueAsteroid 
    src/config.h 
    src/entity.cpp
    src/entity.h
    src/entity_hitbox.cpp
    src/entity_hitbox.h
    src/main.cpp 
    src/glad.c
    src/triangle_mesh.cpp
    src/triangle_mesh.h
    src/circle_mesh.cpp
    src/circle_mesh.h
    src/quad_mesh.cpp
    src/quad_mesh.h
    src/bullet.cpp
    src/bullet.h
    src/game.cpp
    src/game.h
    src/player.cpp
    src/player.h
    src/star.cpp
    src/star.h
    src/asteroid.cpp
    src/asteroid.h
    src/asteroid_mesh.cpp
    src/asteroid_mesh.h
    src/a_math.h
)

target_include_directories(RogueAsteroid PRIVATE dependencies)

target_link_directories(RogueAsteroid PRIVATE lib)

target_link_libraries(RogueAsteroid PRIVATE glfw3 OpenGL::GL)