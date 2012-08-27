/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       Particle.cpp
Brief:      
/************************************************************************/

#include "Particle.h"


// Constructors/Destructors
//

Particle::Particle ( ) {
initAttributes();
}

Particle::~Particle ( ) { }

//
// Methods
//
void Particle::Update( float time )
{
    //If the particle is dead, don't update.
    if( !mIsAlive )
    {
        return;
    }

    //If it's alive, increase it's time alive.  Then check if the time alive is past time to live.
    // If so, set mIsAlive and bail out here.
    mTimeAlive += time;
    if( mTimeAlive >= mTimeToLive )
    {
        mIsAlive = false;
        return;
    }

    //If all is well, then: x' = x + v*t
    mOldPosition = mPos;
    mPos = mPos + ( mVel * time );
}


float Particle::ComputeDepth( CVector3 camPos )
{
    //Use position to calculate depth, which is just distance from particle position to camera position.
    CVector3 vec( mPos - camPos );
    mDepthSq = vec.magnitude();

    return mDepthSq;
}


// Accessor methods
//


// Other methods
//

void Particle::initAttributes ( ) {
	mPos = CVector3(0,0,0);
	mVel = CVector3(0,0,0);
	mColor = CVector3(0,0,1);
	mTimeAlive = 0.0f;
	mTimeToLive = 2.0f;
	mOldPosition = CVector3(0,0,0);
	mSize = 1.0f;
	mRotation = 0.0f;
	mDepthSq = 0.0f;
	mIsAlive = true;
	mIsAdditive = false;
}

