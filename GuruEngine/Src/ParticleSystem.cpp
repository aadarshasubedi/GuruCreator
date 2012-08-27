/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ParticleSystem.cpp
Brief:      
/************************************************************************/

#include "ParticleSystem.h"


// Constructors/Destructors
//

ParticleSystem::ParticleSystem ( ) {
initAttributes();
}

ParticleSystem::~ParticleSystem ( ) { }

//
// Methods
//


// Accessor methods
//


// Other methods
//


/**
 * Update the particle systems based on their various update functions.
 * @param  in_timeslice The timeslice expired over this frame to update the
 * particle system and all groups/particles.
 */
void ParticleSystem::Update (float in_timeslice )
{
    //First make sure that the particle system is alive, and if it is alive that there are particles to update.
    if( !mIsAlive )
    {
        return;
    }

    //Go through the list of layers, and call the render function on them.
    particle_group::iterator g_begin = mParticleGroups.begin();
    particle_group::const_iterator g_end = mParticleGroups.end();

    while( g_begin != g_end )
    {
        //If the particle layer is alive and continuous, update.  If either are false, don't.
        ParticleGroup* parg = (*g_begin);

        parg->Update( in_timeslice );

        ++g_begin;
    }

    //Now that all the groups have been updated, update the age and such of the system itself.
    mTimeAlive += in_timeslice;
    if( (mTimeAlive >= mTimeToLive) && (mTimeToLive != 0))
    {
        KillSystem();
    }
}


/**
 * Function to render the particles.  Will perform sorts, setting up shader
 * programs necessary, textures, etc.
 * @param  in_campos The camera position is needed to render the particles due to
 * the sorting involved.  If blending, then the sort order is extremely important.
 *
 */
void ParticleSystem::Render (CVector3 in_campos )
{
    particle_group::iterator g_begin = mParticleGroups.begin();
    particle_group::const_iterator g_end = mParticleGroups.end();

    while( g_begin != g_end )
    {
        (*g_begin)->Render( in_campos );
        ++g_begin;
    }
}


/**
 * @return CVector3
 */
CVector3 ParticleSystem::GetVelocity ( )
{
    return mVelocity;
}


/**
 * @param  in_velocity
 */
void ParticleSystem::SetVelocity (CVector3 in_velocity )
{
    mVelocity = in_velocity;
}


/**
 * @return CVector3
 */
CVector3 ParticleSystem::GetPosition ( )
{
    return mPosition;
}


/**
 * @param  in_position
 */
void ParticleSystem::SetPosition (CVector3 in_position )
{
    mPosition = in_position;
}


/**
 * @return DQuaternion
 */
DQuaternion ParticleSystem::GetOrienation ( )
{
    return mOrientation;
}


/**
 * @param  in_orientation
 */
void ParticleSystem::SetOrientation (DQuaternion in_orientation )
{
    mOrientation = in_orientation;
}


/**
 * Returns true if the system is still alive and should be updated/rendered.
 * @return bool
 */
bool ParticleSystem::IsAlive ( )
{
    return (mIsAlive || (mTotalParticles > 0) );
}


/**
 * Returns true when the particles have been sorted and ready to render.
 * @return bool
 */
bool ParticleSystem::IsSorted ( )
{
    return mIsSorted;
}


/**
 * @param  in_sort
 */
void ParticleSystem::SetSorted (bool in_sort )
{
    mIsSorted = in_sort;
}


/**
 * Get life of particle system.
 * @return float
 */
float ParticleSystem::GetLife ( )
{
    return mTimeToLive;
}


/**
 * @param  in_lifetime
 */
void ParticleSystem::SetLife (float in_lifetime )
{
    mTimeToLive = in_lifetime;
}


/**
 * Returns system depth back.
 * @return float
 */
float ParticleSystem::GetDepth ( )
{
    return mDepthSq;
}


/**
 * This function computes the depth of the particle system.  This is then used as
 * the basis for the individual particles.
 * @param  in_campos Camera position needed since this function generates the depth
 * for the system.
 */
void ParticleSystem::ComputeDepth (CVector3 in_campos )
{
    CVector3 dist = mPosition - in_campos;
    mDepthSq = dist.magnitude();
}


/**
 * @return ParticleGroup*
 * @param  in_textureids
 * @param  in_numtextures
 * @param  in_density
 */
ParticleGroup* ParticleSystem::AddParticleGroup (GLint in_textureid, GLint in_program, float in_density )
{
    //Create a new particle group, set the appropriate textures and particle density.
    ParticleGroup* parg = new ParticleGroup();
    mParticleGroups.push_back( parg );

    //Set the needed values for the texture and program ids.
    parg->SetTextureId( in_textureid );
    parg->SetProgramId( in_program );

    //And when spawning begins density is needed from the start.
    parg->SetDensity( in_density );

    //Of course, the parent who owns the group, being this system. (this pointer)
    parg->SetParentSystem( this );

    return parg;
}


/**
 * Function used during destruction of the particle system.
 */
void ParticleSystem::ClearAllLayers ( )
{
    //Each group must clear out all of its particles.  Then the actual list can be cleared, calling
    // the destructors for everything needed.
    particle_group::iterator begin = mParticleGroups.begin();
    particle_group::const_iterator end = mParticleGroups.end();

    while( begin != end )
    {
        ParticleGroup* parg = (*begin);
        parg->ClearAllParticles();

        ++begin;
    }

    //Now clear the list, call destructors for the actual groups.
    mParticleGroups.clear();
}


/**
 * The death function to be called when the system needs to be destroyed.
 */
