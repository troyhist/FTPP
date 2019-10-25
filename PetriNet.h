// =======================================================================
// Petri's Nets Processor ver. 1.0 (CPP only)
// Copyright (c) 1994-2001 Kolevatov Sergy
// -----------------------------------------------------------------------
// Petri's Nets main processor
// Used library: PetriElem
// -----------------------------------------------------------------------
// File: PetriNet.h
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 08/ IV.2001
// Last edit: 08/ IV.2001
//     Build: 1
// =======================================================================
// PetriNet.h: interface for the CPetriNet class.
// =======================================================================
#if !defined(AFX_PETRINET_H__B0C216B6_2BF4_11D5_8231_000000000000__INCLUDED_)
#define AFX_PETRINET_H__B0C216B6_2BF4_11D5_8231_000000000000__INCLUDED_
// =======================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// =======================================================================
#include "FileMPN.h"			// Sergy/04.05.2002
// -----------------------------------------------------------------------
#include "PetriElem.h"			// Sergy/08.04.01
// =======================================================================
// Features define (editable)
// =======================================================================
#define	PN_MAXITER		(150)	// Maximal number of iterations 
								//  -to iterate (Sergy/20.05.01)
// =======================================================================
// DO NOT EDIT !!!
// =======================================================================
// Marking types (07.05.2002)
// -----------------------------------------------------------------------
#define PN_MARKNORM		(0)		// normal marking
#define PN_MARKTERM		(1)		// terminal marking
#define PN_MARKDUB		(2)		// dubbing marking
#define PN_MARKOVER		(3)		// overlapping marking
// =======================================================================
#define PN_ERROR		(-1)	// Global Error
#define PN_MAINPROCESS  (-1)	// Main process ID (20.05.2002)
#define PN_UNLITER		(-1)	// Unlimited iterations
#define PN_UNLPROC		(-1)	// Unlimited processors
#define PN_DEFCRIT		(0)		// Default criteria
// =======================================================================
// Criterias for alternative branches selection (Sergy/21.05.2001)
// =======================================================================
#define PN_DEFCRIT			(0)	// DEFAULT: unknown
#define PN_CRMAXWAITTIME	(1)	// Max wait time					(NO)
#define PN_CRMINWRKTIME		(2)	// Min working time
#define PN_CRMAXWRKTIME		(3)	// Max working time
#define PN_CRMAXNUMCHLD		(4)	// Max number of children
#define PN_CRMINPASSTIME	(5)	// Path with minimal passing time	(NO)
#define PN_CRMAXRESTTIME	(6)	// Path with maximal rest time		(NO)
// =======================================================================
class CPetriNet  
{
// -----------------------------------------------------------------------
// !!! PUBLIC STRUCTURES !!!
// -----------------------------------------------------------------------
public:
// -----------------------------------------------------------------------
// Structures
// -----------------------------------------------------------------------
	// -------------------------------------------------------------------
	// PLACES list (09.05.2002)
	// -------------------------------------------------------------------
	struct tagRSRCPL
		{
		int m_nID;				// ID
		char m_sName[16];		// name of the place
		};
	typedef tagRSRCPL protlpl;
	// -------------------------------------------------------------------
	// TRANSITIONs list (09.05.2002)
	// -------------------------------------------------------------------
	struct tagRSRCTR
		{
		int m_nID;				// ID
		int m_nTTF;				// TTF max
		int m_nProbab;			// Probability
		char m_sName[16];		// name of the place
		};
	typedef tagRSRCTR protltr;
	// -------------------------------------------------------------------
	// PMs list (09.05.2002)
	// -------------------------------------------------------------------
	struct tagRSRCPM
		{
		int m_nID;				// ID
		char m_sName[16];		// name of the place
		};
	typedef tagRSRCPM protlpm;
	// -------------------------------------------------------------------
	// Processes list (08.06.2002)
	// -------------------------------------------------------------------
	struct tagRSRCPROC
		{
		int m_nID;				// ID
		char m_sName[16];		// name of the process
		};
	typedef tagRSRCPROC rsrcproc;
	// -------------------------------------------------------------------
	// PM state record (09.05.2002)
	// -------------------------------------------------------------------
	struct tagLOGPMSTATE
		{		
		BOOL m_bStatusOk;	// Processor can function flag
		BOOL m_bBusy;		// Busy flag
		int  m_nCurTaskID;	// current task ID
		int  m_nTTFcur;		// current TTF of current task
		int  m_nTTFmax;		// max TTF of current task
		};
	typedef tagLOGPMSTATE logpmst;
	// -------------------------------------------------------------------
	// Marking types structure for processes (19.05.2002)
	// -------------------------------------------------------------------
	struct tagLOGMARKTYPE
		{
		int m_nMarkType;	// Marking type
		int m_nMarkRef;		// Marking reference
		};
	typedef tagLOGMARKTYPE marktype;
	// -------------------------------------------------------------------
	// History protocol record (09.05.2002)
	// -------------------------------------------------------------------
	struct tagLOGREC
		{
		int m_nCurIterNum;		// current iteration number
		int m_nMarkType;		// marking type (NORM, TERM, DUB, OVER)
		int m_nMarkRef;			// refernces for equal marking		
		BOOL m_bPMStagn;		// PMs' stagnation (20.05.2002)
		int* m_pTokens;			// marking
		logpmst* m_pPMST;		// PMs state
		marktype* m_pProcs;		// Marking types for processes
		};
	typedef tagLOGREC protrec;
	// -------------------------------------------------------------------
	// Processes Statistic (Time, Cycles, etc) (19.05.2002)
	// -------------------------------------------------------------------
	struct tagSTATPROC
		{
		int m_nID;			// process ID
		int m_nCycles;		// cycles
		int m_nTimeInCycles;// time in cycles
		int m_nMarkDub;		// Number of DUB markings (20.05.2002)
		int m_nMarkOver;	// Number of OVER markings (20.05.2002)
		int m_nStagns;		// Number of Stagnations of the process (21.05.2002)
		};
	typedef tagSTATPROC statproc;
	// -------------------------------------------------------------------
	// Statistic information (18.05.2002)
	// -------------------------------------------------------------------
	struct tagSTATISTICS
		{
		BOOL m_bInit;	// inited
		int m_nProcs;	// number of processes (16.05.2002)
		int m_nMaxParallPMs;	// number of parallel-working PMs
		int m_nMinParallPMs;	// number of parallel-working PMs
		statproc m_tPMain;		// Main process (20.05.2002)
		statproc* m_pProcs;		// ptr to processes
		};
	typedef tagSTATISTICS logstat;
	// -------------------------------------------------------------------
	// Protocol records list record (09.05.2002)
	// -------------------------------------------------------------------
	struct tagLOG
		{
		int m_nSize;			// size of history protocol
		protrec* m_pHistory[3];	// history protocol (05.05.2002)
		};
	typedef tagLOG protlist;
	// -------------------------------------------------------------------
	// Protocol resources (28.05.2002) (not ready jet)
	// -------------------------------------------------------------------
	struct tagRSRC
		{
		BOOL m_bInited;	// "inited" flag (28.05.2002)
		int m_nPlaces;	// number of places in the net
		int m_nTrans;	// number of transitions in the net (09.05.2002)
		int m_nConns;	// number of connections in the net (18.05.2002)
		int m_nPMs;		// number of PMs in the net		
		int m_nProcs;	// number of processes (19.05.2002)		
		protlpl* m_pLPL;	// Places list
		protltr* m_pLTR;	// Trans list
		protlpm* m_pLPM;	// PMs list
		rsrcproc* m_pLPROC;	// Processes list (08.06.2002)
		};
	typedef tagRSRC logrsrc;
// -----------------------------------------------------------------------
// !!! STRUCTURES for INTERNAL USE !!!
// -----------------------------------------------------------------------
private:
// -----------------------------------------------------------------------
	// -------------------------------------------------------------------
	// CONNECTION structure (Sergy/04.05.2002)
	// -------------------------------------------------------------------
	struct tagCONN
		{
		int m_nID;				// processor(PM) ID
		int m_nType;			// type of connection (not uses)
		int m_nMult;			// multiple of connections (not supported)
		int m_nDirType;			// Direction of connection:	(not uses)
		int m_nCIDa;			// SRC element ID (PLACE or TRANS)
		int m_nCIDb;			// DEST element ID (PLACE or TRANS)
		};
	typedef tagCONN conn;
	// -------------------------------------------------------------------
	// PM structure (Sergy/04.05.2002)
	// -------------------------------------------------------------------
	struct tagLISTPM
		{
		int  m_nID;			// processor(PM) ID
		BOOL m_bStatusOk;	// flag "processor is non-faulting"		
		BOOL m_bBusy;		// flag "I'm busy"		
		int  m_nCurTaskID;	// current task ID
		int  m_nTTFcur;		// current TTF of task
		int  m_nTasks;		// number of tasks are solved on this PM
		int* m_pTaskIDs;	// Tasks list (list with tasks ID)
		char m_sName[16];	// name of PM
		};
	typedef tagLISTPM pml;
	// -------------------------------------------------------------------
	// Process structure (Sergy/16.05.2002)
	// -------------------------------------------------------------------
	struct tagLISTPROC
		{
		int  m_nID;			// process ID
		int  m_nResources;	// number of resources, belonging to this process
		int* m_pRsrcIDs;	// Resources list (list with resource ID)
		char m_sName[16];	// name of Process
		};
	typedef tagLISTPROC prl;
	// -------------------------------------------------------------------
	// Criteria struct	(Sergy/20.05.01)
	// -------------------------------------------------------------------
	struct tagCRIT
		{
		BOOL m_bCorrect;			// flag: "element is correct"
		BOOL m_bProcessed;			// flag: "element is processed"
		BOOL m_bHaveToPerform;		// flag: "element has to be performed"
		int m_nTotalTrans;			// total transitions in criteria array
		int m_nNumToRun;			// number to run		
		CPetriElem* m_pelem;		// ptr to transition		
		};
	typedef tagCRIT crit;
// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------
public:				
	BOOL CheckNetIDs();
	BOOL Restart();				// Prepares all internal data for network restarting
	BOOL Shutdown();			// Shutdowns inited net
	int GetNumberOfCycles();	// returns number of cycles have been performed up to now
	int GetNumberOfIters();		// returns number of iterations have been performed up to now
	BOOL Run(int nFirstIterNum, int nMaxIter, int nCycles);	// Automatically runs Petri Net
	BOOL ConnectNet();			// connects all the net
	BOOL NewNetFromMPN(CFileMPN::tagMPNMEM *pMPN);	// inits PN from MPN structure
	BOOL SetInitPMs(int* pPMs);			// sets PMs and tasks list
	BOOL SetInitConnections(int *pConnections);
	BOOL SetInitProbab(int *pProbab);	// sets Probability for the first time
	BOOL SetInitTTF(int *pTTFs);		// sets TTF for the first time
	BOOL SetInitMarking(int* pMarking);	// sets marking for the first time
	CPetriNet::tagLOG* GetProtocol();	// returns ptr to the history structure
	CPetriNet::tagRSRC* GetResources();	// Returns ptr to the resources structure
	CPetriNet::tagSTATISTICS* GetStatistics();	// returns ptr to the statistics structure
	int GetLastIterNum();				// Returns number of the last iteration
	BOOL SetName(int n, int type, char* name);
	BOOL New(int nPlaces, int nTrans, int nConns, int nPMs, int nProcs);// creates new Network
	BOOL Init();						// first init
	CPetriNet();						// constructor
	virtual ~CPetriNet();				// destructor
// -----------------------------------------------------------------------
private:	
	BOOL ResetRsrcs();			// Deletes resources
	BOOL InitResources();		// Inits resources
	int GetMinNumOfCycles();
	int GetMemberIdxByID(int nID);	// Returns idx of any member element which has given ID	
	BOOL HPAnalyseCyclingProcs(int nNum);
	BOOL UpdateStatistics();
	BOOL ResetStatistics();		// Clears statistics
	BOOL ResetHistory();		// Clears history
	BOOL InitStatistics();	// Inits statistics
	BOOL InitHistory();		// Inits history protocol for the first time
	int GetLastMarkingType();
	protrec GetCurrentState();
	// -------------------------------------------------------------------
	// Process functions
	// -------------------------------------------------------------------
	BOOL PRIsInStagnation(int nRecNum, int nProcID);	// Returns 'true', if given process at given history record is in stagnation
	BOOL PRCanPMSolveProcess(int nPMID, int nProcID);	// returns 'true' if given PM can solve given process
	BOOL PRIsRsrcBelongToProc(int nRsrcID, int nProcID);	// returns 'true' if given resource belongs to given process
	// -------------------------------------------------------------------
	// History statistics
	// -------------------------------------------------------------------	
	BOOL HSAnaliseHistory();	// gets some statistics from history protocol
	int HSGetNumOfDubOverMarkings(int nNum, BOOL bCalcOver);	// calculates number of DUB or OVER markings in history
	int HSGetNumberOfStagnations();	// returns number of stagnations from history
	int HSGetNumberOfParallPMs(BOOL bFindMax);	// returns max or min number of p/w PMs
	// -------------------------------------------------------------------
	// History protocol functions
	// -------------------------------------------------------------------	
	int HPGetProcDubOverRecNum(int nProc, int nRecNum, BOOL bFindOver);	
	BOOL HPAnalyseCycling(int nNum);	// Calculates number of cycles have been performed
	int HPGetDubOverRecNum(int nRecNum, BOOL bFindOver);// Returns number of state with DUBBING marking for given record number
	BOOL HPAnalyseMarking(int nNum);// Analises marking with given number in the history protocol
	BOOL HPAnalyseMarkingProcs(int nNum); // Analises marking with given number for all processes
	// -------------------------------------------------------------------
	// Load Algorithm
	// -------------------------------------------------------------------	
	int LAGetNumberOfTasksWithRes(CPetriElem** pTasksList, int nSize);	// Returns number of tasks which can be solved on free PM
	CPetriElem**LAGetTasksWithResources(CPetriElem** pTasksList, int nSize);
	CPetriElem**LAGetEnabledTasks();	// Returns ptr to array with enabled transitions or NULL
	int LAGetNumberOfEnabledTasks();	// Returns number of all enabled transitions (tasks)
	pml** LAGetPMsByTaskID(int nTaskID);	// Returns ptr to the ptr list of PMs, which have task with given ID in their tasks list
	int LAGetPMsNumByTaskID(int nTaskID);	// Returns number of PMs, which are able to process a task with given number
	// -------------------------------------------------------------------
	// Run Algorithm
	// -------------------------------------------------------------------
	BOOL RAIteratePMs();	// 3. Iterate PMs
	BOOL RALoadPMs();		// 2. Load PMs with new tasks
	BOOL RAFirePMs();		// 1. PMs fire all ready tasks
	BOOL Connect(int nIDa, int nIDb);
	BOOL Iterate();	// Iterates one iteration of RA (RunAlgorithm)
	CPetriElem* GetElemByID(int nID);// returns Elems ptr with given ID
	BOOL IsTerminated();			// Checks termination
	BOOL IsInited();	
	BOOL AddStateToHistory(int nStep);// adds one history record to history protocol
	int CriteriaGetTask(crit* pCrit, int nTask);
	int CriteriaCompareElem(crit pEl1, crit pEl2, int nCrit);
	BOOL CriteriaSort(crit* pCrit, int nAvailPU, int nMaxPU, int nCrit);
	tagCRIT* CriteriaCreate(int nMaxPU, int nCrit);
// -----------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------
	// -------------------------------------------------------------------
	// Process data
	// -------------------------------------------------------------------
	BOOL	m_bInit;				// Init flag
	BOOL	m_bTerm;				// net working is TERMINATED
	// -------------------------------------------------------------------
	int	m_nCurIterNum;				// current iteration number
	int m_nMaxIter;					// number of interations to perform (not MAXITERNUM)
	// -------------------------------------------------------------------
	int m_nMaxCycles;				// number of cycles to perform (or =0)	
	// -------------------------------------------------------------------
	int m_nPlaces;					// Quantity of places
	int m_nTransitions;				// Quantity of transitions
	int m_nConnections;				// Quantity of connections
	int m_nPMs;						// Number of PMs (03.05.2002)
	int m_nProcs;					// Number of processes (16.05.2002)
	// -------------------------------------------------------------------
	// Net's elements
	// -------------------------------------------------------------------
	CPetriElem* m_pPlaces;			// Elements: places
	CPetriElem* m_pTrans;			// Elements: transitions
	conn*		m_pConns;			// Connections array
	pml*		m_pPMs;				// PMs and tasks list
	prl*		m_pProcs;			// Processes list (16.05.2002)
	// -------------------------------------------------------------------
	// History & Statistics arrays
	// -------------------------------------------------------------------	
	protlist m_tHist;				// history protocol (05.05.2002)	
	logstat m_tStat;				// Statistics (18.05.2002)
	logrsrc m_tRsrcs;				// Resources (28.05.2002)
// -----------------------------------------------------------------------
};
// =======================================================================

#endif // !defined(AFX_PETRINET_H__B0C216B6_2BF4_11D5_8231_000000000000__INCLUDED_)
// =======================================================================