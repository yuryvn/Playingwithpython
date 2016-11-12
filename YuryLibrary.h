#pragma once
#ifdef YURYLIBRARY_EXPORTS
#define YURYLIBRARY_API_ __declspec(dllexport)
#else
#define YURYLIBRARY_API_ __declspec(dllimport)
#endif

#include "RunPythonClass.h"