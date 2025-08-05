#pragma once
#include <iostream>
#include <GL/glew.h>

namespace log_utils {
	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();
}

