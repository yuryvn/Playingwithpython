#include "stdafx.h"
#include "Python.h"
#include "RunPythonClass.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <stack>


PyRunT::PyRunT(int InputVarsSize, char *Pyfilename, int OutputVarSize) :PyVarsInput(new PyVariableT[InputVarsSize]), PyVarsOut(new PyVariableT[OutputVarSize])
{
	Py_Initialize();
	InVarSize = InputVarsSize;
	OutVarSize = OutputVarSize;

	//finding relative location of python script
	std::string PyExec = ExePath() + "\\"+ std::string(Pyfilename);
	std::cout << "PyExecutable " << PyExec << std::endl;

	char * filename = new char[PyExec.size() + 1];
	std::copy(PyExec.begin(), PyExec.end(), filename);
	filename[PyExec.size()] = '\0'; // don't forget the terminating 0
	
	file = filename;

	std::cout << file << std::endl;
	delete[] filename;

}

PyRunT::~PyRunT()
{
	Py_Finalize();
}

std::string PyRunT::ExePath() {
	char buffer[MAX_PATH];
	std::string WithRelease;
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	WithRelease = std::string(buffer).substr(0, pos);
	std::string::size_type pos1 = WithRelease.find_last_of("\\/");
	return std::string(buffer).substr(0, pos1);
}


void PyRunT::RunPythonScript(){

	PyObject *main_module, *main_dict;
	PyObject *sys_module, *sys_dict;

	/* Setup the __main__ module for us to use */
	main_module = PyImport_ImportModule("__main__");
	main_dict = PyModule_GetDict(main_module);

	/* Fetch the sys module */
	sys_module = PyImport_ImportModule("sys");
	sys_dict = PyModule_GetDict(sys_module);
	/* Attach the sys module into the __main__ namespace */
	PyDict_SetItemString(main_dict, "sys", sys_module);

	VarConvertToPython(main_dict);


}

void PyRunT::VarConvertToPython(PyObject *main){
//	InVarSize
//	InputVars

//function will update main dictionary 

	long *intPTR;
	long Length;
	double *doublePTR;
	char *charPTR;
	int check;
	PyObject *OX_obj;
	PyObject *x_obj;

	for (long i = 0; i < InVarSize; i++){

		switch ((*(PyVarsInput + i)).Type)//1- integer,2-array of int,3-double,4-array of double,5-char
		{
		case 1:{
			intPTR = static_cast<long*>((*(PyVarsInput + i)).Value);  //say that Value is actually pointing to integer
			PyDict_SetItemString(main, std::string((*(PyVarsInput + i)).Name).c_str(), PyInt_FromLong(*intPTR));
			break;
		}
		case 2:{
			OX_obj = PyList_New(0);

			intPTR = static_cast<long*>((*(PyVarsInput + i)).Value);

			//fill OX_obj
			Length = (*(PyVarsInput + i)).ValueLength;
			for (long ii = 0; ii < Length; ii++){
				//before adding to OX_obj, neet to convert x variable to python x_obj
				x_obj = PyInt_FromLong(intPTR[ii]);
				check = PyList_Append(OX_obj, x_obj);
			}
			//add list OX_obj as OX to python, now python code has OX defined, same with OY
			PyDict_SetItemString(main, std::string((*(PyVarsInput + i)).Name).c_str(), OX_obj);
			Py_CLEAR(OX_obj);
			Py_CLEAR(x_obj);


			break;
		}
		case 3:{//if double
			doublePTR = static_cast<double*>((*(PyVarsInput + i)).Value);  //say that Value is actually pointing to double
			PyDict_SetItemString(main, std::string((*(PyVarsInput + i)).Name).c_str(), PyFloat_FromDouble(*doublePTR));
			break;
			break;
		}

		case 4:{//if list of doubles
			OX_obj = PyList_New(0);

			doublePTR = static_cast<double*>((*(PyVarsInput + i)).Value);
			Length = (*(PyVarsInput + i)).ValueLength;
			//fill OX_obj
			for (long ii = 0; ii < Length; ii++){
				//before adding to OX_obj, neet to convert x variable to python x_obj
				x_obj = PyFloat_FromDouble(doublePTR[ii]);
				check = PyList_Append(OX_obj, x_obj);
			}
			//add list OX_obj as OX to python, now python code has OX defined, same with OY
			PyDict_SetItemString(main, std::string((*(PyVarsInput + i)).Name).c_str(), OX_obj);
			Py_CLEAR(OX_obj);
			Py_CLEAR(x_obj);

			break;
		}
		case 5:{//if char*
			charPTR = static_cast<char*>((*(PyVarsInput + i)).Value);  //say that Value is actually pointing to char
			PyDict_SetItemString(main, std::string((*(PyVarsInput + i)).Name).c_str(), PyInt_FromLong(*intPTR));
			break;
			break;
		}

		default:
			std::cout << "Unknown type when trying to convert to python";
			break;
		}
	}
}