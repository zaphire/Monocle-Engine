project(Monocle)
cmake_minimum_required(VERSION 2.6.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include_directories(AFTER SYSTEM ${PROJECT_BINARY_DIR} Code)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Build/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Build/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_TESTS OFF CACHE BOOL "Build tests")
set(WITH_OPENAL ON CACHE BOOL "Use OpenAL")
set(WITH_OPENGL ON CACHE BOOL "Use OpenGL")
set(WITH_AUDIOOGG ON CACHE BOOL "Use OGG Vorbis")

if(WIN32)
	add_definitions(-DMONOCLE_WINDOWS)
	set(WINDOWS ON)
endif(WIN32)

if (APPLE)
	add_definitions(-DMONOCLE_APPLE)
	set(MACOSX ON)
	set(APPLE ON)
endif(APPLE)

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	find_package(X11 REQUIRED)
	add_definitions(-DMONOCLE_LINUX)
	set(LINUX ON)
endif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

if(WITH_OPENGL)
	find_package(OpenGL REQUIRED)
	add_definitions(-DMONOCLE_OPENGL)
endif(WITH_OPENGL)

if(WITH_OPENAL)
	find_package(OpenAL REQUIRED)
	add_definitions(-DMONOCLE_OPENAL)
endif(WITH_OPENAL)

if(WITH_AUDIOOGG)
	add_definitions(-DMONOCLE_AUDIO_OGG)
endif(WITH_AUDIOOGG)

add_subdirectory(Code)

