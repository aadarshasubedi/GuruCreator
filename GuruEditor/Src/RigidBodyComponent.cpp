/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       RigidBodyComponent.cpp
Brief:      RigidBodyComponent class implementation.
/************************************************************************/

#include "RigidBodyDescriptor.h"
#include "RigidBodyComponent.h"
#include "PhysicsComponent.h"
#include "Game_Engine.h"
#include "Primitives.h"


RigidBodyComponent::RigidBodyComponent() : ComponentDisciple(), mPhysicsData(NULL)
{
}


RigidBodyComponent::~RigidBodyComponent()
{
	delete mPhysicsData;
}


GameObject* RigidBodyComponent::GetParentObj() 
{ 
	return (GameObject*)mParentRef;
}


#ifdef USE_BOX2D
b2Vec2	RigidBodyComponent::GetPosition()
#else
CVector3 RigidBodyComponent::GetPosition()
#endif
{	
#ifndef USE_BOX2D
	return mPhysicsData->GetPosition();
#else
	return mBodyRef->GetPosition();
#endif
}


#ifdef USE_BOX2D
void	RigidBodyComponent::SetPosition(const b2Vec2& pos, float angle)
#else
void	RigidBodyComponent::SetPosition(const CVector3& pos, float angle)
#endif
{	
#ifdef USE_BOX2D
	mBodyRef->SetXForm( pos, angle );
#else
	//@TODO:  Implement for the base system without Box2D
	CVector3 temp = pos;
	temp += CVector3(0,0,0) * angle;
#endif
}



#ifdef USE_BOX2D
void	RigidBodyComponent::SetBody(b2BodyDef* bodyDef)
#else
void	RigidBodyComponent::SetBody(void* bodyDef)
#endif
{
#ifdef USE_BOX2D
	bodyDef->userData = this;
	mBodyRef = ((Game_Engine*)Game_Engine::GetSingletonPtr())->mWorldScene->mPhysicsWorld_Box2d->CreateBody( bodyDef );	
#else
	//@TODO: Change this to set my physics rigid body with the descriptor.
	bodyDef = NULL;
#endif
}


void RigidBodyComponent::SetMass(float mass)
{
#ifdef USE_BOX2D
	b2MassData massDat;
	massDat.mass = mass;
	mBodyRef->SetMass(&massDat);
#else
	mMass = mass;
#endif
}


void	RigidBodyComponent::SetMassFromShapes()
{
#ifdef USE_BOX2D
	mBodyRef->SetMassFromShapes();
#else
	//@TODO:  Implement for the base system without Box2D
#endif
}


//These functions adjust the mass values to set an object as dynamic or not.
void	RigidBodyComponent::SetDynamic()
{
	SetMassFromShapes();
}


void	RigidBodyComponent::SetStatic()
{
	SetMass(0.0f);
}


#ifdef USE_BOX2D
void	RigidBodyComponent::AddImpulse( const b2Vec2& impulse, float strength )
#else
void	RigidBodyComponent::AddImpulse(const CVector3& impulse, float strength)
#endif
{
#ifdef USE_BOX2D
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyImpulse( strength * impulse, mBodyRef->GetPosition() );
	}
#else
	//@TODO:  Implement for the base system without Box2D
	CVector3 temp = impulse;
	temp += CVector3(0,0,0) * strength;
#endif
}


#ifdef USE_BOX2D
void	RigidBodyComponent::AddForce( const b2Vec2& force, float strength )
#else
void	RigidBodyComponent::AddForce(const CVector3& force, float strength )
#endif
{
#ifdef USE_BOX2D
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyForce( strength * force, mBodyRef->GetPosition() );
	}
#else
	//@TODO:  Implement for the base system without Box2D
	CVector3 temp = force;
	temp += CVector3(0,0,0) * strength;
#endif
}


