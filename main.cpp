// GLEW最好使用静态库，使用动态库会需要取连接.dll文件，但是有的版本不提供.dll文件
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// GLEW库的头文件必须在GLFW上面
#include <glew.h>
#include <glfw3.h>
#include <glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "program.h"
#include "texture.h"
#include "model.h"

const int windowWidth = 1920; //640 * 2;
const int windowHeight = 1080; //480 * 2;
const char *windowTitle = "GLFW window for kaito!";

#define FUNC_SUCCESS(x)		if(x) { std::cout << "[ERROR]: line " << __LINE__ << std::endl; }

float visible = 0.2;
float xPos = 0.0, yPos = 0.0, zPos = 2.0;
double drtRadius = 0.0;
double upRadius = 0.0;
glm::vec3 direct(0.0, 0.0, -1.0);
glm::vec3 up(0.0, 1.0, 0.0);

bool isJump = false;
int jumpIdx = 0;
const int jumpMaxIdx = 28;
float jumpOffset[jumpMaxIdx] = {
	 0.1,  0.08,  0.06,  0.04,  0.03,  0.03,  0.02,  0.015,  0.01,  0.0075,  0.005,  0.003,  0.002,  0.0015,
	-0.0015, -0.002, -0.003, -0.005, -0.0075, -0.01, -0.015, -0.02, -0.03, -0.03, -0.04, -0.06, -0.08, -0.1,
};

/*
 * 通过glfwGetKey获取在GLFW window中按下的按键，并处理
 * window：需要获取按键输入的window
 */
void processKeyboardInput(GLFWwindow *window, unsigned int program)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		visible += 0.02;
		visible = visible >= 1 ? 1 : visible;
		glUniform1f(glGetUniformLocation(program, "visible"), visible);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visible -= 0.02;
		visible = visible <= 0 ? 0 : visible;
		glUniform1f(glGetUniformLocation(program, "visible"), visible);
	}

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) ||
		(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)) {
		static double x, y;
		double x_tmp, y_tmp;
		glfwGetCursorPos(window, &x_tmp, &y_tmp);
		if (x != x_tmp && y != y_tmp) {
			std::cout << "x = " << x << "; y = " << y << std::endl;
			x = x_tmp;
			y = y_tmp;
		}
	}

	float sensitivity = 1.0 / 80;

	/* 通过WASD控制前后左右 */
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		xPos += direct.x * sensitivity;
	//	yPos += direct.y * sensitivity;
		zPos += direct.z * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		xPos -= direct.x * sensitivity;
	//	yPos -= direct.y * sensitivity;
		zPos -= direct.z * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 drt = glm::normalize(glm::cross(direct, up));
		xPos -= drt.x * sensitivity;
	//	yPos -= drt.y * sensitivity;
		zPos -= drt.z * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 drt = glm::normalize(glm::cross(direct, up));
		xPos += drt.x * sensitivity;
	//	yPos += drt.y * sensitivity;
		zPos += drt.z * sensitivity;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!isJump) {
			isJump = true;
			jumpIdx = 0;
		}
	}
	//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
	//	if (!isJump) {
	//		isJump = true;
	//		jumpIdx = 0;
	//	}
	//}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		xPos = 0.0;
		yPos = 0.0;
		zPos = 3.0;
		direct.x = 0.0;
		direct.y = 0.0;
		direct.z = -1.0;
		up.x = 0.0;
		up.y = 1.0;
		up.z = 0.0;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	drtRadius = (xpos - windowWidth / 2) * 1.0 / 1000;
	drtRadius = drtRadius >= glm::radians(360.0) ? drtRadius - glm::radians(360.0) : drtRadius;
	upRadius = (ypos - windowHeight / 2) * 1.0 / 1000;
	upRadius = upRadius >= glm::radians(360.0) ? upRadius - glm::radians(360.0) : upRadius;

	{
		glm::mat4 transMat;
		transMat = glm::rotate(transMat, (float)drtRadius, glm::vec3(0.0, -1.0, 0.0));
		glm::vec4 tmp1(direct.x, direct.y, direct.z, 1.0);
		tmp1 = transMat * tmp1;
		direct.x = tmp1.x;
		direct.y = tmp1.y;
		direct.z = tmp1.z;
		glm::vec4 tmp2(up.x, up.y, up.z, 1.0);
		tmp2 = transMat * tmp2;
		up.x = tmp2.x;
		up.y = tmp2.y;
		up.z = tmp2.z;
	}

	{
		if (!((glm::normalize(direct).y > 0.99 && upRadius < 0) || (glm::normalize(direct).y < -0.99 && upRadius > 0))) {
			glm::mat4 transMat;
			glm::vec3 axis = glm::cross(up, direct);
			transMat = glm::rotate(transMat, (float)upRadius, axis);
			glm::vec4 tmp1(direct.x, direct.y, direct.z, 1.0);
			tmp1 = transMat * tmp1;
			direct.x = tmp1.x;
			direct.y = tmp1.y;
			direct.z = tmp1.z;
			glm::vec4 tmp2(up.x, up.y, up.z, 1.0);
			tmp2 = transMat * tmp2;
			up.x = tmp2.x;
			up.y = tmp2.y;
			up.z = tmp2.z;
		}
	}

	/* 设置指针到屏幕中间的位置 */
	glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);

	drtRadius = 0.0;
	upRadius = 0.0;
}

