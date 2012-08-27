/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       RenderComponent.cpp
Brief:      
/************************************************************************/


#include "RenderComponent.h"
#include "LunarHelper.h"


//////////////////////////////////////////////////////////////////////////////////////
///////Render Component///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
RenderComponent::RenderComponent( ) : ComponentDisciple(), mOffsetPos(0,0,0), mOffsetAngle(0.0f)
{
}


RenderComponent::RenderComponent(const RenderComponent& comp) : ComponentDisciple(comp)
{
}


RenderComponent::~RenderComponent()
{
}


void RenderComponent::render()
{
}


#ifdef USE_BOX2D
//Get and set the offset angle and position.
void	RenderComponent::SetOffsetPos( const b2Vec2& offset )
{
	mOffsetPos = CVector3(offset.x, offset.y, 0.0f);
}
b2Vec2	RenderComponent::GetOffsetPos()
{
	return b2Vec2(mOffsetPos.x, mOffsetPos.y);
}

#else

//Get and set the offset angle and position.
void	RenderComponent::SetOffsetPos( const CVector3& offset )
{
	mOffsetPos = offset;
}
CVector3	RenderComponent::GetOffsetPos()
{
	return mOffsetPos;
}

#endif


void	RenderComponent::SetOffsetAngle( float angle )
{
	mOffsetAngle = angle;
}

float	RenderComponent::GetOffsetAngle()
{
	return mOffsetAngle;
}


#ifdef USE_LUA
LUA_START_DEFINE_CLASS(RenderComponent)
	LUA_NAME_METHOD(RenderComponent, "SetOffsetPos", LSetOffsetPos )
LUA_END_DEFINE_CLASS


LUA_DEFINE_METHOD(RenderComponent, LSetOffsetPos)
{
	if( !ls ) { return 0;}

	float x = (float)lua_tonumber(ls, 1 );
	float y = (float)lua_tonumber(ls, 2 );
	float z = (float)lua_tonumber(ls, 3 );

	this->SetOffsetPos(CVector3(x,y,z));

	return 0;
}


#endif
