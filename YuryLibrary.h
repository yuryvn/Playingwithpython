#pragma once
#ifdef BUILDASDLL
#define YURYLIBRARY_API_ __declspec(dllexport)
#else
#define YURYLIBRARY_API_// __declspec(dllimport)
#endif

#include "RunPythonClass.h"