//class to run python scripts
#pragma once
#ifdef BUILDASDLL
#define YURYLIBRARY_API_ __declspec(dllexport)
#else
#define YURYLIBRARY_API_//__declspec(dllimport) would be if want static lib, nothing for building exe file
#endif

#include "stdafx.h"
#include <string>
#include <Python.h>
#include <iostream>
#include <string>
#include <Windows.h>

namespace YuryLibrary{


	class YURYLIBRARY_API_ PyVariableT
	{
	public:
		PyVariableT(void *V = nullptr, char* N = "x", int T = 1, long VL = 1);
		~PyVariableT();
		char *Name;
		int Type;//1- integer,2-array of int,3-double,4-array of double,5-char
		void *Value;
		long ValueLength;
		void set(void *V = nullptr, char* N = "x", int T = 1, long VL = 1);
	private:

	};

	class YURYLIBRARY_API_ PyRunT
	{
	public:
		PyRunT(int InputVarsSize, char *Pyfilename, int OutputVarSize);
		~PyRunT();
		PyVariableT *PyVarsOut;
		void RunPythonScript();
		void RunPythonScript(const char *importcode);
		void RunPythonScript(const int clearflag);
		PyVariableT *PyVarsInput;
		void setFile(char *PyfileName);
	private:
		int InVarSize, OutVarSize;
		char *file;
		std::string ExePath();
		void VarConvertToPython(PyObject *main);
		void VarConvertFromPython(PyObject *main);
	};
}
