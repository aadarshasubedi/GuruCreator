/*
Author:		Dan Strycharske
Date:       02.05.12
File:       ComponentDisciple.cpp
Brief:      
*/



#include "ComponentDisciple.h"
#include "Entity.h"
#include <algorithm>


using namespace std;


unsigned int ComponentDisciple::guids = 0;


/************************************************************************/
/* Default Constructor
/************************************************************************/
ComponentDisciple::ComponentDisciple() : mType(""), mUid(guids++), mName("NoName"), mFamily(Undefined), mParentRef(NULL), mSiblingComponents(0)
{
}

/*************************************************************************
 Constructor with all parameters
*************************************************************************/
ComponentDisciple::ComponentDisciple(Entity* parent, ComponentFamily fam, std::string type, std::string name)
	: mSiblingComponents(), mType(type), mUid(guids++), mName(name), mFamily(fam), mParentRef(parent)
{
}

/************************************************************************/
/* Copy Constructor
/************************************************************************/
ComponentDisciple::ComponentDisciple(const ComponentDisciple& disc)
	: mSiblingComponents(disc.GetSiblingComponents()), mUid(guids++), mType(disc.mType), mName(disc.mName), mFamily(disc.family())
{
	mParentRef = disc.parent();
}


/************************************************************************/
/* Move Constructor (object can become organ donor)
/************************************************************************/
ComponentDisciple::ComponentDisciple(ComponentDisciple&& other)
	: mSiblingComponents(move( other.mSiblingComponents) ), mType(move(other.mType)), mUid(move(other.mUid)), 
	mName(move(other.mName)), mFamily(move(other.mFamily)), mParentRef(move(other.mParentRef)) 
{
}


/************************************************************************/
/* Destructor
/************************************************************************/
ComponentDisciple::~ComponentDisciple()
{

}


/************************************************************************/
/* Function: GetSiblingComponentsByFamily
	Params: ComponentFamily family
	Return: vector< weak_ptr<ComponentDisciple> >
	Brief:  Goes through the list of sibling components, finds all of 
			the ones of the given 'family' type, and puts them into a 
			vector to be returned.  The vector contains weak_ptr's to not
			increase the ref count as this function is meant to get them
			for reading/observing purposes, but not for maintaining.  If 
			it's determined you need to keep a ref to a component, use
			'GetSiblingComponent' passing in the iterator to the object of 
			interest.  This returns a shared_ptr to the object.
/************************************************************************/
vector< ComponentDisciple* > ComponentDisciple::GetSiblingComponentsByFamily(ComponentFamily family) const
{
	vector< ComponentDisciple*> allOfFamilyType = vector< ComponentDisciple* >();

	//Using new lambdas for realzz!!
	for_each(begin(mSiblingComponents), end(mSiblingComponents), [&](ComponentConnection disciple)
	{
		if( disciple.first->family() == family )
		{
			//Insert into a temp array.
			allOfFamilyType.push_back(disciple.first);
		}
	});

	return allOfFamilyType;
}


/************************************************************************/
/* Function: GetSiblingComponent
	Params: std::vector< ComponentConnection >::iterator
	Return: shared_ptr<ComponentDisciple>
	Brief:  Essentially a ridiculous helper function to get a shared_ptr of the component.
/************************************************************************/
ComponentDisciple* ComponentDisciple::GetSiblingComponent( std::vector< ComponentConnection >::iterator it )
{
	return it->first;
}


/************************************************************************/
/* Function: GetSiblingComponentWeak
	Params: std::vector< ComponentConnection >::iterator
	Return: weak_ptr<ComponentDisciple>
	Brief:  Essentially a ridiculous helper function to get a weak_ptr of the component.
************************************************************************/
ComponentDisciple* ComponentDisciple::GetSiblingComponentWeak( std::vector< ComponentConnection >::iterator it )
{
	return it->first;
}

void ComponentDisciple::AddSiblingComponent( ComponentDisciple* component, ComponentConnectionDir dir)
{
	mSiblingComponents.push_back(ComponentConnection(component,dir));
}


void ComponentDisciple::RemoveSiblingComponent( ComponentDisciple* component)
{
	vector<ComponentConnection>::iterator iter = mSiblingComponents.begin();
	vector<ComponentConnection>::iterator deathRow = iter;

	//Find it and kill it.
	std::for_each(begin(mSiblingComponents), end(mSiblingComponents), [&](ComponentConnection compConn)
	{
		if( component == compConn.first )
		{
			deathRow = iter;
		}
		++iter;
	});	

	mSiblingComponents.erase(deathRow);
}
