
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	AIBotGoalSets.cpp
brief:	

*/

#include "AIBotGoalSets.h"
#include "World.h"	//Need to know the state of the world.



AIBotGoal::AIBotGoal( float weight ) : mWeight(weight)
{
}


//This function will take in and add a state pair to the 
void	AIBotGoal::AddStateToGoal( STATE_PAIR pPair )
{
	mGoalStates.push_back( pPair );
}

//Function checks the mGoalState against all appropriate state variables
// to see if they've all been satisfied.  
//bool	AIBotGoal::CheckIfGoalMet()
//{
//	//@todo:  This function will be a workhorse for the manager, since the goal itself
//	//			will check with al
//	//Okay, here's where a lot of work gets done.  
//	return true;
//}


