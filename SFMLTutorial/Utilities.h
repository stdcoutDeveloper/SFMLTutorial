#pragma once

#define WIN32_LEAN_AND_MEAN // exclude some APIs for reducing Win32 header.
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <Shlwapi.h>
#include <tchar.h>
#include <string>

namespace Utilities
{
    /**
     * \brief W: Unicode
     */
    inline std::wstring GetWorkingDirectoryW()
    {
        HMODULE hModule = GetModuleHandle(nullptr);
        if (hModule)
        {
            TCHAR path[MAX_PATH];
            GetModuleFileName(hModule, path, sizeof(path));
            PathRemoveFileSpec(path);
            wcscat_s(path, _T("\\"));
            return std::wstring(path);
        }
        return _T("");
    }

    /**
     * \brief A: ANSI
     */
    inline std::string GetWorkingDirectoryA()
    {
        HMODULE hModule = GetModuleHandle(nullptr);
        if (hModule)
        {
            char path[MAX_PATH];
            GetModuleFileNameA(hModule, path, sizeof(path));
            PathRemoveFileSpecA(path);
            strcat_s(path, "\\");
            return std::string(path);
        }
        return "";
    }
}
