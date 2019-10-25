// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Fault Tolerant process mapping generator class
// Used library: CPetriNet, CPetriElem, CTPNFile
// -----------------------------------------------------------------------
// File: MapGen.h
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 22/ 05.2002
// Last edit: 22/ 05.2002
//     Build: 1
// =======================================================================
// MapGen.h: interface for the CMapGen class.
// =======================================================================
#if !defined(AFX_MAPGEN_H__4E3B2393_6D79_11D6_8409_00C0DF109891__INCLUDED_)
#define AFX_MAPGEN_H__4E3B2393_6D79_11D6_8409_00C0DF109891__INCLUDED_
// =======================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// =======================================================================
#include "FileMPN.h"			// Sergy/22.05.2002
// -----------------------------------------------------------------------
// =======================================================================
#define MG_ERROR		(-1)	// Global error
#define MG_STATCYCLES	(3)		// Number of cycles to get statistics
#define MG_STATITER		(75)	// Maximal number of iterations to get statistics
// =======================================================================
class CMapGen
{
	// --------------------------------------------------------------------
public:
	// --------------------------------------------------------------------
	// Public structures !!!
	// --------------------------------------------------------------------
	// History structure element (mapping ID) (27.05.2002)
	// --------------------------------------------------------------------
	struct tagMGMAPID
		{
		int m_nMagNum;		// magic number
		int m_nPMs;			// number of PMs in the current mapping
		int m_nTasks;		// number of tasks in the current mapping
		};
	typedef tagMGMAPID mgmapid;
	// --------------------------------------------------------------------
	// Mapping (24.05.2002)
	// --------------------------------------------------------------------
	struct tagMGMAPPING
		{
		BOOL m_bInited;		// "inited" flag
		int m_nPMs;			// number of PMs in this mapping
		int m_nProcs;		// number of Procs in this mapping
		mgmapid m_tID;		// ID of mapping
		CFileMPN::tagMPNPMMEM* m_pPMs;	// PMs
		};
	typedef tagMGMAPPING mapping;
	// --------------------------------------------------------------------
	// Absolute mappings (27.05.2002)
	// --------------------------------------------------------------------
	struct tagMGABSMAPP
		{
		int m_nMappings;		// number of generated mappings
		double m_dProbSurv;		// survive probability
		double m_dProbFault;	// fault probability
		int	m_nMaxPMs;			// max PM's in best conditions
		int m_nFaultPMs;		// max number of fault PMs
		int	m_nTau[2];			// tau
		mapping* m_pMapps;		// ptr to mappings
		};
	typedef tagMGABSMAPP absmapp;
	// --------------------------------------------------------------------
private:
	// --------------------------------------------------------------------
	// Processes (cycles) times (23.05.2002)
	// --------------------------------------------------------------------	
	struct tagMGPROCTIME
		{
		int m_nID;			// cycle ID
		double m_dCycleTime;// default time
		};
	typedef tagMGPROCTIME proctime;
	// --------------------------------------------------------------------	
	// Statistics structure (26.05.2002)
	// --------------------------------------------------------------------	
	struct tagMGSTAT
		{
		int m_nMapCnt;		// counter of found mappings
		int m_nMapCntOpt;	// counter of optimal mappings
		int m_nTotalImits;	// total number of imitations
		int m_nImitIters;	// number of imitated iterations
		int m_nClosedBranches;	// number of closed branches
		int m_nOptMapNum;		// number of the last optimal mapping
		int m_nOptMapBranch;	// number of the last optimal mapping
		int m_nOptMapImitNum;	// number of imit. of the last mapping
		};
	typedef tagMGSTAT mgstat;
	// --------------------------------------------------------------------
	// Branches & Bounds Tree process (branch) (23.05.2002)
	// --------------------------------------------------------------------
	struct tagBBPM;
	struct tagBBPROC
		{
		BOOL m_bClosed;			// flag "Branch is closed"
		int m_nProcID;			// Process's ID
		int m_nChilds;			// number of childs PMs (nodes)
		double m_dTimeMax;		// maximal cycle time
		double m_dTimeMin;		// maximal cycle time		
		tagBBPM* m_pParent;		// ptr to ancestor element
		tagBBPM* m_pNodes;		// ptrs to offspring elements
		};
	typedef tagBBPROC bbproc;
	// --------------------------------------------------------------------
	// Branches & Bounds Tree PM (node) (23.05.2002)
	// --------------------------------------------------------------------
	struct tagBBPM
		{
		BOOL m_bClosed;			// flag "Node is closed"
		int m_nPMID;			// PM's ID		
		int m_nChilds;			// number of childs PROCs (branches)
		tagBBPROC* m_pParent;	// ptr to parent element
		tagBBPROC* m_pBranches;	// ptrs to offspring elements
		};
	typedef tagBBPM bbpm;
	// --------------------------------------------------------------------
public:	
	CPetriNet::tagRSRC* GetResources();	// Returns ptr to CPetriNet::tagRSRC structure
	absmapp* GetMappings();		// Returns ptr to absolute mapping structure
	char* GetErrMsg();			// Returns error msg
	BOOL Generate(double dSurvProb, double dFaultProb, int nTau0, int nTau1);	// Generates mapping
	BOOL SetDataFromMPN(CFileMPN::tagMPNMEM *pMPN);	// Loads class with data
	BOOL Shutdown();		// Shutdowns class
	BOOL Init();			// inits class
	CMapGen();				// constructor
	virtual ~CMapGen();
// -----------------------------------------------------------------------
private:	
	// -------------------------------------------------------------------
	// Debug functions
	// -------------------------------------------------------------------
	char* MGDEB01GetBranchMapping(bbproc* pBranch, int nPMs, int* pPMIDs);
	char* MGDEB01GetBranchPath(bbproc* pBranch);// get text path of the current branch
	int MGDEB01GetTypeOfTime(proctime* pProcTimes, int nSize, int nTau);	// returns type of process time limit
	BOOL MGDEB01PrintConstrains(int nPMs, int nMaxFPMs);// print initial constrains
	BOOL MGDEB01PrintStat();					// print some statistics at the screen
	// -------------------------------------------------------------------
	BOOL ResetResources();		// reset resources structure
	BOOL InitResources();		// inits resources structure
	BOOL CopyPM(CFileMPN::tagMPNPMMEM* pDestPM, CFileMPN::tagMPNPMMEM* pSrcPM);	// makes copy of given PM
	BOOL IsMappingChecked(mapping tMapp);		// returns 'true' if given mapping is already checked
	BOOL Restart();
	mgmapid GetMapIDByMapping(mapping tMapp);	// returns mapping ID by given mapping
	BOOL AddToHistory(mgmapid tMapID);			// adds given mapping's ID to history		
	BOOL IsRsrcATask(int nRsrcID);				// returns 'true' if resource with given ID is a task	
	BOOL DeleteMapping(mapping* pMapp);			// deletes given mapping	
	int GetNumOfTasksByProcID(int nProcID);		// returns number of tasks of the process with given ID
	CFileMPN::tagMPNPROCMEM* GetProcByID(int nProcID);
	double GetASLDOfTime(proctime* pProcTimes);	// returns average square-law deflection of given times array
	int* GetFreeIDs(int nSize, int nVirtIDs, int* pVirtIDs);	// generates array with given number of free IDs
	BOOL DeletePMList(CFileMPN::tagMPNPMMEM* pPMs, int nPMs);	// deletes list with PMs
	mapping GetTestAllocation();	// returns first pre-allocation of processes
	// -------------------------------------------------------------------
	// Global Optimisation algorithm
	// -------------------------------------------------------------------
	proctime* GO01GlobalCheckProcesses();			// calculates maximal number of PMs in the best conditions
	int  GO02GetMaxNumOfFaultPMs(int nTotalPMs);	// returns maximal number of fault PMs according to given probability
	BOOL GO025GetMapping(int nPMs, int* pPMIDs);	// generates mapping for given number of PMs
	bbproc* GO03GetRootElem(int* pPMIDs, int nPMs);	// returns ptr to generated root element
	int* GO05GetIDsOfUsablePMs(bbproc* pBranch, int nPMs, int* pPMIDs);	// returns array of IDs of PMs, which can be used at current step
	int  GO05GetNumOfUsablePMs(bbproc* pBranch, int nPMs, int* pPMIDs);	// returns number of PMs, which can be used at current step
	int* GO06GetIDsOfUnallocProcsForNode(bbpm* pNode);	// returns array of IDs of unallocated processes for given node
	int  GO06GetNumOfUnallocProcsForNode(bbpm* pNode);	// returns number of unallocated processes for given node
	proctime* GO08GetMinCycleTime(bbproc* pBranch, int nPMs, int* pPMIDs);	// returns lower cycle time bound for given branch
	BOOL GO09CloseExceedBranches(bbpm* pNode, int nPMs, int* pPMIDs);		// Closes branches at given node if time of cycle is exceeded
	BOOL GO12UpdateMapRef(CMapGen::tagMGMAPPING tMapp, double dProcTimes);	// Updates referenced mapping with given mapping
	BOOL GO13IsMappingTimeOptimal(double dProcTimes);	// returns 'true' if given mapping time is optimal
	BOOL GO14CheckFinalMapping(bbproc* pBranch, int nPMs, int* pPMIDs);
	BOOL GO16AcceptMapRef();						// accepts referenced mapping to use
	// -------------------------------------------------------------------
	// Additional functions of 'Global Optimisation Algorithm'
	// -------------------------------------------------------------------
	BOOL GOAnalyseBranch(bbproc* pBranch, int nPMs, int* pPMIDs);	// analyses given branch
	BOOL GOAnalyseNode(bbpm* pNode, int nPMs, int* pPMIDs);			// analyses given node
	BOOL GOCheckMinTimes(proctime* pPrTimes, int nSize, int nTau);	// returns 'true' if no time exceeding in given times array
	BOOL GOCloseBranch(bbproc* pBranch);							// closes given branch
	BOOL GOCloseNode(bbpm* pNode);									// closes given node
	int* GOGetIDsOfAllocTasksByPMID(bbproc *pBranch, int nPMID);	// returns array of IDs of tasks are already allocated on given PM
	int* GOGetIDsOfUnaProcsForBranch(bbproc* pBranch);	// returns array of IDs of unallocated processes for given branch
	mapping GOGetMappingForBranch(bbproc* pBranch, int nPMs, int* pPMIDs);
	int GOGetNumOfAllocProcsByPMID(bbproc* pBranch, int nPMID);	// returns number of processes are already allocated on given PM
	int GOGetNumOfAllocTasksByPMID(bbproc* pBranch, int nPMID);	// returns number of tasks are already allocated on given PM
	int GOGetNumOfPMUsing(bbproc* pBranch, int nPMID);	// returns number of times which given PM has been used
	int GOGetNumOfUnaProcsForBranch(bbproc* pBranch);	// returns number of unallocated processes for given branch
	int GOGetNumOfUsedPMFromBranch(bbproc* pBranch, int nPMs, int* pPMIDs);	// returns number of real PM which has been used in mapping
	proctime* GOGetTimeFromImitation(mapping tMapping);		// returns generated array with cycles time for given mapping
	mapping GOGetVirtMapping(bbproc* pBranch, int nPMs, int* pPMIDs, BOOL bMakeBest);	// returns best or worst virtual mapping for current branch
	BOOL GOIsPMIDUsedFromBranch(bbproc* pBranch, int nPMID);	// returns 'true' if PM with given is already used in mapping
	BOOL GOIsProcAllocAtPMFromBranch(bbproc* pBranch, int nProcID, int nPMID);	// returns 'true' if given process is already allocated at given PM
	BOOL GOIsProcAllocFromBranch(bbproc* PBranch, int nProcID);	// returns 'true' if given process is already allocated
	BOOL GOIsProcAllocFromNode(bbpm* pNode, int nProcID);		// returns 'true' if given process is already allocated
	BOOL GOStart(int nPMs, int* pPMIDs);	// starts GO algorithm
	// -------------------------------------------------------------------
	// Combinatoric functions
	// -------------------------------------------------------------------
	int MTHFact(int nN);				// returns factorial of given number
	double MTHPowDN(double dNumber, int nPow);	// returns double 'dNumber' in integer power 'nPow'
	double MTHCombs(int nL, int nN);	// returns number of combinations of nL and nN		
	double MTHSurvProb(int nK, int nN);	// surviving probability function
// -----------------------------------------------------------------------
	BOOL	m_bInited;		// Class is inited (22.05.2002)
	BOOL	m_bLoaded;		// data is loaded into class
	// -------------------------------------------------------------------
	char	m_sErrMsg[256];	// error msg (23.05.2002)
	// -------------------------------------------------------------------
	// Input data
	// -------------------------------------------------------------------	
	//int m_nTrans;			// number of transitions in the file	
	int m_nProcs;			// number of processes in the net	
	// -------------------------------------------------------------------	
	mgmapid*m_pHistory;		// history (27.05.2002)
	int		m_nHistSize;	// history size
	// -------------------------------------------------------------------
	absmapp m_tAbsMapp;		// array of optimal mappings (27.05.2002)	
	mgstat	m_tStat;		// some statistics (26.05.2002)
	mapping	m_tMapRef;		// mapping referenced
	//proctime*	m_pRefProcTimes;	// referenced processes times (25.05.2002)
	double  m_dRefProcTime;	// ASLD of referenced processes times (28.05.2002)	
	CPetriNet::tagRSRC m_tRsrcs;	// resources (28.05.2002)
	// -------------------------------------------------------------------	
	CFileMPN::tagMPNPROCMEM* m_pProcs;	// array with processes	
	CFileMPN::tagMPNMEM* m_pMPN;		// loaded MPN file
// -----------------------------------------------------------------------
};
// =======================================================================
#endif // !defined(AFX_MAPGEN_H__4E3B2393_6D79_11D6_8409_00C0DF109891__INCLUDED_)
// =======================================================================