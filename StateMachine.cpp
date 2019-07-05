/*****************************************************************************

                               StateMachine.cpp

						Copyright 2001, John J. Bolton
	----------------------------------------------------------------------

	$Header: //depot/Libraries/StateMachine/StateMachine.cpp#2 $

	$NoKeywords: $

*****************************************************************************/

#include "StateMachine.h"

#include "Misc/assert.h"

/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

bool StateMachine::Step( bool syncOverride )
{
	// Loop here stepping through the state diagram until the current
	// state's "synchronized" flag is true or there is no transition to
	// take.

	// If syncOverride is true, then stop at the next state regardless
	// of its sync flag.

	do
	{
		// Find the first transition whose condition is true.

		Transition * const	pT	= m_States[ m_CurrentStateId ]->TestTransitions();

		// If none was found, then we have arrived at the end of the line

		if ( !pT )
		{
			return false;
		}

		// Take the transition

		int const nextState = pT->Traverse( m_CurrentStateId );

		// Activate the next state

		ActivateState( nextState );		// Note: Changes m_CurrentStateId

	} while ( !( m_States[ m_CurrentStateId ]->IsSynchronized() || syncOverride ) );

	return true;	// Return success
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

void StateMachine::ActivateState( int id )
{
	assert_limits( 0, id, (int)m_States.size()-1 );

		// Set the new current state

	m_CurrentStateId = id;

	// Execute the action for this state

	m_States[ m_CurrentStateId ]->Activate( id );
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

StateMachine::Transition * StateMachine::State::TestTransitions()
{
	// Search for the first TRUE condition in this state's transition
	// list.

	for ( TransitionList::iterator pT = m_Transitions.begin(); pT != m_Transitions.end(); ++pT )
	{
		if ( ( *pT )->Test() )
		{
			return *pT;
		}
	}

	// A transition with a TRUE condition was not found

	return 0;
}

