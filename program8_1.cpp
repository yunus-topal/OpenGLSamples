#include "program8_1.h"

using namespace std;

void Program8_1::init(GLFWwindow* window) {
	renderingProgram1 = shader_utils::createShaderProgram("vertShader_8_1_0.glsl", "fragShader_8_1_0.glsl");
	renderingProgram2 = shader_utils::createShaderProgram("vertShader_8_1.glsl", "fragShader_8_1.glsl");
	emissiveProgram = shader_utils::createShaderProgram("vertShader_8_1_1.glsl", "fragShader_8_1_0.glsl");
	torusPos = glm::vec3(0.0f, 0.0f, 1.0f);
	spherePos = glm::vec3(0.0f, 3.0f, 1.0f);
	currentLightPos = glm::vec3(initialLightPos.x, initialLightPos.y, initialLightPos.z);

	setupTorusVertices();
	setupSphereVertices();
	setupShadowBuffers(window);
	b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void Program8_1::display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set up view and perspective matrix from the light point of view, for pass 1
	lightVmatrix = glm::lookAt(currentLightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // vector from light to origin
	lightPmatrix = glm::perspective(toRadians(60.0f), aspect, 0.1f, 1000.0f);
	// make the custom frame buffer current, and associate it with the shadow texture
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
	// disable drawing colors, but enable the depth computation
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	passOne();

	// restore the default display buffer, and re-enable drawing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDrawBuffer(GL_FRONT);
	// re-enables drawing colors
	passTwo();
}

void Program8_1::execute() {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 800, "Chapter8 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int newWidth, int newHeight) {
		auto* self = static_cast<Program8_1*>(glfwGetWindowUserPointer(w));
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

void Program8_1::setupTorusVertices(void) {
	std::vector<int> ind = torusModel.getIndices();
	std::vector<glm::vec3> vert = torusModel.getVertices();
	std::vector<glm::vec2> tex = torusModel.getTexCoords();
	std::vector<glm::vec3> norm = torusModel.getNormals();
	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	// vertex positions
	 // texture coordinates
	 // normal vectors
	int numVertices = torusModel.getNumVertices();
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// put the texture coordinates into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// put the normals into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]); // indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);

	glBindVertexArray(0); // leave EBO bound to VAO state
}

void Program8_1::setupSphereVertices() {
	std::vector<int> ind = sphereModel.getIndices();
	auto vert = sphereModel.getVertices();
	auto tex = sphereModel.getTexCoords();
	auto norm = sphereModel.getNormals();

	std::vector<float> pvalues, tvalues, nvalues;
	pvalues.reserve(vert.size() * 3); tvalues.reserve(tex.size() * 2); nvalues.reserve(norm.size() * 3);
	for (auto& v : vert) { pvalues.insert(pvalues.end(), { v.x,v.y,v.z }); }
	for (auto& t : tex) { tvalues.insert(tvalues.end(), { t.x,t.y }); }
	for (auto& n : norm) { nvalues.insert(nvalues.end(), { n.x,n.y,n.z }); }


	glGenVertexArrays(1, vao2);
	glBindVertexArray(vao2[0]);
	glGenBuffers(4, vbo2);
	// put the vertices into buffer #0
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// put the texture coordinates into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// put the normals into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo2[3]); // indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);

	glBindVertexArray(0); // leave EBO bound to VAO state
}

void Program8_1::window_reshape_callback(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
}

void Program8_1::handleInput(GLFWwindow* window, float dt) {
	float v = moveSpeed * dt;

	// WASD: left/right, forward/back (world axes)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += v * glm::vec3(0.0f, 0.0f, -1.0f); // forward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos += v * glm::vec3(0.0f, 0.0f, 1.0f);  // back
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos += v * glm::vec3(-1.0f, 0.0f, 0.0f); // left
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += v * glm::vec3(1.0f, 0.0f, 0.0f);  // right

	// Q/E: down/up
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraPos += v * glm::vec3(0.0f, -1.0f, 0.0f); // down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameraPos += v * glm::vec3(0.0f, 1.0f, 0.0f);  // up

	// Optional: ESC to quit
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

	// ====================
	// Light Controls (Arrow keys + PgUp/PgDn)
	// ====================
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    currentLightPos += v * glm::vec3(0.0f, 0.0f, -1.0f); // forward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  currentLightPos += v * glm::vec3(0.0f, 0.0f, 1.0f);  // back
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  currentLightPos += v * glm::vec3(-1.0f, 0.0f, 0.0f); // left
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) currentLightPos += v * glm::vec3(1.0f, 0.0f, 0.0f);  // right

	// PageUp/PageDown for light up/down
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)   currentLightPos += v * glm::vec3(0.0f, 1.0f, 0.0f); // up
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) currentLightPos += v * glm::vec3(0.0f, -1.0f, 0.0f); // down
}

