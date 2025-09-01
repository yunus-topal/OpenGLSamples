
#include <iostream>
#include "Torus.h"
#include <glm/ext/matrix_transform.hpp>

using namespace std;
Torus::Torus() {
	precV = 48;
	precU = 48;
	inner = 0.5f;
	outer = 0.2f;
	init();
}
Torus::Torus(float innerRadius, float outerRadius, int prec) {
	precU = prec;   // around the tube
	precV = prec;
	inner = innerRadius;
	outer = outerRadius;
	init();
}

Torus::Torus(float innerRadius, float outerRadius, int spherePrec, int ringCount) {
	precU = spherePrec;   // around the tube
	precV = ringCount;   // number of rings
	inner = innerRadius;
	outer = outerRadius;
	init();
}

float Torus::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Torus::init() {
	numVertices = (precU + 1) * (precV + 1);
	numIndices = precU * precV * 6;
	vertices.resize(numVertices);
	texCoords.resize(numVertices);
	normals.resize(numVertices);
	sTangents.resize(numVertices);
	tTangents.resize(numVertices);
	indices.resize(numIndices);

	// Build first ring (v = 0): the tube cross-section
	for (int i = 0; i < precU + 1; i++) {
		float amt = toRadians(i * 360.0f / precU);
		// build the ring by rotating points around the origin, then moving them outward
		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 initPos(rMat * glm::vec4(0.0f, outer, 0.0f, 1.0f));
		vertices[i] = glm::vec3(initPos + glm::vec3(inner, 0.0f, 0.0f));
		// compute texture coordinates for each vertex on the ring
		texCoords[i] = glm::vec2(0.0f, ((float)i / (float)precU));
		// compute tangents and normals -- first tangent is Y-axis rotated around Z
		rMat = glm::rotate(glm::mat4(1.0f), amt + (3.14159f / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
		sTangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f)); // second tangent is -Z.
		normals[i] = glm::cross(tTangents[i], sTangents[i]); // their X-product is the normal.
	}
	// rotate the first ring about Y to get the other rings
	for (int ring = 1; ring < precV + 1; ring++) {
		// rotate the vertex positions of the original ring around the Y axis
		float amt = (float)(toRadians(ring * 360.0f / precV));
		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));

		for (int vert = 0; vert < precU + 1; vert++) {
			int dst = ring * (precU + 1) + vert;
			
			vertices[dst] = glm::vec3(rMat * glm::vec4(vertices[vert], 1.0f));
			sTangents[dst] = glm::vec3(rMat * glm::vec4(sTangents[vert], 1.0f));
			tTangents[dst] = glm::vec3(rMat * glm::vec4(tTangents[vert], 1.0f));
			normals[dst] = glm::vec3(rMat * glm::vec4(normals[vert], 1.0f));
			texCoords[dst] = glm::vec2(2.0f * (float)ring / precV, (float)vert / precU);

		}
	}
	// calculate triangle indices corresponding to the two triangles built per vertex
	int k = 0;
	for (int v = 0; v < precV; ++v) {
		for (int u = 0; u < precU; ++u) {
			int row0 = v * (precU + 1);
			int row1 = (v + 1) * (precU + 1);

			indices[k++] = row0 + u;
			indices[k++] = row1 + u;
			indices[k++] = row0 + u + 1;

			indices[k++] = row0 + u + 1;
			indices[k++] = row1 + u;
			indices[k++] = row1 + u + 1;
		}
	}
}
// accessors for the torus indices and vertices
int Torus::getNumVertices() { return numVertices; }
int Torus::getNumIndices() { return numIndices; }
std::vector<int> Torus::getIndices() { return indices; }
std::vector<glm::vec3> Torus::getVertices() { return vertices; }
std::vector<glm::vec2> Torus::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Torus::getNormals() { return normals; }
std::vector<glm::vec3> Torus::getStangents() { return sTangents; }
std::vector<glm::vec3> Torus::getTtangents() { return tTangents; }