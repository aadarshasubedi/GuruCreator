
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PlanningManager.cpp
brief:	

*/


#include "PlanningManager.h"
#include "AIBot.h"
#include "AIBotGoalSets.h"
#include "AIBotActions.h"
#include "Graphics.h"
#include "World.h"

using namespace PHYSICS;

AIPlanningManager::AIPlanningManager()
{
}

AIPlanningManager::~AIPlanningManager()
{
}

///A* FUNCTIONS///


//This function will loop through all the objects, find the ones with AIBots,
//	and run them through the plan finding.  If there is no plan for that goal,
//	move onto next goal, and try again.  If no goals satisfied, a default goal
//	is always there to make them sit idle.
void	AIPlanningManager::GenerateAllBotPlans()
{	
	//Call the init function which takes care of preparing the goal state array pointer,
	// the list going to be used for the
	typedef  std::vector<AIBot*>    BOTS;
	BOTS tempList;

	//Okay, grab a list of game objects that have the AIBot component.
	unsigned size = (unsigned)CGraphics::GetSingleton().enemiesList.size();
	for( unsigned objIter = 0; objIter < size; ++objIter )
	{
		//See if it has an aibot.
		if( CGraphics::GetSingleton().enemiesList[objIter]->GetAIBot() )
		{
			tempList.push_back( CGraphics::GetSingleton().enemiesList[objIter]->GetAIBot() );
		}
	}

	//Now we have them all, so one by one go through and set the current planning bot to
	// the current one, and set the goal states to the highest priority goal for the bot.
	//DON'T pop it, just front until we know for sure it's not a vaild action path, then we'll
	// actually pop.
	BOTS::iterator botBegin = tempList.begin(), botEnd = tempList.end();

	while( botBegin != botEnd )
	{
		mCurrentPlanningBot = (*botBegin);
		
		Init();

		//From here, this is where the A* basically takes over.  This is the while( SEARCH_PROCESSING ) things.
		Update();

		++botBegin;
	}
}


//Initializes all state variables and gets the goal node from the picker.  If
//  picker has none, then no A* will be calculated as no goal or start has been
//	selected.
void	AIPlanningManager::Init()
{
	//Okay, front the AIGoalSet off.
	AIBotGoal* goal = NULL;
	goal = mCurrentPlanningBot->FrontGoalSet();

	mGoalStates = goal->GetFrontStateArray();

	//Okay, we have the goal state set, we now need to match our current state with just those
	//	variables.

	unsigned size = (unsigned)(*mGoalStates).size();
	for( unsigned stateIter = 0; stateIter < size; ++stateIter )
	{
		STATE_PAIR pair = (*mGoalStates)[stateIter];
		if( pair.second->WorldData() )
		{			
			mCurrentStates.insert( std::make_pair( pair.first, World::GetSingleton().GetStateDataByName( pair.first ) ) );
		}
		else
		{			
			mCurrentStates.insert( std::make_pair( pair.first, mCurrentPlanningBot->GetStateDataByName( pair.first ) ) );
		}
	}

	//Grab the action with the lowest score from the aibot and use this as the starting action.
	mOpenlist.PushQueue( mCurrentPlanningBot->FrontAction() );

	mSearchMode = SEARCH_PROCESSING;
}



//Helper function to compare node pointers and see if it matches the goal node stored.
bool	AIPlanningManager::ActionSatisfiesGoal( AIBotAction* testingAction )
{
	//This action will create a state array based on all the goals state variables.
	//  This will then have this action, in ghost fashion, set the state array accordingly.
	//If this state array now matches with the goal state array, we're good.


	//Firstly the action must check with it's pre-reqs to see if it can even be performed.
	testingAction->CheckActionPreRequisites( mCurrentStates );

	MAP_STATE_ARRAY	newWorldStates = mCurrentStates;

	//UpdateTempStates( newWorldStates, testingAction );

	//unsigned size = (unsigned)mGoalStates.size();
	//for( unsigned stateIter = 0; stateIter < size; ++stateIter )
	//{

	//}


	
	//
	//unsigned size = (unsigned)mGoalStates.size();
	//for( unsigned stateIter = 0; stateIter < size; ++stateIter )
	//{
	//	//With the first state array, grab the stateData* and see if it's world or local.
	//	//From here, use the state name to look in the world or local map to get the data value.

	//	StateData* state = NULL;
	//	if( mGoalStates[stateIter].second->WorldData() )
	//	{
	//		//from the world, grab it.
	//		state = World::GetSingleton().GetStateDataByName( mGoalStates[stateIter].first );

	//	}
	//	else
	//	{
	//		//This state variable is from the bot, grab it.
	//	}
	//	
	//	switch( state->IsType() )
	//	{
	//		//Depending on type
	//	}
	//}
	
	return true;
}



