#include <GL/glew.h>
#include <iostream>
#include "log_utils.h"


namespace log_utils
{

	void printShaderLog(GLuint shader) {
		int len = 0;
		int chWrittn = 0;
		char* log;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0) {
			log = (char*)malloc(len);
			glGetShaderInfoLog(shader, len, &chWrittn, log);
			std::cout << "Shader Info Log: " << log << std::endl;
			free(log);
		}
	}
	void printProgramLog(int prog) {
		int len = 0;
		int chWrittn = 0;
		char* log;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		if (len > 0) {
			log = (char*)malloc(len);
			glGetProgramInfoLog(prog, len, &chWrittn, log);
			std::cout << "Program Info Log: " << log << std::endl;
			free(log);
		}
	}
	bool checkOpenGLError() {
		bool foundError = false;
		int glErr = glGetError();
		while (glErr != GL_NO_ERROR) {
			std::cout << "glError: " << glErr << std::endl;
			foundError = true;
			glErr = glGetError();
		}
		return foundError;
	}
}
