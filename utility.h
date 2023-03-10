#pragma once

#include "SKIFdrv.h"
#include <atlbase.h>

class SK_AutoHandle : public CHandle
{
  // Signed handles are invalid, since handles are pointers and
  //   the signed half of the address space is only for kernel

public:
  SK_AutoHandle(HANDLE hHandle) noexcept : CHandle(hHandle) { };
  ~SK_AutoHandle(void)           noexcept
  {
    // We cannot close these handles because technically they
    //   were never opened (by usermode code).
    if ((intptr_t)m_h < (intptr_t)nullptr)
      m_h = nullptr;

    // Signed handles are often special cases
    //   such as -2 = Current Thread, -1 = Current Process
  }

  const HANDLE& get(void) const noexcept { return m_h; };
};

bool SK_IsAdmin (void);
std::wstring __cdecl SK_FormatStringW (wchar_t const* const _Format, ...);
BOOL FileExists (LPCTSTR szPath);
void ResetWorkingDirectory (void);
std::wstring SKIF_Util_GetLastError (void);
void ShowErrorMessage (DWORD lastError, std::wstring preMsg = L"", std::wstring winTitle = L"");