#include "python_driver.h"
#include <Python.h>
#include <fstream>
#include <vector>

Python::Python(char *programName) : m_programName(programName)
{
	Py_SetProgramName(m_programName);
	Py_Initialize();
}

Python::~Python()
{
	Py_Finalize();
}

void	Python::runFile(const std::string &filename)
{
	PyRun_SimpleString(readFile(filename).c_str());
}

void	Python::runScript(const std::string &script)
{
	PyRun_SimpleString(script.c_str());
}

std::string	Python::readFile(const std::string &filename)
{
	std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	std::ifstream::pos_type	fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<char>	bytes(fileSize);
	ifs.read(&bytes[0], fileSize);
	return std::string(&bytes[0], fileSize);
}