int main()
{
	/* GLFW & GLEW init */
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "[ERROR] GLFW init failed" << std::endl;
		system("pause");
		return -1;
	}

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	if (!window) {
		std::cout << "[ERROR] GLFW window create failed" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "[ERROR] GLEW init failed" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		system("pause");
		return -1;
	}

	/* create program */
	Program program("vertex.shader", "fragment.shader");
	program.useProgram();

	/* vertex info */
	Cube kaito_cube;

	Flat ground;
	
	/* texture */
	Texture texture1("container.jpg");
	Texture texture2("importIDL.dxl");
	Texture texture3("wall.jpg");
	glUniform1f(glGetUniformLocation(program.getProgramId(), "visible"), visible);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1.getTextureId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2.getTextureId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture3.getTextureId());

	glUniform1i(glGetUniformLocation(program.getProgramId(), "texture1"), 0);
	glUniform1i(glGetUniformLocation(program.getProgramId(), "texture2"), 1);
	glUniform1i(glGetUniformLocation(program.getProgramId(), "texture3"), 2);

	/* 3D */
	glm::mat4 model, view, projection;
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
	projection = glm::perspective(glm::radians(45.0f), (1.0f * windowWidth / windowHeight), 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(program.getProgramId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program.getProgramId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program.getProgramId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec3 cubePos[] = {
		glm::vec3(-1.0,  1.0, -1.0),
		glm::vec3( 0.0,  1.0, -1.0),
		glm::vec3( 1.0,  1.0, -1.0),
		glm::vec3(-1.0,  0.0, -1.0),
		glm::vec3( 0.0,  0.0, -1.0),
		glm::vec3( 1.0,  0.0, -1.0),
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3( 0.0, -1.0, -1.0),
		glm::vec3( 1.0, -1.0, -1.0),

		glm::vec3( 0.0,  0.0,  0.0),
		glm::vec3( 0.0, -1.0,  0.0)
	};

	glm::vec3 flatPos[] = {
		glm::vec3(-1.0, -0.25, -1.0),
		glm::vec3(-1.0, -0.25,  0.0),
		glm::vec3(-1.0, -0.25,  1.0),
		glm::vec3( 0.0, -0.25, -1.0),
		glm::vec3( 0.0, -0.25,  0.0),
		glm::vec3( 0.0, -0.25,  1.0),
		glm::vec3( 1.0, -0.25, -1.0),
		glm::vec3( 1.0, -0.25,  0.0),
		glm::vec3( 1.0, -0.25,  1.0)
	};

	/* camera */
	glfwSetCursorPosCallback(window, mouse_callback);


	glClearColor(0.8, 0.6, 0.6, 0.7);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		processKeyboardInput(window, program.getProgramId());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	yPos = 0.0;
		if (isJump) {
			yPos += jumpOffset[jumpIdx++] / 2.0;
			if (jumpIdx == jumpMaxIdx) {
				jumpIdx = 0;
				isJump = false;
			}
		}
		view = glm::lookAt(glm::vec3(xPos, yPos, zPos), glm::vec3(xPos + direct.x, yPos + direct.y, zPos + direct.z), up);

		/* 画立方体 */
		kaito_cube.bindVao(program.getProgramId());
		for (int i = 0; i < 11; i++)
			kaito_cube.drawCube(program.getProgramId(), view, cubePos[i]);

		/* 画底面 */
		ground.bindVao(program.getProgramId());
		for(int i = 0; i < 9; i++)
			ground.drawCube(program.getProgramId(), view, flatPos[i]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}