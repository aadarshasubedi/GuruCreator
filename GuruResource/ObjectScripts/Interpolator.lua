
InterpolationType = { Linear = 0, QuadraticIn = 1, QuadraticOut = 2, QuadraticS = 3, Custom = 4 }

-- Linear interpolation
local function LinearInterp( t )
	return t
end

-- Quadratic-in interpolation
local function QuadInInterp( t )
	return t * t
end

-- Quadratic-out interpolation
local function QuadOutInterp( t )
	return 1 - ((1 - t) * (1 - t))
end

-- S-curve interpolation
local function QuadSInterp( t )
	return 3.0 * (t * t) - 2.0 * (t * t * t)
end


local function Constructor( self, entity, beginVal, endVal, time, typeOrFunc )
	
	self:ClearAndReset(beginVal, endVal, time)
	
	self.AdjustTimeFunc = LinearInterp
	if typeOrFunc == InterpolationType.Linear then
		self.AdjustTimeFunc = LinearInterp
	elseif typeOrFunc == InterpolationType.QuadraticIn then
		self.AdjustTimeFunc = QuadInInterp
	elseif typeOrFunc == InterpolationType.QuadraticOut then
		self.AdjustTimeFunc = QuadOutInterp
	elseif typeOrFunc == InterpolationType.QuadraticS then
		self.AdjustTimeFunc = QuadSInterp
	else
		self.AdjustTimeFunc = typeOrFunc
	end
end

local NewClassDef = RegisterObjectType( Constructor, "Interpolator" )

-- Resets the state of the interpolator object
function NewClassDef:ClearAndReset( beginVal, endVal, time )
	self.beginVal = beginVal
	self.currentValue = beginVal
	self.endVal = endVal
	self.endTime = time
	self.time = 0
	self.active = true
end

function NewClassDef:Reset( )
	self.currentValue = self.beginVal
	self.time = 0
	self.active = true
end

-- Resets the state of the interpolator object to the opposite of what it currently is
function NewClassDef:SwitchAndReset(  )
	local temp = self.beginVal
	self.beginVal = self.endVal
	self.endVal = temp
	
	self.currentValue = beginVal

	self.time = 0
	self.active = true

	if self.AdjustTimeFunc == InterpolationType.QuadraticOut then
		self.AdjustTimeFunc = InterpolationType.QuadraticIn
	elseif self.AdjustTimeFunc == InterpolationType.QuadraticIn then
		self.AdjustTimeFunc = InterpolationType.QuadraticOut
	end
end

function NewClassDef:Update( dt )
	if self.active == true then	
		
		self.time = self.time + dt
		if self.time >= self.endTime then
			self.active = false
			self.time = self.endTime
		end
	
		-- Get the adjusted time
		local t = self.AdjustTimeFunc( self.time / self.endTime )
		self.currentValue = self.beginVal + (self.endVal - self.beginVal) * t
	end
	return self.currentValue
end
