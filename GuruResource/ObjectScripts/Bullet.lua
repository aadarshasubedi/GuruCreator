
--*******************************************
-- Bullet.lua
--*******************************************

local BulletList = { }
local BulletLifeTime = 0.5

-- Step 1 - Write a constructor, takes in an already created game object
local function Constructor( self, gameObj, posx, posy, vecx, vecy, strength )

	table.insert( BulletList, self )

	--in order to do this requires a set linear velocity bind.
	--requires me to set up a specific render object, like a render line or box.
	--do render box.
	
	local physComp = gameObj:CreatePhysicsComponent()
	local gfxComp = gameObj:CreateRenderComponent()--RCompManager:createCircleMesh( gameObj, 2, 16, GL_POLYGON )
	
	self.gameObj = gameObj
	self.physComp = physComp
	self.timeToLive = BulletLifeTime
	
	local mass = 100
	local centerX = posx
	local centerY = posy
	local linearDamp = 1
	local angularDamp = 1
	local angle = 0
	
	physComp:SetBody( mass, centerX, centerY, linearDamp, angularDamp, posx, posy, angle )
	physComp:SetLinearVelocity(vecx, vecy, strength)

	local boxShape = nil--physComp:AddSphereShape( 0.4, 0.1, 0.1, 0.1, false )
	
end

-- Step 2 - Register the object type
local BulletClass = RegisterObjectType( Constructor, "Bullet" )

-- Step 3 - Add any other functions.  There are two ways to define them:
function BulletClass:ProcessContactMessage( otherObject )
	--print( "I a bullet , ", self, " am being Touched by ", otherObject )
	if otherObject ~= nil then
		for k,v in ipairs(BulletList) do
			if v == self then
				table.remove( BulletList, k )
			end
		end
		self:Delete()
	end
end

-- Define an Update - A single per frame update function for all instances of this class
local function BulletListUpdate( elapsedTime, running )

	while running do
		
		--print( "updating ALL bullets: ", table.getn(BulletList) )
		
		--check bullet list, remove out of bound ones
		for k,v in ipairs(BulletList) do
			v.timeToLive = v.timeToLive - elapsedTime
			if v.timeToLive <= 0 then
				v:Delete()
				table.remove( BulletList, k )
			end
		end
		elapsedTime, running = coroutine.yield()
	end

end

RegisterUpdateFunction( BulletListUpdate )



