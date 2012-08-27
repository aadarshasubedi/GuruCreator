/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H


#include <string>
#include "ParticleGroup.h"
#include "DQuaternion.h"


class ParticleSystem
{
	typedef std::vector<ParticleGroup*> particle_group;

public:
	ParticleSystem();
	virtual ~ParticleSystem();

	/**
	 * Update the particle systems based on their various update functions.
	 * @param  in_timeslice The timeslice expired over this frame to update the
	 * particle system and all groups/particles.
	 */
	void Update(float in_timeslice);

	/**
	 * Function to render the particles.  Will perform sorts, setting up shader
	 * programs necessary, textures, etc.
	 * @param  in_campos The camera position is needed to render the particles due to
	 * the sorting involved.  If blending, then the sort order is extremely important.
	 *
	 */
	void Render(CVector3 in_campos = CVector3(0,0,0));

	CVector3 GetVelocity();
	void SetVelocity(CVector3 in_velocity);

	CVector3 GetPosition();
	void SetPosition (CVector3 in_position );

	DQuaternion GetOrienation ( );
	void SetOrientation (DQuaternion in_orientation );

	bool IsAlive ( );

	bool IsSorted ( );
	void SetSorted (bool in_sort );

	float GetLife ( );
	void SetLife (float in_lifetime );

	float GetDepth ( );
	void ComputeDepth (CVector3 in_campos );

	ParticleGroup* AddParticleGroup (GLint in_textureid, GLint in_program, float in_density );

	void ClearAllLayers ( );
	void KillSystem ( );

	void PlaySingleFrame (float in_timeslice );

	void Pause_PlaySystem ( );

	void IncParticleCount ( );
	void DecParticleCount ( );

	void SetGroupSpawnParam_Life (unsigned in_groupid, unsigned in_paramid, float in_value );
	void SetGroupSpawnParam_Pos (unsigned in_groupid, unsigned in_paramid, float in_value );
	void SetGroupSpawnParam_Vel (unsigned in_groupid, unsigned in_paramid, float in_value );
	void SetGroupSpawnParam_Size (unsigned in_groupid, unsigned in_paramid, float in_value );
	void SetGroupSpawnParam_Col (unsigned in_groupid, unsigned in_paramid, float in_value );
	void SetGroupSpawnParam_Rot (unsigned in_groupid, unsigned in_paramid, float in_value );
	void SetGroupSpawnFunc_Color (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Color funcPtr );
	void SetGroupSpawnFunc_Position (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Position funcPtr );
	void SetGroupSpawnFunc_Rotation (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Rotation funcPtr );
	void SetGroupSpawnFunc_Size (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Size funcPtr );
	void SetGroupSpawnFunc_Velocity (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Velocity funcPtr );
	void SetGroupSpawnFunc_Life (unsigned in_groupid, ParticleGroup::SpawnFuncPtr_Life funcPtr );
	void SetGroupUpdateFunc_Color (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_vec3 funcPtr );
	void SetGroupUpdateFunc_Size (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_float funcPtr );
	void SetGroupUpdateFunc_Rotation (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_float funcPtr );
	void SetGroupUpdateFunc_Velocity (unsigned in_groupid, ParticleGroup::UpdateFuncPtr_vec3 funcPtr );

public:
    particle_group* GetGroups() { return &mParticleGroups; }

private:
	// The pointers to the actual particle groups.  These are created here, and should be destroyed here.
	particle_group mParticleGroups;
	// The number of particle groups this system contains.
	unsigned int mNumParticleGroups;
	// The GLint ids for the textures when generated.
	// A particle system may have another system attached to it, or which will begin updating when the current system dies.  In which case, this is not where the pointer is created.  The systems are all created by the ParticleGuru, and if an attachment is needed the pointer is obtained from the DObject and linked here.
	ParticleSystem* mNextSys;
	// Current amount of time the system has existed.
	float mTimeAlive;
	// The total amount of time this system should spawn particles and update.
	float mTimeToLive;
	// The depth squared from the center of the system to the camera position.  Squared depth is used so to avoid the square root operation.
	float mDepthSq;
	// Is this system still alive or has it lived past its age?  If true, let it live.
	bool mIsAlive;
	// Are the particles sorted.  True means they are.
	bool mIsSorted;
	// Current position of the whole system.
	CVector3 mPosition;
	// Current velocity of the whole system.
	CVector3 mVelocity;
	// Global orientation of system
	DQuaternion mOrientation;
	// All the particles alive for this system, across all groups.
	unsigned int mTotalParticles;
	// This value will account for the fact that the system can be stopped without destroying it.
	bool mIsActive;

	void initAttributes();
};


#endif // PARTICLESYSTEM_H
