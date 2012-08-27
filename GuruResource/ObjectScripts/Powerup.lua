

--*******************************************
-- Powerup.lua
--*******************************************
local AnimTime = 0.5
local SignalTime = 1
local RespawnTime = 30
local BombDropoffTime = 10

--1 - red
--2 - blue
--3 - yellow

-- Step 1 - Write a constructor, takes in an already created game object
local function Constructor( self, gameObj, posX, posY, angle, type )

	local scale = 2
	
	local physComp = gameObj:CreatePhysicsComponent()
	self.physComp = physComp

	self.ringList = { }
	local center = { }
	center.gfxComp = RCompManager:createCircleMesh( gameObj, scale, 16, GL_POLYGON )
	center.gfxComp:SetUseOneColor(true)
	center.gfxComp:SetOneColor(0,0,0)
	center.colorInterp = CreateNewObject( "Interpolator", 0, 1, SignalTime, InterpolationType.QuadraticS )
	center.colorInterp.time = 0.675
	table.insert( self.ringList, center )

	for i=3,6 do
		local newRing = { }
		newRing.gfxComp = RCompManager:createCircleMesh( gameObj, scale * (i * 0.5), 16, GL_LINE_LOOP )
		newRing.gfxComp:SetUseOneColor(true)
		newRing.gfxComp:SetOneColor(0,0,0)
		newRing.colorInterp = CreateNewObject( "Interpolator", 0, 1, SignalTime, InterpolationType.QuadraticS )
		newRing.colorInterp.time = AnimTime - ((i-3) / 4) * AnimTime
		table.insert( self.ringList, newRing )
	end
	
	local mass = 5
	local centerX = posX
	local centerY = posY
	local linearDamp = 0.75
	local angularDamp = 1
	local angle = 0
	local isSensor = true
	
	physComp:SetBody( mass, centerX, centerY, linearDamp, angularDamp, posX, posY, angle )

	local radius = 12
	local density = 1.0
	local friction = 0.3
	local restiution = 0.85
	local sphereShape = physComp:AddSphereShape( radius, density, friction, restitution, isSensor )
	
	self.timer = 0
	self.spinTime = 2
	self.magScalar = 10
	self.TrackDist = 2250
	if type ~= nil then
		self.type = type
	else
		self.type = math.random(1,3)
	end
	
	self.posX = posX
	self.posY = posY
	
	self.objType = "PowerUp"
	self.pickedUp = false
	
	self.RegisterUpdateObject( )
	

end

-- Step 2 - Register the object type
local NewClassDef = RegisterObjectType( Constructor, "Powerup" )


-- Step 3 - Add any other functions.  There are two ways to define them:
function NewClassDef:ProcessContactMessage( otherObject )
	
	if otherObject == PLAYER then
		if self.type == GameType.RED then
			PLAYER.region = self.type
			PLAYER:SetRegion_Thrust()
			self:Delete()
		elseif self.type == GameType.GREEN then
			PLAYER.region = self.type
			PLAYER:SetRegion_Thrust()
			self:Delete()
		elseif self.type == GameType.BLUE then
			PLAYER.region = self.type
			PLAYER:SetRegion_Thrust()
			self:Delete()
		elseif self.type == 4 then
			--self.gfxComp:SetOneColor(1,1,1)

			if self.pickedUp == false then
				self.pickedUp = true
				RegisterUpdateFunction( function()
					local waitTime = BombDropoffTime
					local running = true
					while running do
						local dt = coroutine.yield()
						local vx,vy,mag = otherObject.physComp:GetNormalVelocity()
						local px,py = otherObject.physComp:GetPosition()
						
						--@todo, check player velocity to dropoff
						--@todo, explode, create some other entities, damage others around this
						self.physComp:SetPosition( px - vx * 10, py - vy * 10 )
						waitTime = waitTime - dt
						if waitTime <= 0 then
							
							print("explode")
							self:Delete()
							running = false
						end
					end
				end )
			end
		end
		
		RegisterUpdateFunction( function()
			local waitTime = RespawnTime
			while 1 do
				local dt = coroutine.yield()
				waitTime = waitTime - dt
				if waitTime <= 0 then
					CreateNewObject( "Powerup", self.posX, self.posY, 0, self.type)
					return
				end
			end
		end )
	end
end

function NewClassDef:UpdateAnimation( elapsedTime )

	for k,v in ipairs(self.ringList) do
	
		local colorVal = v.colorInterp:Update( elapsedTime )
		if v.colorInterp.active == false then
			v.colorInterp:SwitchAndReset()
		end
		
		v.gfxComp:SetUniScaleFactor(1 + colorVal)
		
		if self.type == 1 then	
			v.gfxComp:SetOneColor(colorVal,0,0)
		elseif self.type == 2 then	
			v.gfxComp:SetOneColor(0,colorVal,0)
		elseif self.type == 3 then	
			v.gfxComp:SetOneColor(0,0,colorVal)
		elseif self.type == 4 then	
			v.gfxComp:SetOneColor(colorVal,colorVal,0)
		end
	end
end
		
function NewClassDef:Update()

	local elapsedTime = 0
	local running = true
	
	while running do
		self:UpdateAnimation( elapsedTime )

		elapsedTime, running = coroutine.yield( )
	end
end