#ifdef USE_BOX2D
void	RigidBodyComponent::AddImpulseAtPoint( const b2Vec2& impulse, const b2Vec2& applyPoint, float strength )
#else
void	RigidBodyComponent::AddImpulseAtPoint(const CVector3& impulse, const Point& applyPoint, float strength)
#endif
{
#ifdef USE_BOX2D
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyImpulse( strength * impulse, applyPoint );
	}
#else
//@TODO:  Implement for the base system without Box2D
	Point newPos = applyPoint + impulse;
	newPos = newPos + CVector3(0,0,0) * strength;
#endif
}


#ifdef USE_BOX2D
void	RigidBodyComponent::AddForceAtPoint( const b2Vec2& force, const b2Vec2& applyPoint, float strength  )
#else
void	RigidBodyComponent::AddForceAtPoint(const CVector3& force, const Point& applyPoint, float strength)
#endif
{
#ifdef USE_BOX2D
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyForce( strength * force, applyPoint );
	}
#else
	//@TODO:  Implement for the base system without Box2D
	Point newPos = applyPoint + force;
	newPos = newPos + CVector3(0,0,0) * strength;
#endif
}

#ifdef USE_BOX2D
void	RigidBodyComponent::SetLinearVelocity( const b2Vec2& velocity, float strength )
#else
void	RigidBodyComponent::SetLinearVelocity(const CVector3& velocity, float strength)
#endif
{
#ifdef USE_BOX2D
	mBodyRef->SetLinearVelocity( strength * velocity );
#else
	//@TODO:  Implement for the base system without Box2D
	CVector3 temp = velocity;
	temp += CVector3(0,0,0) * strength;
#endif
}


#ifdef USE_BOX2D
b2Vec2	RigidBodyComponent::GetLinearVelocity()
#else
CVector3 RigidBodyComponent::GetLinearVelocity()
#endif
{	
#ifndef USE_BOX2D
	//@TODO: This needs to be velocity.
	return mPhysicsData->GetPosition();
#else
	return mBodyRef->GetLinearVelocity();
#endif
}


//Delete shapes from the body.  For now just delete all of them.
void	RigidBodyComponent::DeleteShapes()
{
#ifdef USE_BOX2D
	b2Shape* shapeList = mBodyRef->GetShapeList();
	while( shapeList != NULL )
	{
		b2Shape* nextShape = shapeList->GetNext();
		DeleteShape( shapeList );
		shapeList = nextShape;
	}
#else
//@TODO:  Implement for the base system without Box2D
#endif
}


#ifdef USE_BOX2D
void	RigidBodyComponent::DeleteShape( b2Shape* shape )
#else
void	RigidBodyComponent::DeleteShape( void* shape )
#endif
{

#ifdef USE_BOX2D
	if( mBodyRef != NULL )
	{
		mBodyRef->DestroyShape( shape );
	}
#else
	//@TODO:  Implement for the base system without Box2D
	shape = NULL;
#endif
}

