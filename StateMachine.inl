#if !defined( STATEMACHINE_INL_INCLUDED )
#define STATEMACHINE_INL_INCLUDED

#pragma once

/*****************************************************************************

                               StateMachine.inl

						Copyright 2001, John J. Bolton
	----------------------------------------------------------------------

	$Header: //depot/Libraries/StateMachine/StateMachine.inl#2 $

	$NoKeywords: $

*****************************************************************************/

/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline StateMachine::StateMachine( int nStates, int initial_state )
{
	m_CurrentStateId = initial_state;

	if ( nStates > 0 )
		m_States.resize( nStates );
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline void StateMachine::AddState( int id, StateMachine::State * state )
{
	// If the entry for the state being added has not been allocated, then
	// make space for it ( and all the ones before it too).

	if ( id >= (int)m_States.size() )
	{
		m_States.resize( id + 1 );
	}

	m_States[ id ] = state;
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline StateMachine::State::State( bool s, StateMachine::State::Action a )
	: m_Synchronized( s ), m_Action( a )
{
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline void StateMachine::State::AddTransition( StateMachine::Transition * t )
{
	m_Transitions.push_back( t );
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline void StateMachine::State::Activate( int id )
{
	// If this state has an action associated with entering it, then do it.

	if ( m_Action )
		( *m_Action )( id );
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline StateMachine::Transition::Transition( int ns,
											 StateMachine::Transition::Condition c,
											 StateMachine::Transition::Action a )
	: m_Condition( c ),
	m_NextState( ns ),
	m_Action( a )
{
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline int StateMachine::Transition::Traverse( int fromId )
{
	// If an action was specified, then do it.

	if ( m_Action )
		( *m_Action )( fromId, m_NextState );

	return m_NextState;
}


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

inline bool StateMachine::Transition::Test() const
{
	// If a condition test was specified, then return the results of
	// the test. Otherwise, the transition is unconditional, so return
	// true.

	return ( m_Condition ) ? ( *m_Condition )() : true;
}


#endif // !defined( STATEMACHINE_INL_INCLUDED )
