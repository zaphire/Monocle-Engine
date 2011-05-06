--premake4.lua
--
-- Premake4 solution script
-- Created: Airbash, May/03/2011
-- Example usage: "premake4 --testapp=Pong vs2010"
--

-- Required globals for helper script
_MONOCLE_APP_BASE		= os.getcwd()			--root directory of your project
_MONOCLE_BASE			= os.getcwd()			--root directory for monocle

dofile( (_MONOCLE_BASE.."/premake4-helper.lua") )

print( _MONOCLE_EXTLIB_BASE );

--
-- Monocle Solution
--
solution (_MONOCLE_SOLUTION_NAME)
	basedir( _BUILD_BASE )
	configurations { "Debug", "Release" }

	--Test Application 
	if _OPTIONS["testapp"] ~= NIL then
		monocle_project_testapp( _OPTIONS["testapp"] )
	end

	-- Monocle Core Library
	monocle_project_corelib();
