	-- Generic test app premake4 script
	project (_OPTIONS["testapp"])
		kind "ConsoleApp"
		language "C++"
		basedir	( _BUILD_BASE )
		
		monocle_os_defines()
		monocle_os_includedirs()
		monocle_os_links()

		files { _OPTIONS["testapp"].."/**.h", _OPTIONS["testapp"].."/**.cpp" }
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			monocle_build_targetdir( "debug" )
			
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" } 
			monocle_build_targetdir( "release" )