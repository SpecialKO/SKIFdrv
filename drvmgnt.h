#pragma once

#include "SKIFdrv.h"

bool SvcInstall   (std::wstring svcName, std::wstring fullPath);
bool SvcUninstall (std::wstring svcName);
bool SvcStart     (std::wstring svcName);
bool SvcStop      (std::wstring svcName);
bool DevOpen      (std::wstring devName);