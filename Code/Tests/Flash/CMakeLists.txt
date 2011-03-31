set(FLASH_SRCS
	Main.cpp Flash.cpp
)

add_executable(TestFlash ${FLASH_SRCS})

target_link_Libraries(TestFlash MonocleCore ${CORE_LINK})
