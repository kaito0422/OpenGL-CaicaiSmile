#include "model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube()
{
	unsigned int vbo, vao;

	glCreateBuffers(1, &vbo);
	glCreateVertexArrays(1, &vao);
	vboId = vbo;
	vaoId = vao;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Cube::drawCube(unsigned int programId, const glm::mat4 &view, const glm::vec3 &position)
{
	glm::mat4 view_tmp;
	view_tmp = glm::translate(view, position);
	glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view_tmp));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}



Flat::Flat()
{
	unsigned int vbo, vao;

	glCreateBuffers(1, &vbo);
	glCreateVertexArrays(1, &vao);
	vboId = vbo;
	vaoId = vao;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Flat::drawCube(unsigned int programId, const glm::mat4 &view, const glm::vec3 &position)
{
	glm::mat4 view_tmp;
	view_tmp = glm::translate(view, position);
	glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view_tmp));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}