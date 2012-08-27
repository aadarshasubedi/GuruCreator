

--*******************************************
-- Pickup.lua
--*******************************************
local TrackDist = 2250
local SpeedScalar = 15
	
-- Step 1 - Write a constructor, takes in an already created game object
local function Constructor( self, gameObj, posX, posY, angle, type )

	local scale = 2.5
	
	local physComp = gameObj:CreatePhysicsComponent()
	self.physComp = physComp

	local gfxComp2 = gameObj:CreateRenderComponent()--RCompManager:createCircleMesh( gameObj, scale, 4, GL_POLYGON )
	gfxComp2:SetOffsetPos( scale, scale )
	self.gfxComp2 = gfxComp2

	local isSensor = false

	local gfxComp = gameObj:CreateRenderComponent()--RCompManager:createCircleMesh( gameObj, scale, 4, GL_POLYGON )
	gfxComp2:SetOffsetPos( -scale, -scale )
	self.gfxComp = gfxComp

	gfxComp:SetUseOneColor(true)
	gfxComp:SetOneColor(0,0,1)
	gfxComp2:SetUseOneColor(true)
	gfxComp2:SetOneColor(0,0,1)
	
	local mass = 25
	local centerX = posX
	local centerY = posY
	local linearDamp = 0.75
	local angularDamp = 1
	local angle = 0
	
	physComp:SetBody( mass, centerX, centerY, linearDamp, angularDamp, posX, posY, angle )

	self.radius = 10
	self.density = 1.0
	self.friction = 0.3
	self.restiution = 0.85
	local isSensor = false
	local sphereShape = nil--physComp:AddSphereShape( self.radius, self.density, self.friction, self.restitution, isSensor )
	
	self.timer = 0
	self.spinTime = 2
	self.magScalar = SpeedScalar
	self.TrackDist = TrackDist
	
	self.RegisterUpdateObject( )
end

-- Step 2 - Register the object type
local PickupClass = RegisterObjectType( Constructor, "Pickup" )


-- Step 3 - Add any other functions.  There are two ways to define them:
function PickupClass:ProcessContactMessage( otherObject )
	
	if otherObject == PLAYER then
		self:Delete()
		
		if otherObject == PLAYER then
			PLAYER.pickupCount = PLAYER.pickupCount + 1
		else
			PLAYER.destroyedCount = PLAYER.destroyedCount + 1
		end
		print( "pickup/destroyed = " .. tostring(PLAYER.pickupCount) .."/" ..tostring(PLAYER.destroyedCount) )

		UnRegisterUpdateObject(self)
	end
end

function PickupClass:TrackPlayer()	
	local px,py = PLAYER.physComp:GetPosition()
	local x,y = self.physComp:GetPosition()
	
	local distX = px - x
	local distY = py - y
	local magSqrd = (distX * distX) + (distY * distY)
	if magSqrd < self.TrackDist or self.isTrackingPlayer == true then
		if self.isTrackingPlayer ~= true then
			self.isTrackingPlayer = true
			self.physComp:DeleteShapes()
			local sphereShape = self.physComp:AddSphereShape( self.radius, self.density, self.friction, self.restitution, true )
		end

		local d = math.sqrt(magSqrd)
		distX = distX / d
		distY = distY / d
		--self.physComp:AddImpulse( distX, distY, speed )
		self.physComp:AddForce( distX, distY, magSqrd * self.magScalar )
	end
end

function PickupClass:TripOut( elapsedTime )
	--do the same sin and cos updating on the
	self.timer = self.timer + elapsedTime
	local scalar = self.timer / self.spinTime
	local rangle = scalar * 2 * 3.141597

	local x = 2 * math.sin( rangle  )
	local y = 2 * math.cos( rangle  )

	self.gfxComp:SetOffsetPos( x, y )
	self.gfxComp2:SetOffsetPos( -x, -y )
	self.gfxComp:SetOffsetAngle( scalar * 360 )
	self.gfxComp2:SetOffsetAngle( scalar * 360 )
end
		
function PickupClass:Update()

	local elapsedTime = 0
	local running = true
	
	while running do
		
		self:TripOut( elapsedTime )
		
		self:TrackPlayer()	

		elapsedTime, running = coroutine.yield( )
	end
end
