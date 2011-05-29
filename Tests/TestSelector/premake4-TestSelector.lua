
function TestSelector_includefiles( name )
	files { "../"..name.."/**.h", "../"..name.."/**.cpp" }

	if name ~= "TestSelector" then
		excludes { "../"..name.."/Main.cpp" }
	end

	includedirs{ "../"..name }
end

	-- Generic test app premake4 script
	project (_OPTIONS["testapp"])
		kind "ConsoleApp"
		language "C++"
		basedir	( _BUILD_BASE )
		
		monocle_os_defines()
		monocle_os_includedirs()
		monocle_os_links()

		TestSelector_includefiles("AudioTest")
		TestSelector_includefiles("Flash")
		TestSelector_includefiles("Jumper")
		TestSelector_includefiles("LevelEditor")
		TestSelector_includefiles("Ogmo")
		TestSelector_includefiles("Pong")
		TestSelector_includefiles("PuppetTest")
		TestSelector_includefiles("TestSelector")
		
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			monocle_build_targetdir( "debug" )
			
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" } 
			monocle_build_targetdir( "release" )