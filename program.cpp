#include "program.h"

Program::Program(const std::string &vsFilePath, const std::string &fsFilePath)
{
	/* 读取shader的源文件 */
	std::ifstream vsFile(vsFilePath), fsFile(fsFilePath);
	std::string line;
	std::string vsSrc, fsSrc;
	while (std::getline(vsFile, line)) {
		vsSrc += line;
		vsSrc += "\n";
	}
	while (std::getline(fsFile, line)) {
		fsSrc += line;
		fsSrc += "\n";
	}

	const char *vsPtr, *fsPtr;
	vsPtr = vsSrc.c_str();
	fsPtr = fsSrc.c_str();

	int success;
	char infoLog[512];

	/* 创建并编译shader */
	unsigned int vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vsPtr, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "[ERROR] " << __FILE__ << " | " <<  __LINE__ << ": " << infoLog << std::endl;
		return;
	}
	glShaderSource(fs, 1, &fsPtr, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "[ERROR] " << __FILE__ << " | " << __LINE__ << ": " << infoLog << std::endl;
		return;
	}

	/* 创建program */
	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	/* link program */
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "[ERROR] " << __FILE__ << " | " << __LINE__ << ": " << infoLog << std::endl;
		return;
	}

	programId = program;

	glDeleteShader(vs);
	glDeleteShader(fs);

	isCreateSuccess = true;
}

Program::~Program()
{
	if (isCreateSuccess) {
		glDeleteProgram(programId);
	}
}

void Program::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}

void Program::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Program::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}