--premake4-helper.lua
newoption
{
	trigger		=	"testapp",
	description	=	"Generate project files for a monocle test app"
}

--Set app name
if _MONOCLE_APP == nil then
	if _OPTIONS["testapp"] ~= nil then
		_MONOCLE_APP = _OPTIONS["testapp"]
	else
		_MONOCLE_APP = "Monocle"
	end
end

-- jw: Fix premake4 --help
if _ACTION ~= NIL then
	_BUILD_BASE		= _MONOCLE_APP_BASE.."/Build/gen-".._ACTION.."-".._MONOCLE_APP
else
	_BUILD_BASE		= _MONOCLE_APP_BASE
end

_MONOCLE_INCLUDE		= _MONOCLE_BASE.."/Core"
_MONOCLE_EXTLIB_BASE		= _MONOCLE_BASE.."/Libraries"
_MONOCLE_EXTLIB_LIB_INC		= _MONOCLE_EXTLIB_BASE.."/Compiled"

--Preprocessor os specific defines 
function monocle_os_defines()
	defines {"MONOCLE_OPENGL","MONOCLE_OPENAL","MONOCLE_AUDIO_OGG"}
	if os.is( "windows" ) == true then
		defines {"MONOCLE_WINDOWS"}
	elseif os.is( "linux" ) == true then
		defines {"MONOCLE_LINUX"}
	elseif os.is( "macosx" ) == true then
		defines {"MONOCLE_MAC"}
	end
end

--Get os specific directory name where precompiled libraries are present
function monocle_get_os_lib_dir()
	if os.is( "windows" ) == true then
		return ("Win32")
	elseif os.is( "linux" ) == true then
		return ("Linux")
	elseif os.is( "macosx" ) == true then
		return ("MacOSX")
	end
end

--Set include directories for a 3rd party library 
function monocle_extlib(name)
	-- needs to be called for each library
	includedirs{ (_MONOCLE_EXTLIB_BASE.."/"..name) }
	-- os and compiler specific includes go here
end

--Set include paths for the Monocle libary directories
function monocle_os_includedirs()
	-- needs to be called once in a project
	includedirs{ _MONOCLE_INCLUDE,_MONOCLE_EXTLIB_BASE }
	libdirs { (_MONOCLE_EXTLIB_LIB_INC.."/"..monocle_get_os_lib_dir()) }
end

-- helper function to include header / library paths and link libraries by os
function monocle_os_links()
	links { "MonocleCore" }
	
	monocle_os_links_base()
end

-- convenient if one wants to build a project that includes all the core source 'n libs
function monocle_os_links_base()
	print( _MONOCLE_EXTLIB_BASE );
	
	monocle_extlib("glew")
	monocle_extlib("openal")
	monocle_extlib("TinyXML")
	
	if os.is( "windows" ) == true then
		links {"Winmm", "glew32s", "opengl32", "glu32", "openal32"}
	elseif os.is( "linux" ) == true then
		links { "X11","GLEW","GL","GLU","openal" }
	elseif os.is( "macosx" ) == true then
		links { "OpenGL.framework", "OpenAL.framework", "Cocoa.framework" }
		links {"GLEW" }
	end
end

--Generate path to target build directory
function monocle_build_targetdir( suffix )
	if suffix ~= nil then
		targetdir (_BUILD_BASE.."-bin-"..suffix.."/")
	else
		targetdir (_BUILD_BASE.."-bin/")
	end
end

--Ensure Monocle has a solution name
if _MONOCLE_SOLUTION_NAME == nil then
	_MONOCLE_SOLUTION_NAME = (_MONOCLE_APP.."Solution")
end

--Helper function to include a testapp project into solution
function monocle_project_testapp( name )
		_TESTAPP_PREMAKE_GENERIC_SCRIPT 	= _MONOCLE_BASE.."/Samples/premake4-test-generic.lua"
		_TESTAPP_PREMAKE_SCRIPT 		= _MONOCLE_BASE.."/Samples/"..name.."/premake4-"..name..".lua"
		
		if os.isfile( _TESTAPP_PREMAKE_SCRIPT ) then
			dofile( _TESTAPP_PREMAKE_SCRIPT )
		else
			dofile( _TESTAPP_PREMAKE_GENERIC_SCRIPT )
		end
end

--Helper function to include MonocleCore Library project into solution
function monocle_project_corelib()
	dofile( (_MONOCLE_BASE.."/Core/premake4-monocle.lua") );
end

--end of helper
