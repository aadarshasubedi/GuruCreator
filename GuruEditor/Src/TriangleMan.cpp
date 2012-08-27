


#include "TriangleMan.h"
#include "Game_Engine.h"
#include "Game_Renderer.h"





float TriangleMan::totalSpinTime = 2.0f;
float TriangleMan::totalBallTime = 2.0f;

TriangleMan::TriangleMan( float radius, const CVector3& center )
{
	mRadius = radius;
	mPathRadius = mRadius + 10.0f;
	mBallRadius = 2.0f;

	mCenter = center;

	mSpinType = -1;
	mPrevSpin = -1;
	mState = 0;

	mCurTime = 0.0f;
	mPrevTime = 0.0f;
	
	mBallCurTime = 0.0f;
	mBallPrevTime = 0.0f;

	float halfSide = 3.0f * mRadius / (2 * sqrt( 3.0f ));
	float littleRadius = sqrt( mRadius*mRadius - halfSide*halfSide );

	//top.
	mPoints[0] = CVector3( mCenter.x, mCenter.y + mRadius, 0.0f );
	//left
	mPoints[1] = CVector3( mCenter.x - halfSide, mCenter.y - littleRadius, 0.0f);
	//right
	mPoints[2] = CVector3( mCenter.x + halfSide, mCenter.y - littleRadius, 0.0f );

	mPathColor = CVector3( 0.0f, 0.0f, 0.0f );
	mSpreadPointOne = CVector3( 0.0f, 0.0f, 0.0f );
	mSpreadPointTwo = CVector3( 0.0f, 0.0f, 0.0f );

	//mBodyRef = NULL;	
	//mRenderComp = NULL;
	mRenderComponents.clear();
	mRenderComponents.resize(0);

	mCurRegion = 0;
}


void TriangleMan::initRenderObject( RenderComponent* comp )
{
	//if( mRenderComp )
	//{
	//	delete mRenderComp;
	//}
	//mRenderComp = new RenderComponent( this );


	float halfSide = 3.0f * mRadius / (2 * sqrt( 3.0f ));
	float littleRadius = sqrt( mRadius*mRadius - halfSide*halfSide );

	b2Vec2 points[3];
	//top.
	points[0] = b2Vec2( mCenter.x, mCenter.y + mRadius );
	//left
	points[1] = b2Vec2( mCenter.x - halfSide, mCenter.y - littleRadius);
	//right
	points[2] = b2Vec2( mCenter.x + halfSide, mCenter.y - littleRadius );

	//@todo: comment this out when the render manager is created.
	RenderMesh2D* mesh = (RenderMesh2D*)comp;
	//mRenderComponents.push_back( mesh );

	mesh->setVertices( 3 );
	mesh->vPos(0) = points[0];
	mesh->vPos(1) = points[1];
	mesh->vPos(2) = points[2];

	mesh->SetUseOneColor(true);
	mesh->SetOneColor( CVector3() );
	mesh->changeDrawType( RenderMesh2D::TRIANGLES );


}

void TriangleMan::initPhysicsObject( PhysicsComponent*   )
{	
	////Now create a dynamic object.
	b2BodyDef dynBodyDefine;
	dynBodyDefine.position.Set( 25.0f, 60.0f );
	dynBodyDefine.linearDamping = 0.15f;

	this->mPhysicsComponent->SetBody( &dynBodyDefine );
	this->mPhysicsComponent->AddSphereShape( 10.0f, PhysicsComponent::denFricRest(1.0f, 0.3f, 0.85f) );
	this->mPhysicsComponent->SetMass( 28.0f );

	this->mRenderComponents[0]->SetOffsetPos( b2Vec2(0,0) );
}


//Quick access to the thruster direction.
CVector3 TriangleMan::ThrustingDir()
{
	return (mPoints[1] - mCenter);
}

//Quick access to the attacking direction.
CVector3 TriangleMan::AttackingDir()
{
	return (mPoints[2] - mCenter);
}

void TriangleMan::SetCenter( const CVector3& vec )
{
	mCenter = vec;
	float halfSide = 3.0f * mRadius / (2 * sqrt( 3.0f ));
	float littleRadius = sqrt( mRadius*mRadius - halfSide*halfSide );

	mPoints[0] = CVector3( mCenter.x, mCenter.y + mRadius, 0.0f );
	//left
	mPoints[1] = CVector3( mCenter.x - halfSide, mCenter.y - littleRadius, 0.0f);
	//right
	mPoints[2] = CVector3( mCenter.x + halfSide, mCenter.y - littleRadius, 0.0f );

		
	float angle = ( (mCurTime) / totalSpinTime) * (float)(M_PI);
	mCurAngle = angle;

	float sinAng = sin(angle);
	float cosAng = cos(angle);	


	CVector3 temp;	
	for( unsigned i = 0; i <= 2; ++i )
	{
		mPoints[i] = mPoints[i] - vec;
		temp.x = mPoints[i].x * cosAng + mPoints[i].y * sinAng;
		temp.y = mPoints[i].x * -sinAng + mPoints[i].y * cosAng;
		mPoints[i] = temp + vec;
	}

}


