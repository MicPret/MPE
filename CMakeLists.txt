cmake_minimum_required(VERSION 3.8)

project(MPE)

add_subdirectory(ext/glfw)

add_executable(MPE
	main.cpp
	ext/glad/src/gl.c
)

target_link_libraries(MPE
	glfw
)

target_include_directories(MPE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include)
target_include_directories(MPE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/include)