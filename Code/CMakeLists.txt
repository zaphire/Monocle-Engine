FILE(GLOB CORE_SRCS *.cpp)

# Used later for install
FILE(GLOB CORE_HDRS *.h)

if(WIN32)
	set(PLATFORM_SRCS ${PLATFORM_SRCS}
		Windows/WindowsOpenGLGraphics.cpp
		Windows/WindowsPlatform.cpp
	)
	set(PLATFORM_HDRS ${PLATFORM_HDRS}
		Windows/WindowsPlatform.h
	)
endif(WIN32)

add_library(MonocleCore ${CORE_SRCS} ${PLATFORM_SRCS})

add_subdirectory(Tests)

