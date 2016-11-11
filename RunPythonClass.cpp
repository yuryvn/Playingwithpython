#include "stdafx.h"
#include "Python.h"
#include "RunPythonClass.h"
#include <string>
#include <iostream>
#include <Windows.h>

PyVariableT::PyVariableT(void *V, char* N, int T, long VL)
{
	Name = N;
	Value = V;
	Type = T;
	ValueLength = VL;
}

PyVariableT::~PyVariableT()
{
}

void PyVariableT::set(void *V, char* N, int T, long VL){
	Name = N;
	Value = V;
	Type = T;
	ValueLength = VL;
}


PyRunT::PyRunT(int InputVarsSize, char *Pyfilename, int OutputVarSize) :PyVarsInput(new PyVariableT[InputVarsSize]), PyVarsOut(new PyVariableT[OutputVarSize])
{	
	Py_Initialize();
	InVarSize = InputVarsSize;
	OutVarSize = OutputVarSize;
	file = Pyfilename;
	
}

PyRunT::~PyRunT()
{
	delete []PyVarsOut;
	delete[]PyVarsInput;
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

	//Passing all variables to python
	VarConvertToPython(main_dict);


	

	std::cout << "checking input\n";
	std::cout << "Name=" << (*(PyVarsInput + 1)).Name << std::endl;
	PyObject* CHECK_obj = PyDict_GetItemString(main_dict, (*(PyVarsInput + 1)).Name);
	PyObject* check_obj = PyList_GetItem(CHECK_obj, 500); //just to initialize
	std::cout << "Value of 500th element=" << (double)PyFloat_AsDouble(check_obj) << std::endl;



	//finding relative location of python script
	std::string PyExec = ExePath() + "\\" + std::string(file);
	std::cout << "PyExecutable " << PyExec << std::endl;
	char * filename = new char[PyExec.size() + 1];
	std::copy(PyExec.begin(), PyExec.end(), filename);
	filename[PyExec.size()] = '\0'; // don't forget the terminating 0

	//Running script with wrapper to use PyRun_SimpleFile

	PyObject* PFO = PyFile_FromString(filename, "r");
	std::cout << "running script inside class\n";


	const char *importcode =
		"from numpy import linspace as linspace\n"
		"from matplotlib.pyplot import*\n"
		"from pylab import*\n";

	//PyRun_SimpleString(importcode);

	PyRun_SimpleFile(PyFile_AsFile(PFO), filename);

	//Retrieving output from python
	VarConvertFromPython(main_dict);


}

void PyRunT::VarConvertToPython(PyObject *main){

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
			x_obj = PyInt_FromLong(intPTR[0]); //to initialize

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
			x_obj = PyFloat_FromDouble(doublePTR[0]); //to initialize

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
			PyDict_SetItemString(main, std::string((*(PyVarsInput + i)).Name).c_str(), PyString_FromString(charPTR));
			break;

		}

		default:
			std::cout << "Unknown type when trying to convert to python";
			break;
		}
	}
}

void PyRunT::VarConvertFromPython(PyObject *main){


	//function will update main dictionary 

	long Length;
	PyObject *OX_obj;
	PyObject *x_obj;

	for (long i = 0; i < OutVarSize; i++){

		switch ((*(PyVarsOut + i)).Type)//1- integer,2-array of int,3-double,4-array of double,5-char
		{
		case 1:{
			x_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			*((long *)((*(PyVarsOut + i)).Value))=(long)PyInt_AsLong(x_obj); //we take value, say that it is pointer to long, and then dereference it
			Py_CLEAR(x_obj);
			break;
		}
		case 2:{
			OX_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			x_obj = PyList_GetItem(OX_obj, 0); //just to initialize
			//fill OX_obj
			Length = (*(PyVarsOut + i)).ValueLength;
			for (long ii = 0; ii < Length; ii++){
				//Exctract x_obj, convert to int, add to C++ array;
				x_obj = PyList_GetItem(OX_obj,ii);
				((long *)(*(PyVarsOut + i)).Value)[ii] = (long)PyInt_AsLong(x_obj);
			}
			//add list OX_obj as OX to python, now python code has OX defined, same with OY
			Py_CLEAR(OX_obj);
			Py_CLEAR(x_obj);


			break;
		}
		case 3:{//if double
			x_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			*((double *)((*(PyVarsOut + i)).Value)) = double(PyFloat_AsDouble(x_obj)); //make void pointer to be pointer to double, dereference,and assign number
			Py_CLEAR(x_obj);
			break;
		}

		case 4:{//if list of doubles

			OX_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			x_obj = PyList_GetItem(OX_obj, 0); //just to initialize

			//fill OX_obj
			Length = (*(PyVarsOut + i)).ValueLength;

			for (long ii = 0; ii < Length; ii++){
				//Exctract x_obj, convert to int, add to C++ array;
				x_obj = PyList_GetItem(OX_obj, ii);
				((double *)((*(PyVarsOut + i)).Value))[ii] = (double)PyFloat_AsDouble(x_obj);
			}
			Py_CLEAR(OX_obj);
			Py_CLEAR(x_obj);

			break;
		}
		case 5:{//if char*

			Length = (*(PyVarsOut + i)).ValueLength;
			x_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			
			strcpy_s((char *)((*(PyVarsOut + i)).Value), Length, PyString_AsString(x_obj));
			
			Py_CLEAR(x_obj);
			
			break;
		}

		default:
			std::cout << "Unknown type when trying to convert to python";
			break;
		}
	}
}