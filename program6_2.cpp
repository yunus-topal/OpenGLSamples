#include "program6_2.h"
#include "Torus.h"

using namespace std;

void Program6_2::init(GLFWwindow* window) {
	brickTexture = file_utils::loadTexture("brick.jpg");

	renderingProgram = shader_utils::createShaderProgram("vertShader_5_1.glsl", "fragShader_5_1.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 10.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
	pyrLocX = 2.0f; pyrLocY = 0.0f; pyrLocZ = 1.0f;
	setupVertices();
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}
void Program6_2::display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glEnable(GL_CULL_FACE);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	pLoc = glGetUniformLocation(renderingProgram, "p_matrix");
	useTexLoc = glGetUniformLocation(renderingProgram, "useTex");

	// load perspective matrix
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform1i(useTexLoc, GL_TRUE);

	// build view matrix, model matrix, and model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);


	// ----------------------  pyramid == sun  -------------------------------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f)); // sun rotation

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	//// bind torus vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//// bind torus texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// bind torus normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// bind indices of torus
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);


	// setup textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);

	glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);
	mvStack.pop(); // pop the sun rotation matrix

	// set use tex false for cubes
	glUniform1i(useTexLoc, GL_FALSE);


	// sun position, and view matrices from stack
	mvStack.pop();  mvStack.pop();
}

void Program6_2::execute() {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 800, "Chapter6 - program2", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int newWidth, int newHeight) {
		auto* self = static_cast<Program6_2*>(glfwGetWindowUserPointer(w));
		self->window_reshape_callback(newWidth, newHeight);
		});

	init(window);
	lastFrameTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		double now = glfwGetTime();
		float dt = static_cast<float>(now - lastFrameTime);
		lastFrameTime = now;

		glfwPollEvents();
		handleInput(window, dt);
		display(window, now);
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Program6_2::setupVertices(void) {
	std::vector<int> ind = myTorus.getIndices();
	std::vector<glm::vec3> vert = myTorus.getVertices();
	std::vector<glm::vec2> tex = myTorus.getTexCoords();
	std::vector<glm::vec3> norm = myTorus.getNormals();
	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	// vertex positions
	 // texture coordinates
	 // normal vectors
	int numVertices = myTorus.getNumVertices();
	for (int i = 0; i < numVertices; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vbo);
	// put the vertices into buffer #0
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	// put the texture coordinates into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	// put the normals into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]); // indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

void Program6_2::window_reshape_callback(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
}

void Program6_2::handleInput(GLFWwindow* window, float dt) {
	float v = moveSpeed * dt;

	// WASD: left/right, forward/back (world axes)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraZ -= v; // forward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraZ += v; // back
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraX -= v; // left
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraX += v; // right

	// Q/E: down/up
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraY -= v; // down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameraY += v; // up

	// Optional: ESC to quit
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}
