
#include "AIBot.h"

AIBot::AIBot(  GameObject* pOwner ) : mOwner( pOwner )
{
}


void	AIBot::GiveAIBotGoal( AIBotGoal* pGoal )
{ 
	mGoalSet.PushQueue( pGoal );
}

void	AIBot::GiveAIBotAction( AIBotAction* pAction )
{ 
	mActionSet.push_back( pAction );
}

bool	AIBot::SatisfyGoals()
{
	//@todo:	AI checks it's met it's top priority goal, that is if there's a plan.  This will of course
	//				use the planning manager, which is a singleton.  It will be used for all the AIBots needing
	//				to plan, and so thusly will be available.  It will act as the A* glue to meet states.  

	return true;
}


void	AIBot::AddBotState( STATE_PAIR	 pNewStateForBot )
{
	mAIStates.insert( pNewStateForBot );
}




