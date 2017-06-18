#include <windows.h>
#include <msxml6.h>

int main(int argc, char *argv[])
{
	::CoInitialize(NULL);

	// 创建文档接口
	IXMLDOMDocument2 *p_ixmldoc = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_DOMDocument60,
									NULL,
									CLSCTX_INPROC_SERVER,
									IID_PPV_ARGS(&p_ixmldoc) );

	wchar_t *xml = L"<?xml version='1.0' encoding='GB2312'?><A>SDF</A>";
	short xx;
	p_ixmldoc->loadXML( xml, &xx);

	// p_ixmldoc->get_parsed();

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