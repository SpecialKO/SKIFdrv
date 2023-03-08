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


BOOL
FileExists (LPCTSTR szPath)
{
  DWORD    dwAttrib  = GetFileAttributes (szPath);

  return  (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         !(dwAttrib  & FILE_ATTRIBUTE_DIRECTORY));
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

  message = L" [" + std::to_wstring(GetLastError()) + L"] " + message;

  return message;
}


void
ResetWorkingDirectory (void)
{
  wchar_t path[MAX_PATH];
  GetModuleFileNameW   (nullptr, path, MAX_PATH);
  PathRemoveFileSpecW  (path);
  SetCurrentDirectoryW (path);
}


// Might not be necessary
void ShowErrorMessage (DWORD lastError)
{
  LPWSTR messageBuffer = nullptr;

  size_t size = FormatMessageW (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

  std::wstring message (messageBuffer, size);
  LocalFree (messageBuffer);

  message.erase (std::remove (message.begin(), message.end(), '\n'), message.end());

  MessageBox (NULL, (L"Unexpected error occurred:\n\n[" +std::to_wstring(GetLastError()) + L"] " + message).c_str(),
                     L"SKIFdrv", MB_OK | MB_ICONERROR);
}