/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef PARTICLEGROUP_H
#define PARTICLEGROUP_H


#include <list>
#include <string>
#include "Particle.h"


const unsigned int NUM_PARAMS = 6;

//Forward declaration of ParticleSystem.
class ParticleSystem;


class ParticleGroup
{
public:
	typedef std::list<Particle> particle_list;

public:
	ParticleGroup();
	virtual ~ParticleGroup();

	/**
	 * @param  mUpdateAmount The timeslice by which to update the particle simulation.
	 */
	void Update(float mUpdateAmount);

	/**
	 * @param  mSpawnTime The amount of time to spawn particles.
	 */
	void SpawnParticles(float mSpawnTime);

	/**
	 * @param  camPos The camera position.  This is needed in order to perform the
	 * depth sorting.
	 */
	void Render(CVector3 camPos = CVector3(0,0,0));

	Particle* CreateParticle();

	float GetDensity();
	bool GetAdditive();
	bool GetContinuous();
	ParticleSystem* GetParentSystem();
	GLint GetTextureId();
	GLint GetProgramId();
	particle_list* GetParticleList();
	void SetDensity (float in_density = 1 );
	void SetAdditive (bool in_additive );
	void SetContinuous (bool in_continuous );
	void ToggleContinuous ( );
	void SetProgramId (GLint in_program );
	void SetTextureId (GLint in_textureid );
	void SetParentSystem (ParticleSystem* in_parentsys );
	void ClearAllParticles ( );
	void SetSizeParams (const float* in_sizeparams );
	void SetColorParams (const float* in_colparams );
	void SetPosParams (const float* in_posparams );
	void SetVelParams (const float* in_velparams );
	void SetRotParams (const float* in_rotparams );
	void SetLifeParams (const float* in_lifeparams );
	void SetSizeUpdateParams (const float* up_sizeparams );
	void SetRotUpdateParams (const float* up_rotparams );
	void SetVelUpdateParams (const float* up_velparams );
	void SetColUpdateParams (const float* up_colparams );

	//The typedefs for the update and spawn function pointers.

	typedef void (*UpdateFuncPtr_float)(float, float, float, float[NUM_PARAMS], float&);
	typedef void (*UpdateFuncPtr_vec3)(float, float, float, float[NUM_PARAMS], CVector3&);

	typedef UpdateFuncPtr_float UpdateFuncPtr_Size;
	typedef UpdateFuncPtr_float UpdateFuncPtr_Rotation;
	typedef UpdateFuncPtr_vec3 UpdateFuncPtr_Color;
	typedef UpdateFuncPtr_vec3 UpdateFuncPtr_Velocity;

	UpdateFuncPtr_Size m_SizeUpdateFunc;
	UpdateFuncPtr_Color m_ColorUpdateFunc;
	UpdateFuncPtr_Rotation m_RotationUpdateFunc;
	UpdateFuncPtr_Velocity m_VelocityUpdateFunc;




	typedef void (*SpawnFuncPtr_float)(ParticleSystem*, float, float[NUM_PARAMS], float&);
	typedef SpawnFuncPtr_float SpawnFuncPtr_Size;
	typedef SpawnFuncPtr_float SpawnFuncPtr_Rotation;
	typedef SpawnFuncPtr_float SpawnFuncPtr_Life;

	typedef void (*SpawnFuncPtr_vec3)(ParticleSystem*, float, float[NUM_PARAMS], CVector3&);
	typedef SpawnFuncPtr_vec3 SpawnFuncPtr_Position;
	typedef SpawnFuncPtr_vec3 SpawnFuncPtr_Velocity;
	typedef SpawnFuncPtr_vec3 SpawnFuncPtr_Color;

	SpawnFuncPtr_Size       m_SizeSpawnFunc;
	SpawnFuncPtr_Rotation   m_RotationSpawnFunc;
	SpawnFuncPtr_Life       m_LifeSpawnFunc;
	SpawnFuncPtr_Position   m_PositionSpawnFunc;
	SpawnFuncPtr_Velocity   m_VelocitySpawnFunc;
	SpawnFuncPtr_Color      m_ColorSpawnFunc;

	float mSizeUpdateParams[NUM_PARAMS];
	float mColUpdateParams[NUM_PARAMS];
	float mRotUpdateParams[NUM_PARAMS];
	float mVelUpdateParams[NUM_PARAMS];
	float mPosSpawnParams[NUM_PARAMS];
	float mVelSpawnParams[NUM_PARAMS];
	float mColSpawnParams[NUM_PARAMS];
	float mRotSpawnParams[NUM_PARAMS];
	float mLifeSpawnParams[NUM_PARAMS];
	float mSizeSpawnParams[NUM_PARAMS];
	
private:
	// The list of particles.
	particle_list mParticles;
	// The shader program used to render the particles.
	GLint mEffectProgram;
	// The texture id associated with this particle layer.
	GLint mTextureid;
	// If true, the layer will continue emitting until destroyed.  False means it will emit one frames worth of particles.
	bool mContinuous;
	bool mIsAdditive;
	// The density of particles, used in the spawning functions.
	float mParticleDensity;
	// The parent particle system that owns this particle layer.
	ParticleSystem* mParentSys;

	void initAttributes();
};


#endif // PARTICLEGROUP_H
