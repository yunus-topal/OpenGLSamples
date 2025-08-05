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
}
