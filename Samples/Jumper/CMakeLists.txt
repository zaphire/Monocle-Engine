set(JUMPER_SRCS
	Main.cpp Jumper.cpp
)

add_executable(TestJumper ${JUMPER_SRCS})

target_link_libraries(TestJumper MonocleCore ${CORE_LINK})

