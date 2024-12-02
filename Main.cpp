#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "Variables.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "ShaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Camera.h"

GLfloat vertices[] =
{
	//      COORDINATES      /      COLORS           /    TEXTURE     /       NORMALS     //
	    -1.0f, 0.0f,  1.0f,     0.0f, 0.0f, 0.0f,       0.0f, 0.0f,      0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,       0.0f, 1.0f,      0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,       1.0f, 1.0f,      0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,     0.0f, 0.0f, 0.0f,       1.0f, 0.0f,      0.0f, 1.0f, 0.0f
};
GLfloat planeVertices[] =
{
	10.0f, 0.0f, 0.0f,
	10.0f, 0.0f, 20.0f,
   -10.0f, 0.0f, 20.0f,
   -10.0f, 0.0f, 0.0f
};
GLuint planeIndices[] =
{
	0, 1, 2,
	2, 0, 3
};
GLuint indices[]
{
	0, 1, 2,
	0, 2, 3
};
GLfloat lightVertices[] =
{
	-0.1f,  -0.1f,   0.1f,
	-0.1f,  -0.1f,  -0.1f,
	 0.1f,  -0.1f,  -0.1f,
	 0.1f,  -0.1f,   0.1f,
	-0.1f,   0.1f,   0.1f,
	-0.1f,   0.1f,  -0.1f,
	 0.1f,   0.1f,  -0.1f,
	 0.1f,   0.1f,   0.1f,
};
GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(Variables::windowWidth, Variables::windowHeight, "Snake3D Adam W", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, Variables::windowWidth, Variables::windowHeight);

	// PYRAMID //

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// LIGHTNING //

	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();


	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

	vec3 pyramidPos = vec3(0.0f, 0.0f, 0.0f);
	mat4 pyramidModel = mat4(1.0f);
	pyramidModel = translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// TEXTURE //

	Texture floorTex("floor.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	floorTex.texUnit(shaderProgram, "tex0", 0);
	Texture floor_ao("floor_ao.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	floor_ao.texUnit(shaderProgram, "tex1", 1);

	glEnable(GL_DEPTH_TEST);


	Camera camera(Variables::windowWidth, Variables::windowHeight, vec3(0.0f, 0.0f, 2.0f));
	float lastFrameTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);       //Ustaw kolor buffora Back
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				   //Wyczyœæ bufor Back i wsadŸ do niego kolor z poprzedniego glClearColor

		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");

		floorTex.Bind();
		floor_ao.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	floorTex.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}