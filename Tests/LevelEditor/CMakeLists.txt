set(TEST_LEVELEDITOR_SRCS
	Main.cpp LevelEditorTest.cpp
)

add_executable(TestLevelEditor ${TEST_LEVELEDITOR_SRCS})

target_link_Libraries(TestLevelEditor MonocleCore ${CORE_LINK})
