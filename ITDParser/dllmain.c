#include <windows.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/allocate.h"
#include "inc_irit/iritprsr.h"
#include "ITDParser.h"

extern DWORD TLSIndex;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	IritPrsrSetPolyListCirc(TRUE);
	IritPrsrSetFlattenObjects(FALSE);
	IritPrsrFlattenInvisibleObjects(FALSE);
        TLSIndex = TlsAlloc();

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        TlsFree(TLSIndex);
        break;
    }
    return TRUE;
}

