#include "drvmgnt.h"
#include "utility.h"

// Install the driver service
bool SvcInstall (std::wstring svcName, std::wstring fullPath)
{
  bool ret = false;

  SC_HANDLE schSCManager  = NULL,
            schSCWinRing0 = NULL;

  schSCManager =
    OpenSCManager (
      nullptr,                  // localhost
      nullptr,                  // ServicesActive database
      SC_MANAGER_CREATE_SERVICE // Desired Access
    );

  if (nullptr != schSCManager)
  {
    schSCWinRing0 =
      CreateService (
        schSCManager,
        svcName.c_str(),        // Service Name
        svcName.c_str(),        // Display Name
        SERVICE_ALL_ACCESS,     // Desired Access
        SERVICE_KERNEL_DRIVER,  // Service Type
        SERVICE_SYSTEM_START,   // Start Method
        SERVICE_ERROR_NORMAL,   // Error Control
        fullPath.c_str(),       // Fully qualified path
        NULL,                   // Load Order Group (none)
        NULL,                   // Tag ID (none)
        NULL,                   // Dependencies (none)
        NULL,                   // Driver Object Name (unused; doesn't seem to work)
        NULL                    // Password (none)
      );

    if (nullptr != schSCWinRing0)
    {
      PLOG_INFO << "Driver service " << svcName << " was successfully installed.";
      CloseServiceHandle (schSCWinRing0);
      ret = true;
    }
    else if (ERROR_SERVICE_EXISTS == GetLastError ( ))
    {
      PLOG_WARNING << "Driver service " << svcName << " already exists.";
      ret = true;
    }
    else
      PLOG_ERROR << "Could not create the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

    CloseServiceHandle (schSCManager);
  } else
    PLOG_ERROR << "Could not open the service manager: " << SKIF_Util_GetLastError ( );

  return ret;
}


// Uninstall the driver service
bool SvcUninstall (std::wstring svcName)
{
  bool ret = false;

  SC_HANDLE schSCManager  = NULL,
            schSCWinRing0 = NULL;

  schSCManager =
    OpenSCManager (
      nullptr,                  // localhost
      nullptr,                  // ServicesActive database
      GENERIC_READ              // Desired Access
    );

  if (nullptr != schSCManager)
  {
    schSCWinRing0 =
      OpenService (
        schSCManager,
        svcName.c_str(),        // Service Name
        DELETE                  // Desired Access
      );

    if (nullptr != schSCWinRing0)
    {
      if (DeleteService (schSCWinRing0))
      {
        PLOG_INFO << "Driver service " << svcName << " was marked for deletion.";
        ret = true;
      }
      else if (ERROR_SERVICE_MARKED_FOR_DELETE == GetLastError ( ))
      {
        PLOG_INFO << "Driver service " << svcName << " have already been marked for deletion.";
        ret = true;
      }
      else
        PLOG_ERROR << "Could not uninstall the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

      CloseServiceHandle (schSCWinRing0);
    }
    else if (ERROR_SERVICE_DOES_NOT_EXIST == GetLastError ( ))
    {
      PLOG_INFO << "Driver service " << svcName << " have already been deleted.";
      // It is technically not "correct" to return true here when the
      //   service does not exist, but in our case we do so since the
      //     whole point of stopping the service is to uninstall it.
      ret = true;
    }
    else
      PLOG_ERROR << "Could not open the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

    CloseServiceHandle (schSCManager);
  } else
    PLOG_ERROR << "Could not open the service manager: " << SKIF_Util_GetLastError ( );

  return ret;
}


