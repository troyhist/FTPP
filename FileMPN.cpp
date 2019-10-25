// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Support for MPN file format.
// Used library: PetriNet
// -----------------------------------------------------------------------
// File: MPNFile.h
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 30/ 04.2002
// Last edit: 04/ 05.2002
//     Build: 1
// =======================================================================
// MPNFile.cpp: implementation of the CFileMPN class.
// =======================================================================
#include "stdafx.h"
// -----------------------------------------------------------------------
#include "FileMPN.h"
// =======================================================================
// Construction/Destruction
// =======================================================================
CFileMPN::CFileMPN()
{
// -SERGY-----------------------------------------------------------------
// Description: inits MPN memory structure
//   Parametrs: <none>
//      Return: <none>
// -----------------------------------------------------------------------
//			MD: 01.05.2002
//			LE: 01.05.2002
// -----------------------------------------------------------------------
m_tmpn.m_bCorrect=false;
// -----------------------------------------------------------------------
}
// =======================================================================
CFileMPN::~CFileMPN()
{
Shutdown();
}
// =======================================================================
CFileMPN::tagMPNMEM* CFileMPN::GetPtrMPN()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns Ptr to 'm_tmpn' structure with loaded data
//				or NULL, if no data loaded
//   Parametrs: <none>
//      Return: ptr - success, NULL - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
if (!m_tmpn.m_bCorrect) return(NULL);
// -----------------------------------------------------------------------
return(&m_tmpn);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CFileMPN::LoadFileMPN(char *sFileName)
{
// -SERGY-----------------------------------------------------------------
// Description: Loads data from MPN file. File may be binary or textual.
//   Parametrs: sFileName	- name of the file
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 01.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
char buffer[256];			// temp buffer
BOOL res=false;				// loading result
FILE* fp;					// file ptr
// -----------------------------------------------------------------------
if ((fp=fopen(sFileName, "rb"))==NULL) return(false);
// -----------------------------------------------------------------------
Shutdown();			// resets MPN memory structure
// -----------------------------------------------------------------------
// Checks file's ID
// -----------------------------------------------------------------------
fread(buffer, 1, sizeof(MPN_INITOK)*2, fp);
fclose(fp);
// -----------------------------------------------------------------------
if (strncmp(buffer, MPN_INITOK, strlen(MPN_INITOK))==NULL)
	res=LoadFileMPNbin(sFileName);
else if (strncmp(buffer, MPN_INITOK2, strlen(MPN_INITOK2))==NULL)
	res=LoadFileMPNtxt(sFileName, MPN_FVER02T);
else if (strncmp(buffer, MPN_INITOLDOK01, strlen(MPN_INITOLDOK01))==NULL)
	res=LoadFileMPNtxt(sFileName, MPN_FVER01T);
// -----------------------------------------------------------------------
if (res) m_tmpn.m_bCorrect=true;
return(res); 
}
// =======================================================================
BOOL CFileMPN::LoadFileMPNbin(char *sFileName)
{
return(false);
}
// =======================================================================
BOOL CFileMPN::LoadFileMPNtxt(char *sFileName, int nVersion)
{
// -SERGY-----------------------------------------------------------------
// Description: Loads new petri network from file
//   Parametrs: sFileName	- name of the file
//				nVersion	- version of the text file
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 14.05.01 --> 20.05.01, 01.05.2002
//			LE: 08.05.2002
// -----------------------------------------------------------------------
char buffer[1024];			// temp buffer
BOOL bLoadOk;				// fgets returned non-NULL result
//BOOL res;					// generation result
FILE* fp;					// file ptr
// -----------------------------------------------------------------------
int nElems=0;				// total quantity of elements (Sergy/20.05.01)
// -----------------------------------------------------------------------
mpnelem elem;				// mpn element
mpnelem *allelems;			// array of loaded mpn elements
// -----------------------------------------------------------------------
if (nVersion!=MPN_FVER01T && nVersion!=MPN_FVER02T) return(false);
if ((fp=fopen(sFileName, "r"))==NULL)
	return(false);
// -----------------------------------------------------------------------
// Skips first two lines
// -----------------------------------------------------------------------
fgets(buffer, sizeof(buffer)-1, fp);	// skips ID
if(nVersion==MPN_FVER02T) 
	fgets(buffer, sizeof(buffer)-1, fp);// skips comment
// -----------------------------------------------------------------------
// Followin 3 lines are only for ver01T (MPN01e)
// -----------------------------------------------------------------------
m_tmpn.m_nCriteria=0;			// no criteria
m_tmpn.m_nIter=MPN_UNLITER;		// number of iterations to perform is unknown
m_tmpn.m_nItercur=0;			// current iteration is 0
// -----------------------------------------------------------------------
// Reads number of	places, transitions, connections
//					processors, criteria, iteration, current iteration
// -----------------------------------------------------------------------
if (nVersion==MPN_FVER01T)
	{
	// -------------------------------------------------------------------
	// skips number of PLs, TRs, CNs (08.05.2002)
	// -------------------------------------------------------------------
	fgets(buffer, sizeof(buffer)-1, fp);
	fgets(buffer, sizeof(buffer)-1, fp);
	fgets(buffer, sizeof(buffer)-1, fp);
	}
// -----------------------------------------------------------------------
if(nVersion==MPN_FVER02T)
	{	
	fgets(buffer, sizeof(buffer)-1, fp); sscanf(buffer, "%d", &m_tmpn.m_nCriteria);
	fgets(buffer, sizeof(buffer)-1, fp); sscanf(buffer, "%d", &m_tmpn.m_nIter);
	fgets(buffer, sizeof(buffer)-1, fp); sscanf(buffer, "%d", &m_tmpn.m_nItercur);
	}
// -----------------------------------------------------------------------
// Checks values
// -----------------------------------------------------------------------
if (m_tmpn.m_nCriteria<0 || m_tmpn.m_nItercur<0)
	{ fclose(fp); return(false); }
// -----------------------------------------------------------------------
nElems=0;
// -----------------------------------------------------------------------
// Creates array of MPN elements
// -----------------------------------------------------------------------
allelems=new mpnelem[MPN_MAXELEMS];		// Sergy/08.05.2002
// -----------------------------------------------------------------------
while(!feof(fp) && nElems<MPN_MAXELEMS)
	{
	bLoadOk=fgets(buffer, sizeof(buffer)-1, fp)?true:false;
	if (bLoadOk)	elem=(nVersion==MPN_FVER02T)?
			MPNConvertStringToElem(buffer):
			MPNConvertStringToElemOld(buffer);
	elem.m_nID=nElems+1;			// sets ID to element
	// -------------------------------------------------------------------
	if (elem.m_bCorrect && bLoadOk) allelems[nElems++]=elem;	
	}
// -----------------------------------------------------------------------
fclose(fp);
// -----------------------------------------------------------------------
// Calculates elements of differen types & loads memory mpn structure
// -----------------------------------------------------------------------
MPNCalculateElems(allelems, nElems);		// 08.05.2002
MPNLoadTMemFromTElem(allelems, nElems);
delete allelems;							// 08.05.2002
// -----------------------------------------------------------------------
//return(res);
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CFileMPN::MPNCalculateElems(CFileMPN::tagMPNELEM *pAllElems, int nSize)
{
// -SERGY-----------------------------------------------------------------
// Description: Loads in MPNMEM structure number of PLs, TRs, CNs, PMs
//   Parametrs:	pAllElems	- array with elements
//				nSize		- size of array
//      Return: true - success, false - error
// -----------------------------------------------------------------------
//			MD: 08.05.2002
//			LE: 16.05.2002
// -----------------------------------------------------------------------
int i;
BOOL bRes;
// -----------------------------------------------------------------------
m_tmpn.m_nPNplaces=0;			// no places
m_tmpn.m_nPNtransitions=0;		// no transitions
m_tmpn.m_nPNconnections=0;		// no connections
m_tmpn.m_nPMs=0;				// no processors
m_tmpn.m_nProcs=0;				// no processes
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)	
	switch(pAllElems[i].m_nElemType)
		{
		case MPN_EPLACE:	m_tmpn.m_nPNplaces++;		break;
		case MPN_ETRANS:	m_tmpn.m_nPNtransitions++;	break;
		case MPN_ECONN:		m_tmpn.m_nPNconnections++;	break;
		case MPN_EPM:		m_tmpn.m_nPMs++;			break;
		case MPN_EPROC:		m_tmpn.m_nProcs++;			break;
		default: break;
		}
// -----------------------------------------------------------------------
bRes=	m_tmpn.m_nPNplaces &&
		m_tmpn.m_nPNtransitions &&
		m_tmpn.m_nPNconnections;
return(bRes);
}
// =======================================================================
BOOL CFileMPN::MPNLoadTMemFromTElem(CFileMPN::tagMPNELEM *pAllElems, int nSize)
{
// -SERGY-----------------------------------------------------------------
// Description: Loads MPNMEM structure from given array with MPNELEMs
//   Parametrs:	pAllElems	- array with elements
//				nSize		- size of array
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 16.05.2002
// -----------------------------------------------------------------------
BOOL bProcUnlim=false;				// number of processors is limited
int i, j, iPL, iTR, iCN, iPM, iPR;	// index var for PL, TR, CN, PM, PR
int nCurTaskID;						// cur task ID
// -----------------------------------------------------------------------
if (m_tmpn.m_nPMs==0) 
	{ m_tmpn.m_nPMs=m_tmpn.m_nPNtransitions; bProcUnlim=true; }
// -----------------------------------------------------------------------
m_tmpn.m_pPlaces=new mpnplace[m_tmpn.m_nPNplaces];		// places
m_tmpn.m_pTrans=new mpntrans[m_tmpn.m_nPNtransitions];	// transitions
m_tmpn.m_pConns=new mpnconn[m_tmpn.m_nPNconnections];	// connections
m_tmpn.m_pPMs=new mpnpmmem[m_tmpn.m_nPMs];				// tasks and PMs
m_tmpn.m_pProcs=new mpnprmem[m_tmpn.m_nProcs];			// Processes
// -----------------------------------------------------------------------
iPL=iTR=iCN=iPM=iPR=0;
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)
	{
	switch(pAllElems[i].m_nElemType)
		{
		case MPN_EPLACE:		// Place
			// -----------------------------------------------------------
			m_tmpn.m_pPlaces[iPL].m_nID=pAllElems[i].m_nID;
			m_tmpn.m_pPlaces[iPL].m_nTokens=pAllElems[i].m_nTokens;
			strcpy(m_tmpn.m_pPlaces[iPL].m_sName, pAllElems[i].m_sName);
			iPL++;
			break;
		case MPN_ETRANS:		// Transition
			// -----------------------------------------------------------
			m_tmpn.m_pTrans[iTR].m_nID=pAllElems[i].m_nID;
			m_tmpn.m_pTrans[iTR].m_nTTFmax=pAllElems[i].m_nTTFmax;
			m_tmpn.m_pTrans[iTR].m_nProbab=pAllElems[i].m_nProbab;
			strcpy(m_tmpn.m_pTrans[iTR].m_sName, pAllElems[i].m_sName);
			iTR++;
			break;
		case MPN_ECONN:			// Connection
			// -----------------------------------------------------------
			m_tmpn.m_pConns[iCN].m_nID=pAllElems[i].m_nID;
			m_tmpn.m_pConns[iCN].m_nType=pAllElems[i].m_nType;
			m_tmpn.m_pConns[iCN].m_nMult=pAllElems[i].m_nMult;
			// type of connections is unknown, but it's not important
			// at this step and we set it to MPN_CNPLTR (04.05.2002)
			m_tmpn.m_pConns[iCN].m_nDirType=MPN_CNPLTR;
			m_tmpn.m_pConns[iCN].m_nCIDa=MPNGetElemIDByName(
				pAllElems[i].m_sElemA, pAllElems, nSize);
			m_tmpn.m_pConns[iCN].m_nCIDb=MPNGetElemIDByName(
				pAllElems[i].m_sElemB, pAllElems, nSize);
			iCN++;
			break;
		case MPN_EPM:			// ProcessingModule
			// -----------------------------------------------------------			
			m_tmpn.m_pPMs[iPM].m_nID=pAllElems[i].m_nID;
			m_tmpn.m_pPMs[iPM].m_bStatusOk=true;
			m_tmpn.m_pPMs[iPM].m_bBusy=pAllElems[i].m_bBusy;
			nCurTaskID=MPN_ERROR;
			if (pAllElems[i].m_bBusy)
				nCurTaskID=MPNGetElemIDByName(pAllElems[i].m_sCurTaskName, 
				pAllElems, nSize);
			m_tmpn.m_pPMs[iPM].m_nCurTaskID=nCurTaskID;
			m_tmpn.m_pPMs[iPM].m_nTTFcur=pAllElems[i].m_nTTFcur;
			m_tmpn.m_pPMs[iPM].m_nTasks=pAllElems[i].m_nTasks;
			strcpy(m_tmpn.m_pPMs[iPM].m_sName, pAllElems[i].m_sName);
			// -----------------------------------------------------------
			// copies task list
			//m_tmpn.m_pPMs[iPM].m_pTasks=new mpntal[pAllElems[i].m_nTasks];
			m_tmpn.m_pPMs[iPM].m_pTaskIDs=new int[pAllElems[i].m_nTasks];
			for (j=0; j<pAllElems[i].m_nTasks; j++)				
				//m_tmpn.m_pPMs[iPM].m_pTasks[j].m_nTaskID=
				m_tmpn.m_pPMs[iPM].m_pTaskIDs[j]=
					MPNGetElemIDByName(pAllElems[i].m_sTaskNames[j], 
					pAllElems, nSize);				
			// -----------------------------------------------------------
			iPM++;
			break;
		case MPN_EPROC:			// Process (16.05.2002)
			// -----------------------------------------------------------			
			m_tmpn.m_pProcs[iPR].m_nID=pAllElems[i].m_nID;
			m_tmpn.m_pProcs[iPR].m_nResources=pAllElems[i].m_nResources;
			strcpy(m_tmpn.m_pProcs[iPR].m_sName, pAllElems[i].m_sName);
			// -----------------------------------------------------------
			// copies task list
			m_tmpn.m_pProcs[iPR].m_pRsrcIDs=new int[pAllElems[i].m_nResources];
			for (j=0; j<pAllElems[i].m_nResources; j++)
				m_tmpn.m_pProcs[iPR].m_pRsrcIDs[j]=
					MPNGetElemIDByName(pAllElems[i].m_sRsrcNames[j],
					pAllElems, nSize);
			// -----------------------------------------------------------
			iPR++;
			break;
		default: break;
		}
	}