//Animate function.  This updates the triangle points based off time elapsed and total animation time.
void TriangleMan::AnimateTriMan()
{		
	//float angle = ( (mCurTime-mPrevTime) / totalSpinTime) * (float)(2*M_PI);
	////mCurAngle = angle;

	//float sinAng = sin(angle);
	//float cosAng = cos(angle);	

	//CVector3 temp;	
	//for( unsigned i = 0; i <= 2; ++i )
	//{
	//	mPoints[i] = mPoints[i] - mCenter;
	//	temp.x = mPoints[i].x * cosAng + mPoints[i].y * sinAng;
	//	temp.y = mPoints[i].x * -sinAng + mPoints[i].y * cosAng;
	//	mPoints[i] = temp + mCenter;
	//}
}

void TriangleMan::Render()
{
	//DrawTriMan();
	DrawBall();
	//DrawRegions();
	//mRenderComp->render();
}

void TriangleMan::Update( float time )
{
	mPrevTime = mCurTime;
	mBallPrevTime = mBallCurTime;

	//Their idling, update the time.
	if( mState == 0 )
	{
		mCurTime += (time*mSpinType);		
		mBallCurTime += (time*mSpinType);
	}

	UpdateNormals();

	switch(mCurRegion)
	{
			//green
		case 0:
			{
				//Translate(  );
				mPathColor.z = mPathColor.x = 0.0f;
				mPathColor.y = 1.0f;
				break;
			}
			//red
		case 1:
			{
				//Do animation, mark attacking
				DrawRings();
				mPathColor.y = mPathColor.z = 0.0f;
				mPathColor.x = 1.0f;
				break;
			}
			//blue
		case 2:
			{
				DrawRings( );
				mPathColor.y = mPathColor.x = 0.0f;
				mPathColor.z = 1.0f;
				break;
			}
	}
}


//0-green, 1-red, 2-blue region codes.
unsigned TriangleMan::CheckBallInRegion()
{
	return 0;
}

void TriangleMan::UpdateNormals()
{	
	//Green area is between points 0 and 1.
	//Form vectors, take two dot products, knowing the angle to check is always 60 degrees, or pi / 3.
	if( mBallCurTime >= totalBallTime )
	{
		mBallCurTime = 0.0f;
	}
	float scalar = (mBallCurTime) / totalBallTime * (float)(2.0f * M_PI);
	float scalar2 = (mBallCurTime - 0.35f) / totalBallTime * (float)(2.0f * M_PI);
	float scalar3 = (mBallCurTime + 0.35f) / totalBallTime * (float)(2.0f * M_PI);

	float x = mPathRadius * sin( scalar );
	float y = mPathRadius * cos( scalar );

	float x2 = mPathRadius * sin( scalar2 );
	float y2 = mPathRadius * cos( scalar2 );

	float x3 = mPathRadius * sin( scalar3);
	float y3 = mPathRadius * cos( scalar3);


	mRotatorPoint = CVector3(x,y,0) + mCenter;
	mSpreadPointOne = CVector3(x2,y2,0) + mCenter;
	mSpreadPointTwo = CVector3(x3,y3,0) + mCenter;
}


//Thrusting function.  Stops rotation but updates position based off thrusting direction.  
void TriangleMan::ThrustingTriMan()
{
	CVector3 vec = (mRotatorPoint - mCenter).normal();
	mPhysicsComponent->AddImpulse( 25.0f * b2Vec2(vec.x, vec.y) );
}

//Attacking function.  Stops rotation but moves player forward quickly over the attacking vector.
void TriangleMan::AttackingTriMan()
{
}

void TriangleMan::MagnetizingTriMan()
{
}

void TriangleMan::ChangeSpinDirection()
{
	//He's idling so the spin can be reversed.  otherwise this will make now sense.
	mSpinType *= -1;
}


void TriangleMan::StopSpinning()
{
	if( 0 != mSpinType )
	{
		mPrevSpin = mSpinType;
		mSpinType = 0;
	}
}

void TriangleMan::StartSpinning()
{
	if( 0 == mSpinType )
	{
		mSpinType = mPrevSpin;
		mPrevSpin = mSpinType;
	}
}


void TriangleMan::CheckInput()
{
}


