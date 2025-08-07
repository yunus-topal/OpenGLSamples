#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "file_utils.h"
#include "log_utils.h"

namespace shader_utils {
	GLuint createShaderProgram(const char* vp, const char* fp);

	// TODO: implement custom overrides for geometry and tessellation shaders
	GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);
	GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
	GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp);
} 