// Start the driver service
bool SvcStart (std::wstring svcName)
{
  bool ret = false;

  SC_HANDLE schSCManager  = NULL,
            schSCWinRing0 = NULL;

  schSCManager =
    OpenSCManager (
      nullptr,                  // localhost
      nullptr,                  // ServicesActive database
      GENERIC_READ              // Desired Access
    );

  if (nullptr != schSCManager)
  {
    schSCWinRing0 =
      OpenService (
        schSCManager,
        svcName.c_str(),        // Service Name
        SERVICE_START           // Desired Access
      );

    if (nullptr != schSCWinRing0)
    {
      if (StartService (schSCWinRing0, 0, nullptr))
      {
        PLOG_INFO << "Driver service " << svcName << " was successfully started.";
        ret = true;
      }
      else if (ERROR_SERVICE_ALREADY_RUNNING == GetLastError ( ))
      {
        PLOG_INFO << "Driver service " << svcName << " was already running.";
        ret = true;
      }
      else if (ERROR_ALREADY_EXISTS == GetLastError ( ))
      {
        // It is technically not "correct" to return true here when we could not
        //   start up the service because the NT device already exist, but in our
        //     case we do so since it should resolve itself after a system restart.
        PLOG_WARNING << "Could not start the " << svcName << " driver service because the NT device already exists.";
        PLOG_WARNING << "This is typically an indication of another service already running that relies on the same driver.";
        PLOG_WARNING << "The system might need to be restarted before Special K's service can be started properly.";
        ret = true;
      }
      else
        PLOG_ERROR << "Could not start the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

      CloseServiceHandle (schSCWinRing0);
    }
    else
      PLOG_ERROR << "Could not open the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

    CloseServiceHandle (schSCManager);
  } else
    PLOG_ERROR << "Could not open the service manager: " << SKIF_Util_GetLastError ( );

  return ret;
}


// Stop the driver service
bool SvcStop (std::wstring svcName)
{
  bool ret = false;

  SC_HANDLE schSCManager  = NULL,
            schSCWinRing0 = NULL;

  schSCManager =
    OpenSCManager (
      nullptr,                  // localhost
      nullptr,                  // ServicesActive database
      GENERIC_READ              // Desired Access
    );

  if (nullptr != schSCManager)
  {
    schSCWinRing0 =
      OpenService (
        schSCManager,
        svcName.c_str(),        // Service Name
        SERVICE_STOP            // Desired Access
      );

    if (nullptr != schSCWinRing0)
    {
      SERVICE_STATUS status;

      if (ControlService (schSCWinRing0, SERVICE_CONTROL_STOP, &status))
      {
        PLOG_INFO << "Driver service " << svcName << " was signaled to stop.";
        ret = true;
      }
      else if (ERROR_SERVICE_NOT_ACTIVE == GetLastError ( ))
      {
        PLOG_INFO << "Driver service " << svcName << " is already stopped.";
        ret = true;
      }
      else
        PLOG_ERROR << "Could not stop the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

      CloseServiceHandle (schSCWinRing0);
    }
    else if (ERROR_SERVICE_DOES_NOT_EXIST == GetLastError ( ))
    {
      PLOG_INFO << "Driver service " << svcName << " have already been deleted.";
      // It is technically not "correct" to return true here when the
      //   service does not exist, but in our case we do so since the
      //     whole point of stopping the service is to uninstall it.
      ret = true;
    }
    else
      PLOG_ERROR << "Could not open the " << svcName << " driver service: " << SKIF_Util_GetLastError ( );

    CloseServiceHandle (schSCManager);
  } else
    PLOG_ERROR << "Could not open the service manager: " << SKIF_Util_GetLastError ( );

  return ret;
}


// Open the device to ensure the driver was installed properly
bool DevOpen (std::wstring devName)
{
  bool ret = false;

  HANDLE h = CreateFile (
    devName.c_str(),              // DeviceName / FileName
    GENERIC_READ | GENERIC_WRITE, // Desired Access
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  if (INVALID_HANDLE_VALUE == h)
  {
    PLOG_WARNING << "Could not open the " << devName << " NT device: " << SKIF_Util_GetLastError();
  }
  else
  {
    PLOG_INFO << "NT device " << devName << " was succesfully opened.";
    ret = true;
    CloseHandle (h);
  }

  return ret;
}