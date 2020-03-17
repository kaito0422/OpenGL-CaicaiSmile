#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <glew.h>
#include <glfw3.h>

class Program {
public:
	Program(const std::string &vs, const std::string &fs);
	~Program();

	void useProgram()
	{
		glUseProgram(programId);
	}

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	unsigned int getProgramId() const { return programId; }

private:
	bool isCreateSuccess = false;
	unsigned int programId;
};

#endif