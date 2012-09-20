/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/


#include "Camera.h"
#include "gl_inc.h"
#include "CRenderModule.h"


//Initializing functions.  Setting the camera up in various ways.
Camera::Camera()
{
	Init(0.1f, 2000.f, 640/480, (M_PI / 2.0f)*(180/M_PI), CVector3(0,0.f,-50.f) );

	mPosition = CVector3( 0, 0, 0 );
	mPosition2D = CVector3( 0, 0, 0 );

	mZoomFactor2D = 1.0f;
	mZoomInc = 0.01f;

	mMaxZoomFactor2D = 10.0f;
	mMinZoomFactor2D = 0.1f;
}


Camera::Camera( CRenderModule::RMOptions options )
{
	Init( 0.5f, 2000.f, (float)options.mWidth/(float)options.mHeight, 45.0f, CVector3(0.f,0.f,0.f) );

    mPosition = CVector3( 0, 0, -300 );

	//Set width and heigth for ortho projection.
	w = options.mWidth;
	h = options.mHeight;

	mZoomFactor2D = 1.0f;
	mZoomInc = 0.01f;

	mMaxZoomFactor2D = 10.0f;
	mMinZoomFactor2D = 0.1f;

	mWinHeight = (float)options.mHeight;
	mWinWidth = (float)options.mWidth;

	mRotation = DQuaternion(CVector3(0,0,1), 0);
	GLfloat matrix[16];
	mRotation.CreateMatrixGL(matrix);

	mSide = CVector3(matrix[0], matrix[1], matrix[2] );
	mUp = CVector3(matrix[4], matrix[5], matrix[6] );
	mLook = CVector3(matrix[8], matrix[9], matrix[10] );
}

Camera::~Camera()
{
}

void Camera::Init(	float np, float fp, float ar, float fv, const CVector3& p )
{
	mLookAtPoint = p;

	//This alerts an immediate update to the cameras perspective matrix.
	perspectiveChanged = true;
	perspectiveProj  = true;

	//Alerts to changes made to the view matrix.
	viewChanged = true;

	moveSpeed = 0.0f;
	sideSpeed = 0.0f;
	upSpeed = 0.0f;

	mInverted = false;
	mOrbit = false;

	pitchDegree = 0.0f;
	pitchSpeed  = 0.3f;

	headingDegree = 0.0f;
	headingSpeed = 0.3f;

	rollDegree = 0.0f;
	rollSpeed = 0.3f;

	curSpeed = 1.5f;

    mAspectRatio = ar;
    mFieldOfView = fv;
    mNearDist = np;
    mFarDist = fp;
}


void	Camera::SetModelView()
{
	//Calculate the pitch quaternion based on the pitch degree.  same for heading.
	GLfloat matrix[16];
	DQuaternion q, h;

	DQuaternion heading;
	heading.FromAxisAngle( CVector3(0,1,0), -headingDegree);
	
	DQuaternion pitch;
	pitch.FromAxisAngle( CVector3(1,0,0), pitchDegree);

	DQuaternion roll;
	roll.FromAxisAngle(CVector3(0,0,-1), rollDegree);
	
	//Apply heading and get new side vector.
	DQuaternion rotation = heading * pitch * roll;

	//mRotation = pitch * mRotation;
	mRotation = mRotation * heading;
	mRotation = roll * mRotation;
	
	//mRotation.normalized();
	mRotation.CreateMatrixGL(matrix);	
	rotation.CreateMatrixGL(matrix);

	mUp = CVector3(matrix[4], matrix[5], matrix[6] );
	mLook = CVector3(matrix[8], matrix[9], matrix[10] );
	mSide = CVector3(-matrix[0], -matrix[1], -matrix[2] );
	mUp.normalize();
	mLook.normalize();
	mSide.normalize();
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glPushMatrix();
	glLoadIdentity();
	CVector3 lookAt = mPosition + mLook * 20.0f;
	gluLookAt(mPosition.x, mPosition.y, mPosition.z, lookAt.x, lookAt.y, lookAt.z, mUp.x, mUp.y, mUp.z);
	
	//rollDegree = 0;
	//headingDegree = 0;
	//pitchDegree = 0;
}

