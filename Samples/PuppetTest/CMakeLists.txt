set(PUPPET_SRCS
        Main.cpp PuppetTest.cpp
)

add_executable(TestPuppet ${PUPPET_SRCS})
target_link_libraries(TestPuppet MonocleCore ${CORE_LINK})

