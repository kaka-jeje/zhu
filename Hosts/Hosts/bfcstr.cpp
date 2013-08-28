
#include "stdafx.h"
#include "bfcstr.h"

//////////////////////////////////////////////////////////////////////////
//GB TO UTF8
//////////////////////////////////////////////////////////////////////////
CStringA GBToUTF8(LPCSTR lpszText)
{
	CStringA strUTF;
	if (!lpszText || 0 == lpszText[0])
		return strUTF;

	//����unicode�����С
	int nUnicodeLen=MultiByteToWideChar(CP_ACP,NULL,lpszText,-1,NULL,0);
	if(nUnicodeLen<=0)
	{
		return strUTF;
	}

	//ת��Ϊunicode
	CStringW strUnicode;
	nUnicodeLen = MultiByteToWideChar(CP_ACP,NULL,lpszText,-1,strUnicode.GetBuffer(nUnicodeLen),nUnicodeLen);
	strUnicode.ReleaseBuffer();
	if(nUnicodeLen<=0)
	{
		return strUTF;
	}
	//����utf-8����Ҫ�Ĵ�С
	int nUtf8Len = WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)strUnicode,-1,NULL,0,NULL,NULL);
	if(nUtf8Len<=0)
	{
		return strUTF;
	}

	//ת����utf-8
	nUtf8Len=WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)strUnicode,-1,strUTF.GetBuffer(nUtf8Len),nUtf8Len,NULL,NULL);
	strUTF.ReleaseBuffer();
	return strUTF;
}

CStringA UnicodeToUTF8(LPCWSTR lpszText)
{
	CStringA strUTF;
	if (!lpszText || 0 == lpszText[0])
		return strUTF;

	//����utf-8����Ҫ�Ĵ�С
	int nUtf8Len = WideCharToMultiByte(CP_UTF8,0,lpszText,-1,NULL,0,NULL,NULL);
	if(nUtf8Len<=0)
	{
		return strUTF;
	}

	//ת����utf-8
	nUtf8Len=WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)lpszText,-1,strUTF.GetBuffer(nUtf8Len),nUtf8Len,NULL,NULL);
	strUTF.ReleaseBuffer();
	return strUTF;
}

CStringA UTF8ToURLEncode(LPCSTR lpszText, int nLen)
{
	CStringA strURLEncode;
	for (int i = 0; i < nLen; ++i)
	{
		BYTE by = (BYTE)lpszText[i];
		BYTE high = by / 16;
		BYTE low = by % 16;
		if (high > 9)
		{
			high = 'A' + (high - 10);
		}
		else
		{
			high = '0' + high;
		}
		if (low > 9)
		{
			low = 'A' + (low - 10);
		}
		else
		{
			low = '0' + low;
		}
		strURLEncode += "%";
		strURLEncode += high;
		strURLEncode += low;
	}
	return strURLEncode;
}

CStringA UTF8ToURLEncode(CStringA strText)
{
	return UTF8ToURLEncode((LPCSTR)strText, strText.GetLength());
}

CStringA GBToURLEncodeA(LPCSTR lpszText)
{
	CStringA strUtf = GBToUTF8(lpszText);
	if (strUtf.IsEmpty())
		return strUtf;

	return UTF8ToURLEncode(strUtf);
}

CStringW GBToURLEncodeW(LPCSTR lpszText)
{
	return CStringW(GBToURLEncodeA(lpszText));
}

CStringA UnicodeToURLEncodeA(LPCWSTR lpszText)
{
	CStringA strUtf = UnicodeToUTF8(lpszText);
	if (strUtf.IsEmpty())
		return strUtf;

	return UTF8ToURLEncode(strUtf);
}

CStringW UnicodeToURLEncodeW(LPCWSTR lpszText)
{
	return CStringW(UnicodeToURLEncodeA(lpszText));
}


#ifdef _UNICODE
CStringA GBToUTF8(CString csGB)
{
	int nUnicodeLen = csGB.GetLength();

	if(nUnicodeLen==0)
	{
		//ASSERT(0);
		return "";
	}

	// 	CHAR * pGB=new CHAR[csGB.GetLength()+1];
	// 	strcpy(pGB,csGB);
	// 
	// 	//����unicode�����С
	// 	int nUnicodeLen=MultiByteToWideChar(CP_ACP,NULL,pGB,-1,NULL,0);
	// 
	// 	if(nUnicodeLen<=0)
	// 	{
	// 		ASSERT(0);
	// 
	// 		delete []pGB;
	// 
	// 		return "";
	// 	}

	//ת��Ϊunicode
	WCHAR * pUnicode=new WCHAR[nUnicodeLen+1];
	lstrcpy(pUnicode, csGB);

	// 	nUnicodeLen=MultiByteToWideChar(CP_ACP,NULL,pGB,-1,pUnicode,nUnicodeLen);
	// 
	// 	if(nUnicodeLen<=0)
	// 	{
	// 		ASSERT(0);
	// 
	// 		delete []pGB;
	// 		delete []pUnicode;
	// 
	// 		return "";
	// 	}

	//����utf-8����Ҫ�Ĵ�С
	int nUtf8Len=WideCharToMultiByte(CP_UTF8,0,pUnicode,-1,NULL,0,NULL,NULL);

	if(nUtf8Len<=0)
	{
		ASSERT(0);

		//		delete []pGB;
		delete []pUnicode;

		return "";
	}

	//ת����utf-8
	CHAR * pUtf8=new CHAR[nUtf8Len];

	nUtf8Len=WideCharToMultiByte(CP_UTF8,0,pUnicode,-1,pUtf8,nUtf8Len,NULL,NULL);

	if(nUtf8Len<=0)
	{
		ASSERT(0);

		//		delete []pGB;
		delete []pUnicode;
		delete []pUtf8;

		return "";
	}

	CStringA strUTF(pUtf8);

	//
	//	delete []pGB;
	delete []pUnicode;
	delete []pUtf8;

	return strUTF;
}

