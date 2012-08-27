-- Triple Trouble is a fun and fast game where you use strategic steering skills to bounce into and destroy the enemies.
-- You're direction rotates automatically.  Press the button to go forward - then drift, bounce, or turn on a dime to 
--steer your way through the level and grab powerups that change your color.
-- Hit enemies of the same color, and they'll drop collectables after being hit 3 times!  
-- But watch-out, if your colors don't match then they'll regain health.
-- Grab as many collectables within 5 minutes!

PLAYER = nil
GameType = { RED = 1, GREEN = 2, BLUE = 3  }
BGColorStart = { 0.45, 0.55, 0.55, 0 }
BGColorEnd = { 0.05, 0.15, 0.15, 0 }

objectList = {}

print( "\nFirstWizardScript Is Running!\n" )

WorldScene:SetClearColor( unpack(BGColorStart) )


function dofileparam (filename, ...)
	local f = assert(loadfile(filename))
	return f(...)
end

-- using dofile instead of ScriptGuru:CreateImmediateDisciple gives us
-- better debugging information.  not sure why, probably because the script
-- gets loaded in this script state, and not in a seperate one.  so if there is 
-- an error while updating this script but the problem is from one of these loaded
-- files, the error stuff gets confused unless it was loaded into the same state!

function LoadFiles()
	dofile( "./resource/RenderDefines.lua" ) 
	dofile( "./resource/InputDefines.lua" )
	dofile( "./resource/ObjectTypeManager.lua" )
	dofile( "./resource/ObjectScripts/Interpolator.lua")
	dofile( "./resource/ObjectScripts/PPPlayer.lua")
	dofile( "./resource/Level2.lua")
end		



function FirstOne()

	print( "Running" )
	local running = true
	local eTime = 0.0
	local tTime = 0.0

	LoadFiles()
	CreateLevel()
	
	local currTarget = 0
	
	--Scene.TestFunction( "Libary totally works" )
	--Object.TestLog( FirstObject, "Object library says object created in lua." )

	local spaceTimer = 0
	local spaceLimit = 0.75
	local lastRegion = 0
	local spinChange = false
	local gameTimer = 0
	local timingGame = true
	local goal = 30
	
	while running == true do
		gameTimer = gameTimer + eTime
		
		if( Input.KeyPressed( SDLK_c )) then
			local x2, y2, z = Camera:GetPosition2D()
			--print( "Camera: ", x2, y2, z )
		end
		
		if PLAYER and PLAYER.pickupCount >= goal and timingGame == true then
			--print( "You win: ", string.format("%.4f", gameTimer) )
			timingGame = false
		end

		if( Input.KeyPressed( SDLK_r )) or Input.XBoxBackPressed() then
			CleanupGame()
			LoadFiles()
			CreateLevel()
		end

		UpdateUpdateFunctions( eTime )
		
		eTime = Scene.WaitFrame(1)
		
		ProcessContactMessages()
	end

	--The coroutine has finished running, so clean up anything necessary here and let the
	-- function exit on its own.
end

FirstOne()



--O(n^n) unpack everything from a table
local function unpackall(t)
	local checkedList = { }
	
	local function unpack_work()
	
		for k,v in pairs(t) do
			if checkedList[k] == nil then
				checkedList[k] = true
				return v, unpack_work()
			end
		end
	end
	
	return unpack_work()
end