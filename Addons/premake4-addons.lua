newoption
{
	trigger		= "create-addon",
	value		= "ADDON",
	description	= "Creates a new addon named ADDON in the Addons directory."
}

if _OPTIONS["create-addon"] ~= nil then
	local addon = _OPTIONS["create-addon"]
	print("Creating addon "..addon)

	--print("mkdir \"".._MONOCLE_BASE.."/Addons/"..addon.."\"")
	os.execute("mkdir \"".._MONOCLE_BASE.."/Addons/"..addon.."\"")
	local files = os.matchfiles(_MONOCLE_BASE.."/Addons/default/*")
	
	print("Copying files from default...")
	for k, f in pairs(files) do
		local fin = assert(io.open(f, "r"))
		local text = fin:read("*all")
		fin:close()

		text = text:gsub("$ADDON_NAME", addon)

		local outfname = f:gsub("/default/", "/"..addon.."/")
		local fout = assert(io.open(outfname, "w"))
		fout:write(text)
		fout:close()
	end


	os.exit(0)
end

function monocle_project_addons()
	local addondirs = os.matchdirs(_MONOCLE_BASE.."/Addons/*")
	for i, dir in pairs(addondirs) do
		if dir:find("default") == nil then
			dofile(dir.."/premake.lua")
		end
	end
end