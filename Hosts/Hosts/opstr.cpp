#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <TlHelp32.h>
#include "opstr.h"

#ifndef MAKEINT64
#define MAKEINT64(a,b)	((LONGLONG)(((DWORD)(a)) | ((ULONGLONG)((DWORD)(b))) << 32))
#endif

static UINT sResIDBegin = 55555;

CString GetModuleName()
{
	CString strModuleName;
	::GetModuleFileName(NULL,strModuleName.GetBuffer(MAX_PATH),MAX_PATH);
	strModuleName.ReleaseBuffer();
	return strModuleName;
}

CString GetModulePathName()
{
	CString strModuleName;
	::GetModuleFileName(NULL,strModuleName.GetBuffer(MAX_PATH),MAX_PATH);
	strModuleName.ReleaseBuffer();
	int iPos = strModuleName.ReverseFind(TEXT('\\'));
	if (iPos != -1)
	{
		return strModuleName.Left(iPos+1);
	}
	return TEXT("");
}

LPCTSTR GetAppPath()
{
	static CString g_strAppPath;
	if (g_strAppPath.IsEmpty())
	{
		g_strAppPath = GetModulePathName();
	}
	return (LPCTSTR)g_strAppPath;
}

//=====================================================================
BOOL FileIsExist(LPCTSTR lpszFilename)
{
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	if (::GetFileAttributesEx(lpszFilename,GetFileExInfoStandard,&fileData) != 0)
	{
		if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			return TRUE;
		}
	}
	return FALSE;
}
//查找本地是否存在对应文件或者目录
INT FilePathIsExist(LPCTSTR lpszFilePath)
{
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	if (::GetFileAttributesEx(lpszFilePath,GetFileExInfoStandard,&fileData) != 0)
	{
		if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			return 1; //文件
		}
		return 2; //目录
	}
	return 0; //不存在
}

ULONGLONG GetFileSize(const CString& strFilename)
{
	ULONGLONG ulFileSize = -1;
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	if (::GetFileAttributesEx(strFilename,GetFileExInfoStandard,&fileData) != 0)
	{
		if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			ulFileSize = ((((ULONGLONG)fileData.nFileSizeHigh) << 32) & 0xFFFFFFFF00000000) | fileData.nFileSizeLow;
		}
	}
	return ulFileSize;
}
//=====================================================================

// CString GetFileDate(const CString& strFilename)
// {
// 	return NULL;
// }
// 
// CString GetFileDateTime(CString strFilename)
// {
// 	return NULL;
// }

CString ExtractFileName(CString strPath)
{
	int iPosLeft = strPath.ReverseFind(TEXT('\\'));
	int iPosRight = strPath.ReverseFind(TEXT('/'));
	if (iPosLeft != -1 || iPosRight != -1)
	{
		int iPos = max(iPosLeft,iPosRight) + 1;
		return strPath.Right(strPath.GetLength()-iPos);
	}
	return strPath;
}

CString ExtractFilePath(CString strPath)
{
	int iPosLeft = strPath.ReverseFind(TEXT('\\'));
	int iPosRight = strPath.ReverseFind(TEXT('/'));
	if (iPosLeft != -1 || iPosRight != -1)
	{
		int iPos = max(iPosLeft,iPosRight) + 1;
		return strPath.Left(iPos);
	}
	return strPath;
}

CString ExtractFileExtension(CString strFilename)
{
	CString strExtension;
	int iPos = strFilename.ReverseFind(TEXT('.'));
	if (iPos != -1)
	{
		strExtension = strFilename.Mid(iPos + 1);
	}
	return strExtension;
}

CString ExtractDrive(CString strPath)
{
	int iPos = strPath.Find(TEXT(':'));
	if (iPos != -1)
	{
		return strPath.Left(iPos+1);
	}
	return strPath;
}


