// PlayingWithPython.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Python.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "RunPythonClass.h"


const char *pycode =
"def fact(n):\n"
"    if n <= 1:\n"
"        return 1\n"
"    else:\n"
"        return n*fact(n-1)\n"
"k = fact(i)\n";

const char *plotcode =
"from matplotlib.pyplot import*\n"
"plot(OX, OY, label = 'Pressure drop gradient separated flow')\n"
"#plt.legend(bbox_to_anchor = (0., 1.02, 1., .102), loc = 3,\n"
"#	ncol = 1, mode = 'expand', borderaxespad = 0.)\n"
"ylabel('Presseure drop gradient, kPa/m')\n"
"xlabel('Distance along the pipe, m')\n"
"show()\n";

//get current dir

std::string ExePath() {
	char buffer[MAX_PATH];
	std::string WithRelease;
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	WithRelease = std::string(buffer).substr(0, pos);
	std::string::size_type pos1 = WithRelease.find_last_of("\\/");
	return std::string(buffer).substr(0, pos1);
}

int main()
{	
	std::cout << "my directory is " << ExePath() << "\n";

	PyObject *main_module, *main_dict;
	PyObject *sys_module, *sys_dict;
	PyObject *version_obj, *i_obj, *k_obj, *syspath_obj;
	PyObject *OY_obj, *OX_obj, *y_obj, *x_obj;
	double x=0, y=0;
	char *version_string,*syspath;
	long int i, k;

	Py_Initialize();

	/* Setup the __main__ module for us to use */
	main_module = PyImport_ImportModule("__main__");
	main_dict = PyModule_GetDict(main_module);

	/* Fetch the sys module */
	sys_module = PyImport_ImportModule("sys");
	sys_dict = PyModule_GetDict(sys_module);

	/* Attach the sys module into the __main__ namespace */
	PyDict_SetItemString(main_dict, "sys", sys_module);

	/* Retrieve the Python version from sys and print it out */
	version_obj = PyMapping_GetItemString(sys_dict, "version");
	version_string = PyString_AsString(version_obj);
	printf("%s\n\n", version_string);
	Py_XDECREF(version_obj);

	/*retrieve sys path from sys*/
	PyRun_SimpleString("path0=sys.path[0]\n");
	syspath_obj = PyMapping_GetItemString(main_dict, "path0");
//	std::cout << syspath_obj;
	syspath = PyString_AsString(syspath_obj);
	printf("%s\n\n", syspath);
	Py_XDECREF(syspath_obj);

	/* Inject a variable into __main__, in this case i */
	i = 5;
	i_obj = PyInt_FromLong(i);
	PyDict_SetItemString(main_dict, "i", i_obj);

	/* Run the code snippet above in the current environment */
	PyRun_SimpleString(pycode);

	/* Extract the resultant variable, k */
	k_obj = PyMapping_GetItemString(main_dict, "k");
	k = PyInt_AsLong(k_obj);
	Py_XDECREF(k_obj);
	Py_XDECREF(i_obj);

	/* Show the result of the Python calculation */
	printf("Python calculated that %d! = %d\n", i, k);

	//plotting OY vs OX
	int check = 0;
	//creat OX_obj and OY_obj as python lists
	OX_obj = PyList_New(0);
	OY_obj = PyList_New(0);

	//fill OX_obj and OY_obj
	for (int ii = 0; ii < 1000; ii++){
		x += 0.01;
		y = sin(x);
		//before adding to OX_obj, neet to convert x variable to python x_obj
		x_obj = PyFloat_FromDouble(x);
		y_obj = PyFloat_FromDouble(y);
		check = PyList_Append(OX_obj, x_obj);
		check = PyList_Append(OY_obj, y_obj);
	}
	//add list OX_obj as OX to python, now python code has OX defined, same with OY
	PyDict_SetItemString(main_dict, "OX", OX_obj);
	PyDict_SetItemString(main_dict, "OY", OY_obj);

	Py_CLEAR(OY_obj); Py_CLEAR(OX_obj);


	//finding relative location of python script
	std::string PyExec = ExePath() + "\\plotting.py";
	std::cout << "PyExecutable "<<PyExec << std::endl;
	
	char * filename = new char[PyExec.size() + 1];
	std::copy(PyExec.begin(), PyExec.end(), filename);
	filename[PyExec.size()] = '\0'; // don't forget the terminating 0



//	char *filename = "c:\\work\\C\\learning\\Playingwithpython\\plotting.py";

	//wrapper to use PyRun_SimpleFile
	PyObject* PyFileObject = PyFile_FromString(filename, "r");
	PyRun_SimpleFile(PyFile_AsFile(PyFileObject), filename);

	// don't forget to free the string after finished using it
	delete[] filename;
//	PyRun_SimpleString(plotcode);

	Py_Finalize();

	double OX[1000] = {};
	double OY[1000] = {};
	double xx=0.01;
	for (long i = 0; i < 1000; i++){
		xx+=0.01;
		OX[i] = xx;
		OY[i] = sin(xx) / xx;

	}

	PyRunT *PCont = new PyRunT(2, "plotting.py", 0);
	(*PCont).PyVarsInput[0].set(OX, "OX", 4, 1000);
	(*PCont).PyVarsInput[1].set(OY, "OY", 4, 1000);
	(*PCont).RunPythonScript();
	delete PCont;

	std::cout << "lalalla" << std::endl;

	return 0;
}

