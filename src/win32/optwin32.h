#pragma once

// Forward declarations for optional Win32 API procedures
// implemented in i_main.cpp

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>

#include "i_module.h"

// vanilla mingw is missing this type
#if defined(__MINGW32__) && !defined(REFKNOWNFOLDERID)
#define REFKNOWNFOLDERID const GUID &

#define KF_FLAG_CREATE 0x00008000
#endif

extern FModule Kernel32Module;
extern FModule Shell32Module;
extern FModule User32Module;

namespace OptWin32 {

extern TOptProc<Shell32Module, HRESULT(WINAPI*)(HWND, int, HANDLE, DWORD, LPTSTR)> SHGetFolderPathA;
extern TOptProc<Shell32Module, HRESULT(WINAPI*)(REFKNOWNFOLDERID, DWORD, HANDLE, PWSTR *)> SHGetKnownFolderPath;
extern TOptProc<Kernel32Module, DWORD (WINAPI*)(LPCTSTR, LPTSTR, DWORD)> GetLongPathNameA;
extern TOptProc<User32Module, BOOL(WINAPI*)(HMONITOR, LPMONITORINFO)> GetMonitorInfoA;

} // namespace OptWin32
