-- Monocle Library
project "MonocleCore"
	kind "StaticLib"
	language "C++"

	monocle_os_defines();
	monocle_includedirs();

	basedir	( _BUILD_BASE.."/MonocleCore" )

	files { "Monocle/**.h", "Monocle/**.cpp", "Monocle/**.c" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" } 
