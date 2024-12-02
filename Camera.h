#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "ShaderClass.h"
using namespace glm;

class Camera
{
public:
	vec3 Position;
	vec3 Orientation = vec3(0.0f, 0.0f, -1.0f);
	vec3 Up = vec3(0.0f, 1.0f, 0.0f);
	mat4 cameraMatrix = mat4(1.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 0.4f;
	float sensitivity = 100.0f;

	//       Methods      //

	Camera(int width, int height, vec3 position);

	void Matrix(Shader& shader, const char* uniform);
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Inputs(GLFWwindow* window, float deltaTime);
};