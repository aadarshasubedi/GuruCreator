
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PlannerDataType.h

brief:	This is an abstract class interface for the data types going to be used
		for the state variables.

*/

#ifndef PLANNERDATATYPE_H
#define PLANNERDATATYPE_H

#include <vector>
#include <map>

class StateData;

//typedef std::pair< char*, void* >		STATE_VALUE;
//typedef std::vector< STATE_VALUE >		GOAL_STATES;

typedef std::pair<char*, StateData*>	STATE_PAIR;
typedef std::map< char*, StateData* >	MAP_STATE_ARRAY;


typedef std::vector< STATE_PAIR >		STATE_ARRAY;

//Here is an enumeration of all the different goals the AI have.
enum	AI_GOAL {

	//In case no goals are met, use this default one.
	GOAL_IDLE = 0,
	GOAL_PATROL,
	GOAL_KILLPLAYER,
	GOAL_FINDPLAYER,
	GOAL_COVER,
	GOAL_NUMBEROFGOALS

};




class StateData
{

public:
	enum TYPES { NONE = 0, BOOL, INT, FLOAT, UNSIGNED };
	
public:
	//Constructor will set the void* of data to NULL.
	StateData();//{}

	//Destructor
	~StateData() { if( mData ) { delete mData; } }

	//The rest of the member functions will be pure virtual. 

	//The data will need to have a type associtated, here using an enum.
	virtual TYPES	IsType()	{ return NONE; }

	virtual void*	GetData()	{ return mData; }

	virtual char*	GetName()	{ return mName; }


	//Setters for the name and data.
	void			SetName( char* pName )	{ mName = pName; }

	void			SetData( void* pData )	{ mData = pData; }

	bool			WorldData()				{ return mWorldData; }

protected:
	//Two members for this class.

	//The actual void* which contains the physical data we'll need for this state variable.
	void*		mData;

	//This is the TYPES enum that tells us what this actually is.
	TYPES		mType;

	//This will be helpful to determine what this value actually is.
	char*		mName;

	//If true, this data is contained by the world, so handle it differently.
	bool		mWorldData;

};



class StateData_BOOL : public StateData
{

public:
	//Constructor.
	StateData_BOOL() : StateData() 
	{
		mData = new bool;
		*((bool*)mData) = false;

		mType = BOOL;
	}

	//The definitions of the virtual functions.
	virtual TYPES	IsType()	{ return BOOL; }

	bool			GetDataAsBool()	{ return *((bool*)GetData()); }
	void			SetDataAsBool( bool pData )	{ *((bool*)mData) = pData; }

private:

};


class StateData_FLOAT : public StateData
{

public:
	//Constructor.
	StateData_FLOAT() 
	{		
		mData = new float;
		*((float*)mData) = 0.f;
	}

	//The definitions of the virtual functions.
	virtual TYPES	IsType()						{ return FLOAT; }

	float			GetDataAsFloat()				{ return *((float*)GetData()); }
	void			SetDataAsFloat( float pData )	{ *((float*)mData) = pData; }

private:

};



class StateData_INT : public StateData
{

public:
	//Constructor.
	StateData_INT() 
	{
		mData = new int;
		*((int*)mData) = 0;
	}

	//The definitions of the virtual functions.
	virtual TYPES	IsType()			{ return INT; }
	
	int				GetDataAsInt()		{ return *((int*)mData); }

private:

};


class StateData_UNSIGNED : public StateData
{

public:
	//Constructor.
	StateData_UNSIGNED()
	{
		mData = new unsigned;
		*((unsigned*)mData) = 0;

		mType = UNSIGNED;
	}

	//The definitions of the virtual functions.
	virtual TYPES	IsType()			{ return UNSIGNED; }
	
	unsigned		GetDataAsUnsigned()	{ return *((unsigned*)GetData()); }

	void			SetDataAsUnsigned( unsigned pData )	{ (*(unsigned*)mData) = pData; }

private:

};


#endif


