set(FLASH_SRCS
  Flash.cpp
)

add_executable(TestFlash ${FLASH_SRCS})

target_link_Libraries(TestFlash MonocleCore ${CORE_LINK})
