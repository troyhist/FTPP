// =======================================================================
// Petri's Nets Processor ver. 1.0 (CPP only)
// Copyright (c) 1994-2001 Kolevatov Sergy
// -----------------------------------------------------------------------
// Petri's Nets Basic Element class
// Used library: <none>
// -----------------------------------------------------------------------
// File: PetriElem.h
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 03/ IV.2001
// Last edit: 03/ IV.2001
//     Build: 1
// =======================================================================
// PetriElem.h: interface for the CPetriElem class.
// =======================================================================
#if !defined(AFX_PETRIELEM_H__72A71CE2_2830_11D5_B79E_00A024CF782E__INCLUDED_)
#define AFX_PETRIELEM_H__72A71CE2_2830_11D5_B79E_00A024CF782E__INCLUDED_
// =======================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// =======================================================================
// DO NOT CHANGE !!! (Sergy/07.04.01)
// =======================================================================
#define PE_ERROR	(-1)		// Undefined Error (07.04.01)
// -----------------------------------------------------------------------
// Petri's Nets Element's type
// -----------------------------------------------------------------------
#define PE_NONE			(0)		// No type: Error
#define PE_PLACE		(1)		// Type: Place
#define PE_TRANSITION	(2)		// Type: Transition
#define PE_CONNECTION	(3)		// Type: Connection (not in use)
// -----------------------------------------------------------------------
// Predefined size of array with In and Out ray pointers
// -----------------------------------------------------------------------
#define PE_DEFARRSIZE	(64)	// For small Petri Nets (64 elements)
// =======================================================================
class CPetriElem  
{
// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------
public:		
	BOOL TransFireProbabTest();	// Returns 'true' if transition can fire with given probab
	BOOL TransLoadIntoPM();	// Transition takes Tokens from source places
	BOOL TransIsEnabled();	// Returns 'true' if transition is enabled
	BOOL TransFireFromPM();	// Transition fires with Tokens into destination places
	BOOL DefineTransition(int nID, int nTTF, int nProbab);
	BOOL DefinePlace(int nID, int nTokens);
	int  TransGetProbab();
	BOOL TransSetProbab(int nProbab);
	int TransGetTTF();					// Returns MAX ttf
	int GetType();						// Returns type of element
	int GetID();						// Returns ID of element	
	char* GetName();					// Returns name of element
	int PlaceGetTokens();				// Returns quantity of tokens in element
	BOOL SetComment(char* sComment);
	BOOL SetName(char* sName);
	BOOL SetID(int nID);
	BOOL Reset();					// Resets all internal data
	BOOL TransSetTTF(int nTTFmax);			// sets value of MAX 'time to fire'
	BOOL PlaceSetTokens(int nN);			// sets new quantity of tokens
	BOOL ConnectTo(CPetriElem* cPElem);	// connects two elements
	BOOL IsDefined();					// checks definition of element
	BOOL Define(int nID, int nType);	// defines an element
// -----------------------------------------------------------------------
	CPetriElem();						// default constructor
	CPetriElem(int nID, int nType);		// c. with definition
	virtual ~CPetriElem();				// destructor
	// -------------------------------------------------------------------
// -----------------------------------------------------------------------
protected:
// -----------------------------------------------------------------------
	BOOL AutoGenerateComment();			// Auto generates comment
	BOOL AutoGenerateName();			// Auto generates name
	BOOL AddRayIn(CPetriElem* cPElem);	// adds one inbound ray
	BOOL AddRayOut(CPetriElem* cPElem);	// adds one outbound ray
// -----------------------------------------------------------------------
private:
// -----------------------------------------------------------------------
	BOOL AddRayIn();
	BOOL AddRayOut();
	// -------------------------------------------------------------------
	BOOL TokenAdd();
	BOOL TokenSub();
	// -------------------------------------------------------------------	
// -----------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------
private:				
// -----------------------------------------------------------------------
	// Process data
	// -------------------------------------------------------------------
	BOOL	m_bDefined;		// flag "is element defined ?"
	int		m_nID;			// number (ID) of element
	int		m_nType;		// type of elem: PE_PLACE, PE_TRANSITION
	// -------------------------------------------------------------------
	// PE_PLACE attributes
	// -------------------------------------------------------------------
	int		m_nTokens;		// quantity of tokens in the element
	// -------------------------------------------------------------------
	// PE_TRANSITION attributes
	// -------------------------------------------------------------------
	int		m_nTTFmax;		// max "time to fire"
	int		m_nProbab;		// probability of firing
	//int		m_nTTF;			// current "time to fire"
	//BOOL	m_bTransloaded; // flag "transition is loaded"
	// -------------------------------------------------------------------
	int		m_nInrays;		// quantity of inbound ray
	int		m_nOutrays;		// quantity of outbound ray
	// -------------------------------------------------------------------
	CPetriElem** m_pInrays;	// array of ptrs to inbound rays
	CPetriElem** m_pOutrays;// array of ptrs to outbound rays
	// -------------------------------------------------------------------
	// I/f data
	// -------------------------------------------------------------------	
	char m_sName[16];		// text name of element
	char m_sComment[64];	// comment
// -----------------------------------------------------------------------
protected:
// -----------------------------------------------------------------------
};
// =======================================================================

#endif // !defined(AFX_PETRIELEM_H__72A71CE2_2830_11D5_B79E_00A024CF782E__INCLUDED_)
// =======================================================================