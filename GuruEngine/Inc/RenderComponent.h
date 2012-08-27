
//A definition of a generic 2D render mesh.  It will know how to draw itself,
// via an enum matching that of OpenGL and many other graphics drivers.
//Defines a list of points, and the type of mesh determines how they're rendered.
//The mesh will have a vbo since using OpenGL, make more generic layer so it can
// be used with a DirectX buffer or other approach.  

//In this case, a vertex struct will be handy.  This will help to make a generic layer
// between that and any graphics api.

#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H


#include "Utility.h"
#include "Vector.h"
#include "ComponentDisciple.h"


class GameObject;


struct GuruVertex
{
	GuruVertex() : position(), color(), tex(), normal() {}
	GuruVertex( CVector3 p, CVector3 c,
				CVector3 t, CVector3 n ) : position(p), color(c), tex(t), normal(n) {}

	~GuruVertex() {}

	//For now will have just position and color.
	CVector3 position;
	CVector3 color;
	CVector3 tex;
	CVector3 normal;
};


class RenderComponent : public ComponentDisciple
{
public:
	RenderComponent( );
	RenderComponent(const RenderComponent& comp);
	virtual ~RenderComponent();
	
	//Render all of the added render objects, for now just the meshes.
	void render();

	/************************************************************************/
	/* Virtual - ComponentDisciple
	/************************************************************************/
	virtual void Init() {}
	
	void	SetOffsetAngle( float angle );
	float	GetOffsetAngle();

	//void SetParent( GameObjectPtr parent ) { mParentObj = parent; }
	//GameObjectPtr GetParent() { return mParentObj; }

protected:
	
	//GameObjectPtr mParentObj;

	//A position offset and angle of rotation.
	CVector3	mOffsetPos;
	float	mOffsetAngle;


#ifdef USE_BOX2D
public:
	//Get and set the offset angle and position.
	void	SetOffsetPos( const b2Vec2& offset );
	b2Vec2	GetOffsetPos();
#else //if using box2D
	//Get and set the offset angle and position.
	void	SetOffsetPos( const CVector3& offset );
	CVector3	GetOffsetPos();
#endif //Default position representation if no other physics system is in place.

#ifdef USE_LUA
	LUA_DECLARE_CLASS(RenderComponent)
		LUA_DECLARE_METHOD(LSetOffsetPos)
#endif
};

#endif