CString UTF8ToGB(CStringA csUTF8)
{
	int nLenUTF8 = csUTF8.GetLength();

	if (0 == nLenUTF8)
	{
		//	ASSERT(0);
		return _T("");
	}

	CHAR * pUtf8=new CHAR[nLenUTF8+1];
	memset(pUtf8,0,nLenUTF8+1);
	strcpy_s(pUtf8, nLenUTF8+1, csUTF8);

	//����unicode�����С
	int nUnicodeLen=MultiByteToWideChar(CP_UTF8,NULL,pUtf8,-1,NULL,0);

	if(nUnicodeLen<=0)
	{
		ASSERT(0);

		delete []pUtf8;

		return _T("");
	}

	//ת��Ϊunicode
	WCHAR * pUnicode=new WCHAR[nUnicodeLen];

	nUnicodeLen=MultiByteToWideChar(CP_UTF8,NULL,pUtf8,-1,pUnicode,nUnicodeLen);

	if(nUnicodeLen<=0)
	{
		ASSERT(0);

		delete []pUtf8;
		delete []pUnicode;

		return _T("");
	}

	CString csRet(pUnicode);

	delete []pUtf8;
	delete []pUnicode;
	//delete []pGB;

	return csRet;
}

CString URLEncodeToGB(CString csUrlencode)
{
	if (csUrlencode.GetLength() == 0)
	{
		return _T("");
	}

// 	if (csUrlencode.GetLength()%3 != 0)
// 	{
// 		ASSERT(0);
// 		return _T("");
// 	}
	CStringA csUrlEncodeA(csUrlencode);

	CHAR * pUtf8=new CHAR[csUrlEncodeA.GetLength()+1];
	memset(pUtf8,0,csUrlEncodeA.GetLength()+1);

	int iDest = 0;
	for (int i = 0; i < csUrlEncodeA.GetLength(); )
	{
		if (_T('%') == csUrlEncodeA[i] && i < csUrlEncodeA.GetLength()-2)
		{
			CStringA strTmp(csUrlEncodeA[i+1]);
			strTmp += csUrlEncodeA[i+2];

			CHAR cTmp = (CHAR)strtol(strTmp, NULL, 16);	

			pUtf8[iDest++] = cTmp;

			i += 3;
		}
		else
		{
			pUtf8[iDest++] = csUrlEncodeA[i];

			i += 1;
		}
	}

	CString strGB = UTF8ToGB(pUtf8);

	delete []pUtf8;

	return strGB;
}

CString GBToURLEncode(CString csGB)
{
	if (csGB.GetLength() == 0)
	{
		return _T("");
	}

	CStringA strUtf(GBToUTF8(csGB));
	CStringA strURLEncode;
	for (int i = 0; i < strUtf.GetLength(); ++i)
	{
		BYTE by = (BYTE)strUtf[i];
		BYTE high = by / 16;
		BYTE low = by % 16;
		if (high > 9)
		{
			high = 'A' + (high - 10);
		}
		else
		{
			high = '0' + high;
		}
		if (low > 9)
		{
			low = 'A' + (low - 10);
		}
		else
		{
			low = '0' + low;
		}
		strURLEncode += "%";
		strURLEncode += high;
		strURLEncode += low;
	}
	return CString(strURLEncode);
}


#else


CString UTF8ToGB(CString csUTF8)
{
	if(csUTF8.GetLength()==0)
	{
		//	ASSERT(0);
		return "";
	}

	CHAR * pUtf8=new CHAR[csUTF8.GetLength()+1];
	memset(pUtf8,0,csUTF8.GetLength()+1);
	strcpy(pUtf8, csUTF8);

	//����unicode�����С
	int nUnicodeLen=MultiByteToWideChar(CP_UTF8,NULL,pUtf8,-1,NULL,0);

	if(nUnicodeLen<=0)
	{
		ASSERT(0);

		delete []pUtf8;

		return "";
	}

	//ת��Ϊunicode
	WCHAR * pUnicode=new WCHAR[nUnicodeLen];

	nUnicodeLen=MultiByteToWideChar(CP_UTF8,NULL,pUtf8,-1,pUnicode,nUnicodeLen);

	if(nUnicodeLen<=0)
	{
		ASSERT(0);

		delete []pUtf8;
		delete []pUnicode;

		return "";
	}

	//����GB�����С
	int nGBLen=WideCharToMultiByte(CP_ACP,0,pUnicode,-1,NULL,0,NULL,NULL);

	if(nGBLen<=0)
	{
		ASSERT(0);

		delete []pUtf8;
		delete []pUnicode;

		return "";
	}

	//ת����GB
	CHAR * pGB=new CHAR[nGBLen];

	nGBLen=WideCharToMultiByte(CP_ACP,0,pUnicode,-1,pGB,nGBLen,NULL,NULL);

	if(nGBLen<=0)
	{
		ASSERT(0);

		delete []pUtf8;
		delete []pUnicode;
		delete []pGB;

		return "";
	}

	//
	pGB[nGBLen-1]=0;//ֻ�Ǳ���һ��

	CString csRet=pGB;

	delete []pUtf8;
	delete []pUnicode;
	delete []pGB;

	return csRet;
}
#endif

