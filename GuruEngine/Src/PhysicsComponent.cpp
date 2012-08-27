/************************************************************************
Author:		Dan Strycharske
Date:       02.07.12
File:       PhysicsComponent.cpp
Brief:      
/************************************************************************/

#include "PhysicsComponent.h"
#include "Utility.h"

//Todo list
//lua's Create polygon shape - does it take in a list of points or read from a file? or do we make both?


PhysicsComponent::PhysicsComponent() : ComponentDisciple()
{
#ifdef USE_BOX2D
	mBodyRef = NULL;
#endif
}

PhysicsComponent::~PhysicsComponent() 
{
#ifdef USE_BOX2D
	((Game_Engine*)Game_Engine::GetSingletonPtr())->mWorldScene->mPhysicsWorld_Box2d->DestroyBody( mBodyRef );
	mBodyRef = NULL;
#endif
}


#ifdef USE_BOX2D

//Various adding physics shapes to the body.
b2Shape*	PhysicsComponent::AddShape( b2ShapeDef* shapeDef )
{
	b2Shape* shapeRef = NULL;
	if( mBodyRef )
	{
		shapeDef->userData = this;
		shapeRef = mBodyRef->CreateShape( shapeDef );
	}
	return shapeRef;
}


b2Shape*	PhysicsComponent::AddSphereShape( float radius, denFricRest otherData, bool isSensor )
{
	b2CircleDef sphereDef;
	sphereDef.radius = radius;
	sphereDef.density = otherData.mDensity;
	sphereDef.friction = otherData.mFriction;
	sphereDef.restitution = otherData.mRestitution;
	sphereDef.isSensor = isSensor;
	b2Shape* shape = AddShape( &sphereDef );	
	return shape;
}


b2Shape*	PhysicsComponent::AddBoxShape( float width, float height, denFricRest otherData, bool isSensor )
{
	b2PolygonDef boxDef;	
	boxDef.density = otherData.mDensity;
	boxDef.friction = otherData.mFriction;
	boxDef.restitution = otherData.mRestitution;
	boxDef.isSensor = isSensor;
	boxDef.SetAsBox( width, height );
	b2Shape* shape = AddShape( &boxDef );	
	return shape;
}


b2Shape*	PhysicsComponent::AddTriangleShape( b2Vec2 , b2Vec2 , b2Vec2 , denFricRest otherData, bool isSensor )
{
	//@todo
	b2PolygonDef triDef;
	triDef.density = otherData.mDensity;
	triDef.friction = otherData.mFriction;
	triDef.restitution = otherData.mRestitution;
	triDef.isSensor = isSensor;

	//b2Shape* shape = AddShape( &triDef );
	//return shape;
	return NULL;
}


b2Shape*	PhysicsComponent::AddEquilateralTriangleShape( float , denFricRest otherData, bool isSensor )
{
	//@todo
	b2PolygonDef triDef;
	triDef.density = otherData.mDensity;
	triDef.friction = otherData.mFriction;
	triDef.restitution = otherData.mRestitution;
	triDef.isSensor = isSensor;
	return NULL;
}


b2Shape*	PhysicsComponent::AddPolygonShape( b2PolygonDef* polyDef, bool isSensor )
{
	//@todo	
	polyDef->isSensor = isSensor;
	return NULL;
}


//The mass setting functions.  These should always be called after all shapes have been added.
void	PhysicsComponent::SetBody( b2BodyDef* def )
{
	def->userData = this;
	mBodyRef = ((Game_Engine*)Game_Engine::GetSingletonPtr())->mWorldScene->mPhysicsWorld_Box2d->CreateBody( def );
}

void	PhysicsComponent::SetMass( float mass )
{
	b2MassData data;
	data.mass = mass;
	data.center = mBodyRef->GetPosition();
	mBodyRef->SetMass( &data );
}

void	PhysicsComponent::SetPosition( b2Vec2 pos, float angle )
{
	mBodyRef->SetXForm( pos, angle );
}

void	PhysicsComponent::SetMassFromShapes()
{
	mBodyRef->SetMassFromShapes();
}


//These functions adjust the mass values to set an object as dynamic or not.
void	PhysicsComponent::SetDynamic()
{
	SetMassFromShapes();
}

void	PhysicsComponent::SetStatic()
{
	SetMass(0.0f);
}

void	PhysicsComponent::AddImpulse( const b2Vec2& dir, float strength )
{
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyImpulse( strength * dir, mBodyRef->GetPosition() );
	}
}

void	PhysicsComponent::AddForce( const b2Vec2& dir, float strength )
{
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyForce( strength * dir, mBodyRef->GetPosition() );
	}
}

void	PhysicsComponent::AddImpulseAtPoint( const b2Vec2& dir, const b2Vec2& applyPoint, float strength )
{
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyImpulse( strength * dir, applyPoint );
	}
}

void	PhysicsComponent::AddForceAtPoint( const b2Vec2& dir, const b2Vec2& applyPoint, float strength  )
{
	if( mBodyRef != NULL )
	{
		mBodyRef->ApplyForce( strength * dir, applyPoint );
	}
}

void	PhysicsComponent::SetLinearVelocity( const b2Vec2& vec, float strength )
{
	mBodyRef->SetLinearVelocity( strength * vec );
}

//Delete shapes from the body.  For now just delete all of them.
void	PhysicsComponent::DeleteShapes()
{
	b2Shape* shapeList = mBodyRef->GetShapeList();
	while( shapeList != NULL )
	{
		b2Shape* nextShape = shapeList->GetNext();
		DeleteShape( shapeList );
		shapeList = nextShape;
	}
}

void	PhysicsComponent::DeleteShape( b2Shape* shape )
{
	if( mBodyRef != NULL )
	{
		mBodyRef->DestroyShape( shape );
	}
}

#endif //if using box2d

//This is the key function for access from the render components and such directly.
const CVector3&	PhysicsComponent::GetPosition()
{
 	return mPosition;
}

void	PhysicsComponent::SetPosition( CVector3 pos, float )
{
	mPosition = pos;
}


DQuaternion	PhysicsComponent::GetOrientation()
{
	return DQuaternion();
}


void	PhysicsComponent::SetOrientation( DQuaternion orientation, float )
{
	mOrientation = orientation;
}


#ifdef USE_LUA
LUA_START_DEFINE_CLASS( PhysicsComponent )
	LUA_NAME_METHOD(PhysicsComponent, "SetPosition", LSetPosition)
LUA_END_DEFINE_CLASS


LUA_DEFINE_METHOD( PhysicsComponent, LSetPosition )
{
	if( !ls ) { return 0;}

	float x = (float)lua_tonumber(ls, 1 );
	float y = (float)lua_tonumber(ls, 2 );
	float z = (float)lua_tonumber(ls, 3 );

	this->SetPosition(CVector3(x,y,z));

	return 0;
}

#endif