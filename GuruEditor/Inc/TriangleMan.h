

#ifndef TRIANGLEMAN_H
#define TRIANGLEMAN_H

#include "Utility.h"
#include "GameObject.h"



class TriangleMan : public GameObject
{

public:
	TriangleMan(float radius = 10.0f, const CVector3& center = CVector3(0,0,0) );

	TriangleMan(const TriangleMan& ){}
	~TriangleMan() {}

	virtual void initRenderObject( RenderComponent* comp );
	virtual void initPhysicsObject( PhysicsComponent* physComp );

	//Quick access to the thruster direction.
	CVector3 ThrustingDir();

	//Quick access to the attacking direction.
	CVector3 AttackingDir();

	//Render function.
	void DrawTriMan();
	void DrawBall();
	void DrawRings(float playback = 2.5f );

	void DrawRegions();

	//Animate function.  This updates the triangle points based off time elapsed and total animation time.
	void AnimateTriMan();

	void Update( float time );
	void Render();

	//Thrusting function.  Stops rotation but updates position based off thrusting direction.  
	void ThrustingTriMan();

	//Attacking function.  Stops rotation but moves player forward quickly over the attacking vector.
	void AttackingTriMan();

	void MagnetizingTriMan();

	//Green overrides red and blue.
	//Red overrides blue.
	//Blue overrides neither.
	unsigned CheckBallInRegion();
	void UpdateNormals();

	void ChangeSpinDirection();
	void StopSpinning();
	void StartSpinning();
	void CheckInput();
	
	void SetCenter( const CVector3& vec );

	//Needs the 4 points.
	CVector3 mPoints[3];
	CVector3 mCenter;

	CVector3 mPathColor; 
	CVector3 mGreenNormal;
	CVector3 mRedNormal;
	CVector3 mBlueNormal;
	CVector3 mRotatorPoint;
	CVector3 mSpreadPointOne;
	CVector3 mSpreadPointTwo;

	//@TODO: Exchange for new physics engine classes.
	//void SetBodyRef( b2Body* body ) { mBodyRef = body; }
	//b2Body*	mBodyRef;

	float mRadius;
	float mBallRadius;
	float mPathRadius;

	float mCurAngle;

	//signed short determines if reverse spin, normal spin, or stop spin.
	signed short mSpinType;
	signed short mPrevSpin;

	//unsigned int to determine the state, whether idle, or what action is being done: 0 - idle, 1 - thrusting, 2 - attacking, 3 - magnetizing.
	unsigned short mState;
	unsigned int mCurRegion;

	float mCurTime;
	float mPrevTime;
	static float totalSpinTime;

	float mBallCurTime;
	float mBallPrevTime;
	float totalBallTime;
};

#endif


