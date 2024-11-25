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

GLfloat vertices[] =
{
	//         COORDINATES            /     COLORS           /    TEXTURE     //
	-0.5f,    -0.5f ,	  0.0f,			 1.0f, 0.0f,  0.0f,		0.0f, 0.0f,		    //Lower left corner
	-0.5f,     0.5f ,     0.0f,			 0.0f, 1.0f,  0.0f,		0.0f, 1.0f,			//Upper left corner
	 0.5f,     0.5f ,	  0.0f,			 0.0f, 0.0f,  1.0f,		1.0f, 1.0f,		    //Upper right corner
	 0.5f,	  -0.5f ,	  0.0f,			 1.0f, 1.0f,  1.0f,	    1.0f, 0.0f   		//Lower right corner
};

GLuint indices[]
{
	0, 2, 1, //Upper triangle
	0, 3, 2  //Lower triangle
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

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture rockWall("rock_wall.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	rockWall.texUnit(shaderProgram, "tex0", 0);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);       //Ustaw kolor buffora Back
		glClear(GL_COLOR_BUFFER_BIT);				   //Wyczyœæ bufor Back i wsadŸ do niego kolor z poprzedniego glClearColor
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		rockWall.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	rockWall.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}