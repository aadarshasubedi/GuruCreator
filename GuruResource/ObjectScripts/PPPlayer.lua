-- The final todo list!

-- Finish out current level layout
-- Make other level layouts?
-- 5 minute timer?
-- Win case?
-- Lose case?
-- Title screen


--*******************************************
-- PPPlayer.lua
--*******************************************

-- These configuration variables might be okay as stand alone local variables in this file.
-- Then they'd be more like #defines or static consts that all player instances can use.

-- Visual placement configuration
local PathRadius = 5
local BallTime = 1.5
local PlayerScale = 2.5

-- Physical configuration
local PlayerMass = 100
local PlayerSpeed = 250
local PlayerMaxVelocity = 150	--might actually go faster than this, but it's a pretty close estimate

-- Invincible animation values
local PlayerInvincibleTime = 15
local SlowFlashSpeed = 0.35
local FastFlashSpeed = 0.15

-- Does the direction orb alternate spin directions after each press?
local AlternateSpin = false

-- Camera zoom out - velocity magnitude based
local MinZoomFactor = 2.5
local MaxZoomFactor = 6.5
local MagThreshold = 100

-- Camera zoom in - decay while no input
local CameraZoomInWaitTime = 0.75
local CameraDecayValue = 0.995
local ZoomOutSpeed = 10

-- Double tapping enabled?
local DoubleTapEnabled = false

-- Step 1 - Write a constructor, takes in an already created game object
local function Constructor( self, gameObj, posx, posy, angle )

	--in order to do this requires a set linear velocity bind.
	--requires me to set up a specific render object, like a render line or box.
	--do render box.
	print( "Constructing: PPPlayer")
	
	print( "Constructing: Physics Component")
	self.physComp = gameObj:CreatePhysicsComponent()
	print( "Finished Constructing")

	print( "Constructing: Render Component")
	self.mainRend = gameObj:CreateRenderComponent() --RCompManager:createCircleMesh( gameObj, PlayerScale, 16, GL_POLYGON )	
	print( "Finished Constructing")
	print( "Constructing: Render Component")
	self.ringRend = gameObj:CreateRenderComponent() --RCompManager:createCircleMesh( gameObj, PlayerScale * 2, 16, GL_LINE_LOOP )	
	print( "Finished Constructing")
		
	self.physComp:SetPosition(10.0, 20.0, 20.0)
	--self.mainRend:SetOffsetPos(1.0, 2.0, 2.0)
	self.mainRend:SetUseOneColor(true)
	self.ringRend:SetUseOneColor(true)
	self.ringRend:SetOneColor(1,0,1)
	
	
	--initialize the physics of the player.
	local mass = PlayerMass
	local centerX = posx
	local centerY = posy
	local linearDamp = 0.85
	local angularDamp = 1
	local angle = 0
	
	--physComp:SetBody( mass, centerX, centerY, linearDamp, angularDamp, posx, posy, angle )

	--create some physics collision data for the player.
	local radius = PlayerScale
	local density = 1
	local friction = 0.25
	local rest = 0.73		
	
	local sphereShape = nil--physComp:AddSphereShape( radius, density, friction, rest, false )	
	
	--Now create the ball.
	self.actionBall = gameObj:CreateRenderComponent()--RCompManager:createCircleMesh( gameObj, 2.0, 4, GL_POLYGON )

	-- Per instance Player Logic stuff.  If there were ever two players, we'd
	-- want them each one to have their version of this variable
	self.gameObj = gameObj
	--self.physComp = physComp
	--self.mainRend = gfxCompMain
	--self.ringRend = gfxCompRing
	
	self.timer = 0
	self.pickupCount = 0
	self.destroyedCount = 0
	self.speed = PlayerSpeed
	
	self.isFrozen = nil
	self.isInvincible = nil
	self.isShooting = nil
	self.zoomFactor = MinZoomFactor
	self.region = GameType.RED
	self.isHit = false
	
	self.objType = "Player"
	
	self:RegisterUpdateObject( )	
end

-- Step 2 - Register the object type
local PlayerClass = RegisterObjectType( Constructor, "PPPlayer" )

-- Step 3 - Add any other functions.  There are two ways to define them:
function PlayerClass:ProcessContactMessage( otherObject )
	
end


-- Scale the player.
function PlayerClass:Scale( scaleFac )
	
	self.mainRend:SetUniScaleFactor(scaleFac)
	self.ringRend:SetUniScaleFactor(scaleFac)
	self.actionBall:SetUniScaleFactor(scaleFac)
end



function PlayerClass:UpdateBallPos( elapsedTime, spinDirection )
	
	local x,y,mag = self.physComp:GetNormalVelocity()
	
	local MinSpinSpeed = 1
	local MaxSpinSpeed = 3.0
	
	local speedScalar = 1
	if mag > 25 then
		local scalar = (mag - 25) / PlayerMaxVelocity
		speedScalar = MinSpinSpeed + scalar * (MaxSpinSpeed - MinSpinSpeed)
		if speedScalar > MaxSpinSpeed then speedScalar = MaxSpinSpeed end
	end
	
	--do the same sin and cos updating on the
	self.timer = self.timer + elapsedTime * spinDirection * speedScalar 
	
	if self.timer > BallTime then
		self.timer = self.timer - BallTime
	end
	
	local scalar = self.timer / BallTime

	local angle = scalar * 360
	scalar = scalar * 2 * math.pi	
	
	local x = PathRadius * math.sin( scalar )
	local y = PathRadius * math.cos( scalar )
	
	self.actionBall:SetOffsetPos( x, y )
	self.actionBall:SetOffsetAngle( angle )
	
