# SKIFdrv

Management tool for facilitating driver service installation and uninstallation.

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
