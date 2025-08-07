#include "shader_utils.h"


using namespace std;

namespace shader_utils {
	GLuint createShaderProgram(const char* vp, const char* fp) {
		GLint vertCompiled;
		GLint fragCompiled;
		GLint linked;

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		string vertShaderStr = file_utils::readShaderSource(vp);
		string fragShaderStr = file_utils::readShaderSource(fp);

		const char* vertShaderSrc = vertShaderStr.c_str();
		const char* fragShaderSrc = fragShaderStr.c_str();

		glShaderSource(vShader, 1, &vertShaderSrc, NULL);
		glShaderSource(fShader, 1, &fragShaderSrc, NULL);

		glCompileShader(vShader);
		log_utils::checkOpenGLError();
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1) {
			std::cout << "vertex compilation failed" << std::endl;
			log_utils::printShaderLog(vShader);
		}

		glCompileShader(fShader);
		log_utils::checkOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1) {
			std::cout << "fragment compilation failed" << std::endl;
			log_utils::printShaderLog(fShader);
		}

		GLuint vfProgram = glCreateProgram();
		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);

		glLinkProgram(vfProgram);
		log_utils::checkOpenGLError();
		glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
		if (linked != 1) {
			std::cout << "linking failed" << std::endl;
			log_utils::printProgramLog(vfProgram);
		}

		return vfProgram;
	}
}