/*************************************************************************
 
*************************************************************************/
#ifdef USE_LUA
LUA_START_DEFINE_CLASS( RigidBodyComponent )
	LUA_NAME_METHOD( RigidBodyComponent, "AddSphereShape", LAddSphereShape )
	LUA_NAME_METHOD( RigidBodyComponent, "AddBoxShape", LAddBoxShape )
	LUA_NAME_METHOD( RigidBodyComponent, "AddTriangleShape", LAddTriangleShape )
	LUA_NAME_METHOD( RigidBodyComponent, "AddEquilateralTriangleShape", LAddEquilateralTriangleShape )
	LUA_NAME_METHOD( RigidBodyComponent, "AddPolygonShape", LAddPolygonShape )

	LUA_NAME_METHOD( RigidBodyComponent, "SetMass", LSetMass )
	LUA_NAME_METHOD( RigidBodyComponent, "SetBody", LSetBody )
	LUA_NAME_METHOD( RigidBodyComponent, "SetMassFromShapes", LSetMassFromShapes )
	LUA_NAME_METHOD( RigidBodyComponent, "SetDynamic", LSetDynamic )
	LUA_NAME_METHOD( RigidBodyComponent, "SetStatic", LSetStatic )
	LUA_NAME_METHOD( RigidBodyComponent, "DeleteShapes", LDeleteShapes )
	LUA_NAME_METHOD( RigidBodyComponent, "DeleteShape", LDeleteShape )

	LUA_NAME_METHOD( RigidBodyComponent, "AddImpulse", LAddImpulse )
	LUA_NAME_METHOD( RigidBodyComponent, "AddForce", LAddForce )
	LUA_NAME_METHOD( RigidBodyComponent, "AddImpulseAtPoint", LAddImpulseAtPoint )
	LUA_NAME_METHOD( RigidBodyComponent, "AddForceAtPoint", LAddForceAtPoint)
	LUA_NAME_METHOD( RigidBodyComponent, "SetLinearVelocity", LSetLinearVelocity )
	LUA_NAME_METHOD( RigidBodyComponent, "GetLinearVelocity", LGetLinearVelocity )
	LUA_NAME_METHOD( RigidBodyComponent, "GetNormalVelocity", LGetNormalVelocity )

	LUA_NAME_METHOD( RigidBodyComponent, "GetPosition", LGetPosition )
	LUA_NAME_METHOD( RigidBodyComponent, "SetPosition", LSetPosition )
LUA_END_DEFINE_CLASS


