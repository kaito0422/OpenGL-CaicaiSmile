#ifndef _MODEL_H_
#define _MODEL_H_

#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class ShapeType {
	CUBE = 0,
	FLAT
};

class Cube {
public:

	Cube();

	void bindVao(unsigned int programId) 
	{
		glBindVertexArray(vaoId);
		glUniform1i(glGetUniformLocation(programId, "type"), (int)ShapeType::CUBE);
	}

	unsigned int getVboId() { return vboId; }
	unsigned int getVaoId() { return vaoId; }

	void drawCube(unsigned int programId, const glm::mat4 &view, const glm::vec3 &position);

private:
	float vertices[180] = {
		/* 前 */
			 0.5,  0.5,  0.5,	0.0, 1.0,
			 0.5, -0.5,  0.5,	0.0, 0.0,
			-0.5, -0.5,  0.5,	1.0, 0.0,
			 0.5,  0.5,  0.5,	0.0, 1.0,
			-0.5, -0.5,  0.5,	1.0, 0.0,
			-0.5,  0.5,  0.5,	1.0, 1.0,

		/* 后 */
			-0.5,  0.5, -0.5,	0.0, 1.0,
			-0.5, -0.5, -0.5,	0.0, 0.0,
			 0.5, -0.5, -0.5,	1.0, 0.0,
			-0.5,  0.5, -0.5,	0.0, 1.0,
			 0.5, -0.5, -0.5,	1.0, 0.0,
			 0.5,  0.5, -0.5,	1.0, 1.0,

		/* 左 */
			-0.5,  0.5,  0.5,	0.0, 1.0,
			-0.5, -0.5,  0.5,	0.0, 0.0,
			-0.5, -0.5, -0.5,	1.0, 0.0,
			-0.5,  0.5,  0.5,	0.0, 1.0,
			-0.5, -0.5, -0.5,	1.0, 0.0,
			-0.5,  0.5, -0.5,	1.0, 1.0,

		/* 右*/
			0.5,  0.5, -0.5,	0.0, 1.0,
			0.5, -0.5, -0.5,	0.0, 0.0,
			0.5, -0.5,  0.5,	1.0, 0.0,
			0.5,  0.5, -0.5,	0.0, 1.0,
			0.5, -0.5,  0.5,	1.0, 0.0,
			0.5,  0.5,  0.5,	1.0, 1.0,

		/* 上 */
			 0.5,  0.5, -0.5,	0.0, 1.0,
			 0.5,  0.5,  0.5,	0.0, 0.0,
			-0.5,  0.5,  0.5,	1.0, 0.0,
			 0.5,  0.5, -0.5,	0.0, 1.0,
			-0.5,  0.5,  0.5,	1.0, 0.0,
			-0.5,  0.5, -0.5,	1.0, 1.0,

		/* 下 */
			 0.5, -0.5,  0.5,	0.0, 1.0,
			 0.5, -0.5, -0.5,	0.0, 0.0,
			-0.5, -0.5, -0.5,	1.0, 0.0,
			 0.5, -0.5,  0.5,	0.0, 1.0,
			-0.5, -0.5, -0.5,	1.0, 0.0,
			-0.5, -0.5,  0.5,	1.0, 1.0
	};

	unsigned int vboId, vaoId;
};

class Flat {
public:
	Flat();

	void bindVao(unsigned int programId)
	{
		glBindVertexArray(vaoId);
		glUniform1i(glGetUniformLocation(programId, "type"), (int)ShapeType::FLAT);
	}

	unsigned int getVboId() { return vboId; }
	unsigned int getVaoId() { return vaoId; }

	void drawCube(unsigned int programId, const glm::mat4 &view, const glm::vec3 &position);

private:
	float ground[30] = {
		 1.0, 0.0, -1.0,	1.0, 1.0,
		 1.0, 0.0,  1.0,	1.0, 0.0,
		-1.0, 0.0,  1.0,	0.0, 0.0,
		 1.0, 0.0, -1.0,	1.0, 1.0,
		-1.0, 0.0,  1.0,	0.0, 0.0,
		-1.0, 0.0, -1.0,	0.0, 1.0
	};

	unsigned int vboId, vaoId;
};

#endif