BOOL DeleteFile(LPCTSTR lpszFileName, DWORD dwPlatformID)
{
	// Delete file
	if (!::DeleteFile(lpszFileName))
	{
		if( VER_PLATFORM_WIN32_NT == dwPlatformID) //WINNT系列 
		{
			CString strTemp = lpszFileName;
			strTemp += _T(".tmp");
			//源文件文件改名成重新启动后自动删除
			//rename current file to file.tmp
			if(!MoveFileEx(lpszFileName, strTemp, MOVEFILE_REPLACE_EXISTING)
				|| !MoveFileEx(strTemp, NULL, MOVEFILE_DELAY_UNTIL_REBOOT|MOVEFILE_REPLACE_EXISTING))
				return FALSE;
		}
		else if (VER_PLATFORM_WIN32_WINDOWS == dwPlatformID) //win98
		{
			CString strOldFileName,strIniPathName;
			::GetWindowsDirectory(strIniPathName.GetBuffer(MAX_PATH),MAX_PATH);
			strIniPathName.ReleaseBuffer();
			strIniPathName += _T("\\wininit.ini");
			::GetShortPathName(lpszFileName,strOldFileName.GetBuffer(MAX_PATH),MAX_PATH);
			strOldFileName.ReleaseBuffer();
			if (!::WritePrivateProfileString(TEXT("rename"), _T("NUL"), strOldFileName,strIniPathName))
				return FALSE;
		}
	}
	return TRUE;
}

void DeleteAllFiles(CString strFolder, BOOL bDeleteFolderToo)
{
	if (strFolder.IsEmpty())
		return;

	if (strFolder.GetAt(strFolder.GetLength()-1) != TEXT('\\'))
		strFolder += TEXT('\\');

	DWORD dwPlatformID = VER_PLATFORM_WIN32s;
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((OSVERSIONINFO*)&osvi))
	{
		osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		if(GetVersionEx((OSVERSIONINFO*)&osvi))
		{
			dwPlatformID = osvi.dwPlatformId;
		}
	}
	else
	{
		dwPlatformID = osvi.dwPlatformId;
	}

	WIN32_FIND_DATA wfd;
	HANDLE hFile;
	DWORD dwFileAttr;
	CString strFile;
	CString strSpec = strFolder;
	strSpec += TEXT("*.*");
	CString strPathFile;

	// Find the first file
	hFile = FindFirstFile(strSpec, &wfd);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			strFile = wfd.cFileName;
			strPathFile = strFolder + strFile;
			// Get the file attributes
			dwFileAttr = GetFileAttributes(strPathFile);

			// See if file is read-only : if so unset read-only
			if (dwFileAttr & FILE_ATTRIBUTE_READONLY)
			{
				dwFileAttr &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(strPathFile, dwFileAttr);
			}

			// See if the file is a directory
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Make sure it isn't current or parent directory
				if (strFile != TEXT(".") && strFile != TEXT(".."))
				{
					strPathFile += TEXT("\\");
					// Recursively delete all files in this folder
					DeleteAllFiles(strPathFile, TRUE);

					// Remove the directory
					//RemoveDirectory(strPathFile);
				}
			}
			else
			{
				// Delete the file
				//DeleteFile(strPathFile);
				DeleteFile(strPathFile, dwPlatformID);
			}
		}
		while (FindNextFile(hFile, &wfd));
	}
	
	// Close handle to file
	FindClose(hFile);

	// Delete directory too if needed
	if (bDeleteFolderToo)
	{
		RemoveDirectory(strFolder);
	}
}

