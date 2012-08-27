
local GridSize = 40

--@TODO
function LoadLevel()
	objectList = dofile( "LevelName.lua" )
end

--@TODO
function SaveLevel()
	local file = io.open( "LevelName.lua", "w" )
	file:write( "local LevelList = {\n" )
	for k,v in pairs(objectList) do
		file:write( tostring(k) .. " = {\n" )
		for i,j in pairs(v) do
			file:write( "parameters" )
		end
	end
	file:close()
end


function CreateLevel()	
	local scale = 6.5
	Camera:SetZoomFactor2D( scale )	

	print("Starting to create objects")	

	-- player
	table.insert( objectList, CreateNewObject( "PPPlayer", 0, 0, 0) )
		
	--PLAYER = objectList[ table.getn(objectList) ]
	print("Finished creating objects")
end


function CleanupGame()

	for k,v in ipairs(objectList) do
		--v:Delete()
	end
	UnloadEverything()
end
