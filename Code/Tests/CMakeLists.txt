if(BUILD_TESTS)
    if(TEST_TO_BUILD MATCHES "Jumper")
	    add_subdirectory(Jumper)
	endif(TEST_TO_BUILD MATCHES "Jumper")

	if(TEST_TO_BUILD MATCHES "Flash")
		add_subdirectory(Flash)
	endif(TEST_TO_BUILD MATCHES "Flash")
endif(BUILD_TESTS)

