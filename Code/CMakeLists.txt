FILE(GLOB CORE_SRCS *.cpp)

# Used later for install
FILE(GLOB CORE_HDRS *.h)

if(WIN32)
	FILE(GLOB WINDOWS_SRCS Windows/*.cpp)
	FILE(GLOB WINDOWS_HDRS Windows/*.h)
	set(PLATFORM_SRCS ${PLATFORM_SRCS} ${WINDOWS_SRCS})
	set(PLATFORM_HDRS ${PLATFORM_HDRS} ${WINDOWS_HDRS})
endif(WIN32)

add_library(MonocleCore ${CORE_SRCS} ${PLATFORM_SRCS})

add_subdirectory(Tests)

