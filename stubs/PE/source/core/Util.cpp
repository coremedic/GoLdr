#include "core/Util.hpp"
#include "core/Macros.hpp"
#include <windows.h>

/*!
 * @brief
 * Fetch PE section base
 * @param module
 * Pointer to module
 * @param sectionName
 * Name of PE section
 * @return
 * Pointer to base of section
 */
PVOID FetchPeSectionBase(IN PVOID module, IN PCHAR sectionName) {
    PIMAGE_NT_HEADERS       ntHeader = {};
    PIMAGE_SECTION_HEADER   sectionHeader = {};

    ntHeader = static_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<PVOID>(U_PTR(module) + static_cast<PIMAGE_DOS_HEADER>(module)->e_lfanew));
    sectionHeader = IMAGE_FIRST_SECTION(ntHeader);

    // Loop through sections
    for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++) {
        if (memcmp(sectionName, sectionHeader->Name, strlen(sectionName)) == 0) {
            return C_PTR(U_PTR(module) + sectionHeader[i].VirtualAddress);
        }
    }

    return nullptr;
}

