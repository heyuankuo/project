//
// Cmpnt3.cpp - Component 3
//
#include "ichap07.h"
#include "CUnknown.h"

///////////////////////////////////////////////////////////
//
// Component C
//
class CA : public CUnknown,
           public IFaceDBOS
{
public:	
	// Creation
	static HRESULT CreateInstance(IUnknown* pUnknownOuter,
	                              CUnknown** ppNewComponent) ;

 private:
	// Declare the delegating IUnknown.
	DECLARE_IUNKNOWN

	// Nondelegating IUnknown
	virtual HRESULT __stdcall
		NondelegatingQueryInterface(const IID& iid, void** ppv) ;

	// Interface IZ
	/**
	* 查询人脸数据
	*/
	HRESULT __stdcall QueryFace(UserInfo *user, BYTE *data);

	// Constructor
	CA(IUnknown* pUnknownOuter) ;

	// Destruction
	~CA() ;
} ;
