/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       ComponentDisciple.h
Brief:      
/************************************************************************/

#ifndef COMPONENET_DISCIPLE_H
#define COMPONENET_DISCIPLE_H


#include <stdlib.h>
#include <vector>
#include <string>
#include "LunarHelper.h"


/************************************************************************/
/* Forward declaration 
Since we have to hold a ref in the component, but the entity holds all of
 its components, forward declare to avoid circular includes.
/************************************************************************/
class Entity;


/************************************************************************/
/* Defines types of families (Category)	
/************************************************************************/
enum ComponentFamily
{
	Undefined = 0,
	Graphics,
	Physics,
	Gameplay,
	Script,
	Input,
	Networking
};

enum ComponentConnectionDir
{
	Read = 1,
	Write = 2, 
	Bidirectional = 4
};


/************************************************************************/
/* Forward Declarations & Typedefs
/************************************************************************/
class ComponentDisciple;
typedef std::pair< ComponentDisciple*, ComponentConnectionDir> ComponentConnection;


/************************************************************************/
/* The base Component class for an aggregate style entity system.
/************************************************************************/
class ComponentDisciple
{
	static unsigned int guids;

protected:
	/************************************************************************/
	/* Inter-component communication
	// These are shared because the ComponentGuru will hold
	/************************************************************************/
	std::vector< ComponentConnection > mSiblingComponents;	

	/************************************************************************/
	/* Typing Mechanism
	// The type, referring to a subset of a family.  e.g.
	//   Family:Graphics  Type:Model  Name:Sword
	//   Family:Graphics  Type:RenderTarget  Name:TV_Monitor-01
	/************************************************************************/
	std::string mType;
	unsigned int mUid;
	std::string mName;
	ComponentFamily mFamily;

	/************************************************************************/
	/* anything else that will be of use later
	/************************************************************************/
	Entity* mParentRef;

public:
	/************************************************************************/
	/* Constructors/Destructors
	/************************************************************************/
	ComponentDisciple();
	ComponentDisciple(Entity* parent, ComponentFamily fam, std::string type, std::string name="Main");

	ComponentDisciple(const ComponentDisciple& disc);
	ComponentDisciple(ComponentDisciple&& other);

	virtual ~ComponentDisciple();

	/************************************************************************/
	/* Initializations
	/************************************************************************/
	virtual void Init() {}

	/************************************************************************/
	/* Getters/Setters
	/************************************************************************/
	std::vector< ComponentConnection > GetSiblingComponents() const { return mSiblingComponents; }
	std::vector< ComponentDisciple* > GetSiblingComponentsByFamily(ComponentFamily family) const;
	ComponentDisciple* GetSiblingComponent( std::vector< ComponentConnection >::iterator it );
	ComponentDisciple* GetSiblingComponentWeak( std::vector< ComponentConnection >::iterator it );
	void AddSiblingComponent( ComponentDisciple* component, ComponentConnectionDir dir = Bidirectional);
	void RemoveSiblingComponent( ComponentDisciple* component);
	
	std::string GetType() const { return mType; }
	std::string type() const { return mType; }
	void SetType(std::string type) { mType = type; }

	ComponentFamily GetFamily() const { return mFamily; }
	ComponentFamily family() const  { return mFamily; }
	void SetFamily(ComponentFamily family) { mFamily = family; }

	/*************************************************************************
	 The UID is set when created and should not ever be set otherwise, so only accessors.
	*************************************************************************/
	const unsigned int& GetUid() const { return mUid; }
	const unsigned int& uid() const { return mUid; }

	const std::string& GetName() const { return mName; }
	const std::string& name() const { return mName; }
	void SetName(std::string name) { mName = name; }
	
	/*************************************************************************	 
	The parent has a setter because a component can be cloned for another object
	*************************************************************************/
	Entity* GetParentRef() { return mParentRef; }
	Entity* parent() const { return mParentRef; }
	void SetParent(Entity* inParent) { mParentRef = inParent; }
};


#endif