// -----------------------------------------------------------------------
// in case, if we don't have processor list we make special processor 
// list: one processor for each task
// -----------------------------------------------------------------------
int nFreeElemID=m_tmpn.m_nPNplaces+
				m_tmpn.m_nPNtransitions+
				m_tmpn.m_nPNconnections+
				m_tmpn.m_nProcs+1;
// -----------------------------------------------------------------------
if(bProcUnlim)	
	for (i=0; i<m_tmpn.m_nPMs; i++)
		{
		m_tmpn.m_pPMs[i].m_nID=nFreeElemID+i;
		m_tmpn.m_pPMs[i].m_bStatusOk=true;
		m_tmpn.m_pPMs[i].m_bBusy=false;
		m_tmpn.m_pPMs[i].m_nCurTaskID=0;
		m_tmpn.m_pPMs[i].m_nTTFcur=0;
		m_tmpn.m_pPMs[i].m_nTasks=1;		
		m_tmpn.m_pPMs[i].m_pTaskIDs=new int[1];		
		m_tmpn.m_pPMs[i].m_pTaskIDs[0]=m_tmpn.m_pTrans[i].m_nID;
		sprintf(m_tmpn.m_pPMs[i].m_sName, "pm%d", i+1);
		}
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CFileMPN::Shutdown()
{
// -SERGY-----------------------------------------------------------------
// Description: Destroys memory MPN structure if it exists
//   Parametrs: <none>
//      Return: true - success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 01.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
if (!m_tmpn.m_bCorrect) return(false);
// -----------------------------------------------------------------------
for (i=0; i<m_tmpn.m_nPMs; i++)
	if (m_tmpn.m_pPMs[i].m_nTasks)		// 28.05.2002
		delete m_tmpn.m_pPMs[i].m_pTaskIDs;
for (i=0; i<m_tmpn.m_nProcs; i++)		// 16.05.2002
	if (m_tmpn.m_pProcs[i].m_nResources)
		delete m_tmpn.m_pProcs[i].m_pRsrcIDs;
// -----------------------------------------------------------------------
delete m_tmpn.m_pPlaces;
delete m_tmpn.m_pTrans;
delete m_tmpn.m_pConns;
if(m_tmpn.m_nPMs)	delete [] m_tmpn.m_pPMs;
if(m_tmpn.m_nProcs)	delete [] m_tmpn.m_pProcs;				// 16.05.2002
// -----------------------------------------------------------------------
m_tmpn.m_bCorrect=false;
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
CFileMPN::tagMPNELEM CFileMPN::MPNConvertStringToElem(char *sStr)
{
// -SERGY-----------------------------------------------------------------
// Description: Converts given string into MPN element
//   Parametrs: sStr		- given string
//      Return: MPN element
 // -----------------------------------------------------------------------
//			MD: 14.05.01, 01.05.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
char fld[32];		// text fields of MPN element
char *pf, *pfold;	// pointers to according fields
char *temp;			// temp (for calculating commas)
char *comm;			// ptr to comment symbol
int i;
mpnelem res;		// result element
// -----------------------------------------------------------------------
res.m_bCorrect=false;
// -----------------------------------------------------------------------
if (!sStr) return(res);
comm=strstr(sStr, MPN_ETCOMMENT);
if (comm) *comm=0;	// skips string after comment
// -----------------------------------------------------------------------
pfold=sStr;
pf=strstr(pfold, ","); if (!pf) return(res);
strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
// -----------------------------------------------------------------------
res.m_nElemType=MPN_ENONE;
// -----------------------------------------------------------------------
// Tries to determine element type
// -----------------------------------------------------------------------
if (!strcmp(fld, MPN_ETPLACE))		res.m_nElemType=MPN_EPLACE;
else if(!strcmp(fld, MPN_ETTRANS))	res.m_nElemType=MPN_ETRANS;
else if(!strcmp(fld, MPN_ETCONN))	res.m_nElemType=MPN_ECONN;
else if(!strcmp(fld, MPN_ETPM))		res.m_nElemType=MPN_EPM;
else if(!strcmp(fld, MPN_ETPROC))	res.m_nElemType=MPN_EPROC;
else return(res);
// -----------------------------------------------------------------------
switch(res.m_nElemType)
	{
	case MPN_EPLACE: 
		// ---------------------------------------------------------------
		// gets name
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sName, fld);
		// ---------------------------------------------------------------
		// gets tokens
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nTokens); 
		break;
	case MPN_ETRANS: 
		// ---------------------------------------------------------------
		// gets name
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sName, fld);
		// ---------------------------------------------------------------
		// gets ttf (maximal)
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nTTFmax); 
		// ---------------------------------------------------------------
		// gets probability
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nProbab); 
		break;
	case MPN_ECONN: 
		// ---------------------------------------------------------------
		// gets connection type
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nType); 
		// ---------------------------------------------------------------
		// gets connection multiply
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nMult); 		
		// ---------------------------------------------------------------
		// gets SCR
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sElemA, fld);
		// ---------------------------------------------------------------
		// gets DEST
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sElemB, fld);
		break;
	case MPN_EPM: 
		// ---------------------------------------------------------------
		// gets name
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sName, fld);
		// ---------------------------------------------------------------
		// gets "Processor is busy" flag
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_bBusy); 
		// ---------------------------------------------------------------
		// gets current task ID
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sCurTaskName, fld);		
		// ---------------------------------------------------------------
		// gets current task TTF
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nTTFcur); 
		// ---------------------------------------------------------------		
		// gets number of tasks
		res.m_nTasks=0;
		temp=pfold;
		do	{temp=strstr(temp, ","); 
			if (temp)
				{ res.m_nTasks++; temp++; }
			} while(temp);
		// ---------------------------------------------------------------
		res.m_sTaskNames=new char*[res.m_nTasks];
		// ---------------------------------------------------------------
		for (i=0; i<res.m_nTasks; i++)
			{
			// -----------------------------------------------------------
			// gets name of task
			pf=strstr(pfold, ","); if (!pf) return(res);
			strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
			res.m_sTaskNames[i]=new char [8];
			strcpy(res.m_sTaskNames[i], fld);
			// -----------------------------------------------------------			
			}
		// ---------------------------------------------------------------
		break;
	case MPN_EPROC: 
		// ---------------------------------------------------------------
		// gets name
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sName, fld);
		// ---------------------------------------------------------------		
		// gets number of resources
		res.m_nResources=0;
		temp=pfold;
		do	{temp=strstr(temp, ","); 
			if (temp)
				{ res.m_nResources++; temp++; }
			} while(temp);
		// ---------------------------------------------------------------
		res.m_sRsrcNames=new char*[res.m_nResources];
		// ---------------------------------------------------------------
		for (i=0; i<res.m_nResources; i++)
			{
			// -----------------------------------------------------------
			// gets name of the resource
			pf=strstr(pfold, ","); if (!pf) return(res);
			strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
			res.m_sRsrcNames[i]=new char [8];
			strcpy(res.m_sRsrcNames[i], fld);
			// -----------------------------------------------------------			
			}
		// ---------------------------------------------------------------
		break;
	default: break;
	};
