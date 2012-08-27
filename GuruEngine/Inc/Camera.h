/*
Author:		Dan Strycharske
Date:		06.05.09
File:		Camera.h
Brief:      A generic camera class, to be used most anywhere.
*/

#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED


#include "Utility.h"
#include "DQuaternion.h"
#include "CRenderModule.h"
#include "lunar.h"


class Camera
{
//The camera class needs to have aspect ratio, near and far plane,
// and the FOV.  All necessary transforms will be generated from this data./home/dan/Projects/TheGuruEngine/inc/Utility

public:

    //Initializing functions.  Setting the camera up in various ways.
    Camera();
    Camera( CRenderModule::RMOptions options );

    ~Camera();

    void	Init( float np, float fp, float ar, float fv, const CVector3& p );

    //Accessors and Mutators
    float&          NearDist();
    const float&    NearDist() const;

    float&          FarDist();
    const float&    FarDist() const;

    float&          AspectRatio();
    const float&    AspectRatio() const;

    float&          FieldOfView();
    const float&    FieldOfView() const;

    void    SetNearDistance( const float& neardist );
    void    SetFarDistance( const float& fardist );
    void    SetAspectRatio( const float& aspect );
    void    SetFieldOfView( const float& fov );

	CVector3&		Position();
	const CVector3&	Position() const;

	const CVector3&	Side() const;

	const CVector3&	Up() const;

	const CVector3&	Look() const;

	CPoint3&		LookPoint();
	const CVector3& LookPoint() const;

	bool&			PChanged();
	const bool&		PChanged() const;

	bool&			PType();
	const bool&		PType() const;

	void			ChangeProjectionType( bool type );
	void			ToggleProjectionType( );

	//Getting the projection matrix.
	static Matrix4	GetProjectionMatrix();
	static void GetProjectionMatrix(float* matrix);

	//These will be the getter for OpenGL, since GL usually requires an array of floats
	// rather then user-created classes.

    void    SetPosition( const CVector3& v );
	void	SetPosition2D( const CVector3& v );
	CVector3	GetPosition2D();

    void	SetLookPoint( const CVector3& v );

	//NEW QUATERNION METHODS!!/////////// NOT BOUND TO LUA
    CVector3	SetPerspective();
	/////////////////////////////////////

    void	ChangePitch( float degree );
    void	ChangeHeading( float degree );
    void	ChangeForwardVel( float speed );
    void	ChangeUpVel( float speed );
    void	ChangeSideVel( float speed );

    void    glGetEyePt( GLfloat* eye );
    void    glGetLookPt( GLfloat* look );

	/////////////////////////////////////

    bool	IsInverted() const;
    bool    IsOrbit() const;

    //Easy camera movement functions.
    void    TranslateForward( float speed );
    void    TranslateUp( float speed );
    void    TranslateRight( float speed );

    void    glMakeViewMatrix( GLfloat* matrix );
	void	makeViewMatrix( float* matrix );


	///Zooming features.... 2D for now.
	void	ZoomIn2D();
	void	ZoomOut2D();

	void	ZoomIn2D( float zoomAmount );
	void	ZoomOut2D( float zoomAmount );

	void	SetZoomFactor2D(float zoomAmount);
	float	GetZoomFactor2D();

	int	Height() const;
	int	Width() const;
	int mWinWidth;
	int mWinHeight;

	//Set these up for 3D as well.

protected:

private:
    //Distance to the near plane.
    float mNearDist;

    //Distance to the far plane.
    float mFarDist;

    //The aspect ratio, given as width / height.
    float mAspectRatio;

    //The FOV of the camera, given as an angle in radians.
    float mFieldOfView;
    //The previous 4 values define the Perspective Projection matrix.

	float mZoomFactor2D;
	float mZoomInc;
	float mMaxZoomFactor2D;
	float mMinZoomFactor2D;

    //The camera also needs to establish the view matrix, and this is handled through
    // the camera position and orientation.
    CVector3 mPosition;
	CVector3 mPosition2D; //The x-y coords are used, not z.  For 2D games.
    CVector3 mLook, mSide, mUp;
    CPoint3 mLookAtPoint;

    DQuaternion mPitch;
    DQuaternion mHeading;

    bool perspectiveProj;
    bool perspectiveChanged;

	bool mInverted, mOrbit;

    float pitchSpeed, headingSpeed;
    float moveSpeed, sideSpeed, upSpeed, curSpeed;
    float pitchDegree, headingDegree;

    int w,h;

	//@HACK:  Using booleans for the bitmasks below til i feel like getting to them.
	bool freecam;
	bool freelook;
	bool tracking;
	bool following;

	//@todo:
    //int  cameraModes;  //This is going to be used as a bitmask for certain properties.

    //Orbit/Freecam.
    //Freelook/Y-rot only
    //Non-tracking/tracking -- this is for tracking objects of significance in the scene.  Non-tracking
	//							means camera will be stationary, unless the follow player flag is set.
	//Follow/Idle	--Follows the main character in scene or is a stationary camera.  Essentially steady cam
	//					versus a rigged camera.  @NOTE: To be controlled on a spline, must be IDLE.
    //

	//////////////////////////////////////////////////////////////////////////////////
#ifdef USE_LUA
	LUA_DECLARE_CLASS( Camera )
		LUA_DECLARE_METHOD( LNearDist )
		LUA_DECLARE_METHOD( LFarDist )
		LUA_DECLARE_METHOD( LAspectRatio )
		LUA_DECLARE_METHOD( LFieldOfView )
		LUA_DECLARE_METHOD( LSetNearDistance )
		LUA_DECLARE_METHOD( LSetFarDistance )
		LUA_DECLARE_METHOD( LSetAspectRatio )
		LUA_DECLARE_METHOD( LSetFieldOfView )
		LUA_DECLARE_METHOD( LPosition )
		LUA_DECLARE_METHOD( LSide )
		LUA_DECLARE_METHOD( LUp )
		LUA_DECLARE_METHOD( LLook )
		LUA_DECLARE_METHOD( LLookPoint )
		LUA_DECLARE_METHOD( LPChanged )
		LUA_DECLARE_METHOD( LPType )
		LUA_DECLARE_METHOD( LChangeProjectionType )
		LUA_DECLARE_METHOD( LToggleProjectionType )
		LUA_DECLARE_METHOD( LSetPosition )
		LUA_DECLARE_METHOD( LSetPosition2D )
		LUA_DECLARE_METHOD( LGetPosition2D )
		LUA_DECLARE_METHOD( LSetLookPoint )
		LUA_DECLARE_METHOD( LChangePitch )
		LUA_DECLARE_METHOD( LChangeHeading )
		LUA_DECLARE_METHOD( LChangeForwardVel )
		LUA_DECLARE_METHOD( LChangeUpVel )
		LUA_DECLARE_METHOD( LChangeSideVel )
		LUA_DECLARE_METHOD( LIsInverted )
		LUA_DECLARE_METHOD( LIsOrbit )
		LUA_DECLARE_METHOD( LTranslateForward )
		LUA_DECLARE_METHOD( LTranslateUp )
		LUA_DECLARE_METHOD( LTranslateRight )
		LUA_DECLARE_METHOD( LZoomIn2D )
		LUA_DECLARE_METHOD( LZoomOut2D )
		LUA_DECLARE_METHOD( LSetZoomFactor2D )
		LUA_DECLARE_METHOD( LGetZoomFactor2D )
#endif
};


#endif // CAMERA_INCLUDED
