/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       Entity.cpp
Brief:      
/************************************************************************/

#include "Entity.h"
#include "EntityDescriptor.h"
#include <algorithm>
#include <iostream>


/************************************************************************/
/* Default Constructor
/************************************************************************/
Entity::Entity() : DObject(), mPickSphere(10.0f, Point())//, mPosition(), mOrientation()
{
}


/************************************************************************/
/* Destructor
/************************************************************************/
Entity::~Entity()
{
}


void Entity::AddComponent( ComponentDisciple* disc )
{
	//disc->SetParent(std::make_shared<Entity>(*this));
	//auto toInsert = std::make_pair<const unsigned int, ComponentDisciple> >(disc->uid(), disc);	
	components.push_back( disc );
	//mComponents.insert(std::make_pair<unsigned int, ComponentDisciple*>(disc->GetUid(), disc));
	//mComponentsFamilyMapping.insert( std::make_pair<ComponentFamily, ComponentDisciple*>(disc->family(), disc) );
}


/********************************************************************************
    Function: RemoveComponent
	Params:   shared_ptr<ComponentDisciple>
	Return:   void
	Brief:    Removes from the vector the ptr referring to disc. 
/********************************************************************************/
void Entity::RemoveComponent( ComponentDisciple* disc )
{
	auto begin = components.begin();
	auto end = components.end();
	while( begin != end )
	{
		if (*begin == disc )
		{
			break;
		}
		++begin;
	}
	//mComponents.erase(deadMeat);
	//mComponentsFamilyMapping.erase(deadMeat);
	components.erase(begin);
}


void Entity::RemoveComponent( std::string name, std::string type )
{
	//auto deadMeat = std::find_if(begin(components), end(components), [&](ComponentDisciple* current) 
	//{ 
	//	//return (current->type == type);
	//	//return (current.second->type() == type && current.second->name() == name);
	//});
	//mComponents.erase(deadMeat);
	//mComponentsFamilyMapping.erase(deadMeat);
	//components.erase(deadMeat);
}


void Entity::RemoveComponent( unsigned int uid )
{
	auto deadMeat = std::find_if(begin(components), end(components), [&](ComponentDisciple* current) 
	{ 
		return (current->uid() == uid);
	});
	//mComponents.erase(deadMeat);
	//mComponentsFamilyMapping.erase(deadMeat);
	components.erase(deadMeat);
}


//Naming system, get set name and such.
void Entity::SetName( const char* setToName )
{
	mName = setToName;
}

const char* Entity::GetName() const
{
	return mName.c_str();
}

void Entity::AppendName( const char* addToName )
{
	mName += addToName;
}


void Entity::RotateX(float angle)
{
	RotateByVector(CVector3(1,0,0), angle);
}


void Entity::RotateY(float angle)
{
	RotateByVector(CVector3(0,1,0), angle);
}


void Entity::RotateZ(float angle)
{
	RotateByVector(CVector3(0,0,1), angle);
}


void Entity::RotateByVector(CVector3 vec, float angle)
{
	DQuaternion rotation(vec, angle);
	//rotation.FromAxisAngle(vec,angle);

	DQuaternion pQ(0.0f, mPosition.x, mPosition.y, mPosition.z);
	pQ = pQ * rotation;
	pQ = rotation.inverse() * pQ;
	mPosition = CVector3(pQ.x, pQ.y, pQ.z);

	mOrientation = rotation * mOrientation;
}


void Entity::RotateRelativeTo(CVector3 pos, CVector3 vec, float angle)
{
	DQuaternion rotation(vec, angle);

	DQuaternion pQ(0.0f, mPosition.x + pos.x, mPosition.y + pos.y, mPosition.z + pos.z);

	pQ = pQ * rotation;
	pQ = rotation.inverse() * pQ;
	mRenderPosition = CVector3(pQ.x, pQ.y, pQ.z);

	mOrientation = rotation * mOrientation;
}


void Entity::TranslateLocalX(float amount)
{

}


void Entity::TranslateLocalY(float amount)
{

}


void Entity::TrnaslateLocalZ(float amount)
{

}


void Entity::TrnaslateWorldX(float amount)
{

}


void Entity::TranslateWorldY(float amount)
{

}


void Entity::TranslateWorldZ(float amount)
{

}


void Entity::TranslateVector(float amount, CVector3 direction)
{

}

