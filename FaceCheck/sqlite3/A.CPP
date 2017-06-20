//
// Cmpnt3.cpp - Component 3
//
#include <objbase.h>
#include <sqlite3.h>
#include "ichap07.h"
#include "CUnknown.h"
#include "A.H"

#pragma comment(lib, "sqlite3.lib")

static int callback(void *data, int argc, char **argv, char **azColName) 
{
	UserInfo *user = (UserInfo *)data;
	memset(user->_name, 0, sizeof user->_name);

	strcpy_s((char *)user->_name, sizeof user->_name, argv[3]);

	wchar_t name[6] = { 0 };
	MultiByteToWideChar( CP_UTF8, NULL, user->_name, -1, name, 5);

	return 0;
}

///////////////////////////////////////////////////////////
//
// Interface IFaceDBOS
//
HRESULT CA::QueryFace(UserInfo *user, BYTE *data)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("E:\\faceDB.db", &db);

	if (rc) 
	{
		
		return E_FAIL;
	}
	
	char *sql = "SELECT * from faces";
	rc = sqlite3_exec(db, sql, callback, (void*)user, &zErrMsg);
	if (rc != SQLITE_OK) 
	{
		sqlite3_free(zErrMsg);
	}
	else 
	{
	}

	sqlite3_close(db);
	return S_OK;
}

//
// Constructor
//
CA::CA(IUnknown* pUnknownOuter)
: CUnknown(pUnknownOuter)
{
}

CA::~CA() 
{
}

//
// NondelegatingQueryInterface implementation
//
HRESULT __stdcall CA::NondelegatingQueryInterface(const IID& iid,
                                                  void** ppv)
{ 	
	if (iid == __uuidof(IFaceDBOS))
	{
		return FinishQI(static_cast<IFaceDBOS*>(this), ppv) ;
	}
	else
	{
		return CUnknown::NondelegatingQueryInterface(iid, ppv) ;
	}
}


///////////////////////////////////////////////////////////
//
// Creation function used by CFactory
//
HRESULT CA::CreateInstance(IUnknown* pUnknownOuter,
                           CUnknown** ppNewComponent) 
{
	*ppNewComponent = new CA(pUnknownOuter) ;
	return S_OK ;
}
