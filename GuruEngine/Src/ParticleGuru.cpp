/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ParticleGuru.cpp
Brief:      
/************************************************************************/

#include "ParticleGuru.h"
#include "ParticleSorter.h"
#include <algorithm>


using namespace std;


// Constructors/Destructors
//
ParticleGuru::ParticleGuru ( )
{
    initAttributes();
}


ParticleGuru::~ParticleGuru ( )
{
}


//
// Methods
//
void ParticleGuru::Update( float timeslice )
{
    //Go through all the particle systems and update them with the given time slice.
    particle_systems::iterator sys_begin = mParticleSystems.begin();
    particle_systems::const_iterator sys_end = mParticleSystems.end();

    while( sys_begin != sys_end )
    {
        //Spawn new particles for this frame, and then update.
        //(*sys_begin)->PlaySingleFrame( timeslice );
        (*sys_begin)->Update( timeslice );

        //If any systems are dead at this point, get rid of them.
        if( false == (*sys_begin)->IsAlive() )
        {
            particle_systems::iterator temp = sys_begin;
            ++temp;
            mParticleSystems.erase(sys_begin);
            sys_begin = temp;
        }
        else
        {
            ++sys_begin;
        }
    }
}


void ParticleGuru::AddGroupToRender( ParticleGroup::particle_list* groupParticles )
{
    //Get the iterators, and add each particle to the master list for sorting later.
    ParticleGroup::particle_list::iterator begin = groupParticles->begin();
    ParticleGroup::particle_list::const_iterator end = groupParticles->end();

    while( begin != end )
    {
        mMasterParticles.push_back( ParticleSortNode( (void*)(&(*begin)), (float)((*begin).mDepthSq) ) );
        ++begin;
    }
}


ParticleSystem* ParticleGuru::CreateParticleSystem()
{
    return NULL;
}


ParticleSystem* ParticleGuru::CreateParticleSystem( const string& , bool  )
{
    return NULL;
}


void    ParticleGuru::KillParticleSystem( unsigned index )
{
    mParticleSystems[index]->KillSystem();
//    delete mParticleSystems[index];
}


