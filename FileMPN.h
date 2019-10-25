// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Support for TFN file format.
// Used library: PetriNet
// -----------------------------------------------------------------------
// File: MPNFile.h
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 30/ 04.2002
// Last edit: 01/ 05.2002
//     Build: 1
// =======================================================================
// MPNFile.h: interface for the CFileMPN class.
// =======================================================================
#if !defined(AFX_MPNFILE_H__A953D395_5C56_11D6_83DA_00C0DF109891__INCLUDED_)
#define AFX_MPNFILE_H__A953D395_5C56_11D6_83DA_00C0DF109891__INCLUDED_
// =======================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// =======================================================================
// Features define (editable)
// =======================================================================
// =======================================================================
// !!! DO NOT EDIT !!! CONSTANTS for MPN file structure !!! 
// (Sergy/30.04.2002)
// =======================================================================
#define MPN_ERROR		(-1)	// GLOBAL ERROR
// -----------------------------------------------------------------------
// MPN Files IDs
// -----------------------------------------------------------------------
#define MPN_INITOK		"MPN02d"	// Valid ID tag
#define MPN_INITOK2		"MPNT02d"	// Valid ID tag for text files 
#define MPN_INITOLDOK01	"TPN01e"	// Valid Old ID tag (Sergy/20.05.01)
// -----------------------------------------------------------------------
// MPN Files versions
// -----------------------------------------------------------------------
#define MPN_FVER01T		(0)			// ID = "MPN01e"
#define MPN_FVER02		(1)			// ID = "MPN02d"
#define MPN_FVER02T		(2)			// ID = "MPNT02d"
// -----------------------------------------------------------------------
#define MPN_UNLITER		(-1)	// DEFAULT: Unlimited iterations,
//								//          iterate Petri net until it 
//								//			will be stopped by itself
// -----------------------------------------------------------------------
// Types of connections (30.04.2002)
// -----------------------------------------------------------------------
#define MPN_CNNORM			(0)	// DEFAULT: normal connection
#define MPN_CNINGIB			(1)	// ingibitor connection
#define MPN_CNPLTR			(0) // Direction of connection: PLACE->TRANS
#define MPN_CNTRPL			(1) // Direction of connection: TRANS->PLACE
// -----------------------------------------------------------------------
// Maximal number of elements can be loaded from MPN file
// -----------------------------------------------------------------------
#define MPN_MAXELEMS		(512)
// -----------------------------------------------------------------------
// Numerical types of MPN elements (01.05.2002)
// -----------------------------------------------------------------------
#define MPN_ENONE		(0)		// Error
#define MPN_EPLACE		(1)		// Place
#define MPN_ETRANS		(2)		// Transition
#define MPN_ECONN		(3)		// Connection
#define MPN_EPM			(4)		// ProcessingModule
#define MPN_EPROC		(5)		// Process (16.05.2002)
// -----------------------------------------------------------------------
// Textual types of MPN elements (01.05.2002)
// -----------------------------------------------------------------------
#define MPN_ETCOMMENT	";"				// comment (16.05.2002)
#define MPN_ETPLACE		"PLACE"			// Place
#define MPN_ETTRANS		"TRANSITION"	// Transition
#define MPN_ETCONN		"CONNECTION"	// Connection
#define MPN_ETPM		"PM"			// ProcessingModule
#define MPN_ETPROC		"PROCESS"		// Process (16.05.2002)
// =======================================================================
// MPNFile.h: interface for the CFileMPN class.
// =======================================================================
class CFileMPN
{
// -----------------------------------------------------------------------
// Structures
// -----------------------------------------------------------------------
public:
	// -------------------------------------------------------------------
	// !!! FILE STRUCTURES !!!
	// -------------------------------------------------------------------
	// Header of MPN file // Sergy/30.04.2002
	// -------------------------------------------------------------------
	struct tagMPNFILEHDR			// Header of MPN file 
		{
		char m_sinitMPN[6];			// for version check 
									// (should be "MPN02e")
		char m_sComment[64];		// comment for Petri net
		int m_nPNplaces;			// number of places in the Petri net
		int m_nPNtransitions;		// number of transitions
		int m_nPNconnections;		// number of connections
		// ---------------------------------------------------------------
		int m_nPMs;					// number of non-faulting processors
		int m_nProcs;				// number of processes
		int m_nCriteria;			// criteria for branch choosing
		int m_nIter;				// number of iterations to perform
		int m_nItercur;				// current number of iteration				
		// ---------------------------------------------------------------
		// Structure of the rest of file:
		// [places list]              - size = number of places
		// [transitions list]		  - size = number of transitions
		// [connections list]		  - size = number of connections	
		// [processors list]		  - size = number of processors
		// [basic task allocation list] - size = depends ...
		// [processes list]				- size = depends ...
		// ---------------------------------------------------------------
		};
	typedef tagMPNFILEHDR header;
	// -------------------------------------------------------------------
	// Structure of PLACE record in the file
	// -------------------------------------------------------------------
	struct tagMPNPLACE
		{
		int  m_nID;				// ID of place
		int  m_nTokens;			// tokens in the place
		char m_sName[16];		// name
		char m_sComment[64];	// comment
		};
	typedef tagMPNPLACE mpnplace;
	// -------------------------------------------------------------------
	// Structure of TRANSITION record in the file
	// -------------------------------------------------------------------
	struct tagMPNTRANS
		{
		int  m_nID;				// ID of transition
		int	 m_nTTFmax;			// max "time to fire"
		int  m_nProbab;			// probability of firing [0..100]
								//  -not valid if transition is already
								//  -loaded
		char m_sName[16];		// name
		char m_sComment[64];	// comment
		};
	typedef tagMPNTRANS mpntrans;
	// -------------------------------------------------------------------
	// Structure of CONNECTION record in the file
	// -------------------------------------------------------------------
	struct tagMPNCONN
		{
		int  m_nID;				// ID of connection
		int  m_nType;			// type of connection (normal or 
								//  -inigibitor. Should be = MPN_CNNORM)
		int  m_nMult;			// multiple of connections (not supported 
								//  -yet!!! Should be = 1)
		int m_nDirType;			// Direction of connection:
								//  - PLACE->TRANS or TRANS->PLACE
								//    (MPN_CNPLTR) (MPN_CNTRPL)
		int m_nCIDa;			// SRC element ID (PLACE or TRANS)
		int m_nCIDb;			// DEST element ID (PLACE or TRANS)
		};
	typedef tagMPNCONN mpnconn;
	// -------------------------------------------------------------------
	// Structure of ProcessingModulesLIST record in the file
	// -------------------------------------------------------------------
	struct tagMPNPMLIST
		{
		int  m_nID;				// ID of Processing Module (PM)
		BOOL m_bStatusOk;		// processor can function flag
		BOOL m_bBusy;			// processor is busy
		int  m_nCurTaskID;		// ID of task is being solved
		int  m_nTTFcur;			// current TTF of task
		int  m_nTasks;			// number of tasks are performed on this
								//  -processing module
		char m_sName[16];		// name
		};
	typedef tagMPNPMLIST mpnpml;
	// -------------------------------------------------------------------
	// !!! MEMORY STRUCTURES !!!
	// -------------------------------------------------------------------
	// Structure of ProcessingModules and Tasks List in memory
	// -------------------------------------------------------------------
	struct tagMPNPMMEM
		{
		int  m_nID;				// ID of Processing Module (PM)
		BOOL m_bStatusOk;		// processor can function flag
		BOOL m_bBusy;			// processor is busy
		int  m_nCurTaskID;		// ID of task is being solved
		int  m_nTTFcur;			// current TTF of task
		int  m_nTasks;			// number of tasks are performed on this
								//  -processing module
		int* m_pTaskIDs;		// array of tasks
		char m_sName[16];		// name
		};
	typedef tagMPNPMMEM mpnpmmem;
	// -------------------------------------------------------------------
	// Structure of Process in memory (16.05.2002)
	// -------------------------------------------------------------------
	struct tagMPNPROCMEM
		{
		int  m_nID;				// ID of Process
		int  m_nResources;		// number of resouces, belonging to the process
		int* m_pRsrcIDs;		// array of resorces' IDs
		char m_sName[16];		// name of process
		};
	typedef tagMPNPROCMEM mpnprmem;
	// -------------------------------------------------------------------
	// Structure of MPN file in memory // Sergy/01.05.2002
	// -------------------------------------------------------------------
	struct tagMPNMEM
		{
		// ---------------------------------------------------------------
		BOOL m_bCorrect;			// flag "structure is filled correct"
		// ---------------------------------------------------------------
		int m_nPNplaces;			// number of places in the Petri net
		int m_nPNtransitions;		// number of transitions
		int m_nPNconnections;		// number of connections
		// ---------------------------------------------------------------
		int m_nPMs;					// number of non-faulting processors
		int m_nProcs;				// number of processes (may be =0)
		// ---------------------------------------------------------------
		int m_nCriteria;			// criteria for branch choosing
		int m_nIter;				// number of iterations to perform
		int m_nItercur;				// current number of iteration		
		// ---------------------------------------------------------------
		mpnplace* m_pPlaces;		// array of places
		mpntrans* m_pTrans;			// array of transitions
		mpnconn*  m_pConns;			// array of connections
		mpnpmmem* m_pPMs;			// array of PMs and tasks
		mpnprmem* m_pProcs;			// array of processes (16.05.2002)
		};
	typedef tagMPNMEM mpnmem;	
// -----------------------------------------------------------------------
private:
// -----------------------------------------------------------------------
	// -------------------------------------------------------------------
	// special structure for convertion of MPN element form text-file to 
	// memory representation // Sergy/01.05.2002
	// -------------------------------------------------------------------
	struct tagMPNELEM				// MPN element
		{
		BOOL m_bCorrect;			// flag: "elements is correct"
		int m_nElemType;			// integer type of element		
		// ---------------------------------------------------------------
		int  m_nID;				// common parametr: ID of ...
		char m_sName[16];		// common parametr: name
		// ---------------------------------------------------------------
		// Parametrs for PLACE
		// ---------------------------------------------------------------
		int  m_nTokens;			// see 'tagMPNPLACE'
		// ---------------------------------------------------------------
		// Parametrs for TRANSITION
		// ---------------------------------------------------------------
		int	 m_nTTFmax;			// see 'tagMPNTRANS'
		int  m_nProbab;			// ...
		// ---------------------------------------------------------------
		// Parametrs for CONNECTION
		// ---------------------------------------------------------------
		int  m_nType;			// see 'tagMPNCONN'
		int  m_nMult;			// ...
		char m_sElemA[16];		// SRC element name		
		char m_sElemB[16];		// DEST element name
		// ---------------------------------------------------------------
		// Parametrs for PM
		// ---------------------------------------------------------------
		BOOL m_bBusy;			// see 'tagMPNPMLIST'
		char m_sCurTaskName[16];// ...
		int	 m_nTTFcur;			// ...	
		int  m_nTasks;			// see 'tagMPNPMLIST'
		char **m_sTaskNames;	// ptr to tasks' names
		// ---------------------------------------------------------------
		// Parametrs for PROCESS (16.05.2002)
		// ---------------------------------------------------------------
		int  m_nResources;		// number of resouces, belonging to the process
		char **m_sRsrcNames;	// ptr to tasks' names		
		};
	typedef tagMPNELEM mpnelem;			
// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------
public:
	tagMPNMEM* GetPtrMPN();
// -----------------------------------------------------------------------
	BOOL LoadFileMPN(char* sFileName);	// loads MPN binary or text file
	CFileMPN();
	virtual ~CFileMPN();	
// -----------------------------------------------------------------------
private:
// -----------------------------------------------------------------------		
	BOOL Shutdown();	// Destroys memory MPN structure if it exists
	BOOL LoadFileMPNtxt(char* sFileName, int nVersion);
	BOOL LoadFileMPNbin(char* sFileName);
	BOOL MPNCalculateElems(CFileMPN::tagMPNELEM* pAllElems, int nSize); // Loads in MPNMEM structure number of PLs, TRs, CNs, PMs
	CFileMPN::tagMPNELEM MPNConvertStringToElem(char* sStr);	
	CFileMPN::tagMPNELEM MPNConvertStringToElemOld(char *sStr);
	int		MPNGetElemIDByName(char *sName, CFileMPN::tagMPNELEM *pAllElems, int nSize);
	BOOL	MPNLoadTMemFromTElem(mpnelem* pAllElems, int nSize);	
// -----------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------	
	mpnmem m_tmpn;			// MPN file in memory
// -----------------------------------------------------------------------
};
// =======================================================================
#endif // !defined(AFX_MPNFILE_H__A953D395_5C56_11D6_83DA_00C0DF109891__INCLUDED_)
// =======================================================================