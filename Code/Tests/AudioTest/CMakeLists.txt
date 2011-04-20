set(AUDIOTEST_SRCS
	Main.cpp AudioTest.cpp
)

add_executable(TestPong ${AUDIOTEST_SRCS})

target_link_Libraries(TestPong MonocleCore ${CORE_LINK})
