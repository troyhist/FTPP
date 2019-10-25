// =======================================================================
// Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (CPP only)
// Troy computing, Inc.
// Copyright (c) 1994-2002 Serguei Kolevatov
// -----------------------------------------------------------------------
// Petri's Nets Console Processor Main File for The Magister Dissertation.
// Used library: <none>
// -----------------------------------------------------------------------
// File: PetriProcessor.cpp
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 03/ IV.2001, (30.04.2002)
// Last edit: 03/ IV.2001
//     Build: 1
// =======================================================================
// PetriProcessor.cpp : Defines the entry point for the console application.
// =======================================================================
#include "stdafx.h"
// -----------------------------------------------------------------------
#include <stdio.h>
// -----------------------------------------------------------------------
#include "AutoBuild.h"					// Sergy/20.05.01
// -----------------------------------------------------------------------
#include "PetriNet.h"
#include "MapGen.h"						// 22.05.2002
#include "FileMPN.h"					// 01.05.2002
#include "FileLOG.h"					// 06.05.2002
// =======================================================================
// Precompile definitions !!! (22.05.2002)
// =======================================================================
#define	PP_EXAMPLENAME	"!petri.log"	// name of test file
#define	PP_DEFITER		(100)			// default qunatity of iterations
#define	PP_DEFCRIT		(100)			// default criteria
// =======================================================================
#define	PP_DEFSURVPROB	(99.9999)		// default survival probability
#define	PP_DEFFAULTPROB	(0.001)			// default processor's fault probability
#define	PP_DEFTAU0		(10)			// default tau0
#define	PP_DEFTAU1		(20)			// default tau1
// -----------------------------------------------------------------------
#define	PP_MINTAU0		(3)				// default tau0
#define	PP_MINTAU1		(4)				// default tau1
#define	PP_MINSURVPROB	(50)			// minimal survival probability
#define	PP_MAXFAULTPROB	(30)			// maximal processor's fault probability
// =======================================================================
struct tagFTPPCmdLine	// 08.05.2002
	{
	// -------------------------------------------------------------------
	// Petri processor
	// -------------------------------------------------------------------
	int m_nMaxIter;		// maximal number of iterations to perform
	int m_nCycles;		// number of cycles (16.05.2002)
	int m_nCrit;		// not supported: criteria
	// -------------------------------------------------------------------
	// Mapping generator
	// -------------------------------------------------------------------
	BOOL m_bMapGen;		// mapping generation flag	
	int  m_nTau[2];		// tau0, tau1
	double m_dSurvProb;	// Survival probability
	double m_dFaultProb;// Processor's fault probability
	// -------------------------------------------------------------------
	// Common
	// -------------------------------------------------------------------
	BOOL m_bTextLog;	// text log flag
	BOOL m_bQuite;		// quite run flag (not supported yet)
	// -------------------------------------------------------------------
	// Not supported
	// -------------------------------------------------------------------	
	};					// command line
