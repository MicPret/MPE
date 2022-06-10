cmake_minimum_required(VERSION 3.20)

#-------STANDARD-------
set(CMAKE_CXX_STANDARD 17)

project(MPE)

#--EXTERNAL LIBRARIES--
set(MPE_EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/ext)

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
	${MPE_SOURCES})

#-------INCLUDE--------
target_include_directories(MPE PUBLIC ${MPE_INCLUDE_PATHS})
target_include_directories(MPE PUBLIC ${MPE_EXTERNAL})


#---------LINK---------
target_link_libraries(MPE PUBLIC
	glfw
)

#------RESOURCES-------
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(MPE copy_assets)