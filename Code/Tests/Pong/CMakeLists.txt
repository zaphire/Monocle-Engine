set(PONG_SRCS
  Pong.cpp
)

add_executable(TestPong ${PONG_SRCS})

target_link_Libraries(TestPong MonocleCore ${CORE_LINK})