end

function PlayerClass:Update(  )

	local elapsedTime = 0
	local running = true
	
	local spaceTimer = 0
	local spaceLimit = 0.15
	local lastRegion = 0
	local px, py
	
	local region = 0
	local spinning = true
	local spinDirection = 1	
	
	PlayerInvincibleTime = 1.8
	
	function self:SetRegion_Thrust( )
		local c = {0.1, 0.1, 0.1}
		
		if self.region == GameType.RED then		
			c[1] = 0.8
		elseif self.region == GameType.GREEN then
			c[2] = 0.8
		else
			c[3] = 0.8
		end 
		
		--self.ringRend:SetOneColor( c[1], c[2], c[3] )
		--self.mainRend:SetOneColor( c[1], c[2], c[3] )
		--PLAYER:Scale(2.0)
	end

	function self:SetRegion_Shoot( )
		region = 1
		--self.ringRend:SetOneColor( 0.8, 0.1, 0.1 )
		--self.mainRend:SetOneColor( 0.8, 0.1, 0.1 )
	end
	
	function self:SetInvincible()
		self.isInvincible = PlayerInvincibleTime
		--self:Scale(2.0)
		if self.colorInterp ~= nil then
			--self.colorInterp:Delete()
			--self.colorInterp = nil
		end
	end

	--self:SetRegion_Thrust()
	
	--self:SetInvincible()

	local IsButtonPressed = function()
		if Input.KeyPressed( SDLK_SPACE) then
			return true
		end
		if Input.XBoxButtonPressed() then
			return true
		end
	end
	
	local IsButtonHeld = function ()
		if Input.KeyHeld( SDLK_SPACE ) then
			return true
		end
		if Input.XBoxButtonHeld() then
			return true
		end
	end

	while running do					
				
		--@todo, player logic
		local px,py = 0,0--self.physComp:GetPosition()

		--Update the rotator action ball position.
		if spinning == true then
			--self:UpdateBallPos( elapsedTime, spinDirection )
		end

		local pressed = IsButtonPressed()
		local held = IsButtonHeld()

		if self.isFrozen then
			self.isFrozen = self.isFrozen - elapsedTime
			if self.isFrozen <= 0 then
				self.isFrozen = nil
				--self:SetRegion_Thrust()
			else
				pressed = false
				held = false
				--self.mainRend:SetOneColor(0.25, 0.25, 0.25)
				--self.ringRend:SetOneColor(0.25, 0.25, 0.25)
			end
		end
		
		if self.isShooting then
			self.isShooting = self.isShooting - elapsedTime
			if self.isShooting <= 0 then
				self.isShooting = nil
				--self:SetRegion_Thrust()
			else
				--self:SetRegion_Shoot()
			end
		end
		
		
		if pressed and not held then
			-- how long has it been since we last pressed space
			-- if it's less than spaceLimit then it's a double tap
			if DoubleTapEnabled and spaceTimer < spaceLimit then
				if region == 0 then 
					SetRegion_Shoot()
				else
					SetRegion_Thrust()
				end
				print( "Double Tap" )
			end
			spaceTimer = 0
		end
		
		if pressed or held then
			-- stop spinning the orb
			spinning = false
			self.isHit = false
			if pressed and not held and AlternateSpin then
				spinDirection = spinDirection * -1
			end

			--do some thrusting
			local x,y = 0,0--self.actionBall:GetOffsetPos()
			local mag = math.sqrt(x * x + y * y)
			x = (x / mag)
			y = (y / mag)

			--self.physComp:AddImpulse( x, y, self.speed )				

		else
			spaceTimer = spaceTimer + elapsedTime
			spinning = true
		end	
		
		
		-- Change the camera's zoom level based on player speed
		local px, py = 0,0--self.physComp:GetPosition()
		local vx,vy,mag = 0,0,0--self.physComp:GetNormalVelocity()

		local newZoomFactor = 0
		
		if not pressed and not held  then
			if spaceTimer > CameraZoomInWaitTime then
				-- decay the camera's zoom Factor
				self.zoomFactor = math.pow(self.zoomFactor, CameraDecayValue)

				-- don't go below the min zoom factor
				if self.zoomFactor < MinZoomFactor then
					self.zoomFactor = MinZoomFactor
				end
			end
		else
			-- Increase the zoom value if the player is pressing space
			-- and they're moving fast enough
			if mag > MagThreshold  then
				local scalar = (mag - MagThreshold) / PlayerMaxVelocity
				if scalar > 1 then scalar = 1 end
				newZoomFactor = MinZoomFactor + scalar * (MaxZoomFactor - MinZoomFactor)
				if self.zoomFactor < newZoomFactor and self.isHit ~= true then
					self.zoomFactor = newZoomFactor
					--self.zoomFactor = self.zoomFactor + ZoomOutSpeed * elapsedTime
				end
			end
		end
		
		
		--if been hit, wait a few before you can be hit again.
		
		--print( "mag:", string.format("%.2f", mag), "desired Zoom", string.format("%.2f", newZoomFactor), "zoomFactor:", string.format("%.2f", self.zoomFactor) )
		
		Camera:SetZoomFactor2D( self.zoomFactor )

		-- Update the camera to follow the player
		--local maxLookAhead = 50
		--local lookAhead = (mag / 200) * maxLookAhead
		--Camera:SetPosition2D(px + vx * lookAhead, py + vy * lookAhead)
		Camera:SetPosition2D( px, py )
		
		elapsedTime, running = coroutine.yield()
	end
end
--RegisterUpdateFunction( PlayerClassUpdate )