// -----------------------------------------------------------------------
res.m_bCorrect=true;
return(res);
// -----------------------------------------------------------------------
}
// =======================================================================
CFileMPN::tagMPNELEM CFileMPN::MPNConvertStringToElemOld(char *sStr)
{
// -SERGY-----------------------------------------------------------------
// Description: Converts given string into MPN element of OLD type
//   Parametrs: sStr		- given string
//      Return: MPN element
 // -----------------------------------------------------------------------
//			MD: 14.05.01, 03.05.2002
//			LE: 03.05.2002
// -----------------------------------------------------------------------
char fld[32];		// text fields of MPN element
char *pf, *pfold;	// pointers to according fields
mpnelem res;		// result element
// -----------------------------------------------------------------------
res.m_bCorrect=false;
// -----------------------------------------------------------------------
if (!sStr) return(res);
// -----------------------------------------------------------------------
pfold=sStr;
pf=strstr(pfold, ","); if (!pf) return(res);
strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
// -----------------------------------------------------------------------
// Tries to determine element type
// -----------------------------------------------------------------------
res.m_nElemType=MPN_ENONE;
// -----------------------------------------------------------------------
sscanf(fld, "%d", &res.m_nElemType);
// -----------------------------------------------------------------------
switch(res.m_nElemType)
	{
	case MPN_EPLACE: 
		// ---------------------------------------------------------------
		// gets name
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sName, fld);
		// ---------------------------------------------------------------
		// gets tokens
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nTokens); 
		break;
	case MPN_ETRANS: 
		// ---------------------------------------------------------------
		// gets name
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sName, fld);
		// ---------------------------------------------------------------
		// gets ttf (maximal)
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		sscanf(fld, "%d", &res.m_nTTFmax); 
		// ---------------------------------------------------------------		
		res.m_nProbab=99;	// probability = 100%
		break;
	case MPN_ECONN: 
		// ---------------------------------------------------------------		
		// skips first comma
		pf=strstr(pfold, ","); if (!pf) return(res);
		pfold=pf+1;
		// ---------------------------------------------------------------		
		// gets SCR
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sElemA, fld);
		// ---------------------------------------------------------------
		// gets DEST
		pf=strstr(pfold, ","); if (!pf) return(res);
		strncpy(fld, pfold, pf-pfold);	fld[pf-pfold]=0; pfold=pf+1;
		strcpy(res.m_sElemB, fld);
		// ---------------------------------------------------------------
		res.m_nType=MPN_CNNORM;	// normal connection type (=0)
		res.m_nMult=1;			// connection multiply is sigle (=1 or =0)
		break;
	default: break;
	};
