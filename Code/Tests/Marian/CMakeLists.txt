set(MARIAN_SRCS
	Main.cpp Marian.cpp
)

add_executable(TestMarian ${MARIAN_SRCS})

target_link_libraries(TestMarian MonocleCore ${CORE_LINK})

