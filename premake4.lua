_MONOCLE_BASE			= os.getcwd()
_BUILD_BASE				= os.getcwd().."/Build/gen-".._ACTION
_MONOCLE_INCLUDE		= _MONOCLE_BASE.."/Core/Monocle"
_MONOCLE_LIBRARIES		= _MONOCLE_BASE.."/Libraries"

newoption
{
	trigger		=	"testapp",
	description	=	"Generate project files for a monocle test app"
}


function monocle_os_defines()
	defines {"MONOCLE_OPENGL","MONOCLE_OPENAL","MONOCLE_AUDIO_OGG"}
	if os.is( "windows" ) == true then
		defines {"MONOCLE_WINDOWS"}
	elseif os.is( "linux" ) == true then
		defines {"MONOCLE_LINUX"}
	elseif os.is( "mac" ) == true then
		defines {"MONOCLE_MAC"}
	end
end

function monocle_includedirs()
	includedirs{ _MONOCLE_INCLUDE }
	includedirs{ _MONOCLE_LIBRARIES }
end

function monocle_os_links()
	links { "MonocleCore" }

	if os.is( "windows" ) == true then
		libdirs { (_MONOCLE_LIBRARIES.."/Compiled") }
	end

	if os.is( "windows" ) == true then
		links {"Winmm", "glew32", "opengl32", "glu32", "openal32", "libogg_static","libvorbis_static","libvorbisfile_static"}
	elseif os.is( "linux" ) == true then

	elseif os.is( "mac" ) == true then

	end
end

function monocle_build_targetdir( suffix )
	if suffix ~= nil then
		targetdir (_BUILD_BASE.."/bin-"..suffix.."/")
	else
		targetdir (_BUILD_BASE.."/bin/")
	end

end



_MONOCLE_SOLUTION_NAME = "MonocleProject"
if _OPTIONS["testapp"] ~= nil then
	_MONOCLE_SOLUTION_NAME  = (_OPTIONS["testapp"].."Solution")
end


--
-- Monocle Solution
--
solution (_MONOCLE_SOLUTION_NAME)
	basedir( _BUILD_BASE )
	configurations { "Debug", "Release" }

	-- Monocle Test App
	if _OPTIONS["testapp"] ~= nil then
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