//进程，可能自由控制退出
VOID DeleteAllFilesEx(CString strFolder, BOOL &bRuning, BOOL bDeleteFolderToo /*= FALSE*/)
{
	if (!bRuning)
	{
		return;
	}

	if (strFolder.IsEmpty())
		return;

	if (strFolder.GetAt(strFolder.GetLength()-1) != TEXT('\\'))
		strFolder += TEXT('\\');

	DWORD dwPlatformID = VER_PLATFORM_WIN32s;
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((OSVERSIONINFO*)&osvi))
	{
		osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		if(GetVersionEx((OSVERSIONINFO*)&osvi))
		{
			dwPlatformID = osvi.dwPlatformId;
		}
	}
	else
	{
		dwPlatformID = osvi.dwPlatformId;
	}

	WIN32_FIND_DATA wfd;
	HANDLE hFile;
	DWORD dwFileAttr;
	CString strFile;
	CString strSpec = strFolder;
	strSpec += TEXT("*.*");
	CString strPathFile;

	// Find the first file
	hFile = FindFirstFile(strSpec, &wfd);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!bRuning)
			{
				break;
			}

			strFile = wfd.cFileName;
			strPathFile = strFolder + strFile;
			// Get the file attributes
			dwFileAttr = GetFileAttributes(strPathFile);

			// See if file is read-only : if so unset read-only
			if (dwFileAttr & FILE_ATTRIBUTE_READONLY)
			{
				dwFileAttr &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(strPathFile, dwFileAttr);
			}

			// See if the file is a directory
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Make sure it isn't current or parent directory
				if (strFile != TEXT(".") && strFile != TEXT(".."))
				{
					strPathFile += TEXT("\\");
					// Recursively delete all files in this folder
					DeleteAllFilesEx(strPathFile, bRuning, TRUE);

					// Remove the directory
					//RemoveDirectory(strPathFile);
				}
			}
			else
			{
				// Delete the file
				//DeleteFile(strPathFile);
				DeleteFile(strPathFile, dwPlatformID);
			}
		}
		while (FindNextFile(hFile, &wfd));
	}

	// Close handle to file
	FindClose(hFile);

	// Delete directory too if needed
	if (bDeleteFolderToo)
	{
		RemoveDirectory(strFolder);
	}
}

BOOL CreateFolder(LPCTSTR lpszFolder)
{
	// Declare variables
	CString strFolder = lpszFolder;
	if (strFolder.IsEmpty())
		return FALSE;

	if (strFolder[strFolder.GetLength()-1] == TEXT('\\'))
	{
		strFolder.Delete(strFolder.GetLength()-1);
	}

	//Is folder or foldername's filealready existing
	DWORD dwAttrib = ::GetFileAttributes(strFolder);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib)
	{
		return (BOOL)(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
	}

	CString strTemp = lpszFolder;
	int iPos = strTemp.ReverseFind(TEXT('\\'));
	if (iPos != -1) 
	{
		strTemp.SetAt(iPos, 0);
		if (!CreateFolder(strTemp))
		{
			return FALSE;
		}
		
		// create directory
		if (!::CreateDirectory(strFolder, NULL)) 
			return FALSE;
	}
	
	// If we get here, function is successful
	return TRUE;	
}

//=====================================================================

int CountFiles(CString strFolder, BOOL bSubfolders)
{
	// Declare variables
	int iCount = 0;
	CString	strFilePath, sPattern;
	HANDLE hFile;
	WIN32_FIND_DATA FileInformation;

	// Get pattern
	sPattern = strFolder + TEXT("\\*.*");
	hFile = ::FindFirstFile(sPattern, &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			// Is it something else then current folder?
			if (FileInformation.cFileName[0] != TEXT('.'))
			{
				// Set up path
				strFilePath = strFolder + TEXT("\\") + FileInformation.cFileName;

				// Is it a folder?
				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// Should we search subdirectories too?
					if (bSubfolders)
					{
						// Search subdirectory
						int iRC = CountFiles(strFilePath, bSubfolders);

						// Check result of function
						iCount += iRC;
					}
				}
				else
				{
					// Increase counter
					++iCount;
				}
			}
		} while (::FindNextFile(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);
	}

	// Return count
	return iCount;
}

//=====================================================================

int	GetFolderSize(CString strFolder, BOOL bSubfolders)
{
	// Declare variables
	int iSize = 0;
	CString	strFilePath, sPattern;
	HANDLE hFile;
	WIN32_FIND_DATA FileInformation;

	// Get pattern
	sPattern = strFolder + TEXT("\\*.*");
	hFile = ::FindFirstFile(sPattern, &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			// Is it something else then current folder?
			if (FileInformation.cFileName[0] != TEXT('.'))
			{
				// Set up path
				strFilePath = strFolder + TEXT("\\") + FileInformation.cFileName;

				// Is it a folder?
				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// Should we search subdirectories too?
					if (bSubfolders)
					{
						// Search subdirectory
						int iRS = GetFolderSize(strFilePath, bSubfolders);

						// Check result of function
						iSize += iRS;
					}
				}
				else
				{
					// Increase size
					if (FileInformation.nFileSizeHigh > 0)
					{
						iSize += (FileInformation.nFileSizeHigh * MAXDWORD) + 
								 FileInformation.nFileSizeLow;
					}
					else
					{
						iSize += FileInformation.nFileSizeLow;
					}
				}
			}
		} while (::FindNextFile(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);
	}

	// Return count
	return iSize;
}

