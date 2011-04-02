set(OGMO_SRCS
	Main.cpp Ogmo.cpp
)

add_executable(TestOgmo ${OGMO_SRCS})

target_link_libraries(TestOgmo MonocleCore ${CORE_LINK})

