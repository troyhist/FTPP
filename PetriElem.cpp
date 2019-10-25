// =======================================================================
// Petri's Nets Processor ver. 1.0 (CPP only)
// Copyright (c) 1994-2001 Kolevatov Sergy
// -----------------------------------------------------------------------
// Petri's Nets Basic Element class
// Used library: <none>
// -----------------------------------------------------------------------
// File: PetriElem.cpp
// -----------------------------------------------------------------------
//     Compiler: Visual C++ 6.0
//  Application: Win32 Console
// -----------------------------------------------------------------------
// Made date: 03/ IV.2001
// Last edit: 03/ IV.2001
//     Build: 1
// =======================================================================
// PetriElem.cpp: implementation of the CPetriElem class.
// =======================================================================
#include "stdafx.h"
#include <time.h>					// rand(), srand() / (05.05.2002)
// -----------------------------------------------------------------------
#include "PetriElem.h"
// =======================================================================
// Construction/Destruction
// =======================================================================
CPetriElem::CPetriElem():
	m_bDefined(false)
{
// -SERGY-----------------------------------------------------------------
// Description: The Constructor
//   Parametrs: <none>
//      Return: <none>
// -----------------------------------------------------------------------
//			MD: 03.04.01
//			LE: 03.04.01
// -----------------------------------------------------------------------
srand((unsigned)time(NULL));	// 07.05.2002
Reset();
}
// =======================================================================
CPetriElem::CPetriElem(int nNum, int nType)
{
// -SERGY-----------------------------------------------------------------
// Description: The Constructor
//   Parametrs: num		- number of element
//				type	- type of element (PE_PLACE, PE_TRANSITION)
//      Return: <none>
// -----------------------------------------------------------------------
//			MD: 03.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
CPetriElem();
Define(nNum, nType);
// -----------------------------------------------------------------------
}
// =======================================================================
CPetriElem::~CPetriElem()
{
// -SERGY-----------------------------------------------------------------
// Description: The Destructor
//   Parametrs: <none>
//      Return: <none>
// -----------------------------------------------------------------------
//			MD: 03.04.01
//			LE: 03.04.01
// -----------------------------------------------------------------------
Reset();
}
// =======================================================================
BOOL CPetriElem::Reset()
{
// -SERGY-----------------------------------------------------------------
// Description: Resets all internal data
//   Parametrs: <none>
//      Return: true
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (m_bDefined)
	{
	if(m_nInrays) delete[] m_pInrays;
	if(m_nOutrays) delete[] m_pOutrays;
	}
// -----------------------------------------------------------------------	
m_nID=0; m_nType=PE_ERROR;
// -----------------------------------------------------------------------
m_nTokens=0;
m_nTTFmax=0; m_nProbab=100;	
// -----------------------------------------------------------------------
m_nInrays=0; m_nOutrays=0;
*m_sName=0; *m_sComment=0;
// -----------------------------------------------------------------------
m_bDefined=false; return(true);
}
// =======================================================================
BOOL CPetriElem::AddRayIn()
{
// -SERGY-----------------------------------------------------------------
// Description: Adds one inbound ray to the element
//   Parametrs: <none>
//      Return: true	- success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.04.01
//			LE: 03.04.01
// -----------------------------------------------------------------------
int i;
int qinrays=m_nInrays+1;
// -----------------------------------------------------------------------
CPetriElem** inrays;
// -----------------------------------------------------------------------
inrays=new CPetriElem*[qinrays];
// -----------------------------------------------------------------------
for (i=0; i<m_nInrays; i++)
	inrays[i]=m_pInrays[i];
inrays[m_nInrays]=NULL;
// -----------------------------------------------------------------------
if (m_nInrays) delete[] m_pInrays;
// -----------------------------------------------------------------------
m_nInrays=qinrays;
m_pInrays=inrays;
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::AddRayOut()
{
// -SERGY-----------------------------------------------------------------
// Description: Adds one outbound ray to the element
//   Parametrs: <none>
//      Return: true	- success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
int i;
int qoutrays=m_nOutrays+1;
// -----------------------------------------------------------------------
CPetriElem** outrays;
// -----------------------------------------------------------------------
outrays=new CPetriElem*[qoutrays];
// -----------------------------------------------------------------------
for (i=0; i<m_nOutrays; i++)
	outrays[i]=m_pOutrays[i];
outrays[m_nOutrays]=NULL;
// -----------------------------------------------------------------------
if (m_nOutrays) delete m_pOutrays;
// -----------------------------------------------------------------------
m_nOutrays=qoutrays;
m_pOutrays=outrays;
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::AddRayIn(CPetriElem *cPElem)
{
// -SERGY-----------------------------------------------------------------
// Description: Adds one defined inbound ray to the element
//   Parametrs: cPElem	- ptr to CPetriElem
//      Return: true	- success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.04.01
//			LE: 03.04.01
// -----------------------------------------------------------------------
if(!cPElem) return(false);
// -----------------------------------------------------------------------
AddRayIn();
m_pInrays[m_nInrays-1]=cPElem;
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::AddRayOut(CPetriElem *cPElem)
{
// -SERGY-----------------------------------------------------------------
// Description: Adds one defined outbound ray to the element
//   Parametrs: cPElem	- ptr to CPetriElem
//      Return: true	- success, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.04.01
//			LE: 03.04.01
// -----------------------------------------------------------------------
if(!cPElem) return(false);
// -----------------------------------------------------------------------
AddRayOut();
m_pOutrays[m_nOutrays-1]=cPElem;
// -----------------------------------------------------------------------
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::ConnectTo(CPetriElem* cPElem)
{
// -SERGY-----------------------------------------------------------------
// Description: Connects this Petri element to given one
//   Parametrs: cPElem
//      Return: true	- defined, false - otherwise
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
if (!cPElem) return(false);
if (!cPElem->IsDefined()) return(false);
// -----------------------------------------------------------------------
cPElem->AddRayIn(this);	// adds reference to ourself
AddRayOut(cPElem);		// makes a connection to given elem
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::Define(int nID, int nType)
{
// -SERGY-----------------------------------------------------------------
// Description: Defines element
//   Parametrs: num		- number of element
//				type	- type of element (PE_PLACE, PE_TRANSITION)
//      Return: true	- defined, false - otherwise
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
if (nID<0) nID=0;
if (nType!=PE_PLACE && nType!=PE_TRANSITION) nType=PE_PLACE;
// -----------------------------------------------------------------------
m_bDefined=true;
m_nID=nID;			m_nType=nType;
AutoGenerateName();	AutoGenerateComment();
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
BOOL CPetriElem::DefinePlace(int nID, int nTokens)
{
// -SERGY-----------------------------------------------------------------
// Description: Defines PLACE element
//   Parametrs: nNum	- number of element (uses as a ID)
//				nTokens	- number of tokens
//      Return: true	- defined, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 03.05.2002
// -----------------------------------------------------------------------
Define(nID, PE_PLACE); PlaceSetTokens(nTokens); return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::DefineTransition(int nID, int nTTF, int nProbab)
{
// -SERGY-----------------------------------------------------------------
// Description: Defines TRANSITION element
//   Parametrs: nNum	- number of element (uses as a ID)
//				nTTF	- max TTF
//				nProbab	- probability of firing
//      Return: true	- defined, false - otherwise
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 04.05.2002
// -----------------------------------------------------------------------
Define(nID, PE_TRANSITION);	TransSetTTF(nTTF);	TransSetProbab(nProbab);
//TransSetFlagLoad(bLoaded);	TransSetTTFcur(nTTFcur);	
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::IsDefined()
{
// -SERGY-----------------------------------------------------------------
// Description: Checks definition of element
//   Parametrs: <none>
//      Return: true	- defined, false - otherwise
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
return(m_bDefined);
}
// =======================================================================
BOOL CPetriElem::AutoGenerateComment()
{
// -SERGY-----------------------------------------------------------------
// Description: Automatically generates comment of element according to its
//				properties
//   Parametrs: <none>
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
char temp[64];
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
// -----------------------------------------------------------------------
sprintf(temp, "%-3s: InRays: %2d\tOutRays: %2d\t%s: %2d", 
		(m_nType==PE_PLACE)?"PE_PLACE":"PE_TRANSITION",
		m_nInrays,
		m_nOutrays,
		(m_nType==PE_PLACE)?"Tokens":"TTF",
		(m_nType==PE_PLACE)?m_nTokens:m_nTTFmax);
SetComment(temp); return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::AutoGenerateName()
{
// -SERGY-----------------------------------------------------------------
// Description: Automatically generates name of element according to its
//				number
//   Parametrs: <none>
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
char temp[64];
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
// -----------------------------------------------------------------------
sprintf(temp, "%c%d", (m_nType==PE_PLACE)?'p':'t', m_nID);
SetName(temp); return(true);
}
// =======================================================================
BOOL CPetriElem::SetComment(char *comment)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new comment string of the element
//   Parametrs: ...
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if (!comment) return(false);
strcpy(m_sComment, comment);
return(true);
}
// =======================================================================
BOOL CPetriElem::SetName(char *name)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new name of the element
//   Parametrs: ...
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if (!name) return(false);
strcpy(m_sName, name);
return(true);
}
// =======================================================================
BOOL CPetriElem::SetID(int num)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new number of the element
//   Parametrs: ...
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if(num<0) return(false);
m_nID=num; 
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriElem::PlaceGetTokens()
{
// -SERGY-----------------------------------------------------------------
// Description: Gets number of tokens to the element
//   Parametrs: n		- quantity of tokens
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
if(m_nType!=PE_PLACE) return(PE_ERROR);
return(m_nTokens);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::PlaceSetTokens(int n)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new number of tokens to the element
//   Parametrs: n		- quantity of tokens
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
if(m_nType!=PE_PLACE) return(false);
m_nTokens=n;
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
char* CPetriElem::GetName()
{
// -SERGY-----------------------------------------------------------------
// Description: Gets name of element
//   Parametrs: <none>
//      Return: ptr	- success, NULL - error
// -----------------------------------------------------------------------
//			MD: 12.04.01
//			LE: 12.04.01
// -----------------------------------------------------------------------
if(!IsDefined()) return(NULL);
return(m_sName);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriElem::GetID()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns number of element
//   Parametrs: <none>
//      Return: >=0	- success, PE_ERROR - error
// -----------------------------------------------------------------------
//			MD: 14.05.01
//			LE: 14.05.01
// -----------------------------------------------------------------------
if(!IsDefined()) return(PE_ERROR);
return(m_nID);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriElem::GetType()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns type of element
//   Parametrs: <none>
//      Return: >=0	- success, PE_ERROR - error
// -----------------------------------------------------------------------
//			MD: 14.05.01
//			LE: 14.05.01
// -----------------------------------------------------------------------
if(!IsDefined()) return(PE_ERROR);
return(m_nType);
// -----------------------------------------------------------------------
}
// =======================================================================
//DEL int CPetriElem::GetQuantityOfRaysOut()
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Returns quantity of outrays
//DEL //   Parametrs: <none>
//DEL //      Return: >=0	- success, PE_ERROR - error
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 21.05.01
//DEL //			LE: 21.05.01
//DEL // -----------------------------------------------------------------------
//DEL if(!IsDefined()) return(PE_ERROR);
//DEL return(m_nOutrays);
//DEL // -----------------------------------------------------------------------
//DEL }
// =======================================================================
BOOL CPetriElem::TransSetTTF(int nTTFmax)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new 'ttf' to the element
//   Parametrs: n		- time to fire
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 07.04.01
//			LE: 07.04.01
// -----------------------------------------------------------------------
if(m_nType!=PE_TRANSITION) return(false);
m_nTTFmax=nTTFmax; //m_nTtf=0;
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
int CPetriElem::TransGetTTF()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns maximal 'ttf' of the element
//   Parametrs: <none>
//      Return: >=0	- success, PE_ERROR - error
// -----------------------------------------------------------------------
//			MD: 14.05.01
//			LE: 14.05.01
// -----------------------------------------------------------------------
if(m_nType!=PE_TRANSITION) return(PE_ERROR);
return(m_nTTFmax);
}
// =======================================================================
BOOL CPetriElem::TransSetProbab(int nProbab)
{
// -SERGY-----------------------------------------------------------------
// Description: Sets new value of Firing Probability
//   Parametrs: nProbab	- probability [0; 100]
//      Return: true	- success, false - error
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 03.05.2002
// -----------------------------------------------------------------------
if(m_nType!=PE_TRANSITION) return(false);
m_nProbab=nProbab;
return(true);
}
// =======================================================================
int CPetriElem::TransGetProbab()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns value of Firing Probability
//   Parametrs: <none>
//      Return: >=0	- success, PE_ERROR - error
// -----------------------------------------------------------------------
//			MD: 03.05.2002
//			LE: 03.05.2002
// -----------------------------------------------------------------------
if(m_nType!=PE_TRANSITION) return(PE_ERROR);
return(m_nProbab);
}
// =======================================================================
//DEL BOOL CPetriElem::TransSetFlagLoad(BOOL bLoaded)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Sets flag "transition is loaded"
//DEL //   Parametrs: bLoaded	- new flag
//DEL //      Return: true	- success, false - error
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 03.05.2002
//DEL //			LE: 03.05.2002
//DEL // -----------------------------------------------------------------------
//DEL if(m_nType!=PE_TRANSITION) return(false);
//DEL m_bTransloaded=bLoaded; return(true);
//DEL }
// =======================================================================
//DEL BOOL CPetriElem::TransGetFlagLoad()
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Returns flag "transition is loaded"
//DEL //   Parametrs: <none>
//DEL //      Return: value
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 03.05.2002
//DEL //			LE: 03.05.2002
//DEL // -----------------------------------------------------------------------
//DEL if(m_nType!=PE_TRANSITION) return(false);
//DEL return(m_bTransloaded);
//DEL }
// =======================================================================
//DEL BOOL CPetriElem::TransSetTTFcur(int nTTFcur)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Sets new value of CURRENT 'ttf' to the element
//DEL //   Parametrs: nTTFcur	- current value of 'time to fire'
//DEL //      Return: true	- success, false - error
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 03.05.2002
//DEL //			LE: 03.05.2002
//DEL // -----------------------------------------------------------------------
//DEL if(m_nType!=PE_TRANSITION) return(false);
//DEL m_nTtf=nTTFcur;
//DEL return(true);
//DEL // -----------------------------------------------------------------------
//DEL }
// =======================================================================
//DEL int CPetriElem::TransGetTTFcur()
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Returns current 'ttf' to the element
//DEL //   Parametrs: <none>
//DEL //      Return: >=0	- success, PE_ERROR - error
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 04.05.01
//DEL //			LE: 04.05.01
//DEL // -----------------------------------------------------------------------
//DEL if(m_nType!=PE_TRANSITION) return(PE_ERROR);
//DEL return(m_nTtf);
//DEL }
// =======================================================================
//DEL int CPetriElem::IsElemConnectedToMe(CPetriElem *cPElem)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Checks connection of given element to myself
//DEL //   Parametrs: cPElem	- element to check
//DEL //      Return: number of connection - success, PE_ERROR - otherwise
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 07.04.01
//DEL //			LE: 07.04.01
//DEL // -----------------------------------------------------------------------
//DEL if (!cPElem) return(PE_ERROR);
//DEL if (!cPElem->IsDefined()) return(PE_ERROR);
//DEL // -----------------------------------------------------------------------
//DEL for (int i=0; i<m_nInrays; i++)	
//DEL 	if(m_pInrays[i]==cPElem)		
//DEL 			return(i);	
//DEL // -----------------------------------------------------------------------
//DEL return(PE_ERROR);
//DEL }
// =======================================================================
//DEL int CPetriElem::IsConnectedTo(CPetriElem *cPElem)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Checks connection of myself to given element
//DEL //   Parametrs: cPElem	- element to check
//DEL //      Return: number of connection - success, PE_ERROR - otherwise
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 07.04.01
//DEL //			LE: 07.04.01
//DEL // -----------------------------------------------------------------------
//DEL if (!cPElem) return(PE_ERROR);
//DEL if (!cPElem->IsDefined()) return(PE_ERROR);
//DEL // -----------------------------------------------------------------------
//DEL for (int i=0; i<m_nOutrays; i++)	
//DEL 	if(m_pOutrays[i]==cPElem)		
//DEL 			return(i);	
//DEL // -----------------------------------------------------------------------
//DEL return(PE_ERROR);
//DEL }
// =======================================================================
//DEL BOOL CPetriElem::GetSignalOut(CPetriElem *cPElem)
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Checks signal from given element to myself
//DEL //   Parametrs: cPElem	- element to check
//DEL //      Return: true	- exists, false - otherwise
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 07.04.01
//DEL //			LE: 26.05.01
//DEL // -----------------------------------------------------------------------
//DEL if (!cPElem) return(false);
//DEL if (!cPElem->IsDefined()) return(false);
//DEL if (m_nType!=PE_PLACE) return(false);	// Sergy/26.05.01
//DEL // -----------------------------------------------------------------------
//DEL BOOL res;
//DEL int nConNum=IsConnectedTo(cPElem);
//DEL // -----------------------------------------------------------------------
//DEL if (nConNum==PE_ERROR) return(false);
//DEL res=(m_nTokens>0)?true:false;
//DEL // -----------------------------------------------------------------------
//DEL return(res);
//DEL // -----------------------------------------------------------------------
//DEL }
// =======================================================================
BOOL CPetriElem::TransIsEnabled()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns 'true' if transition is enabled
//   Parametrs: <none>
//      Return: true	- is enabled, false - otherwise
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if (m_nType!=PE_TRANSITION) return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nInrays; i++)	
	if (m_pInrays[i]->PlaceGetTokens()==0)
		return(false);
// -----------------------------------------------------------------------
return(true);
}
// =======================================================================
//DEL BOOL CPetriElem::TransIsReadyToLoad()
//DEL {
//DEL // -SERGY-----------------------------------------------------------------
//DEL // Description: Checks if transition element is ready to load
//DEL //   Parametrs: <none>
//DEL //      Return: true	- is ready, false - otherwise
//DEL // -----------------------------------------------------------------------
//DEL //			MD: 08.04.01
//DEL //			LE: 08.04.01
//DEL // -----------------------------------------------------------------------
//DEL if (!IsDefined()) return(false);
//DEL if (m_nType!=PE_TRANSITION) return(false);
//DEL // -----------------------------------------------------------------------
//DEL for (int i=0; i<m_nInrays; i++)	
//DEL 	if (m_pInrays[i]->GetSignalOut(this)==false)
//DEL 		return(false);
//DEL // -----------------------------------------------------------------------
//DEL return(true);
//DEL }
// =======================================================================
BOOL CPetriElem::TokenAdd()
{
// -SERGY-----------------------------------------------------------------
// Description: Adds one token to element
//   Parametrs: <none>
//      Return: true	- success, false - otherwise (no any tokens)
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (m_nTokens<0) return (false);
// -----------------------------------------------------------------------
m_nTokens++; return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::TokenSub()
{
// -SERGY-----------------------------------------------------------------
// Description: Subtracts one token from element
//   Parametrs: <none>
//      Return: true	- success, false - otherwise (no any tokens)
// -----------------------------------------------------------------------
//			MD: 08.04.01
//			LE: 08.04.01
// -----------------------------------------------------------------------
if (m_nTokens<=0) return (false);
// -----------------------------------------------------------------------
m_nTokens--; return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::TransLoadIntoPM()
{
// -SERGY-----------------------------------------------------------------
// Description: Transition takes Tokens from source places
//   Parametrs: <none>
//      Return: true	- transition has been loaded, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if (m_nType!=PE_TRANSITION) return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nInrays; i++) m_pInrays[i]->TokenSub();
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::TransFireFromPM()
{
// -SERGY-----------------------------------------------------------------
// Description: Transition fires with Tokens into destination places
//   Parametrs: <none>
//      Return: true	- transition has fired, false - error
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if (m_nType!=PE_TRANSITION) return(false);
// -----------------------------------------------------------------------
for (int i=0; i<m_nOutrays; i++) m_pOutrays[i]->TokenAdd();	
return(true);
// -----------------------------------------------------------------------
}
// =======================================================================
BOOL CPetriElem::TransFireProbabTest()
{
// -SERGY-----------------------------------------------------------------
// Description: Returns 'true' if transition can fire with given probab
//   Parametrs: <none>
//      Return: true	- transition can fire, false - misfire
// -----------------------------------------------------------------------
//			MD: 05.05.2002
//			LE: 05.05.2002
// -----------------------------------------------------------------------
if (!IsDefined()) return(false);
if (m_nType!=PE_TRANSITION) return(false);
// -----------------------------------------------------------------------
int nProbab=rand()%101;		// 08.05.2002 (was '100', now '101')
if (nProbab<=m_nProbab) return(true);
return(false);
// -----------------------------------------------------------------------
}
// =======================================================================