void ParticleSystem::KillSystem ( )
{
    mIsAlive = false;

    //Easieast way is to just stop them from continuously emitting, and then
    // when particle count reaches 0 the system will completely cleared.
    particle_group::iterator begin = mParticleGroups.begin();
    particle_group::const_iterator end = mParticleGroups.end();

    while( begin != end )
    {
        ParticleGroup* parg = (*begin);

        parg->SetContinuous( false );

        ++begin;
    }

    ClearAllLayers();
}


/**
 * Creates a single frames
 * @param  in_timeslice The timeslice of the frame for which to spawn.
 */
void ParticleSystem::PlaySingleFrame (float  )
{
    //First, make sure that the system is alive.  If it's alive, then it
    // must be checked to see if it's active.  This is because the system can
    // be paused for debugging or other purposes.
//    if( !mIsAlive && !mIsActive )
//    {
//        return;
//    }
//
//    //Go through each group, and spawn the timeslice amount of particles.
//    particle_group::iterator begin = mParticleGroups.begin();
//    particle_group::const_iterator end = mParticleGroups.end();
//
//    while( begin != end )
//    {
//        ParticleGroup* parg = (*begin);
//
//        parg->SpawnParticles( in_timeslice );
//
//        ++begin;
//    }
}


/**
 * Pauses the particle system in the current state.  No longer updates or spawns
 * any more particles.  Freezes the particle system in place.
 */
void ParticleSystem::Pause_PlaySystem ( )
{
    mIsActive = !mIsActive;
}


/**
 */
void ParticleSystem::IncParticleCount ( )
{
    ++mTotalParticles;
}


/**
 */
void ParticleSystem::DecParticleCount ( )
{
    --mTotalParticles;
}


/**
 * @param  in_groupid
 * @param  in_paramid
 * @param  in_value
 */
void ParticleSystem::SetGroupSpawnParam_Life (unsigned in_groupid, unsigned in_paramid, float in_value )
{
    if( (in_groupid < mParticleGroups.size() ) && ( in_paramid < NUM_PARAMS)  )
    {
        mParticleGroups[in_groupid]->mLifeSpawnParams[in_paramid] = in_value;
    }
}


/**
 * @param  in_groupid
 * @param  in_paramid
 * @param  in_value
 */
void ParticleSystem::SetGroupSpawnParam_Pos (unsigned in_groupid, unsigned in_paramid, float in_value )
{
    if( (in_groupid < mParticleGroups.size() ) && ( in_paramid < NUM_PARAMS)  )
    {
        mParticleGroups[in_groupid]->mPosSpawnParams[in_paramid] = in_value;
    }
}


/**
 * @param  in_groupid
 * @param  in_paramid
 * @param  in_value
 */
void ParticleSystem::SetGroupSpawnParam_Vel (unsigned in_groupid, unsigned in_paramid, float in_value )
{
    if( (in_groupid < mParticleGroups.size() ) && ( in_paramid < NUM_PARAMS)  )
    {
        mParticleGroups[in_groupid]->mVelSpawnParams[in_paramid] = in_value;
    }
}


/**
 * @param  in_groupid
 * @param  in_paramid
 * @param  in_value
 */
void ParticleSystem::SetGroupSpawnParam_Size (unsigned in_groupid, unsigned in_paramid, float in_value )
{
    if( (in_groupid < mParticleGroups.size() ) && ( in_paramid < NUM_PARAMS)  )
    {
        mParticleGroups[in_groupid]->mSizeSpawnParams[in_paramid] = in_value;
    }
}


/**
 * @param  in_groupid
 * @param  in_paramid
 * @param  in_value
 */
void ParticleSystem::SetGroupSpawnParam_Col ( unsigned in_groupid, unsigned in_paramid, float in_value )
{
    if( ( in_groupid < mParticleGroups.size() ) && ( in_paramid < NUM_PARAMS)  )
    {
        mParticleGroups[in_groupid]->mColSpawnParams[in_paramid] = in_value;
    }
}

/**
 * @param  in_groupid
 * @param  in_paramid
 * @param  in_value
 */
void ParticleSystem::SetGroupSpawnParam_Rot (unsigned in_groupid, unsigned in_paramid, float in_value )
{
    if( ( in_groupid < mParticleGroups.size() ) && ( in_paramid < NUM_PARAMS)  )
    {
        mParticleGroups[in_groupid]->mRotSpawnParams[in_paramid] = in_value;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupSpawnFunc_Color (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Color funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_ColorSpawnFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupSpawnFunc_Position (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Position funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_PositionSpawnFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupSpawnFunc_Rotation (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Rotation funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_RotationSpawnFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupSpawnFunc_Size (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Size funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_SizeSpawnFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupSpawnFunc_Velocity (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Velocity funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_VelocitySpawnFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupSpawnFunc_Life (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Life funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_LifeSpawnFunc = funcPtr;
    }
}

/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupUpdateFunc_Color (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_Color funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_ColorUpdateFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupUpdateFunc_Size (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_Size funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_SizeUpdateFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupUpdateFunc_Rotation (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_Rotation funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_RotationUpdateFunc = funcPtr;
    }
}


/**
 * @param  in_groupid
 * @param  funcPtr
 */
void ParticleSystem::SetGroupUpdateFunc_Velocity (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_Velocity funcPtr )
{
    if( in_groupid < mParticleGroups.size() )
    {
        mParticleGroups[in_groupid]->m_VelocityUpdateFunc = funcPtr;
    }
}



void ParticleSystem::initAttributes ( )
{
	mNumParticleGroups = 0;
	mNextSys = NULL;
	mTimeAlive = 0;
	mTimeToLive = 2.0f;
	mDepthSq = 0;
	mIsAlive = true;
	mIsSorted = false;
	mPosition = CVector3(0,0,0);
	mVelocity = CVector3(0,0,0);
	mOrientation = DQuaternion(0,0,0,1);
	mTotalParticles = 0;
	mIsActive = false;
}
