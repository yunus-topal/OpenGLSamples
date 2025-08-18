#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

namespace file_utils {

	std::string readShaderSource(const char* filePath);
	GLuint loadTexture(const char* texImagePath);
}