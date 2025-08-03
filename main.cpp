#include <iostream>
#include "chapter2.h"
#include "transform_utils.h"

int main() {
	std::cout << "hello world!" << std::endl;

	//Program2_1 y = Program2_1();
	//y.execute();

	glm::mat4 translate_matrix = buildTranslate(1.0f, 2.0f, 3.0f);

	Program2_4 x = Program2_4();
	x.execute();
}

