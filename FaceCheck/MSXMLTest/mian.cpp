#include <windows.h>
#include <msxml.h>

int main(int argc, char *argv[])
{
	::CoInitialize(NULL);

	// 创建文档接口
	IXMLDOMDocument *p_ixmldoc = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_DOMDocument,
									NULL,
									CLSCTX_INPROC_SERVER,
									IID_PPV_ARGS(&p_ixmldoc) );

	wchar_t *xml = L"<A>SDF</A>";
	short xx;
	p_ixmldoc->loadXML( xml, &xx);

	IXMLDOMNodeList *resultList = NULL;
	p_ixmldoc->get_childNodes(&resultList);

	IXMLDOMNode * pXDN = NULL;
	resultList->get_item(0, &pXDN);

	wchar_t *ss = NULL;
	pXDN->get_text(&ss);

	pXDN->Release();
	resultList->Release();
	p_ixmldoc->Release();
	::CoUninitialize();
	return 0;
}