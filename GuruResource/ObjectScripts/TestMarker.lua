local TestMarkerAliveList = { }
local TestMarkerDeadList = { }

local GameTotalTime = 5 * 60

local CheckWinCase = function()
	
	local totalTime = 0	
	
	while true do
		elapsedTime, running = coroutine.yield( )
		
		-- check for end game by time's up
		if totalTime >= GameTotalTime then
			print( "Time's up.  You got " .. PLAYER.pickupCount .. " pickups" )
			PLAYER:Delete()
			return
		end

		local scalar = 1 - (totalTime / GameTotalTime)
		local r = BGColorEnd[1] + scalar * (BGColorStart[1] - BGColorEnd[1])
		local g = BGColorEnd[2] + scalar * (BGColorStart[2] - BGColorEnd[2])
		local b = BGColorEnd[3] + scalar * (BGColorStart[3] - BGColorEnd[3])
		WorldScene:SetClearColor( r, g, b, 0 )

		totalTime = totalTime + elapsedTime
	end
end
RegisterUpdateFunction( CheckWinCase )

--*******************************************
-- TestMarker.lua
--*******************************************
local TrackDist = 25000
local speed = 8500
local HitbackSpeed = 9000
local SelfFreezeTime = 3
local OtherFreezeTime = 2
local EyeScale = 2.5
local BoxScale = 10
local PickupExplodeSpeed = 350
local DeathCountdown = 2.5
local RespawnCountdown = 30

-- Step 1 - Write a constructor, takes in an already created game object
local function Constructor( self, gameObj, posX, posY, angle, type )
	
	print("-Constructing: Test Marker-")
	
	print("--Constructing: Physics Comopnent--")
	local physComp = gameObj:CreatePhysicsComponent()
	self.physComp = physComp
	
	---[[
	local gfxCompEyes = {gameObj:CreateRenderComponent(),gameObj:CreateRenderComponent(),gameObj:CreateRenderComponent()}
			
	print("--Constructing: Render Comopnent--")
	--gfxCompEyes[1] = gameObj:CreateRenderComponent()
	--if gfxCompEyes[1] ~= nil then
		--print("---Setting Offset Postion---")
		gfxCompEyes[1]:SetOffsetPos( 5.0, 4.0, 0.0 )
		--print("---Setting One Color Use---")
		--gfxCompEyes[1]:SetUseOneColor(true)
	--end
	print("--Finished Constructing--")
	print("--Constructing: Render Comopnent--")
	--gfxCompEyes[2] = gameObj:CreateRenderComponent()
	if gfxCompEyes[2] ~= nil then
		print("---Setting Offset Postion---")
		--gfxCompEyes[2]:SetOffsetPos( 5, 4, 0 )
		print("---Setting One Color Use---")
		--gfxCompEyes[2]:SetUseOneColor(true)
	end
	print("--Finished Constructing--")
	
	print("--Constructing: Render Comopnent--")
	--gfxCompEyes[3] = gameObj:CreateRenderComponent()
	if gfxCompEyes[3] ~= nil then
		print("---Setting Offset Postion---")
		--gfxCompEyes[3]:SetOffsetPos( 5, 4, 0 )
		print("---Setting One Color Use---")
		--gfxCompEyes[3]:SetUseOneColor(true)
	end
	print("--Finished Constructing--")
	
	gfxCompEyes[1]:SetOneColor(self.eyeColor[1], self.eyeColor[2], self.eyeColor[3])
	gfxCompEyes[2]:SetOneColor(self.eyeColor[1], self.eyeColor[2], self.eyeColor[3])		
	gfxCompEyes[3]:SetOneColor(self.eyeColor[1], self.eyeColor[2], self.eyeColor[3])		
	--]]
		
	self.gfxCompEyes = gfxCompEyes
	gfxCompEyes = nil
	self.health = 3
	
	-- this creates the triangle shape until createTriangleMesh() works
	
	local gfxComp = gameObj:CreateRenderComponent()--RCompManager:createBoxMesh( gameObj, BoxScale, BoxScale, GL_LINES )	
	gfxComp:SetUseOneColor(true)	
	gfxComp:SetOneColor(0.3, 0.1, 0.1)
	
	self.gfxComp = gfxComp
	local sphereShape = nil --physComp:AddBoxShape( radius, radius, density, friction, restitution, isSensor )
	
	self.objType = "enemy"
	self.RegisterUpdateObject( )
end

-- Step 2 - Register the object type
local TestMarkerClass = RegisterObjectType( Constructor, "TestMarker" )


function TestMarkerClass:Explode()
	local px,py = self.physComp:GetPosition()
	table.insert( TestMarkerDeadList, self )
	self:Delete()
	
	for j=1,10 do
		local fx = 1 - math.random() * 2
		local fy = 1 - math.random() * 2
		local newPickup = CreateNewObject( "Pickup", px + fx * BoxScale * 2, py + fy * BoxScale * 2, 1.5)
		newPickup.physComp:AddImpulse(fx, fy, PickupExplodeSpeed)
	end
