-- Monocle Library
project "MonocleCore"
	kind "StaticLib"
	language "C++"
	
	monocle_os_defines();
	monocle_os_includedirs();
	
	basedir	( _BUILD_BASE )
	
	files { "**.h", "**.cpp", "**.c" }
	
	files { _MONOCLE_EXTLIB_BASE.."/stb/**.c" }
	files { _MONOCLE_EXTLIB_BASE.."/TinyXML/**.cpp" }
	files { _MONOCLE_EXTLIB_BASE.."/GL/glew.c" }
	files { _MONOCLE_EXTLIB_BASE.."/ogg/*.c" }
	files { _MONOCLE_EXTLIB_BASE.."/vorbis/*.c" }
	--files { _MONOCLE_EXTLIB_BASE.."/lua/**.c" }

	configuration "macosx"
		files { "**.mm" }
	
	--links{ "TinyXML" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "Debug"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" } 

	
-- Old TinyXML Project (source just added to Core for now)
--[[
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
]]--