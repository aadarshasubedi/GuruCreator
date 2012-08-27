/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       Entity.h
Brief:      
/************************************************************************/

#ifndef GURU_ENTITY_H
#define GURU_ENTITY_H


/************************************************************************/
/* Includes				                                                */
/************************************************************************/
#include "DObject.h"
#include "ComponentDisciple.h"
#include "VQS.h"
#include "Primitives.h"
#include <map>
#include <vector>
#include <stdlib.h>


/************************************************************************/
/* Forward Declarations                                                 */
/************************************************************************/
class EntityDescriptor;
class ComponentDisciple;

/************************************************************************/
/* Class Declaration	                                                */
/************************************************************************/
class Entity : public DObject
{
protected:
	/************************************************************************/
	/* An Entity holds a bunch of Components                                */
	/************************************************************************/
	typedef std::pair<unsigned int, ComponentDisciple*> GuruComponent;
	std::map<unsigned int, ComponentDisciple*> mComponents;
	std::multimap< ComponentFamily, ComponentDisciple*> mComponentsFamilyMapping;
	ComponentDisciple* disciple;

	std::vector<ComponentDisciple*> components;

	//Name and identifier data.
	std::string mName;

public:
	Entity();
	virtual ~Entity();

	//virtual void Init(EntityDescriptor* const descriptor) = 0;
	//virtual void SetUpComponentConnections() = 0;

	//Access to the raw siblings all together.
	void AddComponent( ComponentDisciple* disc );
	void RemoveComponent( ComponentDisciple* disc );
	void RemoveComponent( std::string name, std::string type );
	void RemoveComponent( unsigned int uid );
	//Access to the sibling graph.
	std::multimap< ComponentFamily, ComponentDisciple* > getSiblingMapForWriting() { return mComponentsFamilyMapping; }
	
	//Naming system, get set name and such.
	void SetName( const char* setToName );
	const char* GetName() const;
	void AppendName( const char* addToName );

	CVector3 mPosition;
	CVector3 mRenderPosition;
	DQuaternion mOrientation;

	//Global world coordinate axis rotations.
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void RotateByVector(CVector3 vec, float angle);
	void RotateRelativeTo(CVector3 pos, CVector3 vec, float angle);

	//Easy translation functions for the entity.  Some entities this may do nothing
	// as they don't use their positions.
	void TranslateLocalX(float amount);
	void TranslateLocalY(float amount);
	void TrnaslateLocalZ(float amount);
	void TrnaslateWorldX(float amount);
	void TranslateWorldY(float amount);
	void TranslateWorldZ(float amount);

private:
	void TranslateVector(float amount, CVector3 direction);

public:
	//A sphere for picking purposes
	Sphere mPickSphere;
};


#endif
