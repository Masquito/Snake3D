#pragma once
#include<glad/glad.h>
#include<stb/stb_image.h>
#include"ShaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;
	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