void	Camera::SetPerspective()
{
	if( perspectiveProj )
	{ 
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluPerspective( mFieldOfView, mAspectRatio, mNearDist, mFarDist );

	}
	else
	{
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();

		int rightX = (int)mWinWidth / 20;
		int leftX = -rightX;

		int topY = (int)mWinHeight / 20;
		int botY = -topY;

		gluOrtho2D( (leftX * mZoomFactor2D + mPosition2D.x), (rightX * mZoomFactor2D + mPosition2D.x), (botY * mZoomFactor2D + mPosition2D.y), (topY * mZoomFactor2D + mPosition2D.y) );
	}
}

void	Camera::ChangePitch( float degree )
{
	pitchDegree += degree;
}

void	Camera::ChangeHeading( float degree )
{
	headingDegree += degree;
}

void	Camera::ChangeRoll( float degree )
{
	rollDegree += degree;
}


void	Camera::ChangeForwardVel( float speed )
{
	moveSpeed += speed;
}

void	Camera::ChangeUpVel( float speed )
{
	upSpeed += speed;
}

void	Camera::ChangeSideVel( float speed )
{
	sideSpeed += speed;
}


void    Camera::glGetEyePt( GLfloat* eye )
{
    eye[0] = mPosition.x;
    eye[1] = mPosition.y;
    eye[2] = mPosition.z;
}

void    Camera::glGetLookPt( GLfloat* look )
{
    look[0] = mPosition.x;
    look[1] = mPosition.y;
    look[2] = mPosition.z + 50.f;
}


//Accessors and Mutators
float&          Camera::NearDist()
{
    return mNearDist;
}

const float&    Camera::NearDist() const
{
    return mNearDist;
}

void			Camera::SetNearDistance( const float& neardist )
{
	mNearDist = neardist;
    perspectiveChanged = true;
}


float&          Camera::FarDist()
{
    return mFarDist;
}

const float&    Camera::FarDist() const
{
    return mFarDist;
}

void    Camera::SetFarDistance( const float& fardist )
{
	mFarDist = fardist;
    perspectiveChanged = true;
}


float&          Camera::AspectRatio()
{
    return mAspectRatio;
}

const float&    Camera::AspectRatio() const
{
    return mAspectRatio;
}

void		Camera::SetAspectRatio( const float& aspect )
{
	mAspectRatio = aspect;
    perspectiveChanged = true;
}


float&          Camera::FieldOfView()
{
    return mFieldOfView;
}

const float&    Camera::FieldOfView() const
{
    return mFieldOfView;
	
}

void		Camera::SetFieldOfView( const float& fov )
{
    mFieldOfView = fov;
    perspectiveChanged = true;
}


CVector3&	Camera::Position()
{
	return mPosition;
}
const CVector3&	Camera::Position() const
{
	return mPosition;
}

void	Camera::SetPosition( const CVector3& v )
{
    mPosition = v;
}


CVector3	Camera::GetPosition2D( )
{
    return mPosition2D;
}

void	Camera::SetPosition2D( const CVector3& v )
{	
    mPosition2D = v;
}


const CVector3&	Camera::Side() const
{
	return mSide;
}

const CVector3&	Camera::Up() const
{
	return mUp;
}

const CVector3&	Camera::Look() const
{
	return mLook;
}

CVector3&		Camera::LookPoint()
{
	return mLookAtPoint;
}

void	Camera::SetLookPoint( const CVector3& p )
{
    mLookAtPoint = p;
}


bool&	Camera::PChanged()
{
	return perspectiveChanged;
}

const bool&	Camera::PChanged() const
{
	return perspectiveChanged;
}


bool&	Camera::PType()
{
	return perspectiveProj;
}

const bool& Camera::PType() const
{
	return perspectiveProj;
}

void	Camera::ChangeProjectionType( bool type)
{
	perspectiveProj = type;
	perspectiveChanged = true;
}

void	Camera::ToggleProjectionType( )
{
	perspectiveProj = !perspectiveProj;
	perspectiveChanged = true;
}


Matrix4	Camera::GetProjectionMatrix()
{
	Matrix4 proj;
	float matrix[16] = {0};
	GetProjectionMatrix(matrix);
	proj.setAllElements(matrix);
	return proj;
}