void Program8_1::installLights(GLuint renderingProgram, Material material) {
	// save the light position in a float array
	lightPos[0] = currentLightPos.x;
	lightPos[1] = currentLightPos.y;
	lightPos[2] = currentLightPos.z;
	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");
	// set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mAmbLoc, 1, material.ambient);
	glProgramUniform4fv(renderingProgram, mDiffLoc, 1, material.diffuse);
	glProgramUniform4fv(renderingProgram, mSpecLoc, 1, material.specular);
	glProgramUniform1f(renderingProgram, mShiLoc, material.shininess);
}

void Program8_1::setupShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	screenSizeX = width;
	screenSizeY = height;
	// create the custom frame buffer
	glGenFramebuffers(1, &shadowBuffer);
	// create the shadow texture and configure it to hold depth information.
	// these steps are similar to those in Program 5.2
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		screenSizeX, screenSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

void Program8_1::passOne() {
	glUseProgram(renderingProgram1);

	// If you’re rendering into a shadow-map FBO, bind it here and set its viewport.
	// glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	// glViewport(0, 0, shadowWidth, shadowHeight);

	// Depth-only typical setup for a shadow pass:
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);  // no color writes
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); // flip if your light space changes winding

	glClear(GL_DEPTH_BUFFER_BIT);

	const GLint sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");

	// ---------- Torus from light’s POV ----------
	{
		glm::mat4 mMat = glm::translate(glm::mat4(1.0f), torusPos);
		// Rotate *onto* mMat (don’t multiply by rotate(mMat,...))
		mMat = glm::rotate(mMat, torusModel.toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindVertexArray(vao[0]); // torus VAO (set up in setupTorusVertices)
		glDrawElements(GL_TRIANGLES,
			(GLsizei)torusModel.getNumIndices(),
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	// ---------- Sphere from light’s POV ----------
	{
		glm::mat4 mMat = glm::translate(glm::mat4(1.0f), spherePos);
		glm::mat4 shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindVertexArray(vao2[0]); // sphere VAO (set up in setupSphereVertices)
		glDrawElements(GL_TRIANGLES,
			(GLsizei)sphereModel.getNumIndices(),
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	// Restore color writes for later passes if needed
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	// If you bound an FBO for the shadow map, unbind it here.
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Program8_1::passTwo() {
	glUseProgram(renderingProgram2);
	// pass two vertex and fragment shaders
	 // draw the torus – this time we need to include lighting, materials, normals, etc.
	 // We also need to provide MVP tranforms for BOTH camera space and light space.
	mLoc = glGetUniformLocation(renderingProgram2, "m_matrix");
	vLoc = glGetUniformLocation(renderingProgram2, "v_matrix");
	pLoc = glGetUniformLocation(renderingProgram2, "p_matrix");
	nLoc = glGetUniformLocation(renderingProgram2, "norm_matrix");
	sLoc = glGetUniformLocation(renderingProgram2, "shadowMVP2");

	// ---------- draw the torus ---------- //

	vMat = glm::translate(glm::mat4(1.0f), -cameraPos);
	installLights(renderingProgram2, goldMaterial);
	mMat = glm::translate(glm::mat4(1.0f), torusPos);
	// slight rotation for viewability 
	mMat = glm::rotate(mMat, toRadians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// build the inverse-transpose matrix for the normal vectors
	invTrMat = glm::transpose(glm::inverse(mMat));
	// build the MVP matrix for the torus from the light’s point of view
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
	// put the M, V and P matrices into the corresponding uniforms
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

	//glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLES,
		(GLsizei)torusModel.getNumIndices(),
		GL_UNSIGNED_INT,
		0);
	glBindVertexArray(0);

	// ---------- draw the sphere ---------- //
	installLights(renderingProgram2, bronzeMaterial);
	mMat = glm::translate(glm::mat4(1.0f), spherePos);
	// build the inverse-transpose matrix for the normal vectors
	invTrMat = glm::transpose(glm::inverse(mMat));
	// build the MVP matrix for the torus from the light’s point of view
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

	// view and projection matrices are the same as before, so only need to update the model matrix
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));


	// set up sphere vertices and normals buffers
	//glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(vao2[0]);
	glDrawElements(GL_TRIANGLES,
		(GLsizei)sphereModel.getNumIndices(),
		GL_UNSIGNED_INT,
		0);
	glBindVertexArray(0);

	// --------------------------- draw the light source as a white point --------------------------- //

	glUseProgram(emissiveProgram); // very simple VS/FS that outputs a constant color

	glm::mat4 mLight = glm::translate(glm::mat4(1.0f), currentLightPos)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)); // small radius

	glm::mat4 mvp = pMat * vMat * mLight;

	glUniformMatrix4fv(glGetUniformLocation(emissiveProgram, "uMVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform3f(glGetUniformLocation(emissiveProgram, "uColor"), 1.0f, 0.95f, 0.8f); // warm white
	glUniform1f(glGetUniformLocation(emissiveProgram, "uIntensity"), 1.0f);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);              // <- don’t write depth
	// (optional glow) glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(vao2[0]);         // your sphere VAO
	glDrawElements(GL_TRIANGLES, (GLsizei)sphereModel.getNumIndices(), GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);

}