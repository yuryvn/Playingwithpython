#include "stdafx.h"
#include "Python.h"
#include "RunPythonClass.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <stack>

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
	delete []PyVarsOut;
	delete[]PyVarsInput;
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

	//Passing all variables to python
	VarConvertToPython(main_dict);

	//Running script with wrapper to use PyRun_SimpleFile
	PyObject* PyFileObject = PyFile_FromString(file, "r");
	PyRun_SimpleFile(PyFile_AsFile(PyFileObject), file);

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
			(*(PyVarsOut + i)).Value = new long(PyInt_AsLong(x_obj)); //allocate memory for new variable and set .value to point to this memory block
//			*((long *)((*(PyVarsOut + i)).Value))=(long)PyInt_AsLong(x_obj); //we take value, say that it is pointer to long, and then dereference it
			Py_CLEAR(x_obj);
			break;
		}
		case 2:{
			OX_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			x_obj = PyList_GetItem(OX_obj, 0); //just to initialize
			//fill OX_obj
			Length = (*(PyVarsOut + i)).ValueLength;
			(*(PyVarsOut + i)).Value = new long[Length];
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
			(*(PyVarsOut + i)).Value = new double(PyFloat_AsDouble(x_obj)); //allocate memory for new variable and set .value to point to this memory block
//			*((double *)((*(PyVarsOut + i)).Value)) = (double)PyFloat_AsDouble(x_obj); //we take value, say that it is pointer to long, and then dereference it
			Py_CLEAR(x_obj);
			break;
		}

		case 4:{//if list of doubles

			OX_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			x_obj = PyList_GetItem(OX_obj, 0); //just to initialize

			//fill OX_obj
			Length = (*(PyVarsOut + i)).ValueLength;
			(*(PyVarsOut + i)).Value = new double[Length];
			for (long ii = 0; ii < Length; ii++){
				//Exctract x_obj, convert to int, add to C++ array;
				x_obj = PyList_GetItem(OX_obj, ii);
				((double *)(*(PyVarsOut + i)).Value)[ii] = (double)PyFloat_AsDouble(x_obj);
			}
			//add list OX_obj as OX to python, now python code has OX defined, same with OY
			Py_CLEAR(OX_obj);
			Py_CLEAR(x_obj);

			break;
		}
		case 5:{//if char*

			x_obj = PyDict_GetItemString(main, (*(PyVarsOut + i)).Name);
			(*(PyVarsOut + i)).Value = new char*(PyString_AsString(x_obj)); //allocate memory for new variable and set .value to point to this memory block
	
			Py_CLEAR(x_obj);
			break;
		}

		default:
			std::cout << "Unknown type when trying to convert to python";
			break;
		}
	}
}