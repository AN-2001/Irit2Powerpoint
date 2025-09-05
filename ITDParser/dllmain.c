#include <windows.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/allocate.h"
#include "inc_irit/iritprsr.h"
#include "ITDParser.h"
#include "objectProcessor.h"

extern DWORD TLSIndex;
extern HANDLE gMutex;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TLSIndex = TlsAlloc();
        gMutex = CreateMutex(NULL, FALSE, NULL);
	I2P_LOG_TRACE( "Initialized ITD Parser." );
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        TlsFree(TLSIndex);
        CloseHandle(gMutex);
        break;
    }

    return TRUE;
}

