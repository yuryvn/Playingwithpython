//class to run python scripts

class PyVariableT
{
public:
	PyVariableT(void *Value, char* N = "x", int Type = 1);
	~PyVariableT();
	char *Name;
	int Type;//1- integer,2-array of int,3-double,4-array of double,5-char
	void *Value;
private:

};

PyVariableT::PyVariableT(void *Value,char* N = "x", int Type = 1)
{
}

PyVariableT::~PyVariableT()
{
}

class PyRunT
{
public:
	PyRunT(PyVariableT *InputVars, int InputVarsSize, char *filename);
	~PyRunT();
private:
	PyVariableT* PyVarsList;
	int VarSize;
	char *file;
};

PyRunT::PyRunT(PyVariableT *InputVars,int InputVarsSize,char *filename)
{
}

PyRunT::~PyRunT()
{
}