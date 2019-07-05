#if !defined( STATEMACHINE_H_INCLUDED )
#define STATEMACHINE_H_INCLUDED

#pragma once

/*****************************************************************************

                                StateMachine.h

						Copyright 2001, John J. Bolton
	----------------------------------------------------------------------

	$Header: //depot/Libraries/StateMachine/StateMachine.h#2 $

	$NoKeywords: $

*****************************************************************************/


#include <vector>

class State;

/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

class StateMachine
{
public:

	class State;
	class Transition;

	StateMachine( int nStates = 0, int initial_state = 0 );
	virtual ~StateMachine() {}

	// Add a state to the state machine
	void AddState( int id, State * state );

	// Step the state machine
	virtual bool Step( bool masterSync = false );

	// Set/Get the current state id
	void SetCurrentState( int id )			{ m_CurrentStateId = id; }
	int GetCurrentStateId() const			{ return m_CurrentStateId; }


protected:

	// Shorthand for the state vector type
	typedef std::vector< State * > StateList;

	// Activate a state
	void ActivateState( int id );

	// The current state
	int m_CurrentStateId;

	// Vector of states
	StateList m_States;

};


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

class StateMachine::State
{
	friend class StateMachine;

public:

	typedef void ( *Action )( int id );

	State( bool s = true, Action a = 0 );
	virtual ~State() {}

	// Add a transition to the state
	void AddTransition( StateMachine::Transition * t );

	// Return true if this state is synchronized
	bool IsSynchronized() const					{ return m_Synchronized; }

	// Perform the action for entering this state
	virtual void Activate( int id );

	// Return the first transition whose condition is true
	StateMachine::Transition * TestTransitions();

protected:

	// Shorthand for the transition vector type
	typedef std::vector< StateMachine::Transition * > TransitionList;

	// If true, then the state machine stops at this state regardless of the transition conditions
	bool m_Synchronized;

	// Action taken when the state is activated.
	Action m_Action;

	// Vector of transitions from this state
	TransitionList m_Transitions;	
};


/************************************************************************/
/*																		*/
/*																		*/
/************************************************************************/

class StateMachine::Transition
{
	friend class StateMachine;

public:

	typedef bool ( *Condition )();
	typedef void ( *Action )( int fromId, int toId );

	Transition( int ns, Condition c = 0, Action a = 0 );
	virtual ~Transition() {}

	// Perform the action for this transition and returns the destination state
	virtual int Traverse( int fromId );

	// Return true if the condition for this transition is true
	virtual bool Test() const;


protected:

	// Condition to test in order to traverse this transition
	Condition m_Condition;

	// m_Action taken when this transition is traversed
	Action m_Action;

	// Id of destination state
	int m_NextState;
};


// Inline functions

#include "StateMachine.inl"


#endif // !defined( STATEMACHINE_H_INCLUDED )
