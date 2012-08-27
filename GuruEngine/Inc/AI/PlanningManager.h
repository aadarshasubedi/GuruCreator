
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PlanningManager.h
brief:	

*/


#ifndef PLANNINGMANAGER_H
#define PLANNINGMANAGER_H

//#include "CPriorityQueue.h"
#include "PlannerDataType.h"
#include "Singleton.h"
#include "CPriorityQueue.h"

/*
Class that interacts with the AI objects and the world to make decisions on what the
 object should do.

The AI object will also be specified here, so that anything can derive from this
 and be used by the planning manager.

The actions a bot can do will be specified in a class call AIBotAction.  This will 
 include the end state array for the action, the pre-requisite states, and functions
 to check pre-reqs and to execute the action.


*/

#include <vector>

//Classes implemented in this file.
class AIBotAction;
class AIBotGoal;
//
class AIBot;
//class AIPlanningManager;




/****************************************



***************************************/
class AIPlanningManager : public Singleton<AIPlanningManager>
{
	enum SEARCHMODE {SEARCH_INIT=0, SEARCH_SUCCESS, SEARCH_FAILED, SEARCH_PROCESSING };
	typedef std::vector< AIBotAction* >		GOAL_SATISFYING_ACTIONS;

public:
	AIPlanningManager();
	virtual ~AIPlanningManager();

		///A* FUNCTIONS///

	//Initializes all state variables and gets the goal node from the picker.  If
	//  picker has none, then no A* will be calculated as no goal or start has been
	//	selected.
	void	Init();

	//Helper function to see if this is the last action that is required to make the goal
	//	state happen.  If so, we're done and we can generate the action list.
	bool	ActionSatisfiesGoal( AIBotAction* testingAction  );

	//Checks to see if this node has a parent.
	bool	HasParentAction( AIBotAction* action );

	//Goes through the closed list and finds this node pointer as it's the parent of
	// the current node we're looking at.
	void	 FindParentAction( AIBotAction* parentAction, AIBotAction* action );

	//For during A*, this goes through all of the nodes connected bits and finds all of those
	//  it is connected with.  These will be put onto the open list to be checked later if 
	//	necessary as the algorithm dictates.
	void	 FindNeighborActions( );
	
	//Helpers to use and query the open and closed lists.
	void	PushOpenListQueue( AIBotAction* action );
	void	PushClosedListQueue( AIBotAction* action );
	void	PopOpenListQueue( AIBotAction* action );
	void	PopClosedListQueue( AIBotAction* action );

	//This is the actual "A* algorithm" function.  Short and sweet, it goes
	//	through the nodes, finds neighbors, does heuristic stuff, and checks
	//	next best node.
	void	SearchForPath();

	//This sets the goal node, calculates the first heuristic values for the start node,
	//	clears the waypoints, and begins the search processing to start.  Pushes start
	//	node onto the open list.  The start node has a parent pointer set NULL.
	void	InitializeAStarState();

	//Big work horse of the A* algorithm.  This is what will calculate heuristic values and check if it's on the
	//	open or closed list with lower values.
	void	AddActionToOpenList( AIBotAction* parentAction, AIBotAction* testingAction );

	//Waypoint list generation function.
	void	GenerateActionsFromClosedList( );

	//This function will be called during the update phase, 
	void	Update();

	//To clear data and memory once A* is done, mainly the open/closed lists.
	void	ClearAStarValues();

	//This function will loop through all the objects, find the ones with AIBots,
	//	and run them through the plan finding.  If there is no plan for that goal,
	//	move onto next goal, and try again.  If no goals satisfied, a default goal
	//	is always there to make them sit idle.
	void	GenerateAllBotPlans();


	//Takes all the actions in the GOAL_SATISFYING_ACTIONS list and executes them consecutively.
	//
	//
	bool	ExecuteActionSequence();

private:
	//This will kinda mimic the movement component, as that's the a* pathfinder for each bot.

	//So it needs to have a starting state, and a goal state it's striving for, and it needs
	// the current actions available to use.  There will be the openlist and the closedlist
	// of actions, and the various functions to check values and such.
	
	int					mSearchMode;

	//The two lists to be used during the path finding.
	CTemplatePriorityQueue<AIBotAction*>		mOpenlist;
	CTemplatePriorityQueue<AIBotAction*>		mClosedlist;

	//A pointer to the AIBots current goal state, and the assembled state array of the states we're 
	// concerned with.
	STATE_ARRAY*					mGoalStates;
	MAP_STATE_ARRAY					mCurrentStates;

	//The final list of actions that will 
	GOAL_SATISFYING_ACTIONS		mActionsCompletingGoal;


	////The game object who owns this component.
	AIBot*						mCurrentPlanningBot;

};




#endif




