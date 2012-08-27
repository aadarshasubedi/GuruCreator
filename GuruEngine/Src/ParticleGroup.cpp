/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ParticleGroup.cpp
Brief:      
/************************************************************************/


#include "ParticleGuru.h"


// Constructors/Destructors
//

ParticleGroup::ParticleGroup ( ) {
initAttributes();
}

ParticleGroup::~ParticleGroup ( ) { }

//
// Methods
//


// Accessor methods
//


// Other methods
//


/**
 * @param  mUpdateAmount The timeslice by which to update the particle simulation.
 */
void ParticleGroup::Update (float mUpdateAmount )
{
    //Iterate over all of the particles, see what corresponding update funcs are set, and call them
    // with the appropriate data.  Once the individual property updates have been performed, the position
    // update (main update) can be performed.
    particle_list::iterator iter_begin = mParticles.begin();
    particle_list::const_iterator iter_end = mParticles.end();

    while( iter_begin != iter_end )
    {
        //iter_begin now points to a particle.  So now check the group update functions to see how to update this.
        Particle* par = &(*iter_begin);

        if( par->mIsAlive )
        {
            if( m_SizeUpdateFunc != NULL )
            {
                (*m_SizeUpdateFunc)( mUpdateAmount, par->mTimeAlive, par->mTimeToLive, mSizeUpdateParams, par->mSize );
            }

            if( m_ColorUpdateFunc != NULL )
            {
                (*m_ColorUpdateFunc)( mUpdateAmount, par->mTimeAlive, par->mTimeToLive, mColUpdateParams, par->mColor );
            }

            if( m_RotationUpdateFunc != NULL )
            {
                (*m_RotationUpdateFunc)( mUpdateAmount, par->mTimeAlive, par->mTimeToLive, mRotUpdateParams, par->mRotation);
            }

            if( m_VelocityUpdateFunc != NULL )
            {
                (*m_VelocityUpdateFunc)( mUpdateAmount, par->mTimeAlive, par->mTimeToLive, mVelUpdateParams, par->mVel);
            }

            par->Update( mUpdateAmount );
        }
        else
        {
            particle_list::iterator tempIt = iter_begin;
            ++tempIt;
            mParticles.erase(iter_begin);
            if(mParentSys)
            {
                mParentSys->DecParticleCount();
            }
            iter_begin = tempIt;
        }

        ++iter_begin;
    }

    if( mContinuous )
    {
        SpawnParticles( mUpdateAmount );
    }
}


/**
 * @param  mSpawnTime The amount of time to spawn particles.
 */
void ParticleGroup::SpawnParticles (float mSpawnTime )
{
    //Over the period of spawnTime, create particles and depending on the spawn functions and their validity of pointers, the
    // values will be set initially.  This should look similar to the update function but with particle creation.

    //The creation also depends on the particle density.
    for( unsigned iter = 0; iter < (unsigned)mParticleDensity; ++iter )
    {
        Particle* par = CreateParticle();

        if( m_SizeSpawnFunc != NULL )
        {
            (*m_SizeSpawnFunc)( mParentSys, mSpawnTime, mSizeSpawnParams, par->mSize );
        }
        if( m_ColorSpawnFunc != NULL )
        {
            (*m_ColorSpawnFunc)( mParentSys, mSpawnTime, mColSpawnParams, par->mColor );
        }
        if( m_VelocitySpawnFunc != NULL )
        {
            (*m_VelocitySpawnFunc)( mParentSys, mSpawnTime, mVelSpawnParams, par->mVel );
        }
        if( m_RotationSpawnFunc != NULL )
        {
            (*m_RotationSpawnFunc)( mParentSys, mSpawnTime, mRotSpawnParams, par->mRotation);
        }
        if( m_PositionSpawnFunc!= NULL )
        {
            (*m_PositionSpawnFunc)( mParentSys, mSpawnTime, mPosSpawnParams, par->mPos );
        }
        if( m_LifeSpawnFunc!= NULL )
        {
            (*m_LifeSpawnFunc)( mParentSys, mSpawnTime, mLifeSpawnParams, par->mTimeToLive );
        }
    }
}

/**
 * @return Particle*
 */
Particle* ParticleGroup::CreateParticle ( )
{
    //Create a particle and push it back to the vector.
    mParticles.push_back( Particle() );

    if( mParentSys )
    {
        mParentSys->IncParticleCount();
    }

    Particle* p = &(mParticles.back());

    return p;
}




/**
 * @param  camPos The camera position.  This is needed in order to perform the
 * depth sorting.
 */
