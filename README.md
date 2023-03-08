# SKIFdrv

Management tool for facilitating driver service (un)installation.

Special K can make use of the quite common `WinRing0x64.sys` kernel driver to enable advanced CPU hardware monitoring capabilities in its CPU widget. This tool allows end-users to easily install and uninstall the driver and the necessary components, and is redistributed as a separate installer from the main Special K/SKIF package. The tool creates a driver service on the system called `SK_WinRing0` that autostarts the driver alongside the system boot as this is required to allow unelevated games to make use of it.

The tool will eventually also support uninstalling the legacy driver service `WinRing0_1_2_0` to allow users to migrate over.

## Security concerns

The kernel driver in question exposes the system to [CVE-2020-14979](https://nvd.nist.gov/vuln/detail/CVE-2020-14979) ([details](https://posts.specterops.io/cve-2020-14979-local-privilege-escalation-in-evga-precisionx1-cf63c6b95896)), a Local Privilege Escalation vulnerability that can be exploited by malicious third-party code running locally to elevate their privileges to that of a local administrator.

This is a consequence of the intended purpose of allowing advanced CPU hardware monitoring capabilities in regular games that typically run unelevated (not as an admin), and our reliance of the common `WinRing0_1_2_0` kernel driver. While Special K's custom user-mode driver is cut-down significantly and only implements the relevant CPU sensor code, the kernel driver itself includes more advanced hardware capabilities beyond this, and third-party apps/tools that make use of their own user-mode driver can also make use of those capabilities as a result.

This is sadly nothing we can solve as it is outside of our financial capabilities to create a cut down custom kernel driver implementing only the CPU sensor capabilities.

**If you are a corporate/business user or IT administrator on a corporate device, do not under any circumstances install this driver!**

For regular end-users, whether this is a concern to you or not is ultimately up to you. To exploit the vulnerability malicious code needs to be executed locally and go undetected by the anti-virus suite of the system, and at that point most malware can already easily performed similar local privilege escalations through alternate methods, such as through e.g. a simple [UAC auto-elevation bypass](https://devblogs.microsoft.com/oldnewthing/20160816-00/?p=94105).

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

## Third-party code

* Uses [PLOG](https://github.com/SergiusTheBest/plog), licensed under [MIT](https://github.com/SergiusTheBest/plog/blob/master/LICENSE).