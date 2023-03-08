#pragma once

#include "resource.h"

#ifndef PLOG_ENABLE_WCHAR_INPUT
#define PLOG_ENABLE_WCHAR_INPUT 1
#endif

#define SKIF_LOG_SEPARATOR           "----------------------------"

#include "include/plog/Log.h"
#include "include/plog/Initializers/RollingFileInitializer.h"
#include "include/plog/Appenders/ConsoleAppender.h"