end

function TestMarkerClass:InitiateRespawn()
	local RespawnCountdownFunc = function()
		local respawnCountdown = RespawnCountdown
		while 1 do
			local elapsedTime = coroutine.yield()
			respawnCountdown = respawnCountdown - elapsedTime
			if respawnCountdown <= 0 then
				CreateNewObject( "TestMarker", self.startX, self.startY, 0)
				return
			end
		end
	end
	RegisterUpdateFunction( RespawnCountdownFunc )
end

-- Step 3 - Add any other functions.  There are two ways to define them:
function TestMarkerClass:ProcessContactMessage( otherObject, position, normal )

	if otherObject == PLAYER and self.active == true then
		local px,py = PLAYER.physComp:GetPosition()
		local x,y = self.physComp:GetPosition()
		local distX = px - x
		local distY = py - y
		local magSqrd = (distX * distX) + (distY * distY)
	
		local d = math.sqrt(magSqrd)
		distX = distX / d
		distY = distY / d
		
		-- turn eyes grey
		for i=1,3 do
			self.gfxCompEyes[i]:SetOneColor(0.45,0.45,0.45)
		end
			
		-- deactivate for a bit
		self.active = false
		self.activateTimer = SelfFreezeTime			
		self.physComp:SetLinearVelocity(0,0)
			
		if PLAYER.region ~= self.type or PLAYER.isFrozen ~= nil then
			-- deactivate for a bit
			self.active = false
			self.activateTimer = SelfFreezeTime			
			self.physComp:SetLinearVelocity(0,0)
			
			otherObject.isHit = true
			otherObject.isFrozen = OtherFreezeTime
			otherObject.physComp:AddImpulse( distX, distY, HitbackSpeed )
			if self.health < 3 then
				self.health = self.health + 1
				self.gfxCompEyes[self.health]:IsVisible(true)
			end
		else
			
			self.physComp:AddImpulse( distX, distY, -HitbackSpeed )
			
			self.gfxCompEyes[self.health]:IsVisible(false)
			self.health = self.health - 1
			if self.health == 0 then
				local DieAnimation = function()
					local deathCountdown = DeathCountdown
					local spinAngle = 0
					while 1 do
						local elapsedTime = coroutine.yield()
						if self.health == 0 then
							deathCountdown = deathCountdown - elapsedTime
							if deathCountdown <= 0 then
								self:Explode()
								self:InitiateRespawn()
								return
							else
								spinAngle = spinAngle + elapsedTime
								if spinAngle > 1 then spinAngle = spinAngle - 1 end
								self.gfxComp:SetOffsetAngle( spinAngle * 360 )
							end
						end
					end
				end
				RegisterUpdateFunction( DieAnimation )
				UnRegisterUpdateObject( self )
			end
		end
	end
end

function TestMarkerClass:TrackPlayer()	
	local px,py = PLAYER.physComp:GetPosition()
	local x,y = self.physComp:GetPosition()
	
	local distX = px - x
	local distY = py - y
	local magSqrd = (distX * distX) + (distY * distY)

	if magSqrd < TrackDist then
	
		local d = math.sqrt(magSqrd)
		distX = distX / d
		distY = distY / d
		
		if self.type == PLAYER.region then
			self.physComp:AddForce( distX, distY, -speed )
			for i=1,3 do		
				self.gfxCompEyes[i]:SetOneColor(1, 1, 0)
			end
		else
			self.physComp:AddForce( distX, distY, speed )
			for i=1,3 do		
				self.gfxCompEyes[i]:SetOneColor(self.eyeColor[1], self.eyeColor[2], self.eyeColor[3])
			end
		end
	else
		for i=1,3 do
			self.gfxCompEyes[i]:SetOneColor(0.5,0.5,0.5)
		end
	end
end

function TestMarkerClass:TripOut( elapsedTime )
	--do the same sin and cos updating on the
	self.timer = self.timer + elapsedTime
	local scalar = self.timer / self.spinTime
	scalar = scalar * 2 * 3.141597

	local x = 10 * math.sin( scalar  )
	local y = 10 * math.cos( scalar  )

	self.gfxComp:SetOffsetPos( x, y )
end
		
function TestMarkerClass:Update()

	local elapsedTime = 0
	local running = true
	local angle = 0
	local rotTime = 2.0
	local curTime = 0
	self.active = true
	
	while running do		
	
		if self.active == true then
			self:TrackPlayer()	
		else
			if self.activateTimer ~= nil then
				self.activateTimer = self.activateTimer - elapsedTime
				if self.activateTimer <= 0 then
					self.active = true
					self.activateTimer = nil
					for i=1,3 do		
						self.gfxCompEyes[i]:SetOneColor(self.eyeColor[1], self.eyeColor[2], self.eyeColor[3])
					end
				end
			end
			

		end

		elapsedTime, running = coroutine.yield( )
	end
end
