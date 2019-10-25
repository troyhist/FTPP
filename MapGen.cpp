// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Fault Tolerant process mapping generator class
// Used library: CPetriNet, CPetriElem, CTPNFile
// -----------------------------------------------------------------------
// File: MapGen.cpp
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 22/ 05.2002
// Last edit: 22/ 05.2002
//     Build: 1
// =======================================================================
// MapGen.cpp: implementation of the CMapGen class.
// =======================================================================
#include "stdafx.h"
// -----------------------------------------------------------------------
#include "PetriNet.h"
// -----------------------------------------------------------------------
#include "MapGen.h"
// =======================================================================
// Precompile defenitions
// =======================================================================
// Debug defenitions
// -----------------------------------------------------------------------
#define MGDEB01			// print name of the current branch (26.05.2002)
// =======================================================================
// Construction/Destruction
// =======================================================================
CMapGen::CMapGen()
{
m_bInited=false;
}
// =======================================================================
CMapGen::~CMapGen()
{
Shutdown();
}
// =======================================================================
BOOL CMapGen::Generate(double dSurvProb, double dFaultProb, int nTau0,
					   int nTau1)
{
// -SERGY-----------------------------------------------------------------
// Description: Generates mapping
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 23.05.2002
// -----------------------------------------------------------------------
if (!m_bInited || !m_bLoaded) return(false);
// -----------------------------------------------------------------------
m_tAbsMapp.m_dProbSurv=dSurvProb;	
m_tAbsMapp.m_dProbFault=dFaultProb;
m_tAbsMapp.m_nTau[0]=nTau0;		m_tAbsMapp.m_nTau[1]=nTau1;
// -----------------------------------------------------------------------
BOOL bRes, bTimesOk;
int* pPMIDs;
proctime* pProcTimes;
// -----------------------------------------------------------------------
// Global optimisation algorithm
// -----------------------------------------------------------------------
// Step 01.
// -----------------------------------------------------------------------
pProcTimes=GO01GlobalCheckProcesses();
bTimesOk=GOCheckMinTimes(pProcTimes, m_nProcs, m_tAbsMapp.m_nTau[0]);
delete [] pProcTimes;
// -----------------------------------------------------------------------
if(!bTimesOk)
	{	
	strcpy(m_sErrMsg, "network doesn't satisfy initial constrains");
	return(false);
	}
// -----------------------------------------------------------------------
//m_tAbsMapp.m_nMaxPMs=m_nMaxPMs;
// -----------------------------------------------------------------------
while (!m_tAbsMapp.m_nMappings)
	{
	pPMIDs=GetFreeIDs(m_tAbsMapp.m_nMaxPMs, 0, NULL);
	bRes=GOStart(m_tAbsMapp.m_nMaxPMs, pPMIDs);
	delete pPMIDs; 
	if (!m_tAbsMapp.m_nMappings) { m_tAbsMapp.m_nMaxPMs++; Restart(); }
	if (!bRes) return(false);	
	} 
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
char* CMapGen::GetErrMsg()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns error MSG
//   Parametrs: <none>
//      Return: ptr - success, NULL - otherwise
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 23.05.2002
// -----------------------------------------------------------------------
if (!m_bInited) return(NULL); return(m_sErrMsg);
}
// =======================================================================
CMapGen::tagMGABSMAPP* CMapGen::GetMappings()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to absolute mapping structure
//   Parametrs: <none>
//      Return: ptr - success, NULL - otherwise
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
if (!m_bInited || !m_bLoaded) return(NULL);
// -----------------------------------------------------------------------
return(&m_tAbsMapp);
}
// =======================================================================
CPetriNet::tagRSRC* CMapGen::GetResources()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns ptr to resources
//   Parametrs: <none>
//      Return: ptr - success, NULL - otherwise
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
if (!m_bInited || !m_bLoaded) return(NULL);
// -----------------------------------------------------------------------
return(&m_tRsrcs);
}
// =======================================================================
BOOL CMapGen::Init()
{
// -SERGY-----------------------------------------------------------------
// Description: Inits all internal data and prepares class to work
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
if (m_bInited) Shutdown();	
// -----------------------------------------------------------------------
m_bLoaded=false;
// -----------------------------------------------------------------------
m_nProcs=0;
// -----------------------------------------------------------------------
m_tStat.m_nClosedBranches=0;
m_tStat.m_nMapCnt=0;
m_tStat.m_nMapCntOpt=0;
m_tStat.m_nTotalImits=0;
m_tStat.m_nImitIters=0;
m_tStat.m_nOptMapImitNum=MG_ERROR;	// (27.05.2002)
m_tStat.m_nOptMapNum=MG_ERROR;
m_tStat.m_nOptMapBranch=MG_ERROR;
// -----------------------------------------------------------------------
m_nHistSize=0;					m_pHistory=NULL;
// -----------------------------------------------------------------------
m_tMapRef.m_bInited=false;	// (25.05.2002)
m_tMapRef.m_tID.m_nMagNum=MG_ERROR;	// (27.05.2002)
m_tMapRef.m_tID.m_nPMs=MG_ERROR;	// (27.05.2002)
m_tMapRef.m_tID.m_nTasks=MG_ERROR;	// (27.05.2002)
// -----------------------------------------------------------------------
//m_pRefProcTimes=NULL;		// (25.05.2002)
m_dRefProcTime=0;			// (28.05.2002)
// -----------------------------------------------------------------------
m_tAbsMapp.m_nMappings=0;		m_tAbsMapp.m_pMapps=NULL;
m_tAbsMapp.m_dProbSurv	=MG_ERROR;
m_tAbsMapp.m_dProbFault	=MG_ERROR;	
m_tAbsMapp.m_nMaxPMs	=MG_ERROR;
m_tAbsMapp.m_nFaultPMs	=MG_ERROR;
m_tAbsMapp.m_nTau[0]=MG_ERROR;		m_tAbsMapp.m_nTau[1]=MG_ERROR;
//m_nTrans=0;
*m_sErrMsg=0;
// -----------------------------------------------------------------------
m_bInited=true;
return(true);
}
// =======================================================================
BOOL CMapGen::Shutdown()
{
// -SERGY-----------------------------------------------------------------
// Description: Shutdowns inited class
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
if (!m_bInited || !m_bLoaded) return(false);
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
if (m_nProcs)
	{
	for (i=0; i<m_nProcs; i++) delete m_pProcs[i].m_pRsrcIDs;
	delete [] m_pProcs;
	}
// -----------------------------------------------------------------------
// deletes absolute mapping
// -----------------------------------------------------------------------
if (m_tAbsMapp.m_nMappings)
	{
	for (i=0; i<m_tAbsMapp.m_nMappings; i++) 
		DeleteMapping(&m_tAbsMapp.m_pMapps[i]);
	delete [] m_tAbsMapp.m_pMapps; m_tAbsMapp.m_nMappings=0;
	}
// -----------------------------------------------------------------------
Restart(); ResetResources();
m_bInited=false;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::SetDataFromMPN(CFileMPN::tagMPNMEM *pMPN)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets all init data from MPNMEM structure
//   Parametrs: pMPN	- ptr to MPNMEM structure
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
if (!m_bInited || !pMPN ) return(false);
if (!pMPN->m_bCorrect) return(false);
if (!pMPN->m_nProcs || pMPN->m_nProcs==1) return(false);// no processes
// -----------------------------------------------------------------------
int i, j;
// -----------------------------------------------------------------------
// Some preparations
// -----------------------------------------------------------------------
m_pMPN=pMPN;
m_pMPN->m_nCriteria=0;		// we don't need criteria
m_pMPN->m_nIter=0;			// we don't need Number of Iterations to perform
m_pMPN->m_nItercur=0;		// we don't need Current Iteration Number
if (m_pMPN->m_nPMs) DeletePMList(m_pMPN->m_pPMs, m_pMPN->m_nPMs);
m_pMPN->m_nPMs=0;			// we don't need adjusted task allocation
// -----------------------------------------------------------------------
// Copies some data (processes, etc.)
// -----------------------------------------------------------------------
m_nProcs=pMPN->m_nProcs;
m_pProcs=new CFileMPN::tagMPNPROCMEM[m_nProcs];
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	{
	// -------------------------------------------------------------------
	// Copies processes into internal struct
	// -------------------------------------------------------------------
	m_pProcs[i].m_nID=pMPN->m_pProcs[i].m_nID;
	m_pProcs[i].m_nResources=pMPN->m_pProcs[i].m_nResources;
	m_pProcs[i].m_pRsrcIDs=new int[m_pProcs[i].m_nResources];
	strcpy(m_pProcs[i].m_sName, pMPN->m_pProcs[i].m_sName);
	for (j=0; j<m_pProcs[i].m_nResources; j++)
		m_pProcs[i].m_pRsrcIDs[j]=pMPN->m_pProcs[i].m_pRsrcIDs[j];
	// -------------------------------------------------------------------	
	}
// -----------------------------------------------------------------------
m_bLoaded=true; InitResources();
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::GO16AcceptMapRef()
{
// -SERGY-----------------------------------------------------------------
// Description: accepts referenced mapping to use
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i, j, nMappNew, nLast;
mapping* pMappNew;
// -----------------------------------------------------------------------
nMappNew=m_tAbsMapp.m_nMappings+1;
pMappNew=new mapping[nMappNew];
// -----------------------------------------------------------------------
// copies other mappings
// -----------------------------------------------------------------------
for (i=0; i<m_tAbsMapp.m_nMappings; i++)
	{
	pMappNew[i].m_bInited	=m_tAbsMapp.m_pMapps[i].m_bInited;
	pMappNew[i].m_nPMs		=m_tAbsMapp.m_pMapps[i].m_nPMs;
	pMappNew[i].m_nProcs	=m_tAbsMapp.m_pMapps[i].m_nProcs;
	pMappNew[i].m_tID		=m_tAbsMapp.m_pMapps[i].m_tID;
	pMappNew[i].m_pPMs=new CFileMPN::tagMPNPMMEM[pMappNew[i].m_nPMs];
	for (j=0; j<pMappNew[i].m_nPMs; j++)
		CopyPM(&pMappNew[i].m_pPMs[j], &m_tAbsMapp.m_pMapps[i].m_pPMs[j]);
	}
// -----------------------------------------------------------------------
// Add new mapping
// -----------------------------------------------------------------------
nLast=m_tAbsMapp.m_nMappings;
pMappNew[nLast].m_bInited	=m_tMapRef.m_bInited;
pMappNew[nLast].m_nPMs		=m_tMapRef.m_nPMs;
pMappNew[nLast].m_nProcs	=m_tMapRef.m_nProcs;
pMappNew[nLast].m_tID		=m_tMapRef.m_tID;
pMappNew[nLast].m_pPMs		=new CFileMPN::tagMPNPMMEM[pMappNew[i].m_nPMs];
for (i=0; i<pMappNew[nLast].m_nPMs; i++)
	CopyPM(&pMappNew[nLast].m_pPMs[i], &m_tMapRef.m_pPMs[i]);
// -----------------------------------------------------------------------
// deletes old absolute mapping
// -----------------------------------------------------------------------
for (i=0; i<m_tAbsMapp.m_nMappings; i++)
	DeleteMapping(&m_tAbsMapp.m_pMapps[i]);
delete [] m_tAbsMapp.m_pMapps;
// -----------------------------------------------------------------------
m_tAbsMapp.m_nMappings=nMappNew;
m_tAbsMapp.m_pMapps=pMappNew;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::AddToHistory(CMapGen::tagMGMAPID tMapID)
{
// -SERGY-----------------------------------------------------------------
// Description: adds given mapping's ID to history
//   Parametrs: tMapID	- mapping's ID
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
int i;
int nHistSizeNew=m_nHistSize+1;
mgmapid* pHistoryNew=new mgmapid[nHistSizeNew];
// -----------------------------------------------------------------------
for (i=0; i<m_nHistSize; i++)	
	pHistoryNew[i]=m_pHistory[i];
// -----------------------------------------------------------------------
pHistoryNew[m_nHistSize]=tMapID;
if (m_nHistSize) delete [] m_pHistory;
m_nHistSize=nHistSizeNew;
m_pHistory=pHistoryNew;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::CopyPM(CFileMPN::tagMPNPMMEM *pDestPM, 
					 CFileMPN::tagMPNPMMEM *pSrcPM)
{
// -SERGY-----------------------------------------------------------------
// Description: makes copy of given PM
//   Parametrs: pDestPM		- allocated space for PM structure
//				pSrcPM		- src PM
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i;
strcpy(pDestPM->m_sName, pSrcPM->m_sName);
pDestPM->m_bBusy		=pSrcPM->m_bBusy;
pDestPM->m_bStatusOk	=pSrcPM->m_bStatusOk;
pDestPM->m_nID			=pSrcPM->m_nID;
pDestPM->m_nTasks		=pSrcPM->m_nTasks;
pDestPM->m_nCurTaskID	=pSrcPM->m_nCurTaskID;
pDestPM->m_nTTFcur		=pSrcPM->m_nTTFcur;
pDestPM->m_pTaskIDs		=new int[pDestPM->m_nTasks];
for (i=0; i<pDestPM->m_nTasks; i++)
	pDestPM->m_pTaskIDs[i]=pSrcPM->m_pTaskIDs[i];
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::DeleteMapping(CMapGen::tagMGMAPPING* pMapp)
{
// -SERGY-----------------------------------------------------------------
// Description: deletes mapping
//   Parametrs: tMapp	- mapping
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
DeletePMList(pMapp->m_pPMs, pMapp->m_nPMs); pMapp->m_nPMs=0; 
pMapp->m_bInited=false; return(true);
}
// =======================================================================
BOOL CMapGen::DeletePMList(CFileMPN::tagMPNPMMEM *pPMs, int nPMs)
{
// -SERGY-----------------------------------------------------------------
// Description: deletes list with PMs
//   Parametrs: pPMs	- ptr to PMs list
//				nPMs	- size of PMs list
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
if (!nPMs) return(false);
for (int i=0; i<nPMs; i++) if(pPMs[i].m_nTasks) delete pPMs[i].m_pTaskIDs;
delete [] pPMs; pPMs=NULL; return(true);
}
// =======================================================================
double CMapGen::GetASLDOfTime(CMapGen::tagMGPROCTIME*pProcTimes)
{
// -SERGY-----------------------------------------------------------------
// Description: returns average square-law deflection of given times array
//   Parametrs: pProcTimes	- times of processes
//      Return: NULL - success, MG_ERROR - error
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
double dASLD=0;
for (int i=0; i<m_nProcs; i++) 
	dASLD+=MTHPowDN(pProcTimes[i].m_dCycleTime, 2);
return(dASLD);
}
// =======================================================================
int* CMapGen::GetFreeIDs(int nSize, int nVirtIDs, int* pVirtIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: generates array with 'nSize' free IDs
//   Parametrs: nSize		- number of IDs to generate
//				nVirtIDs	- size of array with virtual IDs (may be =0)
//				pVirtIDs	- array with virtual IDs (may be =NULL)
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
BOOL bFreeID;
int nFreeID;
int i, j=0;			// 'j' should be equal to '0'
int *pIDs;			// array with all IDs
int *pFreeIDs;
int nSizeOld, nSizeAll;
// -----------------------------------------------------------------------
nSizeOld=	m_pMPN->m_nPNplaces+		m_pMPN->m_nPNtransitions+
			m_pMPN->m_nPNconnections+	m_pMPN->m_nProcs+
			nVirtIDs;
nSizeAll=nSizeOld+nSize;
pIDs=new int [nSizeAll];
pFreeIDs=new int[nSize];
// -----------------------------------------------------------------------
for (i=0; i<m_pMPN->m_nPNplaces; i++)	pIDs[j++]=m_pMPN->m_pPlaces[i].m_nID;
for (i=0; i<m_pMPN->m_nPNtransitions; i++)	pIDs[j++]=m_pMPN->m_pTrans[i].m_nID;
for (i=0; i<m_pMPN->m_nPNconnections; i++)	pIDs[j++]=m_pMPN->m_pConns[i].m_nID;
for (i=0; i<m_pMPN->m_nProcs; i++)		pIDs[j++]=m_pMPN->m_pProcs[i].m_nID;
for (i=0; i<nVirtIDs; i++)				pIDs[j++]=pVirtIDs[i];
// -----------------------------------------------------------------------
j=0; nFreeID=1;
while(j<nSize)
	{
	bFreeID=true;
	for (i=0; i<nSizeOld+j; i++) if(pIDs[i]==nFreeID) 
		{ bFreeID=false; break; }
	if (bFreeID)
		{ pFreeIDs[j]=nFreeID; pIDs[nSizeOld+j]=nFreeID; j++; }
	nFreeID++;
	}
// -----------------------------------------------------------------------
delete pIDs; return(pFreeIDs);
// -----------------------------------------------------------------------
}
// =======================================================================
CMapGen::tagMGMAPID CMapGen::GetMapIDByMapping(CMapGen::tagMGMAPPING tMapp)
{
// -SERGY-----------------------------------------------------------------
// Description: returns mapping ID by given mapping
//   Parametrs: tMapp	- mapping
//      Return: inited mappind ID - success
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i, j;
int nMUL;
mgmapid tMapID;
// -----------------------------------------------------------------------
tMapID.m_nMagNum=0;
tMapID.m_nPMs=0;
tMapID.m_nTasks=0;
// -----------------------------------------------------------------------
tMapID.m_nPMs=tMapp.m_nPMs;
for (i=0; i<tMapp.m_nPMs; i++)
	tMapID.m_nTasks+=tMapp.m_pPMs[i].m_nTasks;	
// -----------------------------------------------------------------------
for (i=0; i<tMapp.m_nPMs; i++)
	{
	nMUL=1;
	for (j=0; j<tMapp.m_pPMs[i].m_nTasks; j++)
		nMUL*=tMapp.m_pPMs[i].m_pTaskIDs[j];
	nMUL*=tMapp.m_pPMs[i].m_nTasks;
	tMapID.m_nMagNum+=nMUL;
	}
// -----------------------------------------------------------------------
/*
for (i=0; i<tMapp.m_nPMs; i++)
	if (tMapp.m_pPMs[i].m_nTasks)
		{
		tMapID.m_nPMs++;
		tMapID.m_nTasks+=tMapp.m_pPMs[i].m_nTasks;
		}
// -----------------------------------------------------------------------
for (i=0; i<tMapp.m_nPMs; i++)	
	if (tMapp.m_pPMs[i].m_nTasks)
		{
		nMUL=1;
		for (j=0; j<tMapp.m_pPMs[i].m_nTasks; j++)
			nMUL*=tMapp.m_pPMs[i].m_pTaskIDs[j];
		nMUL*=tMapp.m_pPMs[i].m_nTasks;
		tMapID.m_nMagNum+=nMUL;		
		}
//*/
// -----------------------------------------------------------------------
return(tMapID);
}
// =======================================================================
int CMapGen::GetNumOfTasksByProcID(int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of tasks of the process with given ID
//   Parametrs: nProcID	- process's ID
//      Return: num - success, MG_ERROR - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
int i, nTasks=0;
CFileMPN::tagMPNPROCMEM* pProc;
// -----------------------------------------------------------------------
pProc=GetProcByID(nProcID);
if (!pProc) return(MG_ERROR);
// -----------------------------------------------------------------------
for (i=0; i<pProc->m_nResources; i++)
	if (IsRsrcATask(pProc->m_pRsrcIDs[i]))
		nTasks++;
return(nTasks);
}
// =======================================================================
CFileMPN::tagMPNPROCMEM* CMapGen::GetProcByID(int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns ptr to the process with given ID
//   Parametrs: nProcID	- process's ID
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 25.05.2002
// -----------------------------------------------------------------------
for (int i=0; i<m_nProcs; i++)
	if (m_pProcs[i].m_nID==nProcID) return(&m_pProcs[i]);	
return(NULL);
}
// =======================================================================
CMapGen::tagMGMAPPING CMapGen::GetTestAllocation()
{
// -SERGY-----------------------------------------------------------------
// Description: returns first pre-allocation of processes
//   Parametrs: pMPN	- ptr to MPNMEM structure
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
int i, j;
int *pFreeIDs;
mapping tMapp;
// -----------------------------------------------------------------------
tMapp.m_nPMs=m_nProcs;		// old
//tMapp.m_nPMs=m_nTasks;	// new version (not ready jet)
// -----------------------------------------------------------------------
tMapp.m_bInited=true;
tMapp.m_nProcs=m_nProcs;
tMapp.m_pPMs=new CFileMPN::tagMPNPMMEM[tMapp.m_nPMs];
pFreeIDs=GetFreeIDs(tMapp.m_nPMs, 0, NULL);
// -----------------------------------------------------------------------
for (i=0; i<tMapp.m_nPMs; i++)
	{
	sprintf(tMapp.m_pPMs[i].m_sName, "testpm%d", i+1);
	tMapp.m_pPMs[i].m_nID=pFreeIDs[i];
	tMapp.m_pPMs[i].m_bStatusOk=true;
	tMapp.m_pPMs[i].m_bBusy=false;
	tMapp.m_pPMs[i].m_nCurTaskID=0;
	tMapp.m_pPMs[i].m_nTTFcur=0;
	tMapp.m_pPMs[i].m_nTasks=m_pProcs[i].m_nResources;
	tMapp.m_pPMs[i].m_pTaskIDs=new int[tMapp.m_pPMs[i].m_nTasks];
	for (j=0; j<tMapp.m_pPMs[i].m_nTasks; j++)
		tMapp.m_pPMs[i].m_pTaskIDs[j]=m_pProcs[i].m_pRsrcIDs[j];
	}
// -----------------------------------------------------------------------
delete pFreeIDs; return(tMapp);
}
// =======================================================================
CMapGen::tagMGPROCTIME* CMapGen::GO01GlobalCheckProcesses()
{
// -SERGY-----------------------------------------------------------------
// Description: returns maximal number of PMs in the best conditions
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
int i, nCycles, nTimeInCycles;
double dCycleTime;
CPetriNet pn;
mapping tMapp;
proctime* pProcTimes;
// -----------------------------------------------------------------------
tMapp=GetTestAllocation(); if(!tMapp.m_bInited) return(NULL);
m_pMPN->m_pPMs=tMapp.m_pPMs;
m_pMPN->m_nPMs=tMapp.m_nPMs;
// -----------------------------------------------------------------------
pn.Init();
bRes=pn.NewNetFromMPN(m_pMPN); if(!bRes) return(NULL);
bRes=pn.Run(0, MG_STATITER, MG_STATCYCLES); if(!bRes) return(NULL);
// -----------------------------------------------------------------------
m_tAbsMapp.m_nMaxPMs=pn.GetStatistics()->m_nMaxParallPMs;
pProcTimes=new proctime[tMapp.m_nProcs];
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	{
	nCycles=pn.GetStatistics()->m_pProcs[i].m_nCycles;
	nTimeInCycles=pn.GetStatistics()->m_pProcs[i].m_nTimeInCycles;
	dCycleTime=nCycles?((double)nTimeInCycles/nCycles):MG_ERROR;
	pProcTimes[i].m_nID=pn.GetStatistics()->m_pProcs[i].m_nID;
	pProcTimes[i].m_dCycleTime=dCycleTime;	
	}
// -----------------------------------------------------------------------
pn.Shutdown(); DeleteMapping(&tMapp); m_pMPN->m_nPMs=0;
// -----------------------------------------------------------------------
return(pProcTimes);
}
// =======================================================================
int CMapGen::GO02GetMaxNumOfFaultPMs(int nTotalPMs)
{
// -SERGY-----------------------------------------------------------------
// Description: returns maximal number of fault PMs according to given 
//				total number of PMs
//   Parametrs: nTotalPMs	- total number of PMs
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 23.05.2002
// -----------------------------------------------------------------------
int nMaxFaultPMs;
double dSumProb;
// -----------------------------------------------------------------------
nMaxFaultPMs=0;
dSumProb=0.;
// -----------------------------------------------------------------------
do	{
	dSumProb+=MTHSurvProb(nMaxFaultPMs, nTotalPMs);
	nMaxFaultPMs++;
	} while (dSumProb<m_tAbsMapp.m_dProbSurv);
// -----------------------------------------------------------------------
return(nMaxFaultPMs-1);
}
// =======================================================================
BOOL CMapGen::GO025GetMapping(int nPMs, int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: generates mapping for given number of PMs
//   Parametrs: nTotalPMs	- total number of PMs
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
int i;
bbproc* pRoot;
// -----------------------------------------------------------------------
pRoot=GO03GetRootElem(pPMIDs, nPMs);
// -----------------------------------------------------------------------
for (i=0; i<nPMs; i++)
	{
	bRes=GOAnalyseNode(&pRoot->m_pNodes[i], nPMs, pPMIDs);
	if (!bRes) 
		{ GOCloseBranch(pRoot); return(false); }
	}
// -----------------------------------------------------------------------
for (i=0; i<nPMs; i++) GOCloseNode(&pRoot->m_pNodes[i]);	
// -----------------------------------------------------------------------
GOCloseBranch(pRoot); 
return(true);
}
// =======================================================================
CMapGen::tagBBPROC* CMapGen::GO03GetRootElem(int* pPMIDs, int nPMs)
{
// -SERGY-----------------------------------------------------------------
// Description: returns ptr to generated B&B ROOT element
//   Parametrs: pPMIDs	- array with PM's IDs
//				nPMs	- number of PMs, participating in allocation
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 23.05.2002
//			LE: 23.05.2002
// -----------------------------------------------------------------------
bbproc *pRoot;
// -----------------------------------------------------------------------
pRoot=new bbproc[1];
pRoot->m_bClosed=false;				// node isn't closed
pRoot->m_nProcID=MG_ERROR;			// NO Process
pRoot->m_dTimeMax=0;				// ROOT doesn't have time
pRoot->m_dTimeMin=0;				// ROOT doesn't have time
pRoot->m_nChilds=nPMs;
pRoot->m_pParent=NULL;				// Root doens't have parent
pRoot->m_pNodes=new bbpm[pRoot->m_nChilds];
// -----------------------------------------------------------------------
for (int i=0; i<nPMs; i++)
	{
	pRoot->m_pNodes[i].m_bClosed=false;
	pRoot->m_pNodes[i].m_nPMID=pPMIDs[i];
	pRoot->m_pNodes[i].m_pParent=pRoot;
	}
// -----------------------------------------------------------------------
return(pRoot);
}
// =======================================================================
int* CMapGen::GO05GetIDsOfUsablePMs(CMapGen::tagBBPROC *pBranch, int nPMs, 
									int *pPMIDs)
{
// -!!!-------------------------------------------------------------------
//        TODO: INSERT PM SELECTING ALGORITHM HERE
// -SERGY-----------------------------------------------------------------
// Description: returns number of free PMs
//   Parametrs: pBranch		- ptr to current Branch
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
int i, nUsablePMs;
int *pUsablePMIDs;
// -----------------------------------------------------------------------
nUsablePMs=GO05GetNumOfUsablePMs(pBranch, nPMs, pPMIDs);
if (nUsablePMs==0 || nUsablePMs==MG_ERROR) return(NULL);
// -----------------------------------------------------------------------
pUsablePMIDs=new int [nUsablePMs];
// -----------------------------------------------------------------------
for (i=0; i<nUsablePMs; i++) pUsablePMIDs[i]=pPMIDs[i];	
return(pUsablePMIDs);
// -----------------------------------------------------------------------
}
// =======================================================================
int CMapGen::GO05GetNumOfUsablePMs(CMapGen::tagBBPROC *pBranch, 
									  int nPMs, int* pPMIDs)
{
// -!!!-------------------------------------------------------------------
//        TODO: INSERT PM SELECTING ALGORITHM HERE
// -SERGY-----------------------------------------------------------------
// Description: returns number of free PMs
//   Parametrs: pBranch		- ptr to current Branch
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
if (pBranch->m_pParent==NULL) return(nPMs);	// for ROOT element
// -----------------------------------------------------------------------
int i;
int nUsablePMs=0;
//int nUnaProcs;
int *pPMUsed;			// how many times each PM was alredy used ?
// -----------------------------------------------------------------------
//nUnaProcs=GO06GetNumberOfUnallocProcs(pBranch->m_pParent);
//if (nUnaProcs==0) return(0);
// -----------------------------------------------------------------------
pPMUsed=new int[nPMs];
for (i=0; i<nPMs; i++)
	pPMUsed[i]=GOGetNumOfPMUsing(pBranch, pPMIDs[i]);
// -----------------------------------------------------------------------
// TODO: Here should be PM-selecting algorithm
// -----------------------------------------------------------------------
// BUT: Here it's no algorithm algorithm (24.05.2002)
// -----------------------------------------------------------------------
nUsablePMs=nPMs;
// -----------------------------------------------------------------------
delete pPMUsed; return(nUsablePMs);
}
// =======================================================================
int* CMapGen::GO06GetIDsOfUnallocProcsForNode(CMapGen::tagBBPM *pNode)
{
// -SERGY-----------------------------------------------------------------
// Description: returns array of IDs of unallocated processes for given 
//				node
//   Parametrs: pNode		- ptr to PM elem
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
int i, j, nUnaProcs;
int *pUnaProcIDs;
// -----------------------------------------------------------------------
nUnaProcs=GO06GetNumOfUnallocProcsForNode(pNode);
if (nUnaProcs==0 || nUnaProcs==MG_ERROR) return(NULL);
// -----------------------------------------------------------------------
pUnaProcIDs=new int[nUnaProcs];
j=0;
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	if (!GOIsProcAllocFromNode(pNode, m_pProcs[i].m_nID))
		pUnaProcIDs[j++]=m_pProcs[i].m_nID;	
// -----------------------------------------------------------------------
return(pUnaProcIDs);
}
// =======================================================================
int CMapGen::GO06GetNumOfUnallocProcsForNode(CMapGen::tagBBPM *pNode)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of unallocated processes for given node
//   Parametrs: pNode		- ptr to PM elem
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 23.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
int i, nAllocProcs=0;
for (i=0; i<m_nProcs; i++)
	if (GOIsProcAllocFromNode(pNode, m_pProcs[i].m_nID)) nAllocProcs++;
return(m_nProcs-nAllocProcs);
}
// =======================================================================
CMapGen::tagMGPROCTIME* CMapGen::GO08GetMinCycleTime(CMapGen::tagBBPROC *pBranch, 
									   int nPMs, int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: returns lower cycle time bound for given branch
//   Parametrs: pBranch	- ptr to researched branch
//				nPMs	- total number of PMs
//				pPMIDs	- array of PM's IDs
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
mapping tMapp;
proctime* pProcTimes;
// -----------------------------------------------------------------------
tMapp=GOGetVirtMapping(pBranch, nPMs, pPMIDs, true);
if (tMapp.m_bInited==false) return(NULL);
pProcTimes=GOGetTimeFromImitation(tMapp);
DeleteMapping(&tMapp);
// -----------------------------------------------------------------------
return(pProcTimes);
}
// =======================================================================
BOOL CMapGen::GO09CloseExceedBranches(CMapGen::tagBBPM *pNode, int nPMs, 
									  int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: Closes branches at given node if time of cycle in branch 
//				is exceeded
//   Parametrs: pNode	- ptr to researched branch
//				nPMs	- total number of PMs
//				pPMIDs	- array of PM's IDs
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 25.05.2002
// -----------------------------------------------------------------------
BOOL bDubMapp, bTimesOk;
int i;
mapping tTestMapp;
bbproc* pBranch;
proctime* pProcTimes;
// -----------------------------------------------------------------------
#ifdef MGDEB01
printf("\n");
#endif
for (i=0; i<pNode->m_nChilds; i++)
	{
	// -------------------------------------------------------------------
	// Step 08. Gets min cycles times
	// -------------------------------------------------------------------
	pBranch=&pNode->m_pBranches[i];
#ifdef MGDEB01
	//printf("%s ... ", MGDEB01GetBranchPath(pBranch));
	printf("%s ... ", MGDEB01GetBranchMapping(pBranch, nPMs, pPMIDs));
#endif
	tTestMapp=GOGetMappingForBranch(pBranch, nPMs, pPMIDs);
	bDubMapp=IsMappingChecked(tTestMapp);
#ifdef MGDEB01
	//printf("(%d.%d.%d) ", tTestMapp.m_tID.m_nMagNum, .m_tID.m_nPMs,tTestMapp.m_tID.m_nTasks);
#endif
	if (bDubMapp)
		{
		DeleteMapping(&tTestMapp);
		GOCloseBranch(pBranch);
		#ifdef MGDEB01
		printf("- DUB\n");
		#endif
		continue;		
		}
	// -------------------------------------------------------------------
	AddToHistory(tTestMapp.m_tID); DeleteMapping(&tTestMapp);
	pProcTimes=GO08GetMinCycleTime(pBranch, nPMs, pPMIDs);
	if (!pProcTimes) return(false);
	bTimesOk=GOCheckMinTimes(pProcTimes, m_nProcs, m_tAbsMapp.m_nTau[1]);
	// -------------------------------------------------------------------
	if (!bTimesOk)
		{
		GOCloseBranch(pBranch);
		#ifdef MGDEB01
		int nCType=MGDEB01GetTypeOfTime(pProcTimes, m_nProcs, 
			m_tAbsMapp.m_nTau[1]);		
		printf("- closed");
		if (nCType==2) printf (" [NC]");
		if (nCType==1) 
			printf (" [T]");
		#endif
		}
	// -------------------------------------------------------------------
	delete [] pProcTimes;
	#ifdef MGDEB01
	printf("\n");
	#endif	
	}
return(true);
}
// =======================================================================
BOOL CMapGen::GO12UpdateMapRef(CMapGen::tagMGMAPPING tMapp, 
							   double dProcTimes)
{
// -SERGY-----------------------------------------------------------------
// Description: Updates referenced mapping with given mapping
//   Parametrs: tMapp	- mapping
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i, j;
// -----------------------------------------------------------------------
if(m_tMapRef.m_bInited)
	{
	DeleteMapping(&m_tMapRef);		// (28.05.2002)	
	}
// -----------------------------------------------------------------------
m_tStat.m_nMapCntOpt++;		// updates number of optimal mappings
// -----------------------------------------------------------------------
m_tMapRef.m_bInited=true;
m_tMapRef.m_nPMs=tMapp.m_nPMs;
m_tMapRef.m_nProcs=tMapp.m_nProcs;
m_tMapRef.m_pPMs=new CFileMPN::tagMPNPMMEM[m_tMapRef.m_nPMs];
// -----------------------------------------------------------------------
m_tMapRef.m_tID.m_nMagNum=tMapp.m_tID.m_nMagNum;	// (27.05.2002)
m_tMapRef.m_tID.m_nPMs=tMapp.m_tID.m_nPMs;
m_tMapRef.m_tID.m_nTasks=tMapp.m_tID.m_nTasks;
// -----------------------------------------------------------------------
m_tStat.m_nOptMapImitNum=m_tStat.m_nTotalImits;
m_tStat.m_nOptMapNum=m_tStat.m_nMapCnt;
m_tStat.m_nOptMapBranch=m_tStat.m_nClosedBranches+1;
// -----------------------------------------------------------------------
for (i=0; i<tMapp.m_nPMs; i++)
	{
	m_tMapRef.m_pPMs[i].m_bStatusOk=tMapp.m_pPMs[i].m_bStatusOk;
	m_tMapRef.m_pPMs[i].m_bBusy=tMapp.m_pPMs[i].m_bBusy;	
	m_tMapRef.m_pPMs[i].m_nID=tMapp.m_pPMs[i].m_nID;
	m_tMapRef.m_pPMs[i].m_nTasks=tMapp.m_pPMs[i].m_nTasks;
	m_tMapRef.m_pPMs[i].m_nCurTaskID=tMapp.m_pPMs[i].m_nCurTaskID;
	m_tMapRef.m_pPMs[i].m_nTTFcur=tMapp.m_pPMs[i].m_nTTFcur;
	m_tMapRef.m_pPMs[i].m_pTaskIDs=new int[m_tMapRef.m_pPMs[i].m_nTasks];
	strcpy(m_tMapRef.m_pPMs[i].m_sName, tMapp.m_pPMs[i].m_sName);
	for(j=0; j<m_tMapRef.m_pPMs[i].m_nTasks; j++)
		m_tMapRef.m_pPMs[i].m_pTaskIDs[j]=tMapp.m_pPMs[i].m_pTaskIDs[j];
	}
// -----------------------------------------------------------------------
m_dRefProcTime=dProcTimes;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::GO13IsMappingTimeOptimal(double dProcTimes)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if given time mapping is optimal
//   Parametrs: pProcTimes	- processes cycles times
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
if (dProcTimes<m_dRefProcTime) return(true);
// -----------------------------------------------------------------------
// Checks for exceeding
// -----------------------------------------------------------------------
/*
int i;
for (i=0; i<m_nProcs; i++)	
	if (pProcTimes[i].m_dCycleTime>m_pRefProcTimes[i].m_dCycleTime &&
		m_pRefProcTimes[i].m_dCycleTime>0)
		return(false);
// -----------------------------------------------------------------------
// Checks for optimum
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	if (pProcTimes[i].m_dCycleTime<m_pRefProcTimes[i].m_dCycleTime &&
		pProcTimes[i].m_dCycleTime>0)
		return(true)
//*/
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CMapGen::GO14CheckFinalMapping(CMapGen::tagBBPROC *pBranch, int nPMs, 
									int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: Checks final mapping and 
//   Parametrs: pBranch	- ptr to researched branch
//				nPMs	- total number of PMs
//				pPMIDs	- array of PM's IDs
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
BOOL bOptimal, bDubMapp, bTimesOk;
double dProcTimes;
proctime* pProcTimes;
mapping tMapp;
// -----------------------------------------------------------------------
#ifdef MGDEB01
//printf("%s ", MGDEB01GetBranchPath(pBranch));
printf("%s ", MGDEB01GetBranchMapping(pBranch, nPMs, pPMIDs));
#endif
// -----------------------------------------------------------------------
tMapp=GOGetMappingForBranch(pBranch, nPMs, pPMIDs);
if (!tMapp.m_bInited) return(false);
// -----------------------------------------------------------------------
bDubMapp=IsMappingChecked(tMapp);
if (bDubMapp)
	{
#ifdef MGDEB01
printf("- DUB\n");
#endif
	GOCloseBranch(pBranch);
	DeleteMapping(&tMapp);
	return(true);
	}
// -----------------------------------------------------------------------
AddToHistory(tMapp.m_tID);
pProcTimes=GOGetTimeFromImitation(tMapp);
bTimesOk=GOCheckMinTimes(pProcTimes, m_nProcs, m_tAbsMapp.m_nTau[1]);
if (!bTimesOk)
	{
	// -------------------------------------------------------------------
#ifdef MGDEB01
int nCType=MGDEB01GetTypeOfTime(pProcTimes, m_nProcs, m_tAbsMapp.m_nTau[1]);
printf("- closed");
if (nCType==2) printf (" [NC]");
if (nCType==1) 
	printf (" [T]");
printf("\n");		
#endif
	// -------------------------------------------------------------------
	GOCloseBranch(pBranch);
	DeleteMapping(&tMapp);
	delete [] pProcTimes;
	return(false);
	}
// -----------------------------------------------------------------------
#ifdef MGDEB01
printf("- FOUND\n"
	   "Is it optimal    = ");
#endif
// -----------------------------------------------------------------------
dProcTimes=GetASLDOfTime(pProcTimes); delete [] pProcTimes;
m_tStat.m_nMapCnt++;
// -----------------------------------------------------------------------
if (!m_tMapRef.m_bInited) GO12UpdateMapRef(tMapp, dProcTimes);
bOptimal=GO13IsMappingTimeOptimal(dProcTimes);
if (bOptimal) GO12UpdateMapRef(tMapp, dProcTimes);
// -----------------------------------------------------------------------
GOCloseBranch(pBranch);
DeleteMapping(&tMapp);
// -----------------------------------------------------------------------
#ifdef MGDEB01
printf("%s\n",bOptimal?"YES":"no");
MGDEB01PrintStat();
#endif
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::GOAnalyseBranch(CMapGen::tagBBPROC *pBranch, int nPMs, 
							  int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: Analyses given opened branch
//   Parametrs: pBranch	- ptr to researched branch
//				nPMs	- total number of PMs
//				pPMIDs	- array of PM's IDs
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
int i, nFreePMs;
int nUnaProcs;
int* pFreePMIDs;
// -----------------------------------------------------------------------
// Step 11. Analyses completion of allocation
// -----------------------------------------------------------------------
nUnaProcs=GOGetNumOfUnaProcsForBranch(pBranch);
if (nUnaProcs==MG_ERROR) return(false);
// -----------------------------------------------------------------------
// Allocation is completed
// -----------------------------------------------------------------------
if (nUnaProcs==0)
	{ GO14CheckFinalMapping(pBranch, nPMs, pPMIDs); return(true); }
// -----------------------------------------------------------------------
nFreePMs=GO05GetNumOfUsablePMs(pBranch, nPMs, pPMIDs);
if (nFreePMs==MG_ERROR) return(false);
if (nFreePMs==0) return(true);
pFreePMIDs=GO05GetIDsOfUsablePMs(pBranch, nPMs, pPMIDs);
// -----------------------------------------------------------------------
// Allocates one of unallocated process at current PM
// -----------------------------------------------------------------------
pBranch->m_nChilds=nFreePMs;
pBranch->m_pNodes=new bbpm[nFreePMs];
// -----------------------------------------------------------------------
for (i=0; i<nFreePMs; i++)
	{
	pBranch->m_pNodes[i].m_bClosed=false;
	pBranch->m_pNodes[i].m_nPMID=pFreePMIDs[i];
	pBranch->m_pNodes[i].m_pBranches=NULL;
	pBranch->m_pNodes[i].m_pParent=pBranch;
	}
// -----------------------------------------------------------------------
for (i=0; i<nFreePMs; i++)
	{
	bRes=GOAnalyseNode(&pBranch->m_pNodes[i], nPMs, pPMIDs);
	if (!bRes) return(false);
	}
// -----------------------------------------------------------------------
// Closes all opened nodes (27.05.2002)
// -----------------------------------------------------------------------
for (i=0; i<nFreePMs; i++)	GOCloseNode(&pBranch->m_pNodes[i]);
return(true);
}
// =======================================================================
BOOL CMapGen::GOAnalyseNode(CMapGen::tagBBPM *pNode, int nPMs, int* pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: Analyses given opened node
//   Parametrs: pNode		- ptr to researched node
//				nTotalProcs	- total number of processes
//				pProcIDs	- array of processes' IDs
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 23.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
if (pNode->m_bClosed) return(false);
// -----------------------------------------------------------------------
BOOL bRes;
int i, nUnaProcs;
int* pUnaProcIDs;	//	IDs
// -----------------------------------------------------------------------
// Step 06. Gets list of unallocated processes
// -----------------------------------------------------------------------
nUnaProcs=GO06GetNumOfUnallocProcsForNode(pNode);
if (nUnaProcs==MG_ERROR) {strcpy(m_sErrMsg, "can't get number of unallocated process"); return(false);}
if (nUnaProcs==0) return(false);
pUnaProcIDs=GO06GetIDsOfUnallocProcsForNode(pNode);
// -----------------------------------------------------------------------
// Allocates one of unallocated process at current PM
// -----------------------------------------------------------------------
pNode->m_nChilds=nUnaProcs;
pNode->m_pBranches=new bbproc[nUnaProcs];
// -----------------------------------------------------------------------
for (i=0; i<nUnaProcs; i++)
	{
	pNode->m_pBranches[i].m_bClosed=false;
	pNode->m_pBranches[i].m_dTimeMax=MG_ERROR;
	pNode->m_pBranches[i].m_dTimeMin=MG_ERROR;
	pNode->m_pBranches[i].m_nChilds=0;
	pNode->m_pBranches[i].m_nProcID=pUnaProcIDs[i];
	pNode->m_pBranches[i].m_pParent=pNode;
	pNode->m_pBranches[i].m_pNodes=NULL;
	}
// -----------------------------------------------------------------------
// Step 09. Check cycles times for test allocations & closes branches
// -----------------------------------------------------------------------
if (nUnaProcs>1)
	{
	bRes=GO09CloseExceedBranches(pNode, nPMs, pPMIDs);
	if (!bRes) return(false);
	}
// -----------------------------------------------------------------------
// Checks opened branches
// -----------------------------------------------------------------------
for (i=0; i<nUnaProcs; i++)
	if (!pNode->m_pBranches[i].m_bClosed)
		{
		bRes=GOAnalyseBranch(&pNode->m_pBranches[i], nPMs, pPMIDs);
		if (!bRes) return(false);
		}
// -----------------------------------------------------------------------
// Closes all opened branches (27.05.2002)
// -----------------------------------------------------------------------
for (i=0; i<nUnaProcs; i++)	GOCloseBranch(&pNode->m_pBranches[i]);
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CMapGen::GOCheckMinTimes(CMapGen::tagMGPROCTIME *pProcTimes,int nSize, 
							  int nTau)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if no time exceeding in given times array
//   Parametrs: pPrTimes	- array with cycles times
//				nSize		- size of array
//				nTau		- time limit
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 23.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
for (int i=0; i<nSize; i++)	
	if (pProcTimes[i].m_dCycleTime>nTau||
		pProcTimes[i].m_dCycleTime==MG_ERROR)
		return(false);
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CMapGen::GOCloseBranch(CMapGen::tagBBPROC *pBranch)
{
// -SERGY-----------------------------------------------------------------
// Description: closes given branch
//   Parametrs: pBranch		- branch
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
if (pBranch->m_bClosed) return(true);
// -----------------------------------------------------------------------
pBranch->m_bClosed=true;
m_tStat.m_nClosedBranches++;
if (pBranch->m_nChilds) delete [] pBranch->m_pNodes;
return(true);
}
// =======================================================================
BOOL CMapGen::GOCloseNode(CMapGen::tagBBPM *pNode)
{
// -SERGY-----------------------------------------------------------------
// Description: closes given node
//   Parametrs: pNode		- node
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
if (pNode->m_bClosed) return(true);
// -----------------------------------------------------------------------
pNode->m_bClosed=true;
if (pNode->m_nChilds) delete [] pNode->m_pBranches;
return(true);
}
// =======================================================================
int* CMapGen::GOGetIDsOfAllocTasksByPMID(CMapGen::tagBBPROC *pBranch, 
										 int nPMID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns array of IDs of tasks are already allocated on 
//				given PM
//   Parametrs: pBranch		- branch
//				nPMID		- ID of PM
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
int i, j, k;
int nAllocTasks;
int* pAllocTaskIDs;
// -----------------------------------------------------------------------
nAllocTasks=GOGetNumOfAllocTasksByPMID(pBranch, nPMID);
if (nAllocTasks==0 || nAllocTasks==MG_ERROR) return (NULL);
// -----------------------------------------------------------------------
pAllocTaskIDs=new int[nAllocTasks];
k=0;
for (i=0; i<m_nProcs; i++)
	if (GOIsProcAllocAtPMFromBranch(pBranch, m_pProcs[i].m_nID, nPMID))
		for (j=0; j<m_pProcs[i].m_nResources; j++)
			if (IsRsrcATask(m_pProcs[i].m_pRsrcIDs[j]))
				pAllocTaskIDs[k++]=m_pProcs[i].m_pRsrcIDs[j];
// -----------------------------------------------------------------------
return(pAllocTaskIDs);
}
// =======================================================================
int* CMapGen::GOGetIDsOfUnaProcsForBranch(CMapGen::tagBBPROC *pBranch)
{
// -SERGY-----------------------------------------------------------------
// Description: returns array of IDs of unallocated processes for given 
//				branch
//   Parametrs: pBranch	- ptr to branch
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
int i, j, nUnaProcs;
int *pUnaProcIDs;
// -----------------------------------------------------------------------
nUnaProcs=GOGetNumOfUnaProcsForBranch(pBranch);
if (nUnaProcs==0 || nUnaProcs==MG_ERROR) return(NULL);
// -----------------------------------------------------------------------
pUnaProcIDs=new int[nUnaProcs];
j=0;
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	if (!GOIsProcAllocFromBranch(pBranch, m_pProcs[i].m_nID))
		pUnaProcIDs[j++]=m_pProcs[i].m_nID;	
// -----------------------------------------------------------------------
return(pUnaProcIDs);
}
// =======================================================================
CMapGen::tagMGMAPPING CMapGen::GOGetMappingForBranch(CMapGen::tagBBPROC *pBranch,
													 int nPMs, int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: returns mapping for given branch
//   Parametrs: pBranch		- branch
//				nPMs		- number of PMs
//				pPMIDs		- ID's of PMs
//      Return: inited mapping - success
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
int i, k;
mapping tMapp;
// -----------------------------------------------------------------------
tMapp.m_bInited=true;
tMapp.m_nPMs=GOGetNumOfUsedPMFromBranch(pBranch, nPMs, pPMIDs);
tMapp.m_nProcs=m_nProcs;
tMapp.m_pPMs=new CFileMPN::tagMPNPMMEM[tMapp.m_nPMs];
// -----------------------------------------------------------------------
// Generates mapping for allocated process (real mapping)
// -----------------------------------------------------------------------
k=0;
for (i=0; i<nPMs; i++)
	if (GOIsPMIDUsedFromBranch(pBranch, pPMIDs[i]))
		{		
		tMapp.m_pPMs[k].m_bStatusOk=true;
		tMapp.m_pPMs[k].m_bBusy=false;
		tMapp.m_pPMs[k].m_nID=pPMIDs[i];
		tMapp.m_pPMs[k].m_nCurTaskID=0;
		tMapp.m_pPMs[k].m_nTTFcur=0;
		tMapp.m_pPMs[k].m_nTasks=GOGetNumOfAllocTasksByPMID(pBranch,
			tMapp.m_pPMs[k].m_nID);
		tMapp.m_pPMs[k].m_pTaskIDs=GOGetIDsOfAllocTasksByPMID(pBranch,
			tMapp.m_pPMs[k].m_nID);
		sprintf(tMapp.m_pPMs[k].m_sName, "pm%d", i+1);
		k++;
		}
// -----------------------------------------------------------------------
tMapp.m_tID=GetMapIDByMapping(tMapp);
// -----------------------------------------------------------------------
return(tMapp);
}
// =======================================================================
int CMapGen::GOGetNumOfAllocProcsByPMID(CMapGen::tagBBPROC *pBranch, 
										int nPMID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of processes are already allocated on 
//				given PM
//   Parametrs: pBranch		- branch
//				nPMID		- ID of PM
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 25.05.2002
// -----------------------------------------------------------------------
int i;
int nAllocProcs=0;
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)	
	if (GOIsProcAllocAtPMFromBranch(pBranch, m_pProcs[i].m_nID, nPMID))
		nAllocProcs++;
// -----------------------------------------------------------------------
return(nAllocProcs);
}
// =======================================================================
int CMapGen::GOGetNumOfAllocTasksByPMID(CMapGen::tagBBPROC *pBranch, 
										int nPMID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of tasks are already allocated on given PM
//   Parametrs: pBranch		- branch
//				nPMID		- ID of PM
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
int i;
int nAllocTasks=0;
// -----------------------------------------------------------------------
for (i=0; i<m_nProcs; i++)
	if (GOIsProcAllocAtPMFromBranch(pBranch, m_pProcs[i].m_nID, nPMID))
		nAllocTasks+=GetNumOfTasksByProcID(m_pProcs[i].m_nID);
// -----------------------------------------------------------------------
return(nAllocTasks);
}
// =======================================================================
int CMapGen::GOGetNumOfPMUsing(CMapGen::tagBBPROC *pBranch, int nPMID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of times which given PM has been used
//   Parametrs: pBranch		- number of processes
//				nPMID		- ID of PM
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
int nPMUsing=0;
bbproc* pTemp=pBranch;
// -----------------------------------------------------------------------
while (pTemp->m_pParent!=NULL)
	{
	if (pTemp->m_pParent->m_nPMID==nPMID) nPMUsing++;
	pTemp=pTemp->m_pParent->m_pParent;
	};
// -----------------------------------------------------------------------
return(nPMUsing);
}
// =======================================================================
int CMapGen::GOGetNumOfUnaProcsForBranch(CMapGen::tagBBPROC* pBranch)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of unallocated processes for given node
//   Parametrs: pBranch		- ptr to process
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
int i, nAllocProcs=0;
for (i=0; i<m_nProcs; i++)
	if (GOIsProcAllocFromBranch(pBranch, m_pProcs[i].m_nID))
		nAllocProcs++;
return(m_nProcs-nAllocProcs);
}
// =======================================================================
int CMapGen::GOGetNumOfUsedPMFromBranch(CMapGen::tagBBPROC* pBranch, 
										int nPMs, int* pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of real PM which has been used in mapping
//   Parametrs: pBranch		- current branch
//				nPMs		- number of PMs
//				pPMIDs		- IDs of PMs
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
int i, nUsedPMs=0;
for (i=0; i<nPMs; i++) 
	if (GOIsPMIDUsedFromBranch(pBranch, pPMIDs[i])) nUsedPMs++;
return(nUsedPMs);
// -----------------------------------------------------------------------
}
// =======================================================================
CMapGen::tagMGPROCTIME* CMapGen::GOGetTimeFromImitation(CMapGen::tagMGMAPPING 
													   tMapp)
{
// -SERGY-----------------------------------------------------------------
// Description: returns generated array with cycles time for given mapping
//   Parametrs: mapping	- given mapping
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
int i;
double dCycles, dTimeInCycles;
proctime* pProcTimes;
CPetriNet pn;
// -----------------------------------------------------------------------
m_pMPN->m_nPMs=tMapp.m_nPMs;
m_pMPN->m_pPMs=tMapp.m_pPMs;
// -----------------------------------------------------------------------
pn.Init();
bRes=pn.NewNetFromMPN(m_pMPN); if(!bRes) return(NULL);
bRes=pn.Run(0, MG_STATITER, MG_STATCYCLES); if(!bRes) return(NULL);
// -----------------------------------------------------------------------
pProcTimes=new proctime[tMapp.m_nProcs];
for (i=0; i<tMapp.m_nProcs; i++)
	{
	dCycles=pn.GetStatistics()->m_pProcs[i].m_nCycles;
	dTimeInCycles=pn.GetStatistics()->m_pProcs[i].m_nTimeInCycles;
	pProcTimes[i].m_nID=pn.GetStatistics()->m_pProcs[i].m_nID;
	pProcTimes[i].m_dCycleTime=dCycles?(dTimeInCycles/dCycles):MG_ERROR;
	}
// -----------------------------------------------------------------------
m_tStat.m_nImitIters+=pn.GetNumberOfIters();	// (26.05.2002)
m_tStat.m_nTotalImits++;						// (26.05.2002)
// -----------------------------------------------------------------------
pn.Shutdown(); m_pMPN->m_nPMs=0; // (28.05.2002)
// -----------------------------------------------------------------------
return(pProcTimes);
}
// =======================================================================
CMapGen::tagMGMAPPING CMapGen::GOGetVirtMapping(CMapGen::tagBBPROC*pBranch,
												  int nPMs, int *pPMIDs, 
												  BOOL bMakeBest)
{
// -SERGY-----------------------------------------------------------------
// Description: returns best or worst virtual mapping for current branch
//   Parametrs: pBranch		- ptr to researched branch
//				nPMs		- total number of PMs
//				pPMIDs		- array of PM's IDs
//				bMakeBest	- make best mapping (get Tmin)
//      Return: mapping - success
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
int i, j, k, l;
int nUnaProcs;			// number of unallocated processes
int nRealPMs, nVirtPMs;	// number of real and virtual PMs
int* pUnaProcIDs;		// ID's of unallocated processes
int* pVirtPMIDs;		// ptr to virtual PM's IDs
CFileMPN::tagMPNPROCMEM* pProc;
mapping tMapp;
// -----------------------------------------------------------------------
tMapp.m_bInited=false;
// -----------------------------------------------------------------------
nUnaProcs=GOGetNumOfUnaProcsForBranch(pBranch);
if (nUnaProcs==MG_ERROR) {strcpy	(m_sErrMsg,
 	"can't get number of unallocated process for branch"); return(tMapp);}
if (nUnaProcs==0) return(tMapp);
pUnaProcIDs=GOGetIDsOfUnaProcsForBranch(pBranch);
// -----------------------------------------------------------------------
pVirtPMIDs=GetFreeIDs(nUnaProcs, nPMs, pPMIDs);	// Virtual PM's IDs
nRealPMs=GOGetNumOfUsedPMFromBranch(pBranch, nPMs, pPMIDs);
nVirtPMs=nUnaProcs;
// -----------------------------------------------------------------------
// Generates mapping (25.05.2002)
// -----------------------------------------------------------------------
tMapp.m_bInited=true;
tMapp.m_nPMs=nRealPMs+nVirtPMs;
tMapp.m_nProcs=m_nProcs;
tMapp.m_pPMs=new CFileMPN::tagMPNPMMEM[tMapp.m_nPMs];
// -----------------------------------------------------------------------
// Preinits all PMs
// -----------------------------------------------------------------------
for (i=0; i<tMapp.m_nPMs; i++)
	{
	tMapp.m_pPMs[i].m_bStatusOk=true;
	tMapp.m_pPMs[i].m_bBusy=false;
	tMapp.m_pPMs[i].m_nCurTaskID=0;
	tMapp.m_pPMs[i].m_nTTFcur=0;
	}
// -----------------------------------------------------------------------
// Generates mapping for allocated process (real mapping)
// -----------------------------------------------------------------------
k=0;
for (i=0; i<nPMs; i++)	
	if (GOIsPMIDUsedFromBranch(pBranch, pPMIDs[i]))
		{
		tMapp.m_pPMs[k].m_nID=pPMIDs[i];
		tMapp.m_pPMs[k].m_nTasks=GOGetNumOfAllocTasksByPMID(pBranch,
			tMapp.m_pPMs[k].m_nID);
		tMapp.m_pPMs[k].m_pTaskIDs=GOGetIDsOfAllocTasksByPMID(pBranch,
			tMapp.m_pPMs[k].m_nID);
		sprintf(tMapp.m_pPMs[k].m_sName, "realpm%d", i+1);
		k++;
		}	
// -----------------------------------------------------------------------
// Allocates unallocated processes on different virtual PMs
// -----------------------------------------------------------------------
for (i=nRealPMs; i<tMapp.m_nPMs; i++)
	{
	l=i-nRealPMs; k=0;
	pProc=GetProcByID(pUnaProcIDs[l]);
	tMapp.m_pPMs[i].m_nID=pVirtPMIDs[l];	
	tMapp.m_pPMs[i].m_nTasks=GetNumOfTasksByProcID(pProc->m_nID);
	tMapp.m_pPMs[i].m_pTaskIDs=new int[tMapp.m_pPMs[i].m_nTasks];
	sprintf(tMapp.m_pPMs[i].m_sName, "vpm%d", l+1);
	for (j=0; j<pProc->m_nResources; j++)
		if (IsRsrcATask(pProc->m_pRsrcIDs[j]))
			tMapp.m_pPMs[i].m_pTaskIDs[k++]=pProc->m_pRsrcIDs[j];
	}
// -----------------------------------------------------------------------
return(tMapp);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CMapGen::GOIsProcAllocFromBranch(CMapGen::tagBBPROC* pBranch, 
										  int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if given process is already allocated
//   Parametrs: pBranch		- ptr to branch
//				nProcID		- ID of process
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
bbproc* pTemp=pBranch;
// -----------------------------------------------------------------------
while (pTemp->m_pParent!=NULL)
	{
	if (pTemp->m_nProcID==nProcID) return(true);
	pTemp=pTemp->m_pParent->m_pParent;
	};
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CMapGen::GOIsProcAllocFromNode(CMapGen::tagBBPM *pNode, int nProcID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if given process is already allocated
//   Parametrs: pNode		- ptr to node
//				nProcID		- ID of process
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 24.05.2002
// -----------------------------------------------------------------------
bbpm* pTemp=pNode;
// -----------------------------------------------------------------------
while (pTemp->m_pParent->m_pParent!=NULL)
	{
	if (pTemp->m_pParent->m_nProcID==nProcID) return(true);
	pTemp=pTemp->m_pParent->m_pParent;
	};
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CMapGen::GOIsPMIDUsedFromBranch(CMapGen::tagBBPROC *pBranch, int nPMID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if PM with given is already used in mapping
//   Parametrs: pBranch		- ptr to the branch
//				nPMID		- ID of PM
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
bbproc* pTemp=pBranch;
// -----------------------------------------------------------------------
while (pTemp->m_pParent!=NULL)
	{
	if (pTemp->m_pParent->m_nPMID==nPMID)
		return(true);
	pTemp=pTemp->m_pParent->m_pParent;
	};
// -----------------------------------------------------------------------
return(false);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CMapGen::GOIsProcAllocAtPMFromBranch(CMapGen::tagBBPROC *pBranch,
										  int nProcID, int nPMID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if given process is already allocated at 
//				given PM 
//   Parametrs: pBranch		- ptr to the branch
//				nProcID		- ID of process
//				nPMID		- ID of PM
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 25.05.2002
//			LE: 25.05.2002
// -----------------------------------------------------------------------
bbproc* pTemp=pBranch;
// -----------------------------------------------------------------------
while (pTemp->m_pParent!=NULL)
	{
	if (pTemp->m_nProcID==nProcID && pTemp->m_pParent->m_nPMID==nPMID)
		return(true);
	pTemp=pTemp->m_pParent->m_pParent;
	};
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CMapGen::GOStart(int nPMs, int *pPMIDs)
{
// -SERGY-----------------------------------------------------------------
// Description: starts algorithm of global optimisation
//   Parametrs: nProcs		- number of processes to allocate
//				pProcsIDs	- array of processes' IDs
//				nPMs		- number of PMs
//				pPMIDs		- array of PM's IDs
//      Return: true - success, null - error
// -----------------------------------------------------------------------
//			MD: 24.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
int i;
//int nFaultPMs, nNFPMs;
int nNFPMs;
// -----------------------------------------------------------------------
// Step 02. Gets maximal number of fault PMs
// -----------------------------------------------------------------------
m_tAbsMapp.m_nFaultPMs=GO02GetMaxNumOfFaultPMs(nPMs);
if (m_tAbsMapp.m_nFaultPMs==0)
	{strcpy(m_sErrMsg,"network can't have fault-tolerance");return(false);}
// -----------------------------------------------------------------------
// Step 03. Creating ROOT elem
// -----------------------------------------------------------------------
nNFPMs=nPMs-m_tAbsMapp.m_nFaultPMs;
for (i=nNFPMs; i<nPMs; i++)
	{
	#ifdef MGDEB01
	MGDEB01PrintConstrains(i, m_tAbsMapp.m_nFaultPMs);
	#endif
	bRes=GO025GetMapping(i, pPMIDs);
	if (m_tMapRef.m_bInited) 
		GO16AcceptMapRef();
	// -------------------------------------------------------------------
	#ifdef MGDEB01
	MGDEB01PrintConstrains(i, m_tAbsMapp.m_nFaultPMs);
	MGDEB01PrintStat();
	#endif
	// -------------------------------------------------------------------
	}
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CMapGen::InitResources()
{
// -SERGY-----------------------------------------------------------------
// Description: inits resources
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
m_tRsrcs.m_nPlaces=0;
m_tRsrcs.m_nTrans=m_pMPN->m_nPNtransitions;
m_tRsrcs.m_nConns=0;
m_tRsrcs.m_nPMs=0;
m_tRsrcs.m_nProcs=0;
// -----------------------------------------------------------------------
m_tRsrcs.m_pLPL=NULL;
m_tRsrcs.m_pLTR=new CPetriNet::tagRSRCTR[m_tRsrcs.m_nTrans];
m_tRsrcs.m_pLPM=NULL;
// -----------------------------------------------------------------------
for (i=0; i<m_tRsrcs.m_nTrans; i++)
	{
	m_tRsrcs.m_pLTR[i].m_nID=m_pMPN->m_pTrans[i].m_nID;
	m_tRsrcs.m_pLTR[i].m_nTTF=m_pMPN->m_pTrans[i].m_nTTFmax;
	m_tRsrcs.m_pLTR[i].m_nProbab=m_pMPN->m_pTrans[i].m_nProbab;
	strcpy(m_tRsrcs.m_pLTR[i].m_sName, m_pMPN->m_pTrans[i].m_sName);
	}
// -----------------------------------------------------------------------
m_tRsrcs.m_bInited=true; return(true);
}
// =======================================================================
BOOL CMapGen::IsMappingChecked(CMapGen::tagMGMAPPING tMapp)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if given mapping is already checked
//   Parametrs: tMapp	- mapping
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
for (int i=0; i<m_nHistSize; i++)	
	if (m_pHistory[i].m_nMagNum==tMapp.m_tID.m_nMagNum &&
		m_pHistory[i].m_nPMs==tMapp.m_tID.m_nPMs &&
		m_pHistory[i].m_nTasks==tMapp.m_tID.m_nTasks)
		return(true);
// -----------------------------------------------------------------------
return(false);
}
// =======================================================================
BOOL CMapGen::IsRsrcATask(int nRsrcID)
{
// -SERGY-----------------------------------------------------------------
// Description: returns 'true' if resource with given ID is a task
//   Parametrs: nRsrcID	- resource's ID
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
for (int i=0; i<m_pMPN->m_nPNtransitions; i++)
	if (m_pMPN->m_pTrans[i].m_nID==nRsrcID) return(true);
return(false);
}
// =======================================================================
#ifdef MGDEB01
char* CMapGen::MGDEB01GetBranchMapping(CMapGen::tagBBPROC *pBranch, 
									   int nPMs, int* pPMIDs)
{
// -----------------------------------------------------------------------
//       DEBUG:
// -SERGY-----------------------------------------------------------------
// Description: get text mapping for the current branch
//   Parametrs: pBranch		- ptr to current branch
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
char sTemp[256];
static char sMapp[1024];	// current path
// -----------------------------------------------------------------------
int i, j;
*sMapp=0;
// -----------------------------------------------------------------------
for (i=0; i<nPMs; i++)
	{
	if (GOIsPMIDUsedFromBranch(pBranch, pPMIDs[i]))
		{		
		//strcat(sMapp, "\n");
		sprintf(sTemp, "pmID=%d: ", pPMIDs[i]);
		strcat(sMapp, sTemp);
		for (j=0; j<m_nProcs; j++)			
			if (GOIsProcAllocAtPMFromBranch(pBranch, m_pProcs[j].m_nID, 
				pPMIDs[i]))
				{
				sprintf(sTemp, "%d, ", m_pProcs[j].m_nID);
				strcat(sMapp, sTemp);
				}
		}
	}
// -----------------------------------------------------------------------
return(sMapp);
}
// =======================================================================
char* CMapGen::MGDEB01GetBranchPath(CMapGen::tagBBPROC *pBranch)
{
// -----------------------------------------------------------------------
//       DEBUG:
// -SERGY-----------------------------------------------------------------
// Description: get text path of the current branch
//   Parametrs: pBranch		- ptr to current branch
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
char sTemp[256];
static char sPath[1024];	// current path
// -----------------------------------------------------------------------
int i;
int nCurLevel, nLevel=0;
int nParPMID=MG_ERROR;		// parent's PM ID
bbproc* pTemp=pBranch;
// -----------------------------------------------------------------------
while (pTemp->m_pParent!=NULL) 
	{nLevel++; pTemp=pTemp->m_pParent->m_pParent; };
// -----------------------------------------------------------------------
sprintf(sPath, "ROOT");
for (i=1; i<=nLevel; i++)
	{
	nCurLevel=nLevel; pTemp=pBranch;
	while (nCurLevel!=i)
		{ nCurLevel--; pTemp=pTemp->m_pParent->m_pParent; }
	if (nParPMID==pTemp->m_pParent->m_nPMID)
		sprintf(sTemp, ", %d", pTemp->m_nProcID);
	else
		sprintf(sTemp, " -> pmID=%d: %d", pTemp->m_pParent->m_nPMID,
				pTemp->m_nProcID);		
	strcat(sPath, sTemp);
	nParPMID=pTemp->m_pParent->m_nPMID;
	}
// -----------------------------------------------------------------------
return(sPath);
}
// =======================================================================
int CMapGen::MGDEB01GetTypeOfTime(CMapGen::tagMGPROCTIME*pProcTimes, 
								  int nSize, int nTau)
{
// -SERGY-----------------------------------------------------------------
// Description: returns type of process time limit
//   Parametrs: pProcTime	- time array
//				nSize		- size of time array
//				nTau		- time limit
//      Return: (0) - ok, 
//				(1) - time limit
//				(2) - cycle error (time of a cycle is equal to -1)
// -----------------------------------------------------------------------
//			MD: 04.06.2002
//			LE: 04.06.2002
// -----------------------------------------------------------------------
for (int i=0; i<nSize; i++)
	{
	if(pProcTimes[i].m_dCycleTime>nTau) return (1);
	if(pProcTimes[i].m_dCycleTime==MG_ERROR) return(2);
	}
return(0);
}
// =======================================================================
BOOL CMapGen::MGDEB01PrintConstrains(int nPMs, int nMaxFPMs)
{
// -SERGY-----------------------------------------------------------------
// Description: print initial constrains
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
printf("\n"
	   "Q*                = %10.7lf, Tau0 = %d, Tau1 = %d\n"
	   "Pfault            = %10.7lf\n"
	   "MAX num. of fault PMs = %d\n"
	   "Processes         = %d\n"
	   "PMs               = %d\n"
	   "Closed branches   = %d\n"
	   "DUB branches      = %d (included in closed branches)\n"
	   "Accepted mappings = %d\n",
	   m_tAbsMapp.m_dProbSurv, m_tAbsMapp.m_nTau[0], 
	   m_tAbsMapp.m_nTau[1], m_tAbsMapp.m_dProbFault,
	   nMaxFPMs, m_nProcs, nPMs, m_tStat.m_nClosedBranches, m_nHistSize,
	   m_tAbsMapp.m_nMappings);
return(true);
}
// =======================================================================
BOOL CMapGen::MGDEB01PrintStat()
{
// -SERGY-----------------------------------------------------------------
// Description: print some statistics at the screen
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 26.05.2002
//			LE: 26.05.2002
// -----------------------------------------------------------------------
printf("\n"
	   "Total imitations = %d [%d iterations]\n"
	   "Closed branches  = %d\n"
	   "Total mappings   = %d\n"
	   "Optimal mappings = %d [ID = %d.%d.%d]\n"
	   "Found at branch# = %d, mapp# = %d, imit# = %d\n\n",
	   m_tStat.m_nTotalImits,
	   m_tStat.m_nImitIters,
	   m_tStat.m_nClosedBranches,
	   m_tStat.m_nMapCnt,
	   m_tStat.m_nMapCntOpt,
	   m_tMapRef.m_tID.m_nMagNum,
	   m_tMapRef.m_tID.m_nPMs,
	   m_tMapRef.m_tID.m_nTasks, m_tStat.m_nOptMapBranch, 
	   m_tStat.m_nOptMapNum, m_tStat.m_nOptMapImitNum);
return(true);
}
#endif
// =======================================================================
double CMapGen::MTHCombs(int nL, int nN)
{
// -SERGY-----------------------------------------------------------------
// Description: returns number of combinations of nL and nN
//   Parametrs: nL, nN	- integer numbers
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
int nNF, nLF, nNLF;
double nCombs;
// -----------------------------------------------------------------------
nNF=MTHFact(nN); nLF=MTHFact(nL); nNLF=MTHFact(nN-nL);
nCombs=(double)nNF/(double)(nLF*nNLF);
// -----------------------------------------------------------------------
return(nCombs);
}
// =======================================================================
int CMapGen::MTHFact(int nN)
{
// -SERGY-----------------------------------------------------------------
// Description: returns factorial of given number
//   Parametrs: nN	- integer number
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
if (nN<0) return(MG_ERROR);
if (nN==0) return(1);
// -----------------------------------------------------------------------
int i, nFact=nN;
for (i=1; i<nN; i++)	
	nFact=nFact*(nN-i);	
// -----------------------------------------------------------------------
return(nFact);
// -----------------------------------------------------------------------
}
// =======================================================================
double CMapGen::MTHPowDN(double dNumber, int nPow)
{
// -SERGY-----------------------------------------------------------------
// Description: returns double 'dNumber' in integer power 'nPow'
//   Parametrs: dNumber	- given number
//				nPow	- integer power
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 23.05.2002
//			LE: 23.05.2002
// -----------------------------------------------------------------------
if (nPow==0) return(1);
if (nPow<0) return(MG_ERROR);
if (dNumber==0) return(1);
// -----------------------------------------------------------------------
double dRes=1; for (int i=0; i<nPow; i++) dRes*=dNumber; return(dRes);
// -----------------------------------------------------------------------
}
// =======================================================================
double CMapGen::MTHSurvProb(int nK, int nN)
{
// -SERGY-----------------------------------------------------------------
// Description: returns surviving probability with given parametrs
//   Parametrs: nN	- integer number
//      Return: num - success, MG_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 22.05.2002
//			LE: 22.05.2002
// -----------------------------------------------------------------------
double dP=m_tAbsMapp.m_dProbFault;
double dAntiP=(1-m_tAbsMapp.m_dProbFault);
double dCKN=MTHCombs(nK, nN);
double dRes;
// -----------------------------------------------------------------------
dRes=dCKN*MTHPowDN(dP, nK)*MTHPowDN(dAntiP, nN-nK);
// -----------------------------------------------------------------------
return(dRes);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CMapGen::ResetResources()
{
// -SERGY-----------------------------------------------------------------
// Description: reset resources structure
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
BOOL CMapGen::Restart()
{
// -SERGY-----------------------------------------------------------------
// Description: prepares Mapping Generator for restart
//   Parametrs: <none>
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
if (m_nHistSize) { delete [] m_pHistory; }
if (m_tMapRef.m_bInited) DeleteMapping(&m_tMapRef);
// -----------------------------------------------------------------------
m_nHistSize=0;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