void Camera::GetProjectionMatrix(float* matrix)
{
	GLfloat proj[16] = {};

	glGetFloatv(GL_PROJECTION_MATRIX, proj);
	
	//Go through and set one for one the new matrix.
	CRenderModule::GLMatrixToFloatMatrix(proj, matrix);
}
void Camera::glMakeViewMatrix( GLfloat* matrix )
{
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
void Camera::makeViewMatrix( float* matrix )
{
	GLfloat view[16] = {0};
	glMakeViewMatrix(view);
	CRenderModule::GLMatrixToFloatMatrix(view, matrix);
}


bool 	Camera::IsInverted() const
{
	return mInverted;
}

bool    Camera::IsOrbit() const
{
    return mOrbit;
}


//Easy camera movement functions.
void    Camera::TranslateForward( float speed )
{
    mPosition = mPosition + mLook * speed;
}

void    Camera::TranslateUp( float speed )
{	
	if( perspectiveProj )
	{
		mPosition = mPosition + mUp * speed;
	}
	else
	{
		mPosition2D = mPosition2D + principleAxes[1] * speed * (1.0f / mZoomFactor2D);
	}
}

void    Camera::TranslateRight( float speed )
{
	if( perspectiveProj )
	{
		mPosition = mPosition + mSide * speed;
	}
	else
	{
		mPosition2D = mPosition2D + principleAxes[0] * speed * ( 1.0f / mZoomFactor2D);
	}
}


///Zooming features.... 2D for now.
void	Camera::ZoomIn2D()
{
	mZoomFactor2D += mZoomInc;

	if( mZoomFactor2D > mMaxZoomFactor2D )
	{
		mZoomFactor2D = 4.0f;
	}
}

void	Camera::ZoomOut2D()
{
	mZoomFactor2D -= mZoomInc;
	
	if( mZoomFactor2D < mMinZoomFactor2D )
	{
		mZoomFactor2D = 0.5f;
	}
}

void	Camera::ZoomIn2D( float zoomAmount )
{	
	mZoomFactor2D += zoomAmount;
}

void	Camera::ZoomOut2D( float zoomAmount )
{
	mZoomFactor2D -= zoomAmount;
}


void	Camera::SetZoomFactor2D(float zoomAmount)
{
	mZoomFactor2D = zoomAmount;
}

float	Camera::GetZoomFactor2D()
{
	return mZoomFactor2D;
}


int	Camera::Height() const
{
	return mWinHeight;
}

int	Camera::Width() const
{
	return mWinWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////Camer Lua Lunar bind//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_LUA
LUA_START_DEFINE_CLASS( Camera )
	LUA_NAME_METHOD( Camera, "NearDist", LNearDist )
	LUA_NAME_METHOD( Camera, "FarDist", LFarDist )
	LUA_NAME_METHOD( Camera, "AspectRatio", LAspectRatio )
	LUA_NAME_METHOD( Camera, "FieldOfView", LFieldOfView )
	LUA_NAME_METHOD( Camera, "SetNearDistance", LSetNearDistance )
	LUA_NAME_METHOD( Camera, "SetFarDistance", LSetFarDistance )
	LUA_NAME_METHOD( Camera, "SetAspectRatio", LSetAspectRatio )
	LUA_NAME_METHOD( Camera, "SetFieldOfView", LSetFieldOfView )
	LUA_NAME_METHOD( Camera, "Position", LPosition )
	LUA_NAME_METHOD( Camera, "Side", LSide )
	LUA_NAME_METHOD( Camera, "Up", LUp )
	LUA_NAME_METHOD( Camera, "Look", LLook )
	LUA_NAME_METHOD( Camera, "LookPoint", LLookPoint )
	LUA_NAME_METHOD( Camera, "PChanged", LPChanged )
	LUA_NAME_METHOD( Camera, "PType", LPType )
	LUA_NAME_METHOD( Camera, "ChangeProjectionType", LChangeProjectionType )
	LUA_NAME_METHOD( Camera, "ToggleProjectionType", LToggleProjectionType )
	LUA_NAME_METHOD( Camera, "SetPosition", LSetPosition )
	LUA_NAME_METHOD( Camera, "SetPosition2D", LSetPosition2D )
	LUA_NAME_METHOD( Camera, "GetPosition2D", LGetPosition2D )
	LUA_NAME_METHOD( Camera, "SetLookPoint", LSetLookPoint )
	LUA_NAME_METHOD( Camera, "ChangePitch", LChangePitch )
	LUA_NAME_METHOD( Camera, "ChangeHeading", LChangeHeading )
	LUA_NAME_METHOD( Camera, "ChangeForwardVel", LChangeForwardVel )
	LUA_NAME_METHOD( Camera, "ChangeUpVel", LChangeUpVel )
	LUA_NAME_METHOD( Camera, "ChangeSideVel", LChangeSideVel )
	LUA_NAME_METHOD( Camera, "IsInverted", LIsInverted )
	LUA_NAME_METHOD( Camera, "IsOrbit", LIsOrbit )
	LUA_NAME_METHOD( Camera, "TranslateForward", LTranslateForward )
	LUA_NAME_METHOD( Camera, "TranslateUp", LTranslateUp )
	LUA_NAME_METHOD( Camera, "TranslateRight", LTranslateRight )
	LUA_NAME_METHOD( Camera, "ZoomIn2D", LZoomIn2D )
	LUA_NAME_METHOD( Camera, "ZoomOut2D", LZoomOut2D )
	LUA_NAME_METHOD( Camera, "SetZoomFactor2D", LSetZoomFactor2D )
	LUA_NAME_METHOD( Camera, "GetZoomFactor2D", LGetZoomFactor2D )
LUA_END_DEFINE_CLASS



LUA_DEFINE_METHOD( Camera, LNearDist )
{
	if( !ls ) { return 0;}

	lua_pushnumber(ls, NearDist() );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LFarDist )
{	
	if( !ls ) { return 0;}

	lua_pushnumber(ls, FarDist() );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LAspectRatio )
{
	if( !ls ) { return 0;}

	lua_pushnumber(ls, AspectRatio() );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LFieldOfView )
{	
	if( !ls ) { return 0;}

	lua_pushnumber(ls, FieldOfView() );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetNearDistance )
{
	if( !ls ) { return 0;}

	float value = (float)lua_tonumber(ls,1);

	SetNearDistance(value);

	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetFarDistance )
{	
	if( !ls ) { return 0;}

	float value = (float)lua_tonumber(ls,1);

	SetFarDistance(value);

	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetAspectRatio )
{
	if( !ls ) { return 0;}

	float value = (float)lua_tonumber(ls,1);

	SetAspectRatio(value);

	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetFieldOfView )
{	
	if( !ls ) { return 0;}

	float value = (float)lua_tonumber(ls,1);

	SetFieldOfView(value);

	return 0;
}

LUA_DEFINE_METHOD( Camera, LPosition )
{
	if( !ls ) { return 0;}

	CVector3 vec = Position();
	lua_pushnumber(ls, vec.x);
	lua_pushnumber(ls, vec.y);
	lua_pushnumber(ls, vec.z);

	return 3;
}

LUA_DEFINE_METHOD( Camera, LSide )
{	
	if( !ls ) { return 0;}

	CVector3 vec = Side();
	lua_pushnumber(ls, vec.x);
	lua_pushnumber(ls, vec.y);
	lua_pushnumber(ls, vec.z);

	return 3;
}

LUA_DEFINE_METHOD( Camera, LUp )
{
	if( !ls ) { return 0;}

	CVector3 vec = Up();
	lua_pushnumber(ls, vec.x);
	lua_pushnumber(ls, vec.y);
	lua_pushnumber(ls, vec.z);

	return 3;
}

LUA_DEFINE_METHOD( Camera, LLook )
{	
	if( !ls ) { return 0;}

	CVector3 vec = Look();
	lua_pushnumber(ls, vec.x);
	lua_pushnumber(ls, vec.y);
	lua_pushnumber(ls, vec.z);

	return 3;
}

LUA_DEFINE_METHOD( Camera, LLookPoint )
{
	if( !ls ) { return 0;}

	CPoint3 vec = LookPoint();
	lua_pushnumber(ls, vec.x);
	lua_pushnumber(ls, vec.y);
	lua_pushnumber(ls, vec.z);

	return 3;
}

LUA_DEFINE_METHOD( Camera, LPChanged )
{	
	if( !ls ) { return 0;}

	lua_pushboolean(ls, PChanged() );

	return 1;
}

LUA_DEFINE_METHOD( Camera, LPType )
{
	if( !ls ) { return 0;}

	lua_pushnumber(ls, PType() ? 1 : 0 );

	return 1;
}

LUA_DEFINE_METHOD( Camera, LChangeProjectionType )
{	
	if( !ls ) { return 0;}

	ChangeProjectionType( lua_tointeger(ls,1) ? true : false );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LToggleProjectionType )
{
	if( !ls ) { return 0;}

	ToggleProjectionType();

	//
	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetPosition )
{	
	if( !ls ) { return 0;}

	CVector3 p;
	p.x = (float)lua_tonumber(ls,1);
	p.y = (float)lua_tonumber(ls,2);
	p.z = (float)lua_tonumber(ls,3);

	SetPosition(p);

	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetPosition2D )
{	
	if( !ls ) { return 0;}

	CVector3 p;
	p.x = (float)lua_tonumber(ls,1);
	p.y = (float)lua_tonumber(ls,2);
	p.z = (float)lua_tonumber(ls,3);
	p.z = mPosition.z;

	SetPosition2D(p);

	return 0;
}

LUA_DEFINE_METHOD( Camera, LGetPosition2D )
{	
	if( !ls ) { return 0;}

	CVector3 p = GetPosition2D();
	
	lua_pushnumber(ls,p.x);
	lua_pushnumber(ls,p.y);

	return 2;
}

LUA_DEFINE_METHOD( Camera, LSetLookPoint )
{
	if( !ls ) { return 0;}

	CVector3 p;
	p.x = (float)lua_tonumber(ls,1);
	p.y = (float)lua_tonumber(ls,2);
	p.z = (float)lua_tonumber(ls,3);

	SetPosition(p);
	return 0;
}

LUA_DEFINE_METHOD( Camera, LChangePitch )
{	
	if( !ls ) { return 0;}

	ChangePitch( (float)lua_tonumber(ls, 1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LChangeHeading )
{
	if( !ls ) { return 0;}

	ChangeHeading( (float)lua_tonumber(ls, 1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LChangeForwardVel )
{	
	if( !ls ) { return 0;}

	ChangeForwardVel((float)lua_tonumber(ls, 1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LChangeUpVel )
{
	if( !ls ) { return 0;}

	ChangeForwardVel((float)lua_tonumber(ls, 1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LChangeSideVel )
{	
	if( !ls ) { return 0;}

	ChangeForwardVel((float)lua_tonumber(ls, 1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LIsInverted )
{	
	if( !ls ) { return 0;}

	lua_pushboolean(ls, IsInverted() );

	return 1;
}

LUA_DEFINE_METHOD( Camera, LIsOrbit )
{
	if( !ls ) { return 0;}

	lua_pushboolean(ls, IsOrbit() );

	return 1;
}

LUA_DEFINE_METHOD( Camera, LTranslateForward )
{	
	if( !ls ) { return 0;}

	TranslateForward( (float)lua_tonumber(ls,1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LTranslateUp )
{
	if( !ls ) { return 0;}
	
	TranslateUp( (float)lua_tonumber(ls,1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LTranslateRight )
{	
	if( !ls ) { return 0;}
	
	TranslateRight( (float)lua_tonumber(ls,1) );

	return 0;
}

LUA_DEFINE_METHOD( Camera, LZoomIn2D )
{
	if( !ls ) { return 0;}

	int stackSize = lua_gettop(ls);
	if( stackSize == 1 )
	{
		ZoomIn2D( (float)lua_tonumber(ls,1) );		
	}
	else
	{
		ZoomIn2D();
	}

	return 0;
}

LUA_DEFINE_METHOD( Camera, LZoomOut2D )
{	
	if( !ls ) { return 0;}

	int stackSize = lua_gettop(ls);
	if( stackSize == 1 )
	{
		ZoomOut2D( (float)lua_tonumber(ls,1) );		
	}
	else
	{
		ZoomOut2D();
	}

	return 0;
}

LUA_DEFINE_METHOD( Camera, LSetZoomFactor2D )
{
	if( !ls ) { return 0;}

	int stackSize = lua_gettop(ls);
	if( stackSize == 1 )
	{
		SetZoomFactor2D( (float)lua_tonumber(ls,1) );
	}

	return 0;
}

LUA_DEFINE_METHOD( Camera, LGetZoomFactor2D )
{	
	if( !ls ) { return 0;}

	lua_pushnumber(ls, GetZoomFactor2D() );	

	return 1;
}

#endif
