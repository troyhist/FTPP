// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Support for protocol history LOG file format.
// Used library: <none>
// -----------------------------------------------------------------------
// File: LOGFile.cpp
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 06/ 05.2002
// Last edit: 19/ 05.2002
//     Build: 1
// =======================================================================
// LOGFile.cpp: implementation of the CFileLOG class.
// =======================================================================
#include "stdafx.h"
#include "FileLOG.h"
// -----------------------------------------------------------------------
#include "AutoBuild.h"					// Sergy/19.05.2002
// =======================================================================
#define LFPN_SP1K	(1024)				// 1Kb
#define LFPN_SPMEMBLK	(64)			// Size of one block in KB
// =======================================================================
// Construction/Destruction
// =======================================================================
CFileLOG::CFileLOG()
{
m_bInited=false;
}
// =======================================================================
CFileLOG::~CFileLOG()
{

}
// =======================================================================
BOOL CFileLOG::SetData(CPetriNet::tagRSRC* pRsrcs,
					   CPetriNet::tagLOG* pHist, 
					   CPetriNet::tagSTATISTICS* pStat,
					   CMapGen::tagMGABSMAPP* pAbsMapp)
{
// -SERGY-----------------------------------------------------------------
// Description: Inits internal history array with given data
//   Parametrs: pHistory	- ptr to history array
//				pStat		- ptr to statistics structure
//				pAbsMapp	- ptr to absolute mapping
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 06.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
if (!pRsrcs) return(false);
if (pHist && (pHist->m_nSize<=0 || !pHist->m_pHistory)) return(false);
// -----------------------------------------------------------------------
m_pRsrcs=NULL;	m_pHist=NULL;	m_pStat=NULL;	m_pAbsMapp=NULL;
// -----------------------------------------------------------------------
m_pRsrcs=pRsrcs;		m_pHist=pHist;	m_pStat=pStat;
m_pAbsMapp=pAbsMapp;	m_bInited=true;	return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
char* CFileLOG::GetRsrcNameByID(int nRsrcID)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns resource's name by given ID
//   Parametrs: nRsrcID		- resource's ID
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 09.05.2002
//			LE: 08.06.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
for (i=0; i<m_pRsrcs->m_nTrans; i++)
	if (m_pRsrcs->m_pLTR[i].m_nID==nRsrcID)
		return(m_pRsrcs->m_pLTR[i].m_sName);
// -----------------------------------------------------------------------
for (i=0; i<m_pRsrcs->m_nProcs; i++)
	if (m_pRsrcs->m_pLPROC[i].m_nID==nRsrcID)
		return(m_pRsrcs->m_pLPROC[i].m_sName);
// -----------------------------------------------------------------------
return(NULL);
}
// =======================================================================
BOOL CFileLOG::SaveProtocol(char *sFilename, BOOL bBinary)
{
// -SERGY-----------------------------------------------------------------
// Description: Saves history protocol into given file
//   Parametrs: sFilename	- filename
//				bBinary		- make binary protocol
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 06.05.2002
//			LE: 06.05.2002
// -----------------------------------------------------------------------
if (!m_bInited) return(false);
BOOL bRes;
if (bBinary)
	bRes=SaveProtocolBIN(sFilename);
else
	bRes=SaveProtocolTXT(sFilename);
return(bRes);
}
// =======================================================================
BOOL CFileLOG::SaveProtocolTXT(char *sFilename)
{
// -SERGY-----------------------------------------------------------------
// Description: Saves history protocol into text file
//   Parametrs: sFilename	- filename
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 06.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
char *sLogInfo=NULL, *sStatistics=NULL, *sProtocol=NULL, *sMapping=NULL;
FILE* fp;
// -----------------------------------------------------------------------
if ((fp=fopen(sFilename, "wt"))==NULL) return(false);
// -----------------------------------------------------------------------
if (m_pHist) if(m_pHist->m_nSize) 
	{ sLogInfo=GetTextInfo(); sProtocol=GetTextProtocol(); }
if (m_pStat) sStatistics=GetTextStat();
if (m_pAbsMapp) sMapping=GetTextMapping();
// -----------------------------------------------------------------------
if (sLogInfo)	fwrite(sLogInfo,	strlen(sLogInfo), 1, fp);
if (sStatistics)fwrite(sStatistics, strlen(sStatistics), 1, fp);
if (sMapping)	fwrite(sMapping,	strlen(sMapping), 1, fp);
if (sProtocol)	fwrite(sProtocol,	strlen(sProtocol), 1, fp);
fclose(fp);
// -----------------------------------------------------------------------
if (sLogInfo)	delete sLogInfo;	if (sStatistics)delete sStatistics;
if (sMapping)	delete sMapping;	if (sProtocol)	delete sProtocol;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
char* CFileLOG::GetTextInfo()
{
// -SERGY-----------------------------------------------------------------
// Description: returns generated text array with log-file info
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
char *out;
out=new char[LFPN_SP1K*LFPN_SPMEMBLK]; if (!out) return(false);
// -----------------------------------------------------------------------
int nSize	=m_pHist?m_pHist->m_nSize:CFL_ERROR;
int nPlaces	=m_pRsrcs->m_nPlaces;
int nTrans	=m_pRsrcs->m_nTrans;
int nConns	=m_pRsrcs->m_nConns;	// (18.05.2002)
int nPMs	=m_pRsrcs->m_nPMs;
int nProcs	=m_pRsrcs->m_nProcs;	// (16.05.2002)
// -----------------------------------------------------------------------
int nIterFirst=m_pHist?m_pHist->m_pHistory[0][0].m_nCurIterNum:CFL_ERROR;
int nIterLast=m_pHist?m_pHist->m_pHistory[0][nSize-1].m_nCurIterNum:CFL_ERROR;
int nInters=m_pHist?nIterLast-nIterFirst+1:CFL_ERROR;
// -----------------------------------------------------------------------
sprintf(out, 
		"[loginfo]\n"
		"FTPP build #   = %d\n"
		"records        = %d\n"
		"iterations     = %d [%d...%d]\n\n"
		"[netinfo]\n"
		"places      = %d\n"
		"transitions = %d\n"
		"connections = %d\n"
		"PMs         = %d\n"
		"Processes   = %d\n\n",
		AUTO_BUILDNUMBER, nSize, nInters, nIterFirst, nIterLast,
		nPlaces, nTrans, nConns, nPMs, nProcs);
// -----------------------------------------------------------------------
return(out);
}
// =======================================================================
char* CFileLOG::GetTextMapping()
{
// -SERGY-----------------------------------------------------------------
// Description: returns generated text array with mapping
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 28.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i, j, k;
char *out, sTemp[256];
out=new char[LFPN_SP1K*LFPN_SPMEMBLK]; if (!out) return(false);
// -----------------------------------------------------------------------
sprintf(out, 
		"[loginfo]\n"
		"FTPP build #          = %d\n"
		"Survive probability   = %10.7f %%\n"
		"PM's fault probab.    = %10.7f %%\n"
		"Tau0 - normal mode    = %d (iterations/cycle)\n"
		"Tau1 - distorted mode = %d (iterations/cycle)\n\n"
		"[PM info]\n"
		"MAX num. of PMs       = %d\n"
		"MAX num. of fault PMs = %d (according to given Survive probability)\n"
		"total mappings        = %d\n",
		AUTO_BUILDNUMBER, 
		m_pAbsMapp->m_dProbSurv*100,
		m_pAbsMapp->m_dProbFault*100,
		m_pAbsMapp->m_nTau[0], m_pAbsMapp->m_nTau[1],
		m_pAbsMapp->m_nMaxPMs, m_pAbsMapp->m_nFaultPMs,
		m_pAbsMapp->m_nMappings);
// -----------------------------------------------------------------------
for (i=0; i<m_pAbsMapp->m_nMappings; i++)
	{
	// -------------------------------------------------------------------
	if (!m_pAbsMapp->m_pMapps[i].m_bInited) continue;
	sprintf (sTemp, "\n"
		"[mapping =%d]\n"
		"PMs          = %d\n"
		"Procs        = %d\n"
		"mapping's ID = %d.%d.%d\n\n", i+1,
		m_pAbsMapp->m_pMapps[i].m_nPMs,
		m_pAbsMapp->m_pMapps[i].m_nProcs,
		m_pAbsMapp->m_pMapps[i].m_tID.m_nMagNum,
		m_pAbsMapp->m_pMapps[i].m_tID.m_nPMs,
		m_pAbsMapp->m_pMapps[i].m_tID.m_nTasks);
	strcat(out, sTemp);
	// -------------------------------------------------------------------
	for (j=0; j<m_pAbsMapp->m_pMapps[i].m_nPMs; j++)
		{
		sprintf(sTemp, "PM,%s,%d,%d,%d,",
			m_pAbsMapp->m_pMapps[i].m_pPMs[j].m_sName,
			m_pAbsMapp->m_pMapps[i].m_pPMs[j].m_bBusy,
			m_pAbsMapp->m_pMapps[i].m_pPMs[j].m_nCurTaskID,
			m_pAbsMapp->m_pMapps[i].m_pPMs[j].m_nTTFcur);
		strcat(out, sTemp);
		// ---------------------------------------------------------------
		for (k=0; k<m_pAbsMapp->m_pMapps[i].m_pPMs[j].m_nTasks; k++)
			{
			sprintf(sTemp, "%s,", GetRsrcNameByID(
				m_pAbsMapp->m_pMapps[i].m_pPMs[j].m_pTaskIDs[k]));
			strcat(out, sTemp);
			}
		// ---------------------------------------------------------------
		strcat(out, "\n");
		}
	// -------------------------------------------------------------------
	}
// -----------------------------------------------------------------------
return(out);
}
// =======================================================================
char* CFileLOG::GetTextMarkingType(int nRecNum)
{
// -SERGY-----------------------------------------------------------------
// Description: Returns text description of the marking type
//   Parametrs: nRecNum	- history record number
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 07.05.2002
//			LE: 07.05.2002
// -----------------------------------------------------------------------
static char sRes[16];
// -----------------------------------------------------------------------
int nMarkType;
int nMarkRef;		// referenced record number
int nRefIter;		// referenced record iteration
// -----------------------------------------------------------------------
// always use m_pHistory[1][...] sub array to get Marking properties
// -----------------------------------------------------------------------
nMarkType=m_pHist->m_pHistory[1][nRecNum].m_nMarkType;
// -----------------------------------------------------------------------
switch(nMarkType)
	{
	case PN_MARKTERM: strcpy(sRes, "TERM"); break;
	case PN_MARKDUB:	
		nMarkRef=m_pHist->m_pHistory[1][nRecNum].m_nMarkRef;
		nRefIter=m_pHist->m_pHistory[1][nMarkRef].m_nCurIterNum;
		sprintf(sRes, "DUB iter=%d (%d)", nRefIter, nMarkRef);
		break;
	case PN_MARKOVER:
		nMarkRef=m_pHist->m_pHistory[1][nRecNum].m_nMarkRef;
		nRefIter=m_pHist->m_pHistory[1][nMarkRef].m_nCurIterNum;
		sprintf(sRes, "OVER iter=%d (%d)", nRefIter, nMarkRef);
		break;
	case PN_MARKNORM: default: *sRes=0; break;
	}
return(sRes);
}
// =======================================================================
char* CFileLOG::GetTextProtocol()
{
// -SERGY-----------------------------------------------------------------
// Description: returns generated text array with protocol
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
char *out, *swpout;	// Big output array, temp
// -----------------------------------------------------------------------
char temp[256];
char sXT[3][8];		// for TaskName, TTF, TTFmax
char sStat[16];		// for processor status
char* sTNames[3];	// tasks names
int kmar=1;			// memory allocation ratio (19.05.2002)
int allocsize;		// allocated memory size (19.05.2002)
int i, j;
CPetriNet::tagLOGREC *pHR0, *pHR1, *pHR2;	// ptr to one record
// -----------------------------------------------------------------------
int nSize=m_pHist->m_nSize;
int nPlaces=m_pRsrcs->m_nPlaces;
int nPMs=m_pRsrcs->m_nPMs;
if (!nSize) return(false);
// -----------------------------------------------------------------------
// Allocates memory for buffer (19.05.2002)
// -----------------------------------------------------------------------
allocsize=LFPN_SP1K*LFPN_SPMEMBLK*kmar;
out=new char[allocsize]; if (!out) return(NULL);
// -----------------------------------------------------------------------
// Protocol
// -----------------------------------------------------------------------
strcpy(out, "[protocol]\n");
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)
	{
	// -------------------------------------------------------------------
	pHR0=&m_pHist->m_pHistory[0][i];
	pHR1=&m_pHist->m_pHistory[1][i];
	pHR2=&m_pHist->m_pHistory[2][i];
	// -------------------------------------------------------------------
	sprintf(temp, "\n[iteration =%d]\n", pHR0->m_nCurIterNum);
	strcat(out, temp);
	// -------------------------------------------------------------------
	for (j=0; j<nPlaces; j++)
		{ sprintf(temp, "%s,\t", m_pRsrcs->m_pLPL[j].m_sName);
		strcat(out, temp); }
	strcat(out, "\n");
	// -------------------------------------------------------------------
	// Marking before FIRE
	// -------------------------------------------------------------------
	for (j=0; j<nPlaces; j++)
		{ sprintf(temp, "%d,\t", pHR0->m_pTokens[j]);
		strcat(out, temp); }	
	strcat(out, "\n");
	// -------------------------------------------------------------------
	// Marking after FIRE, before LOAD
	// -------------------------------------------------------------------
	for (j=0; j<nPlaces; j++)
		{
		if (pHR1->m_pTokens[j]!=pHR0->m_pTokens[j])
			sprintf(temp, "%d,\t", pHR1->m_pTokens[j]);
		else strcpy(temp, "-,\t"); strcat(out, temp); }	
	strcat(out, GetTextMarkingType(i));
	strcat(out, "\n");
	// -------------------------------------------------------------------
	// Marking after LOAD
	// -------------------------------------------------------------------
	for (j=0; j<nPlaces; j++)
		{
		if (pHR2->m_pTokens[j]!=pHR1->m_pTokens[j])
			sprintf(temp, "%d,\t", pHR2->m_pTokens[j]);
		else strcpy(temp, "-,\t"); strcat(out, temp); }	
	strcat(out, "\n\n");
	// -------------------------------------------------------------------
	// Stagnation flag (20.05.2002)
	// -------------------------------------------------------------------
	if (pHR1->m_bPMStagn && pHR2->m_bPMStagn) strcat(out, "* STAGN *\n");
	// -------------------------------------------------------------------
	for (j=0; j<nPMs; j++)
		{
		// ---------------------------------------------------------------
		if (pHR0->m_pPMST[j].m_bBusy)
			sTNames[0]=GetRsrcNameByID(pHR0->m_pPMST[j].m_nCurTaskID);
		if (pHR1->m_pPMST[j].m_bBusy)
			sTNames[1]=GetRsrcNameByID(pHR1->m_pPMST[j].m_nCurTaskID);
		if (pHR2->m_pPMST[j].m_bBusy)
			sTNames[2]=GetRsrcNameByID(pHR2->m_pPMST[j].m_nCurTaskID);
		// ---------------------------------------------------------------		
		strcpy(sXT[0], "-"); strcpy(sXT[1], "-"); strcpy(sXT[2], "-");
		if(pHR2->m_pPMST[j].m_bStatusOk && pHR2->m_pPMST[j].m_bBusy)
			{
			strcpy(sXT[0], sTNames[2]);			
			sprintf(sXT[1], "%d", pHR2->m_pPMST[j].m_nTTFmax);
			sprintf(sXT[2], "%d", pHR2->m_pPMST[j].m_nTTFcur);
			}
		// ---------------------------------------------------------------
		strcpy(sStat, "FAULT");
		if(pHR0->m_pPMST[j].m_bStatusOk)
			{
			if (pHR2->m_pPMST[j].m_bBusy)
				strcpy(sStat, "busy");
			if (!pHR1->m_pPMST[j].m_bBusy && !pHR2->m_pPMST[j].m_bBusy)
				strcpy(sStat, "ok");
			}
		sprintf(temp, "%s, %s,\t%s,\t%s,\t%s,\t", 
			m_pRsrcs->m_pLPM[j].m_sName, sStat, sXT[0], sXT[1], sXT[2]);
		strcat(out, temp);
		// ---------------------------------------------------------------
		*temp=0;
		if (pHR0->m_pPMST[j].m_bBusy  && 
			!pHR1->m_pPMST[j].m_bBusy &&
			!pHR2->m_pPMST[j].m_bBusy)
			sprintf(temp, "-> fired %s", sTNames[0]);
		if (pHR0->m_pPMST[j].m_bBusy  && 
			!pHR1->m_pPMST[j].m_bBusy &&
			pHR2->m_pPMST[j].m_bBusy)
			sprintf(temp, "-> fired %s, new %s", sTNames[0], sTNames[2]);
		if (!pHR0->m_pPMST[j].m_bBusy  && 
			!pHR1->m_pPMST[j].m_bBusy  &&
			pHR2->m_pPMST[j].m_bBusy)
			sprintf(temp, "-> new %s", sTNames[2]);
		strcat(out, temp);			
		strcat(out, "\n");
		}
	// -------------------------------------------------------------------	
	if (strlen(out)>(unsigned)allocsize*3/4)
		{		
		kmar++;
		allocsize=LFPN_SP1K*LFPN_SPMEMBLK*kmar;
		swpout=new char[allocsize];
		strcpy(swpout, out);
		delete out;
		out=swpout;
		}
	// -------------------------------------------------------------------
	}
// -----------------------------------------------------------------------
return(out);
}
// =======================================================================
char* CFileLOG::GetTextStat()
{
// -SERGY-----------------------------------------------------------------
// Description: returns generated text array with statistics
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 27.05.2002
//			LE: 27.05.2002
// -----------------------------------------------------------------------
char* out;
char temp[256];
// -----------------------------------------------------------------------
int i;
int nPMs=m_pRsrcs->m_nPMs;
int nProcs=m_pRsrcs->m_nProcs;	// (16.05.2002)
// -----------------------------------------------------------------------
int nCycles=m_pStat->m_tPMain.m_nCycles;
int nTimeInCycles=m_pStat->m_tPMain.m_nTimeInCycles;
// -----------------------------------------------------------------------
out=new char[LFPN_SP1K*LFPN_SPMEMBLK]; if (!out) return(false);
// -----------------------------------------------------------------------
sprintf(out,
	"[statistics]\n"
	"Max p/w PMs    = %d [always free PMs = %d]\n"
	"Min p/w PMs    = %d [max free PMs    = %d]\n\n"		
	"[process: PN_MAINPROCESS]\n"
	"cycles         = %d [%d iterations]\n"
	"avg cycle time = %3.1f (iterations/cycles)\n"
	"DUB markings   = %d\n"
	"OVER markings  = %d\n"
	"stagnations    = %d\n\n"
	,
	m_pStat->m_nMaxParallPMs, nPMs-m_pStat->m_nMaxParallPMs,
	m_pStat->m_nMinParallPMs, nPMs-m_pStat->m_nMinParallPMs,
	nCycles, nTimeInCycles, nCycles?(float)nTimeInCycles/nCycles:0.0, 
	m_pStat->m_tPMain.m_nMarkDub, m_pStat->m_tPMain.m_nMarkOver,
	m_pStat->m_tPMain.m_nStagns);
// -----------------------------------------------------------------------
for (i=0; i<nProcs; i++)
	{
	nCycles=m_pStat->m_pProcs[i].m_nCycles;
	nTimeInCycles=m_pStat->m_pProcs[i].m_nTimeInCycles;
	sprintf(temp, 
		"[process: %s]\n"
		"cycles         = %d [%d iterations]\n"
		"avg cycle time = %3.1f (iterations/cycles)\n"
		"DUB markings   = %d\n"
		"OVER markings  = %d\n"
		"stagnations    = %d\n\n"
		,
		GetRsrcNameByID(m_pRsrcs->m_pLPROC[i].m_nID),
		//m_pStat->m_pProcs[i].m_nID, 
		nCycles, nTimeInCycles, nCycles?(float)nTimeInCycles/nCycles:0.0,
		m_pStat->m_pProcs[i].m_nMarkDub, m_pStat->m_pProcs[i].m_nMarkOver,
		m_pStat->m_pProcs[i].m_nStagns);
	strcat(out, temp);
	}
// -----------------------------------------------------------------------
return(out);
}
// =======================================================================
BOOL CFileLOG::SaveProtocolBIN(char *sFilename)
{
// -SERGY-----------------------------------------------------------------
// Description: Saves history protocol into binary file
//   Parametrs: sFilename	- filename
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 09.05.2002
//			LE: 09.05.2002
// -----------------------------------------------------------------------
int i, j, k;
FILE* fp;
// -----------------------------------------------------------------------
int nSize=m_pHist->m_nSize;
int nPlaces=m_pRsrcs->m_nPlaces;
int nTrans=m_pRsrcs->m_nTrans;
int nPMs=m_pRsrcs->m_nPMs;
if (!nSize) return(false);
// -----------------------------------------------------------------------
int nIterFirst=m_pHist->m_pHistory[0][0].m_nCurIterNum;
int nIterLast=m_pHist->m_pHistory[0][nSize-1].m_nCurIterNum;
int nInters=nIterLast-nIterFirst+1;
// -----------------------------------------------------------------------
logheader tHeader;
// -----------------------------------------------------------------------
if ((fp=fopen(sFilename, "wb"))==NULL)
	return(false);
// -----------------------------------------------------------------------
// Header
// -----------------------------------------------------------------------
strncpy(tHeader.m_sInitLOG, CLOG02e_initLOGok, strlen(CLOG02e_initLOGok));
tHeader.m_nSize=nSize;
tHeader.m_nPlaces=nPlaces;
tHeader.m_nTrans=nTrans;
tHeader.m_nPMs=nPMs;
// -----------------------------------------------------------------------
fwrite (&tHeader, 1, sizeof(logheader), fp);
// -----------------------------------------------------------------------
for (i=0; i<nPlaces; i++)
	fwrite (&m_pRsrcs->m_pLPL[i], 1, sizeof(CPetriNet::tagRSRCPL), fp);
for (i=0; i<nTrans; i++)
	fwrite (&m_pRsrcs->m_pLTR[i], 1, sizeof(CPetriNet::tagRSRCTR), fp);
for (i=0; i<nPMs; i++)
	fwrite (&m_pRsrcs->m_pLPM[i], 1, sizeof(CPetriNet::tagRSRCPM), fp);
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)	
	for (k=0; k<3; k++)
		{
		fwrite (&m_pHist->m_pHistory[k][i].m_nCurIterNum, 1, sizeof(m_pHist->m_pHistory[0][0].m_nCurIterNum), fp);
		fwrite (&m_pHist->m_pHistory[k][i].m_nMarkType, 1, sizeof(m_pHist->m_pHistory[0][0].m_nMarkType), fp);
		fwrite (&m_pHist->m_pHistory[k][i].m_nMarkRef, 1, sizeof(m_pHist->m_pHistory[0][0].m_nMarkRef), fp);
		for (j=0; j<nPlaces; j++)
			fwrite (&m_pHist->m_pHistory[k][i].m_pTokens[j], 1, sizeof(m_pHist->m_pHistory[0][0].m_pTokens[0]), fp);
		for (j=0; j<nPMs; j++)
			fwrite (&m_pHist->m_pHistory[k][i].m_pPMST[j], 1, sizeof(m_pHist->m_pHistory[0][0].m_pPMST[0]), fp);
		}	
// -----------------------------------------------------------------------
fclose(fp); 
return(true);
}
// =======================================================================
