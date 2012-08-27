-- This function takes in a constructor, and returns a class definition
-- that can be used to create new objects that have the same metatable
local function CreateClass( ctor, typename )

	local newClassDefinition = { }
	
	-- [] or . operator
	newClassDefinition.__index = newClassDefinition

	newClassDefinition.typename = typename
	newClassDefinition.objectCount = 0
	
	local mt = { }

	-- overload this table's () operator
	-- hereby refered to as "functables"
	mt.__call = function( class_tbl, gameObj, ... )
	
		-- Wrapping the object in a table here allows us to have two levels of metadata
		-- 1) This obj is a table in lua, so it can have new functions and variables added willy-nilly
		-- and it will have the metatable of this class, so any instance of it can use the functions along
		-- with the type definition.
		-- 2) The gameObj is the C++ GameObject, and it has the metatable assigned by Lunar, and thus
		-- access to the GameObject glue function library

		local obj = { }
		setmetatable( obj, newClassDefinition )
		obj.parentObj = gameObj
		obj.isDead = false
		
		obj.RegisterUpdateObject = function()
			RegisterUpdateObject(obj)
			obj.needToUnregister = true
		end

		newClassDefinition.objectCount = newClassDefinition.objectCount + 1
		
		if ctor then
			ctor( obj, gameObj, ... )
		end


		obj.Delete = function()
			if obj.parentObj ~= nil then
				--WorldScene:DeleteGameObject( obj.parentObj )
				obj.parentObj = nil
				obj.isDead = true
				newClassDefinition.objectCount = newClassDefinition.objectCount - 1
			end
			
			if obj.needToUnregister == true then
				--UnRegisterUpdateObject(obj)
			end
		end
		
		return obj
	end
	
  setmetatable( newClassDefinition, mt )

  return newClassDefinition
end

--*********************************************************
--*********         Global Functions        ***************
--*********************************************************
local ClassConstructorList = { }

function RegisterObjectType( ctor, type )
	local newClass = CreateClass(ctor, type)
	ClassConstructorList[type] = newClass
	print( "Creating new class: " .. tostring(newClass) .. " and registering it for object type: " .. tostring(type) )
	return newClass
end

function CreateNewObject( type, ... )

	print( "== Creating new Game Object Type ==")
	print( "Type: ", type )
	
	local factoryFunc = ClassConstructorList[ type ]
	print( "factoryFunc/class_tbl: ", factoryFunc )
	if factoryFunc ~= nil then
	
		-- Step 1 - Create a C++ game object
		print( "== Calling World Scene to Create ==")
		local gameObj = WorldScene:CreateGameObject()
		print( "Creating gameObj: ", gameObj )
	
		-- Step 2 - Configure it using the Lua game object definitions
		local luaObj = factoryFunc( gameObj, ... )
		print( "Final luaObj: ", luaObj )
	
		-- Step 3 - Save a reference to the Lua object in the Game Object
		-- @TODO, for easier handling of contacts
		gameObj:SetScriptObjRef( luaObj )
		
		return luaObj
	else
		print( "no factory function for this object type: ", type, factoryFunc )
	end
	
	return nil
end


function ProcessContactMessages()

	local contactList = { --[[WorldScene:GetPhysicsContacts()--]] }
	
	for k,v in ipairs(contactList) do
		if v.obj1 ~= nil then
			if v.obj1.ProcessContactMessage ~= nil then
				v.obj1:ProcessContactMessage( v.obj2, v.position, v.normal, v.type )
			end
		end
		if v.obj2 ~= nil then		
			if v.obj2.ProcessContactMessage ~= nil then
				v.obj2:ProcessContactMessage( v.obj1, v.position, v.normal, v.type )
			end
		end
	end
end


local UpdateFunctionList = { }


function RegisterUpdateFunction( updateFunc )

	local newRoutine = coroutine.create( updateFunc )
	table.insert( UpdateFunctionList, newRoutine )
end

local UpdateObjectList = { }
function RegisterUpdateObject( t )
	local newRoutine = coroutine.create( function() t:Update() end )
	UpdateObjectList[t] = newRoutine
end

local UpdatingObjectList = false
local DeleteList = { }
function UnRegisterUpdateObject( t )
	if UpdatingObjectList == false then
		UpdateObjectList[t] = nil
	else
		table.insert(DeleteList, t )
	end
end

function UpdateUpdateFunctions( elapsedTime )

	for k,v in ipairs( UpdateFunctionList ) do
		if coroutine.status(v) == "dead" then
			table.remove( UpdateFunctionList, k )
		else
			local success, error = coroutine.resume( v, elapsedTime, true )
			if success == false then
				print( "Error resuming coroutine: " .. tostring(error) )
				print( debug.traceback() )
				table.remove( UpdateFunctionList, k )
			end
		end
	end

	UpdatingObjectList = true
	
	for k,v in pairs( UpdateObjectList ) do
		if coroutine.status(v) == "dead" then
			UpdateObjectList[k] = nil
		else
			local success, error = coroutine.resume( v, elapsedTime, true )
			if success == false then
				print( "Error resuming coroutine: " .. tostring(error) )
				print( debug.traceback() )
				UpdateObjectList[k] = nil
			end
		end
	end

	UpdatingObjectList = false
	if table.getn(DeleteList) > 0 then
		for k,v in pairs(DeleteList) do
			UpdateObjectList[k] = nil
		end
		DeleteList = { }
	end
end

-- do not call this during a coroutine's update
function UnloadEverything()
	UpdateFunctionList = { }
	UpdateObjectList = { }
end