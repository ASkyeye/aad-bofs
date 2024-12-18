#ifndef BOFDEFS_H
#define BOFDEFS_H

#include <windows.h>

//MSVCRT
WINBASEAPI int __cdecl MSVCRT$printf(const char * _Format,...);
WINBASEAPI int __cdecl MSVCRT$wprintf(const wchar_t * _Format,...);
WINBASEAPI int __cdecl MSVCRT$swprintf_s(wchar_t *buffer, size_t sizeOfBuffer, const wchar_t *format, ...);
WINBASEAPI errno_t __cdecl MSVCRT$wcscpy_s(wchar_t *_Dst, rsize_t _DstSize, const wchar_t *_Src);
WINBASEAPI void *__cdecl MSVCRT$calloc(size_t _NumOfElements, size_t _SizeOfElements);
WINBASEAPI void *__cdecl MSVCRT$malloc(size_t size);
WINBASEAPI void __cdecl MSVCRT$free(void *_Memory);
WINBASEAPI int __cdecl MSVCRT$vsnprintf(char * __restrict__ d,size_t n,const char * __restrict__ format,va_list arg);
WINBASEAPI char* WINAPI MSVCRT$strstr(const char* str1, const char* str2);

#define malloc  MSVCRT$malloc
#define swprintf_s MSVCRT$swprintf_s

//OLE32
WINBASEAPI HRESULT WINAPI OLE32$CoInitializeEx (LPVOID pvReserved, DWORD dwCoInit);
WINBASEAPI HRESULT WINAPI OLE32$CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv);
WINBASEAPI void WINAPI OLE32$CoUninitialize(void);
WINBASEAPI void WINAPI OLE32$CoTaskMemFree(LPVOID pv);
WINBASEAPI HRESULT WINAPI OLE32$CoCreateGuid(GUID* pguid);
WINBASEAPI HRESULT WINAPI OLE32$StringFromCLSID(REFCLSID rclsid, LPOLESTR* lplpsz);

//KERNEL32
WINBASEAPI int WINAPI Kernel32$WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);
WINBASEAPI void * WINAPI KERNEL32$HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
WINBASEAPI HANDLE WINAPI KERNEL32$GetProcessHeap(void);
WINBASEAPI BOOL WINAPI KERNEL32$HeapFree(HANDLE, DWORD, PVOID);
WINBASEAPI void * WINAPI KERNEL32$VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI LPVOID WINAPI KERNEL32$VirtualAllocEx (HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI WINBOOL WINAPI KERNEL32$VirtualProtectEx (HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
WINBASEAPI int WINAPI KERNEL32$MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);

#define intAlloc(size) KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define intFree(addr) KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, addr)

//WINHTTP
WINBASEAPI HINTERNET WINAPI WINHTTP$WinHttpOpen(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags);
WINBASEAPI HINTERNET WINAPI WINHTTP$WinHttpConnect(HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved);
WINBASEAPI HINTERNET WINAPI WINHTTP$WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR *ppwszAcceptTypes, DWORD dwFlags);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpReceiveResponse(HINTERNET hRequest, LPVOID lpReserved);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpCloseHandle(HINTERNET hInternet);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpQueryHeaders(HINTERNET, DWORD, LPCWSTR, LPVOID, LPDWORD, LPDWORD);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpQueryDataAvailable(HINTERNET, LPDWORD);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpReadData(HINTERNET, LPVOID, DWORD, LPDWORD);
WINBASEAPI BOOL WINAPI WINHTTP$WinHttpAddRequestHeaders(HINTERNET, LPCWSTR, DWORD, DWORD);

//ADVAPI32
WINBASEAPI WINBOOL WINAPI ADVAPI32$OpenProcessToken (HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
WINBASEAPI WINBOOL WINAPI ADVAPI32$GetTokenInformation (HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);

typedef HRESULT WINAPI (*CoInitializeEx_t)(LPVOID pvReserved, DWORD dwCoInit);
typedef HRESULT WINAPI (*CoUninitialize_t)(void);
typedef HRESULT WINAPI (*CLSIDFromString_t)(LPCOLESTR lpsz, LPCLSID pclsid);
typedef HRESULT WINAPI (*IIDFromString_t)(LPCOLESTR lpsz, LPIID lpiid);
typedef HRESULT WINAPI (*CoCreateInstance_t)(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv);
typedef LPVOID WINAPI (*CoTaskMemFree_t)(LPVOID pv);
typedef HRESULT (WINAPI * COCREATEGUID)(GUID* pguid);
typedef HRESULT (WINAPI * STRINGFROMCLSID)(REFCLSID rclsid, LPOLESTR* lplpsz);

typedef HINTERNET (WINAPI * WINHTTPOPEN)(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD);
typedef HINTERNET (WINAPI * WINHTTPCONNECT)(HINTERNET, LPCWSTR, INTERNET_PORT, DWORD);
typedef HINTERNET (WINAPI * WINHTTPOPENREQUEST)(HINTERNET, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR*, DWORD);
typedef BOOL (WINAPI * WINHTTPSENDREQUEST)(HINTERNET, LPCWSTR, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR);
typedef BOOL (WINAPI * WINHTTPRECEIVERESPONSE)(HINTERNET, LPVOID);
typedef BOOL (WINAPI * WINHTTPCLOSEHANDLE)(HINTERNET);
typedef BOOL (WINAPI * WINHTTPQUERYHEADERS)(HINTERNET, DWORD, LPCWSTR, LPVOID, LPDWORD, LPDWORD);
typedef BOOL (WINAPI * WINHTTPQUERYDATAAVAILABLE)(HINTERNET, LPDWORD);
typedef BOOL (WINAPI * WINHTTPREADDATA)(HINTERNET, LPVOID, DWORD, LPDWORD);
typedef BOOL (WINAPI * WINHTTPADDREQUESTHEADERS)(HINTERNET, LPCWSTR, DWORD, DWORD);

typedef char* (WINAPI * STRSTR)(const char*, const char*);
typedef int (WINAPI * MULTIBYTETOWIDECHAR)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);

#endif
