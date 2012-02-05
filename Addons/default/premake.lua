project ("Addon.$ADDON_NAME")
	kind "StaticLib"
	language "C++"
	targetname "Monocle$ADDON_NAME"
	basedir	( _BUILD_BASE )
	
	monocle_os_defines()
	monocle_os_includedirs()
	monocle_os_links()

	files { _MONOCLE_BASE.."/Addons/$ADDON_NAME/**.cpp" }
	files { _MONOCLE_BASE.."/Addons/$ADDON_NAME/**.h" }
		
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "Debug"
			
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" } 