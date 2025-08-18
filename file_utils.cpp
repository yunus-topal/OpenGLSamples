#include "file_utils.h"


namespace file_utils
{
	std::string readShaderSource(const char* filePath) {
		std::string content;
		std::ifstream fileStream(filePath, std::ios::in);
		std::string line = "";
		while (!fileStream.eof()) {
			std::getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}

	GLuint loadTexture(const char* texImagePath) {
		GLuint textureID;
		textureID = SOIL_load_OGL_texture(texImagePath,
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (textureID == 0)	std::cout << "could	not	find	texture	file" << texImagePath << std::endl;
		return textureID;
	}
}
