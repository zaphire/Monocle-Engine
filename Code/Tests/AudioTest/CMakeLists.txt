set(AUDIOTEST_SRCS
	Main.cpp AudioTest.cpp
)

add_executable(TestAudio ${AUDIOTEST_SRCS})

target_link_Libraries(TestAudio MonocleCore ${CORE_LINK})
