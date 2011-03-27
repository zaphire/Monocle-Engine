set(FTE_SRCS
	Main.cpp FTE.cpp
)

add_executable(TestFTE ${FTE_SRCS})

target_link_Libraries(TestFTE MonocleCore ${CORE_LINK})
