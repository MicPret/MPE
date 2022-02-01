cmake_minimum_required(VERSION 3.20)

project(MPE)

#--EXTERNAL LIBRARIES--
set(MPE_EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/ext)

#--------IMGUI---------
set(IMGUI_PATH ${MPE_EXTERNAL}/imgui)

file(GLOB IMGUI_SOURCES
	${IMGUI_PATH}/*.cpp
)

#---------MPE----------
file (GLOB MPE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB MPE_INCLUDE_PATHS
	${MPE_EXTERNAL}/*/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

#---------GLFW---------
add_subdirectory(ext/glfw)

#------EXECUTABLE------
add_executable(MPE
	ext/glad/src/gl.c	#glad
	${IMGUI_SOURCES}	#imgui
	${MPE_SOURCES}
 "src/Shader.cpp" "src/Renderer.cpp")

#-------INCLUDE--------
target_include_directories(MPE PUBLIC ${MPE_INCLUDE_PATHS})
target_include_directories(MPE PUBLIC ${MPE_EXTERNAL})


#---------LINK---------
target_link_libraries(MPE PUBLIC
	glfw
)
