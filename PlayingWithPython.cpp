// PlayingWithPython.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Python.h>
#include <iostream>

const char *pycode =
"def fact(n):\n"
"    if n <= 1:\n"
"        return 1\n"
"    else:\n"
"        return n*fact(n-1)\n"
"k = fact(i)";

int main()
{
	PyObject *main_module, *main_dict;
	PyObject *sys_module, *sys_dict;
	PyObject *version_obj, *i_obj, *k_obj, *syspath_obj;
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
	//PyRun_SimpleString("path0=sys.path[0]\n");
	syspath_obj = PyMapping_GetItemString(sys_dict, "path");
	std::cout << syspath_obj;
//	syspath = PyString_AsString(syspath_obj);
//	printf("%s\n\n", syspath);
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

	Py_Finalize();

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

