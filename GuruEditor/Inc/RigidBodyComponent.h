/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       RigidBodyComponent.h
Brief:      
/************************************************************************/


#ifndef RigidBodyComponent_H
#define RigidBodyComponent_H


/************************************************************************/
/* Includes
/************************************************************************/
#include "ComponentDisciple.h"
#include "PhysicsComponent.h"

/************************************************************************/
/* Forward Declarations
/************************************************************************/
class PhysicsComponent;
class GameObject;


class RigidBodyComponent : public ComponentDisciple
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/
	PhysicsComponent* mPhysicsData;

	float mMass;

#ifdef USE_BOX2D
	b2Body* mBodyRef;
#else
#endif

	/************************************************************************/
	/* Private - Member methods
	/************************************************************************/

protected:
	/************************************************************************/
	/* Protected - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Protected - Member methods
	/************************************************************************/

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	RigidBodyComponent();
	virtual ~RigidBodyComponent();

	GameObject* GetParentObj();

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
#ifdef USE_BOX2D
	void AddSphereShape(float radius, PhysicsComponent::denFricRest otherData, bool isSensor);
	void AddBoxShape(b2Shape* shape);
	void AddTriangleShape(b2Shape* shape);
	void AddEquilateralTriangleShape(b2Shape* shape);
	void AddPolygonShape(b2Shape* shape);
	void SetMass(float mass);
	void SetPosition(const b2Vec2& pos, float angle);
	void SetBody(b2BodyDef* bodyDef);
	void SetMassFromShapes();
	void SetDynamic();
	void SetStatic();
	void DeleteShapes();
	void DeleteShape( b2Shape* shape );

	void AddImpulse(const b2Vec2& impulse, float strength);
	void AddForce(const b2Vec2& force, float strength);
	void AddImpulseAtPoint(const b2Vec2& impulse, const b2Vec2& applyPoint, float strength);
	void AddForceAtPoint(const b2Vec2& force, const b2Vec2& applyPoint, float strength);
	void SetLinearVelocity( const b2Vec2& velocity, float strength );
	b2Vec2 GetPosition();
	b2Vec2 GetLinearVelocity();
	b2Vec2 GetNormalVelocity();
#else
	void AddSphereShape(void* shapeDesc);
	void AddBoxShape(void* shapeDesc);
	void AddTriangleShape(void* shapeDesc);
	void AddEquilateralTriangleShape(void* shapeDesc);
	void AddPolygonShape(void* shapeDesc);
	void SetMass(float mass);
	void SetPosition(const CVector3& pos, float angle);
	void SetBody(void* bodyRef);
	void SetMassFromShapes();
	void SetDynamic();
	void SetStatic();
	void DeleteShapes();
	void DeleteShape(void* shapeRef);

	void AddImpulse(const CVector3& impulse, float strength);
	void AddForce(const CVector3& force, float strength);
	void AddImpulseAtPoint(const CVector3& impulse, const CVector3& applyPoint, float strength);
	void AddForceAtPoint(const CVector3& force, const CVector3& applyPoint, float strength);
	void SetLinearVelocity(const CVector3& velocity, float strength);
	CVector3 GetPosition();
	CVector3 GetLinearVelocity();
	CVector3 GetNormalVelocity();
#endif
	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/

#ifdef USE_LUA
	LUA_DECLARE_CLASS(RigidBodyComponent)
	LUA_DECLARE_METHOD( LAddSphereShape )
	LUA_DECLARE_METHOD( LAddBoxShape )
	LUA_DECLARE_METHOD( LAddTriangleShape )
	LUA_DECLARE_METHOD( LAddEquilateralTriangleShape )
	LUA_DECLARE_METHOD( LAddPolygonShape )

	LUA_DECLARE_METHOD( LSetMass )
	LUA_DECLARE_METHOD( LSetBody )
	LUA_DECLARE_METHOD( LSetMassFromShapes )
	LUA_DECLARE_METHOD( LSetDynamic )
	LUA_DECLARE_METHOD( LSetStatic )
	LUA_DECLARE_METHOD( LDeleteShapes )
	LUA_DECLARE_METHOD( LDeleteShape )

	LUA_DECLARE_METHOD( LAddImpulse )
	LUA_DECLARE_METHOD( LAddForce )
	LUA_DECLARE_METHOD( LAddImpulseAtPoint )
	LUA_DECLARE_METHOD( LAddForceAtPoint)
	LUA_DECLARE_METHOD( LSetLinearVelocity )
	LUA_DECLARE_METHOD( LGetLinearVelocity )
	LUA_DECLARE_METHOD( LGetNormalVelocity )

	LUA_DECLARE_METHOD( LGetPosition )
	LUA_DECLARE_METHOD( LSetPosition )
#endif
};


#endif
