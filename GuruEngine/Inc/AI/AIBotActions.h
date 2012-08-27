/****************************************************************************************
Author: Dan Strycharske
Date:	3/4/2012
File:	AIBotActions.h
Brief:  Explain the purpose...

****************************************************************************************/


#ifndef AIBOTACTIONS_H
#define AIBOTACTIONS_H


#include <vector>
#include "PlannerDataType.h"


class AIBot;
class AIBotAction;
typedef std::vector< AIBotAction* >	ACTION_SET;


/****************************************

CLASS:		AIBotAction

ABSTRACT BASE CLASS.

***************************************/
class AIBotAction
{
public:
	//Constructor for the action.
	AIBotAction );

	AIBotAction(MAP_STATE_ARRAY preReqs, MAP_STATE_ARRAY endState);

	//This function will check all the pre-req states in it's pre-req array with the associated AI
	// objects state array, as well as the worlds array.
	virtual		bool	CheckActionPreRequisites( MAP_STATE_ARRAY pCheckerStates ) = 0;

	//This function will be pure virtual, as each action is going to define
	// how it executes.  Before it executes, it must check the pre-reqs, along
	// with any other additional checks it might need to perform.  If successful,
	// it executes, updating the associated objects state array, as well as the
	// world's array if necessary.
	virtual		bool	ExecuteAction( void ) = 0;

	void	AddPreRequisite( STATE_PAIR pAddPreReq )	 { mPreRequisites.insert( pAddPreReq ); }
	void	AddEndState(     STATE_PAIR pAddEndState)	 { mEndStates.insert( pAddEndState ); }

	AIBotAction*	HasParent() const	{ if(mParentAction)		 {return mParentAction;} return NULL; }
	void	SetParentAction( AIBotAction* parentAction ) { mParentAction = parentAction; }

	float	Weight() const	{ return mWeight; }
	void	SetWeight(float pWeight)	{ mWeight = pWeight; }


protected:
	AIBot*		mOwnerBot;
	
	//This let's us know which goal that it will help satisfy.
	AI_GOAL			mGoalAffector;

	//The associated weight of this action.
	float			mWeight;

	//The two state arrays, pre-reqs and endStates.
	MAP_STATE_ARRAY	mPreRequisites;
	MAP_STATE_ARRAY mEndStates;

	//The Action preceding this one in the sequence.  If NULL, then this is the start.
	AIBotAction*	mParentAction;
};


#endif
