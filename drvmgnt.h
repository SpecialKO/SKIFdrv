#pragma once

#include "SKIFdrv.h"

bool SvcInstall   (std::wstring fullpath);
bool SvcUninstall (void);
bool SvcStart     (void);
bool SvcStop      (void);
bool DevOpen      (void);