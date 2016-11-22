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
#include <thread>

namespace YuryLibrary{


	class YURYLIBRARY_API_ PyVariableT
	{
	public:
		//Creating a Variable that will be passed to python, 
		//V- pointer to values, N-name, T-Type(1-int,2-int[VL],3-double,4-double[VL],5-char[VL])
		//VL- length of the variable to be passed, if list or char is passed
		PyVariableT(void *V = nullptr, char* N = "x", int T = 1, long VL = 1);
		~PyVariableT();
		char *Name;
		int Type;//1- integer,2-array of int,3-double,4-array of double,5-char
		void *Value;
		long ValueLength;
		//set a variable
		void set(void *V = nullptr, char* N = "x", int T = 1, long VL = 1);
	private:

	};

	class YURYLIBRARY_API_ PyRunT
	{
	public:
		//parameters are:
		//How many variables to be passed to python
		//python script file "something.py"
		//how many variables will be read from python, when script finishes
		//typical sequence: create PyRunT object, fill variables to pass and to retrive(ex.: this->PyVarsIn[0].set(&foo,"X_double",3); this->PyVarsOut[0].set(mylistofd,"doublelist",4,7))
		//conver vars to python with VarConvertToPython(); then run pythin script; convert vars from python
		PyRunT(int InputVarsSize, char *Pyfilename, int OutputVarSize);
		~PyRunT();
		PyVariableT *PyVarsOut;
		//Run script with saved file name
		void RunPythonScript();
		//Run script in thread with saved file name
		std::thread RunPythonScriptThread();
		//Run char* type string in python
		void RunPythonScript(const char *importcode);
		//Run python script with saved file name, but without any initializations, usually used for deleted variables
		void RunPythonScript(const int clearflag);
		PyVariableT *PyVarsInput;
		//change saved python script filename
		void setFile(char *PyfileName);
		//convert variables to python space
		void VarConvertToPython();
		//retrieve output variables from python space
		void VarConvertFromPython();
		std::thread VarConvertFromPythonThread();

	private:
		int InVarSize, OutVarSize;
		char *file;
		std::string ExePath();
		PyObject *main_module, *main_dict;
		PyObject *sys_module, *sys_dict;
	};
}
