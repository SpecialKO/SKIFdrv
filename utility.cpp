#include "utility.h"


bool
SK_IsAdmin (void)
{
  bool          bRet = false;
  SK_AutoHandle hToken (INVALID_HANDLE_VALUE);

  if ( OpenProcessToken ( GetCurrentProcess ( ),
                            TOKEN_QUERY,
                              &hToken.m_h )
     )
  {
    TOKEN_ELEVATION Elevation = { };

    DWORD cbSize =
      sizeof (TOKEN_ELEVATION);

    if ( GetTokenInformation ( hToken.m_h,
                                 TokenElevation,
                                   &Elevation,
                                     sizeof (Elevation),
                                       &cbSize )
       )
    {
      bRet =
        ( Elevation.TokenIsElevated != 0 );
    }
  }

  return bRet;
}


// Suppress warnings about _vsnwprintf
#pragma warning(disable:4996)
std::wstring
__cdecl
SK_FormatStringW (wchar_t const* const _Format, ...)
{
  size_t len = 0;

  va_list   _ArgList;
  va_start (_ArgList, _Format);
  {
    len =
      _vsnwprintf ( nullptr, 0, _Format, _ArgList ) + 1ui64;
  }
  va_end   (_ArgList);

  size_t alloc_size =
    sizeof (wchar_t) * (len + 2);

  std::unique_ptr <wchar_t []> pData =
    std::make_unique <wchar_t []> (alloc_size);

  if (! pData)
    return std::wstring ();

  va_start (_ArgList, _Format);
  {
    len =
      _vsnwprintf ( (wchar_t *)pData.get (), len + 1, _Format, _ArgList );
  }
  va_end   (_ArgList);

  return
    pData.get ();
}


BOOL
FileExists (LPCTSTR szPath)
{
  DWORD    dwAttrib  = GetFileAttributes (szPath);

  return  (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         !(dwAttrib  & FILE_ATTRIBUTE_DIRECTORY));
}


void
ResetWorkingDirectory (void)
{
  wchar_t path[MAX_PATH];
  GetModuleFileNameW   (nullptr, path, MAX_PATH);
  PathRemoveFileSpecW  (path);
  SetCurrentDirectoryW (path);
}


std::wstring
SKIF_Util_GetLastError (void)
{
  LPWSTR messageBuffer = nullptr;

  size_t size = FormatMessageW (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, GetLastError ( ), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

  std::wstring message (messageBuffer, size);
  LocalFree (messageBuffer);

  message.erase (std::remove (message.begin(), message.end(), '\n'), message.end());

  message = L"[" + std::to_wstring (GetLastError()) + L"] " + message;

  return message;
}


// Currently unused
void
ShowErrorMessage (DWORD lastError, std::wstring preMsg = L"", std::wstring winTitle = L"")
{
  LPWSTR messageBuffer = nullptr;

  size_t size = FormatMessageW (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

  std::wstring message (messageBuffer, size);
  LocalFree (messageBuffer);

  message.erase (std::remove (message.begin(), message.end(), '\n'), message.end());

  if (! preMsg.empty())
    preMsg += L"\n\n";

  MessageBox (NULL, (preMsg + L"[" + std::to_wstring (lastError) + L"] " + message).c_str(),
                     winTitle.c_str(), MB_OK | MB_ICONERROR);
}