// -----------------------------------------------------------------------
res.m_bCorrect=true;
return(res);
// -----------------------------------------------------------------------
}
// =======================================================================
int CFileMPN::MPNGetElemIDByName(char *sName, CFileMPN::tagMPNELEM *pAllElems, int nSize)
{
// -SERGY-----------------------------------------------------------------
// Description: Finds element with given name and returns it's ID.
//				Element may be PLACE, TRANSITION or PM
//   Parametrs: sName		- name of element
//				pAllElems	- array with all loaded elements
//				nSize		- number of elements in pAllElems
//      Return: MPN_ERROR (=-1) - error
// -----------------------------------------------------------------------
//			MD: 04.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
int i;
// -----------------------------------------------------------------------
for (i=0; i<nSize; i++)	
	if(!strcmp(pAllElems[i].m_sName, sName))
		return(pAllElems[i].m_nID);
return(MPN_ERROR);
}
// =======================================================================
//DEL int CFileMPN::GetElemTypeByID(int nID)
//DEL 	{ return(GetElemExtInfByID(nID, 0)); }
// =======================================================================
//DEL int CFileMPN::GetElemIdxByID(int nID)
//DEL 	{ return(GetElemExtInfByID(nID, 1)); }
// =======================================================================
//DEL int CFileMPN::GetElemExtInfByID(int nID, int nInfType)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Returns type or index of given element in PLACE of 
//DEL //				TRANSITION arrays
//DEL //   Parametrs: nID			- ID of element
//DEL //				nInfType	- type of result information 
//DEL //							  (=0 - get type, =1 - get idx)
//DEL //      Return: MPN_ERROR (=-1) - error
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 14.05.01, 03.05.2002
//DEL //			LE: 03.05.2002
//DEL // -----------------------------------------------------------------------
//DEL if (!m_tmpn.m_bCorrect) return (MPN_ERROR);
//DEL if (nID==MPN_ERROR) return (MPN_ERROR);
//DEL // -----------------------------------------------------------------------
//DEL int i;
//DEL // -----------------------------------------------------------------------
//DEL for (i=0; i<m_tmpn.m_nPNplaces; i++)	
//DEL 	if (nID==m_tmpn.m_pPlaces[i].m_nID)
//DEL 		return(nInfType?i:MPN_EPLACE);
//DEL // -----------------------------------------------------------------------
//DEL for (i=0; i<m_tmpn.m_nPNtransitions; i++)	
//DEL 	if (nID==m_tmpn.m_pTrans[i].m_nID)
//DEL 		return(nInfType?i:MPN_ETRANS);
//DEL // -----------------------------------------------------------------------
//DEL return(MPN_ERROR);
//DEL }
// =======================================================================
