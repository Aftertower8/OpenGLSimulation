cmake_minimum_required(VERSION 3.28)
project(simulation)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(externalDep header)

add_library(core
	src/shaderClass.cpp
	src/VAO.cpp
	src/VBO.cpp
	src/EBO.cpp
	)

add_executable(${PROJECT_NAME} src/main.cpp src/glad.c)

target_link_libraries(${PROJECT_NAME} core glfw OpenGL::GL dl)