// =======================================================================
BOOL ProcessCmdLine(int argc, char* argv[], tagFTPPCmdLine* tCmdLine)
{
// -----------------------------------------------------------------------
//			MD: 20.05.01 --> 20.05.01, 08.05.2002
//			LE: 16.05.2002
// -----------------------------------------------------------------------
char *pos;
int i;
// -----------------------------------------------------------------------
tCmdLine->m_nMaxIter=0;
tCmdLine->m_nCycles=0;
tCmdLine->m_nCrit=0;
// -----------------------------------------------------------------------
tCmdLine->m_bMapGen=false;
tCmdLine->m_nTau[0]=PP_DEFTAU0;			// tau0
tCmdLine->m_nTau[1]=PP_DEFTAU1;			// tau1
tCmdLine->m_dSurvProb=PP_DEFSURVPROB;	// Survival probability
tCmdLine->m_dFaultProb=PP_DEFFAULTPROB;	// Processor's fault probability
// -----------------------------------------------------------------------
tCmdLine->m_bTextLog=false;
tCmdLine->m_bQuite=false;
// -----------------------------------------------------------------------
if (argc<=0) return(false);
// -----------------------------------------------------------------------
for (i=0; i<argc; i++)
	{
	// -------------------------------------------------------------------
	// Petri processor
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/i:"))!=NULL)
		{ sscanf(pos+3, "%d", &tCmdLine->m_nMaxIter); 
		if (tCmdLine->m_nMaxIter<=0)
			tCmdLine->m_nMaxIter=PP_DEFITER; }
	if ((pos=strstr(argv[i], "/s:"))!=NULL)
		{ sscanf(pos+3, "%d", &tCmdLine->m_nCycles); 
		if (tCmdLine->m_nCycles<=0) 
			tCmdLine->m_nCycles=1; }	
	if ((pos=strstr(argv[i], "/c:"))!=NULL)
		{ sscanf(pos+3, "%d", &tCmdLine->m_nCrit); 
		if (tCmdLine->m_nCrit<=0 || tCmdLine->m_nCrit>PP_DEFCRIT) 
			tCmdLine->m_nCrit=PP_DEFCRIT; }		
	// -------------------------------------------------------------------
	// Mapping generator
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/m"))!=NULL) tCmdLine->m_bMapGen=true;
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/tau0:"))!=NULL)
		{ sscanf(pos+6, "%d", &tCmdLine->m_nTau[0]);
		if (tCmdLine->m_nTau[0]<=0)
			tCmdLine->m_nTau[0]=PP_DEFTAU0; }
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/tau1:"))!=NULL)
		{ sscanf(pos+6, "%d", &tCmdLine->m_nTau[1]);
		if (tCmdLine->m_nTau[1]<=0)
			tCmdLine->m_nTau[1]=PP_DEFTAU1; }
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/pr:"))!=NULL)
		{ sscanf(pos+4, "%lf", &tCmdLine->m_dSurvProb); 
		if (tCmdLine->m_dSurvProb<=0 || tCmdLine->m_dSurvProb>=100)
			tCmdLine->m_dSurvProb=PP_DEFSURVPROB; }
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/p:"))!=NULL)
		{ sscanf(pos+3, "%lf", &tCmdLine->m_dFaultProb); 
		if (tCmdLine->m_dFaultProb<0 || tCmdLine->m_dFaultProb>=100)
			tCmdLine->m_dFaultProb=PP_DEFFAULTPROB; }
	// -------------------------------------------------------------------
	// Common
	// -------------------------------------------------------------------
	if ((pos=strstr(argv[i], "/q"))!=NULL) tCmdLine->m_bQuite=true;
	if ((pos=strstr(argv[i], "/t"))!=NULL) tCmdLine->m_bTextLog=true;
	// -------------------------------------------------------------------
	}
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL MNMapGen(char* sFileIn, char* sFileOut, double dSurvProb,
			  double dFaultProb, int nTau0, int nTau1, BOOL bTextLog)
{
// -----------------------------------------------------------------------
//			MD: 10.05.2002
//			LE: 28.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
CMapGen mg;					// Mapping generator
CFileMPN tpnfile;			// MPN file
CFileLOG logfile;			// LOG file (06.05.2002)
// -----------------------------------------------------------------------
printf ("Mapping generator:\n\n"
		"Survive probability               = %10.7f %%\n"
		"PM's fault probability            = %10.7f %%\n"
		"Cycle time limit (normal mode)    = %d (iterations/cycle)\n"
		"Cycle time limit (distorted mode) = %d (iterations/cycle)\n"
		"Possible prediction               = %s\n\n",
		(float)dSurvProb, (float)dFaultProb, nTau0, nTau1,
		((dSurvProb>=PP_MINSURVPROB) &&
		(dFaultProb<=PP_MAXFAULTPROB)&&
		(nTau0<nTau1))?"may be":"not enought resources OR time"
		);
// -----------------------------------------------------------------------
if (nTau0>nTau1 || nTau0<PP_MINTAU0)
	{
	printf ("ERROR: 'nTau0' should be equal or greater than %d\n"
			"       or 'nTau1' should be greater than 'nTau0'\n", 
			PP_DEFTAU0);
	return(false);
	}
// -----------------------------------------------------------------------
if(!tpnfile.LoadFileMPN(sFileIn)) 
	{
	printf ("ERROR: Failed to load file '%s' ...\n"
			"DESCR: %s\n", sFileIn, "<none>");
	return(false);
	}
// -----------------------------------------------------------------------
printf ("File info:\n"
		"Places     = %d\tTransitions= %d\tConnections= %d\n"
		"Processes  = %d\n\n",
		tpnfile.GetPtrMPN()->m_nPNplaces,
		tpnfile.GetPtrMPN()->m_nPNtransitions,
		tpnfile.GetPtrMPN()->m_nPNconnections,		
		tpnfile.GetPtrMPN()->m_nProcs);
// -----------------------------------------------------------------------
mg.Init();
printf("FTPP: Making new net from file ... ");
bRes=mg.SetDataFromMPN(tpnfile.GetPtrMPN()); printf("%s\n", bRes?"ok":"error");
// -----------------------------------------------------------------------
printf("FTPP: Generating optimal mapping ... ");
bRes=mg.Generate(dSurvProb/100., dFaultProb/100., nTau0, nTau1); printf("%s\n", bRes?"ok":"error");
// -----------------------------------------------------------------------
printf("FTPP: Transfering protocol into the log file ... ");
bRes=logfile.SetData(mg.GetResources(), NULL, NULL, mg.GetMappings()); 
printf("%s\n", bRes?"ok":"error");
printf("FTPP: Saving the log file ... ");
bRes=logfile.SaveProtocol(sFileOut, !bTextLog); printf("%s\n", bRes?"ok":"error");
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL MNProcessPN(char* sFileIn, char* sFileOut, int nMaxIter, 
				 int nCycles, BOOL bTextLog)
{
// -----------------------------------------------------------------------
//			MD: 03.04.01 --> 20.05.01, 30.04.2002
//			LE: 19.05.2002
// -----------------------------------------------------------------------
BOOL bRes;
CPetriNet pn;				// petri network
CFileMPN tpnfile;			// MPN file
CFileLOG logfile;			// LOG file (06.05.2002)
// -----------------------------------------------------------------------
printf ("Petri net processor:\n"
		"Iterations to perform (no more than): %d\n"
		"Cycles to perform (no more than)    : %d (%s)\n\n",
		nMaxIter, nCycles, nCycles?"limited":"unlimited");
// -----------------------------------------------------------------------
if(!tpnfile.LoadFileMPN(sFileIn)) 
	{
	printf ("ERROR: Failed to load file '%s' ...\n"
			"DESCR: %s\n", sFileIn, "<none>");
	return(false);
	}
// -----------------------------------------------------------------------
printf ("File info:\n"
		"Places     = %d\tTransitions= %d\tConnections= %d\n"
		"PMs        = %d\tProcesses  = %d\tCriteria   = %d\n\n",
		tpnfile.GetPtrMPN()->m_nPNplaces,
		tpnfile.GetPtrMPN()->m_nPNtransitions,
		tpnfile.GetPtrMPN()->m_nPNconnections,
		tpnfile.GetPtrMPN()->m_nPMs,
		tpnfile.GetPtrMPN()->m_nProcs,
		tpnfile.GetPtrMPN()->m_nCriteria);
// -----------------------------------------------------------------------
pn.Init();
printf("FTPP: Making new net from file ... ");
bRes=pn.NewNetFromMPN(tpnfile.GetPtrMPN()); printf("%s\n", bRes?"ok":"error");
// -----------------------------------------------------------------------
printf("FTPP: Runnig ... ");
bRes=pn.Run(tpnfile.GetPtrMPN()->m_nItercur, 
	   nMaxIter?nMaxIter:tpnfile.GetPtrMPN()->m_nIter, 
	   nCycles);
printf("%s\n", bRes?"ok":"error");
// -----------------------------------------------------------------------
printf("FTPP: Transfering protocol into the log file ... ");
bRes=logfile.SetData(pn.GetResources(), pn.GetProtocol(), 
					 pn.GetStatistics(), NULL); 
printf("%s\n", bRes?"ok":"error");
printf("FTPP: Saving the log file ... ");
bRes=logfile.SaveProtocol(sFileOut, !bTextLog); printf("%s\n", bRes?"ok":"error");
// -----------------------------------------------------------------------
printf ("\n"
		"Number of iterations to perform         : %d (in command line %d)\n"
		"Number of iterations have been performed: %d [%d...%d]\n"
		"Number of cycles have been performed    : %d\n\n",
		tpnfile.GetPtrMPN()->m_nIter, nMaxIter, pn.GetNumberOfIters(),
		tpnfile.GetPtrMPN()->m_nItercur, pn.GetLastIterNum()-1,
		pn.GetNumberOfCycles());
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
int main(int argc, char* argv[])
{
// -----------------------------------------------------------------------
//			MD: 03.04.01 --> 20.05.01, 30.04.2002
//			LE: 08.05.2002
// -----------------------------------------------------------------------
printf ("\n"
		"Fault Tolerant Petri nets Processor (FTPP) ver. 1.0 (build %d)\n"
		"Troy computing, Inc.\n"
		"Copyright (c) 1994-2002 Serguei Kolevatov\n\n",
		AUTO_BUILDNUMBER);
// -----------------------------------------------------------------------
if (argc>2 && argc<10)
	{
	// -------------------------------------------------------------------
	// Main part
	// -------------------------------------------------------------------
	char sFileIn[256];			// input file
	char sFileOut[256];			// output file
	tagFTPPCmdLine tCmdLine;	// command line
	// -------------------------------------------------------------------
	strcpy(sFileIn, argv[1]);
	strcpy(sFileOut, argv[2]);
	if (argc>2)	ProcessCmdLine(argc-3, argv+3, &tCmdLine);
	// -------------------------------------------------------------------
	printf ("Summary:\n"
		"Input file : %s\n"
		"Output file: %s (%s format)\n\n",
		sFileIn, sFileOut, tCmdLine.m_bTextLog?"TXT":"BIN");
	// -------------------------------------------------------------------
	if (tCmdLine.m_bMapGen) 
		MNMapGen(sFileIn, sFileOut, tCmdLine.m_dSurvProb, 
		tCmdLine.m_dFaultProb, tCmdLine.m_nTau[0], tCmdLine.m_nTau[1], 
		tCmdLine.m_bTextLog);
	else
		MNProcessPN(sFileIn, sFileOut, tCmdLine.m_nMaxIter, 
		tCmdLine.m_nCycles, tCmdLine.m_bTextLog);
	}
// -----------------------------------------------------------------------
else if (argc==2)
	{
	if(!strcmp(argv[1], "/?"))
		{
		printf(
"Help:\n\n"
"Mapping generator: (MG)\n"
"  Input file should  be in MPN format. PPI of TPN01e formats are not\n"
"supported.  Input file  should  have at least two processes (besides\n"
"main process. Mapping generation takes much time.\n\n"
"Petri net processor: (PP)\n"
"  It's a powerful Petri net imitator. It supports processes definition\n"
"and tasks spreading by using processing modules.\n\n"
	);
		}
	else printf ("ERROR: Incorrect command '%s'", argv[1]);
	}
else
	printf(
	"Usage: FTPP <infile.tpn> <outfile.log> [<keys>]\n"
	"Parametrs:\n"
	"infile.tpn  - input file with Petri Net in MPN or PPI format\n"
	"outfile.log - output file with binary or textual protocol information\n"
	"              of the emulation process or mapping\n\n"	
	"Keys for Mapping Generator:\n"
	"/m          - mapping generation mode\n"
	"/pr:<num>   - survive probability\n"
	"/p:<num>    - PM's fault probability\n"
	"/tau0:<num> - cycle time limit in normal mode\n"
	"/tau1:<num> - cycle time limit in distorted mode\n\n"	
	"Keys for Petri net processor:\n"
	"/i:<num>    - maximal number of iterations to perform\n"
	"/s:<num>    - minimal number of cycles to perform\n\n"
	"/?          - help screen\n"
	"/q          - suppress output to console screen (not ready)\n"
	"/t          - represent log data in textual format\n\n"	
	"    Techinfo: Build no. %d from "__DATE__", "__TIME__"\n"
	"Please visit: http://troyan.da.ru\n"
	"      e-mail: troyan@chat.ru\n", AUTO_BUILDNUMBER);
// -----------------------------------------------------------------------
return(0);
}
// =======================================================================
