# SKIFdrv

Management tool for facilitating driver service (un)installation.

Special K can make use of the quite common `WinRing0x64.sys` driver to enable advanced CPU hardware monitoring capabilities in its CPU widget. This tool allows end-users to easily install and uninstall the driver and the necessary components, and is redistributed as a separate installer from the main Special K/SKIF package. The tool creates a driver service on the system called `SK_WinRing0` that autostarts the driver alongside the system boot, as this is required to allow unelevated games to make use of it.

The tool will eventually also support uninstalling the legacy driver service `WinRing0_1_2_0` to allow users to migrate over.

## Command line arguments

| Argument  | What it does |
| -------------: | ------------- |
| `<empty>`   | Will throw an error message with information about supported command line arguments. |
| `Install`   | Performs an install of the driver and service.  |
| `Uninstall` | Performs an uninstall of the driver and service.  |
| `Legacy`    | **Not implemented!** Intended to perform an **uninstall** of the legacy driver service `WinRing0_1_2_0`. Used to move over to the new `SK_WinRing0` name. |

## Notes

The NT Device name is hardcoded as `\\.\WinRing0_1_2_0` and might conflict with other driver services as a result as only one can be running simultaneously using the same NT Device name.
This should technically not be an issue because different apps can still interface with it.