BOOL Replace(CString& strText, const CString& strFind, const CString& strReplace)
{
	// Convert to lowercase to check where we have to replace
	CString strTextTemp = strText;
	CString strFindTemp = strFind;
	strTextTemp.MakeLower();
	strFindTemp.MakeLower();

	BOOL bRet = FALSE;
	int iBeginTemp = 0, iBegin = 0, iOffset = 0;
	// Search for strFindTemp
	while((iBeginTemp = strTextTemp.Find((LPCTSTR)strFindTemp,iBeginTemp)) != -1)
	{
		iBegin = iBeginTemp + iOffset;
		strText.Delete(iBegin,strFindTemp.GetLength());
		strText.Insert(iBegin,(LPCTSTR)strReplace);
		iBeginTemp += strFindTemp.GetLength();
		iOffset += strReplace.GetLength() - strFindTemp.GetLength();
		bRet = TRUE;
	}
	return bRet;
}

int FindNoCase(CString strText, CString strFind)
{
	// Convert to lowercase to check where we have to replace
	strText.MakeLower();
	strFind.MakeLower();

	// Search for strFindTemp
	return strText.Find((LPCTSTR)strFind);
}

int GetBaseByteLength(LPCSTR pszSrc)
{
	if (!pszSrc)
		return 0;

	// Returns required buffer length in XCHARs
	return int(strlen(pszSrc));
}

int GetBaseByteLength(LPCWSTR pszSource)
{
	if (!pszSource)
		return 0;

	// Returns required buffer length in XCHARs
	return ::WideCharToMultiByte(_AtlGetConversionACP(), 0, pszSource, -1, NULL, 0, NULL, NULL) - 1;
}

#ifndef CP_GB18030
#define CP_GB18030		54936
#endif

CStringA ConvertUnicodeToString(LPCWSTR pSrc)
{
	CStringA strResult;
	UINT nCodePage = CP_GB18030;
	if (!IsValidCodePage(CP_GB18030))
	{
		nCodePage = _AtlGetConversionACP();
	}
	int iBytes = ::WideCharToMultiByte(nCodePage, 0, pSrc, -1, NULL, 0, NULL, NULL);
	if (iBytes)
	{
		iBytes = ::WideCharToMultiByte(nCodePage, 0, pSrc, -1, strResult.GetBuffer(iBytes), iBytes, NULL, NULL);
		strResult.GetBuffer()[iBytes] = 0;
		strResult.ReleaseBufferSetLength(iBytes);
	}
	return strResult;
}

CStringW ConvertStringToUnicode(const char* pSrc)
{
	CStringW strResult;
	UINT nCodePage = CP_GB18030;
	if (!IsValidCodePage(CP_GB18030))
	{
		nCodePage = _AtlGetConversionACP();
	}
	int iLength = ::MultiByteToWideChar(nCodePage, 0, pSrc, -1, NULL, 0);
	if (iLength)
	{
		iLength = ::MultiByteToWideChar(nCodePage, 0, pSrc, -1, strResult.GetBuffer(iLength), iLength);
		strResult.GetBuffer()[iLength] = 0;
		strResult.ReleaseBufferSetLength(iLength);
	}
	return strResult;
}

CStringA ConvertBSTRToStringEx(BSTR pSrc)
{
	return ConvertUnicodeToString((LPCWSTR)pSrc);
}

BSTR ConvertStringToBSTREx(const char* pSrc)
{
	return ConvertStringToUnicode(pSrc).AllocSysString();
}

UINT HashStringKey(LPCTSTR key)
{
	UINT nHash = 0;
	if (key != NULL)
	{
		while (*key)
			nHash = (nHash<<5) + nHash + *key++;
	}
	return nHash;
}

