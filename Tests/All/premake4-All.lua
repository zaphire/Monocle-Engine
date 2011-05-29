
function monocletest_do_project_test( name )
	_OPTIONS["testapp"] = name
	
		_TESTAPP_PREMAKE_GENERIC_SCRIPT 	= "../premake4-test-generic.lua"
		_TESTAPP_PREMAKE_SCRIPT 			= "../".._OPTIONS["testapp"].."/premake4-".._OPTIONS["testapp"]..".lua"
		
		if os.isfile( _TESTAPP_PREMAKE_SCRIPT ) then
			dofile( _TESTAPP_PREMAKE_SCRIPT )
		else
			dofile( _TESTAPP_PREMAKE_GENERIC_SCRIPT )
		end
end

monocletest_do_project_test( "Pong" )
monocletest_do_project_test( "AudioTest" )
monocletest_do_project_test( "Flash" )
monocletest_do_project_test( "Jumper" )
monocletest_do_project_test( "LevelEditor" )
monocletest_do_project_test( "Ogmo" )
monocletest_do_project_test( "PuppetTest" )
monocletest_do_project_test( "Scripted" )
monocletest_do_project_test( "TestSelector" )

