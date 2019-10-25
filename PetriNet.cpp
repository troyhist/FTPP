// =======================================================================
// Petri's Nets Processor ver. 1.0 (CPP only)
// Copyright (c) 1994-2001 Kolevatov Sergy
// -----------------------------------------------------------------------
// Petri's Nets main processor
// Used library: <none>
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
// PetriNet.cpp: implementation of the CPetriNet class.
// =======================================================================
#include "stdafx.h"
#include "PetriNet.h"
// =======================================================================
#define CPN_DEBUG01			// Show saving status
// =======================================================================
// Error messages
// =======================================================================
char* PN_ERRMES[]={
	"No error",							// 0
	"Function not ready yet (#Debug#)",	// 1
	"File not found",					// 2
	"Invalid file ID",					// 3
	"Invalid tag (p-%s, t-%s, c-%s)",	// 4
	"Couldn't open file",				// 5
	"File loading error",				// 6
	"Couldn't connect elements "		// 7
	};
// -----------------------------------------------------------------------
#define PN_NOERROR	(0)
#define PN_DEBUG	(1)
#define PN_FNOTFND	(2)
#define PN_INVFID	(3)
#define PN_INVTAG	(4)
// =======================================================================
// Construction/Destruction
// =======================================================================
CPetriNet::CPetriNet()
{
// -SERGY-----------------------------------------------------------------
// Description: The Constructor
//   Parametrs: <none>
//      Return: <none>
// -----------------------------------------------------------------------
//			MD: 08.04.01 --> 08.04.01
//			LE: 10.05.2002
// -----------------------------------------------------------------------
m_bInit=false;
// -----------------------------------------------------------------------
}
// =======================================================================
CPetriNet::~CPetriNet()
{
// -SERGY-----------------------------------------------------------------
// Description: The Destructor
//   Parametrs: <none>
//      Return: <none>
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (IsInited()) Shutdown();
}
// =======================================================================
BOOL CPetriNet::ResetHistory()
{
// -SERGY-----------------------------------------------------------------
// Description: Clears all the history
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 10.05.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
// -----------------------------------------------------------------------
int i, k;
// -----------------------------------------------------------------------
if (!m_tHist.m_nSize) return(false);
// -----------------------------------------------------------------------
for (i=0; i<m_tHist.m_nSize; i++)	
	for (k=0; k<3; k++)
		{
		delete m_tHist.m_pHistory[k][i].m_pPMST;
		delete m_tHist.m_pHistory[k][i].m_pTokens;
		delete m_tHist.m_pHistory[k][i].m_pProcs;	// (19.05.2002)
		}
// -----------------------------------------------------------------------
delete [] m_tHist.m_pHistory[0];	delete [] m_tHist.m_pHistory[1];
delete [] m_tHist.m_pHistory[2];
// -----------------------------------------------------------------------
m_tHist.m_nSize=0;	return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::ResetRsrcs()
{
// -SERGY-----------------------------------------------------------------
// Description: Deletes resources
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 08.06.2002
// -----------------------------------------------------------------------
if (!m_tRsrcs.m_bInited) return(false);
// -----------------------------------------------------------------------
if (m_tRsrcs.m_nPlaces)	delete [] m_tRsrcs.m_pLPL;
if (m_tRsrcs.m_nTrans)	delete [] m_tRsrcs.m_pLTR;
if (m_tRsrcs.m_nPMs)	delete [] m_tRsrcs.m_pLPM;
if (m_tRsrcs.m_nProcs)	delete [] m_tRsrcs.m_pLPROC;	// 08.06.2002
// -----------------------------------------------------------------------
m_tRsrcs.m_nPlaces=m_tRsrcs.m_nTrans=m_tRsrcs.m_nConns=m_tRsrcs.m_nPMs=
m_tRsrcs.m_nProcs=0;
// -----------------------------------------------------------------------
m_tRsrcs.m_bInited=false;	return(true);
}
// =======================================================================
BOOL CPetriNet::ResetStatistics()
{
// -SERGY-----------------------------------------------------------------
// Description: Clears statistics
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 19.05.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
if (!m_tStat.m_bInit) return(false);
// -----------------------------------------------------------------------
delete [] m_tStat.m_pProcs;
m_tStat.m_bInit=false;
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::Shutdown()
{
// -SERGY-----------------------------------------------------------------
// Description: Shutdowns inited net
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 10.05.2002
//			LE: 10.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
// Elements
// -----------------------------------------------------------------------
for(i=0; i<m_nPlaces; i++) m_pPlaces[i].Reset();
for(i=0; i<m_nTransitions; i++) m_pTrans[i].Reset();
for(i=0; i<m_nPMs; i++) delete m_pPMs[i].m_pTaskIDs;
for(i=0; i<m_nProcs; i++) delete m_pProcs[i].m_pRsrcIDs;
// -----------------------------------------------------------------------
if (m_nPlaces) delete[] m_pPlaces;
if (m_nTransitions) delete[] m_pTrans;
if (m_nConnections) delete [] m_pConns;
if (m_nPMs) delete [] m_pPMs;
if (m_nProcs) delete [] m_pProcs;
// -----------------------------------------------------------------------
ResetHistory();	ResetRsrcs(); ResetStatistics(); m_bInit=false;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::Restart()
{
// -SERGY-----------------------------------------------------------------
// Description: Prepares all internal data for network restarting
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 10.05.2002
//			LE: 17.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
// -----------------------------------------------------------------------
m_nCurIterNum=0;	m_nMaxIter=0;	m_bTerm=false;
m_nMaxCycles=0;	// no cycles (16.05.2002)
// -----------------------------------------------------------------------
ResetHistory(); ResetStatistics();
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::Init()
{
// -SERGY-----------------------------------------------------------------
// Description: Resets all adjustments of Petri's Net
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 08.04.01 --> 04.05.01
//			LE: 28.05.2002
// -----------------------------------------------------------------------
if (IsInited()) Shutdown();	
// -----------------------------------------------------------------------
m_nCurIterNum=0;	m_nMaxIter=0;	// no iterations
m_nMaxCycles=0;		// no cycles (16.05.2002)
m_bTerm=false;		// not terminated
m_bInit=true;		// inited
m_tHist.m_nSize=0;	// no history
m_tRsrcs.m_bInited=false;	// no resources (28.05.2002)
m_tStat.m_bInit=false;		// no statistics (19.05.2002)
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::IsInited()
{
// -SERGY-----------------------------------------------------------------
// Description: Checks basic definition of net
//   Parametrs: <none>
//      Return: true	- inited, false - otherwise
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
return(m_bInit);
}
// =======================================================================
BOOL CPetriNet::New(int nPlaces, int nTrans, int nConns, int nPMs, 
					int nProcs)
{
// -SERGY-----------------------------------------------------------------
// Description: FUNCTION FOR AUTOMATIC NET-CONNECTION
//				Creates new Petri's Net with given number of places and
//				transitions, connections and PMs
//   Parametrs: nPlaces			- number of places
//				nTrans			- number of transition
//				nConns			- number of connections
//				nPMs			- number of PMs
//				nProcs			- number of process
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 16.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
// -----------------------------------------------------------------------
int i;							// idx
// -----------------------------------------------------------------------
if (nPlaces<=0 || nTrans<=0 || nConns<=0 || nPMs<0 || nPlaces<0)
	return(false);
// -----------------------------------------------------------------------
m_nPlaces=nPlaces;
m_nTransitions=nTrans;
m_nConnections=nConns;
m_nPMs=nPMs;
m_nProcs=nProcs;
// -----------------------------------------------------------------------
m_pPlaces=new CPetriElem[m_nPlaces];		// places
m_pTrans=new CPetriElem[m_nTransitions];	// transitions
m_pConns=new conn[m_nConnections];			// Connections array
m_pPMs=new pml[m_nPMs];						// PMs and tasks list
m_pProcs=new prl[m_nProcs];					// Processes list
// -----------------------------------------------------------------------
// Defines elements
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++) m_pPlaces[i].Define(i, PE_PLACE);
for (i=0; i<m_nTransitions; i++) m_pTrans[i].Define(i, PE_TRANSITION);
// -----------------------------------------------------------------------
m_bTerm=false; return (true);
// -----------------------------------------------------------------------
}
// =======================================================================
CPetriNet::tagLOG* CPetriNet::GetProtocol()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to the history protocol array
//   Parametrs: <none>
//      Return: ptr - success, NULL - otherwise
// -----------------------------------------------------------------------
//			MD: 06.05.2002
//			LE: 06.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(NULL);
if (!m_tHist.m_nSize) return(NULL);
return(&m_tHist);
}
// =======================================================================
CPetriNet::tagRSRC* CPetriNet::GetResources()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to the resources structure
//   Parametrs: <none>
//      Return: ptr - success, NULL - otherwise
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(NULL);
if (!m_tRsrcs.m_bInited) return(NULL);
return(&m_tRsrcs);
}
// =======================================================================
CPetriNet::tagSTATISTICS* CPetriNet::GetStatistics()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to the statistics structure
//   Parametrs: <none>
//      Return: ptr - success, NULL - otherwise
// -----------------------------------------------------------------------
//			MD: 19.05.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(NULL);
if (!m_tStat.m_bInit) return(NULL);
return(&m_tStat);
}
// =======================================================================
BOOL CPetriNet::Connect(int nIDa, int nIDb)
{
// -SERGY-----------------------------------------------------------------
// Description: Connects two elements. Elements' types should be 
//				different.
//   Parametrs: nIDa	- ID of start element
//				nIDb	- ID of end element
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
CPetriElem* pElemA=GetElemByID(nIDa);
CPetriElem* pElemB=GetElemByID(nIDb);
// -----------------------------------------------------------------------
if (!pElemA || !pElemB) return(false);
if (pElemA->GetType()==pElemB->GetType()) return(false);
BOOL res=pElemA->ConnectTo(pElemB);
// -----------------------------------------------------------------------
return(res);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::PRCanPMSolveProcess(int nPMID, int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns 'true', if given PM can solve any task from given
//				process
//				PRocess function
//   Parametrs: nPMID		- PM's ID
//				nProcID		- process ID
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
int i, j;
int nProcNum=PN_ERROR, nPMNum=PN_ERROR;
// -----------------------------------------------------------------------
// PN_MAINPROCESS - (=-1) means all Petri net
// -----------------------------------------------------------------------
if (nProcID==PN_MAINPROCESS) return(true);
//for (i=0; i<m_nPMs; i++) if (nPMID==m_pPMs[i].m_nID) { nPMNum=i; break; }
//for (i=0; i<m_nProcs; i++) if (nProcID==m_pProcs[i].m_nID) { nProcNum=i; break; }
nPMNum=GetMemberIdxByID(nPMID);
nProcNum=GetMemberIdxByID(nProcID);
if (nPMNum==PN_ERROR || nProcNum==PN_ERROR) return(false);
// -----------------------------------------------------------------------
for (i=0; i<m_pPMs[nPMNum].m_nTasks; i++)
	for (j=0; j<m_pProcs[nProcNum].m_nResources; j++)
		if (m_pPMs[nPMNum].m_pTaskIDs[i]==m_pProcs[nProcNum].m_pRsrcIDs[j])
			return(true);	
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CPetriNet::PRIsInStagnation(int nRecNum, int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns 'true', if given process at given history record is in stagnation
//				PRocess function
//   Parametrs: nProcID		- process ID
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 21.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
int i, j, nProcNum;
// -----------------------------------------------------------------------
if (nProcID==PN_MAINPROCESS)
	return(m_tHist.m_pHistory[2][nRecNum].m_bPMStagn);
if (m_tHist.m_pHistory[2][nRecNum].m_bPMStagn) return(true);
// -----------------------------------------------------------------------
nProcNum=GetMemberIdxByID(nProcID); if(nProcNum==PN_ERROR) return(false);
// -----------------------------------------------------------------------
for (i=0; i<m_tRsrcs.m_nPMs; i++)	
	if (m_tHist.m_pHistory[2][nRecNum].m_pPMST[i].m_bStatusOk && 
		m_tHist.m_pHistory[2][nRecNum].m_pPMST[i].m_bBusy)		
		for(j=0; j<m_pProcs[nProcNum].m_nResources; j++)
			if(m_tHist.m_pHistory[2][nRecNum].m_pPMST[i].m_nCurTaskID==
				m_pProcs[nProcNum].m_pRsrcIDs[j])
				return(false);
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::PRIsRsrcBelongToProc(int nRsrcID, int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: Checks if given resource bleongs to given process
//				PRrocess ...
//   Parametrs: nResrcID	- resource ID of PLACE or TRANSITION or PM
//				nProcID		- process ID
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
int i, nProcNum=PN_ERROR;
// -----------------------------------------------------------------------
// PN_MAINPROCESS - (=-1) means all Petri net
// -----------------------------------------------------------------------
if (nProcID==PN_MAINPROCESS) return(true);
nProcNum=GetMemberIdxByID(nProcID);	// (21.05.2002)
if (nProcNum==PN_ERROR) return(false);
// -----------------------------------------------------------------------
for (i=0; i<m_pProcs[nProcNum].m_nResources; i++)
	if (m_pProcs[nProcNum].m_pRsrcIDs[i]==nRsrcID) return(true);	
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CPetriNet::IsTerminated()
{
// -SERGY-----------------------------------------------------------------
// Description: Checks if no more tasks to solve and all PMs are free
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 08.04.01 --> 04.05.01, 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
int i;
for (i=0; i<m_nTransitions; i++) 
	if (m_pTrans[i].TransIsEnabled()) 
		return(false);
for (i=0; i<m_nPMs; i++)
	if(m_pPMs[i].m_bStatusOk)
	if(m_pPMs[i].m_bBusy)
			return(false);
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
CPetriNet::crit* CPetriNet::CriteriaCreate(int nMaxPU, int nCrit)
{
// -SERGY-----------------------------------------------------------------
// Description: Generates sorted criteria array with transitions
//   Parametrs: nMaxPU	- quantity of available processor units
//				nCrit	- criteria
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 20.05.01
//			LE: 20.05.01
// -----------------------------------------------------------------------
return(NULL);
/*
int i;							// idx
int j;							// idx2
// -----------------------------------------------------------------------
int nProcRun=0;					// quantity of processes in running
int nProcReady=0;				// quantity of processes which are ready to run
int nAvailPU;					// quantity of available processor units
// -----------------------------------------------------------------------
crit* pCrit=NULL;
// -----------------------------------------------------------------------
// Calculates processes in running
// -----------------------------------------------------------------------
for (i=0; i<m_nTransitions; i++)	
	if (m_pTrans[i].TransIsInProgress())
		nProcRun++;	
// -----------------------------------------------------------------------
// If quantity of processors is unlimited performs the following condition
// -----------------------------------------------------------------------
if (nMaxPU==PN_UNLPROC)
	nMaxPU=m_nTransitions;
// -----------------------------------------------------------------------
// Checks quantity of processes
// -----------------------------------------------------------------------
if (nProcRun>nMaxPU)
	{ sprintf(m_errormsg, 
	"Internal error. CPetriNet::SortByCriteria(...): nProcRun>nMaxPU");
	return(NULL); }
// -----------------------------------------------------------------------
// Calculates quantity of available processors
// -----------------------------------------------------------------------
nAvailPU=nMaxPU-nProcRun;
// -----------------------------------------------------------------------
// Collects processes which to be performed
// -----------------------------------------------------------------------
for (i=0; i<m_nTransitions; i++)
	if (m_pTrans[i].TransIsReadyToLoad())	
		if (!m_pTrans[i].TransIsInProgress())
			nProcReady++;
// -----------------------------------------------------------------------
if (!nProcReady) return(NULL);
// -----------------------------------------------------------------------
// Creates new citeria array
// -----------------------------------------------------------------------
pCrit=new crit[nProcReady];
// -----------------------------------------------------------------------
j=0;			// idx in 'pCrit' array
// -----------------------------------------------------------------------
// Loads criteria array
// -----------------------------------------------------------------------
for (i=0; i<m_nTransitions; i++)
	if (m_pTrans[i].TransIsReadyToLoad())	
		if (!m_pTrans[i].TransIsInProgress())
			{
			pCrit[j].m_bCorrect=true;
			pCrit[j].m_bProcessed=false;
			pCrit[j].m_bHaveToPerform=false;
			pCrit[j].m_nNumToRun=j;
			pCrit[j].m_pelem=&m_pTrans[i];
			pCrit[j].m_nTotalTrans=nProcReady;
			j++;
			}
// -----------------------------------------------------------------------
// Sorts by criteria
// -----------------------------------------------------------------------
CriteriaSort(pCrit, nAvailPU, nMaxPU, nCrit);
// -----------------------------------------------------------------------
return(pCrit);
//*/
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::CriteriaSort(crit *pCrit, int nAvailPU, int nMaxPU, int nCrit)
{
// -SERGY-----------------------------------------------------------------
// Description: Sorts criteria array
//   Parametrs: pCrit		- ptr to Criteria array (MUST BE NOT EMPTY)
//				nAvailPU	- quantity of available processor units
//				nMaxPU		- total quantity of processor units
//				nCrit		- criteria
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 20.05.01
//			LE: 21.05.01
// -----------------------------------------------------------------------
BOOL bDone;				// flag: "Done"
int i;					// idx
int nProcReady;			// quantity of processes which are ready to run
int nCompareRes;		// comparing result
int nMinProcOrPU;
// -----------------------------------------------------------------------
int nTask;				// number of task to run
int nTempNum;			// temporary number
int nSN1;				// sorted numbers of elements
int nSN2;				// sorted numbers of elements
// -----------------------------------------------------------------------
nProcReady=pCrit[0].m_nTotalTrans;
// -----------------------------------------------------------------------
// Renumbers elements
// -----------------------------------------------------------------------
for (i=0; i<nProcReady; i++)
	{
	pCrit[i].m_bCorrect=true;
	pCrit[i].m_bProcessed=false;
	pCrit[i].m_bHaveToPerform=false;
	pCrit[i].m_nNumToRun=i;
	}
// -----------------------------------------------------------------------
// Default bubble sorting
// -----------------------------------------------------------------------
do	{
	bDone=true;
	for (i=0; i<nProcReady-1; i++)
		{
		// ---------------------------------------------------------------
		nSN1=CriteriaGetTask(pCrit, i);
		nSN2=CriteriaGetTask(pCrit, i+1);
		nCompareRes=CriteriaCompareElem(pCrit[nSN1], pCrit[nSN2], nCrit);
		// ---------------------------------------------------------------
		if (nCompareRes==1)
			{
			// -----------------------------------------------------------
			// Elem2 is more than Elem1
			// -----------------------------------------------------------
			nTempNum=pCrit[nSN1].m_nNumToRun;
			pCrit[nSN1].m_nNumToRun=pCrit[nSN2].m_nNumToRun;
			pCrit[nSN2].m_nNumToRun=nTempNum;
			// -----------------------------------------------------------
			bDone=false;
			}
		// ---------------------------------------------------------------
		}
	} while (!bDone);
// -----------------------------------------------------------------------
// Sets 'HaveToPerform' flag
// -----------------------------------------------------------------------
nMinProcOrPU=nAvailPU;
if (nProcReady<nAvailPU) nMinProcOrPU=nProcReady;
// -----------------------------------------------------------------------
for (i=0; i<nMinProcOrPU; i++)
	{
	nTask=CriteriaGetTask(pCrit, i);
	if (nTask==PE_ERROR) 
		return(false);
	pCrit[nTask].m_bHaveToPerform=true;
	}
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
int CPetriNet::CriteriaCompareElem(crit pEl1, crit pEl2, int nCrit)
{
// -SERGY-----------------------------------------------------------------
// Description: Compares 2 elements according to given criteria
//   Parametrs: pEl1	- ptr to the first element
//				pEl2	- ptr to the second element
//				nCrit	- criteria
//      Return: -1		- Element 1 sorts before element 2.
//				0		- Element 1 and element 2 sort the same.
//				1		- Element 1 sorts after element 2. 
// -----------------------------------------------------------------------
//			MD: 21.05.01
//			LE: 21.05.01
// -----------------------------------------------------------------------
int nRes;				// comparing result
int val1;				// time to fire of 1-st elem
int val2;				// time to fire of 2-nd elem
CPetriElem* pPE1=pEl1.m_pelem;			// ptr to 1-st Petri elem
CPetriElem* pPE2=pEl2.m_pelem;			// ptr to 2-nd Petri elem
// -----------------------------------------------------------------------
nRes=0;					// elements are equal
// -----------------------------------------------------------------------
switch(nCrit)
	{
	// -------------------------------------------------------------------
	case PN_CRMINWRKTIME:		// Min working time
		val1=pPE1->TransGetTTF();
		val2=pPE2->TransGetTTF();
		if(val1<val2) nRes=-1;
		if(val2<val1) nRes=1;
		break;
	// -------------------------------------------------------------------
	case PN_CRMAXWRKTIME:		// Max working time
		val1=pPE1->TransGetTTF();
		val2=pPE2->TransGetTTF();
		if(val1>val2) nRes=-1;
		if(val2>val1) nRes=1;
		break;
	// -------------------------------------------------------------------
	case PN_CRMAXNUMCHLD:		// Max number of children
		//val1=pPE1->GetQuantityOfRaysOut();
		//val2=pPE2->GetQuantityOfRaysOut();
		val1=val2=0;
		if(val1>val2) nRes=-1;
		if(val2>val1) nRes=1;
		break;
	// -------------------------------------------------------------------
	case PN_CRMAXWAITTIME:		// Max wait time
	case PN_CRMINPASSTIME:		// Path with minimal passing time 
	case PN_CRMAXRESTTIME:		// Path with maximal rest time 
	case PN_DEFCRIT:
	default: nRes=0; break;
	};
// -----------------------------------------------------------------------
return(nRes);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::CriteriaGetTask(crit* pCrit, int nTask)
{
// -SERGY-----------------------------------------------------------------
// Description: Get task with given number
//   Parametrs: nTask	- number needed of task
//      Return: >=0 - success, PE_ERROR - otherwise
// -----------------------------------------------------------------------
//			MD: 21.05.01
//			LE: 21.05.01
// -----------------------------------------------------------------------
int i;
int nSize;
// -----------------------------------------------------------------------
if (!pCrit) return(PE_ERROR);
nSize=pCrit[0].m_nTotalTrans;
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)	
	if (pCrit[i].m_nNumToRun==nTask) return(i);	
// -----------------------------------------------------------------------
return(PE_ERROR);
// -----------------------------------------------------------------------
}
// =======================================================================
//DEL BOOL CPetriNet::IterateSubLoad(int nMaxPU, int nCrit)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Tries to load all enabled transitions according to given
//DEL //				criteria
//DEL //   Parametrs: nMaxPU	- quantity of available processor units
//DEL //				nCrit	- criteria
//DEL //      Return: true - success, false - otherwise
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 26.05.01
//DEL //			LE: 26.05.01
//DEL // -----------------------------------------------------------------------
//DEL return(false);
//DEL /*
//DEL // -----------------------------------------------------------------------
//DEL BOOL resload;					// result of transition loading
//DEL int i;							// idx
//DEL int nCritSize;					// size of criteria array
//DEL int nTask;						// number of task (priority)
//DEL // -----------------------------------------------------------------------
//DEL crit* pCrit;					// ptr to Criteria array
//DEL // -----------------------------------------------------------------------
//DEL pCrit=CriteriaCreate(nMaxPU, nCrit);
//DEL // -----------------------------------------------------------------------
//DEL // Loads transitions according to the criteria array
//DEL // -----------------------------------------------------------------------
//DEL if (!pCrit) return(true);
//DEL // -----------------------------------------------------------------------
//DEL nCritSize=pCrit[0].m_nTotalTrans;	// gets size of criteria array
//DEL // -----------------------------------------------------------------------
//DEL for (i=0; i<nCritSize; i++)	
//DEL 	{
//DEL 	// -------------------------------------------------------------------
//DEL 	nTask=this->CriteriaGetTask(pCrit, i);	
//DEL 	// -------------------------------------------------------------------
//DEL 	if(pCrit[nTask].m_bHaveToPerform)
//DEL 		if(!pCrit[nTask].m_bProcessed)
//DEL 			{
//DEL 			pCrit[nTask].m_bProcessed=true;
//DEL 			if (pCrit[nTask].m_pelem->TransIsReadyToLoad())
//DEL 				{
//DEL 				resload=pCrit[nTask].m_pelem->TransLoad();
//DEL 				if (resload==false)
//DEL 					return(false);
//DEL 				}
//DEL 			}
//DEL 	// -------------------------------------------------------------------
//DEL 	}
//DEL // -----------------------------------------------------------------------
//DEL // Deletes criteria array
//DEL // -----------------------------------------------------------------------
//DEL delete[] pCrit;
//DEL // -----------------------------------------------------------------------
//DEL return(true);
//DEL // -----------------------------------------------------------------------
//DEL //*/
//DEL }
// =======================================================================
BOOL CPetriNet::Run(int nFirstIterNum, int nMaxIter, int nMaxCycles)
{
// -SERGY-----------------------------------------------------------------
// Description: Automatically runs Petri Net
//   Parametrs: nFirstIterNum	- first iteration number
//				nMaxIter		- quantity of iteration to process 
//								  or 'PN_UNLITER' to skip
//				nMaxCycles		- number of cycles to process (or =0)
//								  (=0 means no cycles counting)
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
if (m_bTerm) return(true);
// -----------------------------------------------------------------------
BOOL bResIter=true;
BOOL bRunUntilCycleLimit=true;
BOOL bTerm=false;		// Networking is TERMINATED (10.05.2002)
int nCurCycleNum, nMaxIterNum;
// -----------------------------------------------------------------------
if (nFirstIterNum<0) nFirstIterNum=0;
if (nMaxIter==PN_UNLITER || nMaxIter<0) nMaxIter=PN_MAXITER;
// -----------------------------------------------------------------------
nMaxIterNum=nMaxIter+nFirstIterNum;
m_nCurIterNum=nFirstIterNum;
m_nMaxIter=nMaxIter;
m_nMaxCycles=nMaxCycles;	// no cycles (16.05.2002)
// -----------------------------------------------------------------------
while(!bTerm && (m_nCurIterNum<nMaxIterNum) && bRunUntilCycleLimit)
	{
	bResIter=Iterate();
	bTerm=IsTerminated();
	//if ((nMaxCycles>0 && GetLastMarkingType()!=PN_MARKNORM)) //&&
	if (nMaxCycles>0)
		{
		nCurCycleNum=GetMinNumOfCycles();	// (22.05.2002)
		if (nCurCycleNum>=m_nMaxCycles)
			bRunUntilCycleLimit=false;		// (17.05.2002)
		}
	if (bTerm && m_tHist.m_nSize)
		m_tHist.m_pHistory[1][m_tHist.m_nSize-1].
		m_nMarkType=PN_MARKTERM;	
	}
// -----------------------------------------------------------------------
m_bTerm=bTerm;
// -----------------------------------------------------------------------
HSAnaliseHistory();
// -----------------------------------------------------------------------
return(bResIter);
}
// =======================================================================
BOOL CPetriNet::Iterate()
{
// -SERGY-----------------------------------------------------------------
// Description: Iterates one iteration of RA (RunAlgorithm)
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 16.05.2002
// -----------------------------------------------------------------------
// -. Save net status (state) to history (step=0)
// 1. PMs fire all ready tasks
// -. Save net status (state) to history (step=1)
// 2. Load PMs with new tasks
// -. Save net status (state) to history (step=2)
// 3. Iterate PMs
// -----------------------------------------------------------------------
int bResIter;
// -----------------------------------------------------------------------
AddStateToHistory(0);
// -----------------------------------------------------------------------
// FIRE
// -----------------------------------------------------------------------
bResIter=RAFirePMs();				// fire
AddStateToHistory(1);
// -----------------------------------------------------------------------
// LOAD
// -----------------------------------------------------------------------
bResIter=bResIter&&RALoadPMs();		// load
AddStateToHistory(2);
// -----------------------------------------------------------------------
// ITERATE
// -----------------------------------------------------------------------
bResIter=bResIter&&RAIteratePMs();	// iterate
// -----------------------------------------------------------------------
HPAnalyseMarking(m_tHist.m_nSize-1);// Analyse the last marking/07.05.2002
HPAnalyseMarkingProcs(m_tHist.m_nSize-1);	// same for processes/20.05.2002
UpdateStatistics();					// 19.05.2002
m_nCurIterNum++;					// Sergy/26.05.01
// -----------------------------------------------------------------------
return(bResIter);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::RAFirePMs()
{
// -SERGY-----------------------------------------------------------------
// Description: PMs fire tasks (transitions), which are already 
//				solved (finished) and changes his status to free
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
int i;
int nTTFmax, nCurTaskID;
CPetriElem* pElem;
// -----------------------------------------------------------------------
for (i=0; i<m_nPMs; i++)	
	if (m_pPMs[i].m_bStatusOk)
	if (m_pPMs[i].m_bBusy)
		{
		nCurTaskID=m_pPMs[i].m_nCurTaskID;
		pElem=GetElemByID(nCurTaskID);
		nTTFmax=pElem->TransGetTTF();
		// We use '>=' cause nTTFmax may be equal to '0'
		if (m_pPMs[i].m_nTTFcur>=nTTFmax)
			{
			pElem->TransFireFromPM();
			m_pPMs[i].m_bBusy=false;
			}
		}	
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::RALoadPMs()
{
// -SERGY-----------------------------------------------------------------
// Description: Loads free PMs with tasks
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 09.05.2002
// -----------------------------------------------------------------------
int i, j, nTaskID;
int nSizeAll, nSizeRes;		// size of tasks arrays
CPetriElem** pEnabledTasks;
CPetriElem** pResTasks;
CPetriElem* pTask;			// ptr to Transition
pml** pPMs;					// PMs ptrs list
int nPMs;
// -----------------------------------------------------------------------
nSizeAll=LAGetNumberOfEnabledTasks();
if (nSizeAll==0) return(true);
if (nSizeAll==PN_ERROR) return(false);
pEnabledTasks=LAGetEnabledTasks();
// -----------------------------------------------------------------------
nSizeRes=LAGetNumberOfTasksWithRes(pEnabledTasks, nSizeAll);
if (nSizeRes==0) {delete pEnabledTasks; return(true); }
if (nSizeRes==PN_ERROR) {delete pEnabledTasks; return(false); }
pResTasks=LAGetTasksWithResources(pEnabledTasks, nSizeAll);
// -----------------------------------------------------------------------
for (i=0; i<nSizeRes; i++)
	{
	// -------------------------------------------------------------------
	pTask=pResTasks[i];
	nTaskID=pTask->GetID();
	// -------------------------------------------------------------------	
	nPMs=LAGetPMsNumByTaskID(nTaskID); if (nPMs==PN_ERROR) return(false);	
	pPMs=LAGetPMsByTaskID(nTaskID);
	// -------------------------------------------------------------------
	if (pTask->TransIsEnabled())	// task still has Tokens-resources		
		for (j=0; j<nPMs; j++)
			{
			if(pPMs[j]->m_bBusy==false)	// task still has PM-resources
				if(pTask->TransFireProbabTest())	// probability - ok
					{
					pPMs[j]->m_bBusy=true;
					pPMs[j]->m_nCurTaskID=nTaskID;
					pPMs[j]->m_nTTFcur=0;
					pTask->TransLoadIntoPM();
					break;
					}
			}
	// -------------------------------------------------------------------
	delete pPMs;
	// -------------------------------------------------------------------
	}
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::RAIteratePMs()
{
// -SERGY-----------------------------------------------------------------
// Description: PMs make one step in task solving
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
for (int i=0; i<m_nPMs; i++)	
	if(m_pPMs[i].m_bStatusOk)
	if(m_pPMs[i].m_bBusy)	
		m_pPMs[i].m_nTTFcur++;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
int CPetriNet::LAGetPMsNumByTaskID(int nTaskID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of PMs, which are able to process a task 
//				with given number
//				LA... - Load Algorithm
//   Parametrs: nTaskID	- task ID
//      Return: num (>0) - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 09.05.2002
//			LE: 09.05.2002
// -----------------------------------------------------------------------
int i, j;
int nNum=0;	// number of PMs, which are able to process a task with given 
			// number
for (i=0; i<m_nPMs; i++)
	for (j=0; j<m_pPMs[i].m_nTasks; j++)
		if	(m_pPMs[i].m_bStatusOk &&
			(m_pPMs[i].m_pTaskIDs[j]==nTaskID))
			nNum++;
// -----------------------------------------------------------------------
return(nNum?nNum:PN_ERROR);
// -----------------------------------------------------------------------
}
// =======================================================================
CPetriNet::tagLISTPM** CPetriNet::LAGetPMsByTaskID(int nTaskID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to the ptr list of PMs, which have task with 
//				given ID in their tasks list
//				LA... - Load Algorithm
//   Parametrs: nTaskID	- task ID
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 09.05.2002
// -----------------------------------------------------------------------
int i, j, nNum;
int nPMs;		// number of PMs
pml** pPMs;		// PMs ptrs list
// -----------------------------------------------------------------------
nPMs=LAGetPMsNumByTaskID(nTaskID); if (nPMs==PN_ERROR) return(NULL);
pPMs=new pml*[nPMs];
nNum=0;
// -----------------------------------------------------------------------
for (i=0; i<m_nPMs; i++)
	for (j=0; j<m_pPMs[i].m_nTasks; j++)
		if	(m_pPMs[i].m_bStatusOk &&
			(m_pPMs[i].m_pTaskIDs[j]==nTaskID))
			pPMs[nNum++]=&m_pPMs[i];
// -----------------------------------------------------------------------
return(pPMs);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::LAGetNumberOfEnabledTasks()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of all enabled transitions (tasks)
//				LA... - Load Algorithm
//   Parametrs: <none>
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
int i, nRes=0;
for (i=0; i<m_nTransitions; i++)	
	if (m_pTrans[i].TransIsEnabled()) nRes++;
// -----------------------------------------------------------------------
return(nRes);
}
// =======================================================================
CPetriElem** CPetriNet::LAGetEnabledTasks()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to array with enabled transitions or NULL
//				LA... - Load Algorithm
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
int i, j, nSize;
CPetriElem** pTasksList;
// -----------------------------------------------------------------------
nSize=LAGetNumberOfEnabledTasks();
if (nSize<=0) return(NULL);
// -----------------------------------------------------------------------
pTasksList=new CPetriElem*[nSize];
j=0;
// -----------------------------------------------------------------------
for (i=0; i<m_nTransitions; i++)	
	if (m_pTrans[i].TransIsEnabled())
		pTasksList[j++]=&m_pTrans[i];
// -----------------------------------------------------------------------
return(pTasksList);
}
// =======================================================================
int CPetriNet::LAGetNumberOfTasksWithRes(CPetriElem **pTasksList, int nSize)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of tasks which can be solved on free PM
//				LA... - Load Algorithm
//   Parametrs: pTaskList	- list of all enabled tasks
//				nSize		- size of array
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 09.05.2002
// -----------------------------------------------------------------------
int i, j, nRes=0;
int nPMs;
pml** pPMs;
for (i=0; i<nSize; i++)
	{	
	nPMs=LAGetPMsNumByTaskID(pTasksList[i]->GetID()); 
	if(nPMs==PN_ERROR) return(PN_ERROR);
	pPMs=LAGetPMsByTaskID(pTasksList[i]->GetID());
	for (j=0; j<nPMs; j++)
		if (!pPMs[j]->m_bBusy) { nRes++; break; }		
	delete pPMs;
	}
// -----------------------------------------------------------------------
return(nRes);
}
// =======================================================================
CPetriElem** CPetriNet::LAGetTasksWithResources(CPetriElem **pTasksList, int nSize)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns tasks list with tasks which can be solved on free PM
//				LA... - Load Algorithm
//   Parametrs: pTaskList	- list of all enabled tasks
//				nSize		- size of array
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
int i, j, nResSize;
int nNum;
int nPMs;
pml** pPMs;
CPetriElem** pTResList;
// -----------------------------------------------------------------------
nResSize=LAGetNumberOfTasksWithRes(pTasksList, nSize);
if (nResSize<=0) return(NULL);
// -----------------------------------------------------------------------
pTResList=new CPetriElem*[nResSize];
nNum=0;
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)
	{
	nPMs=LAGetPMsNumByTaskID(pTasksList[i]->GetID());
	pPMs=LAGetPMsByTaskID(pTasksList[i]->GetID());
	for (j=0; j<nPMs; j++)		
		if (!pPMs[j]->m_bBusy)
			{
			pTResList[nNum++]=pTasksList[i];
			break;
			}
	delete pPMs;
	}
// -----------------------------------------------------------------------
return(pTResList);
}
// =======================================================================
BOOL CPetriNet::SetName(int n, int type, char *name)
{
// -SERGY-----------------------------------------------------------------
// Description: Defines name of elements
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 11.04.01
//			LE: 11.04.01
// -----------------------------------------------------------------------
int res, maxn;
// -----------------------------------------------------------------------
if (!IsInited()) return(false);
// -----------------------------------------------------------------------
maxn=(type==PE_PLACE)?m_nPlaces:m_nTransitions;
// -----------------------------------------------------------------------
if (n<0 || n>=maxn) return(false);
// -----------------------------------------------------------------------
CPetriElem* Elem=(type==PE_PLACE)?&m_pPlaces[n]:&m_pTrans[n];
res=Elem->SetName(name); return(res);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::CheckNetIDs()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns 'true', if no dublicate IDs found
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 21.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
if (!IsInited())	return(false);
// -----------------------------------------------------------------------
BOOL bRes=true;
int i, j=0;			// 'j' should be equal to '0'
int nSize;
int *pIDs;			// array with all IDs
// -----------------------------------------------------------------------
nSize=m_nPlaces+m_nTransitions+m_nConnections+m_nPMs+m_nProcs;
pIDs=new int [nSize];
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++)	pIDs[j++]=m_pPlaces[i].GetID();	
for (i=0; i<m_nTransitions; i++)	pIDs[j++]=m_pTrans[i].GetID();
for (i=0; i<m_nConnections; i++)	pIDs[j++]=m_pConns[i].m_nID;
for (i=0; i<m_nPMs; i++)	pIDs[j++]=m_pPMs[i].m_nID;
for (i=0; i<m_nProcs; i++)	pIDs[j++]=m_pProcs[i].m_nID;
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)
	for (j=0; j<nSize; j++)
		if (pIDs[i]==pIDs[j] && i!=j) bRes=false;
// -----------------------------------------------------------------------
delete pIDs; return(bRes);
}
// =======================================================================
BOOL CPetriNet::ConnectNet()
{
// -SERGY-----------------------------------------------------------------
// Description: Connects Petri net using internal StateConns array
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !m_nConnections)	return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nConnections; i++)
	Connect(m_pConns[i].m_nCIDa, m_pConns[i].m_nCIDb);
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
CPetriElem* CPetriNet::GetElemByID(int nID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to PetriElem with given ID, or NULL
//   Parametrs: nID	- id of element
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++)
	if (nID==m_pPlaces[i].GetID())
		return(&m_pPlaces[i]);
for (i=0; i<m_nTransitions; i++)
	if (nID==m_pTrans[i].GetID())
		return(&m_pTrans[i]);
// -----------------------------------------------------------------------
return(NULL);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::GetMemberIdxByID(int nID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns idx of any member element which has given ID
//   Parametrs: nID	- id of any member element (PL, TR, CN, PM, PROC)
//      Return: idx - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 21.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++)	if (nID==m_pPlaces[i].GetID()) return(i);
for (i=0; i<m_nTransitions; i++)	if (nID==m_pTrans[i].GetID()) return(i);
for (i=0; i<m_nConnections; i++)	if (nID==m_pConns[i].m_nID) return(i);
for (i=0; i<m_nPMs; i++)	if (nID==m_pPMs[i].m_nID) return(i);
for (i=0; i<m_nProcs; i++)	if (nID==m_pProcs[i].m_nID) return(i);
// -----------------------------------------------------------------------
return(PN_ERROR);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::GetMinNumOfCycles()
{
// -SERGY-----------------------------------------------------------------
// Description: Calcultes minimal number of cycles which have been performed
//   Parametrs: <none>
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 30.05.2002
// -----------------------------------------------------------------------
int i, nCycles;
int idxmin=0;
// -----------------------------------------------------------------------
for (i=0; i<m_tStat.m_nProcs; i++)	
	if (m_tStat.m_pProcs[i].m_nCycles<m_tStat.m_pProcs[idxmin].m_nCycles)
		idxmin=i;
// -----------------------------------------------------------------------
nCycles=m_tStat.m_pProcs[idxmin].m_nCycles;
// -----------------------------------------------------------------------
if (m_tStat.m_tPMain.m_nCycles>0 && m_tStat.m_nProcs>0)	// (30.05.2002)
if (m_tStat.m_tPMain.m_nCycles<m_tStat.m_pProcs[idxmin].m_nCycles)
	nCycles=m_tStat.m_tPMain.m_nCycles;
if (m_tStat.m_tPMain.m_nCycles>0 && m_tStat.m_nProcs==0)// (30.05.2002)
	nCycles=m_tStat.m_tPMain.m_nCycles;
// -----------------------------------------------------------------------
return(nCycles);
}
// =======================================================================
BOOL CPetriNet::NewNetFromMPN(CFileMPN::tagMPNMEM *pMPN)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets all init data from MPNMEM structure
//   Parametrs: pMPN	- ptr to MPNMEM structure
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !pMPN || !pMPN->m_bCorrect)
	return(false);
// -----------------------------------------------------------------------
int i, j;
// -----------------------------------------------------------------------
// creates new network
// -----------------------------------------------------------------------
New(pMPN->m_nPNplaces, pMPN->m_nPNtransitions, 
	pMPN->m_nPNconnections, pMPN->m_nPMs, pMPN->m_nProcs);
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++)
	{
	m_pPlaces[i].DefinePlace(pMPN->m_pPlaces[i].m_nID,
		pMPN->m_pPlaces[i].m_nTokens);
	m_pPlaces[i].SetName(pMPN->m_pPlaces[i].m_sName);	
	}
// -----------------------------------------------------------------------
for (i=0; i<m_nTransitions; i++)
	{
	m_pTrans[i].DefineTransition(pMPN->m_pTrans[i].m_nID,
		pMPN->m_pTrans[i].m_nTTFmax,
		pMPN->m_pTrans[i].m_nProbab);
	m_pTrans[i].SetName(pMPN->m_pTrans[i].m_sName);
	}
// -----------------------------------------------------------------------
for (i=0; i<m_nConnections; i++)
	{
	m_pConns[i].m_nID=pMPN->m_pConns[i].m_nID;
	m_pConns[i].m_nType=pMPN->m_pConns[i].m_nType;
	m_pConns[i].m_nMult=pMPN->m_pConns[i].m_nMult;
	m_pConns[i].m_nDirType=pMPN->m_pConns[i].m_nDirType;
	m_pConns[i].m_nCIDa=pMPN->m_pConns[i].m_nCIDa;
	m_pConns[i].m_nCIDb=pMPN->m_pConns[i].m_nCIDb;
	}
// -----------------------------------------------------------------------
for (i=0;  i<m_nPMs; i++)
	{	
	m_pPMs[i].m_nID=pMPN->m_pPMs[i].m_nID;
	m_pPMs[i].m_bStatusOk=pMPN->m_pPMs[i].m_bStatusOk;
	m_pPMs[i].m_nTasks=pMPN->m_pPMs[i].m_nTasks;
	m_pPMs[i].m_bBusy=false;
	m_pPMs[i].m_nCurTaskID=0;
	m_pPMs[i].m_nTTFcur=0;
	m_pPMs[i].m_pTaskIDs=new int [m_pPMs[i].m_nTasks];
	for (j=0; j<m_pPMs[i].m_nTasks; j++)
		//m_pPMs[i].m_pTasksID[j]=pMPN->m_pPMs[i].m_pTasks[j].m_nTaskID;
		m_pPMs[i].m_pTaskIDs[j]=pMPN->m_pPMs[i].m_pTaskIDs[j];
	strcpy(m_pPMs[i].m_sName, pMPN->m_pPMs[i].m_sName);
	}
// -----------------------------------------------------------------------
for (i=0;  i<m_nProcs; i++)
	{	
	m_pProcs[i].m_nID=pMPN->m_pProcs[i].m_nID;
	m_pProcs[i].m_nResources=pMPN->m_pProcs[i].m_nResources;
	m_pProcs[i].m_pRsrcIDs=new int [m_pProcs[i].m_nResources];
	for (j=0; j<m_pProcs[i].m_nResources; j++)
		m_pProcs[i].m_pRsrcIDs[j]=pMPN->m_pProcs[i].m_pRsrcIDs[j];
	strcpy(m_pProcs[i].m_sName, pMPN->m_pProcs[i].m_sName);
	}
// -----------------------------------------------------------------------
ConnectNet(); if (!CheckNetIDs()) return(false); 
InitResources(); // 28.05.2002
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::SetInitConnections(int *pConnections)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets connections in the net
//   Parametrs: pConnections	- array with connections
//				size of array is m_nConnections*2 (!!! in the version !)
//				format of array: 
//				nCIDa[0], nCIDb[0] ... nCIDa[n-1], nCIDb[n-1]
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !m_nPlaces || !m_nTransitions || !pConnections) 
	return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nConnections; i++)
	{
	m_pConns[i].m_nCIDa=pConnections[i*2+0];
	m_pConns[i].m_nCIDb=pConnections[i*2+1];
	}
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::SetInitMarking(int *pMarking)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new marking to the net
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 03.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !m_nPlaces || !pMarking) return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nPlaces; i++)	m_pPlaces[i].PlaceSetTokens(pMarking[i]);
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::SetInitPMs(int *pPMs)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets PMs ID and number of tasks are solved on each PM
//   Parametrs: pPMs	- array with PMs
//				size of array is m_nPMs*2 (!!! in the version !!!)
//				format of array: 
//				nID[0], nTasks[0] ... nID[m_nPMs-1], nTasks[m_nPMs-1]
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !m_nPMs || !pPMs) 
	return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nPMs; i++)
	{
	m_pPMs[i].m_nID=pPMs[i*2+0];
	m_pPMs[i].m_nTasks=pPMs[i*2+1];
	}
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::SetInitProbab(int *pProbab)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new "Probability of firing" to all transitions
//   Parametrs: pProbab	- array with Probability
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !m_nTransitions || !pProbab) return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nTransitions; i++) m_pTrans[i].TransSetProbab(pProbab[i]);
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::SetInitTTF(int *pTTFs)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new TTFs to all transitions
//   Parametrs: pTTFs	- array with TTF
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
if (!IsInited() || !m_nTransitions || !pTTFs) return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nTransitions; i++) m_pTrans[i].TransSetTTF(pTTFs[i]);
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::GetLastIterNum()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of the last iteration
//   Parametrs: <none>
//      Return: ...
// -----------------------------------------------------------------------
//			MD: 11.04.01, 08.05.2002
//			LE: 08.05.2002
// -----------------------------------------------------------------------
return(m_nCurIterNum);
}
// =======================================================================
int CPetriNet::GetNumberOfCycles()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of cycles have been performed up to now
//   Parametrs: <none>
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 17.05.2002
//			LE: 17.05.2002
// -----------------------------------------------------------------------
return(m_tStat.m_tPMain.m_nCycles);
}
// =======================================================================
int CPetriNet::GetNumberOfIters()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of iterations have been performed up to now
//   Parametrs: <none>
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 08.05.2002
//			LE: 08.05.2002
// -----------------------------------------------------------------------
return(m_tHist.m_nSize);
}
// =======================================================================
BOOL CPetriNet::InitHistory()
{
// -SERGY-----------------------------------------------------------------
// Description: Inits history
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 09.05.2002
//			LE: 18.05.2002
// -----------------------------------------------------------------------
m_tHist.m_nSize=0; return(true);
}
// =======================================================================
BOOL CPetriNet::InitResources()
{
// -SERGY-----------------------------------------------------------------
// Description: Inits resources
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 08.06.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
m_tRsrcs.m_nPlaces=m_nPlaces;
m_tRsrcs.m_nTrans=m_nTransitions;	// 09.05.2002
m_tRsrcs.m_nConns=m_nConnections;	// 18.05.2002
m_tRsrcs.m_nPMs=m_nPMs;
m_tRsrcs.m_nProcs=m_nProcs;			// 19.05.2002
// -----------------------------------------------------------------------
m_tRsrcs.m_pLPL=new protlpl[m_nPlaces];			// Places list
m_tRsrcs.m_pLTR=new protltr[m_nTransitions];	// Trans list
m_tRsrcs.m_pLPM=new protlpm[m_nPMs];			// PMs list
m_tRsrcs.m_pLPROC=new rsrcproc[m_nProcs];		// (08.06.2002)
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++)
	{
	m_tRsrcs.m_pLPL[i].m_nID=m_pPlaces[i].GetID();
	strcpy(m_tRsrcs.m_pLPL[i].m_sName, m_pPlaces[i].GetName());
	}
