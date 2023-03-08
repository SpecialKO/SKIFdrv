// SKIFdrv.cpp : Defines the entry point for the application.
//

#include <shlwapi.h>
#include "utility.h"
#include "drvmgnt.h"

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
  UNREFERENCED_PARAMETER (hInstance);
  UNREFERENCED_PARAMETER (hPrevInstance);
  UNREFERENCED_PARAMETER (nCmdShow);

  // We assume everything is in the same folder as the executable
  ResetWorkingDirectory ( );

  wchar_t wszCurrentPath[MAX_PATH + 2] = { };
  GetCurrentDirectory   (MAX_PATH, wszCurrentPath);

  // Paths
  std::wstring log = SK_FormatStringW (LR"(%ws\%ws)", wszCurrentPath, LR"(SKIFdrv.log)"    );
  std::wstring drv = SK_FormatStringW (LR"(%ws\%ws)", wszCurrentPath, LR"(WinRing0x64.sys)");

  // Logging
  DeleteFile (log.c_str());

  plog::init (plog::debug, log.c_str(), 10000000, 1);

  PLOG_INFO << "SKIF Driver Manager (SKIFdrv) v 1.0";
  PLOG_INFO << "Built " __TIME__ ", " __DATE__;
  PLOG_INFO << SKIF_LOG_SEPARATOR;
  PLOG_INFO << "Working directory : " << wszCurrentPath;
  PLOG_INFO << "Driver location   : " << drv;
  PLOG_INFO << "Supported cmd arg : ";
  PLOG_INFO << "    Install - Installs the kernel driver on the system.";
  PLOG_INFO << "  Uninstall - Uninstalls the kernel driver from the system.";
  PLOG_INFO << SKIF_LOG_SEPARATOR;
  
  if (SK_IsAdmin())
  {
    if (FileExists (drv.c_str()))
    {
      PLOG_INFO << "Detected kernel driver at the assumed location.";

      // Uninstall
      if (StrStrIW(lpCmdLine, L"Uninstall") != NULL)
      {
        PLOG_INFO << "Detected 'Uninstall' cmd line argument.";
        
        if (SvcStop      ( ) &&
            SvcUninstall ( ))
        {
          PLOG_INFO << "Driver has been marked for deletion!";
          PLOG_INFO << "It will be uninstalled once all open handles to it have been closed.";
          SetLastError (NO_ERROR);
        }
      }

      // Install
      else if (StrStrIW (lpCmdLine, L"Install") != NULL)
      {
        PLOG_INFO << "Detected 'Install' cmd line argument.";
        
        if (SvcInstall (drv) &&
            SvcStart   (   ) &&
            DevOpen    (   ))
        {
          PLOG_INFO << "Driver has been successfully installed!";
          SetLastError (NO_ERROR);
        }
      }

      // No cmd line recognized
      else
      {
        PLOG_ERROR << "No cmd line argument was recognized!";
        MessageBox (NULL,
                    L"No cmd line argument was recognized!\n"
                    L"\n"
                    L"Supported arguments:\n"
                    L"Install - Installs the kernel driver on the system.\n"
                    L"Uninstall - Uninstalls the kernel driver from the system.",
                    L"SKIFdrv",
                    MB_OK | MB_ICONERROR
        );
      }
    }
    else
      PLOG_ERROR << "Kernel driver was not detected at the assumed location!";
  }
  else
  {
    PLOG_ERROR << "Requires running as an administrator to work properly!";
    MessageBox (NULL,
                L"Requires running as an administrator to work properly!\n"
                L"\n"
                L"Supported cmd line arguments:\n"
                L"Install - Installs the kernel driver on the system.\n"
                L"Uninstall - Uninstalls the kernel driver from the system.",
                L"SKIFdrv",
                MB_OK | MB_ICONERROR
    );
  }
  
  PLOG_INFO << SKIF_LOG_SEPARATOR;
  PLOG_INFO << "Process is finished! Now shutting down...";
  PLOG_INFO << SKIF_Util_GetLastError ( );

  if (NO_ERROR != GetLastError ( ))
    ShowErrorMessage (GetLastError ( ), L"An unexpected error occurred. Please consult the SKIFdrv.log for more information.", L"SKIFdrv");

  return GetLastError ( );
}