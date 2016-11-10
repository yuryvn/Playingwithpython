//class to run python scripts
#include "stdafx.h"
#include <string>

class PyVariableT
{
public:
	PyVariableT(void *V = nullptr, char* N = "x", int T = 1, long VL = 1);
	~PyVariableT();
	char *Name;
	int Type;//1- integer,2-array of int,3-double,4-array of double,5-char
	void *Value;
	long ValueLength;
private:

};

PyVariableT::PyVariableT(void *V,char* N , int T,long VL)
{
	Name = N;
	Value = V;
	Type = T;
	ValueLength = VL;
}

PyVariableT::~PyVariableT()
{
}

class PyRunT
{
public:
	PyRunT(int InputVarsSize, char *Pyfilename, int OutputVarSize);
	~PyRunT();
	PyVariableT *PyVarsOut;
	void RunPythonScript();
	PyVariableT *PyVarsInput;
private:
	int InVarSize,OutVarSize;
	char *file;
	std::string ExePath();
	void VarConvertToPython(PyObject *main);
	void VarConvertFromPython(PyObject *main);
};