LUA_DEFINE_METHOD( RigidBodyComponent, LAddSphereShape )
{
	float radius;
	PhysicsComponent::denFricRest otherData;
	bool isSensor;

	int stackSize = lua_gettop(ls);
	if( stackSize != 5 )
	{
		PRINT_ERR_LUASTACK( ls, "Error - not enough parameters for LAddSphereShape" );
		return 0;
	}

	radius = (float)lua_tonumber(ls, 1);
	otherData.mDensity = (float)lua_tonumber(ls, 2);
	otherData.mFriction = (float)lua_tonumber(ls, 3);
	otherData.mRestitution = (float)lua_tonumber(ls, 4);
	isSensor = lua_toboolean(ls, 5) ? true : false;

#ifdef USE_BOX2D
	//@TODO: Rewrite to use the CRenderModule level calls for generating these shapes.
	b2Shape* newShape = NULL;//AddSphereShape( radius, otherData, isSensor );
	LunarLite<b2Shape>::push(ls, newShape);
#else
	//@TODO: Change this to GuruShape and push that.
	void* newShape = NULL;
	LunarLite<void>::push(ls, newShape);
#endif
	return 1;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddBoxShape )
{
	float width;
	float height;
	PhysicsComponent::denFricRest otherData;
	bool isSensor;

	int stackSize = lua_gettop(ls);
	if( stackSize != 6 )
	{
		PRINT_ERR_LUASTACK( ls, "Error - not enough parameters for LAddBoxShape" );
		return 0;
	}

	width = (float)lua_tonumber(ls, 1);
	height = (float)lua_tonumber(ls, 2);
	otherData.mDensity = (float)lua_tonumber(ls, 3);
	otherData.mFriction = (float)lua_tonumber(ls, 4);
	otherData.mRestitution = (float)lua_tonumber(ls, 5);
	isSensor = lua_toboolean(ls, 6) ? true : false;

#ifdef USE_BOX2D
	//@TODO: Rewrite to use the CRenderModule level calls for generating these shapes.
	b2Shape* newShape = NULL;//AddBoxShape( width, height, otherData, isSensor );
	LunarLite<b2Shape>::push(ls, newShape);
#else
	void* newShape = NULL;
	LunarLite<void>::push(ls, &newShape);
#endif
	return 1;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddTriangleShape )
{

#ifdef USE_BOX2D
	b2Vec2 a;
	b2Vec2 b;
	b2Vec2 c;
#else
	CVector3 a;
	CVector3 b;
	CVector3 c;
#endif
	PhysicsComponent::denFricRest otherData;
	bool isSensor;

	int stackSize = lua_gettop(ls);
	if( stackSize != 10 )
	{
		PRINT_ERR_LUASTACK( ls, "Error - not enough parameters for LAddTriangleShape" );
		return 0;
	}

	//a.x = lua_tonumber(ls, 1);
	//a.y = lua_tonumber(ls, 2);
	a.x = (float)lua_tonumber(ls, 1);
	a.y = (float)lua_tonumber(ls, 2);

	b.x = (float)lua_tonumber(ls, 3);
	b.y = (float)lua_tonumber(ls, 4);

	c.x = (float)lua_tonumber(ls, 5);
	c.y = (float)lua_tonumber(ls, 6);

	otherData.mDensity = (float)lua_tonumber(ls, 7);
	otherData.mFriction = (float)lua_tonumber(ls, 8);
	otherData.mRestitution = (float)lua_tonumber(ls, 9);
	isSensor = lua_toboolean(ls, 10) ? true : false;

#ifdef USE_BOX2D
	//@TODO: Rewrite to use the CRenderModule level calls for generating these shapes.
	b2Shape* newShape = NULL;//AddTriangleShape(a, b, c, otherData, isSensor);
	LunarLite<b2Shape>::push(ls, newShape);
#else
	void* newShape = NULL;
	LunarLite<void>::push(ls, newShape);
#endif
	return 1;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddEquilateralTriangleShape )
{
	float inscribedRadius;
	PhysicsComponent::denFricRest otherData;
	bool isSensor;

	inscribedRadius = (float)lua_tonumber(ls, 1);
	int stackSize = lua_gettop(ls);
	if( stackSize != 5 )
	{
		PRINT_ERR_LUASTACK( ls, "Error - not enough parameters for LAddEquilateralTriangleShape" );
		return 0;
	}

	inscribedRadius = (float)lua_tonumber(ls, 1);
	otherData.mDensity = (float)lua_tonumber(ls, 2);
	otherData.mFriction = (float)lua_tonumber(ls, 3);
	otherData.mRestitution = (float)lua_tonumber(ls, 4);
	isSensor = lua_toboolean(ls, 5) ? true : false;

#ifdef USE_BOX2D
	//@TODO: Rewrite to use the CRenderModule level calls for generating these shapes.
	b2Shape* newShape = NULL;//AddEquilateralTriangleShape( inscribedRadius, otherData, isSensor );
	LunarLite<b2Shape>::push(ls, newShape);
#else
	void* newShape = NULL;
	LunarLite<void>::push(ls, newShape);
#endif
	return 1;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddPolygonShape )
{
	//@todo:
#ifdef USE_BOX2D
	b2PolygonDef polyDef;
#else
	void* polyDef;
#endif
	PhysicsComponent::denFricRest otherData;
	bool isSensor = false;
	bool fromFile = lua_toboolean(ls,1) ? true : false;

	if( fromFile )
	{
		//@TODO: Remove, this is to get rid of a warning only.
#ifdef USE_BOX2D
		polyDef.isSensor = isSensor;
#else
		polyDef = (void*)isSensor;
#endif
	}

	//do we take in a list of vertices from lua, or read in from a file
	otherData.mDensity = (float)lua_tonumber(ls, 2);
	otherData.mFriction = (float)lua_tonumber(ls, 3);
	otherData.mRestitution = (float)lua_tonumber(ls, 4);

#ifdef USE_BOX2D
	//@TODO: Rewrite to use the CRenderModule level calls for generating these shapes.
	b2Shape* newShape = NULL;//AddPolygonShape( &polyDef );
	LunarLite<b2Shape>::push(ls, newShape);
#else
	void* newShape = NULL;
	LunarLite<void>::push(ls, newShape);
#endif
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetBody )
{
#ifdef USE_BOX2D
	b2BodyDef bodyDef;
#else
	std::shared_ptr<RigidBodyDescriptor> bodyDef;
#endif

	int stackSize = lua_gettop(ls);
	if( stackSize < 8 )
	{
		PRINT_ERR_LUASTACK( ls, "Error - not enough parameters for LSetBody" );
		return 0;
	}

#ifdef USE_BOX2D
	//@todo - do we need to calculate the inertia tensor when setting the mass this way?
	bodyDef.massData.mass = (float)lua_tonumber(ls, 1);
	bodyDef.massData.center.x = (float)lua_tonumber(ls, 2);
	bodyDef.massData.center.y = (float)lua_tonumber(ls, 3);

	bodyDef.linearDamping = (float)lua_tonumber(ls, 4);
	bodyDef.angularDamping = (float)lua_tonumber(ls, 5);

	bodyDef.position.x = (float)lua_tonumber(ls, 6);
	bodyDef.position.y = (float)lua_tonumber(ls, 7);
	bodyDef.angle = (float)lua_tonumber(ls, 8);

	bodyDef.isBullet = false;
	if( stackSize == 9 )
	{
		bodyDef.isBullet = (lua_tonumber(ls,9)) ? true : false;
	}

	SetBody( &bodyDef );
#else
	//@TODO:  Implement for the base system without Box2D
#endif
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetMass )
{
	float mass = (float)lua_tonumber(ls, 1);
	SetMass( mass );

	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetPosition )
{
	float x = (float)lua_tonumber( ls, 1 );
	float y = (float)lua_tonumber( ls, 2 );

#ifndef USE_BOX2D
	float z = (float)lua_tonumber( ls, 3 );
	float angle = (float)lua_tonumber(ls, 4);
#else
	float angle = (float)lua_tonumber(ls, 3);
#endif

#ifdef USE_BOX2D
	SetPosition( b2Vec2(x,y), angle );
#else
	//@TODO:  Implement for the base system without Box2D
	SetPosition(CVector3(x,y,z), angle);
#endif
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetMassFromShapes )
{
	if( ls )
	{
		SetMassFromShapes();
	}
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetDynamic )
{
	if( ls )
	{
		SetDynamic();
	}
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetStatic )
{
	if( ls )
	{
		SetStatic();
	}
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddImpulse )
{
	if( lua_gettop(ls) < 3 )
		PRINT_ERR_LUASTACK(ls, "param error" );
#ifdef USE_BOX2D
	b2Vec2 dir;
#else
	CVector3 dir;
#endif

	float strength;

	dir.x = (float)lua_tonumber(ls, 1);
	dir.y = (float)lua_tonumber(ls, 2);

#ifndef USE_BOX2D
	dir.z = (float)lua_tonumber(ls, 3);
	strength = (float)lua_tonumber(ls, 4);
#else
	strength = (float)lua_tonumber(ls, 3);
#endif

	AddImpulse( dir, strength );
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddForce )
{
#ifdef USE_BOX2D
	b2Vec2 dir;
#else
	CVector3 dir;
#endif
	float strength;

	dir.x = (float)lua_tonumber(ls, 1);
	dir.y = (float)lua_tonumber(ls, 2);
	strength = (float)lua_tonumber(ls, 3);

	AddForce( dir, strength );
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddImpulseAtPoint )
{
#ifdef USE_BOX2D
	b2Vec2 dir;
	b2Vec2 point;
#else
	CVector3 dir;
	Point point;
#endif

	float strength;

	dir.x = (float)lua_tonumber(ls, 1);
	dir.y = (float)lua_tonumber(ls, 2);
#ifndef USE_BOX2D
	dir.z = (float)lua_tonumber(ls, 3);
	point.x() = (float)lua_tonumber(ls, 4);
	point.y() = (float)lua_tonumber(ls, 5);
	point.z() = (float)lua_tonumber(ls, 6);
	strength = (float)lua_tonumber(ls, 7);
#else
	point.x = (float)lua_tonumber(ls, 3);
	point.y = (float)lua_tonumber(ls, 4);
	strength = (float)lua_tonumber(ls, 5);
#endif

	AddImpulseAtPoint( dir, point, strength );
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LAddForceAtPoint)
{
#ifdef USE_BOX2D
	b2Vec2 dir;
	b2Vec2 point;
#else
	CVector3 dir;
	Point point;
#endif

	float strength;

	dir.x = (float)lua_tonumber(ls, 1);
	dir.y = (float)lua_tonumber(ls, 2);
#ifndef USE_BOX2D
	dir.z = (float)lua_tonumber(ls, 3);
	point.x() = (float)lua_tonumber(ls, 4);
	point.y() = (float)lua_tonumber(ls, 5);
	point.z() = (float)lua_tonumber(ls, 6);
	strength = (float)lua_tonumber(ls, 7);
#else
	point.x = (float)lua_tonumber(ls, 3);
	point.y = (float)lua_tonumber(ls, 4);
	strength = (float)lua_tonumber(ls, 5);
#endif

	AddForceAtPoint( dir, point, strength );
	return 0;
}

