

--*******************************************
-- Wall.lua
--*******************************************

-- Step 1 - Write a constructor, takes in an already created game object
local function Constructor( self, gameObj, posX, posY, width, height )

	local physComp = gameObj:CreatePhysicsComponent()
	
	local mass = 0
	local centerX = posX
	local centerY = posY
	local linearDamp = 1
	local angularDamp = 1
	local angle = 0

	physComp:SetBody( mass, centerX, centerY, linearDamp, angularDamp, posX, posY, angle )
	
	local sphereShape = physComp:AddBoxShape( width, height, 100, 1, 1, false )
	
	local gfxComp = gameObj:CreateRenderComponent()--RCompManager:createBoxMesh( gameObj, width, height, GL_POLYGON )
	self.gfxComp = gfxComp
	
end

-- Step 2 - Register the object type
local WallClass = RegisterObjectType( Constructor, "Wall" )


-- Step 3 - Add any other functions.  There are two ways to define them:
function WallClass:ProcessContactMessage( otherObject )
	--print( "I a Wall, ", self, " am being Touched by ", otherObject )
end
