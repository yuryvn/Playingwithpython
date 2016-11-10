#include "stdafx.h"
#include "Python.h"
#include "RunPythonClass.h"
#include <string>
#include <iostream>
#include <Windows.h>


PyRunT::PyRunT(PyVariableT *InputVars, int InputVarsSize, char *Pyfilename)
{
	//finding relative location of python script
	std::string PyExec = ExePath() + "\\"+ std::string(Pyfilename);
	std::cout << "PyExecutable " << PyExec << std::endl;

	char * filename = new char[PyExec.size() + 1];
	std::copy(PyExec.begin(), PyExec.end(), filename);
	filename[PyExec.size()] = '\0'; // don't forget the terminating 0
}

PyRunT::~PyRunT()
{
}

std::string ExePath() {
	char buffer[MAX_PATH];
	std::string WithRelease;
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	WithRelease = std::string(buffer).substr(0, pos);
	std::string::size_type pos1 = WithRelease.find_last_of("\\/");
	return std::string(buffer).substr(0, pos1);
}