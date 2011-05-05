-- Monocle Library
project "MonocleCore"
	kind "StaticLib"
	language "C++"
	
	monocle_os_defines();
	monocle_os_includedirs();
	
	basedir	( _BUILD_BASE )
	
	files { "**.h", "**.cpp", "**.c" }
	files { _MONOCLE_EXTLIB_BASE.."/stb/**.c" }
	--files { _MONOCLE_EXTLIB_BASE.."/lua/**.c" }

	configuration "macosx"
		files { "**.mm" }
	
	links{ "TinyXML" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" } 
	
-- TinyXML
-- This should really be distributed as a 3rd party lib
project "TinyXML"
	kind "StaticLib"
	language "C++"
	
	monocle_os_defines();
	monocle_os_includedirs();
	
	basedir	( _BUILD_BASE.."/TinyXML" )
	
	files { _MONOCLE_EXTLIB_BASE.."/TinyXML/**.h", _MONOCLE_EXTLIB_BASE.."/TinyXML/**.cpp" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" } 
