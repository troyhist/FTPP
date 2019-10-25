// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Support for protocol history LOG file format.
// Used library: <none>
// -----------------------------------------------------------------------
// File: LOGFile.h
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 06/ 05.2002
// Last edit: 06/ 05.2002
//     Build: 1
// =======================================================================
// LOGFile.h: interface for the CFileLOG class.
// =======================================================================
#if !defined(AFX_LOGFILE_H__21F512C1_610A_11D6_83E6_00C0DF109891__INCLUDED_)
#define AFX_LOGFILE_H__21F512C1_610A_11D6_83E6_00C0DF109891__INCLUDED_
// =======================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// =======================================================================
#include "PetriNet.h"			// Some structures inside - 06.05.2002
#include "MapGen.h"				// Some structures inside - 27.05.2002
// -----------------------------------------------------------------------
// =======================================================================
// !!! DO NOT EDIT !!!
// =======================================================================
#define CFL_ERROR			(-1)		// global erroe (28.05.2002)
#define CLOG02e_initLOGok	"LOG02e"	// version tag (09.05.2002)
// =======================================================================
class CFileLOG  
{
// -----------------------------------------------------------------------
// Structures
// -----------------------------------------------------------------------
public:
	// -------------------------------------------------------------------
	// !!! FILE STRUCTURES !!!
	// -------------------------------------------------------------------
	// -------------------------------------------------------------------
	// Header of LOG file // Sergy/06.05.2002
	// -------------------------------------------------------------------
	struct tagLOGFILEHDR
		{
		char m_sInitLOG[8];		// init (only first 6 bytes are used)
		int m_nSize;			// size in records
		int m_nPlaces;			// number of places
		int m_nTrans;			// number of transitions
		int m_nConns;			// number of connections
		int m_nPMs;				// number of PMs
		int m_nProcs;			// number of processes
		};
	typedef tagLOGFILEHDR logheader;
// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------
public:
	BOOL SaveProtocol(char* sFilename, BOOL bBinary);	// Saves history protocol into given file
	BOOL SetData(CPetriNet::tagRSRC* pRsrcs, CPetriNet::tagLOG* pHist, 
		CPetriNet::tagSTATISTICS* pStat, CMapGen::tagMGABSMAPP* pAbsMapp);	// Inits internal history array with given data
	CFileLOG();
	virtual ~CFileLOG();
// -----------------------------------------------------------------------
private:
	char* GetTextMapping();		// returns generated text array with mapping
	char* GetTextInfo();		// returns generated text array with log-file info
	char* GetTextProtocol();	// returns generated text array with protocol
	char* GetTextStat();		// returns generated text array with statistics
	char* GetRsrcNameByID(int nRsrcID);		// Returns resource's name by given ID
	char* GetTextMarkingType(int nRecNum);	// Returns text description of the marking type
	BOOL SaveProtocolBIN(char* sFilename);
	BOOL SaveProtocolTXT(char* sFilename);
// -----------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------
	BOOL m_bInited;			// class is ininted with history data	
	CPetriNet::tagLOG* m_pHist;		// ptr to history structure
	CPetriNet::tagRSRC* m_pRsrcs;	// ptr to resources structure (28.05.2002)
	CMapGen::tagMGABSMAPP* m_pAbsMapp;	// ptr to absolut mapping (27.05.2002)
	CPetriNet::tagSTATISTICS* m_pStat;	// ptr to statistics structure
// -----------------------------------------------------------------------
};
// =======================================================================
#endif // !defined(AFX_LOGFILE_H__21F512C1_610A_11D6_83E6_00C0DF109891__INCLUDED_)
// =======================================================================
