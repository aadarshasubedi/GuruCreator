/*
Author:		Dan Strycharske
Date:		06.05.09
File:		PhysicsComponent.h
Brief:      
*/


#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "ComponentDisciple.h"
#include "Utility.h"
#include "DQuaternion.h"

#ifdef USE_LUA
#include "LunarHelper.h"
#endif
#ifdef USE_BOX2D
#include "Box2D.h"
#endif


class PhysicsComponent : public ComponentDisciple
{
public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	struct denFricRest
	{
		denFricRest(float density = 1.0f, float friction = 0.4f, float restitution = 0.8f)
		{
			mDensity = density;
			mFriction = friction;
			mRestitution = restitution;
		}
		float mDensity;
		float mFriction;
		float mRestitution;
	};
	
	//This is the key function for access from the render components and such directly.
	const CVector3&	GetPosition();
	void	SetPosition( CVector3 pos, float angle = 0);
	
	DQuaternion	GetOrientation();
	void	SetOrientation( DQuaternion orientation, float angle = 0);

	/************************************************************************/
	/* Virtual - ComponentDisciple
	/************************************************************************/
	virtual void Init() {}

private:
	CVector3	mPosition;
	DQuaternion mOrientation;

	////////////////////////////////////////////////////////
	//Various adding physics shapes to the body.
#ifdef USE_BOX2D
	b2Shape*	AddShape( b2ShapeDef* shapeDef );
	b2Shape*	AddSphereShape( float radius, denFricRest otherData, bool isSensor = false );
	b2Shape*	AddBoxShape( float width, float height, denFricRest otherData, bool isSensor = false );
	b2Shape*	AddTriangleShape( b2Vec2 a, b2Vec2 b, b2Vec2 c, denFricRest otherData, bool isSensor = false );
	b2Shape*	AddEquilateralTriangleShape( float inscribedRadius, denFricRest otherData, bool isSensor = false );
	b2Shape*	AddPolygonShape( b2PolygonDef* polyDef, bool isSensor = false );

	//The mass setting functions.  These should always be called after all shapes have been added.
	void	SetBody( b2BodyDef* def );
	void	SetMass( float mass );
	void	SetPosition( b2Vec2 pos, float angle = 0);
	void	SetMassFromShapes();

	//These functions adjust the mass values to set an object as dynamic or not.
	void	SetDynamic();
	void	SetStatic();

	//Force and impulse applying functions for the dynamic bodies.
	void	AddImpulse( const b2Vec2& dir, float strength = 1.0f );
	void	AddForce( const b2Vec2& dir, float strength = 1.0f );
	void	AddImpulseAtPoint( const b2Vec2& dir, const b2Vec2& applyPoint, float strength = 1.0f );
	void	AddForceAtPoint( const b2Vec2& dir, const b2Vec2& applyPoint, float strength = 1.0f );
	void	SetLinearVelocity( const b2Vec2& vec, float strength = 1.0f );

	//Delete shapes from the body.  For now just delete all of them.
	void	DeleteShapes();
	void	DeleteShape( b2Shape* shape );
	
	//The Box2d body ref.
	b2Body*	mBodyRef;
#endif

#ifdef USE_LUA
	LUA_DECLARE_CLASS(PhysicsComponent)
		LUA_DECLARE_METHOD(LSetPosition)
#endif
};


#endif
