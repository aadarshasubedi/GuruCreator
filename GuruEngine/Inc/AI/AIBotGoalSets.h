/****************************************************************************************
Author: Dan Strycharske
Date:	3/4/2012
File:	AIBotGoalSets.h
Brief:  Explain the purpose...

****************************************************************************************/


#ifndef AIBOTGOALSETS_H
#define AIBOTGOALSETS_H


#include "PlannerDataType.h"
#include "CPriorityQueue.h"


/****************************************

CLASS:				AIBotGoal

MEMBER DATA:
	1. STATE_ARRAY mGoalState

MEMBER FUNCTIONS: 
	1. CheckIfGoalSatisfied
		RETURNS: bool, If all state data in mGoalState is satisfied, return true.
		
***************************************/
class AIBotGoal
{
	//The only thing the goal has is a state array which represents 
	// the end state variables in order to achieve the goal.
public:
	AIBotGoal(float weight = 1.f );

	//Function checks the mGoalState against all appropriate state variables
	// to see if they've all been satisfied.  
	//bool		CheckIfGoalMet();
	//void		SetName( char*	pName )	{ mName = pName; }


	//For getting and manipulating the weight
	void		SetWeight( float pWeight )		{ mWeight = pWeight; }
	float		Weight() const					{ return mWeight; }
	float&		Weight()						{ return mWeight; }

	void		SetGoalType( AI_GOAL goalType ) { mGoal = goalType; }
	AI_GOAL		GoalType() const				{ return mGoal; }
	AI_GOAL&	GoalType()						{ return mGoal; }

	//This function will take in and add a state pair to the 
	void		AddStateToGoal( STATE_PAIR pPair );

	STATE_ARRAY*	GetFrontStateArray()			
	{ 
		//This basically means it's a NULL state, so don't consider it.
		if( !mGoalStates.empty() )
		{
			return &mGoalStates;
		}
		return NULL;
	}


private:
	//The end states the goal needs to have satisfied 
	STATE_ARRAY		mGoalStates;

	//The weight this goal has.  This will fluxuate and can be set.
	float			mWeight;

	//The goals should all have a name.
	AI_GOAL			mGoal;
};


#endif


