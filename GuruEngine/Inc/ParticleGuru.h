/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef PARTICLEGURU_H
#define PARTICLEGURU_H


#include "DSingleton.h"
#include "ParticleGroup.h"
#include "ParticleSorter.h"
#include "ParticleSystem.h"
#include <string>


const unsigned MAX_PARTICLE_TEXTURES = 10;


class ParticleGuru : public DSingleton<ParticleGuru>
{
public:
    typedef std::vector<ParticleSystem*> particle_systems;

	ParticleGuru ( );
	virtual ~ParticleGuru ( );

	ParticleSystem* CreateParticleSystem();
	ParticleSystem* CreateParticleSystem( const std::string& filename, bool isFactoryName = false );

	void AddGroupToRender( ParticleGroup::particle_list* groupParticles );

	void KillParticleSystem( unsigned index );

	void Update( float timeslice );
	
	//This function will create the vbos and set the screen aligned quad.  No buffering for
	// the particle buffer since it will be filled during render.
	virtual void Init() = 0;

    //This function will push all particles into the master list, along with setting any necessary states.
	virtual void Begin(bool clearMasterList = true, bool sortParticles = false, CVector3 camPos = CVector3(0,0,0) ) = 0;

    //The particle sorting will be performed here before any data sent to graphics card.
    //Here vertex attributes and any data sent to the shader will be done here.
    //The actual render call will also occur here.
	virtual void Render() = 0;

    //This will end the rendering and de-set any states set, and clear the master particles list.
	virtual void End() = 0;
	
	//Function for easy quick testing of a derived classes particle systems.
	virtual ParticleSystem* CreateDemoSystem() = 0;

	//Getters for the varoius functions which will be used as the update and spawn
	// functions in the layers for the particles.
	ParticleGroup::SpawnFuncPtr_Color GetColorSpawnFunc( const std::string& name );
	ParticleGroup::SpawnFuncPtr_Position GetPositionSpawnFunc( const std::string& name );
	ParticleGroup::SpawnFuncPtr_Size GetSizeSpawnFunc( const std::string& name );
	ParticleGroup::SpawnFuncPtr_Rotation GetRotationSpawnFunc( const std::string& name );
	ParticleGroup::SpawnFuncPtr_Velocity GetVelocitySpawnFunc( const std::string& name );
	ParticleGroup::SpawnFuncPtr_Life GetLifeSpawnFunc( const std::string& name );

	ParticleGroup::UpdateFuncPtr_Color GetColorUpdateFunc( const std::string& name );
	ParticleGroup::UpdateFuncPtr_Size GetSizeUpdateFunc( const std::string& name );
	ParticleGroup::UpdateFuncPtr_Velocity GetVelocityUpdateFunc( const std::string& name );
	ParticleGroup::UpdateFuncPtr_Rotation GetRotationUpdateFunc( const std::string& name );
	
public:
    //Accessors and Setters
    particle_systems* GetPSystemsPtr();
    unsigned int GetNumSystems() const;

    psorter_list* GetMasterPListPtr();
    ParticleSorter* GetSorterPtr();

	unsigned GetNumTextures();

    bool    IsLocked() const;
    bool    ShouldSort() const;

protected:
	unsigned mNumTexturesActive;	

	particle_systems mParticleSystems;
	unsigned int mNumSystems;

	//The global list of all particles to render.
	psorter_list mMasterParticles;

	bool mLocked;
	bool mSortParticles;

protected:
	void initAttributes();
	void sortParticles();
};



class ParticleSystemFactory
{
    //The particle factory holds the spawn params for a particle, the string that represents it,
    // textures, layers, and the update params.  Also holds the spawn & update function names.

    //When this class is used to create a particle system, many things happen.
    //A particle system is created within the ParticleGuru.  As this is singleton, no problem.
    //That system then goes through the layer list stored by the factory, and generates an
    // appropriate layer.  Then the spawn and update functions must be set using the given strings.
    //After that, individual layer parameters must be set.

    //All of this happens internally, and then the pointer is returned from the factory.

    //In this way, the Factory acts as a generator, but stores nothing since the DObject handles that.

    //All of this could simply be contained within the DObject itself, but messiness would happen.

    //In this way, any filename given (provided it's a .psys file) will generate a new factory function
    // for that system type.  Any subsequent call to load that file will do nothing since already loaded.
    //
    //From there, any use of that system type string in the CreateParticleSystem function will go to the
    // factory, and looks up an appropriate
};


//Function prototypes for the various spawn and update functions that will be used.

//There will be generic linear decays and growth, as well as constant decays and growth.
template <typename T>
void ExponentialGrowth( float timeslice, float curLife, float age, float params[NUM_PARAMS], T& out_value );

template <typename T>
void ExponentialDecay( float timeslice, float curLife, float age, float params[NUM_PARAMS], T& out_value );

template <typename T>
void LinearGrowth( float timeslice, float curLife, float age, float params[NUM_PARAMS], T& out_value );

template <typename T>
void LinearDecay( float timeslice, float curLife, float age, float params[NUM_PARAMS], T& out_value );

//There will be specific functions for specifically updating certain values, such as life,
// size, color, etc.

//Spawn function.
void SpawnRandomSize( ParticleSystem* parent, float time, float params[NUM_PARAMS], float& size );
void SpawnRandomColor( ParticleSystem* parent, float time, float params[NUM_PARAMS], CVector3& color );
void SpawnRandomLife( ParticleSystem* parent, float time, float params[NUM_PARAMS], float& life );
void SpawnRandomRotation( ParticleSystem* parent, float time, float params[NUM_PARAMS], float& rot );

void SpawnRandomPosition( ParticleSystem* parent, float time, float params[NUM_PARAMS], CVector3& pos );
void SpawnRandomPositionRelative( ParticleSystem* parent, float time, float params[NUM_PARAMS], CVector3& pos );
void SpawnRandomCircleRelative( ParticleSystem* parent, float time, float params[NUM_PARAMS], CVector3& pos );

void SpawnRandomVelocity( ParticleSystem* parent, float time, float params[NUM_PARAMS], CVector3& vel );
void SpawnRandomVelocityRelative( ParticleSystem* parent, float time, float params[NUM_PARAMS], CVector3& vel );

#endif // PARTICLEGURU_H