LUA_DEFINE_METHOD( RigidBodyComponent, LSetLinearVelocity)
{
#ifdef USE_BOX2D
	b2Vec2 dir;
#else
	CVector3 dir;
#endif
	float strength;
	
	dir.x = (float)lua_tonumber(ls, 1);
	dir.y = (float)lua_tonumber(ls, 2);

#ifndef USE_BOX2D
	dir.z = (float)lua_tonumber(ls, 3);
	strength = (float)lua_tonumber(ls, 4);
#else
	strength = (float)lua_tonumber(ls, 3);
#endif

	SetLinearVelocity( dir, strength );

	return 0;
}


LUA_DEFINE_METHOD( RigidBodyComponent, LDeleteShapes )
{
	if( ls )
	{
		DeleteShapes();
	}
	return 0;
}


LUA_DEFINE_METHOD( RigidBodyComponent, LDeleteShape )
{
#ifdef USE_BOX2D
	b2Shape* shape = LunarLite<b2Shape>::check(ls, 1);
#else
	void* shape = LunarLite<void>::check(ls, 1);	
#endif
	DeleteShape(shape);
	return 0;
}


LUA_DEFINE_METHOD( RigidBodyComponent, LGetPosition )
{
#ifdef USE_BOX2D
	b2Vec2 pos = mBodyRef->GetPosition();
#else
	CVector3 pos = GetPosition();
#endif

	lua_pushnumber(ls, pos.x);
	lua_pushnumber(ls, pos.y);

#ifndef USE_BOX2D
	lua_pushnumber(ls, pos.z);
	return 3;
#else
	return 2;
#endif
}

LUA_DEFINE_METHOD( RigidBodyComponent, LGetLinearVelocity )
{
#ifdef USE_BOX2D
	b2Vec2 vel = mBodyRef->GetLinearVelocity();
#else
	CVector3 vel = GetLinearVelocity();
#endif

	lua_pushnumber(ls, vel.x);
	lua_pushnumber(ls, vel.y);

#ifndef USE_BOX2D
	lua_pushnumber(ls, vel.z);
	return 3;
#else
	return 2;
#endif
}

LUA_DEFINE_METHOD( RigidBodyComponent, LGetNormalVelocity )
{
#ifdef USE_BOX2D
	b2Vec2 vel = mBodyRef->GetLinearVelocity();
	float mag = vel.Normalize();
#else
	CVector3 vel = GetLinearVelocity();
	float mag = vel.magnitude();
#endif

	lua_pushnumber(ls, vel.x);
	lua_pushnumber(ls, vel.y);

#ifdef USE_BOX2D
	lua_pushnumber(ls, mag);
	return 3;
#else
	lua_pushnumber(ls, vel.z);
	lua_pushnumber(ls, mag);
	return 4;
#endif
}
#endif