// -----------------------------------------------------------------------
for (i=0; i<m_nTransitions; i++)
	{
	m_tRsrcs.m_pLTR[i].m_nID=m_pTrans[i].GetID();
	m_tRsrcs.m_pLTR[i].m_nTTF=m_pTrans[i].TransGetTTF();
	m_tRsrcs.m_pLTR[i].m_nProbab=m_pTrans[i].TransGetProbab();
	strcpy(m_tRsrcs.m_pLTR[i].m_sName, m_pTrans[i].GetName());
	}
// -----------------------------------------------------------------------
for (i=0; i<m_nPMs; i++)
	{
	m_tRsrcs.m_pLPM[i].m_nID=m_pPMs[i].m_nID;
	strcpy(m_tRsrcs.m_pLPM[i].m_sName, m_pPMs[i].m_sName);
	}
// -----------------------------------------------------------------------
for (i=0; i<m_tRsrcs.m_nProcs; i++)
	{
	m_tRsrcs.m_pLPROC[i].m_nID=m_pProcs[i].m_nID;
	strcpy(m_tRsrcs.m_pLPROC[i].m_sName, m_pProcs[i].m_sName);
	}
// -----------------------------------------------------------------------
m_tRsrcs.m_bInited=true; return(true);
}
// =======================================================================
BOOL CPetriNet::InitStatistics()
{
// -SERGY-----------------------------------------------------------------
// Description: Inits given statistics structure
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 19.05.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
m_tStat.m_nProcs=m_nProcs;	// 16.05.2002
m_tStat.m_nMaxParallPMs=0;	// maximal number of parallel working PMs (17.05.2002)
m_tStat.m_nMinParallPMs=0;	// minimal number of parallel working PMs (20.05.2002)
m_tStat.m_tPMain.m_nStagns=0;		// no stagnations (20.05.2002)
m_tStat.m_tPMain.m_nCycles=0;		// no cycles yet (16.05.2002)
m_tStat.m_tPMain.m_nTimeInCycles=0;	// no time (17.05.2002)
// -----------------------------------------------------------------------
m_tStat.m_pProcs=new statproc[m_nProcs];
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	{
	m_tStat.m_pProcs[i].m_nID=m_pProcs[i].m_nID;	
	m_tStat.m_pProcs[i].m_nCycles=0;
	m_tStat.m_pProcs[i].m_nTimeInCycles=0;
	m_tStat.m_pProcs[i].m_nMarkDub=0;
	m_tStat.m_pProcs[i].m_nMarkOver=0;
	m_tStat.m_pProcs[i].m_nStagns=0;
	}
// -----------------------------------------------------------------------
m_tStat.m_bInit=true; return(true);
}
// =======================================================================
BOOL CPetriNet::AddStateToHistory(int nStep)
{ 
// -SERGY-----------------------------------------------------------------
// Description: Add current state to history array
//   Parametrs: nStep	- step of RA (before FIRE, after FIRE, after LOAD)
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 06.05.2002
//			LE: 07.05.2002
// -----------------------------------------------------------------------
int i;
int nNewHistSize=m_tHist.m_nSize+1;
protrec tNewRec;		// current state
protrec* pNewHistory;	// ptr to the new history array
// -----------------------------------------------------------------------
// Initialization of 'm_tHist'
// -----------------------------------------------------------------------
if (m_tHist.m_nSize==0)	
	InitHistory();	
// -----------------------------------------------------------------------
tNewRec=GetCurrentState();
// -----------------------------------------------------------------------
pNewHistory=new protrec[nNewHistSize];
// -----------------------------------------------------------------------
for (i=0; i<m_tHist.m_nSize; i++) pNewHistory[i]=m_tHist.m_pHistory[nStep][i];
pNewHistory[m_tHist.m_nSize]=tNewRec;
// -----------------------------------------------------------------------
// deletes old history
// -----------------------------------------------------------------------
if (m_tHist.m_nSize)
	{ 
	// !!! The following 2 lines make error-exception (06.05.2002)
	//for (i=0; i<m_nHistSize; i++)	
	//	{ delete m_pHistory[i].m_pMarking; delete m_pHistory[i].m_pPMRs; }
	delete m_tHist.m_pHistory[nStep];
	}
// -----------------------------------------------------------------------
if (nStep==2) m_tHist.m_nSize=nNewHistSize;
m_tHist.m_pHistory[nStep]=pNewHistory;
// -----------------------------------------------------------------------
return(true); 
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::UpdateStatistics()
{
// -SERGY-----------------------------------------------------------------
// Description: Updates statistics
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 19.05.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
if (!m_tStat.m_bInit) InitStatistics();
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
CPetriNet::tagLOGREC CPetriNet::GetCurrentState()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns one history record with current net state
//   Parametrs: <none>
//      Return: rec - success
// -----------------------------------------------------------------------
//			MD: 07.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
int i;
protrec tRec;
CPetriElem* pElem;
// -----------------------------------------------------------------------
// Gets current marking & PMs states
// -----------------------------------------------------------------------
tRec.m_nCurIterNum=m_nCurIterNum;
tRec.m_nMarkType=PN_MARKNORM;		// default marking
tRec.m_nMarkRef=PN_ERROR;				// no referrence
tRec.m_bPMStagn=true;					// stagnation (20.05.2002)
tRec.m_pTokens=new int[m_nPlaces];		// marking
tRec.m_pPMST=new logpmst[m_nPMs];		// PMs
tRec.m_pProcs=new marktype[m_nProcs];	// marking types (19.05.2002)
// -----------------------------------------------------------------------
for (i=0; i<m_nPlaces; i++)
	tRec.m_pTokens[i]=m_pPlaces[i].PlaceGetTokens();
for (i=0; i<m_nPMs; i++)
	{
	tRec.m_bPMStagn=tRec.m_bPMStagn && !m_pPMs[i].m_bBusy;
	tRec.m_pPMST[i].m_bStatusOk=m_pPMs[i].m_bStatusOk;
	tRec.m_pPMST[i].m_bBusy=m_pPMs[i].m_bBusy;
	tRec.m_pPMST[i].m_nCurTaskID=m_pPMs[i].m_nCurTaskID;
	tRec.m_pPMST[i].m_nTTFcur=m_pPMs[i].m_nTTFcur;
	tRec.m_pPMST[i].m_nTTFmax=0;	
	if (m_pPMs[i].m_bStatusOk &&
		m_pPMs[i].m_bBusy)
		{
		pElem=GetElemByID(m_pPMs[i].m_nCurTaskID);
		tRec.m_pPMST[i].m_nTTFmax=pElem->TransGetTTF();
		}	
	}
for (i=0; i<m_nProcs; i++)		// sets default marking
	{
	tRec.m_pProcs[i].m_nMarkType=PN_MARKNORM;
	tRec.m_pProcs[i].m_nMarkRef=PN_ERROR;
	}
// -----------------------------------------------------------------------
return(tRec);
}
// =======================================================================
//DEL BOOL CPetriNet::HPAnalyseAllMarkings()
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Analises all markings in the history protocol
//DEL //   Parametrs: <none>
//DEL //      Return: true - success, false - error
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 07.05.2002
//DEL //			LE: 07.05.2002
//DEL // -----------------------------------------------------------------------
//DEL int i;
//DEL int nHistSize=m_tHist.m_nSize;
//DEL for(i=0; i<nHistSize; i++)	
//DEL 	HPAnalyseMarking(i);
//DEL // -----------------------------------------------------------------------
//DEL return(true);
//DEL }
// =======================================================================
BOOL CPetriNet::HPAnalyseMarking(int nNum)
{
// -SERGY-----------------------------------------------------------------
// Description: Analyses marking from history protocol with given number
//   Parametrs: nNum	- record number in the history protocol
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 08.05.2002
//			LE: 09.05.2002
// -----------------------------------------------------------------------
if(m_tHist.m_nSize==0) return(true);
// -----------------------------------------------------------------------
int nRefDub=HPGetDubOverRecNum(nNum, false);
int nRefOver=HPGetDubOverRecNum(nNum, true);
if(nRefOver!=PN_ERROR)
	{
	m_tHist.m_pHistory[1][nNum].m_nMarkType=PN_MARKOVER;
	m_tHist.m_pHistory[1][nNum].m_nMarkRef=nRefOver;
	}
if(nRefDub!=PN_ERROR)		
	{
	m_tHist.m_pHistory[1][nNum].m_nMarkType=PN_MARKDUB;
	m_tHist.m_pHistory[1][nNum].m_nMarkRef=nRefDub;
	}
HPAnalyseCycling(nNum);		// analize cycling (16.05.2002)
return(true);
}
// =======================================================================
BOOL CPetriNet::HPAnalyseMarkingProcs(int nNum)
{
// -SERGY-----------------------------------------------------------------
// Description: Analyses processes markings from history protocol record
//				with given number
//   Parametrs: nNum	- record number in the history protocol
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
if(m_tHist.m_nSize==0) return(true);
// -----------------------------------------------------------------------
int i, nProcID;
int nRefDub, nRefOver;
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	{
	nProcID=m_pProcs[i].m_nID;
	nRefDub=HPGetProcDubOverRecNum(nProcID, nNum, false);
	nRefOver=HPGetProcDubOverRecNum(nProcID, nNum, true);
	if(nRefOver!=PN_ERROR)
		{
		m_tHist.m_pHistory[1][nNum].m_pProcs[i].m_nMarkType=PN_MARKOVER;
		m_tHist.m_pHistory[1][nNum].m_pProcs[i].m_nMarkRef=nRefOver;
		}
	if(nRefDub!=PN_ERROR)		
		{
		m_tHist.m_pHistory[1][nNum].m_pProcs[i].m_nMarkType=PN_MARKDUB;
		m_tHist.m_pHistory[1][nNum].m_pProcs[i].m_nMarkRef=nRefDub;
		}
	}
// -----------------------------------------------------------------------
HPAnalyseCyclingProcs(nNum);		// analize cycling (16.05.2002)
return(true);
}
// =======================================================================
BOOL CPetriNet::HPAnalyseCycling(int nNum)
{
// -SERGY-----------------------------------------------------------------
// Description: Calculates number of cycles have been performed
//   Parametrs: nNum	- last record number in the history protocol
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 16.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
if(m_tHist.m_nSize==0) return(false);
if (!m_tStat.m_bInit) InitStatistics();	// (22.05.2002)
// -----------------------------------------------------------------------
int i;
int nCycles=0;		// no cycles available
int nMarkRef;
// -----------------------------------------------------------------------
if (m_tHist.m_pHistory[1][nNum].m_nMarkType!=PN_MARKDUB &&
	m_tHist.m_pHistory[1][nNum].m_nMarkType!=PN_MARKOVER)		
	return(true);
nMarkRef=m_tHist.m_pHistory[1][nNum].m_nMarkRef;
// -----------------------------------------------------------------------
// Calculates cycles in backward direction (20.05.2002)
// -----------------------------------------------------------------------
for (i=nNum; i>nMarkRef; i--)
	if ((m_tHist.m_pHistory[1][i].m_nMarkType==PN_MARKDUB ||
		m_tHist.m_pHistory[1][i].m_nMarkType==PN_MARKOVER) &&
		m_tHist.m_pHistory[1][i].m_nMarkRef==nMarkRef)
		nCycles++;	
if (nCycles==0) return(true);
// -----------------------------------------------------------------------
// Calculates time of all cycles (17.05.2002)
// -----------------------------------------------------------------------
if (nCycles>m_tStat.m_tPMain.m_nCycles)
	{	
	m_tStat.m_tPMain.m_nCycles=nCycles;		
	m_tStat.m_tPMain.m_nTimeInCycles=nNum-nMarkRef;
	}
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriNet::HPAnalyseCyclingProcs(int nNum)
{
// -SERGY-----------------------------------------------------------------
// Description: Calculates number of cycles have been performed for each
//				process
//   Parametrs: nNum	- last record number in the history protocol
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
if(m_tHist.m_nSize==0) return(false);
if (!m_tStat.m_bInit) InitStatistics();
// -----------------------------------------------------------------------
int i, k;
int nCycles;		// no cycles available
int nMarkRef;
// -----------------------------------------------------------------------
for(k=0; k<m_nProcs; k++)
	{
	// -------------------------------------------------------------------
	nCycles=0;		// no cycles are counted (21.05.2002)
	// -------------------------------------------------------------------
	if (m_tHist.m_pHistory[1][nNum].m_pProcs[k].m_nMarkType!=PN_MARKDUB &&
		m_tHist.m_pHistory[1][nNum].m_pProcs[k].m_nMarkType!=PN_MARKOVER)		
		continue;
	nMarkRef=m_tHist.m_pHistory[1][nNum].m_pProcs[k].m_nMarkRef;
	// -------------------------------------------------------------------
	// Calculates cycles in backward direction (20.05.2002)
	// -------------------------------------------------------------------
	for (i=nNum; i>nMarkRef; i--)
		if ((m_tHist.m_pHistory[1][i].m_pProcs[k].m_nMarkType==PN_MARKDUB ||
			m_tHist.m_pHistory[1][i].m_pProcs[k].m_nMarkType==PN_MARKOVER) &&
			m_tHist.m_pHistory[1][i].m_pProcs[k].m_nMarkRef==nMarkRef)
			nCycles++;	
	if (nCycles==0) return(true);
	// -------------------------------------------------------------------
	// Calculates time of all cycles (17.05.2002)
	// -------------------------------------------------------------------
	if (nCycles>m_tStat.m_pProcs[k].m_nCycles)
		{
		m_tStat.m_pProcs[k].m_nCycles=nCycles;
		m_tStat.m_pProcs[k].m_nTimeInCycles=nNum-nMarkRef;
		}
	// -------------------------------------------------------------------
	}
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::HPGetDubOverRecNum(int nRecNum, BOOL bFindOver)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of state with DUBBING marking for given 
//				record number
//   Parametrs: nRecNum		- record number
//				bFindOver	- 'true', if we want to find OVERLAPPING 
//							  marking
//      Return: idx - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 07.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
int i, j;
int nTokI, nTokCur;		// number of tokens
BOOL bBusyI,bStatOkI;
BOOL bCurBusy, bCurStatOk;
BOOL bStagnI, bStagnN;	// PMs are in stagnation, but marking is not TERM
int nCurIter;
int nIterI=m_tHist.m_pHistory[1][nRecNum].m_nCurIterNum;
BOOL bEqMark, bEqPMs;	// equal marking, PMs state
// -----------------------------------------------------------------------
for(i=0; i<nRecNum; i++)
	{
	// -------------------------------------------------------------------
	// loooking for identical marking
	// -------------------------------------------------------------------
	bEqMark=true;
	for(j=0; j<m_tRsrcs.m_nPlaces; j++)
		{
		// ---------------------------------------------------------------		
		nTokI=m_tHist.m_pHistory[1][i].m_pTokens[j];
		nTokCur=m_tHist.m_pHistory[1][nRecNum].m_pTokens[j];
		// ---------------------------------------------------------------
		if	(bFindOver && ((nTokI!=nTokCur && nTokI>nTokCur) ||
			(nTokI==0 && nTokI<nTokCur)))
				{ bEqMark=false; break; }
		if (!bFindOver && (nTokI!=nTokCur))
				{ bEqMark=false; break; }
		}
	nCurIter=m_tHist.m_pHistory[1][i].m_nCurIterNum;
	// -------------------------------------------------------------------
	// if we have found identical marking, checks PMs states
	// -------------------------------------------------------------------
	if(bEqMark && nCurIter!=nIterI)
		{
		bEqPMs=true;
		bStagnI=m_tHist.m_pHistory[2][i].m_bPMStagn; // check for stagnation
		bStagnN=m_tHist.m_pHistory[2][nRecNum].m_bPMStagn;
		for (j=0; j<m_tRsrcs.m_nPMs; j++)
			{
			// -----------------------------------------------------------
			bStatOkI=m_tHist.m_pHistory[1][i].m_pPMST[j].m_bStatusOk;
			bBusyI	=m_tHist.m_pHistory[1][i].m_pPMST[j].m_bBusy;
			// -----------------------------------------------------------
			bCurStatOk	=m_tHist.m_pHistory[1][nRecNum].m_pPMST[j].m_bStatusOk;
			bCurBusy	=m_tHist.m_pHistory[1][nRecNum].m_pPMST[j].m_bBusy;
			// -----------------------------------------------------------			
			if (bStatOkI!=bCurStatOk || bBusyI!=bCurBusy)
				{ bEqPMs=false; break; }
			if (bStatOkI && bBusyI)
				{ bEqPMs=false; break; }
			}
		if (bEqPMs && !bStagnI && !bStagnN)
			return(i);
		}
	}
// -----------------------------------------------------------------------
return(PN_ERROR);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::HPGetProcDubOverRecNum(int nProcID, int nRecNum, BOOL bFindOver)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of state with DUBBING marking for given 
//				record number and given PROCESS
//				BASED ON 'HPGetDubOverRecNum (...)'
//   Parametrs: nProcsID	- process' ID
//				nRecNum		- record number
//				bFindOver	- 'true', if we want to find OVERLAPPING 
//							  marking
//      Return: idx - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
int i, j;
int nTokI, nTokN;		// number of tokens
BOOL bBusyI, bStatOkI;
BOOL bBusyN, bStatOkN;
BOOL bStagnI, bStagnN;	// PMs are in stagnation, but marking is not TERM
int nTaskIDN, nTaskIDI;
int nIterN;
int nIterI=m_tHist.m_pHistory[1][nRecNum].m_nCurIterNum;
BOOL bEqMark, bEqPMs;	// equal marking, PMs state
BOOL bHaveCompared;		// elementd have been compared
// -----------------------------------------------------------------------
for(i=0; i<nRecNum; i++)
	{
	// -------------------------------------------------------------------
	// loooking for identical marking
	// -------------------------------------------------------------------
	bEqMark=true;
	bHaveCompared=false;
	for(j=0; j<m_tRsrcs.m_nPlaces; j++)
		{
		// ---------------------------------------------------------------
		// Checks, if given place belongs to given process, else - skips
		// ---------------------------------------------------------------
		if(!PRIsRsrcBelongToProc(m_pPlaces[j].GetID(), nProcID)) continue;		
		// ---------------------------------------------------------------
		bHaveCompared=true;
		nTokI=m_tHist.m_pHistory[1][i].m_pTokens[j];
		nTokN=m_tHist.m_pHistory[1][nRecNum].m_pTokens[j];
		// ---------------------------------------------------------------
		if	(bFindOver && ((nTokI!=nTokN && nTokI>nTokN) ||
			(nTokI==0 && nTokI<nTokN)))
				{ bEqMark=false; break; }
		if (!bFindOver && (nTokI!=nTokN))
				{ bEqMark=false; break; }
		}
	nIterN=m_tHist.m_pHistory[1][i].m_nCurIterNum;
	// -------------------------------------------------------------------
	// if we have found identical marking, checks PMs states
	// -------------------------------------------------------------------
	if(bEqMark && nIterN!=nIterI && bHaveCompared)
		{
		bEqPMs=true;		
		bStagnI=PRIsInStagnation(i, nProcID);		// (21.05.2002)
		bStagnN=PRIsInStagnation(nRecNum, nProcID);	// (21.05.2002)
		for (j=0; j<m_tRsrcs.m_nPMs; j++)
			{
			// -----------------------------------------------------------
			// Checks, if no PMs solve any TASK belonging to given process
			// -----------------------------------------------------------
			if(!PRCanPMSolveProcess(m_pPMs[j].m_nID, nProcID)) continue;
			// -----------------------------------------------------------		
			bStatOkI=m_tHist.m_pHistory[1][i].m_pPMST[j].m_bStatusOk;
			bBusyI	=m_tHist.m_pHistory[1][i].m_pPMST[j].m_bBusy;
			// -----------------------------------------------------------
			bStatOkN=m_tHist.m_pHistory[1][nRecNum].m_pPMST[j].m_bStatusOk;
			bBusyN	=m_tHist.m_pHistory[1][nRecNum].m_pPMST[j].m_bBusy;
			// -----------------------------------------------------------
			// Checks task IDs, if processors are busy
			// -----------------------------------------------------------
			if (bBusyI)
				{
				nTaskIDI=m_tHist.m_pHistory[1][i].m_pPMST[j].m_nCurTaskID;
				bBusyI=bBusyI && PRIsRsrcBelongToProc(nTaskIDI, nProcID);
				}
			if (bBusyN)
				{
				nTaskIDN=m_tHist.m_pHistory[1][i].m_pPMST[j].m_nCurTaskID;
				bBusyN=bBusyN && PRIsRsrcBelongToProc(nTaskIDN, nProcID);
				}
			// -----------------------------------------------------------
			if (bStatOkI!=bStatOkN || bBusyI!=bBusyN)
				{ bEqPMs=false; break; }
			if (bStatOkI && bBusyI)
				{ bEqPMs=false; break; }
			}
		if (bEqPMs && !bStagnI && !bStagnN)
			return(i);
		}
	}
// -----------------------------------------------------------------------
return(PN_ERROR);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriNet::HSAnaliseHistory()
{
// -SERGY-----------------------------------------------------------------
// Description: Calculates some statistics parametrs
//				HS - History Statistics
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 18.05.2002
//			LE: 21.05.2002
// -----------------------------------------------------------------------
if(m_tHist.m_nSize==0) return(false);
// -----------------------------------------------------------------------
int i, j;
m_tStat.m_nMaxParallPMs=HSGetNumberOfParallPMs(true);
m_tStat.m_nMinParallPMs=HSGetNumberOfParallPMs(false);
m_tStat.m_tPMain.m_nStagns=HSGetNumberOfStagnations();
m_tStat.m_tPMain.m_nMarkDub=HSGetNumOfDubOverMarkings(PN_MAINPROCESS, false);
m_tStat.m_tPMain.m_nMarkOver=HSGetNumOfDubOverMarkings(PN_MAINPROCESS, true);
// -----------------------------------------------------------------------
for (i=0; i<m_tStat.m_nProcs; i++)
	{
	m_tStat.m_pProcs[i].m_nMarkDub=HSGetNumOfDubOverMarkings(i, false);
	m_tStat.m_pProcs[i].m_nMarkOver=HSGetNumOfDubOverMarkings(i, true);
	m_tStat.m_pProcs[i].m_nStagns=0;		// (21.05.2002)
	for (j=0; j<m_tHist.m_nSize; j++)
		if(PRIsInStagnation(j, m_tStat.m_pProcs[i].m_nID))
			m_tStat.m_pProcs[i].m_nStagns++;
	}
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriNet::HSGetNumberOfParallPMs(BOOL bFindMax)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of parallel working PMs
//				HS - History Statistics
//   Parametrs: bFindMax	- =true, to find max number, =false, to find 
//							  min number
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 18.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
int i, j;
int res, resmax=0, resmin=m_nPMs;
// -----------------------------------------------------------------------
for (i=0; i<m_tHist.m_nSize; i++)
	{
	res=0;
	for (j=0; j<m_tRsrcs.m_nPMs; j++)
		if (m_tHist.m_pHistory[1][i].m_pPMST[j].m_bBusy ||
			m_tHist.m_pHistory[2][i].m_pPMST[j].m_bBusy)
			res++;
	if (res>resmax) resmax=res;
	if (res<resmin && res>0) resmin=res;
	}
// -----------------------------------------------------------------------
return(bFindMax?resmax:resmin);
}
// =======================================================================
int CPetriNet::HSGetNumberOfStagnations()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of stagnations PMs
//				HS - History Statistics
//   Parametrs: <none>
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
BOOL bStarv;
int i, j;
int res=0;
// -----------------------------------------------------------------------
for (i=0; i<m_tHist.m_nSize; i++)
	{	
	bStarv=true;
	for (j=0; j<m_tRsrcs.m_nPMs; j++)
		if (m_tHist.m_pHistory[1][i].m_pPMST[j].m_bBusy ||
			m_tHist.m_pHistory[2][i].m_pPMST[j].m_bBusy)
			bStarv=false;
	if (bStarv) res++;
	}
// -----------------------------------------------------------------------
return(res);
}
// =======================================================================
int CPetriNet::HSGetNumOfDubOverMarkings(int nNum, BOOL bCalcOver)
{
// -SERGY-----------------------------------------------------------------
// Description: Calculates number of DUB or OVER markings
//				HS - History Statistics
//   Parametrs: nNum	- process number (may be equal to PN_MAINPROCESS)
//				bCalcOver	- to calculate OVER markings
//      Return: num - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 20.05.2002
//			LE: 20.05.2002
// -----------------------------------------------------------------------
int i;
int resdub=0, resover=0;
// -----------------------------------------------------------------------
for (i=0; i<m_tHist.m_nSize; i++)	
	if (nNum==PN_MAINPROCESS)
		{
		if		(m_tHist.m_pHistory[1][i].m_nMarkType==PN_MARKDUB) resdub++;
		else if (m_tHist.m_pHistory[1][i].m_nMarkType==PN_MARKOVER) resover++;
		}
	else
		{
		if		(m_tHist.m_pHistory[1][i].m_pProcs[nNum].m_nMarkType==PN_MARKDUB) resdub++;
		else if (m_tHist.m_pHistory[1][i].m_pProcs[nNum].m_nMarkType==PN_MARKOVER) resover++;
		}
// -----------------------------------------------------------------------
return(bCalcOver?resover:resdub);
}
// =======================================================================
int CPetriNet::GetLastMarkingType()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns type of the last marking in the history
//   Parametrs: <none>
//      Return: type - success, PN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 08.05.2002
//			LE: 08.05.2002
// -----------------------------------------------------------------------
if(m_tHist.m_nSize==0) return(PN_ERROR);
// -----------------------------------------------------------------------
return(m_tHist.m_pHistory[1][m_tHist.m_nSize-1].m_nMarkType);
}
// =======================================================================
