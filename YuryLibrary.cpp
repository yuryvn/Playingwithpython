// PlayingWithPython.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "YuryLibrary.h"



/*
const char *pycode =
"def fact(n):\n"
"    if n <= 1:\n"
"        return 1\n"
"    else:\n"
"        return n*fact(n-1)\n"
"k = fact(i)\n";

*/

const char *plotcode =
"from pylab import*\n"
"from matplotlib.pyplot import*\n"
"plot(OX, OY, label = 'Pressure drop gradient separated flow')\n"
"plt.legend(bbox_to_anchor = (0., 1.02, 1., .102), loc = 3,\n"
"	ncol = 1, mode = 'expand', borderaxespad = 0.)\n"
"ylabel('Presseure drop gradient, kPa/m')\n"
"xlabel('Distance along the pipe, m')\n"
"show()\n";




//	const char *importcode =
//		"from numpy import linspace as linspace\n"
//		"from matplotlib.pyplot import*\n"
//		"from pylab import*\n";


/*
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
*/
int main()
{	
	/*
	std::cout << "my directory is " << ExePath() << "\n";

	PyObject *main_module, *main_dict;
	PyObject *sys_module, *sys_dict;
	PyObject *version_obj, *i_obj, *k_obj, *syspath_obj;
	PyObject *OY_obj, *OX_obj, *y_obj, *x_obj;
	double x=0, y=0;
	char *version_string,*syspath;
	long int i, k;

	Py_Initialize();

	// Setup the __main__ module for us to use 
	main_module = PyImport_ImportModule("__main__");
	main_dict = PyModule_GetDict(main_module);

	// Fetch the sys module
	sys_module = PyImport_ImportModule("sys");
	sys_dict = PyModule_GetDict(sys_module);

	// Attach the sys module into the __main__ namespace
	PyDict_SetItemString(main_dict, "sys", sys_module);

	// Retrieve the Python version from sys and print it out
	version_obj = PyMapping_GetItemString(sys_dict, "version");
	version_string = PyString_AsString(version_obj);
	printf("%s\n\n", version_string);
	Py_XDECREF(version_obj);

	//retrieve sys path from sys
	PyRun_SimpleString("path0=sys.path[0]\n");
	syspath_obj = PyMapping_GetItemString(main_dict, "path0");
//	std::cout << syspath_obj;
	syspath = PyString_AsString(syspath_obj);
	printf("%s\n\n", syspath);
	Py_XDECREF(syspath_obj);

	// Inject a variable into __main__, in this case i
	i = 5;
	i_obj = PyInt_FromLong(i);
	PyDict_SetItemString(main_dict, "i", i_obj);

	// Run the code snippet above in the current environment
	PyRun_SimpleString(pycode);

	// Extract the resultant variable, k
	k_obj = PyMapping_GetItemString(main_dict, "k");
	k = PyInt_AsLong(k_obj);
	Py_XDECREF(k_obj);
	Py_XDECREF(i_obj);

	// Show the result of the Python calculation
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
	delete[] filename;

	// don't forget to free the string after finished using it

//	PyRun_SimpleString(plotcode);


*/

/*
	double OX[1000] = {};
	double OY[1000] = {};
	double xx=0.00;
	for (long i = 0; i < 1000; i++){
		xx+=0.01;
		OX[i] = xx;
		OY[i] = sin(xx) / xx;

	}

	YuryLibrary::PyRunT *PCont = new YuryLibrary::PyRunT(2, "plotting.py", 0);
	(*PCont).PyVarsInput[0].set(OX, "OX", 4, 1000);
	(*PCont).PyVarsInput[1].set(OY, "OY", 4, 1000);

	*/
	//char OZ[40];
	//(*PCont).PyVarsOut[0].set(OZ, "string", 5, 40);

	/*checking variables
	std::cout << (*PCont).PyVarsInput[1].Name << std::endl;
	std::cout << (*PCont).PyVarsInput[1].ValueLength << std::endl;
	std::cout << (*PCont).PyVarsInput[1].Type << std::endl;
	std::cout << ((double *)((*PCont).PyVarsInput[1].Value))[500] << std::endl;
	*/
/*
	(*PCont).RunPythonScript();
	(*PCont).RunPythonScript(plotcode);


	YuryLibrary::PyRunT *PCont1 = new YuryLibrary::PyRunT(3, "plotting2.py", 0);
	(*PCont1).PyVarsInput[0].set(OX, "OX1", 4, 1000);
	(*PCont1).PyVarsInput[1].set(OY, "OY1", 4, 1000);
	(*PCont1).PyVarsInput[2].set(OY, "OZ1", 4, 1000);

	(*PCont1).RunPythonScript();
	(*PCont1).RunPythonScript(plotcode);

	*/
	//std::cout << OZ << std::endl;
	/*
	for (int k = 0; k < 10; k++)
	{
		
		std::cout <<"Name="<< (*PCont).PyVarsOut[0].Name << std::endl<< " Type=";
		std::cout << (*PCont).PyVarsOut[0].Type << " OZelement=" << OZ[k] << std::endl;
	}
	*/
//	delete PCont;
const int PointsAmount = 42;

	std::cout << "generating sample using python, note it is not changing saved gamma parameters, to set/save the parameters use setGammaParameters function" << std::endl;

	YuryLibrary::PyRunT *RP = new YuryLibrary::PyRunT(4, "MolarMassGenerating_Capi.py", 4);
	int SIZE = PointsAmount;
	double Mave = 0, Shape = 2.0, Bound = 30., Scale = 20.;
	double *MolMas, *MolFrac, *MassFrac;
	char *str = "print 'test string'\n";

	MolMas = new double[SIZE];
	MolFrac = new double[SIZE];
	MassFrac = new double[SIZE];

	const char *plotcode =
		"import numpy\n"
		"import matplotlib.pylab as plt\n"
		"from scipy.integrate import quad\n"
		"from scipy.stats import gamma as gamma\n"
		"from numpy import linspace as linspace\n"
		"from matplotlib.pyplot import*\n"
		"from scipy.optimize import curve_fit as curve_fit\n"
		"data = gamma.rvs(alpha, loc=loc, scale=beta, size=SIZE)\n";

	(*RP).PyVarsInput[0].set(&Shape, "alpha", 3);
	(*RP).PyVarsInput[1].set(&Bound, "loc", 3);
	(*RP).PyVarsInput[2].set(&Scale, "beta", 3);
	(*RP).PyVarsInput[3].set(&SIZE, "SIZE", 1);

	(*RP).PyVarsOut[0].set(&Mave, "MolarMassAve", 3);
	(*RP).PyVarsOut[1].set(MolMas, "MolarMasses", 4);
	(*RP).PyVarsOut[2].set(MassFrac, "MassFractions", 4);
	(*RP).PyVarsOut[3].set(MolFrac, "MolFractions", 4);



	(*RP).RunPythonScript();
	//(*RP).RunPythonScript(str);
	for (int i = 0; i < SIZE; std::cout << MolFrac[i++] << " ");

	(*RP).setFile("MolarMassGenerating_Capi_clearVars.py");//file to clear vars
	std::cout << "ready to clear vars";
	(*RP).RunPythonScript(0);//running file to clear vars from dict

	delete RP;


	//Py_Finalize();
	return 0;
}