unsigned ParticleGuru::GetNumTextures()
{
    return mNumTexturesActive;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////          Spawners                    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Getters for the varoius functions which will be used as the update and spawn
// functions in the layers for the particles.
ParticleGroup::SpawnFuncPtr_Color       ParticleGuru::GetColorSpawnFunc( const string& name )
{
    if( name == "SpawnRandomColor" )
    {
        return (SpawnRandomColor);
    }
    return SpawnRandomColor;
}


ParticleGroup::SpawnFuncPtr_Position    ParticleGuru::GetPositionSpawnFunc( const string& name )
{
    if( name == "SpawnRandomPosition" )
    {
        return (SpawnRandomPosition);
    }
    else if( name == "SpawnRandomPositionRelative" )
    {
        return (SpawnRandomPositionRelative);
    }
    else if( name == "SpawnRandomCircleRelative" )
    {
        return (SpawnRandomCircleRelative);
    }
    return SpawnRandomPosition;
}


ParticleGroup::SpawnFuncPtr_Size        ParticleGuru::GetSizeSpawnFunc( const string& name )
{
    if( name == "SpawnRandomSize" )
    {
        return (SpawnRandomSize);
    }
    return SpawnRandomSize;
}


ParticleGroup::SpawnFuncPtr_Rotation    ParticleGuru::GetRotationSpawnFunc( const string& name )
{
    if( name == "SpawnRandomRotation" )
    {
        return (SpawnRandomRotation);
    }
    return SpawnRandomRotation;
}


ParticleGroup::SpawnFuncPtr_Velocity    ParticleGuru::GetVelocitySpawnFunc( const string& name )
{
    if( name == "SpawnRandomVelocity" )
    {
        return (SpawnRandomVelocity);
    }
    else if( name == "SpawnRandomVelocityRelative" )
    {
        return (SpawnRandomVelocityRelative);
    }
    return SpawnRandomVelocity;
}


ParticleGroup::SpawnFuncPtr_Life        ParticleGuru::GetLifeSpawnFunc( const string& name )
{
    if( name == "SpawnRandomLife" )
    {
        return (SpawnRandomLife);
    }
    return SpawnRandomLife;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////          Updaters                    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ParticleGroup::UpdateFuncPtr_Color      ParticleGuru::GetColorUpdateFunc( const string& name )
{
    if( name == "LinearDecay" )
    {
        return (LinearDecay);
    }
    else if( name == "LinearGrowth" )
    {
        return (LinearGrowth);
    }
    else if( name == "ExponentialDecay" )
    {
        return (ExponentialDecay);
    }
    else if( name == "ExponentialGrowth" )
    {
        return (ExponentialGrowth);
    }
    return LinearDecay;
}


ParticleGroup::UpdateFuncPtr_Size       ParticleGuru::GetSizeUpdateFunc( const string& name )
{
    if( name == "LinearDecay" )
    {
        return LinearDecay;
    }
    else if( name == "LinearGrowth" )
    {
        return LinearGrowth;
    }
    else if( name == "ExponentialDecay" )
    {
        return ExponentialDecay;
    }
    else if( name == "ExponentialGrowth" )
    {
        return ExponentialGrowth;
    }
    return LinearDecay;
}


ParticleGroup::UpdateFuncPtr_Velocity   ParticleGuru::GetVelocityUpdateFunc( const string& name )
{
    if( name == "LinearDecay" )
    {
        return (LinearDecay);
    }
    else if( name == "LinearGrowth" )
    {
        return (LinearGrowth);
    }
    else if( name == "ExponentialDecay" )
    {
        return (ExponentialDecay);
    }
    else if( name == "ExponentialGrowth" )
    {
        return (ExponentialGrowth);
    }
    return LinearDecay;
}


ParticleGroup::UpdateFuncPtr_Rotation   ParticleGuru::GetRotationUpdateFunc( const string& name )
{
    if( name == "LinearDecay" )
    {
        return LinearDecay;
    }
    else if( name == "LinearGrowth" )
    {
        return LinearGrowth;
    }
    else if( name == "ExponentialDecay" )
    {
        return ExponentialDecay;
    }
    else if( name == "ExponentialGrowth" )
    {
        return ExponentialGrowth;
    }
    return LinearDecay;
}


// Accessor methods
//
//Accessors and Setters
ParticleGuru::particle_systems* ParticleGuru::GetPSystemsPtr()
{
    return &mParticleSystems;
}


unsigned int ParticleGuru::GetNumSystems() const
{
    return mNumSystems;
}


psorter_list* ParticleGuru::GetMasterPListPtr()
{
    return &mMasterParticles;
}


bool    ParticleGuru::IsLocked() const
{
    return mLocked;
}


bool    ParticleGuru::ShouldSort() const
{
    return mSortParticles;
}


// Other methods
//
void ParticleGuru::initAttributes ( )
{
	mNumSystems = 0;
	mNumTexturesActive = 0;
}


void ParticleGuru::sortParticles ( )
{
    if( mSortParticles )
    {
        std::sort( mMasterParticles.begin(), mMasterParticles.end(), SortParticleDepth );
    }
}


template <typename T>
void ExponentialGrowth( float timeslice, float , float age, float params[NUM_PARAMS], T& out_value )
{
    if( &out_value != NULL && params != NULL )
    {
        T val = out_value * exp( params[1] * age );

        out_value = out_value + ( val * (timeslice * params[0]) );
    }
}


template <typename T>
void ExponentialDecay( float timeslice, float , float age, float params[NUM_PARAMS], T& out_value )
{
    if( &out_value != NULL && params != NULL )
    {
        float val = exp( -params[1] * (age/1000.0f) * timeslice);

        out_value = out_value * val;
    }
}


template <typename T>
void LinearGrowth( float timeslice, float , float , float params[NUM_PARAMS], T& out_value )
{
    if( &out_value != NULL && params != NULL )
    {
        out_value = out_value + (out_value * timeslice * params[0]);
    }
}


template <typename T>
void LinearDecay( float timeslice, float , float , float params[NUM_PARAMS], T& out_value )
{
    if( &out_value != NULL && params != NULL )
    {
        out_value = out_value - (out_value * timeslice * params[0]);
    }
}


//There will be specific functions for specifically updating certain values, such as life,
// size, color, etc.

//Spawn function.
//params[0] = min size
//params[1] = max size
void SpawnRandomSize( ParticleSystem* , float , float params[NUM_PARAMS], float& size )
{
    size = RandFloat( params[0], params[1] );
}


//params[0] = min r
//params[1] = max r
//params[2] = min g
//params[3] = max g
//params[4] = min b
//params[5] = max b
void SpawnRandomColor( ParticleSystem* , float , float params[NUM_PARAMS], CVector3& color )
{
    color = CVector3( RandFloat( params[0], params[1] ), RandFloat( params[2], params[3]), RandFloat( params[4], params[5] ) );
}


//params[0] = min life
//params[1] = max life
void SpawnRandomLife( ParticleSystem* , float , float params[NUM_PARAMS], float& life )
{
    life = RandFloat( params[0], params[1] );
}


//params[0] = min x
//params[1] = max x
//params[2] = min y
//params[3] = max y
//params[4] = min z
//params[5] = max z
void SpawnRandomPosition( ParticleSystem* parent, float , float params[NUM_PARAMS], CVector3& pos )
{
    pos = CVector3( RandFloat( params[0], params[1] ), RandFloat( params[2], params[3]), RandFloat( params[4], params[5] ) );

    if( parent )
    {
        pos = pos + parent->GetPosition();
    }
}


//params[0] = min r
//params[1] = max r
//params[2] = min g
//params[3] = max g
//params[4] = min b
//params[5] = max b
void SpawnRandomPositionRelative( ParticleSystem* parent, float , float params[NUM_PARAMS], CVector3& pos )
{
    CVector3 temp = CVector3( RandFloat( params[0], params[1] ), RandFloat( params[2], params[3]), RandFloat( params[4], params[5] ) );
    if( parent )
    {
        //Put the random position into the space of the system and add position.
        pos = temp + parent->GetPosition();
    }
}


//params[0] = min r
//params[1] = max r
//params[2] = min g
//params[3] = max g
//params[4] = min b
//params[5] = max b
void SpawnRandomCircleRelative( ParticleSystem* , float , float [NUM_PARAMS], CVector3&  )
{
}


//params[0] = min r
//params[1] = max r
//params[2] = min g
//params[3] = max g
//params[4] = min b
//params[5] = max b
void SpawnRandomVelocity( ParticleSystem* , float , float params[NUM_PARAMS], CVector3& vel )
{
    vel = CVector3( RandFloat( params[0], params[1] ), RandFloat( params[2], params[3]), RandFloat( params[4], params[5] ) );
}


//params[0] = min r
//params[1] = max r
//params[2] = min g
//params[3] = max g
//params[4] = min b
//params[5] = max b
void SpawnRandomVelocityRelative( ParticleSystem* parent, float , float params[NUM_PARAMS], CVector3& vel )
{
    vel = CVector3( RandFloat( params[0], params[1] ), RandFloat( params[2], params[3]), RandFloat( params[4], params[5] ) );

    if( parent)
    {
        //Transform here then add velocity.
        vel += parent->GetVelocity();
    }
}


//params[0] = min rotation
//params[1] = max rotation
void SpawnRandomRotation( ParticleSystem* , float , float params[NUM_PARAMS], float& rot )
{
    rot = RandFloat( params[0], params[1] );
}
