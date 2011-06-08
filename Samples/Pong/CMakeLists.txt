set(PONG_SRCS
	Main.cpp Pong.cpp
)

add_executable(TestPong ${PONG_SRCS})

target_link_Libraries(TestPong MonocleCore ${CORE_LINK})
