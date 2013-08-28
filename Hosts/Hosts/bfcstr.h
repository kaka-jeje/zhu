
#pragma once


#ifdef _UNICODE
inline CStringA ToChar(CString str)
{
	return CStringA(str);
}
#else
inline CString ToChar(CString str)
{
	return str;
}
#endif

//////////////////////////////////////////////////////////////////////////
//GB TO UTF8
//////////////////////////////////////////////////////////////////////////
extern CStringA GBToUTF8(LPCSTR lpszText);
extern CStringA UnicodeToUTF8(LPCWSTR lpszText);
extern CStringA GBToURLEncodeA(LPCSTR lpszText);
extern CStringW GBToURLEncodeW(LPCSTR lpszText);
extern CStringA UTF8ToURLEncode(LPCSTR lpszText, int nLen);
extern CStringA UTF8ToURLEncode(CStringA strText);
extern CStringA UnicodeToURLEncodeA(LPCWSTR lpszText);
extern CStringW UnicodeToURLEncodeW(LPCWSTR lpszText);

#ifdef _UNICODE
extern CStringA GBToUTF8(CString csGB);
extern CString UTF8ToGB(CStringA csUTF8);
extern CString URLEncodeToGB(CString csUrlencode);
extern CString GBToURLEncode(CString csGB);
#define UnicodeToURLEncode UnicodeToURLEncodeW
#else
extern CString UTF8ToGB(CString csUTF8);
#define UnicodeToURLEncode UnicodeToURLEncodeA
#endif