int EncodeData(const char* szKey, int nKeyLen, char* szData, int nDataLen)
{
	if (NULL == szKey || nKeyLen <= 0 || NULL == szData || nDataLen <= 0)
		return FALSE;

	//随机数初始化
	::srand((unsigned)time(NULL));
	int iFirstKey = (::rand() * (nKeyLen - 1)) / RAND_MAX;
	int j = iFirstKey;
	for (int i = 0; i < nDataLen; i++, j++)
	{
		if (i >= (nKeyLen - iFirstKey))
			j = 0;

		szData[i] ^= ~szKey[j];
	}
	return iFirstKey;
}

BOOL DecodeData(const char* szKey, int nKeyLen, int iFirstKey, char* szData, int nDataLen)
{
	if (NULL == szKey || nKeyLen <= 0 || NULL == szData || nDataLen <= 0 || iFirstKey < 0)
		return FALSE;

	if (iFirstKey >= nKeyLen)
		iFirstKey = 0;

	int j = iFirstKey;
	for (int i = 0; i < nDataLen; i++, j++)
	{
		if (i >= (nKeyLen - iFirstKey))
			j = 0;

		szData[i] ^= ~szKey[j];
	}
	return TRUE;
}

BOOL AdjustProcessPrivileges(LPCTSTR   szPrivilegesName)   
{   
	HANDLE   hToken;     
	TOKEN_PRIVILEGES   tkp;   

	if(!OpenProcessToken(GetCurrentProcess(),   
		TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))   
	{   
		return   FALSE;   
	}   

	if(!LookupPrivilegeValue(NULL,szPrivilegesName,   
		&tkp.Privileges[0].Luid))   
	{   
		CloseHandle(hToken);   
		return   FALSE;   
	}   

	tkp.PrivilegeCount   =   1;   
	tkp.Privileges[0].Attributes   =   SE_PRIVILEGE_ENABLED;   

	if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,NULL))   
	{   
		CloseHandle(hToken);   
		return   FALSE;   
	}   

	CloseHandle(hToken);   
	return   TRUE;   
}

typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);  


PROCNTQSIP NtQueryInformationProcess;  

typedef struct  
{ 
	DWORD ExitStatus;  
	DWORD PebBaseAddress;  
	DWORD AffinityMask;  
	DWORD BasePriority;  
	ULONG UniqueProcessId;  
	ULONG InheritedFromUniqueProcessId;  
} PROCESS_BASIC_INFORMATION;


DWORD GetParentProcessID(DWORD dwID)
{
	LONG status = 0;
	DWORD dwParentID = (DWORD)-1;
	HANDLE hParent;
	PROCESS_BASIC_INFORMATION pbi;
	memset(&pbi, 0, sizeof(PROCESS_BASIC_INFORMATION));
	AdjustProcessPrivileges(SE_DEBUG_NAME);

	hParent = OpenProcess(PROCESS_VM_READ, FALSE, dwID);
	if(!hParent)
	{
		return (DWORD)-1;
	}

	NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle(_T("ntdll")), "NtQueryInformationProcess"); 
	status = NtQueryInformationProcess( hParent,  
		                                0,  
	                                    (PVOID)&pbi,  
		                                sizeof(PROCESS_BASIC_INFORMATION),  
	                                    NULL);  
	if(!status)  
		dwParentID = pbi.InheritedFromUniqueProcessId;

	CloseHandle(hParent); 

	return dwParentID;
}

DWORD GetProcessId(CString szExeName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hPro = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hPro == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	BOOL bEnd = Process32First(hPro, &pe32);
	while(bEnd)
	{
		if(szExeName.CompareNoCase(pe32.szExeFile) == 0)
		{
			return pe32.th32ProcessID;
		}
		bEnd = Process32Next(hPro, &pe32);
	}
	return 0;
}

DWORD GetParentProcessId(CString szExeName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hPro = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hPro == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	BOOL bEnd = Process32First(hPro, &pe32);
	while(bEnd)
	{
		if(szExeName.CompareNoCase(pe32.szExeFile) == 0)
		{
			return pe32.th32ParentProcessID;
		}
		bEnd = Process32Next(hPro, &pe32);
	}
	return 0;
}