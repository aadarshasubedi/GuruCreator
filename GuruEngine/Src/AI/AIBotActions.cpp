
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	AIBotActions.cpp
brief:	

*/

#include "AIBotActions.h"
#include "AIBot.h"
#include "Movement.h"
#include "Graphics.h"

//Constructor for the action.
AIBotAction::AIBotAction( AIBot*	pOwnerBot )
: mOwnerBot( pOwnerBot), mParentAction(NULL)
{
}

AIBotAction::AIBotAction( AIBot* pOwnerBot, MAP_STATE_ARRAY preReqs, MAP_STATE_ARRAY endState )
: mOwnerBot( pOwnerBot), mParentAction(NULL), mPreRequisites(preReqs), mEndStates(endState)
{
}





AIBotAction_MOVE::AIBotAction_MOVE( AIBot* pOwnerBot) : AIBotAction(pOwnerBot)
{
	mGoalAffector = GOAL_PATROL;
}


AIBotAction_MOVE::AIBotAction_MOVE( AIBot* pOwnerBot, MAP_STATE_ARRAY preReqs, MAP_STATE_ARRAY endState ) 
: AIBotAction(pOwnerBot, preReqs, endState)
{
	mGoalAffector = GOAL_PATROL;
}

bool	AIBotAction_MOVE::CheckActionPreRequisites( MAP_STATE_ARRAY pCheckerStates )
{
	//See if the pre-reqs have been set for this action.  If so, the action can execute.  If not
	//  it means this state now needs to be put up on the 

	bool pass = false;

	unsigned numPreqs = (unsigned)mPreRequisites.size();
	for( unsigned preqsIter = 0; preqsIter < numPreqs; ++preqsIter )
	{
		MAP_STATE_ARRAY::iterator stateBegin = mPreRequisites.begin(), stateEnd = mPreRequisites.end();

		while( stateBegin != stateEnd )
		{
			char* name = (*stateBegin).first;
			MAP_STATE_ARRAY::iterator iter = pCheckerStates.find( name );
			MAP_STATE_ARRAY::iterator end = pCheckerStates.end();

			if( iter == end )
			{
			}
			else
			{
				//get the first pre-req, find it in the checker state, and compare data.
				STATE_PAIR	pair = (*iter);
				StateData* data = pair.second;

				//find out the type of data this is, and compare it.
				switch( data->IsType() )
				{
					case StateData::TYPES::UNSIGNED :
					{
						if( ((StateData_UNSIGNED*)data)->GetDataAsUnsigned() == ((StateData_UNSIGNED*)(*stateBegin).second)->GetDataAsUnsigned() )
						{
							pass = true;
						}
						else
						{
							pass = false;
						}
						break;
					}

					case StateData::TYPES::FLOAT :
					{
						if( ((StateData_FLOAT*)data)->GetDataAsFloat() == ((StateData_FLOAT*)(*stateBegin).second)->GetDataAsFloat() )
						{
							pass = true;
						}
						else
						{
							pass = false;
						}
						break;
					}

					case StateData::TYPES::BOOL :
					{
						if( ((StateData_BOOL*)data)->GetDataAsBool() == ((StateData_BOOL*)(*stateBegin).second)->GetDataAsBool() )
						{
							pass = true;
						}
						else
						{
							pass = false;
						}
						break;
					}

					case StateData::TYPES::INT :
					{
						if( ((StateData_INT*)data)->GetDataAsInt() == ((StateData_INT*)(*stateBegin).second)->GetDataAsInt() )
						{
							pass = true;
						}
						else
						{
							pass = false;
						}
						break;
					}

					default :
					{
						pass = false;
						break;
					}
				}

				if( pass )
				{
					++stateBegin;;
				}
				else
				{
					break;
				}
			}
		}
	}
	return pass;
}

bool	AIBotAction_MOVE::ExecuteAction( void )
{
	if( mOwnerBot )
	{
		Movement*	mover = mOwnerBot->Owner()->GetMovement() ;
		CPicker*	picker = CGraphics::GetSingleton().picker;
		if( mover && picker->GetPickedObject() )
		{
			mover->SetGoalNode((PathingNode*)picker->GetPickedObject());
			mover->InitializeAStarState();
			//mover->InitializeAStarState();
		}
	}
	return true;
}


