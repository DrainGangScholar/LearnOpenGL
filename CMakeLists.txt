cmake_minimum_required(VERSION 3.29)
project(opengl)

#find system glfw3 and opengl
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
#add local glad
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
#add source file
add_executable(main src/main.cpp)
#link necessary libraries
target_link_libraries(main PRIVATE OpenGL::GL glfw glad)
