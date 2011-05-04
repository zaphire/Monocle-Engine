--
-- Premake4 solution script
-- Created: Airbash, May/03/2011
-- Example usage: "premake4 --testapp=Pong vs2010"
--

newoption
{
	trigger		=	"testapp",
	description	=	"Generate project files for a monocle test app"
}


_MONOCLE_APP = "Monocle"
if _OPTIONS["testapp"] ~= nil then
	_MONOCLE_APP = _OPTIONS["testapp"]
end



_MONOCLE_BASE			= os.getcwd()

-- jw: Fix premake4 --help
if _ACTION ~= NIL then
	_BUILD_BASE		= os.getcwd().."/Build/gen-".._ACTION.."-".._MONOCLE_APP
else
	_BUILD_BASE		= os.getcwd()
end

_MONOCLE_INCLUDE		= _MONOCLE_BASE.."/Core"
_MONOCLE_EXTLIB_BASE		= _MONOCLE_BASE.."/Libraries"
_MONOCLE_EXTLIB_LIB_INC		= _MONOCLE_EXTLIB_BASE.."/Compiled"


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

function monocle_get_os_lib_dir()
	if os.is( "windows" ) == true then
		return ("Win32")
	elseif os.is( "linux" ) == true then
		return ("Linux")
	elseif os.is( "macosx" ) == true then
		return ("MacOSX")
	end
end

function monocle_extlib(name)
	-- needs to be called for each library
	includedirs{ (_MONOCLE_EXTLIB_BASE.."/"..name) }
	-- os and compiler specific includes go here
end

function monocle_os_includedirs()
	-- needs to be called once in a project
	includedirs{ _MONOCLE_INCLUDE,_MONOCLE_EXTLIB_BASE }
	libdirs { (_MONOCLE_EXTLIB_LIB_INC.."/"..monocle_get_os_lib_dir()) }
end

function monocle_os_links()
	-- helper function to include header / library paths and link libraries by os
	-- nightmare for case-sensitivity on linux
	links { "MonocleCore","TinyXML" }
	
	print( _MONOCLE_EXTLIB_BASE );
	
	monocle_extlib("glew")
	monocle_extlib("openal")
	monocle_extlib("ogg")
	monocle_extlib("vorbis")
	monocle_extlib("TinyXML")
	
	if os.is( "windows" ) == true then
		links {"Winmm", "glew32s", "opengl32", "glu32", "openal32", "libogg_static","libvorbis_static","libvorbisfile_static"}
	elseif os.is( "linux" ) == true then
		-- this needs to be tested
		links { "X11","GLEW","GL","GLU","openal","ogg","vorbis","vorbisfile","vorbisenc" }
	elseif os.is( "macosx" ) == true then
		links { "OpenGL.framework", "OpenAL.framework", "Cocoa.framework" }
		links {"GLEW", "ogg","vorbis","vorbisfile","vorbisenc" }
	end
end

function monocle_build_targetdir( suffix )
	if suffix ~= nil then
		targetdir (_BUILD_BASE.."-bin-"..suffix.."/")
	else
		targetdir (_BUILD_BASE.."-bin/")
	end
end



_MONOCLE_SOLUTION_NAME = "MonocleLibrary"
if _OPTIONS["testapp"] ~= nil then
	_MONOCLE_SOLUTION_NAME  = (_OPTIONS["testapp"].."Solution")
end


--
-- Monocle Solution
--
solution (_MONOCLE_SOLUTION_NAME)
	basedir( _BUILD_BASE )
	configurations { "Debug", "Release" }

	if _OPTIONS["testapp"] ~= NIL then
		_TESTAPP_PREMAKE_GENERIC_SCRIPT 	= "Tests/premake4-test-generic.lua"
		_TESTAPP_PREMAKE_SCRIPT 			= "Tests/".._OPTIONS["testapp"].."/premake4-".._OPTIONS["testapp"]..".lua"
		
		if os.isfile( _TESTAPP_PREMAKE_SCRIPT ) then
			dofile( _TESTAPP_PREMAKE_SCRIPT )
		else
			dofile( _TESTAPP_PREMAKE_GENERIC_SCRIPT )
		end
	end

	-- Monocle Core Library
	dofile( "Core/premake4-monocle.lua" );
