file(GLOB CORE_SRCS *.cpp)
file(GLOB COLLIDERS_SRCS Colliders/*.cpp)
file(GLOB OPENAL_SRCS OpenAL/*.cpp)
file(GLOB OPENGL_SRCS OpenGL/*.cpp OpenGL/glpng/*.c OpenGL/glpng/*/*.c)

set(CORE_LINK ${CORE_LINK} ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY})

if(WINDOWS)
	file(GLOB WINDOWS_SRCS Windows/*.cpp)
	set(PLATFORM_SRCS ${PLATFORM_SRCS} ${WINDOWS_SRCS})
endif(WINDOWS)

if(LINUX)
	include_directories(${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR})
	set(CORE_LINK ${CORE_LINK} ${X11_LIBRARIES})
	file(GLOB LINUX_SRCS Linux/*.cpp)
	set(PLATFORM_SRCS ${PLATFORM_SRCS} ${LINUX_SRCS})
endif(LINUX)

add_library(MonocleCore
	${CORE_SRCS}
	${COLLIDERS_SRCS}
	${PLATFORM_SRCS}
	${OPENAL_SRCS}
	${OPENGL_SRCS}
)

# TODO: Add proper dependency handling
target_link_libraries(MonocleCore ${CORE_LINK})

add_subdirectory(Tests)

