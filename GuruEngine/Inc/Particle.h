/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef PARTICLE_H
#define PARTICLE_H


#include <string>
#include <list>
#include <cmath>
#include <stdlib.h>
#include "Utility.h"
#include "DQuaternion.h"
#include "gl_inc.h"


//Some helper random functions for generating floats and ints easy.
template<typename T>
inline float RandFloat(T min, T max)
{
	return ((max - min) * rand()/(float)RAND_MAX + min);
}

// Random int generator
template<typename T>
inline int RandInt(T min, T max)
{
	return (int)((max - min) * rand()/(float)RAND_MAX + min);
}


class Particle
{
public:
	// Constructors/Destructors
	//
	Particle( );
	virtual ~Particle( );

	//Function: Update
	//@brief: Function to update the particle.  The various group update functions will be used.
	//@param: time   The timeslice for this particle to update over.
	void Update(float time);

	//Function: Update
	//@brief: Function to update the particle.  The various group update functions will be used.
	//@param: time   The timeslice for this particle to update over.
	//@return: float    Returns the depth calculated.  This is also stored internally in the particle,
	//                  but the return value is here for any other uses without accessing the particles directly.
	float ComputeDepth(CVector3 camPos = CVector3(0,0,0));

    inline
    bool operator<(const Particle& rhs)
    {
        return (this->mDepthSq < rhs.mDepthSq);
    }

	// Public attributes
	// The current position of the particle.
	CVector3 mPos;
	// Velocity of the particle.  Will be used much later when force appliers get involved.
	CVector3 mVel;
	// The color value of this particle.
	CVector3 mColor;
	// The amount of time this particle has been alive.  When exceeds mTimeToLive, particle is destroyed.
	float mTimeAlive;
	// The amount of time this particle will be alive, in seconds.
	float mTimeToLive;
	// Position last frame for this particle.
	CVector3 mOldPosition;
	// Relative size of the particle.  Used to determine the offset points of the sprite/quad.
	float mSize;
	// A rotation value for an offset of the particle texture.
	float mRotation;
	// The depth squared, as this way no need to calulate square root.
	float mDepthSq;
	// Determines if the particle is dead or not.  When particle is created, this is set true.  When mTimeAlive > mTimeToLive, this is set to false.
	bool mIsAlive;
	// To control whether additive or multiplicative blending is used.  false means multiplicative.
	bool mIsAdditive;

private:
	void initAttributes();
};


#endif // PARTICLE_H