void ParticleGroup::Render (CVector3 camPos )
{
    //Since the group is where the actual particle data is stored, here is where the particles
    // will be updated into the DObject's master list.

    particle_list::iterator begin = mParticles.begin();
    particle_list::const_iterator end = mParticles.end();

    //If the parent system is sorted already, simply update the depths.
    if( mParentSys->IsSorted() )
    {
        //Go through the list and compute the depths.
        while( begin != end )
        {
            (*begin).ComputeDepth( camPos );
            ++begin;
        }
    }
    else
    {
        //Update the parent systems depth, and then set the depths based on that.
        mParentSys->ComputeDepth( camPos);

        while( begin != end )
        {
            (*begin).mDepthSq = mParentSys->GetDepth();
            ++begin;
        }
    }

    //This call adds all of the particles to the sorter list to be sorted at the start of the rendering portion.
    ParticleGuru::GetSingletonPtr()->AddGroupToRender( &(this->mParticles) );
}


/**
 * @return float
 */
float ParticleGroup::GetDensity ( )
{
    return mParticleDensity;
}


/**
 * @return bool
 */
bool ParticleGroup::GetAdditive ( )
{
    return mIsAdditive;
}


/**
 * @return bool
 */
bool ParticleGroup::GetContinuous ( )
{
    return mContinuous;
}


/**
 * @return ParticleSystem*
 */
ParticleSystem* ParticleGroup::GetParentSystem ( )
{
    return mParentSys;
}


/**
 * @return GLint
 */
GLint ParticleGroup::GetTextureId ( )
{
    return mTextureid;
}


/**
 * @return GLint
 */
GLint ParticleGroup::GetProgramId ( )
{
    return mEffectProgram;
}


/**
 * @return Particle*
 */
ParticleGroup::particle_list* ParticleGroup::GetParticleList ( )
{
    return &mParticles;
}


/**
 * @param  in_density
 */
void ParticleGroup::SetDensity (float in_density )
{
    mParticleDensity = in_density;
}


/**
 * @param  in_additive
 */
void ParticleGroup::SetAdditive (bool in_additive )
{
    mIsAdditive = in_additive;
}


/**
 * @param  in_continuous
 */
void ParticleGroup::SetContinuous (bool in_continuous )
{
    mContinuous = in_continuous;
}


/**
 */
void ParticleGroup::ToggleContinuous ( )
{
    mContinuous = !mContinuous;
}


/**
 * @param  in_program
 */
void ParticleGroup::SetProgramId (GLint in_program )
{
    mEffectProgram = in_program;
}


/**
 * @param  in_textureid
 */
void ParticleGroup::SetTextureId (GLint in_textureid )
{
    mTextureid = in_textureid;
}


/**
 * @param  in_parentsys
 */
void ParticleGroup::SetParentSystem (ParticleSystem* in_parentsys )
{
    mParentSys = in_parentsys;
}


/**
 */
void ParticleGroup::ClearAllParticles ( )
{
    //Since the vector holds actual particles and not pointers, calling clear is sufficient since that will call all destructors
    // and then clear the list.
    mParticles.clear();
}


/**
 * @param  in_params
 */
void ParticleGroup::SetSizeParams (const float* in_sizeparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mSizeSpawnParams, in_sizeparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  in_colparams
 */
void ParticleGroup::SetColorParams (const float* in_colparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mColSpawnParams, in_colparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  in_posparams
 */
void ParticleGroup::SetPosParams (const float* in_posparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mPosSpawnParams, in_posparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  in_velparams
 */
void ParticleGroup::SetVelParams (const float* in_velparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mVelSpawnParams, in_velparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  in_rotparams
 */
void ParticleGroup::SetRotParams (const float* in_rotparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mRotSpawnParams, in_rotparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  in_lifeparams
 */
void ParticleGroup::SetLifeParams (const float* in_lifeparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mLifeSpawnParams, in_lifeparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  up_sizeparams
 */
void ParticleGroup::SetSizeUpdateParams (const float* up_sizeparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mSizeUpdateParams, up_sizeparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  up_rotparams
 */
void ParticleGroup::SetRotUpdateParams (const float* up_rotparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mRotUpdateParams, up_rotparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  up_velparams
 */
void ParticleGroup::SetVelUpdateParams (const float* up_velparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mVelUpdateParams, up_velparams, sizeof(float)*NUM_PARAMS );
}


/**
 * @param  up_colparams
 */
void ParticleGroup::SetColUpdateParams (const float* up_colparams )
{
    //Could use a for loop to iterate over NUM_PARAM elements, but easier to use a memcpy.
    memcpy( mColUpdateParams, up_colparams, sizeof(float)*NUM_PARAMS );
}

void ParticleGroup::initAttributes ( )
{
	mEffectProgram = 0;
	mTextureid = 0;
	mContinuous = false;
	mIsAdditive = false;
	mParticleDensity = 1.0f;
	mParentSys = NULL;
}