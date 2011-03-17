set(PONG_SRCS
	Jumper.cpp
)

add_executable(TestJumper ${PONG_SRCS})

target_link_libraries(TestJumper MonocleCore ${CORE_LINK})

