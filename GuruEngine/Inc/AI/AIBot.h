/****************************************************************************************
Author: Dan Strycharske
Date:	3/4/2012
File:	AIBot.h
Brief:  Explain the purpose...

****************************************************************************************/


#ifndef AIBOT_H
#define AIBOT_H


#include "AIBotGoalSets.h"
#include "CPriorityQueue.h"
#include "AIBotActions.h"
#include <vector>


/****************************************


The bot is the base AI class which will give whatever entity the ability to interact with the
 planner so as to get planning results.  

***************************************/
class AIBot
{
	//The bot needs to know the current goals and abilities that it possesses so that the manager
	// can for each goal, go through the list of actions available and find the best path to 
	// satisfy the goal.
public:
	AIBot();

	//The bot should have a function like satisfy top goal, and if it can't, pop and put on bottom,
	// and try next goal.  A false will be returned if none of the goals could be satisfied, in which
	// case a default goal will be specified directly.
	bool	SatisfyGoals();


	//Function to add a goal to the set.  
	void	GiveAIBotGoal( AIBotGoal* pGoal );
	void	GiveAIBotAction( AIBotAction* pAction );

	void	AddBotState( STATE_PAIR	 pNewStateForBot );

	AIBotGoal*	FrontGoalSet( )					
	{ 
		AIBotGoal* dummy = NULL;
		mGoalSet.FrontOfQueue( dummy ); 
		return dummy;
	}

	AIBotAction*	FrontAction()
	{
		if( 0 == mActionSet.size() )
		{
			//Do nothing, no actions.
			return NULL;
		}
		else
		{
			//Just take the first one and go with it.
			return mActionSet[0];
		}
	}

	StateData*		GetStateDataByName( char* pName )		
	{ 
		return mAIStates.find(pName)->second;
	}

	GameObject*		Owner()			{ return mOwner; }

private:
	//The AIBot needs to know what it's goals are, in priority, and what it's list of available
	// actions are.  The actions will be weighted, and the goals will be in a priority queue.
	CTemplatePriorityQueue<AIBotGoal*>		mGoalSet;

	//This is the list of the AI's available abilities. 
	ACTION_SET								mActionSet;

	//This state array is going to be one of the things the planning manager interfaces.
	MAP_STATE_ARRAY							mAIStates;

	//The owner of this AIBot component.
	GameObject*								mOwner;

};




#endif