//Checks to see if this node has a parent.
bool	AIPlanningManager::HasParentAction( AIBotAction* action )
{
	if( action->HasParent() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Goes through the closed list and finds this node pointer as it's the parent of
// the current node we're looking at.
void	AIPlanningManager::FindParentAction( AIBotAction* , AIBotAction*  )
{
}

//For during A*, this goes through all of the nodes connected bits and finds all of those
//  it is connected with.  These will be put onto the open list to be checked later if 
//	necessary as the algorithm dictates.
void	AIPlanningManager::FindNeighborActions( )
{
}

//Helpers to use and query the open and closed lists.
void	AIPlanningManager::PushOpenListQueue( AIBotAction* action )
{
	mOpenlist.PushQueue( action );
}
void	AIPlanningManager::PushClosedListQueue( AIBotAction* action )
{
	mClosedlist.PushQueue( action );
}
void	AIPlanningManager::PopOpenListQueue( AIBotAction* action )
{
	mOpenlist.PopQueue( action );
}
void	AIPlanningManager::PopClosedListQueue( AIBotAction* action )
{
	mClosedlist.PopQueue( action );
}

//This sets the goal node, calculates the first heuristic values for the start node,
//	clears the waypoints, and begins the search processing to start.  Pushes start
//	node onto the open list.  The start node has a parent pointer set NULL.
void	AIPlanningManager::InitializeAStarState()
{
}

//Big work horse of the A* algorithm.  This is what will calculate heuristic values and check if it's on the
//	open or closed list with lower values.
void	AIPlanningManager::AddActionToOpenList( AIBotAction* , AIBotAction*  )
{
}

//Waypoint list generation function.
void	AIPlanningManager::GenerateActionsFromClosedList( )
{
	//Go through the closed list, starting with the goal.  It should be last always but we'll find
	// it by address here to be sure.
	bool startNodeFound = false;

	if( mClosedlist.IsQueueEmpty() )
	{
	}
	else
	{
		AIBotAction* tempAction = NULL;
		mClosedlist.FrontOfQueue(tempAction);

		//mPathpointList.clear();

		while( !startNodeFound )
		{
			mActionsCompletingGoal.push_back( tempAction );

			//mPathpointList.push_back( temp->GetPosition() );
			tempAction = tempAction->HasParent();

			if( tempAction )
			{
				//The start hasn't been found yet, keep going.
			}
			else
			{
				//We found the start, so set the boolean and be done.
				startNodeFound = true;
			}
		}
	}
}
	
bool	AIPlanningManager::ExecuteActionSequence()
{
	bool pass = true;

	//Loop through the actions and call the execute for them all.
	unsigned actionsSize = (unsigned)mActionsCompletingGoal.size();
	for( unsigned actionIter = 0; actionIter < actionsSize; ++actionIter )
	{		
		//Execute each one.
		pass = mActionsCompletingGoal[actionIter]->ExecuteAction();
	}

	return pass;
}


//This is the actual "A* algorithm" function.  Short and sweet, it goes
//	through the nodes, finds neighbors, does heuristic stuff, and checks
//	next best node.
void	AIPlanningManager::SearchForPath()
{
	if( mSearchMode == SEARCH_SUCCESS || mSearchMode == SEARCH_FAILED )
	{
		//We're done i guess.
		return;
	}
	
	if( !mOpenlist.IsQueueEmpty() )
	{
		
		AIBotAction* action = NULL;
		mOpenlist.PopQueue(action);

		if( ActionSatisfiesGoal(action) )
		{
			mSearchMode = SEARCH_SUCCESS;
			PushClosedListQueue(action);
		}
		else
		{
			//FindNeighborsOfNode(action);
			PushClosedListQueue(action);
		}
	}
	else
	{
		//Fail check
		mSearchMode = SEARCH_FAILED;
	}
}


//This function will be called during the update phase, 
void	AIPlanningManager::Update()
{
	//If path successfully found, do that stuff here.
	if( SEARCH_INIT == mSearchMode )
	{
		//Do nothing.
	}
	
	//If path not yet found but still looking, do that here.
	if( SEARCH_PROCESSING == mSearchMode )
	{
		//mMovementType = MOVEMENT_NULL;

		//Check if it's single step of not.
		//if( mSingleStep )
		//{
		//	SearchForPath();
		//}
		//else
		//{
		while( SEARCH_PROCESSING == mSearchMode )
		{
			SearchForPath();
		}
		//}
	}

	if( SEARCH_SUCCESS == mSearchMode )
	{
		//Then generate the actual path from the closed list.
		GenerateActionsFromClosedList( );

		mOpenlist.Clear();
		mClosedlist.Clear();
		mGoalStates = NULL;

		ExecuteActionSequence();
	}

	

	//If path failed to be found, do that here.
	if( SEARCH_FAILED == mSearchMode )
	{
		//YOU SUCK!!!!
		//
		//Clear waypoint list.
		//And push your own position to keep character from walking in place.

		//m_waypointList.clear();
		//m_waypointList.push_back( m_owner->GetBody().GetPos() );

		//g_terrain.ResetColors();

		mOpenlist.Clear();
		mClosedlist.Clear();
		mGoalStates = NULL;
		//mWaypointList.clear();
		//mPathpointList.clear();

		mSearchMode = SEARCH_INIT;
	}
}


void	AIPlanningManager::ClearAStarValues()
{
}

