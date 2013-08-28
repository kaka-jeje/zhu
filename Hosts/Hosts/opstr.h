/** 
 *  Created by FreeCat(jhuix) in 20120711
 *  由金晖于20120711创建
 */
#ifndef __OPSTR_H
#define __OPSTR_H

#include <atlstr.h>

extern CString GetModuleName();
extern CString GetModulePathName();
extern LPCTSTR GetAppPath();
extern BOOL FileIsExist(LPCTSTR lpszFilename);
extern INT FilePathIsExist(LPCTSTR lpszFilePath);
extern ULONGLONG GetFileSize(const CString& strFilename);
// extern CString GetFileDate(const CString& strFilename);
// extern CString GetFileDateTime(CString strFilename);
extern CString ExtractFileName(CString strPath);
extern CString ExtractFilePath(CString strPath);
extern CString ExtractFileExtension(CString strFilename);
extern CString ExtractDrive(CString strPath);
extern BOOL DeleteFile(LPCTSTR lpszFileName, DWORD dwPlatformID);
extern VOID DeleteAllFiles(CString strFolder, BOOL bDeleteFolderToo = FALSE);
extern VOID DeleteAllFilesEx(CString strFolder, BOOL &bRuning, BOOL bDeleteFolderToo = FALSE);
extern BOOL CreateFolder(LPCTSTR lpszFolder);
extern INT CountFiles(CString strFolder, BOOL bSubfolders = TRUE);
extern INT GetFolderSize(CString strFolder, BOOL bSubfolders = TRUE);
extern BOOL Replace(CString& strText, const CString& strFind, const CString& strReplace);
extern int GetBaseByteLength(LPCSTR pszSrc);
extern int GetBaseByteLength(LPCWSTR pszSource);
extern CStringA ConvertUnicodeToString(LPCWSTR pSrc);
extern CStringW ConvertStringToUnicode(const char* pSrc);
extern CStringA ConvertBSTRToStringEx(BSTR pSrc);
extern BSTR ConvertStringToBSTREx(const char* pSrc);
extern UINT HashStringKey(LPCTSTR key);
extern int EncodeData(const char* szKey, int nKeyLen, char* szData, int nDataLen);
extern BOOL DecodeData(const char* szKey, int nKeyLen, int iFirstKey, char* szData, int nDataLen);
extern BOOL AdjustProcessPrivileges(LPCTSTR   szPrivilegesName);
extern DWORD GetParentProcessID(DWORD dwID);
extern DWORD GetProcessId(CString szExeName);
extern DWORD GetParentProcessId(CString szExeName);

#endif // __OPSTR_H
